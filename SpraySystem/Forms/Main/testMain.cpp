#include "testMain.h"

#include <QAbstractButton>
#include <QTime>
#include <QTimer>

#include "Data/StaticData.h"
#include "Forms/Devices/devicelist.h"
#include "Forms/Devices/mcmove.h"
#include "Forms/Login/loginform.h"
#include "Forms/Products/List/productlistform.h"
#include "Forms/Systems/systemeditform.h"
#include "Forms/Users/List/userlistform.h"
#include "Forms/global.h"
TestMain::TestMain() {
    isRun = false;
    vws::DataInit::Init();
    startDevices();
    QString msg;
    mainprocess = new MainProcess();
    std::cout << "开始...." << std::endl;
    mainprocess->Start(msg);
}

TestMain::~TestMain() {
}

void TestMain::startDevices() {
    QString msg;
    DeviceManager *deviceManager = DeviceManager::getInstance();
    // PLC
    auto plc = deviceManager->getPlc();
    auto retplc = plc->start();
    msg = retplc > 0 ? "PLC启动成功" : "PLC启动失败";

    // 运动控制器
    auto mc = deviceManager->getMC();
    auto retmc = mc->init();
#ifndef PLAN_FAKE_DATA
    retmc = mc->start();

    msg = retmc > 0 ? "运动控制器启动成功" : "运动控制器启动失败";
#endif
    // 机器人
    auto rbt = deviceManager->getRobot(0);
    auto retrbt = rbt->init();
    if (retrbt > 0) {
        std::cout << "robot1 init: " << retrbt << std::endl;
        retrbt = rbt->start();
        std::cout << "robot1 start: " << retrbt << std::endl;
    } else {
        msg = rbt->getName() + ", 启动失败";
    }

    // 相机
    auto camera1 = deviceManager->getCamera(0);

    auto ret = camera1->init();
    if (ret > 0) {
        std::cout << "camera1 init: " << ret << std::endl;
        ret = camera1->start();
        std::cout << "camera1 start: " << ret << std::endl;
    } else {
        msg = camera1->getName() + "， 启动失败";
    }

    auto camera2 = deviceManager->getCamera(1);
    ret = camera2->init();
    if (ret > 0) {
        std::cout << "camera2 init: " << ret << std::endl;
        ret = camera2->start();
        std::cout << "camera2 start: " << ret << std::endl;
    } else {
        msg = camera2->getName() + "， 启动失败";
    }

    /***设备状态监控***/
    devicemonitor = new DeviceMonitor();
    devicemonitorThread = new QThread();

    devicemonitor->moveToThread(devicemonitorThread);
    if (!devicemonitorThread->isRunning()) {
        devicemonitorThread->start();
    }
    connect(this, SIGNAL(startMonitorDevice_signal()), devicemonitor, SLOT(startMonitorDevice_slot()));
    // connect(devicemonitor, SIGNAL(deviceConnectError_signal(QString, int)), this, SLOT(deviceConnectError_slot(QString, int)));
    emit this->startMonitorDevice_signal();
    /***设备状态监控***/
}
