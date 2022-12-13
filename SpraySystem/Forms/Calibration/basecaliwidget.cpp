#include "Forms/Calibration/basecaliwidget.h"

#include <QDir>
#include <QFile>

#include "Util/jsonParser/jsonparser.hpp"

baseCaliWidget::baseCaliWidget(QWidget* parent) : QWidget(parent) {
  QDir dir;
  auto folderPath = QDir::currentPath() + "/" + _dataFolderName.c_str();
  if (!dir.exists(folderPath)) {
    dir.mkpath(folderPath);
  }
  _dataFilePath = folderPath.toStdString() + "/" + _dataFileName;
  _resultFilePath = folderPath.toStdString() + "/" + _resultFileName;
}

baseCaliWidget::~baseCaliWidget() {}

void baseCaliWidget::readData() {
  _dataDoc->read(_dataFilePath);
  if (_dataDoc->IsNull() || !_dataDoc->IsObject()) {
    _dataDoc->SetObject();
  }
}

void baseCaliWidget::writeData() { _dataDoc->write(_dataFilePath); }

void baseCaliWidget::readResult() {
  _resultDoc->read(_resultFilePath);
  if (_resultDoc->IsNull() || !_resultDoc->IsObject()) {
    _resultDoc->SetObject();
  }
}

void baseCaliWidget::writeResult() { _resultDoc->write(_resultFilePath); }

void baseCaliWidget::dumpData(std::ostream& out) {
  rapidjson::StringBuffer sb;
  rapidjson::PrettyWriter<rapidjson::StringBuffer> w(sb);
  _dataDoc->Accept(w);
  out << sb.GetString() << std::endl;
}
void baseCaliWidget::dumpResult(std::ostream& out) {
  rapidjson::StringBuffer sb;
  rapidjson::PrettyWriter<rapidjson::StringBuffer> w(sb);
  _resultDoc->Accept(w);
  out << sb.GetString() << std::endl;
}