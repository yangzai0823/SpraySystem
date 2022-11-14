#include "mainprocess.h"
#include <Eigen/Core>
#include <Eigen/Geometry>
#include "VWSRobot/VWSRobot.h"
#include "Data/StaticData.h"

std::mutex MainProcess::_mutex;
void imgFunc_b(const VWSCamera::ImageData &data, void *pUser)
{
    MainProcess::CameraCallbackData *cameraCallbackData = (MainProcess::CameraCallbackData *)pUser;
    auto mainProcess = cameraCallbackData->mainProcess;
    auto cameraOperator = cameraCallbackData->camera;
    static int64_t q = data.RGB8PlanarImage.nTimeStamp;
    static int i = 0;
    std::cout << ++i << "  " << data.RGB8PlanarImage.nTimeStamp << "      " << data.RGB8PlanarImage.nTimeStamp - q << std::endl;
    q = data.RGB8PlanarImage.nTimeStamp;

    if (mainProcess->q_img.size() == 20)
    {
        for (int i = 0; i < 5; i++)
        {
            auto rm_img = mainProcess->q_img.dequeue();
            DeviceManager::getInstance()->getCamera(0)->deleteImage(rm_img);
        }
    }
    mainProcess->q_img.enqueue(data);
    VisionData v;

    if (cameraCallbackData->up_or_bottom == 0)
    {
        QVariant vData;
        vData.setValue(data);
        emit mainProcess->sendImgData_u(vData);
    }
    else
    {
        // std::cout << "获得下相机图像" << std::endl;
        QVariant vData;
        vData.setValue(data);
        emit mainProcess->sendImgData_b(vData);
    }
}

void MainProcess::Test(PLCData data)
{
    QVariant vData;
    vData.setValue(data);

    recevieData_Slot(vData);
}

void MainProcess::TestImg()
{
    // std::cout << "获得下相机图像" << std::endl;
    // ImageData data;
    // emit sendImgData_b(data);
}

void MainProcess::recevieData_Slot(QVariant data)
{
    plcdata = data.value<PLCData>();
    // bottomWork();
    ContextStateMachine::SMContext context_b;
    context_b.flag_laser = plcdata.flag_laser_b;
    context_b.flag_camera = plcdata.flag_camera_b;
    context_b.laserCouple1 = {plcdata.laser1, plcdata.laser2};

    QVariant vdata;
    vdata.setValue(context_b);
    emit sendPlcData_b(vdata);

    ContextStateMachine::SMContext context_u;
    context_u.flag_laser = plcdata.flag_laser_u;
    context_u.flag_camera = plcdata.flag_camera_u;
    context_u.laserCouple1 = {plcdata.laser3,plcdata.laser4};
    QVariant vdata_u;
    vdata_u.setValue(context_u);
    emit sendPlcData_u(vdata_u);
}

MainProcess::MainProcess()
{
    vws::DataInit::Init();

    DeviceManager *deviceManager = DeviceManager::getInstance();
    // PLC
    auto plc = deviceManager->getPlc();
    connect(plc, SIGNAL(recevieData_Signal(QVariant)), this, SLOT(recevieData_Slot(QVariant)));

    //运动控制器
    auto mc = deviceManager->getMC();
    connect(mc, SIGNAL(getTrajParam_Signal()), this, SLOT(getTrajParam_Slot()));

    //相机
    auto camera1 = deviceManager->getCamera(0);
    camera1CallbackData_b = new CameraCallbackData();
    camera1CallbackData_b->up_or_bottom = 1;
    camera1CallbackData_b->mainProcess = this;

    auto camera2 = deviceManager->getCamera(1);
    camera2CallbackData_u = new CameraCallbackData();
    camera2CallbackData_u->up_or_bottom = 0;
    camera2CallbackData_u->mainProcess = this;

   camera1->RegisterFrameCallBack(imgFunc_b, (void *)(camera1CallbackData_b));
    camera2->RegisterFrameCallBack(imgFunc_b,(void *)(camera2CallbackData_u));

    visionContext = new VisionContext();
    trajProc = new TrajectoryProcess();
    trajThread = new QThread;
    trajProc->moveToThread(trajThread);
    connect(this, SIGNAL(begintraj_Singal(MainProcess *)), trajProc, SLOT(begintraj_Slot(MainProcess *)));
    trajThread->start();

    connect(this, SIGNAL(sendTrajParam_Signal()), this, SLOT(getTrajParam_Slot()));

    sm_bottom = new ContextStateMachine();
    connect(this, SIGNAL(sendPlcData_b(QVariant)), sm_bottom, SLOT(sendPlcData_Slot(QVariant)));
    connect(this, SIGNAL(sendImgData_b(QVariant)), sm_bottom, SLOT(sendImgData_Slot(QVariant)));
    connect(sm_bottom, SIGNAL(beginVision_Singal(ContextStateMachine *, bool)), this, SLOT(beginVision_Slot(ContextStateMachine *, bool)));
    connect(this, SIGNAL(finishVision_Signal_b(bool)), sm_bottom, SLOT(finishVision_Slot(bool)));
    sm_bottom->Name = "Bottom";
    sm_bottom->start();

    sm_top = new ContextStateMachine();
    connect(this, SIGNAL(sendPlcData_u(QVariant)), sm_top, SLOT(sendPlcData_Slot(QVariant)));
    connect(this, SIGNAL(sendImgData_u(QVariant)), sm_top, SLOT(sendImgData_Slot(QVariant)));
    connect(sm_top, SIGNAL(beginVision_Singal(ContextStateMachine *, bool)), this, SLOT(beginVision_Slot(ContextStateMachine *, bool)));
    connect(this, SIGNAL(finishVision_Signal_u(bool)), sm_top, SLOT(finishVision_Slot(bool)));
    sm_top->Name = "Top";
    sm_top->start();
}

