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
    /**
     * @brief 0:空闲状态，1:拍照, 2:检测到上方箱体，3:检测到下方箱体
     * 4：上方箱体测距数据异常，重新采集
     * 5：下方箱体测距数据异常，重新采集
     */
    uint16_t flagtop = 0;
    uint16_t flagbottom = 0;
private:
    uint32_t topwork(vws::PLCData plcData,vws::MainData &mainData);
    uint32_t bottomwork(vws::PLCData plcData,vws::MainData &mainData);
    int64_t getTimeStamp();
    bool LaserOk(float laser1, float laser2);
};

#endif // SINGALPROCESS_H
