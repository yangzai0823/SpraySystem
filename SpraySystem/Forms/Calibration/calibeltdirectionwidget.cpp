#include "calibeltdirectionwidget.h"

#include <calibration.h>
#include <rapidjson/allocators.h>
#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>

#include <Eigen/Core>
#include <QDir>
#include <QFile>
#include <QString>
#include <QtConcurrent/QtConcurrent>

#include "Device/MotionController/mcoperator.h"
#include "Device/Robot/robotoperator.h"
#include "ui_calibeltdirectionwidget.h"

extern QString CALIBRATE_DATA_FILE;
extern QString CALIBRATE_RESULT_FILE;
extern QString DATA_FOLDER_NAME;

caliBeltDirectionWidget::caliBeltDirectionWidget(QWidget* parent)
    : QWidget(parent),
      ui(new Ui::caliBeltDirectionWidget),
      _dataMainKey("beltDirectionCaliDatas"),
      _resultMainKey("beltDirectionCalibration"),
      _dataDoc(new rapidjson::Document()),
      _resultDoc(new rapidjson::Document()),
      _frontExtraAxisDirection(nullptr) {
  ui->setupUi(this);
  // other
  QtConcurrent::run([this]() {
    ensureFileExist();
    readData();
    readResult();
    readCalibratedDatas();
    ensureJsonStruct();
    clearResult();
    updateTreeView();
  });
}

caliBeltDirectionWidget::~caliBeltDirectionWidget() { delete ui; }

void caliBeltDirectionWidget::setDevice(RobotOperator* robot,
                                        MCOperator* motionController) {
  _robot = robot;
  _motionController = motionController;
}

void caliBeltDirectionWidget::ensureFileExist() {
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

int caliBeltDirectionWidget::ensureJsonStruct() {
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

void caliBeltDirectionWidget::readCalibratedDatas() {
  if (!_resultDoc->HasMember("frontExtraAxisCalibration") ||
      !(*_resultDoc)["frontExtraAxisCalibration"].HasMember("direction") ||
      (*_resultDoc)["frontExtraAxisCalibration"]["direction"].Size() != 3) {
    std::cout << "frontExtraAxisDirection doesn't exist" << std::endl;
    return;
  }
  auto& arr = (*_resultDoc)["frontExtraAxisCalibration"]["direction"];
  _frontExtraAxisDirection = std::make_unique<Eigen::Vector3f>();
  (*_frontExtraAxisDirection)[0] = arr[0].GetFloat();
  (*_frontExtraAxisDirection)[1] = arr[1].GetFloat();
  (*_frontExtraAxisDirection)[2] = arr[2].GetFloat();
}

void caliBeltDirectionWidget::readData() {
  QFile file(_dataFilePath);
  file.open(QIODevice::ReadOnly | QIODevice::Text);
  auto str = file.readAll().toStdString();
  file.close();

  _dataDoc->Parse(str.c_str(), str.size());
}

void caliBeltDirectionWidget::writeData() {
  // data
  QFile file(_dataFilePath);
  file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate);
  rapidjson::StringBuffer sb;
  rapidjson::PrettyWriter<rapidjson::StringBuffer> w(sb);
  _dataDoc->Accept(w);
  file.write(sb.GetString(), sb.GetSize());
  file.close();
}

void caliBeltDirectionWidget::readResult() {
  QFile file(_resFilePath);
  file.open(QIODevice::ReadOnly | QIODevice::Text);
  auto str = file.readAll().toStdString();
  file.close();

  _resultDoc->Parse(str.c_str(), str.size());
}

void caliBeltDirectionWidget::writeResult() {
  // result
  QFile file(_resFilePath);
  file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate);
  rapidjson::StringBuffer sb;
  rapidjson::PrettyWriter<rapidjson::StringBuffer> w(sb);
  _resultDoc->Accept(w);
  file.write(sb.GetString(), sb.GetSize());
  file.close();
}

int caliBeltDirectionWidget::readDeviceData(std::array<float, 5>& data) {
  // data:{position, x, y, z}
  try {
    VWSRobot::RobotPosition pose;
    if (1 != _robot->getRobotPosition(pose)) {
      return -1;
    };
    auto extraPosition = _motionController->getChainEncoders()[1];
    auto beltPosition = _motionController->getChainEncoders()[0];
    data[0] = extraPosition;
    data[1] = pose.pos[0];
    data[2] = pose.pos[1];
    data[3] = pose.pos[2];
    data[4] = beltPosition;
    return 0;
  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
    return -1;
  }
}

void caliBeltDirectionWidget::clearResult() {
  ensureJsonStruct();
  std::string resultMainKey = _resultMainKey.toStdString();
  (*_resultDoc)[resultMainKey.c_str()]["direction"].Clear();
}

