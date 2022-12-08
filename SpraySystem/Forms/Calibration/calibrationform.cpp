#include "calibrationform.h"

#include <qstandarditemmodel.h>

#include <QDoubleSpinBox>
#include <QGraphicsPixmapItem>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonValue>
#include <QLineEdit>
#include <QSpinBox>
#include <QThread>
#include <QTimer>

#include "Device/Camera/cameraoperator.h"
#include "Device/MotionController/mcoperator.h"
#include "Device/PLC/plcoperator.h"
#include "Device/Robot/robotoperator.h"
#include "Device/devicemanager.h"
#include "Forms/Calibration/calibeltdirectionwidget.h"
#include "Forms/Calibration/caliextraaxiswidget.h"
#include "Forms/Calibration/calihandeyewidget.h"
#include "Util/QJsonModel/qjsonmodel.h"
#include "ui_calibrationform.h"

calibrationform::calibrationform(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::calibrationform),
      _extendedWidget(NULL),
      _device(new caliDevice),
      _timer_updatePositions(new QTimer(this)) {
  ui->setupUi(this);
  // DELETEME
  connectDevice();
  // other initialization
  // TODO change: use thd to emit signals to change ui
  _thd_updatePositions = new QThread();
  _timer_updatePositions = new QTimer();
  _timer_updatePositions->setInterval(200);
  _timer_updatePositions->moveToThread(_thd_updatePositions);
  connect(_timer_updatePositions, SIGNAL(timeout()), this,
          SLOT(updateDeviceStatus()), Qt::DirectConnection);
  connect(_thd_updatePositions, SIGNAL(started()), _timer_updatePositions,
          SLOT(start()));
  // TODO
  _thd_updatePositions->start();
}

calibrationform::~calibrationform() {
  _thd_updatePositions->exit();
  _thd_updatePositions->wait();
  // delete _timer_updatePositions;
  delete _thd_updatePositions;
  delete ui;
}

void calibrationform::initDevice() {
  auto dm = DeviceManager::getInstance();
  _device = std::make_shared<caliDevice>();
  // _device->robot0 = dm->getRobot(0);
  // _device->robot1 = dm->getRobot(1);
  // _device->camera0 = dm->getCamera(0);
  // _device->camera1 = dm->getCamera(1);
}

void calibrationform::connectDevice() {
  DeviceManager *deviceManager = DeviceManager::getInstance();
  // PLC
  auto plc = deviceManager->getPlc();
  plc->start();
  _device->pcl = plc;
  // 运动控制器
  auto mc = deviceManager->getMC();
  mc->start();
  _device->motionController = mc;
  // 机器人
  auto rbt = deviceManager->getRobot(0);
  rbt->init();
  rbt->start();
  _device->robot0 = rbt.get();
// 相机
#if (0)
  auto camera1 = deviceManager->getCamera(0);

  auto ret = camera1->init();
  std::cout << "camera1 init: " << ret << std::endl;
  ret = camera1->start();
  std::cout << "camera1 start: " << ret << std::endl;
  _device->camera0 = camera1;

  auto camera2 = deviceManager->getCamera(1);
  ret = camera2->init();
  std::cout << "camera2 init: " << ret << std::endl;
  ret = camera2->start();
  std::cout << "camera2 start: " << ret << std::endl;
  _device->camera1 = camera2;
#endif
}

void calibrationform::updateDeviceStatus() {
  // robot0
  updateRobotPosition(_device->robot0, _device->motionController,
                      ui->SpinBox_tcp0, ui->LineEdit_pose0,
                      ui->lineEdit_extraAxis0);
  //
  updateBeltPosition(_device->motionController, ui->lineEdit_beltPosition);
}

void calibrationform::updateRobotPosition(RobotOperator *robot,
                                          MCOperator *motionController,
                                          QSpinBox *sbox_tcp,
                                          QLineEdit *edit_pose,
                                          QLineEdit *edit_extraPosition) {
  if (robot == nullptr || motionController == nullptr) {
    return;
  }
  auto tcpNum = sbox_tcp->value();
  // TODO specify tcp
  VWSRobot::RobotPosition pose;
  if (robot->getRobotPosition(pose) == 1) {
    auto str__ = QString("%1,%2,%3,%4,%5,%6,%7")
                     .arg(pose.pos[0], 0, 'f', _digits)
                     .arg(pose.pos[1], 0, 'f', _digits)
                     .arg(pose.pos[2], 0, 'f', _digits)
                     .arg(pose.orient[0], 0, 'f', _digits)
                     .arg(pose.orient[1], 0, 'f', _digits)
                     .arg(pose.orient[2], 0, 'f', _digits)
                     .arg(pose.orient[3], 0, 'f', _digits);
    edit_pose->setText(str__);
  } else {
    auto str__ = QString("%1,%2,%3,%4,%5,%6,%7")
                     .arg("null")
                     .arg("null")
                     .arg("null")
                     .arg("null")
                     .arg("null")
                     .arg("null")
                     .arg("null");
    edit_pose->setText(str__);
  }
  // 0: extraAxis   1: belt
  auto position = motionController->getRealTimeEncoder()[0];
  {
    auto str__ = QString::number(position, 'f', _digits);
    edit_extraPosition->setText(str__);
  }
}

