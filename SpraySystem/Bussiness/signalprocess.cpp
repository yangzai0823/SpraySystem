#include "signalprocess.h"
#include "mutex"
#include "Data/StaticData.h"
#include "Device/devicemanager.h"

std::mutex _mutex;
SignalProcess::SignalProcess()
{

}

uint32_t SignalProcess::work(vws::PLCData plcData,vws::MainData & mainData)
{
    _mutex.lock();
    //获取当前时间戳，对比触发时刻记录的时间戳
    int64_t currentStamp = getTimeStamp();
    if(this->signal!=0 && currentStamp - mainData.mtimeStamp>vws::SingalTimeOut){
        std::cout<<"触发时间超过规定时长，清空存储信息"<<std::endl;
        mainData.cell = 0;
        signal = 0;
        mainData.box1signal = {0,0};
        mainData.box2signal = {0,0};
        DeviceManager::getInstance()->getCamera(0)->deleteImage(mainData.image1);
        //         DeviceManager::getInstance()->getCamera(1)->deleteImage(mainData.image2);
    }
    //1. 空闲状态下，相机拍照触发
    if(plcData.flag3 && signal == 0){
        std::cout<<"拍照信号触发，记录箱子尺寸，并记录时间戳开始等待箱体检测信息号"<<std::endl;
        signal = 1;
        //2. 记录测距
        mainData.box1signal = {plcData.laser1,plcData.laser1};
        mainData.box2signal = {plcData.laser3,plcData.laser4}; //若只有一层 laser直为最大不会使用
        mainData.mtimeStamp = getTimeStamp();
    }
    else if(plcData.flag1 && signal ==1){
        std::cout<<"检测到上侧箱体"<<std::endl;
        signal = 2;
        mainData.cell = 1;
    }
    //下层感应开关触发，且上层已经触发
    else if(plcData.flag2 && signal == 2){
        std::cout<<"检测到下侧箱体"<<std::endl;
        signal = 3;
        mainData.cell = 2;
    }
    _mutex.unlock();

    //箱子整体流程检测完毕重置信号状态
    if(signal==3){
        signal = 0;
        return 3;
    }
    return signal;
}

int64_t SignalProcess::getTimeStamp()
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

