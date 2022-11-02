#ifndef DEVICEMONITOR_H
#define DEVICEMONITOR_H

#include <QObject>
#include <QThread>
#include <QMutex>
//#include "Device/Robot/robot.h"
//#include "Device/Camera/camera.h"
//#include "Device/PLC/plc.h"

class DeviceMonitor : public QObject
{
    Q_OBJECT
public:
//    explicit DeviceMonitor(Robot* robot1, Robot* robot2, Camera* camera1, Camera* camera2, PLC* plc);

    void stopWork();
public slots:
    void  startMonitorDevice_slot();
private:
//    Robot *robot1;
//    Robot *robot2;
//    Camera *camera1;
//    Camera *camera2;
//    PLC *plc;

    int robot1State =-1;
    int robot2State = -1;
    int camera1State = -1;
    int camera2State = -1;
    int plcState=-1;

    bool stop=false;
    QMutex lock;
signals:
    void deviceConnectError_signal(QString device, int state);
};

#endif // DEVICEMONITOR_H
