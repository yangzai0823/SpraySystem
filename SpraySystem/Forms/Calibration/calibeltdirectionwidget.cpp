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
#include "Forms/Calibration/basecaliwidget.h"
#include "ui_calibeltdirectionwidget.h"

caliBeltDirectionWidget::caliBeltDirectionWidget(QWidget* parent)
    : baseCaliWidget(parent),
      ui(new Ui::caliBeltDirectionWidget),
      _frontExtraAxisDirection(nullptr) {
  ui->setupUi(this);
  // parent initialization
  _dataMainKey = "beltDirectionCaliDatas";
  _resultMainKey = "beltDirectionCalibration";
  // other
  QtConcurrent::run([this]() {
    readData();
    writeData();
    readResult();
    writeResult();
    readCalibratedDatas();
    clearResult();
    updateTree();
  });
}

caliBeltDirectionWidget::~caliBeltDirectionWidget() { delete ui; }

void caliBeltDirectionWidget::setDevice(RobotOperator* robot,
                                        MCOperator* motionController) {
  _robot = robot;
  _motionController = motionController;
}

void caliBeltDirectionWidget::readCalibratedDatas() {
  if (_resultDoc->IsNull()) {
    return;
  }
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

int caliBeltDirectionWidget::readDeviceData(std::array<float, 5>& data) {
  // data:{position, x, y, z}
  try {
    VWSRobot::RobotPosition pose;
    if (1 != _robot->getRobotPosition(pose)) {
      return -1;
    };
    auto extraPosition = _motionController->getRealTimeEncoder()[1];
    auto beltPosition = _motionController->getRealTimeEncoder()[0];
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
  auto token = jsonParser::toToken({_resultMainKey, "direction"});
  auto v = rapidjson::Pointer(token).Get(*_resultDoc);
  if (v) {
    if (v->IsArray()) {
      v->Clear();
    } else {
      v->SetArray();
    }
  }
}

void caliBeltDirectionWidget::recordData(const std::array<float, 5>& data) {
  auto& allocator = _dataDoc->GetAllocator();
  rapidjson::Value obj__(rapidjson::kObjectType);
  {
    rapidjson::Value arr__(rapidjson::kArrayType);
    arr__.PushBack(data[1], allocator);
    arr__.PushBack(data[2], allocator);
    arr__.PushBack(data[3], allocator);
    obj__.AddMember(rapidjson::Value("robotPosition", allocator), arr__,
                    allocator);
    obj__.AddMember<float>("extraAxisPosition", data[0], allocator);
    obj__.AddMember<float>("beltPosition", data[4], allocator);
  }

  auto token = jsonParser::toToken({_dataMainKey, "datas"});
  auto v = rapidjson::Pointer(token).Get(*_dataDoc);
  if (v) {
    if (!v->IsArray()) {
      v->SetArray();
    }
    v->PushBack(obj__, allocator);
  } else {
    rapidjson::Pointer(token)
        .Set(*_dataDoc, rapidjson::Value(rapidjson::kArrayType))
        .PushBack(obj__, allocator);
  }
}

void caliBeltDirectionWidget::deleteLastItem() {
  auto token = jsonParser::toToken({_dataMainKey, "datas"});
  auto v = rapidjson::Pointer(token).Get(*_dataDoc);
  if (v && v->IsArray() && !v->Empty()) {
    v->PopBack();
  }
}

void caliBeltDirectionWidget::calculate() {
  // _API_ int getRobotBeltDirection(
  // 	const std::vector<Eigen::Vector3f> points,
  // 	const std::vector<float> beltPositions,
  // 	Eigen::Vector3f& beltDirection);

  if (_frontExtraAxisDirection == nullptr) {
    std::cout << "_frontExtraAxisDirection not found" << std::endl;
    return;
  }
  auto token = jsonParser::toToken({_dataMainKey, "datas"});
  auto v = rapidjson::Pointer(token).Get(*_dataDoc);
  size_t len(0);
  if (v && v->IsArray()) {
    len = v->Size();
  } else {
    return;
  }

  if (len < 3) {
    std::cout << "data size less than 3, recod more data" << std::endl;
    return;
  }
  std::vector<Eigen::Vector3f> points;
  std::vector<float> beltPositions;
  Eigen::Vector3f stationPos(0, 0, 0);
  float extraAxisPos(0);
  for (auto&& i : (*v).GetArray()) {
    beltPositions.push_back(i["beltPosition"].GetFloat());
    extraAxisPos = i["extraAxisPosition"].GetFloat();
    auto& points__ = i["robotPosition"];
    stationPos[0] = points__[0].GetFloat();
    stationPos[1] = points__[1].GetFloat();
    stationPos[2] = points__[2].GetFloat();
    stationPos += extraAxisPos * (*_frontExtraAxisDirection);
    points.push_back(stationPos);
  }
  Eigen::Vector3f dir;
  try {
    if (0 != getRobotBeltDirection(points, beltPositions, dir)) {
      return;
    }
  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
    return;
  }
  // save results json
  _resultDoc->setArray<float>({_resultMainKey, "direction"},
                              {dir[0], dir[1], dir[2]});
}

void caliBeltDirectionWidget::updateTree() {
  jsonParser doc__;
  auto& allocator = doc__.GetAllocator();
  doc__.SetObject();
  // set calibrated results
  rapidjson::Value v(rapidjson::kArrayType);
  if (_frontExtraAxisDirection != nullptr) {
    v.PushBack((*_frontExtraAxisDirection)[0], allocator);
    v.PushBack((*_frontExtraAxisDirection)[1], allocator);
    v.PushBack((*_frontExtraAxisDirection)[2], allocator);
  }
  doc__.setValue({"preCalibrated", "frontExtraAxisDirection"}, v);
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

void caliBeltDirectionWidget::on_btn_record_clicked() {
  QtConcurrent::run([this]() {
    clearResult();
    writeResult();
    // FIXME
#if 0
    std::array<float, 5> data;
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
    updateTree();
  });
}

void caliBeltDirectionWidget::on_btn_calculate_clicked() {
  QtConcurrent::run([this]() {
    clearResult();
    writeResult();
    calculate();
    writeResult();
    updateTree();
  });
}

void caliBeltDirectionWidget::on_btn_delete_clicked() {
  QtConcurrent::run([this]() {
    clearResult();
    writeResult();
    deleteLastItem();
    writeData();
    updateTree();
  });
}
