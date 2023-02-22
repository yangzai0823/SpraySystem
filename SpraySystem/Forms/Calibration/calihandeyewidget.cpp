#include "calihandeyewidget.h"

#include <calibration.h>
#include <qimage.h>
#include <rapidjson/allocators.h>
#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>

#include <Eigen/Core>
#include <QDir>
#include <QFile>
#include <QString>
#include <QtConcurrent/QtConcurrent>

#include "Device/Camera/cameraoperator.h"
#include "Device/MotionController/mcoperator.h"
#include "Device/Robot/robotoperator.h"
#include "ui_calihandeyewidget.h"

extern QString CALIBRATE_DATA_FILE;
extern QString CALIBRATE_RESULT_FILE;
extern QString DATA_FOLDER_NAME;

template <typename T>
T* readRawBinaryPixelData(const std::string& path, size_t& w, size_t& h,
                          size_t& channel) {
  std::ifstream infile(path, std::ios::in | std::ios::binary);
  if (!infile) {
    return nullptr;
  }
  infile.read((char*)&w, sizeof(size_t));
  infile.read((char*)&h, sizeof(size_t));
  infile.read((char*)&channel, sizeof(size_t));
  size_t len = w * h * channel;
  auto data = new T[len];
  infile.read((char*)data, len * sizeof(T));
  infile.close();
  return data;
}
template <typename T>
void writeRawBinaryPixelData(const std::string& path, size_t w, size_t h,
                             size_t channel, T* data) {
  std::ofstream outfile(path,
                        std::ios::out | std::ios::binary | std::ios::trunc);
  if (!outfile) {
    return;
  }
  outfile.write((char*)&w, sizeof(size_t));
  outfile.write((char*)&h, sizeof(size_t));
  outfile.write((char*)&channel, sizeof(size_t));
  outfile.write((char*)data, w * h * channel * sizeof(T));
  outfile.close();
}

caliHandEyewWidget::caliHandEyewWidget(const std::string& prefix,
                                       QWidget* parent)
    : baseCaliWidget(parent),
      ui(new Ui::caliHandEyewWidget),
      _prefix(prefix),
      _frontExtraAxisDirection(nullptr),
      _robotBeltDirection(nullptr),
      _thd_updateImageView(nullptr),
      _timer_updateImageView(nullptr) {
  ui->setupUi(this);
  enableButton(0);
  //
  _dataMainKey = _prefix + "handEyeCaliDatas";
  _resultMainKey = _prefix + "handEyeCalibration";
  // update image
  _thd_updateImageView = new QThread();
  _timer_updateImageView = new QTimer();
  _timer_updateImageView->setInterval(500);
  _timer_updateImageView->moveToThread(_thd_updateImageView);
  connect(_timer_updateImageView, SIGNAL(timeout()), this,
          SLOT(on_UpdateImage()), Qt::DirectConnection);
  connect(_thd_updateImageView, SIGNAL(started()), _timer_updateImageView,
          SLOT(start()));

  _thd_updateImageView->start();
  // other
  QtConcurrent::run([this]() {
    readData();
    readResult();
    readCalibratedDatas();
    updateTree();
    connectDevice();
  });
}

caliHandEyewWidget::~caliHandEyewWidget() {
  _thd_updateImageView->exit();
  delete ui;
}

void caliHandEyewWidget::setDevice(RobotOperator* robot,
                                   MCOperator* motionController,
                                   CameraOperator* camera) {
  _robot = robot;
  _motionController = motionController;
  _camera = camera;
}

