#include "mainprocess.h"
#include <Eigen/Core>
#include <Eigen/Geometry>
#include "VWSRobot/VWSRobot.h"
#include "Data/StaticData.h"

std::mutex MainProcess::_mutex;
int saveToFile(std::string fileName,const VWSCamera::ImageData &data){
    std::string ply = fileName+".ply";
    std::ofstream outFile;
    //打开文件
    outFile.open(ply);
    int nPointNum = data.PointCloudImage.nDataLen / (sizeof(float) * 3);
    float* pSrcValue = (float*)(data.PointCloudImage.pData);
    outFile<<"ply"<<std::endl;
    outFile << "format ascii 1.0" << std::endl;
    outFile << "comment author:hik-robot" << std::endl;
    outFile << "element vertex "<< nPointNum << std::endl;
    outFile << "property float x" << std::endl;
    outFile << "property float y" << std::endl;
    outFile << "property float z" << std::endl;
    outFile << "end_header" << std::endl;
    for (int nPntIndex = 0; nPntIndex < nPointNum; ++nPntIndex) {
        outFile<<pSrcValue[nPntIndex * 3 + 0]<<" "<<pSrcValue[nPntIndex * 3 + 1]<<" "<<pSrcValue[nPntIndex * 3 + 2]<<std::endl;
    }
    outFile.close();
}


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
         std::cout << " 获得上相机图像" << std::endl;

        QVariant vData;
        vData.setValue(data);
        emit mainProcess->sendImgData_u(vData);
    }
    else
    {
        std::cout<< " 获得下相机图像" << std::endl;
        QVariant vData;
        vData.setValue(data);
        emit mainProcess->sendImgData_b(vData);
    }
    // std::cout<<"保存"<<std::endl;
    // static int index = 0;
    // saveToFile("/home/vws/Demo/0/"+std::to_string(index),data);
    // index++;
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
    context_b.laserCouple1 = {plcdata.laser_bottom_head, plcdata.laser_bottom_behind};

    QVariant vdata;
    vdata.setValue(context_b);
    emit sendPlcData_b(vdata);

    ContextStateMachine::SMContext context_u;
    context_u.flag_laser = plcdata.flag_laser_u;
    context_u.flag_camera = plcdata.flag_camera_u;
    context_u.laserCouple1 = {plcdata.laser_up_head,plcdata.laser_up_behind};
    QVariant vdata_u;
    vdata_u.setValue(context_u);
    emit sendPlcData_u(vdata_u);
}

