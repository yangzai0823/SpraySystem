#pragma once
#include <rapidjson/document.h>

#include <QByteArray>
#include <QWidget>
#include <Util/jsonParser/jsonparser.hpp>
#include <iostream>
#include <memory>
#include <string>

class baseCaliWidget : public QWidget {
  Q_OBJECT
 protected:
  // file name &path
  const std::string _dataFileName = "calibrationDatas.json";
  const std::string _resultFileName = "calibrationResults.json";
  const std::string _dataFolderName = "CalibrationData";
  std::string _dataFilePath = "";
  std::string _resultFilePath = "";
  // key
  std::string _dataMainKey = "";
  std::string _resultMainKey = "";
  // rapidjson document
  std::unique_ptr<jsonParser> _dataDoc = std::make_unique<jsonParser>();
  std::unique_ptr<jsonParser> _resultDoc = std::make_unique<jsonParser>();

 public:
  baseCaliWidget(QWidget* parent);
  ~baseCaliWidget();

 protected:
  void ensureFileExist();
  virtual void readCalibratedDatas(){};
  void readData();
  void writeData();
  void readResult();
  void writeResult();
  virtual void deleteLastItem() = 0;
  virtual void clearResult() = 0;
  void dumpData(std::ostream& out = std::cout);
  void dumpResult(std::ostream& out = std::cout);
  virtual void updateTree() = 0;

 signals:
  void updateTreeView(const QByteArray&);
};