// int caliHandEyewWidget::ensureJsonStruct() {
//   //// data json
//   if (_dataDoc->IsNull()) {
//     _dataDoc->SetObject();
//   }
//   if (!_dataDoc->IsObject()) {
//     std::cout << "data json format error" << std::endl;
//     return -1;
//   }
//   // main
//   auto& allocatorData = _dataDoc->GetAllocator();
//   std::string dataMainKey = _dataMainKey.toStdString();
//   if (!_dataDoc->GetObject().HasMember(dataMainKey.c_str())) {
//     rapidjson::Value obj(rapidjson::kObjectType);
//     _dataDoc->AddMember(rapidjson::Value(dataMainKey.c_str(), allocatorData),
//                         obj, allocatorData);
//   }
//   // data
//   auto& main = (*_dataDoc)[dataMainKey.c_str()];
//   if (!main.HasMember("datas")) {
//     main.AddMember(rapidjson::Value("datas", allocatorData),
//                    rapidjson::Value(rapidjson::kArrayType), allocatorData);
//   }
//   //// result json
//   if (_resultDoc->IsNull()) {
//     _resultDoc->SetObject();
//   }
//   if (!_resultDoc->IsObject()) {
//     std::cout << "data json format error" << std::endl;
//     return -1;
//   }
//   // main
//   auto& allocatorResult = _dataDoc->GetAllocator();
//   std::string resultMainKey = _resultMainKey.toStdString();
//   if (!_resultDoc->GetObject().HasMember(resultMainKey.c_str())) {
//     _resultDoc->AddMember(
//         rapidjson::Value(resultMainKey.c_str(), allocatorResult),
//         rapidjson::Value(rapidjson::kObjectType), allocatorResult);
//   }
//   // direction
//   auto& resultMain = (*_resultDoc)[resultMainKey.c_str()];
//   if (!resultMain.HasMember("direction")) {
//     resultMain.AddMember(rapidjson::Value("direction", allocatorResult),
//                          rapidjson::Value(rapidjson::kArrayType),
//                          allocatorResult);
//   }
// }

void caliHandEyewWidget::readCalibratedDatas() {
  if (_resultDoc->IsNull()) {
    return;
  }
  {
    std::vector<float> arr;
    if (0 >
        _resultDoc->getArray({"frontExtraAxisCalibration", "direction"}, arr)) {
      std::cout << "frontExtraAxisDirection doesn't exist" << std::endl;
      return;
    } else {
      if (arr.size() != 3) {
        std::cout << "frontExtraAxisDirection length must equal 3" << std::endl;
        return;
      }
      _frontExtraAxisDirection = std::make_unique<Eigen::Vector3f>();
      (*_frontExtraAxisDirection)[0] = arr[0];
      (*_frontExtraAxisDirection)[1] = arr[1];
      (*_frontExtraAxisDirection)[2] = arr[2];
    }
  }
  {
    std::vector<float> arr;
    if (0 >
        _resultDoc->getArray({"beltDirectionCalibration", "direction"}, arr)) {
      std::cout << "beltDirection doesn't exist" << std::endl;
      return;
    } else {
      if (arr.size() != 3) {
        std::cout << "beltDirection length must equal 3" << std::endl;
        return;
      }
      _robotBeltDirection = std::make_unique<Eigen::Vector3f>();
      (*_robotBeltDirection)[0] = arr[0];
      (*_robotBeltDirection)[1] = arr[1];
      (*_robotBeltDirection)[2] = arr[2];
    }
  }
}

void caliHandEyewWidget::clearResult() {
  auto token = jsonParser::toToken({_resultMainKey, "handEyeMatrix"});
  auto v = rapidjson::Pointer(token).Get(*_resultDoc);
  if (v) {
    if (v->IsArray()) {
      v->Clear();
    } else {
      v->SetArray();
    }
  }
}

