#include "trajectoryprocess.h"
#include "mainprocess.h"
#include "Data/StaticData.h"
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

PlanTask TrajectoryProcess::tryGetPlanTask(vws::PlanTaskInfo *task,
                                           std::vector<vws::PlanTaskInfo> &q1,
                                           std::vector<vws::PlanTaskInfo> &q2,
                                           int64_t encoder_off1,
                                           int64_t encoder_off2,
                                           int64_t current_encoder,
                                           bool isIncrease, float units){
  PlanTask env_info;
  float valid_range = 3000;  // mm
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
        env_info.envs_.push_back(btask);
      } 
    }
    for (int i = 0; i < q1.size(); i++) {
      vws::PlanTaskInfo utask = q1[i];
      if (utask.encoder + encoder_off1 == encoder) continue;
      if (ahead(utask.encoder + encoder_off1, encoder, isIncrease) && utask.face == 1 &&
          plan_range > llabs(utask.encoder + encoder_off1 - encoder)) {
        env_info.envs_.push_back(utask);
      } else if (!ahead(utask.encoder + encoder_off1, encoder, isIncrease) &&
                  utask.face == 0 &&
                  plan_range > llabs(utask.encoder + encoder_off1 - encoder)) {
        env_info.envs_.push_back(utask);
      }
    }
    env_info.targets_.push_back(*task);
    task->flag = true;
    return env_info;
}

/**
 * @brief 
 * 
 * @param task 上层任务
 * @param q1   上层队列
 * @param q2    下层队列
 * @param encoder_off1      上层队列编码器修正
 * @param encoder_off2      下层队列编码器修正
 * @param current_encoder   
 * @param isIncrease 
 * @param units 
 * @return PlanTask 
 */
PlanTask TrajectoryProcess::tryGetPlanTaskInTwoLayer(vws::PlanTaskInfo *task,
                                           std::vector<vws::PlanTaskInfo> &q1,
                                           std::vector<vws::PlanTaskInfo> &q2,
                                           int64_t encoder_off1,
                                           int64_t encoder_off2,
                                           int64_t current_encoder,
                                           bool isIncrease, float units){
  PlanTask env_info;
  float valid_range = 3000;  // mm
  //    float speed = vdata->getChainSpeed();  // mm/s

    float expire_t = 4 * 1000;
    int64_t expire_en = expire_t * units;
    int64_t coplan_range = 300 * units;
    int64_t plan_range = 2000 * units;
    int64_t plan_delay = 1200 * units;
    int equal_th = 400;
    bool face = task->face;

    // 规划上层前端面
    // 看下层编码器
    auto encoder = task->encoder + encoder_off1;
    env_info.targets_.push_back(*task);

    // q2 一定不同层
    for (int i = 0; i < q2.size(); i++) {
      vws::PlanTaskInfo btask = q2[i];
      if(llabs(btask.encoder + encoder_off2 - encoder) < coplan_range && 
        btask.face == task->face){
        env_info.targets_.push_back(btask);
        q2[i].flag = 1;
      } else if (plan_range > llabs(btask.encoder + encoder_off2 - encoder)) {
        env_info.envs_.push_back(btask);
      }
    }
    // 上层队列中
    for (int i = 0; i < q1.size(); i++) {
      vws::PlanTaskInfo utask = q1[i];
      if (utask.encoder + encoder_off1 == encoder){
        
        continue;
      }
      if (ahead(utask.encoder + encoder_off1, encoder, isIncrease) && utask.face == 1 &&
          plan_range > llabs(utask.encoder + encoder_off1 - encoder)) {
        env_info.envs_.push_back(utask);
      } else if (!ahead(utask.encoder + encoder_off1, encoder, isIncrease) &&
                  utask.face == 0 &&
                  plan_range > llabs(utask.encoder + encoder_off1 - encoder)) {
        env_info.envs_.push_back(utask);
      }
    }
    task->flag = true;
    return env_info;
}


Eigen::Quaterniond getPaintOrientation(bool front, bool invert){
  Eigen::Matrix3d rot;
  if(!invert && front){
    // rot 对应矩阵为旋转矩阵，每一列对应
    rot << 1, 0, 0, // 
    0, 0, -1, // 
    0, 1, 0;
  }else if(!invert && !front){
    rot << 1, 0, 0, // 
    0, 0, 1, // 
    0, -1, 0;
  }else if(invert && front){
    rot << -1, 0, 0, // 
    0, 0, -1, // 
    0, -1, 0;
  }else{
    rot << -1, 0, 0, // 
    0, 0, 1, // 
    0, 1, 0;
  }
  Eigen::Quaterniond quat(rot);
  return quat;
}
/**
 * @brief Get the Seam Paint Orientation object
 * 
 * @param mode          true : 模式1, front，并且同向，false : 模式2,
 * @return Eigen::Quaterniond 
 */
