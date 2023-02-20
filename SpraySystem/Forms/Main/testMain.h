#ifndef TestMain_H
#define TestMain_H

#include <memory>
// #include "Device/Robot/robot.h"
// #include "Device/Camera/camera.h"
// #include "Device/PLC/plc.h"
#include "Bussiness/mainprocess.h"
#include "DataAccess/Models/User/user.h"
#include "devicemonitor.h"

class TestMain : public QObject {
    Q_OBJECT
public:
    TestMain();
    ~TestMain();
signals:
    void startMonitorDevice_signal();

private:
    bool isRun;
    MainProcess::CameraCallbackData *camera1CallbackData;

    //    Robot *robot1;
    //    Robot *robot2;
    //    Camera *camera1;
    //    Camera *camera2;
    //    PLC *plc;

    DeviceMonitor *devicemonitor;  // 监听所有设备状态
    QThread *devicemonitorThread;
    MainProcess *mainprocess = nullptr;

    void startDevices();
};
#endif  // TestMain_H
