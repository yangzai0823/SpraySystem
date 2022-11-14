#include "devicemonitor.h"
#include <iostream>
#include <QDateTime>
#include <QDebug>


//DeviceMonitor::DeviceMonitor(Robot* robot1, Robot* robot2, Camera* camera1, Camera* camera2, PLC* plc)
//{
//    this->robot1 = robot1;
//    this->robot2 = robot2;
//    this->camera1 = camera1;
//    this->camera2 = camera2;
//    this->plc = plc;
//}

void DeviceMonitor::stopWork()
{
    stop=true;
}

void DeviceMonitor::startMonitorDevice_slot()
{
    while (!stop) {
        //QMutexLocker locker(&lock);

        //检查设备状态
        int state=-1;
        //robot1
//        state = robot1->state();
//        if(state!=robot1State){
//            robot1State = state;
//            emit deviceConnectError_signal("robot1",state);
//        }
//        //robot2
//        state = robot2->state();
//        if(state!=robot2State){
//            robot2State = state;
//            emit deviceConnectError_signal("robot2",state);
//        }

        QString strTime = QDateTime::currentDateTime().toString("hh:mm:ss");
        qDebug() << strTime.toUtf8().data()<<"，检查设备状态";
        QThread::sleep(10);
    }
}
