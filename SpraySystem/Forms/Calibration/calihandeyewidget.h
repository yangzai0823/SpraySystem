#pragma once

#include <rapidjson/document.h>

#include <Eigen/Dense>
#include <QString>
#include <QWidget>
#include <array>
#include <memory>
#include <string>

namespace Ui {
class caliHandEyewWidget;
}

class RobotOperator;
class MCOperator;

// TODO exception handler
// TODO handle nan
class caliHandEyewWidget : public QWidget {
  Q_OBJECT

 public:
  explicit caliHandEyewWidget(const QString& prefix, QWidget* parent = nullptr);
  ~caliHandEyewWidget();
  void setDevice(RobotOperator* robot, MCOperator* _motionController);

 private:
  void ensureFileExist();
  int ensureJsonStruct();
  void readCalibratedDatas();
  void readData();
  void writeData();
  int readDeviceData(std::array<float, 5>& data);
  void readResult();
  void clearResult();
  void writeResult();

  void recordData(const std::array<float, 5>& data);
  void deleteLastItem();
  void updateTreeView();
  void calculate();
  void dumpJson();

 signals:
  void updateTreeView(const QByteArray&);

 private slots:
  void on_btn_record_clicked();

  void on_btn_calculate_clicked();

  void on_btn_delete_clicked();

 private:
  Ui::caliHandEyewWidget* ui;
  // device
  RobotOperator* _robot;
  MCOperator* _motionController;
  // file name
  QString _dataFilePath;
  QString _resFilePath;
  // QJsonDoc
  QString _dataMainKey;
  QString _resultMainKey;
  std::unique_ptr<rapidjson::Document> _dataDoc;
  std::unique_ptr<rapidjson::Document> _resultDoc;
  // data
  std::unique_ptr<Eigen::Vector3f> _frontExtraAxisDirection;
};
