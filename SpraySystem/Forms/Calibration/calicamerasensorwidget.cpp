#include "calicamerasensorwidget.h"

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
#include "Device/PLC/plcoperator.h"
#include "ui_calicamerasensorwidget.h"

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

caliCameraSensorWidget::caliCameraSensorWidget(const QString& prefix,
                                               QWidget* parent)
    : QWidget(parent),
      ui(new Ui::caliCameraSensorWidget),
      _prefix(prefix),
      _dataMainKey(prefix + "CameraSensorCaliDatas"),
      _resultMainKey(prefix + "CameraSensorCalibration"),
      _dataDoc(new jsonParser()),
      _resultDoc(new jsonParser()),
      _frontExtraAxisDirection(nullptr),
      _robotBeltDirection(nullptr),
      _thd_updateImageView(nullptr),
      _timer_updateImageView(nullptr) {
  ui->setupUi(this);
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
    ensureFileExist();
    readData();
    readResult();
    readCalibratedDatas();
    ensureJsonStruct();
    clearResult();
    updateTreeView();
    connectDevice();
  });
}

caliCameraSensorWidget::~caliCameraSensorWidget() {
  _thd_updateImageView->exit();
  delete ui;
}

void caliCameraSensorWidget::setDevice(PLCOperator* plc,
                                       CameraOperator* camera) {
  _plc = plc;
  _camera = camera;
}

void caliCameraSensorWidget::ensureFileExist() {
  auto folderPath = QDir::currentPath() + "/" + DATA_FOLDER_NAME;
  QDir dir;
  if (!dir.exists(folderPath)) {
    dir.mkpath(folderPath);
  }
  auto dataFilePath = folderPath + "/" + CALIBRATE_DATA_FILE;
  {
    QFile file(dataFilePath);
    if (!file.exists()) {
      file.open(QIODevice::WriteOnly);
      file.close();
    }
    _dataFilePath = dataFilePath;
  }
  auto resFilePath = folderPath + "/" + CALIBRATE_RESULT_FILE;
  {
    QFile file(resFilePath);
    if (!file.exists()) {
      file.open(QIODevice::WriteOnly);
      file.close();
    }
    _resFilePath = resFilePath;
  }
}

int caliCameraSensorWidget::ensureJsonStruct() {
  //// data json
  if (_dataDoc->IsNull()) {
    _dataDoc->SetObject();
  }
  if (!_dataDoc->IsObject()) {
    std::cout << "data json format error" << std::endl;
    return -1;
  }
  // main
  auto& allocatorData = _dataDoc->GetAllocator();
  std::string dataMainKey = _dataMainKey.toStdString();
  if (!_dataDoc->GetObject().HasMember(dataMainKey.c_str())) {
    rapidjson::Value obj(rapidjson::kObjectType);
    _dataDoc->AddMember(rapidjson::Value(dataMainKey.c_str(), allocatorData),
                        obj, allocatorData);
  }
  // data
  auto& main = (*_dataDoc)[dataMainKey.c_str()];
  if (!main.HasMember("datas")) {
    main.AddMember(rapidjson::Value("datas", allocatorData),
                   rapidjson::Value(rapidjson::kArrayType), allocatorData);
  }
  //// result json
  if (_resultDoc->IsNull()) {
    _resultDoc->SetObject();
  }
  if (!_resultDoc->IsObject()) {
    std::cout << "data json format error" << std::endl;
    return -1;
  }
  // main
  auto& allocatorResult = _dataDoc->GetAllocator();
  std::string resultMainKey = _resultMainKey.toStdString();
  if (!_resultDoc->GetObject().HasMember(resultMainKey.c_str())) {
    _resultDoc->AddMember(
        rapidjson::Value(resultMainKey.c_str(), allocatorResult),
        rapidjson::Value(rapidjson::kObjectType), allocatorResult);
  }
  // direction
  auto& resultMain = (*_resultDoc)[resultMainKey.c_str()];
  if (!resultMain.HasMember("direction")) {
    resultMain.AddMember(rapidjson::Value("direction", allocatorResult),
                         rapidjson::Value(rapidjson::kArrayType),
                         allocatorResult);
  }
}

