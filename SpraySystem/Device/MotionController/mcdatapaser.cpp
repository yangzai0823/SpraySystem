#include "mcdatapaser.h"
#include <QtEndian>
#include <byteswap.h>

mcdatapaser::mcdatapaser()
{

}

void mcdatapaser::DataPaser(QByteArray buf, vws::MCData *data)
{
    static QByteArray buff2;
   buff2.append(buf);


   int var = buf.size();
#pragma pack(1)
    struct DDD{
        uint8_t head;
        int32_t a1;
        int32_t a2;
    };
#pragma pack()
    while (buff2.size()>9) {

        if(buff2.size()>=9){

            auto v1 = buff2.data();
            DDD* pd = (DDD*)v1;            
            auto d1 = bswap_32(pd->a1);
            auto d2 = bswap_32(pd->a2);

            // std::cout<<"MC head: "<<(int)(pd->head)<<std::endl;

            switch (pd->head) {
            case 6:   //心跳
                data->heart = d1;
                break;
            case 3:   //拍照时刻悬挂链
                data->bchainencoder=true;
                std::cout<<"拍照时刻悬挂链, 下相机： "<<std::to_string(d1)<<", 上相机: "<<std::to_string(d2)<<std::endl;
                data->encoder1 = d1;
                data->encoder2 = d2;
                break;
            case 4:  //实时编码器数值
                data->brealtimeencoder = true;
                data->realtimeencoder1 = d1;
                data->realtimeencoder2 = d2;
                break;
            case 5:  //获取规划参数
                std::cout<<"运动控制器请求参数"<<std::endl;
                data->btrajparam=true;
                data->getparam = d1;
                emit getTrajParam_Signal();
                break;
            default:
                break;
            }


            //清除前9个
            try {
                buff2.remove(0,9);
            } catch (QString ex)
            {

            }

        }
    }
}

void mcdatapaser::DataPaser(QByteArray buf)
{
    DataPaser(buf,mcData);
}
