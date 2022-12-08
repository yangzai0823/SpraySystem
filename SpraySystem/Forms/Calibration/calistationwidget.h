#pragma once

#include <Eigen/Dense>
#include <QString>
#include <QWidget>
#include <array>
#include <memory>
#include <mutex>
#include <string>

#include "Util/jsonParser/jsonparser.hpp"

namespace Ui {
class caliStationWidget;
}

class RobotOperator;
class MCOperator;

// TODO exception handler
// TODO handle nan
class caliStationWidget : public QWidget {
  Q_OBJECT

 public:
  explicit caliStationWidget(QWidget* parent = nullptr);
  ~caliStationWidget();
  void setDevice(RobotOperator* frontRobot, MCOperator* frontMotionController,
                 RobotOperator* backRobot, MCOperator* backMotionController);

 private:
  void ensureFileExist();
  int ensureJsonStruct();
  void readCalibratedDatas();
  void readData();
  void writeData();
  void readResult();
  void clearResult();
  void writeResult();

  int readBeltData(const std::string& prefix, float& robotBeltPos);
  void recordBeltData(const std::string& prefix, float robotBeltPos);
  int readRobotData(const std::string& prefix, float& extraAxisPos,
                    Eigen::Vector3f& robotPos);
  void recordRobotData(const std::string& prefix, float extraAxisPos,
                       const Eigen::Vector3f& robotPos);

  void deleteLastItem();
  void updateTreeView();
  int calculate();
  void dumpJson();

  // DELETEME
  void connectDevice();

 signals:
  void updateTreeView(const QByteArray&);

 private slots:
  void on_btn_calculate_clicked();

  void on_btn_delete_clicked();

  void on_btn_recordBeltPos_clicked();

  void on_btn_recordRobotPos_clicked();

 private:
  Ui::caliStationWidget* ui;
  // device
  RobotOperator* _robot1;
  RobotOperator* _robot2;
  MCOperator* _motionController1;
  MCOperator* _motionController2;
  // file name
  QString _dataFilePath;
  QString _resFilePath;
  // QJsonDoc
  QString _dataMainKey;
  QString _resultMainKey;
  std::unique_ptr<jsonParser> _dataDoc;
  std::unique_ptr<jsonParser> _resultDoc;
  // data
  std::unique_ptr<Eigen::Vector3f> _frontExtraAxisDirection;
  std::unique_ptr<Eigen::Vector3f> _backExtraAxisDirection;
  std::unique_ptr<Eigen::Vector3f> _robotBeltDirection;
};
