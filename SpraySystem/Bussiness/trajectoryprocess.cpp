#include "trajectoryprocess.h"
#include "mainprocess.h"

TrajectoryProcess::TrajectoryProcess()
{
    visionContext = std::make_shared<VisionContext>();
}


bool ahead(int64_t a, int64_t relative, bool dir){
    if(dir){
      return a < relative;
    }else{
      return a > relative;
    }
}

std::vector<vws::PlanTaskInfo> TrajectoryProcess::tryGetPlanTask(MainProcess* vdata, vws::PlanTaskInfo *task,
                                                std::vector<vws::PlanTaskInfo> &q1,
                                                std::vector<vws::PlanTaskInfo> &q2,
                                                int64_t encoder_off1, int64_t encoder_off2,int64_t current_encoder,
                                                bool dir){
    std::vector<vws::PlanTaskInfo> env_info;
     float valid_range = 3000;       //mm
    bool isIncrease = vdata->getChainEncoderDir();
    float units = vdata->getChainUnits();
//    float speed = vdata->getChainSpeed();  // mm/s

    float expire_t = 4 * 1000;
    int64_t expire_en = expire_t * units;
    int64_t plan_range = 2000 * units;
    int64_t plan_delay = 1200 * units;
    int equal_th = 400;
    bool face = task->face;

    // 规划上层前端面
    // 看下层编码器
    auto encoder = task->encoder + encoder_off1;
    bool down = task->face == 1;

    // q2 一定不同层
    for (int i = 0; i < q2.size(); i++) {
      vws::PlanTaskInfo btask = q2[i];
      if (plan_range > llabs(btask.encoder + encoder_off2 - encoder)) {
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

void fakeData(MainProcess *vdata, int64_t encoder) {
  static int64_t init_encoder = encoder;
  static bool genUp = true;
  static bool genBottom = true;
  static float last_pos = -1;
  static bool init = true;
  float units = 1 / 0.419;
  float LminUp = 1000, Lrange = 1000;
  float LminBottom = 500;
  float Mininterval = 600, Drange = 200;
  float H = 800, W = 600;
  float rotAngle = 10.0 / 180.0 * M_PI;
  static float Lu = 0, Ld = 0, Pu = 0, Pd = 0;
  static Eigen::Quaterniond Ru, Rd;
  if (genUp) {
    float old_Lu = Lu;
    Lu = ((float)rand() / RAND_MAX) * Lrange + LminUp;
    float interval = ((float)rand() / RAND_MAX) * Drange + Mininterval;
    if(init){
      Pu = -Lu;
      interval = 0;
      init = false;
    }
    Pu = Pu + old_Lu + interval;
    float rx, ry, rz;
    rx = ((float)rand() / RAND_MAX - 0.5) * rotAngle;
    ry = ((float)rand() / RAND_MAX - 0.5) * rotAngle;
    rz = ((float)rand() / RAND_MAX - 0.5) * rotAngle;
    Ru = Eigen::AngleAxisd(rz, Eigen::Vector3d::UnitZ()) *
         Eigen::AngleAxisd(ry, Eigen::Vector3d::UnitY()) *
         Eigen::AngleAxisd(rx, Eigen::Vector3d::UnitX());
    genUp = false;
    std::cout << "gen up" << Pu << std::endl;
  }
  if(genBottom){
    Ld = Lu - ((float)rand() / RAND_MAX) * LminBottom;
    float dl = Lu - Ld;
    Pd = Pu + ((float)rand() / RAND_MAX) * dl;
    float rx, ry, rz;
    rx = ((float)rand() / RAND_MAX - 0.5) * rotAngle;
    ry = ((float)rand() / RAND_MAX - 0.5) * rotAngle;
    rz = ((float)rand() / RAND_MAX - 0.5) * rotAngle;
    Rd = Eigen::AngleAxisd(rz, Eigen::Vector3d::UnitZ()) *
         Eigen::AngleAxisd(ry, Eigen::Vector3d::UnitY()) *
         Eigen::AngleAxisd(rx, Eigen::Vector3d::UnitX());
    genBottom = false;
    std::cout << "gen bottom " << Pd << std::endl;
  }
  float current_pos = (float)(init_encoder - encoder) / units;
  std::cout << "current_pos : " << current_pos << ", L: " << Pu << " "<< Pu + Lu << ", " << Pd<< " "<< Pd + Ld
            << ", flag : " << genUp << ", " << genBottom << std::endl;
  if (current_pos >= Pu && last_pos < Pu) {
    vws::PlanTaskInfo info;
    info.boxInfo = Eigen::Isometry3d::Identity();
    info.boxInfo.translate(Eigen::Vector3d(1000, -3000-Lu/2.0, 1000 + 1200));
    info.boxInfo.rotate(Rd);
    info.lx = W;
    info.ly = Lu;
    info.lz = H;
    info.face = 0;
    info.encoder = init_encoder - (Pu * units);
    info.diff = 300;
    info.flag = 1;
    vdata->GetPlanTaskInfo(1)->push_back(info);
    std::cout << "push up 0: " << info.encoder << std::endl;
  }
  if (current_pos>= Pu + Lu && last_pos < Pu + Lu) {
    vws::PlanTaskInfo info;
    info.boxInfo = Eigen::Isometry3d::Identity();
    info.boxInfo.translate(Eigen::Vector3d(1000, -3000+Lu/2.0, 1000 + 1200));
    info.boxInfo.rotate(Rd);
    info.lx = W;
    info.ly = Lu;
    info.lz = H;
    info.face = 1;
    info.encoder = init_encoder - ((Pu + Lu) * units);
    info.diff = 300;
    info.flag = 1;
    vdata->GetPlanTaskInfo(1)->push_back(info);
    genUp = true;
    // genBottom = (((float)rand() / RAND_MAX - 0.5) > 0.5);
    genBottom = true;
    std::cout << "push up 1: " << info.encoder << std::endl;
  }
  if(current_pos>= Pd && last_pos < Pd){
    vws::PlanTaskInfo info;
    info.boxInfo = Eigen::Isometry3d::Identity();
    info.boxInfo.translate(Eigen::Vector3d(1000, -3000-Ld/2.0, 1000));
    info.boxInfo.rotate(Rd);
    info.lx = W;
    info.ly = Ld;
    info.lz = H;
    info.face = 0;
    info.encoder = init_encoder - (Pd * units);
    info.diff = 300;
    info.flag = 0;
    vdata->GetPlanTaskInfo(0)->push_back(info);
    std::cout << "push bottom 0: " << info.encoder << std::endl;
  }
  if(current_pos>= Pd + Ld && last_pos < Pd + Ld){
    vws::PlanTaskInfo info;
    info.boxInfo = Eigen::Isometry3d::Identity();
    info.boxInfo.translate(Eigen::Vector3d(1000, -3000+Ld/2.0, 1000));
    info.boxInfo.rotate(Rd);
    info.lx = W;
    info.ly = Ld;
    info.lz = H;
    info.face = 1;
    info.encoder = init_encoder - ((Pd + Ld) * units);
    info.diff = 300;
    info.flag = 0;
    vdata->GetPlanTaskInfo(0)->push_back(info);
    std::cout << "push bottom 1: " << info.encoder << std::endl;
  }
  last_pos = current_pos;
}

void TrajectoryProcess::begintraj_Slot(MainProcess* vdata)
{
  fakeData(vdata, vdata->getChainEncoder());
  std::cout << "开始规划" << std::endl;

  TrajectoryGenerator *generator = TrajectoryGenerator::Instance();
  BaseStrategy *planstrategy;
  BaseStrategy *sevenstrategy;
  // auto context = std::make_shared<TrajectoryContext>(generator, planstrategy,
  //                                                    sevenstrategy);
  float valid_range = 6000;  // mm
  int64_t current_encoder = vdata->getChainEncoder();
  bool isIncrease = vdata->getChainEncoderDir();
  float units = vdata->getChainUnits();
  int64_t expire_range = valid_range * units;
  int64_t plan_delay = 1200 * units;
  int equal_th = 400;
  auto bottom_task_q = vdata->GetPlanTaskInfo(0);
  auto upper_task_q = vdata->GetPlanTaskInfo(1);
  int64_t bottom_2_upper = 0;

  SortedTaskQ taskQ;
  //
  std::vector<vws::PlanTaskInfo> env_info;
  for (int i = 0; i < upper_task_q->size(); i++) {
    auto task = &(*upper_task_q)[i];
    if (task->flag == false && (task->encoder - current_encoder) > plan_delay) {
      env_info = tryGetPlanTask(vdata, &(*upper_task_q)[i], *upper_task_q,
                                *bottom_task_q, 0, bottom_2_upper, current_encoder, isIncrease);
      std::cout << "taskQ : push up " << task->encoder << ", " << (task->face == 0
          ? "head "
          : "tail") << std::endl;
      taskQ.push(std::make_pair(task->encoder, env_info));
    }
  }
  for (int i = 0; i < bottom_task_q->size(); i++) {
    auto task = &(*bottom_task_q)[i];
    if (task->flag == false && (task->encoder - current_encoder) > plan_delay) {
      env_info = tryGetPlanTask(vdata, &(*bottom_task_q)[i], *bottom_task_q, *upper_task_q,
                      bottom_2_upper, 0, current_encoder,isIncrease);
      std::cout << "taskQ : push bottom " << task->encoder << ", " << (task->face == 0
        ? "head "
        : "tail" )<< std::endl;

      taskQ.push(std::make_pair(task->encoder + bottom_2_upper, env_info));
    }
  }


    //todo:: 清理队列。把超过一定距离的task清除掉
  auto ptask = upper_task_q->begin();
  while(ptask != upper_task_q->end()){
    if(llabs(ptask->encoder - current_encoder) > expire_range){
      auto p = ptask;
      ptask++;
      upper_task_q->erase(p);
      std::cout << "erase : up" << std::endl;
    } else {
      ptask++;
    }
  }

  ptask = bottom_task_q->begin();
  while(ptask != bottom_task_q->end()){
    if(llabs(ptask->encoder - current_encoder) > expire_range){
      auto p = ptask;
      ptask++;
      bottom_task_q->erase(p);
      std::cout << "erase : bottom" << std::endl;
    }else{
      ptask++;
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
    // 生成喷涂约束
    auto task = task_info.back();
    int64_t ref_encoder = task.encoder;
    auto boxInfo = task.boxInfo;
    auto boxSize = Eigen::Vector3d(task.lx, task.ly, task.lz);
    auto boxCenter = boxInfo.translation();
    Eigen::VectorXd p, ori;
    Eigen::VectorXd init_dof, traj, entry_traj, quit_traj;
    init_dof = vdata->getRobotWaitPose();
    int ndof = 6;

    auto p1 = generator->bottomnearpont(boxInfo.translation(), boxSize);
    float dest_y = -task.diff;
    if (!((task.face == 0) ^ invert)) {
      p1[1] = p1[1] - boxSize[1];
      dest_y = -dest_y;
    }
    float diff = dest_y - p1[1];
    // 创建环境
    for (int i = 0; i < task_info.size(); i++) {
      int64_t diff_encoder = task_info[i].encoder - ref_encoder;
      float diff_mm = diff_encoder / units;
      diff_mm = invert ? -diff_mm : diff_mm;
      auto boxInfo_1 = task_info[i].boxInfo;
      auto boxSize_1 =
          Eigen::Vector3d(task_info[i].lx, task_info[i].ly, task_info[i].lz);
      auto boxCenter_1 = boxInfo_1.translation();
      boxCenter_1[1] += diff + diff_mm;
      generator->AddBoxEnvirInfo(
          boxCenter_1, boxSize_1, Eigen::Quaterniond(boxInfo.rotation()),
          (QString("box") + QString::number(i)).toStdString());
    }

    boxCenter[1] += diff;
    generator->GeneratePaintConstraint(
        boxCenter, boxSize, Eigen::Quaterniond(boxInfo.rotation()),
        getPaintOrientation(), task.face == 0, invert, p, ori);
    bool ret = generator->GeneratePaintTrajectory(init_dof, p, ori, traj, ndof);

    int N = p.size() / ndof;
    if (ret) {
      ret = generator->GenerateEntryTrajectory(
          init_dof, traj.block(0, 0, ndof, 1), 20, entry_traj, ndof, 3);
      if (ret) {
        ret = generator->GenerateEntryTrajectory(
            traj.block(ndof * (N - 1), 0, ndof, 1), init_dof, 20, quit_traj,
            ndof, 3);
      }
    }

    auto mc_data =
        generator->calChainZeroPoint(p1, 0, task.encoder, isIncrease);

    // 清除环境
    generator->clearEnv();
    if (!ret) {
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
