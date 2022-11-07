#include "plcdatapaser.h"
#include <QtEndian>
#include <byteswap.h>
#include <iostream>

PLCDataPaser::PLCDataPaser()
{

}

void PLCDataPaser::DataPaser(QByteArray buf, vws::PLCData *data)
{
#pragma pack(1)
    struct DDD{
        int16_t laser1;
        int16_t laser2;
        int16_t laser3;
        int16_t laser4;

        uint8_t flag1 : 1;
        uint8_t flag2 : 1;
        uint8_t flag_camera_b : 1;
        uint8_t flag_cmaera_u : 1;
    };
#pragma pack()

     char* v1 = buf.data();
     DDD* pd = (DDD*)v1;            
    if(data!=nullptr){
        data->laser1 = pd->laser1;
        data->laser2 = pd->laser2;
        data->laser3 = pd->laser3;
        data->laser4 = pd->laser4;
        data->flag_laser_u = (bool)pd->flag1;
        data->flag_laser_b = (bool)pd->flag2;
        data->flag_camera_b = (bool)pd->flag_camera_b;
        data->flag_camera_u = (bool)pd->flag_cmaera_u;
    }
     //todo: 相机触发高低位变换
}

void PLCDataPaser::DataPaser(QByteArray buf)
{
    DataPaser(buf,plcdata);
    emit readyRead_Signal();
    
}