void caliCameraSensorWidget::readCalibratedDatas() {
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

void caliCameraSensorWidget::readData() {
  _dataDoc->read(_dataFilePath.toStdString());
}

void caliCameraSensorWidget::writeData() {
  _dataDoc->write(_dataFilePath.toStdString());
}

void caliCameraSensorWidget::readResult() {
  _resultDoc->read(_resFilePath.toStdString());
}

void caliCameraSensorWidget::writeResult() {
  _resultDoc->write(_resFilePath.toStdString());
}

void caliCameraSensorWidget::clearResult() {
  std::string resultMainKey = _resultMainKey.toStdString();
  (*_resultDoc)[resultMainKey.c_str()]["direction"].Clear();
  if (auto v =
          rapidjson::Pointer(jsonParser::toToken({resultMainKey, "direction"}))
              .Get(*_resultDoc)) {
    v->Clear();
  }
}

int caliCameraSensorWidget::readData(std::string& rgbPath, std::string& xyzPath,
                                     float& dist) {
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
      QDir::current().absolutePath().toStdString() + "/" +
      DATA_FOLDER_NAME.toStdString() + "/" +
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
  float dist__(0);
  try {
    // TODO change to single sensor
    if (_prefix == "up") {
      dist__ = _plc->plcData.laser_up_head;
    } else {  // down
      dist__ = _plc->plcData.laser_bottom_head;
    }
  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
    return -1;
  }
  rgbPath = rgbPath_;
  xyzPath = xyzPath_;
  dist = dist__;
  return 0;
}

void caliCameraSensorWidget::recordData(const std::string& rgbPath,
                                        const std::string& xyzPath,
                                        float dist) {
  auto dataToken = jsonParser::toToken({_dataMainKey.toStdString(), "datas"});
  auto v = rapidjson::Pointer(dataToken).Get(*_dataDoc);
  if (v == nullptr) {
    rapidjson::Pointer(dataToken).Set(*_dataDoc,
                                      rapidjson::Value(rapidjson::kArrayType));
    v = rapidjson::Pointer(dataToken).Get(*_dataDoc);
  }

  auto& allocator = _dataDoc->GetAllocator();
  rapidjson::Value obj(rapidjson::kObjectType);
  {
    obj.AddMember("image", rapidjson::Value(rgbPath, allocator), allocator);
    obj.AddMember("pointcloud", rapidjson::Value(xyzPath, allocator),
                  allocator);
    obj.AddMember<float>("sensorDist", dist, allocator);
  }
  v->PushBack(obj, allocator);
}

void caliCameraSensorWidget::deleteLastItem() {
  if (0 != ensureJsonStruct()) {
    return;
  };

  std::string mainKey = _dataMainKey.toStdString();
  auto& datas = (*_dataDoc)[mainKey.c_str()]["datas"];
  if (!datas.Empty()) {
    datas.PopBack();
  }
}

int caliCameraSensorWidget::calculate() {
  // _API_ int getSensorOriginDirection(
  // 	const std::vector<cameraSensorCaliData>& datas,
  // 	Eigen::Vector3f& origin,
  // 	Eigen::Vector3f& direction);
  auto dataToken = jsonParser::toToken({_dataMainKey.toStdString(), "datas"});
  auto dataValue = rapidjson::Pointer(dataToken).Get(*_dataDoc);
  if (dataValue == nullptr) {
    return -1;
  }
  assert(dataValue->IsArray());
  assert(dataValue->Size() > 1);

  std::vector<cameraSensorCaliData> caliDatas;
  for (size_t i = 0; i < dataValue->Size(); i++) {
    cameraSensorCaliData caliData;
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
    // sensor distance
    caliData.dist = data["sensorDist"].GetFloat();
    caliDatas.push_back(caliData);
  }
  assert(caliDatas.size() > 1);
  if (caliDatas.size() <= 1) {
    return -1;
  }
  // calculate
  Eigen::Vector3f origin(0, 0, 0), dir(0, 0, 0);
  try {
    if (0 > getSensorOriginDirection(caliDatas, origin, dir)) {
      return -1;
    };
  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
    return -1;
  }
  // set json
  _resultDoc->setArray<float>({_resultMainKey.toStdString(), "origin"},
                              {origin[0], origin[1], origin[2]});
  _resultDoc->setArray<float>({_resultMainKey.toStdString(), "direction"},
                              {origin[0], origin[1], origin[2]});
}

void caliCameraSensorWidget::updateTreeView() {
  jsonParser doc__;
  auto& allocator = doc__.GetAllocator();
  doc__.SetObject();
  // copy main domain
  std::string dataMainKey = _dataMainKey.toStdString();
  auto& dataMain = (*_dataDoc)[dataMainKey.c_str()];
  for (auto it = dataMain.MemberBegin(); it < dataMain.MemberEnd(); it++) {
    // copy
    rapidjson::Value key, value;
    key.CopyFrom(it->name, allocator);
    value.CopyFrom(it->value, allocator);
    doc__.AddMember(key, value, allocator);
  }
  // copy result
  std::string resultMainKey = _resultMainKey.toStdString();
  auto& resultMain = (*_resultDoc)[resultMainKey.c_str()];
  doc__.AddMember("results", rapidjson::Value(rapidjson::kObjectType),
                  allocator);
  for (auto it = resultMain.MemberBegin(); it < resultMain.MemberEnd(); it++) {
    // copy
    rapidjson::Value key, value;
    key.CopyFrom(it->name, allocator);
    value.CopyFrom(it->value, allocator);
    doc__["results"].AddMember(key, value, allocator);
  }

  // to qbytearray
  rapidjson::StringBuffer sb;
  rapidjson::PrettyWriter<rapidjson::StringBuffer> w(sb);
  doc__.Accept(w);
  QByteArray arr(sb.GetString(), sb.GetSize());
  emit updateTreeView(arr);
}

void caliCameraSensorWidget::dumpJson() {
  {
    rapidjson::StringBuffer sb;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> w(sb);
    _dataDoc->Accept(w);
    std::cout << sb.GetString() << std::endl;
  }
  {
    rapidjson::StringBuffer sb;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> w(sb);
    _resultDoc->Accept(w);
    std::cout << sb.GetString() << std::endl;
  }
}

void caliCameraSensorWidget::on_btn_record_clicked() {
  QtConcurrent::run([this]() {
    clearResult();
    writeResult();
    // FIXME
#if 1
    std::string rgb, xyz;
    float dist(0);
    if (0 != readData(rgb, xyz, dist)) {
      std::cout << "read data error" << std::endl;
      return;
    }
    recordData(rgb, xyz, dist);
#else
    recordData("camera", "pcd", 1);
#endif
    writeData();
    // update  tree
    updateTreeView();
  });
}

void caliCameraSensorWidget::on_btn_calculate_clicked() {
  QtConcurrent::run([this]() {
    clearResult();
    writeResult();
    calculate();
    writeResult();
    updateTreeView();
  });
}

void caliCameraSensorWidget::on_btn_delete_clicked() {
  QtConcurrent::run([this]() {
    clearResult();
    writeResult();
    deleteLastItem();
    writeData();
    updateTreeView();
  });
}

void caliCameraSensorWidget::connectDevice() {
  __camera = new VWSCamera();
  __camera->Init("192.168.125.99", 1);
  __camera->connect();
  __camera->changeTriggerMode(false);
  __camera->startGrab();
}

void caliCameraSensorWidget::on_UpdateImage() {
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
  bool useOrigin = false;
  size_t gridWidth(0), gridHeight(0);
  float gridSize(0);
  // find grid
  uchar* buff = NULL;
  size_t w = data.RGB8PlanarImage.nWidth;
  size_t h = data.RGB8PlanarImage.nHeight;
  if (!useOrigin) {
    useOrigin = 0 != getLaserMarkerImage(w, h, data.RGB8PlanarImage.pData,
                                         (void**)&buff);
  }
  // show image
  void* dst = useOrigin ? data.RGB8PlanarImage.pData : buff;

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