MainProcess::MainProcess()
{
    // vws::DataInit::Init();

    DeviceManager *deviceManager = DeviceManager::getInstance();
    // PLC
    auto plc = deviceManager->getPlc();
    connect(plc, SIGNAL(recevieData_Signal(QVariant)), this, SLOT(recevieData_Slot(QVariant)));

    //运动控制器
    auto mc = deviceManager->getMC();
    connect(mc, SIGNAL(getTrajParam_Signal()), this, SLOT(getTrajParam_Slot()));
    mc->startReceive();

    //相机
    auto camera1 = deviceManager->getCamera(vws::Camera_bottom);
    camera1CallbackData_b = new CameraCallbackData();
    camera1CallbackData_b->up_or_bottom = 1;
    camera1CallbackData_b->mainProcess = this;

    auto camera2 = deviceManager->getCamera(vws::Camera_top);
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
   
    connect(sm_bottom, SIGNAL(beginVision_Head_Signal(ContextStateMachine *)), this, SLOT(beginVision_head_Slot(ContextStateMachine *)));
    connect(sm_bottom,SIGNAL(finishVision_Head_Signal(ContextStateMachine *)),this,SLOT(finsihVision_head_Slot(ContextStateMachine *)));
    connect(sm_bottom,SIGNAL(beginVision_Trail_Signal(ContextStateMachine *)),this,SLOT(beginVision_trail_Slot(ContextStateMachine *)));
    
    connect(this, SIGNAL(finishVision_Signal_b(bool)), sm_bottom, SLOT(finishVision_Slot(bool)));
    sm_bottom->Name = "Bottom";
    sm_bottom->Context.index = 0;
    sm_bottom->Context.visionData.top_or_bottom = 1; 
    sm_bottom->start();

    sm_top = new ContextStateMachine();
    connect(this, SIGNAL(sendPlcData_u(QVariant)), sm_top, SLOT(sendPlcData_Slot(QVariant)));
    connect(this, SIGNAL(sendImgData_u(QVariant)), sm_top, SLOT(sendImgData_Slot(QVariant)));
    connect(sm_top, SIGNAL(beginVision_Singal(ContextStateMachine *, bool)), this, SLOT(beginVision_Slot(ContextStateMachine *, bool)));
    
    connect(sm_top, SIGNAL(beginVision_Head_Signal(ContextStateMachine *)), this, SLOT(beginVision_head_Slot(ContextStateMachine *)));
    connect(sm_top,SIGNAL(finishVision_Head_Signal(ContextStateMachine *)),this,SLOT(finsihVision_head_Slot(ContextStateMachine *)));
    connect(sm_top,SIGNAL(beginVision_Trail_Signal(ContextStateMachine *)),this,SLOT(beginVision_trail_Slot(ContextStateMachine *)));
    

    connect(this, SIGNAL(finishVision_Signal_u(bool)), sm_top, SLOT(finishVision_Slot(bool)));
    sm_top->Name = "Top";
    sm_top->Context.index = 1;
    sm_top->Context.visionData.top_or_bottom = 0;
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
    en -= 200;
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
    val << 14.8/180.0*M_PI, -55 / 180.0 * M_PI, -33.57 / 180.0 * M_PI,145.6/180.0*M_PI,-105/180.0*M_PI, -60.7/180.0*M_PI;
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
    // return;
    auto mc = DeviceManager::getInstance()->getMC();

    if (mcQueue.count() > 0)
    {
        std::cout<<"运动控制器队列: "<<std::endl;
        for(int i=0;i<mcQueue.length();i++){
            auto tmp = mcQueue.at(i);
            std::cout<<std::to_string(tmp[0])<<", "<<std::endl;
        }

        auto param = mcQueue.dequeue();
        auto zeropoint = param[0];
        auto offset = param[1];
        // offset = -1;

        if(sendtorbt())
        {

            std::cout << "运动控制发送信息: " << zeropoint << std::endl;
            mc->sendTrajParam(zeropoint, offset);

            mcRequest = false;
        }
    }
    else
    {
        std::cout << "运动控制器请求数据，前队列为空" << std::endl;
        mcRequest = true;
    }
}

