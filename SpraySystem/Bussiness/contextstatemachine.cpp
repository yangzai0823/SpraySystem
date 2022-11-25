#include "contextstatemachine.h"
#include "Data/StaticData.h"
#include <iostream>
#include "Device/devicemanager.h"
std::mutex ContextStateMachine::_mutex;
ContextStateMachine::ContextStateMachine()
{
    Name = "状态机";

    //    visionContext = new VisionContext();

    /*初始化状态*/
    stateIDLE = new QState(this);
    waitLaserSignal = new QState(this);
    processHeadImg = new QState(this);
    encoderOutofLimit = new QState(this);
    waitTrailProcess = new QState(this);
    processTrailImg = new QState(this);

    /**绑定跳转**/
    tranWaitSignal = stateIDLE->addTransition(this, SIGNAL(cameraSignalOn()), waitLaserSignal);

    tranProcessHeadImg = waitLaserSignal->addTransition(this, SIGNAL(laserSignalOnAndImgReady()), processHeadImg);
    tranHeadImgTimeout = waitLaserSignal->addTransition(this, SIGNAL(headImgTimeout()), stateIDLE);

    tranWaiTrailProcess = processHeadImg->addTransition(this, SIGNAL(headDone()), waitTrailProcess);

    tranProcessTrailImg = waitTrailProcess->addTransition(this, SIGNAL(cameraSignalOffAndImgReady()), processTrailImg);
    tranEncoderOutofLimit = waitTrailProcess->addTransition(this, SIGNAL(outOfLimt()), encoderOutofLimit);
    tranTrailImgTimeout = waitTrailProcess->addTransition(this, SIGNAL(trailImgTimeout()), stateIDLE);

    tranEncoderOutofLimit = encoderOutofLimit->addTransition(this,SIGNAL(cameraSignalOffAndImgReady()), processTrailImg);
    tranEncoderOutofLimit = encoderOutofLimit->addTransition(this,SIGNAL(trailImgTimeout()), stateIDLE);


    tranIDLE = processTrailImg->addTransition(this, SIGNAL(trailDone()), stateIDLE);

    /**enter 事件**/
    connect(waitLaserSignal, SIGNAL(entered()), this, SLOT(enteredWaitLaserSignal_Slot()));
    connect(waitLaserSignal,SIGNAL(exited()),this,SLOT(exitWaitLaserSignal_Slot()));

    connect(processHeadImg, SIGNAL(entered()), this, SLOT(enteredProcessHeadImg_Slot()));

    connect(encoderOutofLimit,SIGNAL(entered()),this,SLOT(enteredEncoderOutofLimit_Slot()));

    connect(waitTrailProcess, SIGNAL(entered()), this, SLOT(enterdWaitTrailProcess_Slot()));
    connect(waitTrailProcess,SIGNAL(exited()),this,SLOT(exitWaitTrailProcess_Slot()));

    connect(processTrailImg, SIGNAL(entered()), this, SLOT(enteredProcessTrailImg_Slot()));

    connect(stateIDLE, SIGNAL(entered()), this, SLOT(enteredIDLE_Slot()));
    this->setInitialState(stateIDLE);

    timer_img_head = new QTimer();
    connect(timer_img_head, SIGNAL(timeout()), this, SLOT(headTimer_Slot()));
    timer_img_trail = new QTimer();
    connect(timer_img_trail, SIGNAL(timeout()), this, SLOT(trailTimer_Slot()));

    headDisMonitor = new HeadDistanceMonitor();
    connect(this,SIGNAL(beginMonitroDis_Signal(float)),headDisMonitor,SLOT(Work_Slot(float)));
    connect(headDisMonitor,SIGNAL(outOfLimit_Signal()),this,SLOT(outOfLimit_Slot()));
    thread_distance = new QThread();
    headDisMonitor->moveToThread(thread_distance);
    // if(!thread_distance->isRunning()){
    //     thread_distance->start();
    // }
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

void ContextStateMachine::outOfLimit_Slot(){
    //进入编码器超限状态
    emit outOfLimt();
}

void ContextStateMachine::enteredWaitLaserSignal_Slot()
{
    std::cout << "******箱体： " << Name.toStdString() << "，进入状态： 等待箱体感应信号******" << std::endl;
    Context.laserCouple1 = tmplaserdata;
    // Context.laserCouple2 = data.laserCouple2;
    std::cout<<"测距： "<<std::to_string(Context.laserCouple1[0])<<", "<<std::to_string(Context.laserCouple1[1])<<std::endl;

    //读取拍照时刻编码器数值
     Context.encoder_img_head = DeviceManager::getInstance()->getMC()->getChainEncoders()[Context.index];
    std::cout << "StateMachine, 编码器数值： " << Context.encoder_img_head << std::endl;

    //开启计时器    
    timer_img_head->start(interval);
    timer_img_trail->start(interval);
}
void ContextStateMachine::exitWaitLaserSignal_Slot()
{
    std::cout << "******箱体： " << Name.toStdString() << "，退出状态： 等待箱体感应信号******" << std::endl;
    timer_img_head->stop();
}

void ContextStateMachine::enteredEncoderOutofLimit_Slot()
{
    //使用固定长度处理头部图像信息
    emit finishVision_Head_Signal(this);
}

void ContextStateMachine::enteredProcessHeadImg_Slot()
{
    std::cout << "******箱体： " << Name.toStdString() << "，进入状态： 处理头部图像******" << std::endl;

     //开始监听悬挂链运行距离
    emit beginMonitroDis_Signal(Context.encoder_img_head);

    // emit beginVision_Singal(this, true);
    //视觉处理头部
    emit beginVision_Head_Signal(this);
}

void ContextStateMachine::enterdWaitTrailProcess_Slot()
{
    std::cout << "******箱体： " << Name.toStdString() << "，进入状态： 等待尾部图像处理******" << std::endl;
  
}
void ContextStateMachine::exitWaitTrailProcess_Slot()
{
    std::cout << "******箱体： " << Name.toStdString() << "，退出状态： 等待尾部图像处理******" << std::endl;
    //停止监听悬挂链运行距离
    headDisMonitor->Stop();
}

void ContextStateMachine::enteredProcessTrailImg_Slot()
{
    std::cout << "******箱体： " << Name.toStdString() << "，进入状态： 处理尾部图像******" << std::endl;
    
    timer_img_trail->stop();

    Context.encoder_img_trail = DeviceManager::getInstance()->getMC()->getChainEncoders()[Context.index];
    std::cout << Name.toStdString()<< "StateMachine, 编码器数值, head: "<<Context.encoder_img_head <<" ,trail: " << Context.encoder_img_trail << std::endl;

    // emit beginVision_Singal(this, false);
    //视觉处理尾部
    emit beginVision_Trail_Signal(this);
}

void ContextStateMachine::enteredIDLE_Slot()
{
    std::cout << "******箱体： " << Name.toStdString() << "，进入状态： IDLE******" << std::endl;
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
