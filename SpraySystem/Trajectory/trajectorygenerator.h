#ifndef TRAJECTORYGENERATOR_H
#define TRAJECTORYGENERATOR_H
#include <Eigen/Core>
#include <Eigen/Geometry>
#include <vector>
#include <istream>
#include <iostream>

#include "planner/ompl_planning_class.h"
#include "planner/planning_utils.h"
#include "planner/utils.h"

#include "VWSRobot/VWSRobot.h"

class TrajectoryGenerator
{
public:
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

    TrajectoryGenerator();

    ~TrajectoryGenerator();

    /**
     * @brief 生成环境信息
     */
    void GenerateEnvirInfo();

    /**
     * @brief 箱体环境信息
     */
    void BoxEnvirInfo(Eigen::Vector3d boxcenter,Eigen::Vector3d boxsize,Eigen::Quaterniond boxq);

    /**
     * @brief 生成喷涂轨迹
     */
    void GenerateTrajectory(Eigen::Vector3d boxCenterPoint,Eigen::Vector3d boxSize,Eigen::Quaterniond boxq);

    /**
     * @brief 生成机器人喷涂任务
     */
     VWSRobot::RobotTask GenerateSprayTask();

    /**
     * @brief 计算悬挂链0点
     */
    std::vector<float> calChainZeroPoint(TrajParam param);

private:
    VwsPlanEnv *pt;
    OpenRAVE::RobotBasePtr robot;
    Eigen::VectorXd planRet;
    /**
     * @brief 编码器数值和实际距离转换系数
     */
    float chainFactor =0.4198727819755431f; //25/600;
    Eigen::Vector3d robotBeltDirection;
    Eigen::Isometry3d handEyeMatrix;
    Eigen::Vector3d extraAxisDirection;

private:
    Eigen::Vector3d topfarpoint(Eigen::Vector3d boxCenterPoint,Eigen::Vector3d boxSize);
    Eigen::Vector3d topnearpoint(Eigen::Vector3d boxCenterPoint,Eigen::Vector3d boxSize);
    Eigen::Vector3d bottomnearpont(Eigen::Vector3d boxCenterPoint,Eigen::Vector3d boxSize);
};

#endif // TRAJECTORYGENERATOR_H
