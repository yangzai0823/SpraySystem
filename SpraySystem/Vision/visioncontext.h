#ifndef VISIONCONTEXT_H
#define VISIONCONTEXT_H

#include <vector>
#include <Eigen/Core>
#include <Eigen/Geometry>
#include "VWSCamera/VWSCamera.h"
#include <Eigen/Core>
#include <Eigen/Geometry>
#include "VWSVision/head/heightandpose/SizeAndPose.h"
#include "Data/StructData.h"

using ImageData = VWSCamera::ImageData;
using VisionData = vws::VisionData;
class VisionContext
{
public:

    VisionContext();

    /** @brief 头部视觉处理
     * @param ImageData,图像
     * @param senorNums, 测距值
    */
    void work_head(ImageData data,std::vector<float> senorNums, VisionData & VisionData);
    /** @brief 尾部视觉处理
     * @param ImageData,图像
     * @param senorNums, 拍照时刻编码器数值
    */
    void work_trail(ImageData data,std::vector<float> senorNums, VisionData & VisionData); 
    
    void getPoseAndHeight(ImageData data, VisionData &visionData);
    void getPoseANdHeight_Trail(ImageData data,VisionData &VisionData);
    /** @brief 计算深度
     * @param 测距传感器数值
     * @param 传感器相对距离,标定参数
     * @param VisionData
    */
    void getWidth(std::vector<float> senorNums, float senorDistance, VisionData &visionData);
    /**
     * @brief 计算长度
     * @param 两次拍照的编码器数值
     * @param VisionData
    */
    void getLenght( std::vector<float> encoerNUms, VisionData &visionData);
    void getPose(VisionData &visionData);
    void RobotCenterPose(vws::VisionData &visionData, double handEyeMatrix[12]);
    /**
     * @brief 使用指定长度生成箱体中心
    */
    void RobotCenterPose(vws::VisionData &visionData, double handEyeMatrix[12], double length);
 private:
     HTuple ImageConver(ImageData data);
     /**
      * @brief RobotCenterPose
      * @param visionData
      */
private:
    HTuple hv_CamParam;
    double hv_SetParas[6] = {900, 1720, -1220, 880, 6, 1e5};
};

#endif // VISIONCONTEXT_H
