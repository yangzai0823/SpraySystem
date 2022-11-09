#include "mainprocess.h"
#include <Eigen/Core>
#include <Eigen/Geometry>
#include "VWSRobot/VWSRobot.h"
#include "Data/StaticData.h"

std::mutex MainProcess::_mutex;
void imgFunc(const VWSCamera::ImageData &data, void* pUser)
{
    MainProcess::CameraCallbackData *cameraCallbackData = (MainProcess::CameraCallbackData*)pUser;
    auto mainProcess = cameraCallbackData->mainProcess;
    auto cameraOperator = cameraCallbackData->camera;
    static int64_t q = data.RGB8PlanarImage.nTimeStamp;
    static int i = 0;
    std::cout<<++i<<"  "<<data.RGB8PlanarImage.nTimeStamp<<"      "<<data.RGB8PlanarImage.nTimeStamp - q<<std::endl;
    q = data.RGB8PlanarImage.nTimeStamp;

    VisionData v;

    if(cameraCallbackData->camera->getName()=="Camera 2"){

    }
    else{
        std::cout<<"获得下相机图像"<<std::endl;

        mainProcess->mainData.currentBottom.image.push_back(data);
        if(mainProcess->mainData.currentBottom.image.size()==1){
            std::cout<<"头部图像"<<std::endl;
            auto ret = mainProcess->checkHeadImgAndSignal(mainProcess->mainData.currentBottom
                                                          ,mainProcess->mainData.currentBottom.flag_laser_b,true);
            if(ret == true){
                emit mainProcess->laserSignalOnAndImgReady_b();
            }
        }
        else{
            std::cout<<"尾部图像"<<std::endl;
            auto ret = mainProcess->checkTrailImgAndSignal(mainProcess->mainData.currentBottom
                                                           ,mainProcess->mainData.currentBottom.flag_camera_b,true);
            if(ret == true){
                emit mainProcess->cameraSignalOffAndImgReady_b();
            }
        }
    }

}

void MainProcess::recevieData_Slot(QVariant data)
{
    plcdata = data.value<PLCData>();
    bottomWork();

}

MainProcess::MainProcess()
{
    vws::DataInit::Init();

    DeviceManager *deviceManager = DeviceManager::getInstance();
    //PLC
    auto plc = deviceManager->getPlc();
    plc->start();
    connect(plc,SIGNAL(recevieData_Signal(QVariant)),this,SLOT(recevieData_Slot(QVariant)));

    //运动控制器
    auto mc = deviceManager->getMC();
    mc->start();
    connect(mc,SIGNAL(getTrajParam_Signal()),this,SLOT(getTrajParam_Slot()));

    //机器人
    auto rbt = deviceManager->getRobot(0);
    rbt->init();
    rbt->start();

    //相机
    auto camera1 = deviceManager->getCamera(0);
    camera1CallbackData = new  CameraCallbackData();
    camera1CallbackData->camera = camera1;
    camera1CallbackData->mainProcess = this;

    auto ret = camera1->init();
    std::cout<<"camera init: "<<ret<<std::endl;
    ret = camera1->start();
    std::cout<<"camera start: "<<ret<<std::endl;
    camera1->RegisterFrameCallBack(imgFunc,(void *)(camera1CallbackData));

    visionContext = new VisionContext();
    trajProc = new TrajectoryProcess();
    trajThread = new QThread;
    trajProc->moveToThread(trajThread);
    connect(this,SIGNAL(begintraj_Singal(MainProcess* )), trajProc,SLOT(begintraj_Slot(MainProcess* )));
    trajThread->start();

    connect(this,SIGNAL(sendTrajParam_Signal()),this,SLOT(getTrajParam_Slot()));

    InitMachine_b();
}

MainProcess::~MainProcess()
{  
    trajThread->quit();
    trajThread->wait();
    delete trajThread;
    delete trajProc;

    delete camera1CallbackData;
    delete camera2CallbackData;

//    delete signalProcess;
}

std::vector<vws::PlanTaskInfo> *MainProcess::GetPlanTaskInfo(int upper_or_bottom)
{
    if(upper_or_bottom == 0){
         return &qPlanTaskInfoBottom;
    }else
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
    return 1/0.4198727819755431f;
}

