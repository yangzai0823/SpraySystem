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
#include "Bussiness/signalprocess.h"
#include "mutex"
#include "deque"

using RobotTask = VWSRobot::RobotTask;
using RobotPosition = VWSRobot::RobotPosition;
using ImageData = VWSCamera::ImageData;
using TrajParam = vws::TrajParam;
using MainData = vws::MainData;
//Q_DECLARE_METATYPE(TrajData)  //注册结构体
class MainProcess : public QObject
{
    Q_OBJECT
public:
    struct CameraCallbackData{
        std::shared_ptr<CameraOperator> camera;
        MainProcess *mainProcess;
    };
    MainProcess();
    ~MainProcess();
    void Execute();

    void sendtorbt();
    /**
     * @brief Get the Plan Task Info object
     * 
     * @param upper_or_bottom    0: 表示底层箱子， 1：表示上层
     * @return std::deque<vws::PlanTaskInfo>* 
     */
    std::deque<vws::PlanTaskInfo> * GetPlanTaskInfo(int upper_or_bottom = 0);
    float getChainSpeed(); // mm/s
    uint64_t getChainEncoder();

private:
    void VisionProcessing();
    void TrajectoryProcessing(bool range, bool camera);
    void UpdateCurrentData();
public:
    ImageData currentData;
    VisionData visionData;
    VisionContext *visionContext;
    MainData mainData;
private:
    QQueue<RobotTask> trajQueue;
    QQueue<std::vector<float>> mcQueue;
    std::deque<vws::PlanTaskInfo> qPlanTaskInfoTop;
    std::deque<vws::PlanTaskInfo> qPlanTaskInfoBottom;

    TrajectoryProcess *trajProc;
    QThread *trajThread;
    PLCData plcdata;

    QtSocketClient *imgSocket;
    int64_t cameraStamp;

    CameraCallbackData *camera1CallbackData;
    CameraCallbackData *camera2CallbackData;

    SignalProcess *signalProcess;

    static std::mutex _mutex;
private slots:
    /**
     * @brief PLC槽
     * @param data
     */
    void recevieData_Slot(QVariant data);
    /**
     * @brief 轨迹规划反馈槽
     * @param varmc
     * @param varrbt
     */
    void traj_Slot(QVariant varmc,QVariant varrbt);
    void imgData_Slot(QByteArray imgData);
    /**
     * @brief 运动控制器获取参数槽
     */
    void getTrajParam_Slot();
signals:
    void begintraj_Singal(QVariant data);
};

#endif // MAINPROCESS_H
