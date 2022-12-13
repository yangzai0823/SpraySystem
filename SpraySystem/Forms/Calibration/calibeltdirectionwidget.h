#pragma once

#include <Forms/Calibration/basecaliwidget.h>
#include <rapidjson/document.h>

#include <Eigen/Dense>
#include <QString>
#include <QWidget>
#include <array>
#include <memory>
#include <string>

namespace Ui {
class caliBeltDirectionWidget;
}

class RobotOperator;
class MCOperator;

class caliBeltDirectionWidget : public baseCaliWidget {
  Q_OBJECT

 public:
  explicit caliBeltDirectionWidget(QWidget* parent = nullptr);
  ~caliBeltDirectionWidget();
  void setDevice(RobotOperator* robot, MCOperator* _motionController);

 private:
  virtual void readCalibratedDatas() override;
  virtual void clearResult() override;
  virtual void deleteLastItem() override;

  int readDeviceData(std::array<float, 5>& data);
  void recordData(const std::array<float, 5>& data);
  virtual void updateTree() override;
  void calculate();

 private slots:
  void on_btn_record_clicked();

  void on_btn_calculate_clicked();

  void on_btn_delete_clicked();

 private:
  Ui::caliBeltDirectionWidget* ui;
  // device
  RobotOperator* _robot;
  MCOperator* _motionController;
  // data
  std::unique_ptr<Eigen::Vector3f> _frontExtraAxisDirection;
};