Eigen::VectorXd MainProcess::getRobotWaitPose() const
{
    Eigen::VectorXd val(6);
    val << 1.5, 0, 0, 0, 0, 0;
    std::cout<<"getRobotWaitPose: "<<val[0]<<std::endl;
    return val;
}

bool MainProcess::getChainEncoderDir() const
{
    //TODO: 根据实际情况
    return false;
}

void MainProcess::VisionProcessing(vws::ProcessData data,bool upper_or_bottom, bool ishead)
{
    std::cout<<"视觉处理"<<std::endl;

    int index = 0;
    if(!ishead){
        index=1;
    }

    VisionData visionData;
    visionContext->work(data.image[index],vws::handEyeMatrix, visionData);
    // DeviceManager::getInstance()->getCamera(0)->deleteImage(data.image[index]);
    if(index==1){
        data.image.clear();
    }
    data.imgFlag =false;
    vws::PlanTaskInfo planTaskInfo;
    planTaskInfo.diff =  vws::diff;
    planTaskInfo.lx = 535;
    planTaskInfo.ly = vws::BoxLenght;
    planTaskInfo.lz = vws::BoxHeight;
    planTaskInfo.encoder = data.imgEncoder;
    planTaskInfo.face =data.face;
    planTaskInfo.boxInfo = Eigen::Isometry3d::Identity();


    std::cout<<"四元数: "<<visionData.robotpose[3]<<", "<<visionData.robotpose[4]<<", "<<visionData.robotpose[5]<<","<<visionData.robotpose[6]<<std::endl;
    planTaskInfo.boxInfo.prerotate(Eigen::Quaterniond(visionData.robotpose[3],visionData.robotpose[4],visionData.robotpose[5],visionData.robotpose[6]));

    std::cout<<"坐标： "<<visionData.robotpose[0]<<", "<<visionData.robotpose[1]<<", "<<visionData.robotpose[2]<<std::endl;
    planTaskInfo.boxInfo.pretranslate(Eigen::Vector3d(visionData.robotpose[0],visionData.robotpose[1],visionData.robotpose[2]));
    if(upper_or_bottom){
       std::cout<<"顶层箱子参数进入队列"<<std::endl;
        qPlanTaskInfoTop.push_back(planTaskInfo);
    }
    else{
          std::cout<<"底层箱子参数进入队列"<<std::endl;
        qPlanTaskInfoBottom.push_back(planTaskInfo);
    }

    std::cout<<"触发轨迹规划信号"<<std::endl;

    emit begintraj_Singal(this);
}

void MainProcess::getTrajParam_Slot()
{
    auto mc= DeviceManager::getInstance()->getMC();


    if(mcQueue.count()>0){
        auto param = mcQueue.dequeue();
        auto zeropoint = param[0];
        auto offset = param[1];
        // offset = -1;

        std::cout<<"运动控制发送信息: "<<zeropoint<<std::endl;
        mc->sendTrajParam(zeropoint,offset);

        sendtorbt();
        mcRequest = false;
    }else{
        std::cout<<"运动控制器请求数据，前队列为空"<<std::endl;
         mcRequest = true;
    }
}

void MainProcess::SetRobotTaskInfo(std::vector<float> mc_data,std::vector<RobotTask> robotTasks){
    std::cout<<"规划结果写入队列"<<std::endl;
    mcQueue.push_back(mc_data);
    trajQueue.push_back(robotTasks);

    if(mcRequest){
        // getTrajParam_Slot();
        std::cout<<"运动控制器请求过数据，但未发送"<<std::endl;
        emit sendTrajParam_Signal();
    }
}

void MainProcess::sendtorbt()
{
    static std::vector<VWSRobot::RobotTask> staticTask;

    auto rbt = DeviceManager::getInstance()->getRobot(0);
    rbt->start();
    std::vector<VWSRobot::RobotTask> rbtparam;
    std::cout<<"机器人发送信息: "<<std::endl;
    if(trajQueue.count()>0){
         rbtparam = trajQueue.dequeue();
         staticTask = rbtparam;
    }
    else
    {
        rbtparam = staticTask;
    }

    
    rbt->sendData(rbtparam);
    std::cout<<"发送结束"<<std::endl;
}


