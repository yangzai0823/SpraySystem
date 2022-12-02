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

    timer_img_head = new QTimer();
    connect(timer_img_head, SIGNAL(timeout()), this, SLOT(headTimer_Slot()));
    timer_img_trail = new QTimer();
    connect(timer_img_trail, SIGNAL(timeout()), this, SLOT(trailTimer_Slot()));
}

ContextStateMachine::~ContextStateMachine()
{
    delete timer_img_head;
    delete timer_img_trail;
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

void ContextStateMachine::finishVision_Slot(bool ishead)
{
    if (ishead)
    {
        emit headDone();
    }
    else
    {
        emit trailDone();
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
    emit beginVision_Head_Signal(this);
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
    emit beginVision_Trail_Signal(this);
}

void ContextStateMachine::enteredIDLE_Slot()
{
    CLog::getInstance()->log("*****箱体： " + Name + "，进入状态： IDLE******");

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

    // vws::VisionData vd;
    // vd.hasError =false;
    // Context.visionData  = vd;

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
