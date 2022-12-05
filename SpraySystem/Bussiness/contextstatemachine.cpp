#include "contextstatemachine.h"
#include "Data/StaticData.h"
#include <iostream>
#include "Device/devicemanager.h"
#include "Util/Log/clog.h"

std::mutex ContextStateMachine::_mutex;
float ContextStateMachine::getImgEncoder(bool init)
{
    float result = 0;
    int i=0;
    bool success = false;

    while (true)
    {
        /* code */
        result = DeviceManager::getInstance()->getMC()->getChainEncoders(success)[Context.index];
        if(init){
            if(success || i>=10){
                break;
            }
        }
        else{
            if(result!=pre_img_encoder || i>=10){
                break;
            }
        }
        i++;
        usleep(200000);
    }

    if(success==false){
        CLog::getInstance()->log("读取拍照时刻编码器数值超时");
        CLog::getInstance()->log("当前读数： "+QString::number(result)+", 前一次数值： "+QString::number(pre_img_encoder));
        //设备报警
        emit alarm();
    }
    pre_img_encoder = result;
    return result;
}

ContextStateMachine::ContextStateMachine()
{
    Name = "状态机";
    visionContext = new VisionContext();

    /*初始化状态*/
    parentState = new QState(this);
    deviceAlarm = new QState(this);
    stateIDLE = new QState(parentState);
    waitLaserSignal = new QState(parentState);
    processHeadImg = new QState(parentState);
    waitTrailProcess = new QState(parentState);
    processTrailImg = new QState(parentState);

    /**绑定跳转**/
    parentState->addTransition(this,SIGNAL(alarm()),deviceAlarm);
    tranWaitSignal = stateIDLE->addTransition(this, SIGNAL(cameraSignalOn()), waitLaserSignal);

    tranProcessHeadImg = waitLaserSignal->addTransition(this, SIGNAL(laserSignalOnAndImgReady()), processHeadImg);
    tranHeadImgTimeout = waitLaserSignal->addTransition(this, SIGNAL(headImgTimeout()), stateIDLE);

    tranWaiTrailProcess = processHeadImg->addTransition(this, SIGNAL(headDone()), waitTrailProcess);

    tranProcessTrailImg = waitTrailProcess->addTransition(this, SIGNAL(cameraSignalOffAndImgReady()), processTrailImg);
    tranTrailImgTimeout = waitTrailProcess->addTransition(this, SIGNAL(trailImgTimeout()), stateIDLE);


    tranIDLE = processTrailImg->addTransition(this, SIGNAL(trailDone()), stateIDLE);

    /**enter 事件**/
    connect(parentState,SIGNAL(entered()),this,SLOT(enteredParentState_Slot()));
    connect(deviceAlarm,SIGNAL(entered()),this,SLOT(enteredAlarm_Slot()));

    connect(waitLaserSignal, SIGNAL(entered()), this, SLOT(enteredWaitLaserSignal_Slot()));
    connect(waitLaserSignal,SIGNAL(exited()),this,SLOT(exitWaitLaserSignal_Slot()));

    connect(processHeadImg, SIGNAL(entered()), this, SLOT(enteredProcessHeadImg_Slot()));

    connect(waitTrailProcess, SIGNAL(entered()), this, SLOT(enterdWaitTrailProcess_Slot()));
    connect(waitTrailProcess,SIGNAL(exited()),this,SLOT(exitWaitTrailProcess_Slot()));

    connect(processTrailImg, SIGNAL(entered()), this, SLOT(enteredProcessTrailImg_Slot()));

    connect(stateIDLE, SIGNAL(entered()), this, SLOT(enteredIDLE_Slot()));

    parentState->setInitialState(stateIDLE);
    this->setInitialState(parentState);

 
    sm_thread = new QThread();
    moveToThread(sm_thread);
}

ContextStateMachine::~ContextStateMachine()
{
    sm_thread->quit();
    sm_thread->wait();
    delete sm_thread;
    delete visionContext;
    delete timer_img_head;
    delete timer_img_trail;
}

void ContextStateMachine::StartRun()
{

    timer_img_head = new QTimer();
    timer_img_head->moveToThread(sm_thread);
    connect(timer_img_head, SIGNAL(timeout()), this, SLOT(headTimer_Slot()));

    timer_img_trail = new QTimer();
    timer_img_trail->moveToThread(sm_thread);
    connect(timer_img_trail, SIGNAL(timeout()), this, SLOT(trailTimer_Slot()));

    sm_thread->start();
    start();
}