void calibrationform::updateBeltPosition(MCOperator *motionController,
                                         QLineEdit *edit_beltPosition) {
  if (motionController == nullptr) {
    return;
  }
  auto position = motionController->getRealTimeEncoder()[1];
  {
    auto tmp = motionController->getRealTimeEncoder();
    std::cout << tmp[0] << "\t" << tmp[1] << std::endl;
  }
  {
    auto str__ = QString::number(position, 'f', _digits);
    edit_beltPosition->setText(str__);
  }
}

void calibrationform::onUpdateTreeView(const QByteArray &arr) {
  auto model = new QJsonModel(arr);
  ui->treeView->setModel(model);
  ui->treeView->expandAll();
}

void calibrationform::onUpdateImage(const QPixmap &pixmap) {
  auto scene = ui->graphicsView->scene();
  if (scene == nullptr) {
    scene = new QGraphicsScene(ui->graphicsView);
    ui->graphicsView->setScene(scene);
  }
  scene->clear();
  auto scenePix = scene->addPixmap(pixmap);
  ui->graphicsView->fitInView(scenePix, Qt::AspectRatioMode::KeepAspectRatio);
  ui->graphicsView->update();
}

void calibrationform::saveExtraAxisData() {}
void calibrationform::on_btn_caliExtraAxis_clicked() {
  if (_extendedWidget != NULL) {
    delete _extendedWidget;
  }
  caliExtraAxisWidget *widget__ = NULL;
  if (ui->comboBox_extraAxisType->currentText() == "front") {
    widget__ = new caliExtraAxisWidget("front", this);
    widget__->setDevice(_device->robot0, _device->motionController);
  } else if (ui->comboBox_extraAxisType->currentText() == "back") {
    widget__ = new caliExtraAxisWidget("back", this);
    widget__->setDevice(_device->robot1, _device->motionController);
  }
  _extendedWidget = static_cast<QWidget *>(widget__);
  connect(_extendedWidget, SIGNAL(updateTreeView(const QByteArray &)), this,
          SLOT(onUpdateTreeView(const QByteArray &)));

  ui->centralwidget->layout()->addWidget(_extendedWidget);
}
void calibrationform::on_btn_caliBeltDirection_clicked() {
  if (_extendedWidget != NULL) {
    delete _extendedWidget;
  }
  auto widget__ = new caliBeltDirectionWidget(this);
  widget__->setDevice(_device->robot0, _device->motionController);
  _extendedWidget = static_cast<QWidget *>(widget__);
  connect(_extendedWidget, SIGNAL(updateTreeView(const QByteArray &)), this,
          SLOT(onUpdateTreeView(const QByteArray &)));

  ui->centralwidget->layout()->addWidget(_extendedWidget);
}

void calibrationform::on_btn_caliHandEye_clicked() {
  if (_extendedWidget != NULL) {
    delete _extendedWidget;
  }
  caliHandEyewWidget *widget__ = NULL;
  if (ui->comboBox_cameraType->currentText() == "up") {
    widget__ = new caliHandEyewWidget("up", this);
    widget__->setDevice(_device->robot0, _device->motionController,
                        _device->camera0);
  } else if (ui->comboBox_cameraType->currentText() == "down") {
    widget__ = new caliHandEyewWidget("down", this);
    widget__->setDevice(_device->robot0, _device->motionController,
                        _device->camera1);
  }
  _extendedWidget = static_cast<QWidget *>(widget__);
  connect(_extendedWidget, SIGNAL(updateTreeView(const QByteArray &)), this,
          SLOT(onUpdateTreeView(const QByteArray &)));
  connect(_extendedWidget, SIGNAL(updateImage(const QPixmap &)), this,
          SLOT(onUpdateImage(const QPixmap &)));

  ui->centralwidget->layout()->addWidget(_extendedWidget);
}

void calibrationform::on_btn_caliSensor_clicked() {}

void calibrationform::on_btn_caliStation_clicked() {}