void MainProcess::beginVision_Slot(ContextStateMachine *sm, bool ishead)
{
    #ifdef YASKAWA_ZERO_OFFSET
    double zero_offset = 600;
    #else
    double zero_offset = 0;
    #endif
    std::string strheadortrail = ishead?"头部":"尾部";
    
    std::cout << "开始"<<strheadortrail<<"视觉处理" << std::endl;

    int index = 0;
    if (!ishead)
    {
        index = 1;
    }

    auto handEyeMatrix_ub = sm->Name=="Bottom"?vws::handEyeMatrix_b_rbt1:vws::handEyeMatrix_b_rbt1;
 
    vws::VisionData visionData;
    visionContext->work(ishead ? sm->Context.img_head : sm->Context.img_trail
    , sm->Context.visionData);
    // DeviceManager::getInstance()->getCamera(0)->deleteImage(data.image[index]);

    vws::PlanTaskInfo planTaskInfo;
    planTaskInfo.diff = vws::diff;
    planTaskInfo.lx = 535;
    planTaskInfo.ly = vws::BoxLenght;
    planTaskInfo.lz = vws::BoxHeight;
    planTaskInfo.encoder = ishead ? sm->Context.encoder_img_head : sm->Context.encoder_img_trail;
    planTaskInfo.face = ishead ? 0 : 1;
    planTaskInfo.boxInfo = Eigen::Isometry3d::Identity();

    std::cout << "四元数: " << sm->Context.visionData.robotpose[3] << ", " << sm->Context.visionData.robotpose[4] << ", " << sm->Context.visionData.robotpose[5] << "," << sm->Context.visionData.robotpose[6] << std::endl;
    planTaskInfo.boxInfo.prerotate(Eigen::Quaterniond(sm->Context.visionData.robotpose[3], sm->Context.visionData.robotpose[4], sm->Context.visionData.robotpose[5], sm->Context.visionData.robotpose[6]));

    std::cout << "坐标： " << sm->Context.visionData.robotpose[0] << ", " << sm->Context.visionData.robotpose[1] << ", " << sm->Context.visionData.robotpose[2] << std::endl;
    planTaskInfo.boxInfo.pretranslate(Eigen::Vector3d(sm->Context.visionData.robotpose[0], sm->Context.visionData.robotpose[1], sm->Context.visionData.robotpose[2]+zero_offset));

    //根据状态机判断底层/顶层
    if (sm->Name == "Bottom")
    {
        std::cout << "底层箱子"<<strheadortrail<<"参数进入队列" << std::endl;
        planTaskInfo.isup = false;
        qPlanTaskInfoBottom.push_back(planTaskInfo);
        emit finishVision_Signal_b(ishead);
    }
    else
    {
        std::cout << "顶层箱子"<<strheadortrail<<"参数进入队列" << std::endl;
        planTaskInfo.isup = true;
        qPlanTaskInfoTop.push_back(planTaskInfo);
        emit finishVision_Signal_u(ishead);
    }
    std::cout << "视觉处理结束，触发轨迹规划信号" << std::endl;

    emit begintraj_Singal(this);
}

void MainProcess::beginVision_head_Slot(ContextStateMachine *sm)
{
    std::cout << "头部开始视觉处理" << std::endl;

    visionContext->work_head(sm->Context.img_head
    , sm->Context.laserCouple1
    ,  sm->Context.visionData);

     //根据状态机判断底层/顶层, 头部处理结束
    if (sm->Name == "Bottom")
    {
        emit finishVision_Signal_b(true);
    }
    else
    {
        emit finishVision_Signal_u(true);
    }

}

