#ifndef DEVICEMANAGER_H
#define DEVICEMANAGER_H

#include <QObject>
#include <memory>
#include <mutex>

#include "Camera/cameraoperator.h"
#include "Robot/robotoperator.h"
#include "PLC/plcoperator.h"
#include "MotionController/mcoperator.h"
#include "DataAccess/Models/Camera/camera.h"
#include "DataAccess/Models/Robot/robot.h"
#include "DataAccess/Models/PLC/plc.h"
#include "DataAccess/Models/MotionController/motioncontroller.h"

class DeviceManager
{
public:
    static DeviceManager* getInstance();

    ~DeviceManager();
    int addCamera(QString name,QString ip,int port);
    void saveCamera();
    QList<std::shared_ptr<CameraOperator>> cameraList();
    std::shared_ptr<CameraOperator> getCamera(int index);
    std::shared_ptr<CameraOperator> getCamera(QString name);

    int addRobot(QString name,QString ip,int port);
    void saveRobot();
    QList<std::shared_ptr<RobotOperator>> robotList();
    std::shared_ptr<RobotOperator> getRobot(int index);
    std::shared_ptr<RobotOperator> getRobot(QString name);

    int addPLC(QString name,QString ip,int port);
    void savePLC();
    PLCOperator* getPlc();

    int addMC(QString name,QString ip,int port);
    void saveMC();
    QList<MCOperator*> getMCList();
    MCOperator* getMC(int index = 0);
    MCOperator* getMC(QString name);

    void disconnectDevices();
private:
    static std::mutex _mutex;
    static DeviceManager *deviceManager;

    DeviceManager();
    QList<std::shared_ptr<CameraOperator>> cameras;
    QList<std::shared_ptr<RobotOperator>> robots;
    QList<MCOperator*> mcs;
    PLCOperator* plcoperator = nullptr;
    MCOperator* mcoperator = nullptr;

};

#endif // DEVICEMANAGER_H