void caliBeltDirectionWidget::recordData(const std::array<float, 5>& data) {
  if (0 != ensureJsonStruct()) {
    return;
  };

  std::string mainKey = _dataMainKey.toStdString();
  auto& datas = (*_dataDoc)[mainKey.c_str()]["datas"];
  auto& allocator = _dataDoc->GetAllocator();
  {
    rapidjson::Value obj__(rapidjson::kObjectType);
    rapidjson::Value arr__(rapidjson::kArrayType);
    arr__.PushBack(data[1], allocator);
    arr__.PushBack(data[2], allocator);
    arr__.PushBack(data[3], allocator);
    obj__.AddMember(rapidjson::Value("robotPosition", allocator), arr__,
                    allocator);
    obj__.AddMember<float>("extraAxisPosition", data[0], allocator);
    obj__.AddMember<float>("beltPosition", data[4], allocator);

    datas.PushBack(obj__, allocator);
  }
}

void caliBeltDirectionWidget::deleteLastItem() {
  if (0 != ensureJsonStruct()) {
    return;
  };

  std::string mainKey = _dataMainKey.toStdString();
  auto& datas = (*_dataDoc)[mainKey.c_str()]["datas"];
  if (!datas.Empty()) {
    datas.PopBack();
  }
}

void caliBeltDirectionWidget::calculate() {
  // _API_ int getRobotBeltDirection(
  // 	const std::vector<Eigen::Vector3f> points,
  // 	const std::vector<float> beltPositions,
  // 	Eigen::Vector3f& beltDirection);

  try {
    if (_frontExtraAxisDirection == nullptr) {
      std::cout << "_frontExtraAxisDirection not found" << std::endl;
      return;
    }
    ensureJsonStruct();
    std::string mainKey = _dataMainKey.toStdString();
    auto& datas = (*_dataDoc)[mainKey.c_str()]["datas"];
    if (datas.Size() < 3) {
      std::cout << "data size less than 3, recod more data" << std::endl;
      return;
    }
    std::vector<Eigen::Vector3f> points;
    std::vector<float> beltPositions;
    Eigen::Vector3f stationPos(0, 0, 0);
    float extraAxisPos(0);
    for (auto i = datas.Begin(); i != datas.End(); i++) {
      beltPositions.push_back((*i)["beltPosition"].GetFloat());
      extraAxisPos = (*i)["extraAxisPosition"].GetFloat();
      auto& points__ = (*i)["robotPosition"];
      stationPos[0] = points__[0].GetFloat();
      stationPos[1] = points__[1].GetFloat();
      stationPos[2] = points__[2].GetFloat();
      stationPos += extraAxisPos * (*_frontExtraAxisDirection);
      points.push_back(stationPos);
    }
    Eigen::Vector3f dir;
    if (0 != getRobotBeltDirection(points, beltPositions, dir)) {
      return;
    }
    // TODO save results json
    std::string resultMainKey = _resultMainKey.toStdString();
    auto& allocator = _resultDoc->GetAllocator();
    auto& direction = (*_resultDoc)[resultMainKey.c_str()]["direction"];
    direction.Clear();
    direction.PushBack(dir[0], allocator);
    direction.PushBack(dir[1], allocator);
    direction.PushBack(dir[2], allocator);

  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
  }
}

void caliBeltDirectionWidget::updateTreeView() {
  rapidjson::Document doc__;
  auto& allocator = doc__.GetAllocator();
  doc__.SetObject();
  // set calibrated results
  {
    rapidjson::Value v(rapidjson::kArrayType);
    if (_frontExtraAxisDirection != nullptr) {
      v.PushBack((*_frontExtraAxisDirection)[0], allocator);
      v.PushBack((*_frontExtraAxisDirection)[1], allocator);
      v.PushBack((*_frontExtraAxisDirection)[2], allocator);
    }
    doc__.AddMember("frontExtraAxisDirection", v, allocator);
  }
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

void caliBeltDirectionWidget::dumpJson() {
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

void caliBeltDirectionWidget::on_btn_record_clicked() {
  QtConcurrent::run([this]() {
    clearResult();
    writeResult();
    // FIXME
#if 0
    std::array<float, 4> data;
    if (0 != readDeviceData(data)) {
      std::cout << "read data error" << std::endl;
      return;
    }
    recordData(data);
#else
    recordData({0, 2, 3, 4, 5});
    recordData({0, 3, 4, 5, 6});
    recordData({0, 4, 5, 6, 7});
#endif
    writeData();
    // update  tree
    updateTreeView();
  });
}

void caliBeltDirectionWidget::on_btn_calculate_clicked() {
  QtConcurrent::run([this]() {
    clearResult();
    writeResult();
    calculate();
    writeResult();
    updateTreeView();
  });
}

void caliBeltDirectionWidget::on_btn_delete_clicked() {
  QtConcurrent::run([this]() {
    clearResult();
    writeResult();
    deleteLastItem();
    writeData();
    updateTreeView();
  });
}
