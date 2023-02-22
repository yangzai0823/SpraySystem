#include "caliextraaxiswidget.h"

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
#include "ui_caliextraaxiswidget.h"

caliExtraAxisWidget::caliExtraAxisWidget(const std::string& prefix,
                                         QWidget* parent)
    : baseCaliWidget(parent),
      ui(new Ui::caliExtraAxisWidget),
      _jsonPrefix(prefix) {
  ui->setupUi(this);
  //
  _dataMainKey = _jsonPrefix + "ExtraAxisCaliDatas";
  _resultMainKey = _jsonPrefix + "ExtraAxisCalibration";
  // other
  QtConcurrent::run([this]() {
    readData();
    writeData();
    readResult();
    writeResult();
    updateTree();
  });
}

caliExtraAxisWidget::~caliExtraAxisWidget() { delete ui; }

void caliExtraAxisWidget::setDevice(RobotOperator* robot,
                                    MCOperator* motionController) {
  _robot = robot;
  _motionController = motionController;
}

int caliExtraAxisWidget::readDeviceData(std::array<float, 4>& data) {
  // data:{position, x, y, z}
  try {
    bool success;
    VWSRobot::RobotPosition pose;
    if (1 != _robot->getRobotPosition(pose)) {
      return -1;
    };
    auto position = _motionController->getRealTimeEncoder(success)[0];
    data[0] = position;
    data[1] = pose.pos[0];
    data[2] = pose.pos[1];
    data[3] = pose.pos[2];
    return 0;
  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
    return -1;
  }
}

void caliExtraAxisWidget::clearResult() {
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

void caliExtraAxisWidget::recordData(const std::array<float, 4>& data) {
  rapidjson::Value obj__(rapidjson::kObjectType);
  auto& allocator = _dataDoc->GetAllocator();
  {
    rapidjson::Value arr__(rapidjson::kArrayType);
    arr__.PushBack(data[1], allocator);
    arr__.PushBack(data[2], allocator);
    arr__.PushBack(data[3], allocator);
    obj__.AddMember(rapidjson::Value("robotPosition", allocator), arr__,
                    allocator);
    obj__.AddMember<float>("extraAxisPosition", data[0], allocator);
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

void caliExtraAxisWidget::deleteLastItem() {
  auto token = jsonParser::toToken({_dataMainKey, "datas"});
  auto v = rapidjson::Pointer(token).Get(*_dataDoc);
  if (v && v->IsArray() && !v->Empty()) {
    v->PopBack();
  }
}

void caliExtraAxisWidget::calculate() {
  // _API_ int getExtraAxisDirection(
  // const std::vector<Eigen::Vector3f> points,
  // const std::vector<float> positions,
  // Eigen::Vector3f& extraAxisDirection);
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
  std::vector<float> positions;
  for (auto&& i : v->GetArray()) {
    positions.push_back(i["extraAxisPosition"].GetFloat());
    auto& points__ = i["robotPosition"];
    points.emplace_back(points__[0].GetFloat(), points__[1].GetFloat(),
                        points__[2].GetFloat());
  }
  Eigen::Vector3f dir;
  try {
    if (0 != getExtraAxisDirection(points, positions, dir)) {
      return;
    }

  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
  }
  // save results json
  _resultDoc->setArray<float>({_resultMainKey, "direction"},
                              {dir[0], dir[1], dir[2]});
}

void caliExtraAxisWidget::updateTree() {
  rapidjson::Document doc__;
  auto& allocator = doc__.GetAllocator();
  doc__.SetObject();
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

void caliExtraAxisWidget::on_btn_record_clicked() {
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
    recordData({1, 2, 3, 4});
    recordData({2, 3, 4, 5});
    recordData({3, 4, 5, 6});
#endif
    writeData();
    // update  tree
    updateTree();
  });
}

void caliExtraAxisWidget::on_btn_calculate_clicked() {
  QtConcurrent::run([this]() {
    clearResult();
    writeResult();
    calculate();
    writeResult();
    updateTree();
  });
}

void caliExtraAxisWidget::on_btn_delete_clicked() {
  QtConcurrent::run([this]() {
    clearResult();
    writeResult();
    deleteLastItem();
    writeData();
    updateTree();
  });
}