Eigen::Quaterniond getSeamPaintOrientation(bool front, bool invert){
  Eigen::Matrix3d rot;
  // rot 对应矩阵为旋转矩阵，每一列对应
  if(front && !invert){
    rot << 1, 0, 0, // 
    0, 0, 1, // 
    0, -1, 0;
  }else if (!front && !invert){
    rot << 1, 0, 0, // 
    0, 0, -1, // 
    0, 1, 0;
    // rot = rot * Eigen::AngleAxisd(M_PI / 4.0, Eigen::Vector3d::UnitY());
  }else if(invert && front){
    rot << -1, 0, 0, // 
    0, 0, 1, // 
    0, 1, 0;
  }else{
    rot << -1, 0, 0, // 
    0, 0, -1, // 
    0, -1, 0;
  }
  rot = rot * Eigen::AngleAxisd(M_PI / 4.0, Eigen::Vector3d::UnitY());

  Eigen::Quaterniond quat(rot);
  return quat;
}

void AbbRbt(Eigen::VectorXd planRet, RobotTask &tk)
{
  int cnt = 0;
   for(int i = 0; i < planRet.size() / 6; i++){
       std::cout << i << ": ";
       std::array<float,7> jv;
       for(int n = 0; n < 6; n++){
           jv[n] = planRet[cnt++] / M_PI * 180.0;
           // IRB 1410机器人3轴和2轴耦合，因此需要添加下面的补偿
#ifdef ABB1410_JOINT_CONVERT
           if(n == 2){
               jv[n] += jv[n-1];
           }
#endif
           std::cout << jv[n] << ", ";
       }
       std::cout << std::endl;
       tk.track.push_back(jv);
   }
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
    info.boxInfo.translate(Eigen::Vector3d(2200, -3000-Lu/2.0, 100 + 1200));
    info.boxInfo.rotate(Rd);
    info.lx = W;
    info.ly = Lu;
    info.lz = H;
    info.face = 0;
    info.encoder = init_encoder - (Pu * units);
    info.diff = 0;
    info.flag = 0;
    vdata->GetPlanTaskInfo(1)->push_back(info);
    std::cout << "push up 0: " << info.encoder << std::endl;
  }
  if (current_pos>= Pu + Lu && last_pos < Pu + Lu) {
    vws::PlanTaskInfo info;
    info.boxInfo = Eigen::Isometry3d::Identity();
    info.boxInfo.translate(Eigen::Vector3d(2200, -3000+Lu/2.0, 100 + 1200));
    info.boxInfo.rotate(Rd);
    info.lx = W;
    info.ly = Lu;
    info.lz = H;
    info.face = 1;
    info.encoder = init_encoder - ((Pu + Lu) * units);
    info.diff = 0;
    info.flag = 0;
    vdata->GetPlanTaskInfo(1)->push_back(info);
    genUp = true;
    // genBottom = (((float)rand() / RAND_MAX - 0.5) > 0.5);
    genBottom = true;
    std::cout << "push up 1: " << info.encoder << std::endl;
  }
  if(current_pos>= Pd && last_pos < Pd){
    vws::PlanTaskInfo info;
    info.boxInfo = Eigen::Isometry3d::Identity();
    info.boxInfo.translate(Eigen::Vector3d(2200, -3000-Ld/2.0, 100));
    info.boxInfo.rotate(Rd);
    info.lx = W;
    info.ly = Ld;
    info.lz = H;
    info.face = 0;
    info.encoder = init_encoder - (Pd * units);
    info.diff = 0;
    info.flag = 0;
    vdata->GetPlanTaskInfo(0)->push_back(info);
    std::cout << "push bottom 0: " << info.encoder << std::endl;
  }
  if(current_pos>= Pd + Ld && last_pos < Pd + Ld){
    vws::PlanTaskInfo info;
    info.boxInfo = Eigen::Isometry3d::Identity();
    info.boxInfo.translate(Eigen::Vector3d(2200, -3000+Ld/2.0, 100));
    info.boxInfo.rotate(Rd);
    info.lx = W;
    info.ly = Ld;
    info.lz = H;
    info.face = 1;
    info.encoder = init_encoder - ((Pd + Ld) * units);
    info.diff = 0;
    info.flag = 0;
    vdata->GetPlanTaskInfo(0)->push_back(info);
    std::cout << "push bottom 1: " << info.encoder << std::endl;
  }
  last_pos = current_pos;
}

