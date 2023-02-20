#ifndef MAINPROCESS_H
#define MAINPROCESS_H
#include <vector>
#include <QQueue>
#include <QObject>
#include <QVariant>
#include <QThread>
#include <memory>

#include "Trajectory/trajectorygenerator.h"
#include "VWSRobot/VWSRobot.h"
#include "VWSCamera/VWSCamera.h"
#include "Device/devicemanager.h"
#include "trajectoryprocess.h"
#include "Util/Socket/qtsocketclient.h"
#include "Trajectory/trajectorygenerator.h"
#include "mutex"
#include "deque"
#include <QStateMachine>
#include "Data/SignalState.h"
#include "contextstatemachine.h"
#include "Util/threadsafevector.h"
using RobotTask = VWSRobot::RobotTask;
using RobotPosition = VWSRobot::RobotPosition;
using ImageData = VWSCamera::ImageData;

class MainProcess : public QObject
{
    Q_OBJECT
public:
    struct CameraCallbackData
    {
        std::shared_ptr<CameraOperator> camera;
        MainProcess *mainProcess;
        /* @brief 0：顶， 1： 底*/
        bool up_or_bottom = 0;
    };
    MainProcess();
    ~MainProcess();
    int Start(QString &msg);
    void Stop();

    /**
     * @brief Get the Plan Task Info object
     *
     * @param upper_or_bottom    0: 表示底层箱子， 1：表示上层
     * @return ThreadSafeVector<vws::PlanTaskInfo>*
     */
    ThreadSafeVector<vws::PlanTaskInfo> *GetPlanTaskInfo(int upper_or_bottom = 0);
    float getChainSpeed() const; // mm/s
    int64_t getChainEncoder() const;
    float getChainUnits() const;              //
    Eigen::VectorXd getRobotWaitPose() const; //
    bool getChainEncoderDir() const;          // true: 运行时编码器值增加，false ： 运行时编码器值减小

    void triggerTest() { emit begintraj_Singal(this); }

    void SetRobotTaskInfo(std::vector<float> mc_data, std::vector<RobotTask> robotTasks);

    void Test(PLCData data);
    void TestImg();

private:
    void TrajectoryProcessing(bool range, bool camera);

public:
    VisionContext *visionContext;
    PLCData plcdata;
    QQueue<ImageData> q_img;
    /** @brief 运行状态*/
    int State = -1;
#ifdef LOCALTEST
    /* @brief 下相机回调输入参数*/
    CameraCallbackData *camera1CallbackData_b = nullptr;
    /* @brief 上相机回调输入参数*/
    CameraCallbackData *camera2CallbackData_u = nullptr;
#endif
private:
    QQueue<std::vector<VWSRobot::RobotTask>> trajQueue;
    QQueue<std::vector<float>> mcQueue;
    std::vector<vws::PlanTaskInfo> qPlanTaskInfoTop;
    std::vector<vws::PlanTaskInfo> qPlanTaskInfoBottom;

    ThreadSafeVector<vws::PlanTaskInfo> safeQPlanTaskInfoTop;
    ThreadSafeVector<vws::PlanTaskInfo> safeQPlanTaskInfoBottom;

    TrajectoryProcess *trajProc;
    QThread *trajThread;
#ifndef LOCALTEST
    /* @brief 下相机回调输入参数*/
    CameraCallbackData *camera1CallbackData_b;
    /* @brief 上相机回调输入参数*/
    CameraCallbackData *camera2CallbackData_u;
#endif
    static std::mutex _mutex;
    /** @brief 规划结果如队列加锁*/
    static std::mutex _trajret_mutex;
    ContextStateMachine *sm_top;
    ContextStateMachine *sm_bottom;

    /* @brief 运动控制器请求*/
    bool mcRequest = false;
    uint8_t axisNum = 0;

    std::vector<VWSRobot::RobotTask> rbtFailTask;
    int nFail = 0;
    int nRbt = 0;
private slots:
    /**
     * @brief PLC槽
     * @param data
     */
    void recevieData_Slot(QVariant data);
    /**
     * @brief 运动控制器获取参数槽
     */
    void getTrajParam_Slot(quint8 axisNum);
    /** @brief 向机器人发送轨迹*/
    void sendToRBT_Slot(quint8 axisNum);
    void alarm_Slot();
    /**
     * @brief
     *
     * @param upper_or_bottom  0: 表示底层箱子， 1：表示上层
     */
    void begintraj_Slot(QVariant planTaskInfo, bool up_or_bttom);

signals:
    void begintraj_Singal(MainProcess *data);

    void sendPlcData_b(QVariant vData);
    void sendPlcData_u(QVariant vData);
    void sendImgData_b(QVariant vData);
    void sendImgData_u(QVariant vData);

    /* @brief 运动控制器发送数据*/
    void sendTrajParam_Signal(quint8);

    void alarm_Signal();
};

#endif // MAINPROCESS_H
