#include "mcdatapaser.h"
#include <QtEndian>
#include <byteswap.h>
#include "Util/Log/clog.h"

//直接计算法计算crc
int16_t mcdatapaser::do_crc(unsigned char *ptr, int len) {
    unsigned int i;
    int16_t crc = 0xFFFF;

    while (len--) {
        crc ^= *ptr++;
        for (i = 0; i < 8; ++i) {
            if (crc & 1)
                crc = (crc >> 1) ^ 0xA001;
            else
                crc = (crc >> 1);
        }
    }

    return crc;
}

char* sub_str(unsigned char* dest,char* source, int start, int end){
    for(int i=0;i< (end -start);i++){
        dest[i] = source[start+i];
    }
}


mcdatapaser::mcdatapaser()
{

}

void mcdatapaser::DataPaser(QByteArray buf, vws::MCData *mcdata)
{
    char* v1 = buf.data();
    int size = strlen(v1);
    for(int i=0;i<strlen(v1)-1;i++){
        auto v2 =  (v1[i]<<8)+v1[i+1];
        if(v2==0xabcd)
        {
            //校验报尾
            int index = i+13;  //报头2个字节，计数2个字节，类别1个字节，数据8个字节
            auto trail = (v1[index]<<8)+v1[index+1];
            unsigned char * data;
            sub_str(data,v1,i+2,index);
            auto crc = do_crc(data,11);
            if(crc == trail){
                //解析数据
                parseData(data,mcdata);
                break;
            }
        }
    }
   
}

void mcdatapaser::parseData(unsigned char* v1, vws::MCData *data)
{
#pragma pack(1)
    struct DDD{
        uint16_t head;
        uint8_t order;
        int32_t a1;
        int32_t a2;
    };
#pragma pack()
     DDD* pd = (DDD*)v1;       
    uint16_t head = bswap_16(pd->head);
    //校验报头， 0xabcd
    if(head == 0xabcd){

    }

    int32_t d1 = bswap_32(pd->a1);
    int32_t d2 = bswap_32(pd->a2);

    // std::cout<<"MC head: "<<(int)(pd->head)<<std::endl;

    switch (pd->order) {
        case 34:   //心跳
            data->heart = d1;
            break;
        case 31:   //拍照时刻悬挂链
            data->bchainencoder=true;
            std::cout<<"拍照时刻悬挂链, 下相机： "<<std::to_string(d1)<<", 上相机: "<<std::to_string(d2)<<std::endl;
            data->encoder1 = d1;
            data->encoder2 = d2;
            break;
        case 32:  //实时编码器数值
            data->brealtimeencoder = true;
            data->realtimeencoder1 = d1;
            data->realtimeencoder2 = d2;
            break;
        case 33:  //获取规划参数
            CLog::getInstance()->log("运动控制器请求参数");
            data->btrajparam=true;
            data->getparam = d1;
            emit getTrajParam_Signal();
            break;
        case 36: //发送轨迹给机器人
            CLog::getInstance()->log("运动控制器请求发送轨迹给机器人");
            data->brbt = true;
            data->sendtorbt = d1;
            emit sendToRBT_Signal();
            break;
        case 35:
            std::cout<<"运动控制器复位成功"<<std::endl;
            data->breset = true;
            break;
        case 37:
            std::cout<<"运动控制器报错"<<std::endl;
        default:
            break;
    }
}


void mcdatapaser::DataPaser(QByteArray buf)
{
    DataPaser(buf,mcData);
}