SortedTaskQ TrajectoryProcess::PrepareTaskInfoOneByOne(
    std::vector<vws::PlanTaskInfo> *upper_task_q,
    std::vector<vws::PlanTaskInfo> *bottom_task_q, 
    int64_t current_encoder,int64_t bottom_2_upper,
    bool isIncrease, float units, int64_t plan_delay){
  SortedTaskQ taskQ;
  //
  PlanTask env_info;
  for (int i = 0; i < upper_task_q->size(); i++) {
    auto task = &(*upper_task_q)[i];
    if (task->flag == false && (task->encoder - current_encoder) > plan_delay) {
      env_info = tryGetPlanTask(&(*upper_task_q)[i], *upper_task_q,
                                *bottom_task_q, 0, bottom_2_upper, current_encoder, isIncrease, units);
      std::cout << "taskQ : push up " << task->encoder << ", " << (task->face == 0
          ? "head "
          : "tail") << std::endl;
      taskQ.push(std::make_pair(task->encoder, env_info));
    }
  }
  for (int i = 0; i < bottom_task_q->size(); i++) {
    auto task = &(*bottom_task_q)[i];
    if (task->flag == false && (task->encoder - current_encoder) > plan_delay) {
      env_info = tryGetPlanTask(&(*bottom_task_q)[i], *bottom_task_q, *upper_task_q,
                      bottom_2_upper, 0, current_encoder,isIncrease, units);
      std::cout << "taskQ : push bottom " << task->encoder << ", " << (task->face == 0
        ? "head "
        : "tail" )<< std::endl;

      taskQ.push(std::make_pair(task->encoder + bottom_2_upper, env_info));
    }
  }
  return taskQ;
}


SortedTaskQ TrajectoryProcess::PrepareTaskInfoOneLayer(
    std::vector<vws::PlanTaskInfo> *q1,
    std::vector<vws::PlanTaskInfo> *q2, 
    int64_t current_encoder,int64_t q1_offset,int64_t q2_offset,
    bool isIncrease, float units, int64_t plan_delay){
  SortedTaskQ taskQ;
  //
  PlanTask env_info;
  for (int i = 0; i < q1->size(); i++) {
    auto task = &(*q1)[i];
    if (task->flag == false && (task->encoder - current_encoder) > plan_delay) {
      env_info =
          tryGetPlanTask(&(*q1)[i], *q1, *q2, q1_offset,
                         q2_offset, current_encoder, isIncrease, units);
      std::cout << "taskQ : push up " << task->encoder << ", "
                << (task->face == 0 ? "head " : "tail") << std::endl;
      taskQ.push(std::make_pair(task->encoder, env_info));
    }
  }
  return taskQ;
}


SortedTaskQ TrajectoryProcess::PrepareTaskInfoTwoLayers(
    std::vector<vws::PlanTaskInfo> *upper_task_q,
    std::vector<vws::PlanTaskInfo> *bottom_task_q, 
    int64_t current_encoder,int64_t bottom_2_upper,
    bool isIncrease, float units, int64_t plan_delay){
  SortedTaskQ taskQ;
  //
  PlanTask env_info;
  for (int i = 0; i < upper_task_q->size(); i++) {
    auto task = &(*upper_task_q)[i];
    if (task->flag == false && (task->encoder - current_encoder) > plan_delay) {
      env_info = tryGetPlanTaskInTwoLayer(&(*upper_task_q)[i], *upper_task_q,
                                *bottom_task_q, 0, bottom_2_upper, current_encoder, isIncrease, units);
      std::cout << "taskQ : push up " << task->encoder << ", " << (task->face == 0
          ? "head "
          : "tail") << std::endl;
      taskQ.push(std::make_pair(task->encoder, env_info));
    }
  }
  return taskQ;
}


/**
 * @brief Create a Plan Scene object
 * 
 * @param generator 
 * @param task_info 
 * @param units 
 * @param invert 
 */
