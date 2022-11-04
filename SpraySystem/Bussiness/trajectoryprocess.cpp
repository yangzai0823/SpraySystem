#include "trajectoryprocess.h"


TrajectoryProcess::TrajectoryProcess()
{
    visionContext = std::make_shared<VisionContext>();
}

void TrajectoryProcess::begintraj_Slot(QVariant vdata)
{

    std::cout<<"开始规划"<<std::endl;

    TrajectoryGenerator *generator =  TrajectoryGenerator::Instance();
            BaseStrategy *planstrategy;
            BaseStrategy *sevenstrategy;
    auto context = std::make_shared<TrajectoryContext>(generator,planstrategy,sevenstrategy);
    //1. 计算悬挂链0点


    //1. 对设开关测量值

    //2. 上下相机图像信息

    //3. 轨迹规划


    TrajParam param = vdata.value<TrajParam>();
    auto mcdata = context->calChainZeroPoint(param);

    TrajParam newparam = param;


    newparam.boxCenterPoint = newparam.boxCenterPoint/1000;

    newparam.boxSize = newparam.boxSize/1000;
    auto rbtdata = context->Work(newparam);


    QVariant varmc;
    QVariant varrbt;
    varmc.setValue(mcdata);
    varrbt.setValue(rbtdata);
    emit traj_Signal(varmc,varrbt);

}
