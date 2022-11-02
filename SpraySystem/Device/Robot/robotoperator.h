#ifndef ROBOTOPERATOR_H
#define ROBOTOPERATOR_H

#include <QObject>
#include <memory>
#include <DataAccess/Models/Robot/robot.h>
#include "Device/deviceoperator.h"
#include "VWSRobot/VWSRobot.h"
using RobotTask = VWSRobot::RobotTask;
using RobotPosition = VWSRobot::RobotPosition;

class RobotOperator : public DeviceOperator
{
public:
    RobotOperator(std::shared_ptr<Robot> robot);
    int init();
    int start();
    void close();
    int getState();
     int sendData(std::vector<RobotTask> taskData);
     int getRobotPosition(RobotPosition & data);
     QString getName();
 private:
    std::shared_ptr<VWSRobot> vwsRobot;
};

#endif // ROBOTOPERATOR_H
