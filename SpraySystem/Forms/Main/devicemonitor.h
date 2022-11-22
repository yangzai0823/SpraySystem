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
    void stopWork();
public slots:
    void  startMonitorDevice_slot();
private:

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
