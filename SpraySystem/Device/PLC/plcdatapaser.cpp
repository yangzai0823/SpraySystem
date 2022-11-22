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
    struct DDD
    {
        int16_t laser1;
        int16_t laser2;
        int16_t laser3;
        int16_t laser4;
        int16_t laser5;
        int16_t laser6;
        int16_t laser7;
        int16_t laser8;
        uint8_t flag1 : 1;
        uint8_t flag2 : 1;
        uint8_t flag_camera_b : 1;
        uint8_t flag_cmaera_u : 1;
    };
#pragma pack()

    char *v1 = buf.data();
    DDD *pd = (DDD *)v1;
    if (data != nullptr)
    {
        data->laser1 = pd->laser1;
        data->laser2 = pd->laser2;
        data->laser3 = pd->laser3;
        data->laser4 = pd->laser4;

        data->flag_laser_u = (bool)pd->flag1;
        data->flag_laser_b = (bool)pd->flag2;
        data->flag_camera_b = (bool)pd->flag_camera_b;
        data->flag_camera_u = (bool)pd->flag_cmaera_u;
    }
}

void PLCDataPaser::DataPaser(QByteArray buf)
{
    isemit = false; //默认不发送
    //解析plc信号
    DataPaser(buf, plcdata);

    //监测信号变化，触发plc信号； 注意：箱体晃动可能导致信号多次触发，通过上层状态转换避免重复操作
    if (!init)
    {
        std::cout<<"plc 数据初始化"<<std::endl;
        init = true;
        pre_camera_b = plcdata->flag_camera_b;
        pre_camera_u = plcdata->flag_camera_u;
        pre_laser_b = plcdata->flag_laser_b;
        pre_laser_u = plcdata->flag_laser_u;
        return;
    }
    else
    {
        if (plcdata->flag_camera_b == true && pre_camera_b == false)
        {
            // std::cout << "下相机，低-> 高" << std::endl;

            pre_camera_b = plcdata->flag_camera_b; //更新
            isemit = true;
        }
        else if (plcdata->flag_camera_b == false && pre_camera_b == true)
        {
            // std::cout << "下相机，高-> 低" << std::endl;

            pre_camera_b = plcdata->flag_camera_b; //更新
            isemit = true;
        }

        if(plcdata->flag_camera_u == true && pre_camera_u == false){
            // std::cout << "上相机，低-> 高"<<std::endl;
            pre_camera_u = plcdata->flag_camera_u;
            isemit = true;
        }
        else if(plcdata->flag_camera_u == false && pre_camera_u == true){
        // std::cout << "上相机，高-> 低" << std::endl;

            pre_camera_u = plcdata->flag_camera_u; //更新
            isemit = true;
        }

        if (plcdata->flag_laser_u != pre_laser_u)
        {
            if (plcdata->flag_laser_u == true && pre_laser_u == false)
            {
                // std::cout << "上箱体检测信息号" << std::endl;
                isemit = true;
            }
            pre_laser_u = plcdata->flag_laser_u;
        }
        if (plcdata->flag_laser_b != pre_laser_b)
        {
            if (plcdata->flag_laser_b == true && pre_laser_b == false)
            {
                std::cout << "下箱体检测信息号" << std::endl;
                isemit = true;
            }
            pre_laser_b = plcdata->flag_laser_b;
        }
    }

    if (isemit)
    {
        std::cout << "触发plc信号" << std::endl;
        emit readyRead_Signal();
    }
}