MainProcess::~MainProcess()
{
    trajThread->quit();
    trajThread->wait();
    delete trajThread;
    delete trajProc;

    delete camera1CallbackData_b;
    delete camera2CallbackData_u;

    //    delete signalProcess;

    delete sm_bottom;
}

std::vector<vws::PlanTaskInfo> *MainProcess::GetPlanTaskInfo(int upper_or_bottom)
{
    if (upper_or_bottom == 0)
    {
        return &qPlanTaskInfoBottom;
    }
    else
    {
        return &qPlanTaskInfoTop;
    }
}

float MainProcess::getChainSpeed() const
{
    return 80;
}

int64_t MainProcess::getChainEncoder() const
{
#ifndef PLAN_FAKE_DATA
    // todo:: 返回的编码器数值类型改为int64_t
    auto val = DeviceManager::getInstance()->getMC()->getRealTimeEncoder();
    std::cout << "encoder value: " << val[0] << ", " << val[1] << std::endl;
    return val[1];
#else
    static uint64_t en = 1e8;
    en -= 50;
    return en;
#endif
}

float MainProcess::getChainUnits() const
{
    return 1 / 0.4198727819755431f;
}

Eigen::VectorXd MainProcess::getRobotWaitPose() const
{
    Eigen::VectorXd val(6);
    val << 1.5, 0, 0, 0, 0, 0;
    std::cout << "getRobotWaitPose: " << val[0] << std::endl;
    return val;
}

bool MainProcess::getChainEncoderDir() const
{
    // TODO: 根据实际情况
    return false;
}

void MainProcess::getTrajParam_Slot()
{
    auto mc = DeviceManager::getInstance()->getMC();

    if (mcQueue.count() > 0)
    {
        auto param = mcQueue.dequeue();
        auto zeropoint = param[0];
        auto offset = param[1];
        // offset = -1;

        std::cout << "运动控制发送信息: " << zeropoint << std::endl;
        mc->sendTrajParam(zeropoint, offset);

        sendtorbt();
        mcRequest = false;
    }
    else
    {
        std::cout << "运动控制器请求数据，前队列为空" << std::endl;
        mcRequest = true;
    }
}

void MainProcess::beginVision_Slot(ContextStateMachine *sm, bool ishead)
{
    std::cout << "开始视觉处理" << std::endl;

    int index = 0;
    if (!ishead)
    {
        index = 1;
    }

    vws::VisionData visionData;
    visionContext->work(ishead ? sm->Context.img_head : sm->Context.img_trail, vws::handEyeMatrix, visionData);
    // DeviceManager::getInstance()->getCamera(0)->deleteImage(data.image[index]);

    vws::PlanTaskInfo planTaskInfo;
    planTaskInfo.diff = vws::diff;
    planTaskInfo.lx = 535;
    planTaskInfo.ly = vws::BoxLenght;
    planTaskInfo.lz = vws::BoxHeight;
    planTaskInfo.encoder = ishead ? sm->Context.encoder_img_head : sm->Context.encoder_img_trail;
    planTaskInfo.face = ishead ? 0 : 1;
    planTaskInfo.boxInfo = Eigen::Isometry3d::Identity();

    std::cout << "四元数: " << visionData.robotpose[3] << ", " << visionData.robotpose[4] << ", " << visionData.robotpose[5] << "," << visionData.robotpose[6] << std::endl;
    planTaskInfo.boxInfo.prerotate(Eigen::Quaterniond(visionData.robotpose[3], visionData.robotpose[4], visionData.robotpose[5], visionData.robotpose[6]));

    std::cout << "坐标： " << visionData.robotpose[0] << ", " << visionData.robotpose[1] << ", " << visionData.robotpose[2] << std::endl;
    planTaskInfo.boxInfo.pretranslate(Eigen::Vector3d(visionData.robotpose[0], visionData.robotpose[1], visionData.robotpose[2]));

    //根据状态机判断底层/顶层
    if (sm->Name == "Bottom")
    {
        std::cout << "顶层箱子参数进入队列" << std::endl;
        qPlanTaskInfoTop.push_back(planTaskInfo);
        emit finishVision_Signal_b(ishead);
    }
    else
    {
        std::cout << "底层箱子参数进入队列" << std::endl;
        qPlanTaskInfoBottom.push_back(planTaskInfo);
        emit finishVision_Signal_u(ishead);
    }
    std::cout << "视觉处理结束，触发轨迹规划信号" << std::endl;

    // emit begintraj_Singal(this);
}

void MainProcess::SetRobotTaskInfo(std::vector<float> mc_data, std::vector<RobotTask> robotTasks)
{
    std::cout << "规划结果写入队列" << std::endl;
    mcQueue.push_back(mc_data);
    trajQueue.push_back(robotTasks);

    if (mcRequest)
    {
        // getTrajParam_Slot();
        std::cout << "运动控制器请求过数据，但未发送" << std::endl;
        // emit sendTrajParam_Signal();
    }
}

void MainProcess::sendtorbt()
{
    static std::vector<VWSRobot::RobotTask> staticTask;

    auto rbt = DeviceManager::getInstance()->getRobot(0);
    rbt->start();
    std::vector<VWSRobot::RobotTask> rbtparam;
    std::cout << "机器人发送信息: " << std::endl;
    if (trajQueue.count() > 0)
    {
        rbtparam = trajQueue.dequeue();
        staticTask = rbtparam;
    }
    else
    {
        rbtparam = staticTask;
    }

    rbt->sendData(rbtparam);
    std::cout << "发送结束" << std::endl;
}
