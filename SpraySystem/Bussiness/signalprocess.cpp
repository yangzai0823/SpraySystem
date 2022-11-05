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
    uint32_t ret = 0;
    _mutex.lock();
//    auto retTop = topwork(plcData,mainData);
    auto retBottom = bottomwork(plcData,mainData);
    if(retBottom==3){
       ret =2;
    }
    _mutex.unlock();
    return ret;
}

uint32_t SignalProcess::topwork(vws::PLCData plcData, vws::MainData &mainData)
{
    //获取当前时间戳，对比触发时刻记录的时间戳
    int64_t currentStamp = getTimeStamp();
    if(mainData.currentTop.flag!=0 && currentStamp - mainData.currentTop.mtimeStamp>vws::TopSingalTimeOut){
        mainData.currentTop.flag = 0;
        std::cout<<"触发时间超过规定时长，清空存储信息"<<std::endl;
    }

    if(plcData.flag_camera_b && mainData.currentTop.flag==0){
        std::cout<<"上相机拍照信号触发，记录箱子尺寸，并记录时间戳开始等待箱体检测信息号"<<std::endl;
        mainData.currentTop.mtimeStamp = currentStamp;
        mainData.currentTop.flag = 1;
        //2. 记录测距
        mainData.currentTop.box1signal = {plcData.laser1,plcData.laser2};
        //获取拍照时刻编码器数值
        mainData.currentTop.imgEncoder = DeviceManager::getInstance()->getMC()->getChainEncoders()[topface];
        mainData.currentTop.face = topface;
        if(topface ==0){
            topface =1;
        }else{
            topface = 0;
        }
    }
    else if(plcData.flag_laser_b){
       std::cout<<"检测到上侧箱体"<<std::endl;
        mainData.currentTop.flag = 2;
    }

    return mainData.currentTop.flag;
}

uint32_t SignalProcess::bottomwork(vws::PLCData plcData, vws::MainData &mainData)
{
    //静态参数，前后两次相机触发共用一个参数
    static std::vector<float> staticSignal;
    //获取当前时间戳，对比触发时刻记录的时间戳
    int64_t currentStamp = getTimeStamp();
    if(mainData.currentBottom.flag!=0 && currentStamp - mainData.currentBottom.mtimeStamp>vws::BottomSingalTimeOut
    || mainData.currentBottom.flag ==5){
        std::cout<<"下箱体触发时间超过规定时长 或 数据已被处理，清空存储信息"<<std::endl;
        mainData.currentBottom.flag = 0;
        mainData.currentBottom.box1signal.clear();
    }
    
    if(plcData.flag_camera_b && !bottomflag && mainData.currentBottom.flag == 0){
        std::cout<<"下相机第一次拍照信号触发，记录箱子尺寸，并记录时间戳开始等待箱体检测信息号"<<std::endl;
        bottomflag = 1;

        mainData.currentBottom.mtimeStamp = currentStamp;
        mainData.currentBottom.flag=1;
        //2. 记录测距
        mainData.currentBottom.box1signal = {plcData.laser3,plcData.laser4};
        staticSignal = mainData.currentBottom.box1signal;
        //获取拍照时刻编码器数值
        mainData.currentBottom.imgEncoder = DeviceManager::getInstance()->getMC()->getChainEncoders()[0];
        mainData.currentBottom.face = 0;
    }
    else if(!plcData.flag_camera_b && bottomflag){
        std::cout<<"下相机第二次拍照触发信息号，记录前一次箱子尺寸"<<std::endl;
        bottomflag = 0;
        mainData.currentBottom.flag = 2;
        mainData.currentBottom.imgEncoder = DeviceManager::getInstance()->getMC()->getChainEncoders()[1];
        mainData.currentBottom.face = 1;
        mainData.currentBottom.box1signal = staticSignal;
    }
    else if(plcData.flag_laser_b && mainData.currentBottom.flag ==1){
        std::cout<<"检测到下侧箱体"<<std::endl;
        mainData.currentBottom.flag = 2;
    }

    return mainData.currentBottom.flag;
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

bool SignalProcess::LaserOk(float laser1, float laser2)
{
    if(laser1>vws::LaserBottom && laser1<vws::LaserUp && laser2>vws::LaserBottom && laser2<vws::LaserUp)
    {
        return true;
    }
    return false;
}
