#pragma once

#include <VWSCamera/VWSCamera.h>

#include <Eigen/Dense>
#include <QString>
#include <QWidget>
#include <array>
#include <memory>
#include <mutex>
#include <string>

#include "Forms/Calibration/basecaliwidget.h"

namespace Ui {
class caliCameraSensorWidget;
}

class CameraOperator;
class PLCOperator;

class caliCameraSensorWidget : public baseCaliWidget {
  Q_OBJECT

 public:
  explicit caliCameraSensorWidget(const std::string& prefix,
                                  QWidget* parent = nullptr);
  ~caliCameraSensorWidget();
  void setDevice(PLCOperator* pcl, CameraOperator* camera);

 private:
  virtual void readCalibratedDatas() override;
  virtual void clearResult() override;
  virtual void deleteLastItem() override;

  int readCameraSensorData(std::string& rgbPath, std::string& xyzPath,
                           float& dist);
  void recordCameraSensorData(const std::string& rgbPath,
                              const std::string& xyzPath, float dist);
  void updateTree();
  int calculate();

  // DELETEME
  void connectDevice();

 signals:
  void updateImage(const QPixmap&);

 private slots:
  void on_btn_record_clicked();
  void on_btn_calculate_clicked();
  void on_btn_delete_clicked();
  void on_UpdateImage();

 private:
  Ui::caliCameraSensorWidget* ui;
  // device
  CameraOperator* _camera;
  PLCOperator* _plc;
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
