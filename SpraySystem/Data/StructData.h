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

namespace vws{
    /**
     * @brief PLC数据
     */
    struct PLCData{
        int heart;
        float laser1 = 0;
        float laser2 = 0;
        float laser3 = 0;
        float laser4 = 0;
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
     * @brief 流程数据
     */
    struct ProcessData{
//        SignalState::State state = SignalState::State::IDLE;
        bool imgFlag=false;
        bool imgFlag_trail= false;
        bool flag_laser_b = false;
        bool flag_camera_b = false;
        std::vector<ImageData> image;

        /**
         * @brief 箱体信息：测距1、测距2
         */
        std::vector<float> box1signal;

        int64_t mtimeStamp = 0;
        /**
         * @brief 拍照时刻编码器数值
        */
        int64_t imgEncoder;
        /*
        * @brief 前后面，0：前面，1：后面
        */
        int face;
    };

    /**
     * @brief 主程序
     */
    struct MainData{
        ProcessData currentTop;
        ProcessData currentBottom;
    };

    /**
     * @brief 轨迹规划任务信息
     */
    struct PlanTaskInfo{
        bool flag  = false; //true: 以规划
        float lx;
        float ly;
        float lz;
        
        Eigen::Isometry3d boxInfo;
        
        int face;  //0：前面， 1：后面
        int64_t encoder; //拍照时刻
        float diff;  //偏移
    };

    /**
     * @brief 运动控制器数据
     */
    struct MCData{
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
    struct VisionData{
        float length;
        float width;
        float height;
        std::vector<float> imgpose;
        std::vector<float> robotpose;
        std::vector<float> righttop;
        std::vector<float> rightbottom;
        std::vector<float> lefttop;
        std::vector<float> leftbottom;
        std::vector<float> normalvector; /*前三位为法向量*/
    };

    /**
     * @brief 轨迹规划参数
     */
    struct TrajParam{
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
