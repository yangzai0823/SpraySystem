#pragma once

#include <Forms/Calibration/basecaliwidget.h>
#include <VWSCamera/VWSCamera.h>

#include <Eigen/Dense>
#include <QString>
#include <QWidget>
#include <array>
#include <memory>
#include <mutex>
#include <string>

namespace Ui {
class caliHandEyewWidget;
}

class RobotOperator;
class MCOperator;
class CameraOperator;

class caliHandEyewWidget : public baseCaliWidget {
  Q_OBJECT

 public:
  explicit caliHandEyewWidget(const std::string& prefix,
                              QWidget* parent = nullptr);
  ~caliHandEyewWidget();
  void setDevice(RobotOperator* robot, MCOperator* motionController,
                 CameraOperator* camera);

 private:
  void readCalibratedDatas();
  virtual void clearResult() override;

  int readCameraData(std::string& rgbPath, std::string& xyzPath,
                     float& cameraBeltPos);
  void recordCameraData(const std::string& rgbPath, const std::string& xyzPath,
                        float cameraBeltPos);
  int readStationData(float& robotBeltPos, float& extraAxisPos,
                      Eigen::Vector3f& robotPos);
  void recordStationData(const float& robotBeltPos, const float& extraAxisPos,
                         const Eigen::Vector3f& robotPos);
  void recordRobotData(const float& extraAxisPos,
                       const Eigen::Vector3f& robotPos);
  void parseGridInfo(size_t& w, size_t& h, float& size);
  void deleteLastItem();
  void updateTree();
  int calculate();

  // button
  void enableButton(size_t index);

  // DELETEME
  void connectDevice();

 signals:
  void updateImage(const QPixmap&);

 private slots:
  void on_btn_capture_clicked();
  void on_btn_calculate_clicked();
  void on_btn_delete_clicked();
  void on_btn_record1_clicked();
  void on_btn_record2_clicked();
  void on_btn_record3_clicked();
  void on_UpdateImage();

 private:
  Ui::caliHandEyewWidget* ui;
  // device
  RobotOperator* _robot;
  MCOperator* _motionController;
  CameraOperator* _camera;
  // key prefix
  std::string _prefix;
  // data
  std::unique_ptr<Eigen::Vector3f> _frontExtraAxisDirection;
  std::unique_ptr<Eigen::Vector3f> _robotBeltDirection;
  // timer to update image
  QThread* _thd_updateImageView;
  QTimer* _timer_updateImageView;
  std::mutex _lock;

  // DELETEME
  VWSCamera* __camera;
};
