#include "caliextraaxiswidget.h"

#include <rapidjson/allocators.h>
#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>

#include <QDir>
#include <QFile>
#include <QString>
#include <QtConcurrent/QtConcurrent>

#include "Device/MotionController/mcoperator.h"
#include "Device/Robot/robotoperator.h"
#include "ui_caliextraaxiswidget.h"

QString CALIBRATE_DATA_FILE = "calibrateDatas.json";
QString CALIBRATE_RESULT_FILE = "calibrateResults.json";
QString DATA_FOLDER_NAME = "Data";

caliExtraAxisWidget::caliExtraAxisWidget(QWidget* parent)
    : QWidget(parent),
      ui(new Ui::caliExtraAxisWidget),
      _doc(new rapidjson::Document()) {
  ui->setupUi(this);
  // other
  ensureFileExist();
  readJson();
  // DELETEME
  // QtConcurrent::run([this]() { writeJson(); });
}

caliExtraAxisWidget::~caliExtraAxisWidget() { delete ui; }

void caliExtraAxisWidget::setJsonPrefix(const QString& prefix) {
  _jsonPrefix = prefix;
}
void caliExtraAxisWidget::setDevice(RobotOperator* robot,
                                    MCOperator* motionController) {
  _robot = robot;
  _motionController = motionController;
}

void caliExtraAxisWidget::ensureFileExist() {
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

int caliExtraAxisWidget::ensureJsonStruct() {
  if (_doc->IsNull()) {
    _doc->SetObject();
  }
  if (!_doc->IsObject()) {
    std::cout << "json format error" << std::endl;
    return -1;
  }
  // main
  auto& allocator = _doc->GetAllocator();
  std::string mainKey = _jsonPrefix.toStdString() + "ExtraAxisCaliDatas";
  if (!_doc->GetObject().HasMember(mainKey.c_str())) {
    rapidjson::Value obj(rapidjson::kObjectType);
    _doc->AddMember(rapidjson::Value(mainKey.c_str(), allocator), obj,
                    allocator);
  }
  // data
  auto& main = (*_doc)[mainKey.c_str()];
  if (!main.HasMember("datas")) {
    main.AddMember(rapidjson::Value("datas", allocator),
                   rapidjson::Value(rapidjson::kArrayType), allocator);
  }
}

void caliExtraAxisWidget::readJson() {
  QFile file(_dataFilePath);
  file.open(QIODevice::ReadOnly | QIODevice::Text);
  auto str = file.readAll().toStdString();
  file.close();

  _doc->Parse(str.c_str(), str.size());
}

void caliExtraAxisWidget::writeJson() {
  QFile file(_dataFilePath);
  file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate);
  rapidjson::StringBuffer sb;
  rapidjson::PrettyWriter<rapidjson::StringBuffer> w(sb);
  _doc->Accept(w);
  file.write(sb.GetString(), sb.GetSize());
  file.close();
}

int caliExtraAxisWidget::readData(std::array<float, 4>& data) {
  // data:{position, x, y, z}
  try {
    VWSRobot::RobotPosition pose;
    if (1 != _robot->getRobotPosition(pose)) {
      return -1;
    };
    auto position = _motionController->getChainEncoders()[0];
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

void caliExtraAxisWidget::recordData(const std::array<float, 4>& data) {
  if (0 != ensureJsonStruct()) {
    return;
  };

  std::string mainKey = _jsonPrefix.toStdString() + "ExtraAxisCaliDatas";
  auto& datas = (*_doc)[mainKey.c_str()]["datas"];
  auto& allocator = _doc->GetAllocator();
  {
    rapidjson::Value obj__(rapidjson::kObjectType);
    rapidjson::Value arr__(rapidjson::kArrayType);
    arr__.PushBack(data[1], allocator);
    arr__.PushBack(data[2], allocator);
    arr__.PushBack(data[3], allocator);
    obj__.AddMember(rapidjson::Value("robotPosition", allocator), arr__,
                    allocator);
    obj__.AddMember<float>("extraAxisPosition", data[0], allocator);
    datas.PushBack(obj__, allocator);
  }
}

void caliExtraAxisWidget::deleteLastItem() {
  // if (_doc->isEmpty()) {
  //   _doc->setObject(QJsonObject());
  // }
  // if (!_doc->isObject()) {
  //   std::cout << "json format error" << std::endl;
  //   return;
  // }
  // // main
  // auto root = _doc->object();
  // QString mainKey = _jsonPrefix + "ExtraAxisCaliDatas";
  // if (!root.contains(mainKey)) {
  //   root.insert(mainKey, QJsonObject());
  // }
  // auto main = root[mainKey].toObject();
  // // datas
  // if (!main.contains("datas")) {
  //   main.insert("datas", QJsonArray());
  // }
  // auto datas = main["datas"].toArray();
  // {
  //   QJsonObject obj__;
  //   obj__.insert("robotPosition", QJsonArray({data[1], data[2], data[3]}));
  //   obj__.insert("extraAxisPosition", data[0]);
  //   datas.append(obj__);
  // }
  // // assign
  // main["datas"] = datas;
  // root[mainKey] = main;
  // _doc->setObject(root);
}
void caliExtraAxisWidget::clearItems() {}

void caliExtraAxisWidget::updateTreeView() {
  std::string mainKey = _jsonPrefix.toStdString() + "ExtraAxisCaliDatas";
  auto& main = (*_doc)[mainKey.c_str()];
  auto& allocator = _doc->GetAllocator();
  // copy main domain
  rapidjson::Document doc__(&allocator);
  doc__.SetObject();
  for (auto it = main.MemberBegin(); it < main.MemberEnd(); it++) {
    // copy
    rapidjson::Value key, value;
    key.CopyFrom(it->name, allocator);
    value.CopyFrom(it->value, allocator);
    doc__.AddMember(key, value, allocator);
  }
  // to qbytearray
  rapidjson::StringBuffer sb;
  rapidjson::PrettyWriter<rapidjson::StringBuffer> w(sb);
  doc__.Accept(w);
  QByteArray arr(sb.GetString(), sb.GetSize());
  emit updateTreeView(arr);
}

void caliExtraAxisWidget::dumpJson() {
  rapidjson::StringBuffer sb;
  rapidjson::PrettyWriter<rapidjson::StringBuffer> w(sb);
  _doc->Accept(w);
  std::cout << sb.GetString() << std::endl;
}

void caliExtraAxisWidget::on_btn_record_clicked() {
  QtConcurrent::run([this]() {
  // FIXME
#if 0
  std::array<float, 4> data;
  if (0 != readData(data)) {
    std::cout << "read data error" << std::endl;
    return;
  }
   recordData(data);
#else
    recordData({1, 2, 3, 4});
#endif
    writeJson();
    // update  tree
    updateTreeView();
  });
}

void caliExtraAxisWidget::on_btn_calculate_clicked() { ensureJsonStruct(); }

void caliExtraAxisWidget::on_btn_save_clicked() {}

void caliExtraAxisWidget::on_btn_delete_clicked() {}
