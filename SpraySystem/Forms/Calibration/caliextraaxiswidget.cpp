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

QString CALIBRATE_DATA_FILE = "calibrateDatas.json";
QString CALIBRATE_RESULT_FILE = "calibrateResults.json";
QString DATA_FOLDER_NAME = "Data";

caliExtraAxisWidget::caliExtraAxisWidget(const QString& prefix, QWidget* parent)
    : QWidget(parent),
      ui(new Ui::caliExtraAxisWidget),
      _jsonPrefix(prefix),
      _dataMainKey("ExtraAxisCaliDatas"),
      _resultMainKey("ExtraAxisCalibration"),
      _dataDoc(new jsonParser()),
      _resultDoc(new jsonParser()) {
  ui->setupUi(this);
  // other
  QtConcurrent::run([this]() {
    ensureFileExist();
    readData();
    readResult();
    ensureJsonStruct();
    clearResult();
    updateTreeView();
  });
}

caliExtraAxisWidget::~caliExtraAxisWidget() { delete ui; }

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
  std::string dataMainKey =
      _jsonPrefix.toStdString() + _dataMainKey.toStdString();
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
  std::string resultMainKey =
      _jsonPrefix.toStdString() + _resultMainKey.toStdString();
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

void caliExtraAxisWidget::readData() {
  _dataDoc->read(_dataFilePath.toStdString());
}

void caliExtraAxisWidget::writeData() {
  _dataDoc->write(_dataFilePath.toStdString());
}

void caliExtraAxisWidget::readResult() {
  _resultDoc->read(_resFilePath.toStdString());
}

void caliExtraAxisWidget::writeResult() {
  _resultDoc->write(_resFilePath.toStdString());
}

int caliExtraAxisWidget::readDeviceData(std::array<float, 4>& data) {
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

void caliExtraAxisWidget::clearResult() {
  std::string resultMainKey =
      _jsonPrefix.toStdString() + _resultMainKey.toStdString();
  _resultDoc->setArray<float>({resultMainKey, "direction"}, {});
}

void caliExtraAxisWidget::recordData(const std::array<float, 4>& data) {
  std::string mainKey = _jsonPrefix.toStdString() + _dataMainKey.toStdString();
  std::vector<std::string> tokens = {mainKey, "datas"};
  auto token = jsonParser::toToken(tokens);
  if (!_dataDoc->keyExists(tokens)) {
    _dataDoc->setArray(tokens);
  }
  {
    auto& allocator = _dataDoc->GetAllocator();
    rapidjson::Value obj__(rapidjson::kObjectType);
    rapidjson::Value arr__(rapidjson::kArrayType);
    arr__.PushBack(data[1], allocator);
    arr__.PushBack(data[2], allocator);
    arr__.PushBack(data[3], allocator);
    obj__.AddMember(rapidjson::Value("robotPosition", allocator), arr__,
                    allocator);
    obj__.AddMember<float>("extraAxisPosition", data[0], allocator);
    rapidjson::Pointer(token).Get(*_dataDoc)->PushBack(obj__, allocator);
  }
}

void caliExtraAxisWidget::deleteLastItem() {
  if (0 != ensureJsonStruct()) {
    return;
  };

  std::string mainKey = _jsonPrefix.toStdString() + _dataMainKey.toStdString();
  std::vector<std::string> tokens = {mainKey, "datas"};
  _dataDoc->popBack(tokens);
}

void caliExtraAxisWidget::calculate() {
  // _API_ int getExtraAxisDirection(
  // const std::vector<Eigen::Vector3f> points,
  // const std::vector<float> positions,
  // Eigen::Vector3f& extraAxisDirection);

  try {
    ensureJsonStruct();
    std::string mainKey =
        _jsonPrefix.toStdString() + _dataMainKey.toStdString();
    auto token = jsonParser::toToken({mainKey, "datas"});
    auto datas = rapidjson::Pointer(token).Get(*_dataDoc)->GetArray();
    if (datas.Size() < 3) {
      std::cout << "data size less than 3, recod more data" << std::endl;
      return;
    }
    std::vector<Eigen::Vector3f> points;
    std::vector<float> positions;
    for (auto i = datas.Begin(); i != datas.End(); i++) {
      positions.push_back((*i)["extraAxisPosition"].GetFloat());
      auto& points__ = (*i)["robotPosition"];
      points.emplace_back(points__[0].GetFloat(), points__[1].GetFloat(),
                          points__[2].GetFloat());
    }
    Eigen::Vector3f dir;
    if (0 != getExtraAxisDirection(points, positions, dir)) {
      return;
    }
    // TODO save results json
    std::string resultMainKey =
        _jsonPrefix.toStdString() + _resultMainKey.toStdString();
    _resultDoc->setArray<float>({resultMainKey, "direction"},
                                {dir[0], dir[1], dir[2]});

  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
  }
}

void caliExtraAxisWidget::updateTreeView() {
  rapidjson::Document doc__;
  auto& allocator = doc__.GetAllocator();
  doc__.SetObject();
  // copy main domain
  std::string dataMainKey =
      _jsonPrefix.toStdString() + _dataMainKey.toStdString();
  auto& dataMain = (*_dataDoc)[dataMainKey.c_str()];
  for (auto it = dataMain.MemberBegin(); it < dataMain.MemberEnd(); it++) {
    // copy
    rapidjson::Value key, value;
    key.CopyFrom(it->name, allocator);
    value.CopyFrom(it->value, allocator);
    doc__.AddMember(key, value, allocator);
  }
  // copy result
  std::string resultMainKey =
      _jsonPrefix.toStdString() + _resultMainKey.toStdString();
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

void caliExtraAxisWidget::dumpJson() {
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
    updateTreeView();
  });
}

void caliExtraAxisWidget::on_btn_calculate_clicked() {
  QtConcurrent::run([this]() {
    clearResult();
    writeResult();
    calculate();
    writeResult();
    updateTreeView();
  });
}

void caliExtraAxisWidget::on_btn_delete_clicked() {
  QtConcurrent::run([this]() {
    clearResult();
    writeResult();
    deleteLastItem();
    writeData();
    updateTreeView();
  });
}