int caliHandEyewWidget::readCameraData(std::string& rgbPath,
                                       std::string& xyzPath,
                                       float& beltPosition) {
  // TODO change to cameraoperator
  VWSCamera::ImageData data;
  try {
    {
      std::lock_guard<std::mutex> guard(_lock);
      if (0 > __camera->FetchFrame(1000, data)) {
        return -1;
      };
    }
  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
    return -1;
  }
  std::string name =
      QDir::current().absolutePath().toStdString() + "/" + _dataFolderName +
      "/" +
      std::to_string(
          std::chrono::system_clock::now().time_since_epoch().count());
  auto rgbPath_ = name + ".rgb";
  auto xyzPath_ = name + ".xyz";
  writeRawBinaryPixelData<uchar>(rgbPath_, data.RGB8PlanarImage.nWidth,
                                 data.RGB8PlanarImage.nHeight, 3u,
                                 (uchar*)data.RGB8PlanarImage.pData);
  writeRawBinaryPixelData<float>(xyzPath_, data.PointCloudImage.nWidth,
                                 data.PointCloudImage.nHeight, 3u,
                                 (float*)data.PointCloudImage.pData);
  delete data.RGB8PlanarImage.pData;
  delete data.PointCloudImage.pData;
  // camera belt position
  float beltPos_(0);
  try {
    bool success;
    beltPos_ = _motionController->getRealTimeEncoder(success)[1];
  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
    return -1;
  }
  rgbPath = rgbPath_;
  xyzPath = xyzPath_;
  beltPosition = beltPos_;
  return 0;
}

void caliHandEyewWidget::recordCameraData(const std::string& rgbPath,
                                          const std::string& xyzPath,
                                          float cameraBeltPos) {
  auto& allocator = _dataDoc->GetAllocator();
  rapidjson::Value obj(rapidjson::kObjectType);
  {
    obj.AddMember("image", rapidjson::Value(rgbPath, allocator), allocator);
    obj.AddMember("pointcloud", rapidjson::Value(xyzPath, allocator),
                  allocator);
    obj.AddMember<float>("cameraBeltPos", cameraBeltPos, allocator);
    obj.AddMember<float>("robotBeltPos", 0, allocator);
    obj.AddMember("extraAxisPos", rapidjson::Value(rapidjson::kArrayType),
                  allocator);
    obj.AddMember("robotPos", rapidjson::Value(rapidjson::kArrayType),
                  allocator);
  }

  auto dataToken = jsonParser::toToken({_dataMainKey, "datas"});
  auto v = rapidjson::Pointer(dataToken).Get(*_dataDoc);
  if (v) {
    if (!v->IsArray()) {
      v->SetArray();
    }
    v->PushBack(obj, allocator);
  } else {
    rapidjson::Pointer(dataToken)
        .Set(*_dataDoc, rapidjson::Value(rapidjson::kArrayType))
        .PushBack(obj, allocator);
  }
}