int64_t MainProcess::getTimeStamp()
{
    //毫秒数
    int mSecond = 0;
    struct timeval tv;
    struct timezone tz;
    struct tm* p;
    gettimeofday(&tv,&tz);
    p = localtime(&tv.tv_sec);
    mSecond = tv.tv_usec / 1000;
    int64_t timeStamp = ((int64_t)time(NULL))*1000 + mSecond;

    return timeStamp;
}

void MainProcess::bottomWork()
{
    //获取当前时间戳，对比触发时刻记录的时间戳
    int64_t currentStamp = getTimeStamp();

    auto currentState = GetMachineState(machine_b);
//    auto currentState = mainData.currentBottom.state;
    if(plcdata.flag_camera_b){
        mainData.currentBottom.flag_laser_b= true;
        emit cameraSignalOn_b();
    }
    
    if(!plcdata.flag_camera_b){
        std::cout<<"下相机第二次拍照触发信息号，记录前一次箱子尺寸"<<std::endl;
        //检查图像是否获取，如果获取图像更新则更新状态
        auto ret = checkTrailImgAndSignal(mainData.currentBottom,false,mainData.currentBottom.imgFlag);
        if(ret == true){
            emit cameraSignalOffAndImgReady_b();
        }
    }
    
    if(plcdata.flag_laser_b){
        std::cout<<"检测到下侧箱体"<<std::endl;
        //检查图像是否获取，如果获取图像更新则更新状态
        auto ret = checkHeadImgAndSignal(mainData.currentBottom,true,mainData.currentBottom.imgFlag);
        if(ret == true){
            emit laserSignalOnAndImgReady_b();
        }
    }
    // else if(!plcdata.flag_camera_b && currentState!=SignalState::State::HeadImgDone){
    //     std::cout<<"头部未结束，获得尾部拍照信号,锁存信号"<<std::endl;
    //     mainData.currentBottom.flag_camera_b = true;
    // }
}
bool MainProcess::checkHeadImgAndSignal(vws::ProcessData &pData,bool flag_laser,bool imgFlag)
{
    bool ok = false;
    // _mutex.lock();
    if(imgFlag && flag_laser)
    {
        ok = true;
    }
    if(pData.imgFlag != imgFlag){
        pData.imgFlag = imgFlag;
    }
    if(pData.flag_laser_b!=flag_laser){
        pData.flag_laser_b = flag_laser;
    }
    // _mutex.unlock();
    return ok;
}

bool MainProcess::checkTrailImgAndSignal(vws::ProcessData &pData,bool flag_camera, bool imgFlag)
{
    bool ok = false;
    // _mutex.lock();
    if(imgFlag && !flag_camera)
    {
        ok = true;
    }
    if(pData.imgFlag != imgFlag){
        pData.imgFlag = imgFlag;
    }
    if(pData.flag_camera_b!=flag_camera){
        pData.flag_camera_b = flag_camera;
    }
    // _mutex.unlock();
    return ok;
}

SignalState::State MainProcess::GetMachineState(QStateMachine *machine)
{
    auto ret  = machine->property("state");
    return ret.value<SignalState::State>();
}