void createPlanScene(TrajectoryGenerator *generator, PlanTask& task_info, float units,
bool invert){
  // 生成喷涂约束
  auto task = task_info.targets_.front();
  int64_t ref_encoder = task.encoder;
  auto boxInfo = task.boxInfo;
  auto boxSize = Eigen::Vector3d(task.lx, task.ly, task.lz);
  auto boxCenter = boxInfo.translation();
  Eigen::VectorXd p, ori;
  Eigen::VectorXd init_dof, traj, entry_traj, quit_traj;
  int ndof = 6;

  auto p1 = generator->bottomnearpont(boxInfo.translation(), boxSize, invert);
  float dest_y = -task.diff;
  if (!((task.face == 0))) {
    p1[1] = p1[1] - boxSize[1];
    dest_y = -dest_y;
  }
  p1 = boxInfo.rotation() * (p1 - boxCenter) + boxCenter;
  float diff = dest_y - p1[1];
  // 创建环境
  std::vector<vws::PlanTaskInfo> ptask_vector[2];
  ptask_vector[0] = (task_info.envs_);
  ptask_vector[1] = (task_info.targets_);
  for (int np = 0; np < 2; np++) {
    std::vector<vws::PlanTaskInfo> ptask_info = ptask_vector[np];
    for (int i = 0; i < ptask_info.size(); i++) {
      int64_t diff_encoder = (ptask_info)[i].encoder - ref_encoder;
      float diff_mm = diff_encoder / units;
      diff_mm = invert ? -diff_mm : diff_mm;
      auto boxInfo_1 = (ptask_info)[i].boxInfo;
      auto boxSize_1 = Eigen::Vector3d((ptask_info)[i].lx, (ptask_info)[i].ly,
                                       (ptask_info)[i].lz);
      auto boxCenter_1 = boxInfo_1.translation();
      boxCenter_1[1] += diff + diff_mm;
      generator->AddBoxEnvirInfo(boxCenter_1, boxSize_1,
                                 Eigen::Quaterniond(boxInfo_1.rotation()),
                                 (QString("box_") + QString::number(np) +
                                  QString("_") + QString::number(i))
                                     .toStdString());
      // 添加挂钩对应碰撞体，use a thin(x) long(z) box to simulate the hook
      double hook_h = 400;

      generator->AddBoxHookEnvirInfo(
          boxCenter_1, boxSize_1, Eigen::Quaterniond(boxInfo_1.rotation()),
          hook_h,
          (QString("box_hook_") + QString::number(np) + QString("_") +
           QString::number(i))
              .toStdString());
    }
  }
}


bool planTwoLayersTask(TrajectoryGenerator *generator, PlanTask &task_info,
                 const Eigen::VectorXd &init_dof,
                 std::vector<TrajectoryInfo> &out_traj,
                 std::vector<float> &mc_data, float units, bool isIncrease,
                 bool invert, bool plane_first = true, bool plan_weld = true){
  bool front = task_info.targets_.front().face == 0;
  TrajectoryInfo single_traj;
  auto task = task_info.targets_.front();
  int64_t ref_encoder = task.encoder;
  auto boxInfo = task.boxInfo;
  auto boxSize = Eigen::Vector3d(task.lx, task.ly, task.lz);
  auto boxCenter = boxInfo.translation();
  Eigen::VectorXd p, ori, p_weld, ori_weld;
  Eigen::VectorXd traj, traj_weld, traj_transite, entry_traj, quit_traj;
  int ndof = 6;

  // 计算时首先在机器人坐标系下计算出左下方点p1，
  // 然后根据偏移要求，计算偏移后的位置。
  // 对于模式2,p1点的位置在箱体另一侧。
  // dest_y 为跟随时p1点y分量在机器人坐标系下应该处于的位置。
  // 前端面和后端面对应的偏移量符号相反。
  auto p1 = generator->bottomnearpont(boxInfo.translation(), boxSize, invert);
  float dest_y = -task.diff;
  if (!((task.face == 0))) {
    p1[1] = p1[1] - boxSize[1];
    dest_y = -dest_y;
  }
  p1 = boxInfo.rotation() * (p1 - boxInfo.translation()) + boxInfo.translation();
  float diff = dest_y - p1[1];

  boxCenter[1] += diff;
  
  generator->GenerateShrinkedPaintConstraint(
      boxCenter, boxSize, Eigen::Quaterniond(boxInfo.rotation()),
      getPaintOrientation(task.face == 0, invert), 0, boxSize[2] / 2.0 - 50, task.face == 0, invert, 0,
      p, ori);
  generator->GenerateSeamPaintConstraint(
      boxCenter, boxSize, Eigen::Quaterniond(boxInfo.rotation()),
      getSeamPaintOrientation(task.face == 0, invert), 20, boxSize[2] - 200,
      task.face == 0, invert, p_weld, ori_weld);

  std::vector<Eigen::VectorXd> plan_path, plan_ori;
  if(plane_first){
    plan_path.push_back(p);
    plan_ori.push_back(ori);
    plan_path.push_back(p_weld);
    plan_ori.push_back(ori_weld);
  }else{
    plan_path.push_back(p_weld);
    plan_ori.push_back(ori_weld);
    plan_path.push_back(p);
    plan_ori.push_back(ori);
  }


  if (plan_weld) {
    if(front){

    }
  }
}

