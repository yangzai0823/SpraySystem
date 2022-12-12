#include "mcdatapaser.h"
#include <QtEndian>
#include <byteswap.h>
#include "Util/Log/clog.h"
#include <QDate>
#include <QString>
#include <iostream>
#include <thread>


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

    // std::thread::id id1 = std::this_thread::get_id();
    // std::cout << "DataPaser 线程ID: "<< id1 << std::endl;


    wholeBuf.append(buf);

    if(wholeBuf.size()>=N){
        int headIndex = -1;
        int trailIndex = -1;

        char* v1 = wholeBuf.data();
        int size = wholeBuf.size();
        for(int i=0;i<wholeBuf.size();i++){
            u_int16_t v2 = to_int(v1[i+1], v1[i]);

            if(v2==0xabcd)
            {
                headIndex = i;
                //校验报尾 
                int index = i+N_Data+2;  //报头2个字节，数据28位
                
                u_int16_t trail = to_int(v1[index+1],v1[index]);
                unsigned char data[N_Data];
                // sub_str(data,v1,i+2,index);
                memcpy(data,v1+i+2,N_Data);
                u_int16_t crc = do_crc(data,N_Data);
                if(crc == trail){
                    trailIndex = index;

                    //解析数据
                    char data2[N];

                    memcpy(data2,v1+i,N);

                    // reply_order = (char *)data2;

                    parseData(data2,mcdata);
                    break;
                }
                else 
                {
                    CLog::getInstance()->log("尾部校验失败");
                }
            }
            else{
                    CLog::getInstance()->log("头部校验失败");
            }
        }
        if(headIndex != -1 && trailIndex!=-1){
            //处理一个完整数据包
            if(wholeBuf.size()>=trailIndex+2){
                wholeBuf.remove(0,trailIndex+2);
            }
        }else if(headIndex == -1){
            //未找到头
            wholeBuf.clear();
        }
    }
    parse_mutex.unlock();
}

void mcdatapaser::parseData(char* v1, vws::MCData *data)
{
#pragma pack(1)
    struct DDD{
        uint16_t head;
        u_int16_t num;
        u_int8_t axisNum;
        uint8_t order;
        int32_t a1;
        int32_t a2;
        int32_t a3;
        int32_t a4;
        int32_t a5;
        int32_t a6;
        uint16_t trail;
    };
#pragma pack()
    DDD* pd = (DDD*)v1;       

    int32_t d1 = bswap_32(pd->a1);
    int32_t d2 = bswap_32(pd->a2);

    u_int16_t dnum=  bswap_16(pd->num);
    data->order = pd->order;
    switch (pd->order) {
        case 34:   //心跳
            data->heart = d1;
            break;
        case 31:   //拍照时刻悬挂链
            data->b_chain_encoder=true;
            std::cout<<"拍照时刻悬挂链, 下相机： "<<std::to_string(d1)<<", 上相机: "<<std::to_string(d2)<<std::endl;
            data->encoder1 = d1;
            data->encoder2 = d2;

            semaphore_master.release();
            break;
        case 32:  //实时编码器数值
            data->b_realtime_encoder = true;
            data->realtimeencoder1 = d1;
            data->realtimeencoder2 = d2;

            semaphore_master.release();
            break;
        case 33:  //请求规划参数
        {
            // QString current_date_time = QDateTime::currentDateTime().toString(Qt::ISODateWithMs);
            // std::cout<<"MC请求轨迹规划参数: "<<current_date_time.toStdString()<<std::endl;
            CLog::getInstance()->log("MC Receive, MC请求轨迹规划参数");


            data->b_request_traj_param=true;
            data->getparam = d1;

            memcpy(reply_order,v1,N);
   
            semaphone_slave.release();

            break;
        }
        case 35: //发送轨迹给机器人
        {
            CLog::getInstance()->log("MC Receive, 请求发送轨迹给机器人");
            data->b_request_rbt_param = true;
            data->sendtorbt = d1;

            memcpy(reply_order,v1,N);

            semaphone_slave.release();

            break;
        }
        case 36:
            std::cout<<"运动控制器报错"<<std::endl;
            emit mcWarning_Signal(dnum);
            break;
        case 0:
            // std::cout<<"答应收到轨迹规划参数"<<std::endl;
            CLog::getInstance()->log("答应收到轨迹规划参数");
            data->b_receive_traj_param = true;
            semaphore_master.release();
            break;
        case 5:
            std::cout<<"应答复位消息"<<std::endl;
            data->b_receive_reset = true;
            semaphore_master.release();
            break;
        case 6:
        {
            std::cout<<"应答初次连接"<<std::endl;
            data->b_receive_start = true;
            semaphore_master.release();
            break;
        }
        case 7:
            std::cout<<"应答机器人发送结果"<<std::endl;
            data->b_receive_rbt_result = true;
            semaphore_master.release();
        default:
            break;
    }
}


void mcdatapaser::DataPaser(QByteArray buf)
{
    DataPaser(buf,mcData);
}