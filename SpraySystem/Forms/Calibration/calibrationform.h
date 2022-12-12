#ifndef CALIBRATIONFORM_H
#define CALIBRATIONFORM_H

#include <qpixmap.h>

#include <Forms/Calibration/deviceupdater.hpp>
#include <QByteArray>
#include <QMainWindow>
#include <memory>

namespace Ui {
class calibrationform;
}  // namespace Ui
class QSpinBox;
class QLineEdit;
class QDoubleSpinBox;

class RobotOperator;
class CameraOperator;
class PLCOperator;
class MCOperator;
class deviceUpdater;
class deviceData;

class calibrationform : public QMainWindow {
  Q_OBJECT

 public:
  explicit calibrationform(QWidget *parent = nullptr);
  ~calibrationform();

 public slots:

  void updateDeviceStatus(deviceData *);
  void onUpdateTreeView(const QByteArray &arr);
  void onUpdateImage(const QPixmap &pixmap);

 private slots:
  void on_btn_caliExtraAxis_clicked();
  void on_btn_caliBeltDirection_clicked();
  void on_btn_caliHandEye_clicked();
  void on_btn_caliSensor_clicked();
  void on_btn_caliStation_clicked();

 private:
  struct caliDevice {
    RobotOperator *robot0;
    RobotOperator *robot1;
    CameraOperator *camera0;
    CameraOperator *camera1;
    MCOperator *motionController;
    PLCOperator *pcl;
  };
  void initDevice();
  // DELETEME temporary usage to construct device
  void connectDevice();
  void updateRobotPosition(RobotOperator *robot, MCOperator *motionController,
                           QSpinBox *sbox_tcp, QLineEdit *edit_pose,
                           QLineEdit *edit_extraPosition);
  void updateBeltPosition(MCOperator *motionController,
                          QLineEdit *edit_beltPosition);

 private:
  Ui::calibrationform *ui;
  QWidget *_extendedWidget;

  const std::string _caliResultFileName;
  // device
  std::shared_ptr<caliDevice> _device;
  // timer to update positions
  deviceUpdater *_deviceUpdater;
};

#endif  // CALIBRATIONFORM_H
