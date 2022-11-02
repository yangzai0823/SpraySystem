#ifndef TRAJECTORYCONTEXT_H
#define TRAJECTORYCONTEXT_H

#include <Eigen/Core>
#include <Eigen/Geometry>
#include <QObject>
#include "Strategy/basestrategy.h"
#include "trajectorygenerator.h"
#include "VWSRobot/VWSRobot.h"
using RobotTask = VWSRobot::RobotTask;
using TrajParam = TrajectoryGenerator::TrajParam;

Q_DECLARE_METATYPE(TrajParam)  //注册结构体
class TrajectoryContext
{
public:
    TrajectoryContext(TrajectoryGenerator *generator,BaseStrategy *planstrategy,BaseStrategy *sevenstrategy);
    ~TrajectoryContext();

    /**
     * @brief 轨迹规划
     * @return
     */
    RobotTask  Work(TrajParam param);

    std::vector<float> calChainZeroPoint(TrajParam param);
private:
    BaseStrategy* planstrategy;
    BaseStrategy* sevenstrategy;
    TrajectoryGenerator* generator;

    Eigen::Vector3d bottomnearpont(Eigen::Vector3d boxCenterPoint,Eigen::Vector3d boxSize);
};

#endif // TRAJECTORYCONTEXT_H
