#include "robotoperator.h"
#include <unistd.h>

RobotOperator::RobotOperator(std::shared_ptr<Robot> robot)
{
    this->ip = robot->Ip;
    this->port = robot->Port.toUInt();
    this->name = robot->Name;
}

int RobotOperator::init()
{
    int ret = -1;
    vwsRobot = std::make_shared<VWSRobot>();
    for(int i=0;i<3;i++){
        ret = vwsRobot->Init(ip.toStdString());
        if(ret>0){
            break;
        }
        usleep(30);
    }
    return ret;
}

int RobotOperator::start()
{
    int ret = -1;
    for(int i=0;i<3;i++){
        ret = vwsRobot->start();
        if(ret>0){
            break;
        }
        usleep(30);
    }
    return ret;
}

void RobotOperator::close()
{
    if (vwsRobot == nullptr)
        return;
    auto ret = vwsRobot->close();
}

int RobotOperator::getState()
{
    if (vwsRobot == nullptr)
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
