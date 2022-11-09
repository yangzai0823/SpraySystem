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
    void getPoseAndHeight(ImageData data, VisionData &visionData);
    void getWidth(std::vector<float> senorNums, float senorDistance, VisionData &visionData);
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
};

#endif // VISIONCONTEXT_H
