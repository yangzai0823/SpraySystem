#include "devicemanager.h"
#include "DataAccess/Repositories/Camera/camerarepository.h"
#include "DataAccess/Repositories/Robot/robotrepository.h"
#include "DataAccess/Repositories/PLC/plcrepository.h"
#include "DataAccess/Repositories/MotionController/motioncontrollerrepository.h"

QMutex DeviceManager::mutex;
DeviceManager *DeviceManager::deviceManager = NULL;

DeviceManager::DeviceManager()
{
    auto camlist = CameraRepository::getInstance()->list();
    foreach(auto item, camlist){
        std::shared_ptr<CameraOperator> cam = std::make_shared<CameraOperator>(item);
        cameras.append(cam);
    }

    auto rbtlist = RobotRepository::getInstance()->list();
    foreach(auto item, rbtlist){
        std::shared_ptr<RobotOperator> rbt = std::make_shared<RobotOperator>(item);
        robots.append(rbt);
    }
}

DeviceManager* DeviceManager::getInstance()
{
    if(deviceManager == NULL){
        mutex.lock();
        if(deviceManager==NULL){
            deviceManager = new DeviceManager();
        }
    }

    return deviceManager;
}


DeviceManager::~DeviceManager()
{
    foreach(auto item, cameras){
        item->close();
    }
    foreach(auto item, robots){
        item->close();
    }

    plcoperator->close();
    delete  plcoperator;
}

#pragma region 相机{

int DeviceManager::addCamera(QString name,QString ip,int port)
{
    std::shared_ptr<Camera> camera = std::make_shared<Camera>();
    camera->Name = name;
    camera->Ip=ip;
    camera->Port = QString::number(port);
    auto camList= CameraRepository::getInstance()->list();
    camList.append(camera);
    CameraRepository::getInstance()->save(camList);

   std::shared_ptr<CameraOperator> cam = std::make_shared<CameraOperator>(camera);
   //cam->connect();
   cameras.append(cam);
}

void DeviceManager::saveCamera()
{
    auto camlist = CameraRepository::getInstance()->list();

    bool update;
    foreach(auto cam, cameras){
        if(cam->updated){
            update = true;
            cam->updated=false;
            foreach(auto item, camlist){
                if(item->Name == cam->getName()){
                    item->Ip = cam->getIP();
                    item->Port =QString::number( cam->getPort());
                    break;
                }
            }
        }
    }
    if(update){
        CameraRepository::getInstance()->save(camlist);
    }
}

QList<std::shared_ptr<CameraOperator> > DeviceManager::cameraList()
{
    return cameras;
}

std::shared_ptr<CameraOperator> DeviceManager::getCamera(int index)
{
    if(cameras.count()>index+1){
        return cameras.at(index);
    }
    else{
        return nullptr;
    }
}

std::shared_ptr<CameraOperator> DeviceManager::getCamera(QString name)
{
    foreach (auto item, cameras) {
        if(item->getName() == name){
            return item;
        }
    }

    return nullptr;
}
#pragma endregion}

#pragma region 机器人{
int DeviceManager::addRobot(QString name,QString ip,int port)
{
    std::shared_ptr<Robot> robot = std::make_shared<Robot>();
    robot->Name=name;
    robot->Ip = ip;
    robot->Port =QString::number(port);
    auto rbtlist = RobotRepository::getInstance()->list();
    rbtlist.append(robot);
    RobotRepository::getInstance()->save(rbtlist);

    std::shared_ptr<RobotOperator> rbt = std::make_shared<RobotOperator>(robot);
    //rbt->connect();
    robots.append(rbt);
}

void DeviceManager::saveRobot()
{
    auto rbtlist = RobotRepository::getInstance()->list();

    bool update;
    foreach(auto rbt, robots){
        if(rbt->updated){
            update = true;
            rbt->updated=false;
            foreach(auto item, rbtlist){
                if(item->Name == rbt->getName()){
                    item->Ip = rbt->getIP();
                    item->Port =QString::number( rbt->getPort());
                    break;
                }
            }
        }
    }
    if(update){
        RobotRepository::getInstance()->save(rbtlist);
    }
}

QList<std::shared_ptr<RobotOperator> > DeviceManager::robotList()
{
    return robots;
}

std::shared_ptr<RobotOperator> DeviceManager::getRobot(int index)
{
    if(robots.count()>index+1){
        return robots.at(index);
    }
    else{
        return nullptr;
    }
}

std::shared_ptr<RobotOperator> DeviceManager::getRobot(QString name)
{
    foreach (auto item, robots) {
        if(item->getName() == name){
            return item;
        }
    }

    return nullptr;
}

#pragma endregion}


#pragma region PLC{
int DeviceManager::addPLC(QString name,QString ip,int port)
{
    std::shared_ptr<PLC> plc = std::make_shared<PLC>();
    plc->Name=name;
    plc->Ip = ip;
    plc->Port =QString::number(port);

    PLCRepository::getInstance()->update(plc);

    plcoperator = new PLCOperator(plc);//std::make_shared<PLCOperator>(plc);

}

void DeviceManager::savePLC()
{
    auto plc = PLCRepository::getInstance()->query();
    if(plcoperator->updated){
        plc->Ip= plcoperator->getIP();
        plc->Port =QString::number( plcoperator->getPort());
        PLCRepository::getInstance()->update(plc);
    }
}

PLCOperator* DeviceManager::getPlc()
{
    if(plcoperator==nullptr){
        std::shared_ptr<PLC> plc = PLCRepository::getInstance()->query();
        plcoperator = new PLCOperator(plc);
//        plcoperator->init();
    }
    return this->plcoperator;
}





#pragma endregion}


#pragma region 运动控制器{
int DeviceManager::addMC(QString name, QString ip, int port)
{
    std::shared_ptr<MotionController> plc = std::make_shared<MotionController>();
    plc->Name=name;
    plc->Ip = ip;
    plc->Port =QString::number(port);

    MotionControllerRepository::getInstance()->update(plc);

//    plcoperator = new MCOperator(plc);//std::make_shared<PLCOperator>(plc);

}

void DeviceManager::saveMC()
{
    auto mc = MotionControllerRepository::getInstance()->query();
    if(mcoperator->updated){
        mc->Ip= mcoperator->getIP();
        mc->Port =QString::number( mcoperator->getPort());
        MotionControllerRepository::getInstance()->update(mc);
    }
}

MCOperator *DeviceManager::getMC()
{
    if(mcoperator==nullptr){
        std::shared_ptr<MotionController> mc = MotionControllerRepository::getInstance()->query();
        mcoperator = new MCOperator(mc);
        mcoperator->init();
    }
    return mcoperator;
}

#pragma endregion}

void DeviceManager::disconnectDevices()
{
    foreach(auto item, robots){
        item->close();
    }

    foreach(auto item, cameras){
        item->close();
    }

    plcoperator->close();
}

