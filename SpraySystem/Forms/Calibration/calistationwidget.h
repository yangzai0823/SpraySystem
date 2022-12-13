#pragma once

#include <Eigen/Dense>
#include <QString>
#include <QWidget>
#include <array>
#include <memory>
#include <mutex>
#include <string>

#include "Forms/Calibration/basecaliwidget.h"

namespace Ui {
class caliStationWidget;
}

class RobotOperator;
class MCOperator;

class caliStationWidget : public baseCaliWidget {
  Q_OBJECT

 public:
  explicit caliStationWidget(QWidget* parent = nullptr);
  ~caliStationWidget();
  void setDevice(RobotOperator* frontRobot, MCOperator* frontMotionController,
                 RobotOperator* backRobot, MCOperator* backMotionController);

 private:
  virtual void readCalibratedDatas() override;
  virtual void deleteLastItem() override;
  virtual void clearResult() override;

  int readBeltData(const std::string& prefix, float& robotBeltPos);
  void recordBeltData(const std::string& prefix, float robotBeltPos);
  int readRobotData(const std::string& prefix, float& extraAxisPos,
                    Eigen::Vector3f& robotPos);
  void recordRobotData(const std::string& prefix, float extraAxisPos,
                       const Eigen::Vector3f& robotPos);

  void updateTree();
  int calculate();

  // DELETEME
  void connectDevice();
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
  // data
  std::unique_ptr<Eigen::Vector3f> _frontExtraAxisDirection;
  std::unique_ptr<Eigen::Vector3f> _backExtraAxisDirection;
  std::unique_ptr<Eigen::Vector3f> _robotBeltDirection;
};
