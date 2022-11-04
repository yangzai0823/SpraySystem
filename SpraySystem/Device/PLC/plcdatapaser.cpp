#include "plcdatapaser.h"
#include <QtEndian>
#include <byteswap.h>
#include <iostream>

PLCDataPaser::PLCDataPaser()
{

}

void PLCDataPaser::DataPaser(QByteArray buf, vws::PLCData &data)
{
#pragma pack(1)
    struct DDD{
        int16_t laser1;
        int16_t laser2;
        int16_t laser3;
        int16_t laser4;

        uint8_t flag1;
        uint8_t flag2;
    };
#pragma pack()

     char* v1 = buf.data();
     DDD* pd = (DDD*)v1;            

     uint8_t b1=false;
     uint8_t b2=false;
     uint8_t b3=false;
     uint8_t b4=false;

     b1 = (pd->flag1&0xf0)>>4;
     b2 = pd->flag1&0xf;
     b3 = (pd->flag2&0xf0)>>4;
     b4 = pd->flag2&0xf;

     data.laser1 = pd->laser1;
     data.laser2 = pd->laser2;
     data.laser3 = pd->laser3;
     data.laser4 = pd->laser4;
     data.flag1 = (bool)b1;
     data.flag2 = (bool)b2;
     data.flag3 = (bool)b3;
}
