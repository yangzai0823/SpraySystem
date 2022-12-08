#pragma once

#include <VWSCamera/VWSCamera.h>

#include <Eigen/Dense>
#include <QString>
#include <QWidget>
#include <array>
#include <memory>
#include <mutex>
#include <string>

#include "Util/jsonParser/jsonparser.hpp"

namespace Ui {
class caliHandEyewWidget;
}

class RobotOperator;
class MCOperator;
class CameraOperator;

// TODO exception handler
// TODO handle nan
class caliHandEyewWidget : public QWidget {
  Q_OBJECT

 public:
  explicit caliHandEyewWidget(const QString& prefix, QWidget* parent = nullptr);
  ~caliHandEyewWidget();
  void setDevice(RobotOperator* robot, MCOperator* motionController,
                 CameraOperator* camera);

 private:
  void ensureFileExist();
  int ensureJsonStruct();
  void readCalibratedDatas();
  void readData();
  void writeData();
  void readResult();
  void clearResult();
  void writeResult();

  void recordData(const std::array<float, 5>& data);
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

  int parseGridInfo(size_t& w, size_t& h, float& size);

  void deleteLastItem();
  void updateTreeView();
  int calculate();
  void dumpJson();

  // button
  void enableButton(size_t index);

  // DELETEME
  void connectDevice();

 signals:
  void updateTreeView(const QByteArray&);
  void updateImage(const QPixmap&);
  void exit();

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
  std::unique_ptr<Eigen::Vector3f> _robotBeltDirection;
  // timer to update image
  QThread* _thd_updateImageView;
  QTimer* _timer_updateImageView;
  std::mutex _lock;

  // DELETEME
  VWSCamera* __camera;
};
