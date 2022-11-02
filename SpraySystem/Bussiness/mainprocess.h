#ifndef MAINPROCESS_H
#define MAINPROCESS_H
#include <vector>
#include <QQueue>
#include <QObject>
#include <QVariant>
#include <QThread>
#include <memory>

#include "Trajectory/trajectorycontext.h"
#include "Trajectory/trajectorygenerator.h"
#include "VWSRobot/VWSRobot.h"
#include "VWSCamera/VWSCamera.h"
#include "Device/devicemanager.h"
#include "trajectoryprocess.h"
#include "Util/Socket/qtsocketclient.h"
#include "Trajectory/trajectorygenerator.h"

using RobotTask = VWSRobot::RobotTask;
using RobotPosition = VWSRobot::RobotPosition;
using ImageData = VWSCamera::ImageData;
using TrajParam = TrajectoryGenerator::TrajParam;

//Q_DECLARE_METATYPE(TrajData)  //注册结构体
class MainProcess : public QObject
{
    Q_OBJECT
public:

    MainProcess();
    ~MainProcess();
    void Execute();

    void sendtorbt();
private:
    void VisionProcessing();
    void TrajectoryProcessing(bool range, bool camera);
private:
    QQueue<RobotTask> trajQueue;
    QQueue<std::vector<float>> mcQueue;

    TrajectoryProcess *trajProc;
    QThread *trajThread;
    PLCData plcdata;

    QtSocketClient *imgSocket;



private slots:
    void recevieData_Slot(QVariant data);
    void traj_Slot(QVariant varmc,QVariant varrbt);
    void imgData_Slot(QByteArray imgData);
    void getTrajParam_Slot();
signals:
    void begintraj_Singal(QVariant data);
};

#endif // MAINPROCESS_H
