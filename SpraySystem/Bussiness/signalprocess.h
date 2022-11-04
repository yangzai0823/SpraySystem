#ifndef SINGALPROCESS_H
#define SINGALPROCESS_H
#include "Data/StructData.h"

class SignalProcess
{
public:
    SignalProcess();
    /**
     * @brief 处理PLC信号，0:空闲，1：拍照，2：上方箱体信息采集完毕，3：下方箱体信息采集完毕
     * @param plcData
     * @param mainData
     * @return
     */
    uint32_t work(vws::PLCData plcData,vws::MainData & mainData);

private:
    /**
     * @brief 0:空闲状态，1:拍照, 2:检测到上方箱体，3:检测到下方箱体
     */
    uint32_t signal=0;
private:
    int64_t getTimeStamp();
};

#endif // SINGALPROCESS_H
