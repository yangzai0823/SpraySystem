#include "mainprocess.h"
#include <Eigen/Core>
#include <Eigen/Geometry>
#include "VWSRobot/VWSRobot.h"
#include "Data/StaticData.h"
#include "Util/Log/clog.h"

std::mutex MainProcess::_mutex;
std::mutex MainProcess::_trajret_mutex;
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


void testMove()
{
    double start[6] = { -31.7623,
                    33.8082,
                    -40.1508,
                    20.9592,
                    -37.6706,
                    -0.7618};
    double end[6] = { -27.0692,
                    -55.7453,
                    -43.5885,
                    105.0391,
                    -122.7237,
                    -53.6061};

    std::vector<RobotTask> rbt_tasks;
    RobotTask rbttask;
    std::array<float,7UL> jv;
    float speed = 50;
    int N = 4;
    for(int n = 0; n < N; n++){
        for(int i = 0; i < 6; i++){
            jv[i] = (end[i] - start[i]) / (N - 1) * n + start[i];
        }
        rbttask.point = jv;
        rbttask.speed[0] = speed;
        rbttask.speed[1] = speed;
        rbttask.taskType = VWSRobot::TaskType::track_1;
        rbt_tasks.push_back(rbttask);    
    }

    for(int i = 0; i < 6; i++){
        jv[i] = start[i];
    }
    rbttask.point = jv;
    rbttask.speed[0] = speed;
    rbttask.speed[1] = speed;
    rbttask.taskType = VWSRobot::TaskType::track_1;
    rbt_tasks.push_back(rbttask);    

    DeviceManager::getInstance()->getRobot(0)->sendData(rbt_tasks);
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
    // testMove();

    // QVariant vData;
    // vData.setValue(data);

    // recevieData_Slot(vData);
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
    CLog::getInstance()->log("启动程序");

    DeviceManager *deviceManager = DeviceManager::getInstance();
    // PLC
    auto plc = deviceManager->getPlc();
    connect(plc, SIGNAL(recevieData_Signal(QVariant)), this, SLOT(recevieData_Slot(QVariant)));

    //运动控制器
    auto mc = deviceManager->getMC();
    connect(mc, SIGNAL(getTrajParam_Signal()), this, SLOT(getTrajParam_Slot()));
    connect(mc,SIGNAL(sendToRBT_Signal()),this,SLOT(sendToRBT_Slot()));
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
    connect(sm_bottom,SIGNAL(alarm()),this,SLOT(alarm_Slot()));
    connect(this, SIGNAL(sendPlcData_b(QVariant)), sm_bottom, SLOT(sendPlcData_Slot(QVariant)));
    connect(this, SIGNAL(sendImgData_b(QVariant)), sm_bottom, SLOT(sendImgData_Slot(QVariant)));
    connect(sm_bottom,SIGNAL(begintraj_Singal(QVariant, bool))
            ,this,SLOT(begintraj_Slot(QVariant, bool)));
    sm_bottom->Name = "Bottom";
    sm_bottom->Context.index = 0;
    sm_bottom->Context.visionData.top_or_bottom = 1; 
    sm_bottom->StartRun();

    sm_top = new ContextStateMachine();
    connect(this, SIGNAL(sendPlcData_u(QVariant)), sm_top, SLOT(sendPlcData_Slot(QVariant)));
    connect(this, SIGNAL(sendImgData_u(QVariant)), sm_top, SLOT(sendImgData_Slot(QVariant)));
    connect(sm_top,SIGNAL(begintraj_Singal(QVariant, bool))
        ,this,SLOT(begintraj_Slot(QVariant, bool)));
    sm_top->Name = "Top";
    sm_top->Context.index = 1;
    sm_top->Context.visionData.top_or_bottom = 0;
    sm_top->StartRun();

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

ThreadSafeVector<vws::PlanTaskInfo> *MainProcess::GetPlanTaskInfo(int upper_or_bottom)
{
    if (upper_or_bottom == 0)
    {
        return &safeQPlanTaskInfoBottom;
    }
    else
    {
        return &safeQPlanTaskInfoTop;
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
        CLog::getInstance()->log("运动控制器队列");
        for(int i=0;i<mcQueue.length();i++){
            auto tmp = mcQueue.at(i);
            std::cout<<std::to_string(tmp[0])<<", "<<std::endl;
        }

        auto param = mcQueue.dequeue();
        auto zeropoint = param[0];
        auto offset = param[1];
        // offset = -1;

        QString strlog = "运动控制发送信息"+QString::number(zeropoint);
        CLog::getInstance()->log(strlog);
        mc->sendTrajParam(zeropoint, offset);

        mcRequest = false;
    }
    else
    {
        CLog::getInstance()->log("运动控制器请求数据，前队列为空");
        mcRequest = true;
    }
}

void MainProcess::sendToRBT_Slot()
{
    auto rbt = DeviceManager::getInstance()->getRobot(0);
    rbt->start();
    std::vector<VWSRobot::RobotTask> rbtparam;
    if (trajQueue.count() > 0)
    {
        CLog::getInstance()->log("机器人发送信息");
        rbtparam = trajQueue.dequeue();
    }
    else
    {
        CLog::getInstance()->log("机器人队列为空");
    }

    auto ret =rbt->sendData(rbtparam);
    if(ret>0){
        CLog::getInstance()->log("机器人发送任务成功, 点数： "+ QString::number(rbtparam.size())+", 队列剩余： "+QString::number(trajQueue.size()));
    }
    else{
        CLog::getInstance()->log("机器人发送任务失败");
    }
}

void MainProcess::SetRobotTaskInfo(std::vector<float> mc_data, std::vector<RobotTask> robotTasks)
{
    _trajret_mutex.lock();
    CLog::getInstance()->log("规划结果入队列");
    mcQueue.push_back(mc_data);
    trajQueue.push_back(robotTasks);

    if (mcRequest)
    {
        // std::cout << "运动控制器请求过数据，但未发送" << std::endl;
        CLog::getInstance()->log("运动控制器请求过数据，但未发送");
        emit sendTrajParam_Signal();
    }
    _trajret_mutex.unlock();
}

void MainProcess::alarm_Slot(){

}

void MainProcess::begintraj_Slot(QVariant vdata, bool up_or_bttom)
{
    vws::PlanTaskInfo planTaskInfo = vdata.value<vws::PlanTaskInfo>();
    if(up_or_bttom){
        // qPlanTaskInfoTop.push_back(planTaskInfo);
        CLog::getInstance()->log("顶层视觉处理结束，触发轨迹规划信号");
        safeQPlanTaskInfoTop.push_back(planTaskInfo);
        CLog::getInstance()->log(QString::number(safeQPlanTaskInfoTop.getVector().size()));
    }
    else{
        // qPlanTaskInfoBottom.push_back(planTaskInfo);
        CLog::getInstance()->log("低层视觉处理结束，触发轨迹规划信号");
        safeQPlanTaskInfoBottom.push_back(planTaskInfo);
        CLog::getInstance()->log(QString::number(safeQPlanTaskInfoBottom.getVector().size()));
    }

    emit begintraj_Singal(this);
}