void ContextStateMachine::checkHeadLaserAndImg()
{
    _mutex.lock();
#ifdef _STATEPRINT_
    // std::cout << "enter checkHeadLaserAndImg" << std::endl;
#endif
    // std::cout << "箱体： " << Name.toStdString() << ", 检查头部图像和信号" << std::endl;
    if (Context.flag_laser == true && Context.flag_img_head == true)
    {
        // std::cout << "箱体： " << Name.toStdString() << "头部感应信号和图像齐全" << std::endl;
        emit laserSignalOnAndImgReady();
    }
    else
    {
        // std::cout << "箱体： " << Name.toStdString() << "感应信号和图像不齐全" << std::endl;
    }
    _mutex.unlock();
}

void ContextStateMachine::checkTrailLaserAndImg()
{
#ifdef _STATEPRINT_
    // std::cout << "enter checkTrailLaserAndImg" << std::endl;
#endif
    _mutex.lock();
    if (!Context.flag_camera && Context.flag_img_trail)
    {
        // std::cout << "箱体： " << Name.toStdString() << "尾部感应信号和图像齐全" << std::endl;
        emit cameraSignalOffAndImgReady();
    }
    else
    {
        // std::cout << "箱体： " << Name.toStdString() << "感应信号和图像不齐全" << std::endl;
    }
    _mutex.unlock();
}


void ContextStateMachine::enteredParentState_Slot()
{
    CLog::getInstance()->log("*****箱体： " + Name + "，进入状态： Parent******");
}

void ContextStateMachine::enteredAlarm_Slot()
{
    CLog::getInstance()->log("*****箱体： " + Name + "，进入状态： Alarm******");
}

void ContextStateMachine::sendPlcData_Slot(QVariant vData)
{
    SMContext data = vData.value<SMContext>();
    if (data.flag_camera)
    {
        std::cout<< Name.toStdString()  << "， 第一次拍照触发信息号" << std::endl;
        // Context.flag_laser = true;
        tmplaserdata =  data.laserCouple1;
        emit cameraSignalOn();
    }

    if (data.flag_laser)
    {
        std::cout << "检测到" << Name.toStdString() << "箱体" << std::endl;
        Context.flag_laser = true;
        checkHeadLaserAndImg();
    }

    if (!data.flag_camera)
    {
       std::cout<< Name.toStdString()  << "， 第二次拍照触发信息号" << std::endl;
        Context.flag_camera = false;
        checkTrailLaserAndImg();
    }
}

void ContextStateMachine::sendImgData_Slot(QVariant vData)
{
    VWSCamera::ImageData img = vData.value<VWSCamera::ImageData>();
    if (!Context.flag_img_head)
    {
        // std::cout << "sendImgData_Slot"
        //           << "headImg" << std::endl;
        Context.flag_img_head = true;
        Context.img_head = img;
        std::cout << "******箱体： " << Name.toStdString() << " ，收到头部图像信号"<<std::endl;
        checkHeadLaserAndImg();
    }
    else
    {
        Context.flag_img_trail = true;
        Context.img_trail = img;
        std::cout << "******箱体： " << Name.toStdString() <<"，收到尾部图像信号"<<std::endl;
        checkTrailLaserAndImg();
    }
}

void ContextStateMachine::enteredWaitLaserSignal_Slot()
{
    CLog::getInstance()->log("*****箱体： " + Name + "，进入状态： 等待箱体感应信号******");

    //校验测距
    if(vws::DataVerify::RangingVerify(tmplaserdata[0],tmplaserdata[1])){
        Context.laserCouple1 = tmplaserdata;
    }
    else{
        CLog::getInstance()->log("测距异常",CLog::CLOG_LEVEL::REEROR);
        Context.laserCouple1 = {vws::rangeMin,vws::rangeMin};
    }
    std::cout<<"测距： "<<std::to_string(Context.laserCouple1[0])<<", "<<std::to_string(Context.laserCouple1[1])<<std::endl;
    
    //读取拍照时刻编码器数值
    Context.encoder_img_head = getImgEncoder();
    std::cout << "StateMachine, 编码器数值： " << Context.encoder_img_head << std::endl;

    //开启计时器    
    timer_img_head->start(interval);
    timer_img_trail->start(interval);
}
void ContextStateMachine::exitWaitLaserSignal_Slot()
{
    CLog::getInstance()->log("*****箱体： " + Name + "，退出状态： 等待箱体感应信号******");

    timer_img_head->stop();
}

