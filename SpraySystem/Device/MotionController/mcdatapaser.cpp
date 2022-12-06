#include "mcdatapaser.h"
#include <QtEndian>
#include <byteswap.h>
#include "Util/Log/clog.h"

//直接计算法计算crc
u_int16_t mcdatapaser::do_crc(unsigned char *ptr, int len) {
    unsigned int i;
    u_int16_t crc = 0xFFFF;

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

u_int16_t to_int(char v1, char v2){
    QByteArray headArry;
        headArry.append(v1);
        headArry.append(v2);
    char * val = headArry.data();
    u_int16_t ret = *(u_int16_t*)val;
    return ret;
}

mcdatapaser::mcdatapaser()
{

}

void mcdatapaser::DataPaser(QByteArray buf, vws::MCData *mcdata)
{
    parse_mutex.lock();
    wholeBuf.append(buf);

    if(wholeBuf.size()>=15){
        int headIndex = -1;
        int trailIndex = -1;

        char* v1 = wholeBuf.data();
        int size = wholeBuf.size();
        for(int i=0;i<wholeBuf.size()-1;i++){
            u_int16_t v2 = to_int(v1[i+1], v1[i]);

            if(v2==0xabcd)
            {
                headIndex = i;
                //校验报尾
                int index = i+13;  //报头2个字节，计数2个字节，类别1个字节，数据8个字节
                
                u_int16_t trail = to_int(v1[index+1],v1[index]);
                unsigned char data[11];
                sub_str(data,v1,i+2,index);
                u_int16_t crc = do_crc(data,11);
                if(crc == trail){
                    trailIndex = index;

                    //解析数据
                   parseData(data,mcdata);
                    break;
                }
            }
        }
        if(headIndex != -1 && trailIndex!=-1){
            //处理一个完整数据包
            wholeBuf.remove(0,trailIndex+2);
        }else if(headIndex == -1){
            //未找到头
            wholeBuf.clear();
        }
    }
    parse_mutex.unlock();
}

void mcdatapaser::parseData(unsigned char* v1, vws::MCData *data)
{
#pragma pack(1)
    struct DDD{
        uint16_t head;
        u_int16_t num;
        uint8_t order;
        int32_t a1;
        int32_t a2;
    };
#pragma pack()
    DDD* pd = (DDD*)v1;       

    int32_t d1 = bswap_32(pd->a1);
    int32_t d2 = bswap_32(pd->a2);

    switch (pd->order) {
        case 34:   //心跳
            data->heart = d1;
            break;
        case 31:   //拍照时刻悬挂链
            data->b_chain_encoder=true;
            std::cout<<"拍照时刻悬挂链, 下相机： "<<std::to_string(d1)<<", 上相机: "<<std::to_string(d2)<<std::endl;
            data->encoder1 = d1;
            data->encoder2 = d2;
            break;
        case 32:  //实时编码器数值
            data->b_realtime_encoder = true;
            data->realtimeencoder1 = d1;
            data->realtimeencoder2 = d2;
            break;
        case 33:  //请求规划参数
            CLog::getInstance()->log("运动控制器请求参数");
            data->b_request_traj_param=true;
            data->getparam = d1;
            emit getTrajParam_Signal();
            break;
        case 35: //发送轨迹给机器人
            CLog::getInstance()->log("运动控制器请求发送轨迹给机器人");
            data->b_request_rbt_param = true;
            data->sendtorbt = d1;
            emit sendToRBT_Signal(pd->num);
            break;
        case 36:
            std::cout<<"运动控制器报错"<<std::endl;
            emit mcWarning_Signal(pd->num);
            break;
        case 0:
            //答应收到轨迹规划参数
            data->b_receive_traj_param = true;
            break;
        case 5:
            //应答复位消息
            data->b_receive_reset = true;
            break;
        case 6:
            //应答初次连接
            data->b_receive_start = true;
            break;
        case 7:
            //应答机器人发送结果
            data->b_receive_rbt_result = true;
        default:
            break;
    }
}


void mcdatapaser::DataPaser(QByteArray buf)
{
    DataPaser(buf,mcData);
}
