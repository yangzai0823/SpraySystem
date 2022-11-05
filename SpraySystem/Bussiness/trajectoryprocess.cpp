#include "trajectoryprocess.h"
#include "mainprocess.h"

TrajectoryProcess::TrajectoryProcess()
{
    visionContext = std::make_shared<VisionContext>();
}


bool ahead(uint64_t a, uint64_t relative, bool dir){
    if(dir){
      return a < relative;
    }else{
      return a > relative;
    }
}

std::vector<vws::PlanTaskInfo> TrajectoryProcess::tryGetPlanTask(MainProcess* vdata, vws::PlanTaskInfo *task,
                                                std::vector<vws::PlanTaskInfo> &q1,
                                                std::vector<vws::PlanTaskInfo> &q2,
                                                uint64_t encoder_off1, uint64_t encoder_off2,
                                                bool dir){
    std::vector<vws::PlanTaskInfo> env_info;
     float valid_range = 3000;       //mm
    uint64_t current_encoder = vdata->getChainEncoder();
    bool isIncrease = vdata->getChainEncoderDir();
    float units = vdata->getChainUnits();
    float speed = vdata->getChainSpeed();  // mm/s

        float expire_t = 4 * 1000 / speed;
    uint64_t expire_en = expire_t * speed * units;
    uint64_t plan_range = 1000 * units;
    uint64_t plan_delay = 1200 * units;
    int equal_th = 400;
    bool face = task->face;

    // 规划上层前端面
    // 看下层编码器
    auto encoder = task->encoder + encoder_off1;
      

    for (int i = 0; i < q2.size(); i++) {
      vws::PlanTaskInfo btask = q2[i];
      if (llabs(btask.encoder + encoder_off2 - encoder) < equal_th) {
        env_info.push_back(btask);
      } else if (ahead(btask.encoder + encoder_off2, encoder, isIncrease) &&
                 btask.face == !face &&
                 plan_range > llabs(btask.encoder + encoder_off2 - encoder)) {
        env_info.push_back(btask);
      } else if (!ahead(btask.encoder + encoder_off2, encoder, isIncrease) &&
                 btask.face == face &&
                 plan_range > llabs(btask.encoder + encoder_off2 - encoder)) {
        env_info.push_back(btask);
      }
    }
    for (int i = 0; i < q1.size(); i++) {
      vws::PlanTaskInfo utask = q1[i];
      if (utask.encoder + encoder_off1 == encoder) continue;
      if (ahead(utask.encoder + encoder_off1, encoder, isIncrease) && utask.face == 1 &&
          plan_range > llabs(utask.encoder + encoder_off1 - encoder)) {
        env_info.push_back(utask);
      } else if (!ahead(utask.encoder + encoder_off1, encoder, isIncrease) &&
                  utask.face == 0 &&
                  plan_range > llabs(utask.encoder + encoder_off1 - encoder)) {
        env_info.push_back(utask);
      }
    }
    env_info.push_back(*task);
    task->flag = true;
    return env_info;
}

  template<typename _Tp>
    struct PairFirstLess : public binary_function<std::pair<int, _Tp>, std::pair<int, _Tp>, bool>
    {
      _GLIBCXX14_CONSTEXPR
      bool
      operator()(const std::pair<int, _Tp>& __x, const std::pair<int, _Tp>& __y) const
      { return __x.first < __y.first; }
    };

typedef std::priority_queue<std::pair<int, std::vector<vws::PlanTaskInfo>>,
                              vector<std::pair<int, std::vector<vws::PlanTaskInfo>>>,
                              PairFirstLess<std::vector<vws::PlanTaskInfo>>>
SortedTaskQ;


Eigen::Quaterniond getPaintOrientation(){
  Eigen::Matrix3d rot;
  // rot 对应矩阵为旋转矩阵，每一列对应
  rot << 1, 0, 0, // 
  0, 0, -1, // 
  0, 1, 0;
  Eigen::Quaterniond quat(rot);
  return quat;
}