int caliHandEyewWidget::readStationData(float& robotBeltPos,
                                        float& extraAxisPos,
                                        Eigen::Vector3f& robotPos) {
  float beltPos_(0), extraAxisPos_(0);
  float robotPos_[3];
  try {
    bool success;
    auto pos__ = _motionController->getRealTimeEncoder(success);
    extraAxisPos_ = pos__[0];
    beltPos_ = pos__[1];

    VWSRobot::RobotPosition pose;
    if (1 != _robot->getRobotPosition(pose)) {
      return -1;
    };
    robotPos_[0] = pose.pos[0];
    robotPos_[1] = pose.pos[1];
    robotPos_[2] = pose.pos[2];
  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
    return -1;
  }
  robotBeltPos = beltPos_;
  extraAxisPos = extraAxisPos_;
  robotPos[0] = robotPos_[0];
  robotPos[1] = robotPos_[1];
  robotPos[2] = robotPos_[2];
  return 0;
}
void caliHandEyewWidget::recordStationData(const float& robotBeltPos,
                                           const float& extraAxisPos,
                                           const Eigen::Vector3f& robotPos) {
  auto dataToken = jsonParser::toToken({_dataMainKey, "datas"});
  auto v = rapidjson::Pointer(dataToken).Get(*_dataDoc);
  assert(v != nullptr);
  assert(v->IsArray());

  auto len = v->Size();
  assert(len > 0);
  if (!v || !v->IsArray() || v->Size() == 0) {
    return;
  }

  auto& allocator = _dataDoc->GetAllocator();
  (*v)[len - 1]["robotBeltPos"].SetFloat(robotBeltPos);
  (*v)[len - 1]["extraAxisPos"].PushBack<float>(extraAxisPos, allocator);

  {
    rapidjson::Value arr(rapidjson::kArrayType);
    arr.PushBack<float>(robotPos[0], allocator);
    arr.PushBack<float>(robotPos[1], allocator);
    arr.PushBack<float>(robotPos[2], allocator);
    (*v)[len - 1]["robotPos"].PushBack(arr, allocator);
  }
}
void caliHandEyewWidget::recordRobotData(const float& extraAxisPos,
                                         const Eigen::Vector3f& robotPos) {
  auto dataToken = jsonParser::toToken({_dataMainKey, "datas"});
  auto v = rapidjson::Pointer(dataToken).Get(*_dataDoc);
  assert(v != nullptr);
  assert(v->IsArray());

  auto len = v->Size();
  assert(len > 0);

  auto& allocator = _dataDoc->GetAllocator();
  (*v)[len - 1]["extraAxisPos"].PushBack<float>(extraAxisPos, allocator);

  {
    rapidjson::Value arr(rapidjson::kArrayType);
    arr.PushBack<float>(robotPos[0], allocator);
    arr.PushBack<float>(robotPos[1], allocator);
    arr.PushBack<float>(robotPos[2], allocator);
    (*v)[len - 1]["robotPos"].PushBack(arr, allocator);
  }
}

void caliHandEyewWidget::deleteLastItem() {
  auto token = jsonParser::toToken({_dataMainKey, "datas"});
  auto v = rapidjson::Pointer(token).Get(*_dataDoc);
  if (v && v->IsArray() && !v->Empty()) {
    v->PopBack();
  }
}

void caliHandEyewWidget::parseGridInfo(size_t& w, size_t& h, float& size) {
  w = ui->spinBox_gridWidth->text().toUInt();
  h = ui->spinBox_gridHeight->text().toUInt();
  size = ui->doubleSpinBox_gridSize->text().toFloat();
}