/**
 * @brief 规划任务信息中的第一个任务。任务信息中包含场景信息和规划目标信息。只规划其中第一个目标的喷涂路径
 *        目前规划一个面和一条边
 * @param generator 
 * @param task_info 
 * @param init_dof 
 * @param res 
 * @param mc_data 
 * @param units 
 * @param isIncrease 
 * @param invert 
 * @param plane_first     指示是否先喷面：true - 先喷面再喷焊缝， false - 先喷焊缝再喷面
 * @return true 
 * @return false 
 */
bool planOneTask(TrajectoryGenerator *generator,
                                        PlanTask &task_info,
                                        const Eigen::VectorXd &init_dof,
                                        std::vector<TrajectoryInfo> & out_traj,
                                        std::vector<float> & mc_data,
                                        float units,bool isIncrease, bool invert, bool plane_first = true){
  TrajectoryInfo single_traj;
  auto task = task_info.targets_.front();
  int64_t ref_encoder = task.encoder;
  auto boxInfo = task.boxInfo;
  auto boxSize = Eigen::Vector3d(task.lx, task.ly, task.lz);
  auto boxCenter = boxInfo.translation();
  Eigen::VectorXd p, ori, p_weld, ori_weld;
  Eigen::VectorXd traj, traj_weld, traj_transite, entry_traj, quit_traj;
  int ndof = 6;
  auto p1 = generator->bottomnearpont(boxInfo.translation(), boxSize, invert);

  float dest_y = -task.diff;
  if (!((task.face == 0))) {
    p1[1] = p1[1] - boxSize[1];
    dest_y = -dest_y;
  }

  p1 = boxInfo.rotation() * (p1 - boxCenter) + boxCenter;

  float diff = dest_y - p1[1];

  boxCenter[1] += diff;
  generator->GenerateShrinkedPaintConstraint(
      boxCenter, boxSize, Eigen::Quaterniond(boxInfo.rotation()),
      getPaintOrientation(task.face == 0, invert), 0, boxSize[2] / 2.0 - 50, task.face == 0, invert, 0,
      p, ori);
  generator->GenerateSeamPaintConstraint(
      boxCenter, boxSize, Eigen::Quaterniond(boxInfo.rotation()),
      getSeamPaintOrientation(task.face == 0, invert), 20, boxSize[2] - 200, task.face == 0, invert,
      p_weld, ori_weld);

  std::vector<Eigen::VectorXd> plan_path, plan_ori;
  if(plane_first){
    plan_path.push_back(p);
    plan_ori.push_back(ori);
    plan_path.push_back(p_weld);
    plan_ori.push_back(ori_weld);
  }else{
    plan_path.push_back(p_weld);
    plan_ori.push_back(ori_weld);
    plan_path.push_back(p);
    plan_ori.push_back(ori);
  }

  auto init_pose = init_dof;
  bool ret;
  for (int i = 0; i < plan_path.size(); i++) {
    std::cout << "Gen Paint Path " << i << std::endl;
    ret = generator->GeneratePaintTrajectory(init_pose, plan_path[i], plan_ori[i], traj, ndof);
    if(ret){
      int N = traj.size() / ndof;
      Eigen::VectorXd paint_start, paint_end;
      paint_start = traj.block(0, 0, ndof, 1);
      std::cout << "Gen Entry Path " << i << std::endl;
      ret &= generator->GenerateEntryTrajectory(
        init_pose, traj.block(0, 0, ndof, 1), 20, entry_traj, ndof, 3);
      if(ret){
        single_traj.traj_ = entry_traj;
        single_traj.type_ = TransitionTraj;
        out_traj.push_back(single_traj);
        single_traj.traj_ = traj;
        single_traj.type_ = PaintTraj;
        out_traj.push_back(single_traj);
      }else{
        return false;
      }

      init_pose = traj.block(ndof * (N - 1), 0, ndof, 1);
    }else{
      return false;
    }
  }
  std::cout << "Gen Quit Path " << std::endl;
  ret = generator->GenerateEntryTrajectory(
        init_pose, init_dof, 20, quit_traj, ndof, 3, true);
  if(ret){
      single_traj.traj_ = quit_traj;
      single_traj.type_ = TransitionTraj;
      out_traj.push_back(single_traj);
  }

  mc_data =
      generator->calChainZeroPoint(p1, 0, task.encoder, isIncrease);
  mc_data[1] = task.face == 0 ? -task.diff : task.diff;     // 前提是机器人正方向和箱体允许方向相同，否则取负号
  std::cout << "task encoder: " << task.encoder << std::endl;
  std::cout << "p1 pos: " << p1[0] << ", "<< p1[1] << ", "<< p1[2] << std::endl;
  std::cout <<"mc data: " << mc_data[0] << "," << mc_data[1] << std::endl;

  return ret;
}