void ContextStateMachine::enteredProcessHeadImg_Slot()
{
    // std::cout << "******箱体： " << Name.toStdString() << "，进入状态： 处理头部图像******" << std::endl;
    CLog::getInstance()->log("*****箱体： " + Name + "，进入状态： 处理头部图像******");

    //视觉处理头部
    beginVision_head();

    emit headDone();
}

void ContextStateMachine::enterdWaitTrailProcess_Slot()
{
    CLog::getInstance()->log("*****箱体： " + Name + "，进入状态： 等待尾部图像处理******");
  
}
void ContextStateMachine::exitWaitTrailProcess_Slot()
{
    CLog::getInstance()->log("*****箱体： " + Name + "，退出状态： 等待尾部图像处理******");
}

void ContextStateMachine::enteredProcessTrailImg_Slot()
{
    CLog::getInstance()->log("*****箱体： " + Name + "，进入状态： 处理尾部图像******");

    
    timer_img_trail->stop();

    // Context.encoder_img_trail = DeviceManager::getInstance()->getMC()->getChainEncoders()[Context.index];
    Context.encoder_img_trail = getImgEncoder();
    std::cout << Name.toStdString()<< "StateMachine, 编码器数值, head: "<<Context.encoder_img_head <<" ,trail: " << Context.encoder_img_trail << std::endl;

    //视觉处理尾部
    beginVision_trail();

    emit trailDone();
}

void ContextStateMachine::enteredIDLE_Slot()
{
    CLog::getInstance()->log("*****箱体： " + Name + "，进入状态： IDLE******");
    std::thread::id id = std::this_thread::get_id();
    std::cout << "socketclient slot 线程ID: "<< id << std::endl;

    if(timer_img_head->isActive()){
        timer_img_head->stop();
    }
    if(timer_img_trail){
        timer_img_trail->stop();
    }
    Context.flag_laser = false;
    Context.flag_camera = false;
    Context.flag_img_head = false;
    Context.flag_img_trail = false;
    Context.laserCouple1.clear();
    Context.laserCouple2.clear();

    //初始化数值
    pre_img_encoder = getImgEncoder(true);
}

void ContextStateMachine::headTimer_Slot()
{
    time_head += this->interval;
    if (time_head > timeout_head)
    {
        timer_img_head->stop();
        std::cout << "箱体： " << Name.toStdString() << "，头部图像反馈超时" << std::endl;
        emit headImgTimeout();
    }
}
void ContextStateMachine::trailTimer_Slot()
{
    time_trail += this->interval;
    if (time_trail > timeout_trail)
    {
        timer_img_trail->stop();
        std::cout << "箱体： " << Name.toStdString() << "，尾部图像反馈超时" << std::endl;
        emit trailImgTimeout();
    }
}


void ContextStateMachine::beginVision_head()
{
    #ifdef YASKAWA_ZERO_OFFSET
    double zero_offset = 600;
    #else
    double zero_offset = 0;
    #endif

    std::cout << "头部开始视觉处理" << std::endl;

    visionContext->work_head(Context.img_head
    , Context.laserCouple1
    ,  Context.visionData);
    
     vws::PlanTaskInfo planTaskInfo_head;
    if(Context.visionData.hasError){
        //标记为已规划，当障碍物处理
        planTaskInfo_head.flag = true;
    }
    planTaskInfo_head.diff = vws::diff;
    planTaskInfo_head.lx = Context.visionData.width;
    planTaskInfo_head.ly = Context.visionData.length_head;
    planTaskInfo_head.lz = Context.visionData.height;
    planTaskInfo_head.encoder =  Context.encoder_img_head;
    planTaskInfo_head.face =  0;
    planTaskInfo_head.boxInfo = Eigen::Isometry3d::Identity();


    QString strlog = Name+"， 头部， Plan Task, 编码器数值："+QString::number(planTaskInfo_head.encoder);
    CLog::getInstance()->log(strlog);
    strlog = Name+ "， 头部四元数: " +QString::number(Context.visionData.robotpose_head[3])
            + ", " +QString::number(Context.visionData.robotpose_head[4])
            + ", " + QString::number(Context.visionData.robotpose_head[5])
            + "," + QString::number(Context.visionData.robotpose_head[6]);
    CLog::getInstance()->log(strlog);
        strlog = Name+ "， 头部坐标： " +QString::number(Context.visionData.robotpose_head[0])
            + ", " + QString::number(Context.visionData.robotpose_head[1]) 
            + ", " + QString::number(Context.visionData.robotpose_head[2]);
    CLog::getInstance()->log(strlog);
    strlog = Name +", 头部尺寸： "+QString::number(planTaskInfo_head.lx)+", "+QString::number(planTaskInfo_head.ly)+", "+QString::number(planTaskInfo_head.lz);
    CLog::getInstance()->log(strlog);

    planTaskInfo_head.boxInfo.prerotate(Eigen::Quaterniond(Context.visionData.robotpose_head[3], Context.visionData.robotpose_head[4], Context.visionData.robotpose_head[5], Context.visionData.robotpose_head[6]));
    planTaskInfo_head.boxInfo.pretranslate(Eigen::Vector3d(Context.visionData.robotpose_head[0], Context.visionData.robotpose_head[1], Context.visionData.robotpose_head[2]+zero_offset));

    QVariant vdata;

    //根据状态机判断底层/顶层
    if (Name == "Bottom")
    {
        CLog::getInstance()->log("低层箱子头部被参数进入队列");
        planTaskInfo_head.isup = false;
        vdata.setValue(planTaskInfo_head);
        emit finishVision_Signal_b(true);
        emit begintraj_Singal(vdata,0);
    }
    else
    {
        CLog::getInstance()->log("顶层箱子头部被参数进入队列");
        planTaskInfo_head.isup = true;
        vdata.setValue(planTaskInfo_head);

        emit finishVision_Signal_u(true);
        emit begintraj_Singal(vdata,1);
    }
}

