#include "trajectorycontext.h"

TrajectoryContext::TrajectoryContext(TrajectoryGenerator *generator,BaseStrategy *planstrategy,BaseStrategy *sevenstrategy)
{
    this->planstrategy = planstrategy;
    this->sevenstrategy = sevenstrategy;
    this->generator = generator;

}

TrajectoryContext::~TrajectoryContext()
{
    delete planstrategy;
    delete sevenstrategy;
    delete generator;
}

RobotTask  TrajectoryContext::Work(TrajParam param)
{

    auto p1 = bottomnearpont(param.boxCenterPoint,param.boxSize);
    auto y = p1[1]+ param.offsetOfTraj/1000.0;
    param.boxCenterPoint[1] = param.boxCenterPoint[1] - y;

    generator->GenerateEnvirInfo();
    generator->BoxEnvirInfo(param.boxCenterPoint,param.boxSize,param.boxQuat);
    generator->GenerateTrajectory(param.boxCenterPoint,param.boxSize,param.boxQuat);
    auto rbtTask = generator->GenerateSprayTask();

    return rbtTask;
}

std::vector<float> TrajectoryContext::calChainZeroPoint(TrajParam param)
{
    auto ret = generator->calChainZeroPoint(param);
    return ret;
}


Eigen::Vector3d TrajectoryContext::bottomnearpont(Eigen::Vector3d boxCenterPoint,Eigen::Vector3d boxSize)
{
    double x= boxCenterPoint[0] - boxSize[0]/2;
    double y =boxCenterPoint[1] + boxSize[1]/2;
    double z= boxCenterPoint[2] - boxSize[2]/2;
    Eigen::Vector3d p1(x,y,z);

    return p1;
}
