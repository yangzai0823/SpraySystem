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
#include "mutex"
#include "deque"
#include <QStateMachine>
#include "Data/SignalState.h"
#include "contextstatemachine.h"

using RobotTask = VWSRobot::RobotTask;
using RobotPosition = VWSRobot::RobotPosition;
using ImageData = VWSCamera::ImageData;

// Q_DECLARE_METATYPE(TrajData)  //注册结构体
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
    void Execute();

    // void sendtorbt();
    /**
     * @brief Get the Plan Task Info object
     *
     * @param upper_or_bottom    0: 表示底层箱子， 1：表示上层
     * @return std::vector<vws::PlanTaskInfo>*
     */
    std::vector<vws::PlanTaskInfo> *GetPlanTaskInfo(int upper_or_bottom = 0);
    float getChainSpeed() const; // mm/s
    int64_t getChainEncoder() const;
    float getChainUnits() const;              //
    Eigen::VectorXd getRobotWaitPose() const; //
    bool getChainEncoderDir() const;          // true: 运行时编码器值增加，false ： 运行时编码器值减小

    void triggerTest() { emit begintraj_Singal(this); }

    void SetRobotTaskInfo(std::vector<float> mc_data, std::vector<RobotTask> robotTasks);

    void sendtorbt();
    void Test(PLCData data);
    void TestImg();

private:
    void TrajectoryProcessing(bool range, bool camera);

public:
    VisionContext *visionContext;
    PLCData plcdata;
    QQueue<ImageData> q_img;

private:
    QQueue<std::vector<VWSRobot::RobotTask>> trajQueue;
    QQueue<std::vector<float>> mcQueue;
    std::vector<vws::PlanTaskInfo> qPlanTaskInfoTop;
    std::vector<vws::PlanTaskInfo> qPlanTaskInfoBottom;

    TrajectoryProcess *trajProc;
    QThread *trajThread;

    /* @brief 下相机回调输入参数*/
    CameraCallbackData *camera1CallbackData_b;
    /* @brief 上相机回调输入参数*/
    CameraCallbackData *camera2CallbackData_u;

    static std::mutex _mutex;
    ContextStateMachine *sm_top;
    ContextStateMachine *sm_bottom;

    /* @brief 运动控制器请求*/
    bool mcRequest = false;

private slots:
    /**
     * @brief PLC槽
     * @param data
     */
    void recevieData_Slot(QVariant data);
    /**
     * @brief 运动控制器获取参数槽
     */
    void getTrajParam_Slot();

    /**
     * @brief 开始视觉处理，
     */
    void beginVision_Slot(ContextStateMachine *sm, bool ishead);

     /**
     * @brief 开始视觉处理，头部
     */
    void beginVision_head_Slot(ContextStateMachine *sm);

    /**
     * @brief 结束头部处理，使用固定长度计算箱体中心
    */
    void finsihVision_head_Slot(ContextStateMachine *sm);

     /**
     * @brief 开始视觉处理，尾部部
     */
    void beginVision_trail_Slot(ContextStateMachine *sm);


signals:
    void begintraj_Singal(MainProcess *data);

    void sendPlcData_b(QVariant vData);
    void sendPlcData_u(QVariant vData);
    void sendImgData_b(QVariant vData);
    void sendImgData_u(QVariant vData);
    void finishVision_Signal_b(bool ishead);
    void finishVision_Signal_u(bool ishead);

    /* @brief 运动控制器发送数据*/
    void sendTrajParam_Signal();
};

#endif // MAINPROCESS_H
