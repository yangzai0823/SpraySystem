#include "HeadDistanceMonitor.h"
#include <unistd.h>
#include "Device/devicemanager.h"
#include "Data/StaticData.h"

void HeadDistanceMonitor::Work_Slot(float camera_encoder)
{
    flag_stop = false;
    while (!flag_stop)
    {
        /* code */
//      float realtime_encoder = DeviceManager::getInstance()->getMC()->getRealTimeEncoder()[0];
//        float diff = (camera_encoder-realtime_encoder) * vws::chainFactor;
//        if(diff>=vws::HeadMoveMaxLength){
//            //std::cout<<"头部运行距离超出规定范围"<<std::endl;
//            emit outOfLimit_Signal();
//        }
//       usleep(10);
    }
    
}

void HeadDistanceMonitor::Stop()
{
    flag_stop = true;
}
