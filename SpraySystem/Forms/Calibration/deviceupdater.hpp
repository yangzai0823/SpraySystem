#pragma once
#include <QThread>
#include <QTimer>

#include "Device/MotionController/mcoperator.h"
#include "Device/PLC/plcoperator.h"
#include "Device/Robot/robotoperator.h"

struct robotData {
  float robotPosition[7];
  float extraAxisPosition;
};

struct deviceData {
  robotData robotInfo1;
  robotData robotInfo2;
  float beltPos;
};

class deviceUpdater : public QThread {
  Q_OBJECT
 private:
  deviceData _data = deviceData();
  RobotOperator *_robot1 = NULL;
  MCOperator *_motionController1 = NULL;
  RobotOperator *_robot2 = NULL;
  MCOperator *_motionController2 = NULL;
  size_t _robotTcp1 = 0;
  size_t _robotTcp2 = 0;

 public:
  deviceUpdater() {}
  ~deviceUpdater() {}
  void setDevice(RobotOperator *robot1_, MCOperator *motionController1_,
                 RobotOperator *robot2_, MCOperator *motionController2_) {
    _robot1 = robot1_;
    _robot2 = robot2_;
    _motionController1 = motionController1_;
    _motionController2 = motionController2_;
  }
  void setTcp(size_t tcp1, size_t tcp2) {
    _robotTcp1 = tcp1;
    _robotTcp2 = tcp2;
  }

 protected:
  void run() override {
    QTimer *timer = new QTimer(this);
    timer->setInterval(500);
    connect(timer, SIGNAL(timeout()), this, SLOT(getDeviceData()));
    timer->start();
    this->exec();
  }

 signals:
  void update(deviceData *);

 private slots:
  void getDeviceData() {
    getRobotData(_robot1, _motionController1, _data.robotInfo1);
    getRobotData(_robot2, _motionController2, _data.robotInfo2);
    getBeltData(_motionController1, _data.beltPos);
    emit update(&_data);
  }

 private:
  void getRobotData(RobotOperator *robot, MCOperator *controller,
                    robotData &data) {
    // TODO specify tcp
    VWSRobot::RobotPosition pose;
    int ret(-1);
    try {
      ret = robot->getRobotPosition(pose);
    } catch (const std::exception &e) {
      std::cerr << e.what() << '\n';
      ret = -1;
    }
    if (ret == 0) {
      data.robotPosition[0] = pose.pos[0];
      data.robotPosition[1] = pose.pos[1];
      data.robotPosition[2] = pose.pos[2];
      data.robotPosition[3] = pose.orient[0];
      data.robotPosition[4] = pose.orient[1];
      data.robotPosition[5] = pose.orient[2];
      data.robotPosition[6] = pose.orient[3];
    } else {
      data.robotPosition[0] = 0;
      data.robotPosition[1] = 0;
      data.robotPosition[2] = 0;
      data.robotPosition[3] = 0;
      data.robotPosition[4] = 0;
      data.robotPosition[5] = 0;
      data.robotPosition[6] = 0;
    }
    // extra axis
    float position(0);
    try {
      position = controller->getRealTimeEncoder()[0];
    } catch (const std::exception &e) {
      std::cerr << e.what() << '\n';
      position = 0;
    }

    data.extraAxisPosition = position;
  }

  void getBeltData(MCOperator *controller, float &data) {
    // belt
    float position(0);
    try {
      position = controller->getRealTimeEncoder()[1];
    } catch (const std::exception &e) {
      std::cerr << e.what() << '\n';
      position = 0;
    }

    data = position;
  }
};
