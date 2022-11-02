#include "robotoperator.h"

RobotOperator::RobotOperator(std::shared_ptr<Robot> robot)
{
    this->ip = robot->Ip;
    this->port = robot->Port.toUInt();
    this->name = robot->Name;
}

int RobotOperator::init()
{
    vwsRobot = std::make_shared<VWSRobot>();
    return vwsRobot->Init(ip.toStdString());
}

int RobotOperator::start()
{
    auto ret = vwsRobot->start();
    return ret;
}

void RobotOperator::close()
{
    if(vwsRobot==nullptr)
        return;
    auto ret = vwsRobot->close();
}

int RobotOperator::getState()
{
    if(vwsRobot==nullptr)
        return -1;
    auto ret = vwsRobot->state();
    return ret;
}

int RobotOperator::sendData(std::vector<RobotTask> taskData)
{
    auto ret = vwsRobot->sendData(taskData);
    return ret;
}

int RobotOperator::getRobotPosition(RobotPosition &data)
{
    auto ret = vwsRobot->getRobotPosition(data);
    return ret;
}

QString RobotOperator::getName()
{
    return name;
}