int caliHandEyewWidget::calculate() {
  // _API_ int getHandEyeMatrix(
  // 	const std::vector<handEyeCaliData>& datas,
  // 	size_t plateWidth,
  // 	size_t plateHeight,
  // 	float gridSize,
  // 	const Eigen::Vector3f& beltDir,
  // 	Eigen::Isometry3f& matrix);

  // obj.AddMember("image", rapidjson::Value(rgbPath, allocator), allocator);
  // obj.AddMember("pointcloud", rapidjson::Value(xyzPath, allocator),
  //               allocator);
  // obj.AddMember<float>("cameraBeltPos", cameraBeltPos, allocator);
  // obj.AddMember<float>("robotBeltPos", 0, allocator);
  // obj.AddMember("extraAxisPos", rapidjson::Value(rapidjson::kArrayType),
  //               allocator);
  // obj.AddMember("robotPos", rapidjson::Value(rapidjson::kArrayType),
  //               allocator);

  if (_frontExtraAxisDirection == nullptr) {
    std::cout << "frontExtraAxisDirection not found" << std::endl;
    return -1;
  }
  if (_robotBeltDirection == nullptr) {
    std::cout << "robotBeltDirection not found" << std::endl;
    return -1;
  }
  auto dataToken = jsonParser::toToken({_dataMainKey, "datas"});
  auto dataValue = rapidjson::Pointer(dataToken).Get(*_dataDoc);
  if (!dataValue || !dataValue->IsArray() || !dataValue->Size() == 0) {
    return -1;
  }

  size_t gridWidth(0);
  size_t gridHeight(0);
  float gridSize(0);
  parseGridInfo(gridWidth, gridHeight, gridSize);

  std::vector<handEyeCaliData> caliDatas;
  for (size_t i = 0; i < dataValue->Size(); i++) {
    handEyeCaliData caliData;
    auto& data = (*dataValue)[i];
    // rgb
    auto rgbPath = data["image"].GetString();
    {
      size_t w(0), h(0), c(0);
      auto buff = readRawBinaryPixelData<uchar>(rgbPath, w, h, c);
      if (buff == nullptr) {
        std::cout << "read rgb file error : \t" << rgbPath << std::endl;
        continue;
      }
      caliData.rgb = buff;
      caliData.width = w;
      caliData.height = h;
    }
    // xyz
    auto xyzPath = data["pointcloud"].GetString();
    {
      size_t w(0), h(0), c(0);
      auto buff = readRawBinaryPixelData<float>(xyzPath, w, h, c);
      if (buff == nullptr) {
        std::cout << "read xyz file error : \t" << rgbPath << std::endl;
        continue;
      }
      caliData.xyz = buff;
    }
    // cameraBeltPos
    auto cameraBeltPos = data["cameraBeltPos"].GetFloat();
    caliData.pos1 = cameraBeltPos;
    // robotBeltPos
    auto robotBeltPos = data["robotBeltPos"].GetFloat();
    caliData.pos2 = robotBeltPos;
    // stationPos
    {
      auto extraAxisArr = data["extraAxisPos"].GetArray();
      auto robotPos = data["robotPos"].GetArray();
      assert(extraAxisArr.Size() == 3 && robotPos.Size() == 3);
      if (extraAxisArr.Size() != 3 || robotPos.Size() != 3) {
        continue;
      }
      caliData.p1 =
          Eigen::Vector3f(robotPos[0][0].GetFloat(), robotPos[0][1].GetFloat(),
                          robotPos[0][2].GetFloat()) +
          extraAxisArr[0].GetFloat() * (*_frontExtraAxisDirection);
      caliData.p2 =
          Eigen::Vector3f(robotPos[1][0].GetFloat(), robotPos[1][1].GetFloat(),
                          robotPos[1][2].GetFloat()) +
          extraAxisArr[1].GetFloat() * (*_frontExtraAxisDirection);
      caliData.p3 =
          Eigen::Vector3f(robotPos[2][0].GetFloat(), robotPos[2][1].GetFloat(),
                          robotPos[2][2].GetFloat()) +
          extraAxisArr[2].GetFloat() * (*_frontExtraAxisDirection);
    }
    caliDatas.push_back(caliData);
  }
  assert(caliDatas.size() > 0);
  if (caliDatas.size() == 0) {
    return -1;
  }
  // calculate
  Eigen::Isometry3f handEyeMatrix;
  try {
    if (0 > getHandEyeMatrix(caliDatas, gridWidth, gridHeight, gridSize,
                             *_robotBeltDirection, handEyeMatrix)) {
      return -1;
    };
  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
    return -1;
  }
  // set json
  std::vector<float> matrix__;
  for (size_t i = 0; i < 3; i++) {
    for (size_t j = 0; j < 4; j++) {
      matrix__.push_back(handEyeMatrix(i, j));
    }
  }

  _resultDoc->setArray<float>({_resultMainKey, "handEyeMatrix"}, matrix__);
}

