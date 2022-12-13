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

caliStationWidget::caliStationWidget(QWidget* parent)
    : baseCaliWidget(parent),
      ui(new Ui::caliStationWidget),
      _frontExtraAxisDirection(nullptr),
      _backExtraAxisDirection(nullptr),
      _robotBeltDirection(nullptr) {
  ui->setupUi(this);
  //
  _dataMainKey = "stationCaliDatas";
  _resultMainKey = "stationCalibration";
  // other
  QtConcurrent::run([this]() {
    readData();
    readResult();
    readCalibratedDatas();
    updateTree();
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

void caliStationWidget::clearResult() {
  auto token = jsonParser::toToken({_resultMainKey, "transformationMatrix"});
  auto v = rapidjson::Pointer(token).Get(*_resultDoc);
  if (v) {
    if (v->IsArray()) {
      v->Clear();
    } else {
      v->SetArray();
    }
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
  _dataDoc->setValue<float>({_dataMainKey, "datas", prefix + "BeltPos"},
                            robotBeltPos);
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
  auto& allocator = _dataDoc->GetAllocator();
  // extraaxis
  {
    auto token =
        jsonParser::toToken({_dataMainKey, "datas", prefix + "ExtraAxisPos"});
    auto v = rapidjson::Pointer(token).Get(*_dataDoc);
    if (v) {
      if (!v->IsArray()) {
        v->SetArray();
      }
      v->PushBack(extraAxisPos, allocator);
    } else {
      rapidjson::Pointer(token).Set(*_dataDoc, extraAxisPos);
    }
  }
  // robotPos
  rapidjson::Value arr(rapidjson::kArrayType);
  arr.PushBack<float>(robotPos[0], allocator);
  arr.PushBack<float>(robotPos[1], allocator);
  arr.PushBack<float>(robotPos[2], allocator);
  {
    auto token =
        jsonParser::toToken({_dataMainKey, "datas", prefix + "RobotPos"});
    auto v = rapidjson::Pointer(token).Get(*_dataDoc);
    if (v) {
      if (!v->IsArray()) {
        v->SetArray();
      }
      v->PushBack(arr, allocator);
    } else {
      rapidjson::Pointer(token).Set(*_dataDoc, arr);
    }
  }
}

void caliStationWidget::deleteLastItem() {
  {
    auto token =
        jsonParser::toToken({_dataMainKey, "datas", "frontExtraAxisPos"});
    auto v = rapidjson::Pointer(token).Get(*_dataDoc);
    if (v && v->IsArray() && !v->Empty()) {
      v->PopBack();
    }
  }
  {
    auto token =
        jsonParser::toToken({_dataMainKey, "datas", "backExtraAxisPos"});
    auto v = rapidjson::Pointer(token).Get(*_dataDoc);
    if (v && v->IsArray() && !v->Empty()) {
      v->PopBack();
    }
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
  if (_backExtraAxisDirection == nullptr) {
    std::cout << "backExtraAxisDirection not found" << std::endl;
    return -1;
  }
  if (_robotBeltDirection == nullptr) {
    std::cout << "robotBeltDirection not found" << std::endl;
    return -1;
  }
  auto dataToken = jsonParser::toToken({_dataMainKey, "datas"});
  auto dataValue = rapidjson::Pointer(dataToken).Get(*_dataDoc);
  if (dataValue == nullptr || !dataValue->IsObject()) {
    return -1;
  }

  stationCaliData caliDatas;
  {
    auto v = rapidjson::Pointer(
                 jsonParser::toToken({_dataMainKey, "datas", "frontBeltPos"}))
                 .Get(*_dataDoc);
    if (!v || !v->IsFloat()) {
      return -1;
    }
    caliDatas.beltPos1 = (*dataValue)["frontBeltPos"].GetFloat();
  }
  {
    auto v = rapidjson::Pointer(
                 jsonParser::toToken({_dataMainKey, "datas", "backBeltPos"}))
                 .Get(*_dataDoc);
    if (!v || !v->IsFloat()) {
      return -1;
    }
    caliDatas.beltPos2 = (*dataValue)["backBeltPos"].GetFloat();
  }
  {
    auto v = rapidjson::Pointer(jsonParser::toToken({_dataMainKey, "datas",
                                                     "frontExtraAxisPos"}))
                 .Get(*_dataDoc);
    if (!v || !v->IsArray()) {
      return -1;
    }
    for (auto&& i : v->GetArray()) {
      if (!i.IsFloat()) {
        return -1;
      }
      caliDatas.extraAxisPos1.push_back(i.GetFloat());
    }
  }
  {
    auto v = rapidjson::Pointer(jsonParser::toToken({_dataMainKey, "datas",
                                                     "backExtraAxisPos"}))
                 .Get(*_dataDoc);
    if (!v || !v->IsArray()) {
      return -1;
    }
    for (auto&& i : v->GetArray()) {
      if (!i.IsFloat()) {
        return -1;
      }
      caliDatas.extraAxisPos2.push_back(i.GetFloat());
    }
  }
  {
    auto v = rapidjson::Pointer(
                 jsonParser::toToken({_dataMainKey, "datas", "frontRobotPos"}))
                 .Get(*_dataDoc);
    if (!v || !v->IsArray()) {
      return -1;
    }
    for (auto&& i : v->GetArray()) {
      if (!i.IsArray() || i.Size() != 3) {
        return -1;
      }
      caliDatas.robotPos1.emplace_back(i[0].GetFloat(), i[1].GetFloat(),
                                       i[2].GetFloat());
    }
  }
  {
    auto v = rapidjson::Pointer(
                 jsonParser::toToken({_dataMainKey, "datas", "backRobotPos"}))
                 .Get(*_dataDoc);
    if (!v || !v->IsArray()) {
      return -1;
    }
    for (auto&& i : v->GetArray()) {
      if (!i.IsArray() || i.Size() != 3) {
        return -1;
      }
      caliDatas.robotPos2.emplace_back(i[0].GetFloat(), i[1].GetFloat(),
                                       i[2].GetFloat());
    }
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
  _resultDoc->setArray<float>({_resultMainKey, "transformationMatrix"},
                              matrix__);
}

void caliStationWidget::updateTree() {
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
    doc__.setArray<float>({"preCalibrated", "robotBeltDirection"},
                          {(*_robotBeltDirection)[0], (*_robotBeltDirection)[1],
                           (*_robotBeltDirection)[2]});
  }
  // copy main domain
  auto dataMain = rapidjson::Pointer("/" + _dataMainKey).Get(*_dataDoc);
  if (dataMain) {
    for (auto it = dataMain->MemberBegin(); it < dataMain->MemberEnd(); it++) {
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
    for (auto it = resultMain->MemberBegin(); it < resultMain->MemberEnd();
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
    updateTree();
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
    updateTree();
  });
}

void caliStationWidget::on_btn_calculate_clicked() {
  QtConcurrent::run([this]() {
    clearResult();
    writeResult();
    calculate();
    writeResult();
    updateTree();
  });
}

void caliStationWidget::on_btn_delete_clicked() {
  QtConcurrent::run([this]() {
    clearResult();
    writeResult();
    deleteLastItem();
    writeData();
    updateTree();
  });
}