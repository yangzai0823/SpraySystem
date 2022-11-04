#ifndef PLCDATA_H
#define PLCDATA_H
#include "VWSCamera/VWSCamera.h"
#include <iostream>
#include <vector>
#include <Eigen/Core>
#include <Eigen/Geometry>
using ImageData = VWSCamera::ImageData;

namespace vws{
    /**
     * @brief PLC数据
     */
    struct PLCData{
        int heart;
        float laser1;
        float laser2;
        float laser3;
        float laser4;
        /**
         * @brief 上方箱体感应
         */
        bool flag1;
        /**
         * @brief 下方箱体感应
         */
        bool flag2;
        /**
         * @brief 相机信号
         */
        bool flag3;
    };

    /**
     * @brief 主程序
     */
    struct MainData{
        ImageData  image1;
        ImageData  image2;
        /**
         * @brief 上册箱体信息：测距1、测距2
         */
        std::vector<float> box1signal;
        /**
         * @brief 下册箱体信息：测距1、测距2
         */
        std::vector<float> box2signal;
        int cell = 0;
        int64_t mtimeStamp = 0;
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
