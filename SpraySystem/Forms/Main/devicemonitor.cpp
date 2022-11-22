#include "devicemonitor.h"
#include <iostream>
#include <QDateTime>
#include <QDebug>
#include "Device/devicemanager.h"
#include "Device/Camera/cameraoperator.h"
#include <memory>
#include "Data/StaticData.h"
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
      state = DeviceManager::getInstance()->getCamera(vws::Camera_top)->getState();
       if(state!=camera1State){
           camera1State = state;
           emit deviceConnectError_signal(vws::Camera_top,state);
       }
       state = DeviceManager::getInstance()->getCamera(vws::Camera_bottom)->getState();
       if(state!=camera2State){
           camera2State = state;
           emit deviceConnectError_signal(vws::Camera_bottom,state);
       }

       state = DeviceManager::getInstance()->getRobot(vws::Robot1)->getState();
        if(state!=robot1State){
            robot1State = state;
            emit deviceConnectError_signal(vws::Robot1,state);
        }
       state = DeviceManager::getInstance()->getRobot(vws::Robot2)->getState();
        if(state!=robot2State){
            robot2State = state;
            emit deviceConnectError_signal(vws::Robot2,state);
        }
        // QString strTime = QDateTime::currentDateTime().toString("hh:mm:ss");
        // qDebug() << strTime.toUtf8().data()<<"，检查设备状态";
        QThread::sleep(10);
    }
}
