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
    if(this->flagtop!=0 && currentStamp - mainData.mtimeStampTop>vws::BottomSingalTimeOut){
        std::cout<<"触发时间超过规定时长，清空存储信息"<<std::endl;
        mainData.cell = 0;
        flagtop = 0;
        mainData.box1signal = {0,0};
    }

    if(plcData.flag4 && flagtop==0){
        std::cout<<"上相机拍照信号触发，记录箱子尺寸，并记录时间戳开始等待箱体检测信息号"<<std::endl;
        flagtop = 1;
        //2. 记录测距
        if(LaserOk(plcData.laser1,plcData.laser2)){
            mainData.box1signal = {plcData.laser1,plcData.laser2};
        }
        mainData.mtimeStampTop = currentStamp;
    }

    if(flagtop ==1 && mainData.box1signal.size()==0){
        if(LaserOk(plcData.laser1,plcData.laser2)){
            mainData.box1signal = {plcData.laser1,plcData.laser2};
        }
    }
}

uint32_t SignalProcess::bottomwork(vws::PLCData plcData, vws::MainData &mainData)
{
    //获取当前时间戳，对比触发时刻记录的时间戳
    int64_t currentStamp = getTimeStamp();
    if(this->flagbottom!=0 && currentStamp - mainData.mtimeStampBottom>vws::BottomSingalTimeOut){
        std::cout<<"下箱体触发时间超过规定时长，清空存储信息"<<std::endl;
        flagbottom = 0;
        mainData.currentBottom.box1signal.clear();
    }

    if(plcData.flag3){
        std::cout<<"下相机拍照信号触发，记录箱子尺寸，并记录时间戳开始等待箱体检测信息号"<<std::endl;
        mainData.currentBottom.mtimeStamp = currentStamp;
        mainData.currentBottom.flag=1;
        //2. 记录测距
        if(LaserOk(plcData.laser3,plcData.laser4)){
            mainData.currentBottom.box1signal = {plcData.laser3,plcData.laser4};
        }
    }
    else if(plcData.flag1){
        std::cout<<"检测到下侧箱体"<<std::endl;
        if(mainData.currentBottom.box1signal.size()==0){
            mainData.currentBottom.flag=2;
        }
        else{
            mainData.currentBottom.flag = 3;
        }
    }

    //测距失败重新采集
    if(mainData.currentBottom.flag==3){
        if(LaserOk(plcData.laser3,plcData.laser4)){
            mainData.currentBottom.box1signal = {plcData.laser3,plcData.laser4};
            mainData.currentBottom.flag =3;
        }
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