void ContextStateMachine::beginVision_trail()
{
    #ifdef YASKAWA_ZERO_OFFSET
    double zero_offset = 600;
    #else
    double zero_offset = 0;
    #endif

   CLog::getInstance()->log("尾部开始视觉处理");

    std::vector<float> senorNums = {Context.encoder_img_head,Context.encoder_img_trail};
    visionContext->work_trail(Context.img_trail
    , senorNums
    ,  Context.visionData);


    //参数入规划队列, 头部如果未入队列，将头部也入队列
    vws::PlanTaskInfo planTaskInfo;
    if(Context.visionData.hasError){
        planTaskInfo.flag= true;
    }
    planTaskInfo.diff = vws::diff;
    planTaskInfo.lx = Context.visionData.width;
    planTaskInfo.ly = Context.visionData.length;
    planTaskInfo.lz = Context.visionData.height;
    planTaskInfo.encoder =  Context.encoder_img_trail;
    planTaskInfo.face =  1;
    planTaskInfo.boxInfo = Eigen::Isometry3d::Identity();

    QString strlog = Name+"， 尾部， Plan Task, 编码器数值："+QString::number(planTaskInfo.encoder);
    CLog::getInstance()->log(strlog);

    strlog = Name+ "， 尾部四元数: " +QString::number(Context.visionData.robotpose[3])
                + ", " +QString::number(Context.visionData.robotpose[4])
                + ", " + QString::number(Context.visionData.robotpose[5])
                + "," + QString::number(Context.visionData.robotpose[6]);
    CLog::getInstance()->log(strlog);            

    strlog = Name+ "， 尾部坐标： " +QString::number(Context.visionData.robotpose[0])
                + ", " + QString::number(Context.visionData.robotpose[1]) 
                + ", " + QString::number(Context.visionData.robotpose[2]);
    CLog::getInstance()->log(strlog);
    strlog = Name +", 尾部尺寸： "+QString::number(planTaskInfo.lx)+", "+QString::number(planTaskInfo.ly)+", "+QString::number(planTaskInfo.lz);
    CLog::getInstance()->log(strlog);

    planTaskInfo.boxInfo.prerotate(Eigen::Quaterniond(Context.visionData.robotpose[3], Context.visionData.robotpose[4], Context.visionData.robotpose[5], Context.visionData.robotpose[6]));
    planTaskInfo.boxInfo.pretranslate(Eigen::Vector3d(Context.visionData.robotpose[0], Context.visionData.robotpose[1], Context.visionData.robotpose[2]+zero_offset));

    QVariant vdata;
    //根据状态机判断底层/顶层
    if (Name == "Bottom")
    {
        CLog::getInstance()->log("底层箱子尾部参数进入队列");
        planTaskInfo.isup = false;
        vdata.setValue(planTaskInfo);
        emit finishVision_Signal_b(0);
        emit begintraj_Singal(vdata,0);
    }
    else
    {
        CLog::getInstance()->log("顶层箱子尾部被参数进入队列");
        planTaskInfo.isup = true;
        vdata.setValue(planTaskInfo);
        emit finishVision_Signal_u(0);
        emit begintraj_Singal(vdata, 1);
    }

    // if (Name == "Bottom")
    // {
    //     emit finishVision_Signal_b(false);
    // }
    // else
    // {
    //     emit finishVision_Signal_u(false);
    // }

}
