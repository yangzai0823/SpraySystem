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
    headProcessDone = new QState(this);
    processTrailImg = new QState(this);

    /**绑定跳转**/
    tranWaitSignal = stateIDLE->addTransition(this, SIGNAL(cameraSignalOn()), waitLaserSignal);

    tranProcessHeadImg = waitLaserSignal->addTransition(this, SIGNAL(laserSignalOnAndImgReady()), processHeadImg);
    //tranHeadImgTimeout = waitLaserSignal->addTransition(this, SIGNAL(headImgTimeout()), stateIDLE);

    tranHeadProcessDone = processHeadImg->addTransition(this, SIGNAL(headDone()), headProcessDone);

    tranProcessTrailImg = headProcessDone->addTransition(this, SIGNAL(cameraSignalOffAndImgReady()), processTrailImg);
    //tranTrailImgTimeout = headProcessDone->addTransition(this, SIGNAL(trailImgTimeout()), stateIDLE);

    tranIDLE = processTrailImg->addTransition(this, SIGNAL(trailDone()), stateIDLE);

    /**enter 事件**/
    connect(waitLaserSignal, SIGNAL(entered()), this, SLOT(enteredWaitLaserSignal_Slot()));
    connect(processHeadImg, SIGNAL(entered()), this, SLOT(enteredProcessHeadImg_Slot()));
    connect(headProcessDone, SIGNAL(entered()), this, SLOT(enterdHeadProcessDone_Slot()));
    connect(processTrailImg, SIGNAL(entered()), this, SLOT(enteredProcessTrailImg_Slot()));
    connect(stateIDLE, SIGNAL(entered()), this, SLOT(enteredIDLE_Slot()));
    this->setInitialState(stateIDLE);

    timer_img_head = new QTimer();
    connect(timer_img_head, SIGNAL(timeout()), this, SLOT(headTimer_Slot()));
    timer_img_trail = new QTimer();
    connect(timer_img_trail, SIGNAL(timeout()), this, SLOT(trailTimer_Slot()));
}

ContextStateMachine::~ContextStateMachine()
{
    //    delete visionContext;

    delete stateIDLE;
    delete waitLaserSignal;
    delete processHeadImg;
    delete headProcessDone;
    delete processTrailImg;
    delete stateIDLE;

    delete tranWaitSignal;
    delete tranProcessHeadImg;
    delete tranHeadProcessDone;
    delete tranProcessTrailImg;
    delete tranIDLE;

    delete timer_img_head;
    delete timer_img_trail;
}

void ContextStateMachine::checkHeadLaserAndImg()
{
    _mutex.lock();
#ifdef _STATEPRINT_
    std::cout << "enter checkHeadLaserAndImg" << std::endl;
#endif
    // std::cout << "箱体： " << Name.toStdString() << ", 检查头部图像和信号" << std::endl;
    if (Context.flag_laser == true && Context.flag_img_head == true)
    {
        std::cout << "箱体： " << Name.toStdString() << "感应信号和图像齐全" << std::endl;
        emit laserSignalOnAndImgReady();
    }
    else
    {
        std::cout << "箱体： " << Name.toStdString() << "感应信号和图像不齐全" << std::endl;
    }
    _mutex.unlock();
}

void ContextStateMachine::checkTrailLaserAndImg()
{
#ifdef _STATEPRINT_
    std::cout << "enter checkTrailLaserAndImg" << std::endl;
#endif
    _mutex.lock();
    // std::cout << "箱体： " << Name.toStdString() << "，检查尾部图像和信号" << std::endl;
    if (!Context.flag_camera && Context.flag_img_trail)
    {
        std::cout << "箱体： " << Name.toStdString() << "感应信号和图像齐全" << std::endl;
        emit cameraSignalOffAndImgReady();
    }
    else
    {
        std::cout << "箱体： " << Name.toStdString() << "感应信号和图像不齐全" << std::endl;
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
        Context.laserCouple1 = data.laserCouple1;
        Context.laserCouple2 = data.laserCouple2;

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
        // std::cout << "下相机第二次拍照触发信息号" << std::endl;
        Context.flag_camera = false;
        checkTrailLaserAndImg();
    }
}

void ContextStateMachine::sendImgData_Slot(QVariant vData)
{
    VWSCamera::ImageData img = vData.value<VWSCamera::ImageData>();
    if (!Context.flag_img_head)
    {
        std::cout << "sendImgData_Slot"
                  << "headImg" << std::endl;
        Context.flag_img_head = true;
        Context.img_head = img;
        std::cout<<"收到头部图像信号"<<std::endl;
        checkHeadLaserAndImg();
    }
    else
    {
        Context.flag_img_trail = true;
        Context.img_trail = img;
        std::cout<<"收到尾部图像信号"<<std::endl;
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
    std::cout << "******箱体： " << Name.toStdString() << "，进入状态： 等待箱体感应信号******" << std::endl;

    //读取拍照时刻编码器数值
     Context.encoder_img_head = DeviceManager::getInstance()->getMC()->getChainEncoders()[Context.index];

    //开启计时器
    timer_img_head->start(interval);
}

void ContextStateMachine::enteredProcessHeadImg_Slot()
{
    std::cout << "******箱体： " << Name.toStdString() << "，进入状态： 处理头部图像******" << std::endl;
     timer_img_head->stop();
   
    emit beginVision_Singal(this, true);
}

void ContextStateMachine::enterdHeadProcessDone_Slot()
{
    std::cout << "******箱体： " << Name.toStdString() << "，进入状态： 头部处理完成******" << std::endl;
    //TODO：启动线程监控是否超过规定距离，超过则按规定距离作为箱体长度计算头部信息
  
}

void ContextStateMachine::enteredProcessTrailImg_Slot()
{
    std::cout << "******箱体： " << Name.toStdString() << "，进入状态： 处理尾部图像******" << std::endl;
    //TODO： 停止距离监控线程


    Context.encoder_img_trail = DeviceManager::getInstance()->getMC()->getChainEncoders()[Context.index];
    emit beginVision_Singal(this, false);
}

void ContextStateMachine::enteredIDLE_Slot()
{
    std::cout << "******箱体： " << Name.toStdString() << "，进入状态： IDLE******" << std::endl;
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
    if (time_head > timeout)
    {
        std::cout << "箱体： " << Name.toStdString() << "，头部图像反馈超时" << std::endl;
        timer_img_head->stop();
        emit headImgTimeout();
    }
}
void ContextStateMachine::timeoutHeadImg_Slot()
{
}
void ContextStateMachine::trailTimer_Slot()
{
    std::cout << "timeout" << std::endl;
}
void ContextStateMachine::timeoutTrailImg_Slot()
{
    std::cout << "箱体： " << Name.toStdString() << "，尾部图像反馈超时" << std::endl;
}
