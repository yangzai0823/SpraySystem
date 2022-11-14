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

    void work(ImageData data,Eigen::Isometry3d handEyeMatrix,VisionData & visionData);
    void work_head(ImageData data, VisionData & VisionData);
    void getPoseAndHeight(ImageData data, VisionData &visionData);
    /** @brief 计算深度
     * @param 测距传感器数值
     * @param 传感器相对距离,标定参数
     * @param VisionData
    */
    void getWidth(std::vector<float> senorNums, float senorDistance, double * normalvector);
    /**
     * @brief 计算长度
     * @param 标定编码器方向向量
     * @param 两次拍照的编码器数值
     * @param VisionData
    */
    void getLenght(std::vector<float> encoderVector, std::vector<float> encoerNUms, VisionData &visionData);
    void getPose(VisionData &visionData);

    void visionProcessFunc(const VWSCamera::ImageData &data, vws::VisionData &visionData);
 private:
     HTuple ImageConver(ImageData data);
     /**
      * @brief RobotCenterPose
      * @param visionData
      */
     void RobotCenterPose(VisionData &visionData, double handEyeMatrix[12]);
private:
    HTuple hv_CamParam;
    double hv_SetParas[6] = {900, 1720, -1220, 880, 6, 1e5};
};

#endif // VISIONCONTEXT_H