std::vector<RobotTask> convertToRobotTask(const std::vector<TrajectoryInfo> &traj_info){
  std::vector<RobotTask> rbttasks;
  TrajectoryType t = TransitionTraj;

  for (int n = 0; n < traj_info.size(); n++){
    auto traj = traj_info[n];
    if(traj.type_ != t){
      if(traj.type_ == PaintTraj){
        RobotTask rbttask;
        rbttask.track.clear();
        // todo:: 开喷涂任务
      }else{
        RobotTask rbttask;
        rbttask.track.clear();
        // todo:: 关喷涂任务
      }
    }
    RobotTask rbttask;
    rbttask.task = VWSRobot::TaskType::MOVEABSJ;
    rbttask.speed[0] = vws::rbtspeed;
    rbttask.speed[1] = vws::rbtspeed;
    AbbRbt(traj.traj_, rbttask);
    rbttasks.push_back(rbttask);
  }
  return rbttasks;
}


  void clearQueue(std::vector<vws::PlanTaskInfo> & q, int64_t current_encoder, int64_t expire_range){
    int N = q.size();
    std::vector<int> ind;
    for(int i = 0; i < N; i++){
      if(llabs(q[i].encoder - current_encoder) > expire_range){
        ind.push_back(i);
      }
    }

    for(int i = ind.size()-1; i >= 0; i--){
      q.erase(q.begin() + ind[i]);
    }
  }

void TrajectoryProcess::begintraj_Slot(MainProcess* vdata)
{
#ifdef PLAN_FAKE_DATA
  fakeData(vdata, vdata->getChainEncoder());
#endif

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
  int64_t bottom_2_upper = 270*units;

  SortedTaskQ taskQ =
      PrepareTaskInfoOneLayer(bottom_task_q, upper_task_q, current_encoder,
                              bottom_2_upper, 0, isIncrease, units, plan_delay);
  // PrepareTaskInfoOneLayer(upper_task_q, bottom_task_q, current_encoder, 0,
  //                          bottom_2_upper, isIncrease, units, plan_delay);
  

    //todo:: 清理队列。把超过一定距离的task清除掉
    clearQueue(*upper_task_q, current_encoder, expire_range);
    clearQueue(*bottom_task_q, current_encoder, expire_range);
  


  //*************************************************************************
  //*                           规划任务
  //*       从队列中取出头部数据，根据箱体信息生成场景，然后依次规划路径
  //*************************************************************************
  bool invert = false;
  while (!taskQ.empty()) {
    std::cout << "开始规划" << std::endl;
    auto task_info = taskQ.top().second;
    taskQ.pop();
    std::vector<float> mc_data;
    Eigen::VectorXd init_dof;
    init_dof = vdata->getRobotWaitPose();
    createPlanScene(generator, task_info, units, invert);
    std::vector<TrajectoryInfo> traj_info;
    bool ret = planOneTask(generator, task_info, init_dof, traj_info, mc_data, units,
                           isIncrease, invert, task_info.targets_.front().face == 0);

    generator->clearEnv();
    if (!ret) {
      continue;
    }

    if(ret){
      std::cout << "规划成功" << std::endl;
      auto rbttasks = convertToRobotTask(traj_info);

      vdata->SetRobotTaskInfo(mc_data, rbttasks);
    }
  }
}
