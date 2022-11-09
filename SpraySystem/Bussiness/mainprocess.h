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
//#include "statemachinefactory.h"
#include <QStateMachine>
#include "Data/SignalState.h"


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

    // void sendtorbt();
    /**
     * @brief Get the Plan Task Info object
     * 
     * @param upper_or_bottom    0: 表示底层箱子， 1：表示上层
     * @return std::vector<vws::PlanTaskInfo>* 
     */
    std::vector<vws::PlanTaskInfo> * GetPlanTaskInfo(int upper_or_bottom = 0);
    float getChainSpeed() const; // mm/s
    int64_t getChainEncoder() const;
    float getChainUnits() const;        //
    Eigen::VectorXd getRobotWaitPose() const;     //
    bool getChainEncoderDir() const;        // true: 运行时编码器值增加，false ： 运行时编码器值减小

    void triggerTest() { emit begintraj_Singal(this); }

    void SetRobotTaskInfo(std::vector<float> mc_data,std::vector<RobotTask> robotTasks);

    void sendtorbt();
    
private:
/// @brief 
/// @param data 
/// @param upper_or_bottom   0: 表示底层箱子， 1：表示上层
/// @param ishead   0: 表示尾部， 1：表示头部

void VisionProcessing(vws::ProcessData data,bool upper_or_bottom=0, bool ishead = 1);

private:
    void TrajectoryProcessing(bool range, bool camera);


public:
    VisionContext *visionContext;
    MainData mainData;
     PLCData plcdata;
private:
    QQueue<std::vector<VWSRobot::RobotTask>> trajQueue;
    QQueue<std::vector<float>> mcQueue;
    std::vector<vws::PlanTaskInfo> qPlanTaskInfoTop;
    std::vector<vws::PlanTaskInfo> qPlanTaskInfoBottom;

    TrajectoryProcess *trajProc;
    QThread *trajThread;


    QtSocketClient *imgSocket;

    CameraCallbackData *camera1CallbackData;
    CameraCallbackData *camera2CallbackData;


    static std::mutex _mutex;

    // bool imgFlag = false;
private slots:
    /**
     * @brief PLC槽
     * @param data
     */
    void recevieData_Slot(QVariant data);

    // void imgData_Slot(QByteArray imgData);
    /**
     * @brief 运动控制器获取参数槽
     */
    void getTrajParam_Slot();
signals:
    void begintraj_Singal(MainProcess* data);

private:
//    StateMachineFactory *stateMachine_b;

     std::vector<float> staticSignal;
     bool mcRequest=false;
public:
     bool checkHeadImgAndSignal(vws::ProcessData &pData,bool flag_laser,bool imgFlag);
     bool checkTrailImgAndSignal(vws::ProcessData &pData,bool flag_camera,bool imgFlag);

     void bottomWork();
private:
     int64_t getTimeStamp();


     SignalState::State GetMachineState(QStateMachine *machine);
     void InitMachine_b();
private slots:
    void enteredWaitLaserSignal_b();
    void enteredProcessHeadImg_b();
    void enterdHeadProcessDone_b();
    void enteredProcessTrailImg_b();
    void enteredIDLE_b();
public:
    QStateMachine *machine_b;
    QState *stateIDLE_b;
    QState *waitLaserSignal_b;
    QState *processHeadImg_b;
    QState *headProcessDone_b;
    QState *processTrailImg_b;

    QSignalTransition *tranWaitSignal;
    QSignalTransition *tranProcessHeadImg;
    QSignalTransition *tranHeadProcessDone;
    QSignalTransition *tranProcessTrailImg;
    QSignalTransition *tranIDLE;

    QStateMachine *getMachine() const;
    void setMachine(QStateMachine *value);

signals:
    void cameraSignalOn_b();
    void laserSignalOnAndImgReady_b();
    void headDone_b();
    void cameraSignalOffAndImgReady_b();
    void trailDone_b();

    void sendTrajParam_Signal();
};

#endif // MAINPROCESS_H