void MainProcess::InitMachine_b()
{
    machine_b = new QStateMachine();
    stateIDLE_b = new QState(machine_b);
    waitLaserSignal_b = new QState(machine_b);
    processHeadImg_b = new QState(machine_b);
    headProcessDone_b = new QState(machine_b);
    processTrailImg_b = new QState(machine_b);

    tranWaitSignal = stateIDLE_b->addTransition(this,SIGNAL(cameraSignalOn_b()),waitLaserSignal_b);
    tranProcessHeadImg = waitLaserSignal_b->addTransition(this,SIGNAL(laserSignalOnAndImgReady_b()),processHeadImg_b);
    tranHeadProcessDone = processHeadImg_b->addTransition(this,SIGNAL(headDone_b()),headProcessDone_b);
    tranProcessTrailImg = headProcessDone_b->addTransition(this,SIGNAL(cameraSignalOffAndImgReady_b()),processTrailImg_b);
    tranIDLE = processTrailImg_b->addTransition(this,SIGNAL(trailDone_b()),stateIDLE_b);

    waitLaserSignal_b->assignProperty(machine_b,"state",SignalState::State::WaitLaserSignalOn);
    processHeadImg_b->assignProperty(machine_b,"state",SignalState::State::ProcessHeadImg);
    headProcessDone_b->assignProperty(machine_b,"state",SignalState::State::HeadImgDone);
    processHeadImg_b->assignProperty(machine_b,"state",SignalState::State::ProcessTrailImg);
    stateIDLE_b->assignProperty(machine_b,"state",SignalState::State::IDLE);

    connect(waitLaserSignal_b,SIGNAL(entered()),this,SLOT(enteredWaitLaserSignal_b()));
    connect(processHeadImg_b,SIGNAL(entered()),this,SLOT(enteredProcessHeadImg_b()));
    connect(headProcessDone_b,SIGNAL(entered()),this,SLOT(enterdHeadProcessDone_b()));
    connect(processTrailImg_b,SIGNAL(entered()),this,SLOT(enteredProcessTrailImg_b()));
    connect(stateIDLE_b,SIGNAL(entered()),this,SLOT(enteredIDLE_b()));

    machine_b->setInitialState(stateIDLE_b);
    machine_b->start();
}

void MainProcess::enteredWaitLaserSignal_b()
{
    std::cout<<"enteredWaitLaserSignal: "<<"下相机第一次拍照信号触发，记录箱子尺寸，并记录时间戳开始等待箱体检测信息号"<<std::endl;

    //获取当前时间戳，对比触发时刻记录的时间戳
    int64_t currentStamp = getTimeStamp();
    mainData.currentBottom.mtimeStamp = currentStamp;
//    this->UpdateState(mainData->currentBottom, SignalState::Trigger::CameraSignalOn);

    //2. 记录测距
    mainData.currentBottom.box1signal = {plcdata.laser3,plcdata.laser4};
    staticSignal = mainData.currentBottom.box1signal;
    //获取拍照时刻编码器数值
    mainData.currentBottom.imgEncoder = DeviceManager::getInstance()->getMC()->getChainEncoders()[0];
    std::cout<<"拍照时刻编码器： "<<mainData.currentBottom.imgEncoder;
    mainData.currentBottom.face = 0;
}

void MainProcess::enteredProcessHeadImg_b()
{
    std::cout<<"enteredProcessHeadImg_b: "<<"视觉处理"<<std::endl;
    VisionProcessing(mainData.currentBottom,1,true);
    emit headDone_b();
}

void MainProcess::enterdHeadProcessDone_b()
{
    auto ret = checkHeadImgAndSignal(mainData.currentBottom,mainData.currentBottom.flag_camera_b,mainData.currentBottom.imgFlag_trail);
    if(ret == true){
        emit cameraSignalOffAndImgReady_b();
    }
}

void MainProcess::enteredProcessTrailImg_b()
{
    std::cout<<"enteredProcessTrailImg_b: "<<"视觉处理"<<std::endl;

    mainData.currentBottom.imgEncoder = DeviceManager::getInstance()->getMC()->getChainEncoders()[1];
    mainData.currentBottom.face = 1;
    mainData.currentBottom.box1signal = staticSignal;

    VisionProcessing(mainData.currentBottom,1,false);
    emit trailDone_b();
}

void MainProcess::enteredIDLE_b()
{
    std::cout<<"清理数据"<<std::endl;
    if(mainData.currentBottom.image.size()>0){
        for(int i = 0;i<mainData.currentBottom.image.size();i++){
            DeviceManager::getInstance()->getCamera(0)->deleteImage(mainData.currentBottom.image[0]);
        }
        mainData.currentBottom.image.clear();
    }
    mainData.currentBottom.flag_laser_b = false;
    mainData.currentTop.flag_camera_b = false;
}

QStateMachine *MainProcess::getMachine() const
{
    return machine_b;
}

void MainProcess::setMachine(QStateMachine *value)
{
    machine_b = value;
}