void caliHandEyewWidget::updateTree() {
  jsonParser doc__;
  auto& allocator = doc__.GetAllocator();
  doc__.SetObject();
  // set calibrated results
  if (_frontExtraAxisDirection == nullptr) {
    doc__.setArray({"preCalibrated", "frontExtraAxisDirection"});
  } else {
    doc__.setArray<float>(
        {"preCalibrated", "frontExtraAxisDirection"},
        {(*_frontExtraAxisDirection)[0], (*_frontExtraAxisDirection)[1],
         (*_frontExtraAxisDirection)[2]});
  }
  if (_robotBeltDirection == nullptr) {
    doc__.setArray({"preCalibrated", "robotBeltDirection"});
  } else {
    doc__.setArray<float>({"preCalibrated", "robotBeltDirection"},
                          {(*_robotBeltDirection)[0], (*_robotBeltDirection)[1],
                           (*_robotBeltDirection)[2]});
  }
  // copy main domain
  auto dataMain = rapidjson::Pointer("/" + _dataMainKey).Get(*_dataDoc);
  if (dataMain) {
    for (auto it = dataMain->MemberBegin(); it != dataMain->MemberEnd(); it++) {
      // copy
      rapidjson::Value key, value;
      key.CopyFrom(it->name, allocator);
      value.CopyFrom(it->value, allocator);
      doc__.AddMember(key, value, allocator);
    }
  }
  // copy result
  auto resultMain = rapidjson::Pointer("/" + _resultMainKey).Get(*_resultDoc);
  doc__.AddMember("results", rapidjson::Value(rapidjson::kObjectType),
                  allocator);
  if (resultMain) {
    for (auto it = resultMain->MemberBegin(); it != resultMain->MemberEnd();
         it++) {
      // copy
      rapidjson::Value key, value;
      key.CopyFrom(it->name, allocator);
      value.CopyFrom(it->value, allocator);
      doc__["results"].AddMember(key, value, allocator);
    }
  }

  // to qbytearray
  rapidjson::StringBuffer sb;
  rapidjson::PrettyWriter<rapidjson::StringBuffer> w(sb);
  doc__.Accept(w);
  QByteArray arr(sb.GetString(), sb.GetSize());
  emit updateTreeView(arr);
}

void caliHandEyewWidget::on_btn_capture_clicked() {
  QtConcurrent::run([this]() {
    clearResult();
    writeResult();
    // FIXME
#if 1
    std::string rgb, xyz;
    float beltPos(0);
    if (0 != readCameraData(rgb, xyz, beltPos)) {
      std::cout << "read camera data error" << std::endl;
      return;
    }
    std::cout << "beltPos" << beltPos << std::endl;
    recordCameraData(rgb, xyz, beltPos);
#else
    recordCameraData("camera", "pcd", 1);
#endif
    writeData();
    // update  tree
    enableButton(1);
    updateTree();
  });
}

void caliHandEyewWidget::on_btn_record1_clicked() {
  QtConcurrent::run([this]() {
    clearResult();
    writeResult();
    // FIXME
#if 1
    std::string rgb, xyz;
    float beltPos(0), extraAxis(0);
    Eigen::Vector3f robotPos(0, 0, 0);
    if (0 != readStationData(beltPos, extraAxis, robotPos)) {
      std::cout << "read robot data error" << std::endl;
      return;
    }
    recordStationData(beltPos, extraAxis, robotPos);
#else
    recordStationData(0, 1, Eigen::Vector3f(1, 0, 0));
#endif
    writeData();
    // update  tree
    enableButton(2);
    updateTree();
  });
}

void caliHandEyewWidget::on_btn_record2_clicked() {
  QtConcurrent::run([this]() {
    clearResult();
    writeResult();
    // FIXME
#if 1

    std::string rgb, xyz;
    float beltPos(0), extraAxis(0);
    Eigen::Vector3f robotPos(0, 0, 0);
    if (0 != readStationData(beltPos, extraAxis, robotPos)) {
      std::cout << "read robot data error" << std::endl;
      return;
    }
    recordRobotData(extraAxis, robotPos);
#else
    recordStationData(0, 1, Eigen::Vector3f(1, 0, 0));
#endif
    writeData();
    // update  tree
    enableButton(3);
    updateTree();
  });
}