void MainProcess::beginVision_trail_Slot(ContextStateMachine *sm)
{
    #ifdef YASKAWA_ZERO_OFFSET
    double zero_offset = 600;
    #else
    double zero_offset = 0;
    #endif

   std::cout << "尾部开始视觉处理" << std::endl;

    visionContext->work_trail(sm->Context.img_trail
    , sm->Context.laserCouple1
    ,  sm->Context.visionData);

    if(!sm->Context.visionData.head_done){
        vws::PlanTaskInfo planTaskInfo_head;
        planTaskInfo_head.diff = vws::diff;
        planTaskInfo_head.lx = sm->Context.visionData.width;
        planTaskInfo_head.ly = sm->Context.visionData.length;
        planTaskInfo_head.lz = sm->Context.visionData.height;
        planTaskInfo_head.encoder =  sm->Context.encoder_img_head;
        planTaskInfo_head.face =  0;
        planTaskInfo_head.boxInfo = Eigen::Isometry3d::Identity();

        std::cout << sm->Name.toStdString()<<"， 头部， Plan Task, 编码器数值："<<std::to_string(planTaskInfo_head.encoder)<<std::endl;
        std::cout << sm->Name.toStdString()<< "， 头部四元数: " << sm->Context.visionData.robotpose_head[3] << ", " << sm->Context.visionData.robotpose_head[4] << ", " << sm->Context.visionData.robotpose_head[5] << "," << sm->Context.visionData.robotpose_head[6] << std::endl;
        planTaskInfo_head.boxInfo.prerotate(Eigen::Quaterniond(sm->Context.visionData.robotpose_head[3], sm->Context.visionData.robotpose_head[4], sm->Context.visionData.robotpose_head[5], sm->Context.visionData.robotpose_head[6]));
        std::cout << sm->Name.toStdString() << "， 头部坐标： " << sm->Context.visionData.robotpose_head[0] << ", " << sm->Context.visionData.robotpose_head[1] << ", " << sm->Context.visionData.robotpose_head[2] << std::endl;
        planTaskInfo_head.boxInfo.pretranslate(Eigen::Vector3d(sm->Context.visionData.robotpose_head[0], sm->Context.visionData.robotpose_head[1], sm->Context.visionData.robotpose_head[2]+zero_offset));

        //根据状态机判断底层/顶层
        if (sm->Name == "Bottom")
        {
            std::cout << "底层箱子头部参数进入队列" << std::endl;
            planTaskInfo_head.isup = false;
            qPlanTaskInfoBottom.push_back(planTaskInfo_head);
        }
        else
        {
            std::cout << "顶层箱子头部被参数进入队列" << std::endl;
            planTaskInfo_head.isup = true;
            qPlanTaskInfoTop.push_back(planTaskInfo_head);
        }
    }


    //参数入规划队列, 头部如果未入队列，将头部也入队列
    vws::PlanTaskInfo planTaskInfo;
    planTaskInfo.diff = vws::diff;
    planTaskInfo.lx = sm->Context.visionData.width;
    planTaskInfo.ly = sm->Context.visionData.length;
    planTaskInfo.lz = sm->Context.visionData.height;
    planTaskInfo.encoder =  sm->Context.encoder_img_trail;
    planTaskInfo.face =  1;
    planTaskInfo.boxInfo = Eigen::Isometry3d::Identity();

    std::cout << sm->Name.toStdString()<<"， 尾部， Plan Task, 编码器数值："<<std::to_string(planTaskInfo.encoder)<<std::endl;
    std::cout  << sm->Name.toStdString()<< "， 尾部四元数: " << sm->Context.visionData.robotpose[3] << ", " << sm->Context.visionData.robotpose[4] << ", " << sm->Context.visionData.robotpose[5] << "," << sm->Context.visionData.robotpose[6] << std::endl;
    planTaskInfo.boxInfo.prerotate(Eigen::Quaterniond(sm->Context.visionData.robotpose[3], sm->Context.visionData.robotpose[4], sm->Context.visionData.robotpose[5], sm->Context.visionData.robotpose[6]));
    std::cout << sm->Name.toStdString() << "， 尾部坐标： " << sm->Context.visionData.robotpose[0] << ", " << sm->Context.visionData.robotpose[1] << ", " << sm->Context.visionData.robotpose[2] << std::endl;
    planTaskInfo.boxInfo.pretranslate(Eigen::Vector3d(sm->Context.visionData.robotpose[0], sm->Context.visionData.robotpose[1], sm->Context.visionData.robotpose[2]+zero_offset));

    //根据状态机判断底层/顶层
    if (sm->Name == "Bottom")
    {
        std::cout << "底层箱子尾部参数进入队列" << std::endl;
        planTaskInfo.isup = false;
        qPlanTaskInfoBottom.push_back(planTaskInfo);
        emit finishVision_Signal_b(0);
    }
    else
    {
        std::cout << "顶层箱子尾部被参数进入队列" << std::endl;
        planTaskInfo.isup = true;
        qPlanTaskInfoTop.push_back(planTaskInfo);
        emit finishVision_Signal_u(0);
    }

    if (sm->Name == "Bottom")
    {
        emit finishVision_Signal_b(false);
    }
    else
    {
        emit finishVision_Signal_u(false);
    }

    std::cout << "视觉处理结束，触发轨迹规划信号" << std::endl;
    emit begintraj_Singal(this);
}