void TrajectoryProcess::begintraj_Slot(MainProcess* vdata)
{
    
    std::cout<<"开始规划"<<std::endl;

    TrajectoryGenerator *generator =  TrajectoryGenerator::Instance();
            BaseStrategy *planstrategy;
            BaseStrategy *sevenstrategy;
    auto context = std::make_shared<TrajectoryContext>(generator,planstrategy,sevenstrategy);
    float valid_range = 3000;       //mm
    uint64_t current_encoder = vdata->getChainEncoder();
    bool isIncrease = vdata->getChainEncoderDir();
    float units = vdata->getChainUnits();
    uint64_t plan_delay = 1200 * units;
    int equal_th = 400;
    auto bottom_task_q = vdata->GetPlanTaskInfo(0);
    auto upper_task_q = vdata->GetPlanTaskInfo(1);
    uint64_t bottom_2_upper = 0;

    SortedTaskQ taskQ;
    //
    std::vector<vws::PlanTaskInfo> env_info;
    for (int i = 0; i < upper_task_q->size(); i++) {
      auto task = &(*upper_task_q)[i];
      if (task->flag == false && (task->encoder - current_encoder) > plan_delay) {
        env_info = tryGetPlanTask(vdata, &(*upper_task_q)[i], *upper_task_q, *bottom_task_q,
                       0, bottom_2_upper, isIncrease);
        taskQ.push(std::make_pair(task->encoder, env_info));
      }
    }
    for (int i = 0; i < bottom_task_q->size(); i++) {
      auto task = &(*bottom_task_q)[i];
      if (task->flag == false && (task->encoder - current_encoder) > plan_delay) {
        env_info = tryGetPlanTask(vdata, &(*bottom_task_q)[i], *bottom_task_q, *upper_task_q,
                       bottom_2_upper, 0, isIncrease);
        taskQ.push(std::make_pair(task->encoder + bottom_2_upper, env_info));
      }
    }

    //*************************************************************************
    //*                           规划任务                                    
    //*       从队列中取出头部数据，根据箱体信息生成场景，然后依次规划路径
    //*************************************************************************
    bool invert = false;
    while (!taskQ.empty()) {
      auto task_info = taskQ.top().second;
      taskQ.pop();
      // 创建环境
      for (int i = 0; i < task_info.size(); i++){
        auto boxInfo = task_info[i].boxInfo;
        auto boxSize =
            Eigen::Vector3d(task_info[i].lx, task_info[i].ly, task_info[i].lz);
        generator->AddBoxEnvirInfo(boxInfo.translation(), boxSize,
                                   Eigen::Quaterniond(boxInfo.rotation()),
                                   "tool");
      }
      // 生成喷涂约束
      auto task = task_info.back();
      auto boxInfo = task.boxInfo;
      auto boxSize = Eigen::Vector3d(task.lx, task.ly, task.lz);
      Eigen::VectorXd p, ori;
      Eigen::VectorXd init_dof, traj, entry_traj, quit_traj;
      init_dof = vdata->getRobotWaitPose();
      int ndof = 6;
      generator->GeneratePaintConstraint(boxInfo.translation(), boxSize,
                                         Eigen::Quaterniond(boxInfo.rotation()),
                                         getPaintOrientation(), task.face == 0,
                                         invert, p, ori);
      bool ret = generator->GeneratePaintTrajectory(init_dof, p, ori, traj, ndof);
      int N = p.size() / ndof;
      if(ret){
        ret = generator->GenerateEntryTrajectory(
            init_dof, p.block(0, 0, ndof, 1), 20, entry_traj, ndof, 3);
        if (ret) {
          ret = generator->GenerateEntryTrajectory(
              p.block(ndof * (N - 1), 0, ndof, 1), init_dof, 20, quit_traj,
              ndof, 3);
        }
      }
      auto p1 = generator->bottomnearpont(boxInfo.translation(), boxSize);
      if(!((task.face == 0) ^ invert)){
        p1[1] = p1[1] - boxSize[1];
      }
      auto mc_data = generator->calChainZeroPoint(p1, 0, task.encoder, isIncrease);

      // 清除环境
      generator->clearEnv();
      if(!ret){
        continue;
      }
    }
    //1. 计算悬挂链0点


    //1. 对设开关测量值

    //2. 上下相机图像信息

    //3. 轨迹规划



    // auto rbtdata = context->Work(vdata);
    // auto mcdata = context->calChainZeroPoint();


    QVariant varmc;
    QVariant varrbt;
    // varmc.setValue(mcdata);
    // varrbt.setValue(rbtdata);
    // emit traj_Signal(varmc,varrbt);

}
