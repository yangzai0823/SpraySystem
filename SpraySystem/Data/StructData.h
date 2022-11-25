#ifndef PLCDATA_H
#define PLCDATA_H
#include "VWSCamera/VWSCamera.h"
//#include "SignalState.h"
#include <iostream>
#include <vector>
#include <Eigen/Core>
#include <Eigen/Geometry>
#include <boost/foreach.hpp>
#include <QQueue>

using ImageData = VWSCamera::ImageData;

namespace vws
{
    /**
     * @brief PLC数据
     */
    struct PLCData
    {
        int heart;
        float laser_up_head = 0;
        float laser_up_behind = 0;
        float laser_bottom_head = 0;
        float laser_bottom_behind = 0;
        /**
         * @brief 上层箱体感应信号
         */
        bool flag_laser_u = 0;
        /**
         * @brief 下层箱体感应信号
         */
        bool flag_laser_b = 0;
        /**
         * @brief 上相机拍照信号，0：尾部触发，1：头部触发
         */
        bool flag_camera_b = 0;
        /**
         * @brief 下相机拍照信号，0：尾部触发，1：头部触发
         */
        bool flag_camera_u = 0;
    };

    /**
     * @brief 轨迹规划任务信息
     */
    struct PlanTaskInfo
    {
        bool flag = false; // true: 以规划
        float lx;
        float ly;
        float lz;

        Eigen::Isometry3d boxInfo;

        int face;        // 0：前面， 1：后面
        int64_t encoder; //拍照时刻
        float diff;      //偏移
        bool isup;          // true: 上层， false ： 下层
    };

    /**
     * @brief 运动控制器数据
     */
    struct MCData
    {
        std::vector<uint8_t> headvect;
        int encoder1;
        int encoder2;
        int realtimeencoder1;
        int realtimeencoder2;
        int getparam;
        int heart;

        bool bchainencoder = false;
        bool brealtimeencoder = false;
        bool btrajparam = false;
    };

    /**
     * @brief 视觉数据
     */
    struct VisionData
    {
        float length;
        float length_head;
        float width;
        float height;
        /** @brief 头部信息是否已使用固定长度计算*/
        bool head_done=false;
        std::vector<float> imgpose;
        std::vector<double> robotpose;

        std::vector<double> robotpose_head;

        std::vector<float> imgpose_trail;
        std::vector<double> robotpose_trail;
        
        std::vector<double> righttop;
        std::vector<double> rightbottom;
        std::vector<double> lefttop;
        std::vector<double> leftbottom;
        std::vector<double> normalvector; /*前三位为法向量*/
        std::vector<double> normalvector_head;

        /** @brief 0: top, 1: bottom*/
        bool top_or_bottom; 
    };
    /**
     * @brief 轨迹规划参数
     */
    struct TrajParam
    {
        Eigen::Vector3d boxCenterPoint;
        Eigen::Vector3d boxSize;
        Eigen::Quaterniond boxQuat;
        /**
         * @brief 拍照时刻编码器数值
         */
        float encoder;
        float sevenEncoder;
        /**
         * @brief 跟踪偏移量
         */
        float offsetOfTraj;
    };
} 
#endif // PLCDATA_H