void MainProcess::finsihVision_head_Slot(ContextStateMachine *sm)
{
        #ifdef YASKAWA_ZERO_OFFSET
    double zero_offset = 600;
    #else
    double zero_offset = 0;
    #endif

    std::cout << "使用固定长度处理头部" << std::endl;
     auto handEyeMatrix_ub = sm->Name=="Bottom"?vws::handEyeMatrix_b_rbt1:vws::handEyeMatrix_b_rbt1;
 
    visionContext->work_headWithLength(sm->Context.visionData);

    vws::PlanTaskInfo planTaskInfo;
    planTaskInfo.diff = vws::diff;
    planTaskInfo.lx = sm->Context.visionData.width;
    planTaskInfo.ly = sm->Context.visionData.length_head;
    planTaskInfo.lz = sm->Context.visionData.height;
    planTaskInfo.encoder =  sm->Context.encoder_img_head;
    planTaskInfo.face =  0;
    planTaskInfo.boxInfo = Eigen::Isometry3d::Identity();

    std::cout << sm->Name.toStdString()<<", Plan Task, 编码器数值："<<std::to_string(planTaskInfo.encoder)<<std::endl;
    std::cout << sm->Name.toStdString() << ", 头部四元数: " << sm->Context.visionData.robotpose_head[3] << ", " << sm->Context.visionData.robotpose_head[4] << ", " << sm->Context.visionData.robotpose_head[5] << "," << sm->Context.visionData.robotpose_head[6] << std::endl;
    planTaskInfo.boxInfo.prerotate(Eigen::Quaterniond(sm->Context.visionData.robotpose_head[3], sm->Context.visionData.robotpose_head[4], sm->Context.visionData.robotpose_head[5], sm->Context.visionData.robotpose_head[6]));
    std::cout << sm->Name.toStdString() <<", 头部坐标： " << sm->Context.visionData.robotpose_head[0] << ", " << sm->Context.visionData.robotpose_head[1] << ", " << sm->Context.visionData.robotpose_head[2] << std::endl;
    planTaskInfo.boxInfo.pretranslate(Eigen::Vector3d(sm->Context.visionData.robotpose_head[0], sm->Context.visionData.robotpose_head[1], sm->Context.visionData.robotpose_head[2]+zero_offset));
   
     //根据状态机判断底层/顶层
    if (sm->Name == "Bottom")
    {
        std::cout << "底层箱子头部参数进入队列" << std::endl;
        planTaskInfo.isup = false;
        qPlanTaskInfoBottom.push_back(planTaskInfo);
        emit finishVision_Signal_b(1);
    }
    else
    {
        std::cout << "顶层箱子头部被参数进入队列" << std::endl;
        planTaskInfo.isup = true;
        qPlanTaskInfoTop.push_back(planTaskInfo);
        emit finishVision_Signal_u(1);
    }

    //根据状态机判断底层/顶层, 尾部处理结束
    if (sm->Name == "Bottom")
    {
        emit finishVision_Signal_b(false);
    }
    else
    {
        emit finishVision_Signal_u(false);
    }
    
    emit begintraj_Singal(this);
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
        emit sendTrajParam_Signal();
    }
}

bool MainProcess::sendtorbt()
{
    auto rbt = DeviceManager::getInstance()->getRobot(0);
    rbt->start();
    std::vector<VWSRobot::RobotTask> rbtparam;
    if (trajQueue.count() > 0)
    {
        std::cout << "机器人发送信息: " << std::endl;
        rbtparam = trajQueue.dequeue();
    }
    else
    {
        std::cout<<"机器人队列为空"<<std::endl;
        // rbtparam = staticTask;
        return false;
    }

    auto ret =rbt->sendData(rbtparam);
    if(ret>0){
        std::cout << "机器人发送任务成功" << std::endl;
        return true;
    }
    else{
        std::cout << "机器人发送任务失败" << std::endl;
        return false;
    }
}
