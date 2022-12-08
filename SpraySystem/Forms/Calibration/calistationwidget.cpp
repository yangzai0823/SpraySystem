#include "calistationwidget.h"

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

#include "Device/MotionController/mcoperator.h"
#include "Device/Robot/robotoperator.h"
#include "Util/jsonParser/jsonparser.hpp"
#include "calibration.h"
#include "ui_calistationwidget.h"

extern QString CALIBRATE_DATA_FILE;
extern QString CALIBRATE_RESULT_FILE;
extern QString DATA_FOLDER_NAME;

caliStationWidget::caliStationWidget(QWidget* parent)
    : QWidget(parent),
      ui(new Ui::caliStationWidget),
      _dataMainKey("stationCaliDatas"),
      _resultMainKey("stationCalibration"),
      _dataDoc(new jsonParser()),
      _resultDoc(new jsonParser()),
      _frontExtraAxisDirection(nullptr),
      _backExtraAxisDirection(nullptr),
      _robotBeltDirection(nullptr) {
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

caliStationWidget::~caliStationWidget() { delete ui; }

void caliStationWidget::setDevice(RobotOperator* frontRobot,
                                  MCOperator* frontMotionController,
                                  RobotOperator* backRobot,
                                  MCOperator* backMotionController) {
  _robot1 = frontRobot;
  _robot2 = backRobot;
  _motionController1 = frontMotionController;
  _motionController2 = backMotionController;
}

void caliStationWidget::ensureFileExist() {
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

int caliStationWidget::ensureJsonStruct() {
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

void caliStationWidget::readCalibratedDatas() {
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
      _frontExtraAxisDirection =
          std::make_unique<Eigen::Vector3f>(arr[0], arr[1], arr[2]);
    }
  }
  {
    std::vector<float> arr;
    if (0 >
        _resultDoc->getArray({"backExtraAxisCalibration", "direction"}, arr)) {
      std::cout << "backExtraAxisDirection doesn't exist" << std::endl;
      return;
    } else {
      if (arr.size() != 3) {
        std::cout << "backExtraAxisDirection length must equal 3" << std::endl;
        return;
      }
      _backExtraAxisDirection =
          std::make_unique<Eigen::Vector3f>(arr[0], arr[1], arr[2]);
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
      _robotBeltDirection =
          std::make_unique<Eigen::Vector3f>(arr[0], arr[1], arr[2]);
    }
  }
}

void caliStationWidget::readData() {
  _dataDoc->read(_dataFilePath.toStdString());
}

void caliStationWidget::writeData() {
  _dataDoc->write(_dataFilePath.toStdString());
}

void caliStationWidget::readResult() {
  _resultDoc->read(_resFilePath.toStdString());
}

void caliStationWidget::writeResult() {
  _resultDoc->write(_resFilePath.toStdString());
}

void caliStationWidget::clearResult() {
  std::string resultMainKey = _resultMainKey.toStdString();
  (*_resultDoc)[resultMainKey.c_str()]["direction"].Clear();
  if (auto v =
          rapidjson::Pointer(jsonParser::toToken({resultMainKey, "direction"}))
              .Get(*_resultDoc)) {
    v->Clear();
  }
}

int caliStationWidget::readBeltData(const std::string& prefix,
                                    float& robotBeltPos) {
  float beltPos_(0);
  MCOperator* mc = prefix == "front" ? _motionController1 : _motionController2;
  try {
    beltPos_ = mc->getRealTimeEncoder()[1];
  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
    return -1;
  }
  robotBeltPos = beltPos_;
  return 0;
}
void caliStationWidget::recordBeltData(const std::string& prefix,
                                       float robotBeltPos) {
  _dataDoc->setValue<float>(
      {_dataMainKey.toStdString(), "datas", prefix + "BeltPos"}, robotBeltPos);
}

int caliStationWidget::readRobotData(const std::string& prefix,
                                     float& extraAxisPos,
                                     Eigen::Vector3f& robotPos) {
  MCOperator* mc = prefix == "front" ? _motionController1 : _motionController2;
  RobotOperator* robot = prefix == "front" ? _robot1 : _robot2;
  float extraAxisPos_(0);
  Eigen::Vector3f robotPos_;
  try {
    extraAxisPos_ = mc->getRealTimeEncoder()[0];

    VWSRobot::RobotPosition pose;
    if (1 != robot->getRobotPosition(pose)) {
      return -1;
    };
    robotPos_[0] = pose.pos[0];
    robotPos_[1] = pose.pos[1];
    robotPos_[2] = pose.pos[2];
  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
    return -1;
  }
  extraAxisPos = extraAxisPos_;
  robotPos = robotPos_;
  return 0;
}
// TODO JSON EXCEPTION
void caliStationWidget::recordRobotData(const std::string& prefix,
                                        float extraAxisPos,
                                        const Eigen::Vector3f& robotPos) {
  // extraaxis
  _dataDoc->pushBack<float>(
      {_dataMainKey.toStdString(), "datas", prefix + "ExtraAxisPos"},
      extraAxisPos);
  // robotPos
  auto& allocator = _dataDoc->GetAllocator();
  rapidjson::Value arr(rapidjson::kArrayType);
  arr.PushBack<float>(robotPos[0], allocator);
  arr.PushBack<float>(robotPos[1], allocator);
  arr.PushBack<float>(robotPos[2], allocator);
  _dataDoc->pushBack({_dataMainKey.toStdString(), "datas", prefix + "RobotPos"},
                     arr);
}

void caliStationWidget::deleteLastItem() {
  if (0 != ensureJsonStruct()) {
    return;
  };

  std::string mainKey = _dataMainKey.toStdString();
  auto& datas = (*_dataDoc)[mainKey.c_str()]["datas"];
  if (!datas.Empty()) {
    datas.PopBack();
  }
}

int caliStationWidget::calculate() {
  // _API_ int getStationTransformMatrix(
  // 	const stationCaliData& datas,
  // 	const Eigen::Vector3f& extraAxisDir1,
  // 	const Eigen::Vector3f& extraAxisDir2,
  // 	const Eigen::Vector3f& beltDir,
  // 	Eigen::Isometry3f& transf);

  if (_frontExtraAxisDirection == nullptr) {
    std::cout << "frontExtraAxisDirection not found" << std::endl;
    return -1;
  }
  if (_robotBeltDirection == nullptr) {
    std::cout << "robotBeltDirection not found" << std::endl;
    return -1;
  }
  auto dataToken = jsonParser::toToken({_dataMainKey.toStdString(), "datas"});
  auto dataValue = rapidjson::Pointer(dataToken).Get(*_dataDoc);
  if (dataValue == nullptr) {
    return -1;
  }
  // TODO assertion
  assert(dataValue->IsObject());

  stationCaliData caliDatas;
  caliDatas.beltPos1 = (*dataValue)["frontBeltPos"].GetFloat();
  caliDatas.beltPos2 = (*dataValue)["backBeltPos"].GetFloat();

  for (auto&& i : (*dataValue)["frontExtraAxisPos"].GetArray()) {
    caliDatas.extraAxisPos1.push_back(i.GetFloat());
  }
  for (auto&& i : (*dataValue)["backExtraAxisPos"].GetArray()) {
    caliDatas.extraAxisPos2.push_back(i.GetFloat());
  }
  for (auto&& i : (*dataValue)["frontRobotPos"].GetArray()) {
    caliDatas.robotPos1.emplace_back(i[0].GetFloat(), i[1].GetFloat(),
                                     i[2].GetFloat());
  }
  for (auto&& i : (*dataValue)["backRobotPos"].GetArray()) {
    caliDatas.robotPos2.emplace_back(i[0].GetFloat(), i[1].GetFloat(),
                                     i[2].GetFloat());
  }
  // calculate
  Eigen::Isometry3f matrix;
  try {
    if (0 > getStationTransformMatrix(caliDatas, *_frontExtraAxisDirection,
                                      *_backExtraAxisDirection,
                                      *_robotBeltDirection, matrix)) {
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
      matrix__.push_back(matrix(i, j));
    }
  }
  _resultDoc->setArray<float>(
      {_resultMainKey.toStdString(), "transformationMatrix"}, matrix__);
}

void caliStationWidget::updateTreeView() {
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
  if (_backExtraAxisDirection == nullptr) {
    doc__.setArray({"preCalibrated", "backExtraAxisDirection"});
  } else {
    doc__.setArray<float>(
        {"preCalibrated", "backExtraAxisDirection"},
        {(*_backExtraAxisDirection)[0], (*_backExtraAxisDirection)[1],
         (*_backExtraAxisDirection)[2]});
  }
  if (_robotBeltDirection == nullptr) {
    doc__.setArray({"preCalibrated", "robotBeltDirection"});
  } else {
    doc__.setArray<float>({"11", "robotBeltDirection"},
                          {(*_robotBeltDirection)[0], (*_robotBeltDirection)[1],
                           (*_robotBeltDirection)[2]});
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

void caliStationWidget::dumpJson() {
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

void caliStationWidget::on_btn_recordBeltPos_clicked() {
  QtConcurrent::run([this]() {
    clearResult();
    writeResult();

    float beltPos(0);
    std::string prefix = ui->comboBox_robotType->currentText().toStdString();
    if (0 != readBeltData(prefix, beltPos)) {
      std::cout << "read belt data error" << std::endl;
      return;
    }
    recordBeltData(prefix, beltPos);

    writeData();
    // update  tree
    updateTreeView();
  });
}

void caliStationWidget::on_btn_recordRobotPos_clicked() {
  QtConcurrent::run([this]() {
    clearResult();
    writeResult();

    float extraAxis(0);
    std::string prefix = ui->comboBox_robotType->currentText().toStdString();
    Eigen::Vector3f robotPos(0, 0, 0);
    if (0 != readRobotData(prefix, extraAxis, robotPos)) {
      std::cout << "read robot data error" << std::endl;
      return;
    }
    recordRobotData(prefix, extraAxis, robotPos);

    writeData();
    // update  tree
    updateTreeView();
  });
}

void caliStationWidget::on_btn_calculate_clicked() {
  QtConcurrent::run([this]() {
    clearResult();
    writeResult();
    calculate();
    writeResult();
    updateTreeView();
  });
}

void caliStationWidget::on_btn_delete_clicked() {
  QtConcurrent::run([this]() {
    clearResult();
    writeResult();
    deleteLastItem();
    writeData();
    updateTreeView();
  });
}