void caliHandEyewWidget::on_btn_record3_clicked() {
  QtConcurrent::run([this]() {
    clearResult();
    writeResult();
    // FIXME
#if 1

    std::string rgb, xyz;
    float beltPos(0), extraAxis(0);
    Eigen::Vector3f robotPos(0, 0, 0);
    if (0 != readStationData(beltPos, extraAxis, robotPos)) {
      std::cout << "read robot data error" << std::endl;
      return;
    }
    recordRobotData(extraAxis, robotPos);
#else
    recordStationData(0, 1, Eigen::Vector3f(1, 0, 0));
#endif
    writeData();
    // update  tree
    enableButton(0);
    updateTree();
  });
}
void caliHandEyewWidget::on_btn_calculate_clicked() {
  QtConcurrent::run([this]() {
    clearResult();
    writeResult();
    calculate();
    writeResult();
    updateTree();
  });
}

void caliHandEyewWidget::on_btn_delete_clicked() {
  QtConcurrent::run([this]() {
    clearResult();
    writeResult();
    deleteLastItem();
    writeData();
    updateTree();
    enableButton(0);
  });
}

void caliHandEyewWidget::enableButton(size_t index) {
  switch (index) {
    case 0:
      ui->btn_capture->setEnabled(true);
      ui->btn_record1->setEnabled(false);
      ui->btn_record2->setEnabled(false);
      ui->btn_record3->setEnabled(false);
      break;
    case 1:
      ui->btn_capture->setEnabled(false);
      ui->btn_record1->setEnabled(true);
      ui->btn_record2->setEnabled(false);
      ui->btn_record3->setEnabled(false);
      break;
    case 2:
      ui->btn_capture->setEnabled(false);
      ui->btn_record1->setEnabled(false);
      ui->btn_record2->setEnabled(true);
      ui->btn_record3->setEnabled(false);
      break;
    case 3:
      ui->btn_capture->setEnabled(false);
      ui->btn_record1->setEnabled(false);
      ui->btn_record2->setEnabled(false);
      ui->btn_record3->setEnabled(true);
      break;
    default:
      ui->btn_capture->setEnabled(true);
      ui->btn_record1->setEnabled(false);
      ui->btn_record2->setEnabled(false);
      ui->btn_record3->setEnabled(false);
      break;
  }
}

void caliHandEyewWidget::connectDevice() {
  __camera = new VWSCamera();
  __camera->Init("192.168.125.99", 1);
  __camera->connect();
  __camera->changeTriggerMode(false);
  __camera->startGrab();
}

void caliHandEyewWidget::on_UpdateImage() {
  // FIXME
  if (!__camera) {
    return;
  }
  VWSCamera::ImageData data;
  {
    std::lock_guard<std::mutex> guard(_lock);
    if (0 > __camera->FetchFrame(1, data)) {
      return;
    }
  }

  size_t gridWidth(0), gridHeight(0);
  float gridSize(0);
  parseGridInfo(gridWidth, gridHeight, gridSize);
  // find grid
  uchar* buff = NULL;
  size_t w = data.RGB8PlanarImage.nWidth;
  size_t h = data.RGB8PlanarImage.nHeight;

  // show image
  void* dst = 0 != getMarkedGridPlateImage(w, h, data.RGB8PlanarImage.pData,
                                           gridWidth, gridHeight, (void**)&buff)
                  ? data.RGB8PlanarImage.pData
                  : buff;

  QImage img(w, h, QImage::Format::Format_RGB32);
  // cpy
  {
    uchar* src = (uchar*)dst;
    for (size_t i = 0; i < h; i++) {
      auto row = img.scanLine(i);
      for (size_t j = 0; j < w; j++) {
        // TODO 效率
        img.setPixel(j, i, qRgb(src[0], src[1], src[2]));
        src += 3;
        row += 4;
      }
    }
  }
  delete data.RGB8PlanarImage.pData;
  delete data.PointCloudImage.pData;
  if (buff) {
    delete buff;
  }
  auto pixmap = QPixmap::fromImage(img);
  emit updateImage(pixmap);
}