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
class caliCameraSensorWidget;
}

class CameraOperator;
class PLCOperator;

// TODO exception handler
// TODO handle nan
class caliCameraSensorWidget : public QWidget {
  Q_OBJECT

 public:
  explicit caliCameraSensorWidget(const QString& prefix,
                                  QWidget* parent = nullptr);
  ~caliCameraSensorWidget();
  void setDevice(PLCOperator* pcl, CameraOperator* camera);

 private:
  void ensureFileExist();
  int ensureJsonStruct();
  void readCalibratedDatas();
  void readData();
  void writeData();
  void readResult();
  void clearResult();
  void writeResult();

  int readData(std::string& rgbPath, std::string& xyzPath, float& dist);
  void recordData(const std::string& rgbPath, const std::string& xyzPath,
                  float dist);

  void deleteLastItem();
  void updateTreeView();
  int calculate();
  void dumpJson();

  // DELETEME
  void connectDevice();

 signals:
  void updateTreeView(const QByteArray&);
  void updateImage(const QPixmap&);
  void exit();

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

  // file name
  QString _dataFilePath;
  QString _resFilePath;
  // QJsonDoc
  QString _prefix;
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
