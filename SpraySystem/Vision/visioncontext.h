#ifndef VISIONCONTEXT_H
#define VISIONCONTEXT_H

#include <vector>
#include <Eigen/Core>
#include <Eigen/Geometry>
#include "VWSCamera/VWSCamera.h"
using ImageData = VWSCamera::ImageData;

class VisionContext
{
public:
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
        std::vector<float> normalvector;
    };
    VisionContext();

    void getPoseAndHeight(ImageData image, VisionData &visionData);
    void getWidth(std::vector<float> senorNums, float senorDistance, VisionData &visionData);
    void getLenght(std::vector<float> encoderVector, std::vector<float> encoerNUms, VisionData &visionData);
    void getPose(VisionData &visionData);
};

#endif // VISIONCONTEXT_H
