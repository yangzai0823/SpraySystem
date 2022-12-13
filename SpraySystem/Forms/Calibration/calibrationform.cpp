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
#include <QtConcurrent/QtConcurrent>

#include "Device/Camera/cameraoperator.h"
#include "Device/MotionController/mcoperator.h"
#include "Device/PLC/plcoperator.h"
#include "Device/Robot/robotoperator.h"
#include "Device/devicemanager.h"
#include "Forms/Calibration/calibeltdirectionwidget.h"
#include "Forms/Calibration/calicamerasensorwidget.h"
#include "Forms/Calibration/caliextraaxiswidget.h"
#include "Forms/Calibration/calihandeyewidget.h"
#include "Forms/Calibration/calistationwidget.h"
#include "Forms/Calibration/deviceupdater.hpp"
#include "Util/QJsonModel/qjsonmodel.h"
#include "ui_calibrationform.h"

calibrationform::calibrationform(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::calibrationform),
      _extendedWidget(NULL),
      _device(new caliDevice),
      _deviceUpdater(new deviceUpdater()) {
  ui->setupUi(this);
  // DELETEME
  QtConcurrent::run([this]() { connectDevice(); });
  // other initialization
  _deviceUpdater->start();
}

calibrationform::~calibrationform() { delete ui; }

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

void calibrationform::updateDeviceStatus(deviceData *data) {
  const size_t digits = 3;
  {
    ui->lineEdit_pose0->setText(
        QString("%1,%2,%3,%4,%5,%6,%7")
            .arg(data->robotInfo1.robotPosition[0], 0, 'f', digits)
            .arg(data->robotInfo1.robotPosition[1], 0, 'f', digits)
            .arg(data->robotInfo1.robotPosition[2], 0, 'f', digits)
            .arg(data->robotInfo1.robotPosition[3], 0, 'f', digits)
            .arg(data->robotInfo1.robotPosition[4], 0, 'f', digits)
            .arg(data->robotInfo1.robotPosition[5], 0, 'f', digits)
            .arg(data->robotInfo1.robotPosition[6], 0, 'f', digits));

    ui->lineEdit_extraAxis0->setText(
        QString::number(data->robotInfo1.extraAxisPosition, 'f', digits));
  }
  {
    ui->lineEdit_pose1->setText(
        QString("%1,%2,%3,%4,%5,%6,%7")
            .arg(data->robotInfo2.robotPosition[0], 0, 'f', digits)
            .arg(data->robotInfo2.robotPosition[1], 0, 'f', digits)
            .arg(data->robotInfo2.robotPosition[2], 0, 'f', digits)
            .arg(data->robotInfo2.robotPosition[3], 0, 'f', digits)
            .arg(data->robotInfo2.robotPosition[4], 0, 'f', digits)
            .arg(data->robotInfo2.robotPosition[5], 0, 'f', digits)
            .arg(data->robotInfo2.robotPosition[6], 0, 'f', digits));
    ui->lineEdit_extraAxis1->setText(
        QString::number(data->robotInfo2.extraAxisPosition, 'f', digits));
  }
  ui->lineEdit_beltPosition->setText(
      QString::number(data->beltPos, 'f', digits));
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

void calibrationform::on_btn_caliSensor_clicked() {
  if (_extendedWidget != NULL) {
    delete _extendedWidget;
  }
  caliCameraSensorWidget *widget__ = NULL;
  if (ui->comboBox_sensorType->currentText() == "up") {
    widget__ = new caliCameraSensorWidget("up", this);
    widget__->setDevice(_device->pcl, _device->camera1);
  } else if (ui->comboBox_sensorType->currentText() == "down") {
    widget__ = new caliCameraSensorWidget("down", this);
    widget__->setDevice(_device->pcl, _device->camera1);
  }

  _extendedWidget = static_cast<QWidget *>(widget__);
  connect(_extendedWidget, SIGNAL(updateTreeView(const QByteArray &)), this,
          SLOT(onUpdateTreeView(const QByteArray &)));
  connect(_extendedWidget, SIGNAL(updateImage(const QPixmap &)), this,
          SLOT(onUpdateImage(const QPixmap &)));

  ui->centralwidget->layout()->addWidget(_extendedWidget);
}

void calibrationform::on_btn_caliStation_clicked() {
  if (_extendedWidget != NULL) {
    delete _extendedWidget;
  }
  caliStationWidget *widget__ = NULL;
  widget__ = new caliStationWidget(this);
  widget__->setDevice(_device->robot0, _device->motionController,
                      _device->robot0, _device->motionController);

  _extendedWidget = static_cast<QWidget *>(widget__);
  connect(_extendedWidget, SIGNAL(updateTreeView(const QByteArray &)), this,
          SLOT(onUpdateTreeView(const QByteArray &)));

  ui->centralwidget->layout()->addWidget(_extendedWidget);
}
