#ifndef SINGALPROCESS_H
#define SINGALPROCESS_H
#include "Data/StructData.h"
#include <QQueue>

class SignalProcess
{
public:
    SignalProcess();
    /**
     * @brief 1: 上层完成，2：下层完成
     * @param plcData
     * @param mainData
     * @return
     */
    uint32_t work(vws::PLCData plcData,vws::MainData & mainData);
private:
    uint32_t topwork(vws::PLCData plcData,vws::MainData &mainData);
    uint32_t bottomwork(vws::PLCData plcData,vws::MainData &mainData);
    int64_t getTimeStamp();
    bool LaserOk(float laser1, float laser2);
private:
    bool topflag = 0;
    bool bottomflag = 0;
    /**
     * @brief 0:前面，1：后面
    */
    int topface=0; 
        /**
     * @brief 0:前面，1：后面
    */
    int bottomface=0; 
};

#endif // SINGALPROCESS_H
