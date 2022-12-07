#include "trajectoryprocess.h"
#include "Data/StaticData.h"
#include "Util/Log/clog.h"
#include "mainprocess.h"

Eigen::VectorXd reverseVector(Eigen::VectorXd &v, int ndof) {
  int N = v.size() / ndof;
  Eigen::VectorXd ret(v.size());
  int dest_cnt = 0, src_cnt = v.size() - ndof;
  for (int i = 0; i < N; i++, dest_cnt += ndof, src_cnt -= ndof) {
    ret.middleRows(dest_cnt, ndof) = v.middleRows(src_cnt, ndof);
  }
  return ret;
}

Eigen::VectorXd mergeVectors(const std::vector<Eigen::VectorXd> &vecs) {
  Eigen::VectorXd merged;
  int len = 0;
  for (int i = 0; i < vecs.size(); i++) {
    len += vecs[i].size();
  }
  merged.resize(len);
  int cnt = 0;
  for (int i = 0; i < vecs.size(); i++) {
    merged.middleRows(cnt, vecs[i].size()) = vecs[i];
  }
  return merged;
}

PlanStragety::PlanStragety(const std::string &filename) : filename_(filename) {
  load();
}

void PlanStragety::load() {
  vws::properties::Config config;
  vws::properties::JsonSerializer::fromJson(config, filename_);
  auto robot = config.mapGetChild("Robot");
  teachPose_ = robot.mapGetChild("TeachPose");
  transition_ = robot.mapGetChild("Transition");
  speed_ = robot.mapGetChild("speed");
  tactic_ = robot.mapGetChild("PlanTactic");
  strategies_ = robot.mapGetChild("Stragety");

  if (!robot.mapGetBool("YInvert", &invert_)) {
    std::cout << "Error: YInvert info(bool) is missing" << std::endl;
    abort();
  }

  if (!robot.mapGetBool("YIncrease", &y_increase_)) {
    std::cout << "Error: YIncrease info(bool) is missing" << std::endl;
    abort();
  }

  if (!robot.mapGetInt("ndof", &ndof)) {
    std::cout << "Error: ndof info(int string) is missing" << std::endl;
    abort();
  }

  double *vars[] = {&paint_extreme_pos_, &front_seam_space_,
                    &back_seam_space_,   &front_plane_space_,
                    &back_plane_space_,  &pos_of_paint_house_,
                    &guard_dist_,        &transition_radius_,
                    &extra_axis_max_,    &extra_axis_min_};
  const char *var_names[] = {"PaintExtremePos",
                             "FrontSeamExtraPaintSpace",
                             "BackSeamExtraPaintSpace",
                             "FrontPlanePaintSpace",
                             "BackPlanePaintSpace",
                             "PosOfPaintHouse",
                             "GuardDist",
                             "TransitionRadius",
                             "ExtraAxisMax",
                             "ExtraAxisMin"};
  for (int i = 0; i < sizeof(var_names) / sizeof(const char *); i++) {
    float f;
    if (!robot.mapGetFloat(QString::fromLatin1(var_names[i]), &f)) {
      std::cout << "Error: " << var_names[i] << " info(float) is missing"
                << std::endl;
      abort();
    } else {
      std::cout << var_names[i] << " is " << f << std::endl;
    }
    *vars[i] = f;
  }
  auto it = teachPose_.mapIterator();
  while (it.isValid()) {
    auto jv_array = it.currentChild().mapGetChild("joints");
    int N = jv_array.listLength();
    Eigen::VectorXd jv(N);
    for (int i = 0; i < N; i++) {
      jv[i] = jv_array.listChildAt(i).getValue().toDouble();
    }
    teachPoseMap_[it.currentKey().toStdString()] = jv / 180.0 * M_PI;
    it.advance();
  }

  it = transition_.mapIterator();
  while (it.isValid()) {
    auto jv_array = it.currentChild();
    int N = jv_array.listLength();
    Eigen::VectorXd jv(N);
    bool ok = false;
    for (int i = 0; i < N; i++) {
      jv[i] = jv_array.listChildAt(i).getValue().toDouble(&ok);
      if (!ok) {
        std::cout << "joint values is not double" << std::endl;
        abort();
      }
    }
    transitionsMap_[it.currentKey().toStdString()] = jv / 180.0 * M_PI;
    it.advance();
  }

  it = speed_.mapIterator();
  while (it.isValid()) {
    float s;
    bool ok;
    s = it.currentChild().getValue().toDouble(&ok);
    if (ok) {
      speedMap_[it.currentKey().toStdString()] = s;
    } else {
      std::cout << "speed configuration error" << std::endl;
      abort();
    }
    it.advance();
  }

  it = strategies_.mapIterator();
  while (it.isValid()) {
    strategiesMap_[it.currentKey().toStdString()] =
        it.currentChild().getValue().toString().toStdString();
    it.advance();
  }
}

bool PlanStragety::speedOf(const std::string &key, double &s) const {
  if (speedMap_.find(key) != speedMap_.end()) {
    s = speedMap_.at(key);
    return true;
  }
  return false;
}
bool PlanStragety::transitionOf(const std::string &from, const std::string &to,
                                Eigen::VectorXd &t) const {
  std::string key = from + "-" + to;
  std::string ikey = to + "-" + from;
  if (transitionsMap_.find(key) != transitionsMap_.end()) {
    t = transitionsMap_.at(key);
    Eigen::VectorXd toVec;
    teachPoseOf(to, toVec);
    std::vector<Eigen::VectorXd> vecs;
    vecs.push_back(t);
    vecs.push_back(toVec);
    t = mergeVectors(vecs);
    return true;
  } else if (transitionsMap_.find(ikey) != transitionsMap_.end()) {
    t = transitionsMap_.at(ikey);
    Eigen::VectorXd toVec;
    teachPoseOf(to, toVec);
    std::vector<Eigen::VectorXd> vecs;
    vecs.push_back(reverseVector(t, ndof));
    vecs.push_back(toVec);
    t = mergeVectors(vecs);
    return true;
  }
  return false;
}
bool PlanStragety::teachPoseOf(const std::string &key,
                               Eigen::VectorXd &pose) const {
  if (teachPoseMap_.find(key) != teachPoseMap_.end()) {
    pose = teachPoseMap_.at(key);
    return true;
  }
  return false;
}
bool PlanStragety::tacticOf(const std::string &name, std::string &work_tactic,
                            std::string &cancle_tactic, std::string &follow,
                            float &follow_offset) const {
  int N = tactic_.listLength();
  for (int i = 0; i < N; i++) {
    auto t = tactic_.listChildAt(i);
    QString tactic_name;
    t.mapGetString("name", &tactic_name);
    if (name.compare(tactic_name.toStdString()) == 0) {
      QString ss;
      t.mapGetString("work", &ss);
      work_tactic = ss.toStdString();
      t.mapGetString("cancle", &ss);
      cancle_tactic = ss.toStdString();
      t.mapGetString("follow", &ss);
      follow = ss.toStdString();
      t.mapGetFloat("followOffset", &follow_offset);
      return true;
    }
  }
  return false;
}
bool PlanStragety::strategyOf(bool issingle, bool isup, bool isfront,
                              std::string &strategy) const {
  std::string single_str = issingle ? "single" : "multi";
  std::string up_str = isup ? "-up" : "-down";
  std::string front_str = isfront ? "-front" : "-back";
  std::string key;
  if (issingle) {
    key = single_str + up_str + front_str;
  } else {
    key = single_str + front_str;
  }

  CLog::getInstance()->log("规划， 使用策略： " + QString(key.c_str()));

  if (strategiesMap_.find(key) != strategiesMap_.end()) {
    strategy = strategiesMap_.at(key);
    return true;
  } else {
    std::cout << "no stragety named " << key << " is defined" << std::endl;
    return false;
  }
}
//************************************************************

TrajectoryProcess::TrajectoryProcess()
    : stragety1_("Data/trajgen1.json"), stragety2_("Data/trajgen2.json") {
  visionContext = std::make_shared<VisionContext>();
  logfile_.open("log.txt", std::ios_base::trunc);
}

TrajectoryProcess::~TrajectoryProcess() {}

bool ahead(int64_t a, int64_t relative, bool dir) {
  if (dir) {
    return a < relative;
  } else {
    return a > relative;
  }
}

PlanTask TrajectoryProcess::tryGetPlanTask(vws::PlanTaskInfo *task,
                                           std::vector<vws::PlanTaskInfo> &q1,
                                           std::vector<vws::PlanTaskInfo> &q2,
                                           int64_t encoder_off1,
                                           int64_t encoder_off2,
                                           int64_t current_encoder,
                                           bool isIncrease, float units) {
  PlanTask env_info;
  float valid_range = 3000; // mm
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
    if (utask.encoder + encoder_off1 == encoder)
      continue;
    if (ahead(utask.encoder + encoder_off1, encoder, isIncrease) &&
        utask.face == 1 &&
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
PlanTask TrajectoryProcess::tryGetPlanTaskInTwoLayer(
    vws::PlanTaskInfo *task, std::vector<vws::PlanTaskInfo> &q1,
    std::vector<vws::PlanTaskInfo> &q2, int64_t encoder_off1,
    int64_t encoder_off2, int64_t current_encoder, bool isIncrease,
    float units) {
  PlanTask env_info;
  float valid_range = 3000; // mm
                            //    float speed = vdata->getChainSpeed();  // mm/s

  float expire_t = 4 * 1000;
  int64_t expire_en = expire_t * units;
  int64_t coplan_range = 300 * units;
  int64_t plan_range = 2000 * units;
  int64_t plan_delay = 2200 * units;
  int equal_th = 400;
  bool face = task->face;

  // 规划上层前端面
  // 看下层编码器
  auto encoder =
      task->isup ? task->encoder + encoder_off1 : task->encoder + encoder_off2;
  env_info.targets_.push_back(*task);
  task->flag = true;

  enum LAYERS_INFO { UP, DOWN, BOTH };
  LAYERS_INFO layer = task->isup ? UP : DOWN;

  // q2 一定不同层
  for (int i = 0; i < q2.size(); i++) {
    vws::PlanTaskInfo btask = q2[i];
    if (llabs(btask.encoder + encoder_off2 - encoder) < coplan_range &&
        btask.face == task->face && btask.flag == false) {
      env_info.targets_.push_back(btask);
      q2[i].flag = 1;
    } else if (plan_range > llabs(btask.encoder + encoder_off2 - encoder)) {
      env_info.envs_.push_back(btask);
    }
  }

  for (int i = 0; i < q1.size(); i++) {
    vws::PlanTaskInfo btask = q1[i];
    if (llabs(btask.encoder + encoder_off2 - encoder) < coplan_range &&
        btask.face == task->face && btask.flag == false) {
      env_info.targets_.push_back(btask);
      q1[i].flag = 1;
    } else if (plan_range > llabs(btask.encoder + encoder_off2 - encoder)) {
      env_info.envs_.push_back(btask);
    }
  }
  // // 上层队列中
  // for (int i = 0; i < q1.size(); i++) {
  //   vws::PlanTaskInfo utask = q1[i];
  //   if (utask.encoder + encoder_off1 == encoder){

  //     continue;
  //   }
  //   if (ahead(utask.encoder + encoder_off1, encoder, isIncrease) &&
  //   utask.face == 1 &&
  //       plan_range > llabs(utask.encoder + encoder_off1 - encoder)) {
  //     env_info.envs_.push_back(utask);
  //   } else if (!ahead(utask.encoder + encoder_off1, encoder, isIncrease) &&
  //               utask.face == 0 &&
  //               plan_range > llabs(utask.encoder + encoder_off1 - encoder)) {
  //     env_info.envs_.push_back(utask);
  //   }
  // }
  return env_info;
}

Eigen::Quaterniond getPaintOrientation(bool front, bool invert) {
  Eigen::Matrix3d rot;
  if (!invert && front) {
    // rot 对应矩阵为旋转矩阵，每一列对应
    rot << 1, 0, 0, //
        0, 0, -1,   //
        0, 1, 0;
  } else if (!invert && !front) {
    rot << 1, 0, 0, //
        0, 0, 1,    //
        0, -1, 0;
  } else if (invert && front) {
    rot << -1, 0, 0, //
        0, 0, -1,    //
        0, -1, 0;
  } else {
    rot << -1, 0, 0, //
        0, 0, 1,     //
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
Eigen::Quaterniond getSeamPaintOrientation(bool front, bool invert) {
  Eigen::Matrix3d rot;
  // rot 对应矩阵为旋转矩阵，每一列对应
  if (front && !invert) {
    rot << 1, 0, 0, //
        0, 0, 1,    //
        0, -1, 0;
  } else if (!front && !invert) {
    rot << 1, 0, 0, //
        0, 0, -1,   //
        0, 1, 0;
    // rot = rot * Eigen::AngleAxisd(M_PI / 4.0, Eigen::Vector3d::UnitY());
  } else if (invert && front) {
    rot << -1, 0, 0, //
        0, 0, 1,     //
        0, 1, 0;
  } else {
    rot << -1, 0, 0, //
        0, 0, -1,    //
        0, -1, 0;
  }
  rot = rot * Eigen::AngleAxisd(M_PI / 4.0, Eigen::Vector3d::UnitY());

  Eigen::Quaterniond quat(rot);
  return quat;
}

void AbbRbt(Eigen::VectorXd planRet, RobotTask &tk) {
  int cnt = 0;
  for (int i = 0; i < planRet.size() / 6; i++) {
    std::cout << i << ": ";
    std::array<float, 7> jv;
    for (int n = 0; n < 6; n++) {
      jv[n] = planRet[cnt++] / M_PI * 180.0;
      // IRB 1410机器人3轴和2轴耦合，因此需要添加下面的补偿
#ifdef ABB1410_JOINT_CONVERT
      if (n == 2) {
        jv[n] += jv[n - 1];
      }
#endif
      std::cout << jv[n] << ", ";
    }
    std::cout << std::endl;
    // tk.track.push_back(jv);  //TODO: 数据结构修改
  }
}

void AbbRbt(Eigen::VectorXd planRet, RobotTask &tk, int index) {
  int cnt = 0;
  for (int i = 0; i < planRet.size() / 6; i++) {
    std::cout << i << ": ";
    std::array<float, 7> jv;
    for (int n = 0; n < 6; n++) {
      jv[n] = planRet[cnt++] / M_PI * 180.0;
      // IRB 1410机器人3轴和2轴耦合，因此需要添加下面的补偿
#ifdef ABB1410_JOINT_CONVERT
      if (n == 2) {
        jv[n] += jv[n - 1];
      }
#endif
      std::cout << jv[n] << ", ";
    }
    std::cout << std::endl;
    //  tk.track[index].push_back(jv);
  }
}

void Rbt(Eigen::VectorXd planRet, float speed, VWSRobot::TaskType taskType,
         std::vector<RobotTask> &rbt_tasks) {
  int cnt = 0;
  for (int i = 0; i < planRet.size() / 6; i++) {
    std::cout << i << ": ";
    std::array<float, 7> jv;
    for (int n = 0; n < 6; n++) {
      jv[n] = planRet[cnt++] / M_PI * 180.0;
      // IRB 1410机器人3轴和2轴耦合，因此需要添加下面的补偿
#ifdef ABB1410_JOINT_CONVERT
      if (n == 2) {
        jv[n] += jv[n - 1];
      }
#endif
      std::cout << jv[n] << ", ";
    }
    std::cout << std::endl;
    //  tk.track[index].push_back(jv);
    RobotTask rbttask;
    rbttask.point = jv;
    rbttask.speed[0] = speed;
    rbttask.speed[1] = speed;
    rbttask.taskType = taskType;
    rbt_tasks.push_back(rbttask);
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
    if (init) {
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
  if (genBottom) {
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
  std::cout << "current_pos : " << current_pos << ", L: " << Pu << " "
            << Pu + Lu << ", " << Pd << " " << Pd + Ld << ", flag : " << genUp
            << ", " << genBottom << std::endl;
  if (current_pos >= Pu && last_pos < Pu) {
    vws::PlanTaskInfo info;
    info.boxInfo = Eigen::Isometry3d::Identity();
    info.boxInfo.translate(
        Eigen::Vector3d(2200, -3000 - Lu / 2.0, -500 + 1200));
    info.boxInfo.rotate(Rd);
    info.lx = W;
    info.ly = Lu;
    info.lz = H;
    info.face = 0;
    info.encoder = init_encoder - (Pu * units);
    info.diff = 0;
    info.flag = 0;
    info.isup = true;
    vdata->GetPlanTaskInfo(1)->push_back(info);
    std::cout << "push up 0: " << info.encoder << std::endl;
  }
  if (current_pos >= Pu + Lu && last_pos < Pu + Lu) {
    vws::PlanTaskInfo info;
    info.boxInfo = Eigen::Isometry3d::Identity();
    info.boxInfo.translate(
        Eigen::Vector3d(2200, -3000 + Lu / 2.0, -500 + 1200));
    info.boxInfo.rotate(Rd);
    info.lx = W;
    info.ly = Lu;
    info.lz = H;
    info.face = 1;
    info.encoder = init_encoder - ((Pu + Lu) * units);
    info.diff = 0;
    info.flag = 0;
    info.isup = true;
    vdata->GetPlanTaskInfo(1)->push_back(info);
    genUp = true;
    // genBottom = (((float)rand() / RAND_MAX - 0.5) > 0.5);
    genBottom = true;
    std::cout << "push up 1: " << info.encoder << std::endl;
  }
  if (current_pos >= Pd && last_pos < Pd) {
    vws::PlanTaskInfo info;
    info.boxInfo = Eigen::Isometry3d::Identity();
    info.boxInfo.translate(Eigen::Vector3d(2200, -3000 - Ld / 2.0, -500));
    info.boxInfo.rotate(Rd);
    info.lx = W;
    info.ly = Ld;
    info.lz = H;
    info.face = 0;
    info.encoder = init_encoder - (Pd * units);
    info.diff = 0;
    info.flag = 0;
    info.isup = false;
    vdata->GetPlanTaskInfo(0)->push_back(info);
    std::cout << "push bottom 0: " << info.encoder << std::endl;
  }
  if (current_pos >= Pd + Ld && last_pos < Pd + Ld) {
    vws::PlanTaskInfo info;
    info.boxInfo = Eigen::Isometry3d::Identity();
    info.boxInfo.translate(Eigen::Vector3d(2200, -3000 + Ld / 2.0, -500));
    info.boxInfo.rotate(Rd);
    info.lx = W;
    info.ly = Ld;
    info.lz = H;
    info.face = 1;
    info.encoder = init_encoder - ((Pd + Ld) * units);
    info.diff = 0;
    info.flag = 0;
    info.isup = false;
    vdata->GetPlanTaskInfo(0)->push_back(info);
    std::cout << "push bottom 1: " << info.encoder << std::endl;
  }
  last_pos = current_pos;
}

SortedTaskQ TrajectoryProcess::PrepareTaskInfoOneByOne(
    std::vector<vws::PlanTaskInfo> *upper_task_q,
    std::vector<vws::PlanTaskInfo> *bottom_task_q, int64_t current_encoder,
    int64_t bottom_2_upper, bool isIncrease, float units, int64_t plan_delay) {
  SortedTaskQ taskQ;
  //
  PlanTask env_info;
  for (int i = 0; i < upper_task_q->size(); i++) {
    auto task = &(*upper_task_q)[i];
    if (task->flag == false && (task->encoder - current_encoder) > plan_delay) {
      env_info =
          tryGetPlanTask(&(*upper_task_q)[i], *upper_task_q, *bottom_task_q, 0,
                         bottom_2_upper, current_encoder, isIncrease, units);
      std::cout << "taskQ : push up " << task->encoder << ", "
                << (task->face == 0 ? "head " : "tail") << std::endl;
      taskQ.push(std::make_pair(task->encoder, env_info));
    }
  }
  for (int i = 0; i < bottom_task_q->size(); i++) {
    auto task = &(*bottom_task_q)[i];
    if (task->flag == false && (task->encoder - current_encoder) > plan_delay) {
      env_info =
          tryGetPlanTask(&(*bottom_task_q)[i], *bottom_task_q, *upper_task_q,
                         bottom_2_upper, 0, current_encoder, isIncrease, units);
      std::cout << "taskQ : push bottom " << task->encoder << ", "
                << (task->face == 0 ? "head " : "tail") << std::endl;

      taskQ.push(std::make_pair(task->encoder + bottom_2_upper, env_info));
    }
  }
  return taskQ;
}

SortedTaskQ TrajectoryProcess::PrepareTaskInfoOneLayer(
    std::vector<vws::PlanTaskInfo> *q1, std::vector<vws::PlanTaskInfo> *q2,
    int64_t current_encoder, int64_t q1_offset, int64_t q2_offset,
    bool isIncrease, float units, int64_t plan_delay) {
  SortedTaskQ taskQ;
  //
  PlanTask env_info;
  for (int i = 0; i < q1->size(); i++) {
    auto task = &(*q1)[i];
    if (task->flag == false && (task->encoder - current_encoder) > plan_delay) {
      env_info = tryGetPlanTask(&(*q1)[i], *q1, *q2, q1_offset, q2_offset,
                                current_encoder, isIncrease, units);
      std::cout << "taskQ : push up " << task->encoder << ", "
                << (task->face == 0 ? "head " : "tail") << std::endl;
      taskQ.push(std::make_pair(task->encoder, env_info));
    }
  }
  return taskQ;
}

SortedTaskQ TrajectoryProcess::PrepareTaskInfoTwoLayers(
    ThreadSafeVector<vws::PlanTaskInfo> *safe_upper_task_q,
    ThreadSafeVector<vws::PlanTaskInfo> *safe_bottom_task_q,
    int64_t current_encoder, int64_t bottom_2_upper, bool isIncrease,
    float units, int64_t plan_delay) {
  SortedTaskQ taskQ;
  std::vector<vws::PlanTaskInfo> *upper_task_q, *bottom_task_q;
  upper_task_q = &(safe_upper_task_q->getVector());
  bottom_task_q = &(safe_bottom_task_q->getVector());
  //
  PlanTask env_info;
  for (int i = 0; i < upper_task_q->size(); i++) {
    auto task = &(*upper_task_q)[i];
    if (task->flag == false && (task->encoder - current_encoder) > plan_delay) {
      env_info = tryGetPlanTaskInTwoLayer(&(*upper_task_q)[i], *upper_task_q,
                                          *bottom_task_q, 0, bottom_2_upper,
                                          current_encoder, isIncrease, units);
      // std::cout << "taskQ : push up " << task->encoder << ", " << (task->face
      // == 0 ? "head " : "tail") << std::endl;
      CLog::getInstance()->log("taskQ : push up " +
                               QString::number(task->encoder) + ", " +
                               (task->face == 0 ? "head " : "tail"));
      taskQ.push(std::make_pair(task->encoder, env_info));
    }
  }
  for (int i = 0; i < bottom_task_q->size(); i++) {
    auto task = &(*bottom_task_q)[i];
    if (task->flag == false && (task->encoder - current_encoder) > plan_delay) {
      env_info = tryGetPlanTaskInTwoLayer(&(*bottom_task_q)[i], *upper_task_q,
                                          *bottom_task_q, 0, bottom_2_upper,
                                          current_encoder, isIncrease, units);
      // std::cout << "taskQ : push down " << task->encoder  + bottom_2_upper<<
      // ", " << (task->face == 0 ? "head " : "tail") << std::endl;
      CLog::getInstance()->log("taskQ : push down " +
                               QString::number(task->encoder + bottom_2_upper) +
                               ", " + (task->face == 0 ? "head " : "tail"));
      taskQ.push(std::make_pair(task->encoder + bottom_2_upper, env_info));
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
void createPlanScene(TrajectoryGenerator *generator, PlanTask &task_info,
                     float units, bool invert) {
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
      // invert ^ increase = true: 取反
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

double computeDiff(vws::PlanTaskInfo &task, TrajectoryGenerator *generator,
                   float offset, Eigen::Vector3d &p1, bool invert) {
  int64_t ref_encoder = task.encoder;
  auto boxInfo = task.boxInfo;
  auto boxSize = Eigen::Vector3d(task.lx, task.ly, task.lz);
  auto boxCenter = boxInfo.translation();

  // 计算时首先在机器人坐标系下计算出左下方点p1，
  // 然后根据偏移要求，计算偏移后的位置。
  // 对于模式2,p1点的位置在箱体另一侧。
  // dest_y 为跟随时p1点y分量在机器人坐标系下应该处于的位置。
  // 前端面和后端面对应的偏移量符号相反。
  p1 = generator->bottomnearpont(boxInfo.translation(), boxSize, invert);
  // float dest_y = -task.diff;
  float dest_y = invert ? offset : -offset;
  if (!((task.face == 0))) {
    p1[1] = p1[1] - boxSize[1];
  }
  p1 =
      boxInfo.rotation() * (p1 - boxInfo.translation()) + boxInfo.translation();
  float diff = dest_y - p1[1];
  return diff;
}

bool planTwoLayersTask(TrajectoryGenerator *generator, PlanTask &task_info,
                       const Eigen::VectorXd &init_dof,
                       std::vector<TrajectoryInfo> &out_traj,
                       std::vector<float> &mc_data, float units,
                       bool isIncrease, bool invert, bool plane_first = true,
                       bool plan_weld = true) {
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
  p1 =
      boxInfo.rotation() * (p1 - boxInfo.translation()) + boxInfo.translation();
  float diff = dest_y - p1[1];

  boxCenter[1] += diff;

  generator->GenerateShrinkedPaintConstraint(
      boxCenter, boxSize, Eigen::Quaterniond(boxInfo.rotation()),
      getPaintOrientation(task.face == 0, invert), 0, boxSize[2] / 2.0 - 50,
      task.face == 0, invert, 0, p, ori);
  generator->GenerateSeamPaintConstraint(
      boxCenter, boxSize, Eigen::Quaterniond(boxInfo.rotation()),
      getSeamPaintOrientation(task.face == 0, invert), 20, boxSize[2] - 200,
      task.face == 0, invert, p_weld, ori_weld);

  std::vector<Eigen::VectorXd> plan_path, plan_ori;
  if (plane_first) {
    plan_path.push_back(p);
    plan_ori.push_back(ori);
    plan_path.push_back(p_weld);
    plan_ori.push_back(ori_weld);
  } else {
    plan_path.push_back(p_weld);
    plan_ori.push_back(ori_weld);
    plan_path.push_back(p);
    plan_ori.push_back(ori);
  }
}

bool getStragety(PlanTask &task_info, PlanStragety &stragety, std::string &s) {
  int n = task_info.targets_.size();
  bool isfront = task_info.targets_.front().face == 0;
  bool isup = task_info.targets_.front().isup;
  // 第一个机器人
  if (stragety.strategyOf(n == 1, isup, isfront, s)) {
    return true;
  } else {
    return false;
  }
}

bool validCancleTactic(const PlanStragety &stragety,
                       const std::string &tactic) {
  if (tactic == "") {
    return true;
  }
  QString qs = QString::fromStdString(tactic);
  auto names = qs.split("-");
  Eigen::VectorXd pose;
  for (int i = 0; i < names.size(); i++) {
    std::string tpose = names[i].toStdString();
    if (!stragety.teachPoseOf(tpose, pose)) {
      return false;
    }
  }
  return true;
}
bool validWorkTactic(const PlanStragety &stragety, const std::string &tactic,
                     int &safe_index) {
  safe_index = -1;
  QString qs = QString::fromStdString(tactic);
  auto names = qs.split("-");
  bool isSafe = true;
  Eigen::VectorXd pose;
  std::string valid_chars[3] = {"sp", "ud", "fb"};
  for (int i = 0; i < names.size(); i++) {
    std::string tpose = names[i].toStdString();
    if (stragety.teachPoseOf(tpose, pose)) {
      if (isSafe) {
        safe_index = i;
      }
      continue;
    }
    isSafe = false;
    // 不是示教点，看是否满足条件
    int N = tpose.size();
    if (N > 3 || N <= 0) {
      return false;
    }
    for (int n = 0; n < N; n++) {
      if (valid_chars[n].find(tpose[n]) == valid_chars[n].size()) {
        return false;
      }
    }
  }
  if (safe_index == -1) {
    return false;
  }
  return true;
}

bool getTaskInfo(PlanTask &task_info, bool isup, vws::PlanTaskInfo &info) {
  for (auto it = task_info.targets_.begin(); it != task_info.targets_.end();
       it++) {
    if (it->isup == isup) {
      info = *it;
      return true;
    }
  }
  return false;
}

bool planPaintPath(TrajectoryGenerator *generator, Eigen::VectorXd &init_dof,
                   Eigen::VectorXd &p, Eigen::VectorXd &ori,
                   std::vector<TrajectoryInfo> &out_traj,
                   Eigen::VectorXd &next_dof, int ndof, bool forward,
                   bool isSeam) {
  TrajectoryInfo single_traj;
  Eigen::VectorXd traj, entry_traj;
  auto init_pose = init_dof;
  auto plan_path = p;
  auto plan_ori = ori;
  if (!forward) {
    plan_path = reverseVector(p, 3);
    plan_ori = reverseVector(ori, 4);
  }
  bool ret;

  std::cout << "Gen Paint Path " << std::endl;
  ret = generator->GeneratePaintTrajectory(init_pose, plan_path, plan_ori, traj,
                                           ndof);
  if (ret) {
    int N = traj.size() / ndof;
    Eigen::VectorXd paint_start, paint_end;
    paint_start = traj.block(0, 0, ndof, 1);
    std::cout << "Gen Entry Path " << std::endl;
    ret &=
        generator->GenerateEntryTrajectory(init_pose, traj.block(0, 0, ndof, 1),
                                           6, entry_traj, ndof, 3, true); //显示
    if (ret) {
      //   logfile_ << "Entry Path dist " << " - " <<
      //   generator->pathDist(entry_traj, ndof)
      //           << std::endl;
      single_traj.traj_ = entry_traj;
      single_traj.type_ = TrajectoryType::UnsafeTransitionTraj;
      out_traj.push_back(single_traj);
      single_traj.traj_ = traj;
      single_traj.type_ = isSeam ? TrajectoryType::SeamPaintTraj
                                 : TrajectoryType::PlanePaintTraj;
      out_traj.push_back(single_traj);
    } else {
      return false;
    }
    next_dof = traj.block(ndof * (N - 1), 0, ndof, 1);
  } else {
    return false;
  }
  return true;
}

/**
 * @brief Create a Plan Scene object
 *
 * @param generator
 * @param task_info
 * @param units
 * @param invert
 */
bool createPlanSceneUsingTactic(TrajectoryGenerator *generator,
                                PlanTask &task_info, PlanStragety &stragety,
                                float units) {
  // 生成喷涂约束
  std::string s;
  bool ret = getStragety(task_info, stragety, s);
  if (!ret) {
    return ret;
  }
  bool invert = stragety.isInvert();
  std::string work_tactic, cancle_tactic, follow;
  float follow_offset;
  TrajectoryInfo singleTraj;
  ret = stragety.tacticOf(s, work_tactic, cancle_tactic, follow, follow_offset);
  if (!ret) {
    std::cout << "no tactic named " << s << " is defined " << std::endl;
    return ret;
  }
  bool isfollowup = follow[0] == 'u';
  vws::PlanTaskInfo follow_info;
  Eigen::Vector3d p1;
  getTaskInfo(task_info, isfollowup, follow_info);
  bool isfollowfront = follow_info.face == 0;
  int64_t follow_encoder = follow_info.encoder;
  // diff need to be added to boxcenter[1]
  double diff = computeDiff(follow_info, generator, follow_offset, p1, invert);

  auto task = follow_info;
  int64_t ref_encoder = task.encoder;
  auto boxInfo = task.boxInfo;
  auto boxSize = Eigen::Vector3d(task.lx, task.ly, task.lz);
  auto boxCenter = boxInfo.translation();
  int ndof = stragety.getNdof();

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

bool getExtraPaintSpace(PlanStragety &stragety, const std::string &name,
                        bool isFront, double &space) {
  switch (name[0]) {
  case 's':
    space = isFront ? stragety.getFrontSeamExtraPaintSpace()
                    : stragety.getBackSeamExtraPaintSpace();
    return true;
    break;
  case 'p':
    space = isFront ? stragety.getFrontPlanePaintSpace()
                    : stragety.getBackPlanePaintSpace();
    return true;
  default:
    return false;
  }
}

/**
 * @brief Create a Plan Scene object
 *
 * @param generator
 * @param task_info
 * @param units
 * @param invert
 */
bool createSafeRegionUsingTactic(TrajectoryGenerator *generator,
                                 PlanTask &task_info, PlanStragety &stragety,
                                 float units) {
  // 生成喷涂约束
  std::string s;
  bool ret = getStragety(task_info, stragety, s);
  if (!ret) {
    return ret;
  }
  bool invert = stragety.isInvert();
  std::string work_tactic, cancle_tactic, follow;
  float follow_offset;
  TrajectoryInfo singleTraj;
  ret = stragety.tacticOf(s, work_tactic, cancle_tactic, follow, follow_offset);
  if (!ret) {
    std::cout << "no tactic named " << s << " is defined " << std::endl;
    return ret;
  }
  bool isfollowup = follow[0] == 'u';
  vws::PlanTaskInfo follow_info;
  Eigen::Vector3d p1;
  getTaskInfo(task_info, isfollowup, follow_info);
  bool isfollowfront = follow_info.face == 0;
  int64_t follow_encoder = follow_info.encoder;
  // diff need to be added to boxcenter[1]
  auto task = follow_info;
  int64_t ref_encoder = task.encoder;
  double diff = computeDiff(follow_info, generator, follow_offset, p1, invert);

  auto boxInfo = task.boxInfo;
  auto boxSize = Eigen::Vector3d(task.lx, task.ly, task.lz);
  auto boxCenter = boxInfo.translation();
  int ndof = stragety.getNdof();

  double ymax = stragety.getTransitionRadius();
  double start_y = 0;
  double first_paint_y = 2000;
  int safe_index;
  if (!validWorkTactic(stragety, work_tactic, safe_index)) {
    return false;
  }
  // 计算喷涂时候机器人动作在Y方向上的最大距离。
  QStringList seq = QString::fromStdString(work_tactic).split('-');
  for (int i = safe_index + 1; i < seq.size(); i++) {
    vws::PlanTaskInfo current_task;
    bool isup = seq[i][1] == 'u';
    getTaskInfo(task_info, isup, current_task);
    bool isfront = current_task.face == 0;
    double space;
    Eigen::Vector3d corner_point;
    computeDiff(current_task, generator, 0, corner_point, invert);
    int64_t diff_encoder = current_task.encoder - ref_encoder;
    float diff_mm = diff_encoder / units;
    diff_mm = invert ? -diff_mm : diff_mm;
    // 假设：粉房均在机器人的y负方向上
    double paint_y = corner_point[1] + diff + diff_mm;
    first_paint_y = first_paint_y < paint_y ? first_paint_y : paint_y;
    if (getExtraPaintSpace(stragety, seq[i].toStdString(), isfront, space)) {
      // 喷涂目标中首先开始的点的位置为y方向最小。
      double d = paint_y - space;
      d = fabs(d);
      ymax = d > ymax ? d : ymax;
    }
  }
  std::cout << "ymax is " << ymax << std::endl;
  // 创建粉房安全平面
  auto wallCenter = boxCenter;
  wallCenter[1] = -ymax;
  // todo::
  // 这里存在默认假设，即invert为true的机器人安装在粉房前面，invert为false的安装在粉房后面
  generator->AddBoxEnvirInfo(wallCenter, Eigen::Vector3d(4000, 1, 2000),
                             Eigen::Quaterniond(1, 0, 0, 0),
                             QString("paint_house").toStdString());

  double pos_paint_house = stragety.getPosOfPaintHouse();
  double dpos_neg, dpos_pos;
  bool isIncrease = stragety.isYIncrease();
  double axis_max, axis_min;
  axis_max = stragety.getExtraAxisMax();
  axis_min = stragety.getExtraAxisMin();
  double paint_extremity = stragety.getPaintExtremePos();
  double paint_guard_dist = stragety.getGuardDist();
  if (invert) {
    // invert 决定了机器人与粉房的位置关系
    // 第一个机器人，粉房前喷涂，
    if (isIncrease) {
      // 机器人编码器为增加的，则粉房位于max端
      dpos_neg = pos_paint_house - paint_extremity + first_paint_y;
      dpos_pos = pos_paint_house - ymax - paint_guard_dist;
    } else {
      // 粉房位于min端
      dpos_neg = pos_paint_house + ymax + paint_guard_dist;
      dpos_pos = pos_paint_house + paint_extremity - first_paint_y;
    }
  } else {
    // 第二个机器人, first_paint_y 含义为最后一个需要喷涂的点位。
    if (isIncrease) {
      // 粉房位于min端
      dpos_neg = pos_paint_house + ymax + paint_guard_dist;
      dpos_pos = pos_paint_house + paint_extremity - first_paint_y;
    } else {
      // 粉房位于max端
      dpos_neg = pos_paint_house - paint_extremity + first_paint_y;
      dpos_pos = pos_paint_house - ymax - paint_guard_dist;
    }
  }
  dpos_neg = fmax(fmin(dpos_neg, axis_max), axis_min);
  dpos_pos = fmax(fmin(dpos_pos, axis_max), axis_min);
  std::cout << "dpos_neg : " << dpos_neg << ", dpos_pos : " << dpos_pos
            << std::endl;
}

bool planTaskUsingTactic(TrajectoryGenerator *generator, PlanTask &task_info,
                         PlanStragety &stragety,
                         std::vector<TrajectoryInfo> &out_safe_traj,
                         std::vector<TrajectoryInfo> &out_unsafe_traj,
                         std::vector<TrajectoryInfo> &out_cancle_traj,
                         std::vector<float> &mc_data, float units,
                         bool isIncrease, int ndof) {
  std::string s;
  // 获取规划策略，规划策略定义了：
  // 单层规划和双层规划
  // 规划时候的工作轨迹顺序和取消任务轨迹。
  // 字符串定义了规划的顺序，以 '-' 分割：
  // 例如 "d2-pdb-d2-u2-puf-u2" 含义如下：
  // d2 位于 teachPose中，因此为示教点
  // pdb ： p表示规划面，d表示down，下层，b表示正向规划，即计算出轨迹的约束反向
  // u2 位于 teachPose中，因此为示教点
  // puf 中u表示up，上层。
  // 整体含义为：
  // 从示教点d2移动到下层平面轨迹的最后一个点，进行喷涂任务到达第一个点，
  // 再移动到示教点d2,
  // 再移动到示教点u2，移动到上层平面喷涂轨迹的第一个点，喷涂后到达最后一个点，
  // 再移动到u2示教点。
  bool ret = getStragety(task_info, stragety, s);
  if (!ret) {
    return ret;
  }
  bool invert = stragety.isInvert();
  std::string work_tactic, cancle_tactic, follow;
  TrajectoryInfo singleTraj;
  float follow_offset;
  ret = stragety.tacticOf(s, work_tactic, cancle_tactic, follow, follow_offset);
  if (!ret) {
    std::cout << "no tactic named " << s << " is defined " << std::endl;
    return ret;
  }
  std::cout << "tactic : " << work_tactic << ", " << cancle_tactic << std::endl;

  bool isfollowup = follow[0] == 'u';
  vws::PlanTaskInfo follow_info;
  Eigen::Vector3d p1;
  getTaskInfo(task_info, isfollowup, follow_info);
  bool isfollowfront = follow_info.face == 0;
  int64_t follow_encoder = follow_info.encoder;
  // diff need to be added to boxcenter[1]
  double diff = computeDiff(follow_info, generator, follow_offset, p1, invert);

  int safe_index;

  // 检测work tactic是否有效
  bool isvalid = validWorkTactic(stragety, work_tactic, safe_index);
  if (!isvalid) {
    return false;
  }
  // 检测cancle tactic是否有效
  isvalid = validCancleTactic(stragety, cancle_tactic);
  if (!isvalid) {
    return false;
  }

  // 生成取消轨迹
  QString cancleStr = QString::fromStdString(cancle_tactic);
  QStringList cancle_seq = cancleStr.split("-");
  std::string last_pos = "";
  std::vector<Eigen::VectorXd> cancle_poses;
  for (int i = 0; i < cancle_seq.size(); i++) {
    // todo:: 直接查找transition表，获取轨迹
    Eigen::VectorXd pos;
    std::string current_pos = cancle_seq[i].toStdString();
    stragety.teachPoseOf(current_pos, pos);
    if (last_pos != "") {
      std::string trans_str = last_pos + "-" + current_pos;
      bool ret = stragety.transitionOf(last_pos, current_pos, pos);
      if (!ret) {
        std::cout << "transition not defined: " << trans_str << std::endl;
        return false;
      }
      cancle_poses.push_back(pos);
    } else {
      cancle_poses.push_back(pos);
    }
    last_pos = current_pos;
  }
  if (cancle_poses.size() > 0) {
    singleTraj.type_ = TrajectoryType::SafeTransitionTraj;
    singleTraj.traj_ = mergeVectors(cancle_poses);
    out_cancle_traj.push_back(singleTraj);
  }

  // 生成安全轨迹
  QString workStr = QString::fromStdString(work_tactic);
  QStringList seq = workStr.split("-");
  last_pos = "";
  std::vector<Eigen::VectorXd> safe_poses;
  for (int i = 0; i <= safe_index; i++) {
    // todo:: 直接查找transition表，获取轨迹
    Eigen::VectorXd pos;
    std::string current_pos = seq[i].toStdString();
    stragety.teachPoseOf(current_pos, pos);
    if (last_pos != "") {
      std::string trans_str = last_pos + "-" + current_pos;
      bool ret = stragety.transitionOf(last_pos, current_pos, pos);
      if (!ret) {
        std::cout << "transition not defined: " << trans_str << std::endl;
        return false;
      }
      safe_poses.push_back(pos);
    } else {
      safe_poses.push_back(pos);
    }
    last_pos = current_pos;
  }

  if (safe_poses.size() > 0) {
    singleTraj.type_ = TrajectoryType::SafeTransitionTraj;
    singleTraj.traj_ = mergeVectors(safe_poses);
    out_safe_traj.push_back(singleTraj);
  }

  // 不安全区域轨迹生成。生成方式包括直接读取已有示教轨迹和规划轨迹
  // 生成过程为遍历所有点（例如u2），点分为示教点和喷涂点（p或s开头）
  // 生成时机器人从第一个点开始，每次向后一个点移动。
  // 如果两个点都是示教点，生成方式为直接读取示教点移动轨迹
  // 否则，先规划到后一个点，如果后一个点不是喷涂类点，则结束，
  // 如果后一个点为喷涂类点，则继续规划喷涂轨迹
  std::vector<Eigen::VectorXd> unsafe_poses;
  Eigen::VectorXd init_dof;
  stragety.teachPoseOf(seq[safe_index].toStdString(), init_dof);
  for (int i = safe_index + 1; i < seq.size(); i++) {
    last_pos = seq[i - 1].toStdString();
    std::string current_pos = seq[i].toStdString();
    Eigen::VectorXd pos1, pos2;
    Eigen::VectorXd t;
    bool isteach_1, isteach_2;
    // 两个都是示教点
    isteach_1 = stragety.teachPoseOf(last_pos, pos1);
    isteach_2 = stragety.teachPoseOf(current_pos, pos2);
    if (isteach_1 && isteach_2) {
      init_dof = pos2;
      if (stragety.transitionOf(last_pos, current_pos, t)) {
        singleTraj.type_ = TrajectoryType::UnsafeTransitionTraj;
        singleTraj.traj_ = t;
        out_unsafe_traj.push_back(singleTraj);
      } else {
        return false;
      }
    } else if (!isteach_2) { // 示教点-》喷涂点， 或者 喷涂点-》喷涂点
      // 如果第二个不是示教点，需要先计算喷涂约束，
      // 再计算两段轨迹，第一段为转移，第二段为喷涂
      // 规划焊缝
      bool isup = current_pos[1] == 'u';
      bool isforward = current_pos[2] == 'f';
      vws::PlanTaskInfo info;
      getTaskInfo(task_info, isup, info);
      auto boxCenter = info.boxInfo.translation();
      Eigen::Vector3d boxSize(info.lx, info.ly, info.lz);

      //
      bool neg = isIncrease ^ invert;
      float diff_encoder = (info.encoder - follow_encoder) / units;
      diff_encoder = neg ? -diff_encoder : diff_encoder;
      boxCenter[1] += diff + diff_encoder;
      bool isfront = info.face == 0;
      if (current_pos[0] == 's') {
        Eigen::VectorXd p_weld, ori_weld;
        generator->GenerateSeamPaintConstraint(
            boxCenter, boxSize, Eigen::Quaterniond(info.boxInfo.rotation()),
            getSeamPaintOrientation(isfront, invert), 20, boxSize[2] - 200,
            isfront, invert, p_weld, ori_weld);
        if (!planPaintPath(generator, init_dof, p_weld, ori_weld,
                           out_unsafe_traj, init_dof, ndof, isforward, true)) {
          return false;
        }
      } else {
        // 规划平面
        Eigen::VectorXd p, ori;
        generator->GenerateShrinkedPaintConstraint(
            boxCenter, boxSize, Eigen::Quaterniond(info.boxInfo.rotation()),
            getPaintOrientation(isfront, invert), 0, boxSize[2] / 2.0 - 50,
            isfront, invert, 0, p, ori);
        if (!planPaintPath(generator, init_dof, p, ori, out_unsafe_traj,
                           init_dof, ndof, isforward, false)) {
          return false;
        }
      }
    } else {
      // 喷涂点-》示教点
      Eigen::VectorXd entry_traj;
      if (!generator->GenerateEntryTrajectory(init_dof, pos2, 6, entry_traj,
                                              ndof, 3, true)) { //显示
        return false;
      }
      singleTraj.type_ = TrajectoryType::UnsafeTransitionTraj;
      singleTraj.traj_ = entry_traj;
      out_unsafe_traj.push_back(singleTraj);
      init_dof = pos2;
    }
  }

  mc_data = generator->calChainZeroPoint(p1, follow_offset, follow_info.encoder,
                                         isIncrease, invert);
  mc_data[1] =
      isfollowfront
          ? -follow_info.diff
          : follow_info
                .diff; // 前提是机器人正方向和箱体允许方向相同，否则取负号
  // std::cout << "task encoder: " << follow_info.encoder << std::endl;
  CLog::getInstance()->log("task encoder: " +
                           QString::number(follow_info.encoder));
  // std::cout << "p1 pos: " << p1[0] << ", " << p1[1] << ", " << p1[2] <<
  // std::endl;
  auto box_info = follow_info.boxInfo.translation().transpose();
  CLog::getInstance()->log("box center: " + QString::number(box_info[0]) +
                           ", " + QString::number(box_info[1]) + ", " +
                           QString::number(box_info[2]));
  CLog::getInstance()->log("p1 pos: " + QString::number(p1[0]) + ", " +
                           QString::number(p1[1]) + ", " +
                           QString::number(p1[2]));
  // std::cout << "mc data: " << mc_data[0] << "," << mc_data[1] << std::endl;
  CLog::getInstance()->log("mc data: " + QString::number(mc_data[0]) + "," +
                           QString::number(mc_data[1]));
  return true;
}

/**
 * @brief
 * 规划任务信息中的第一个任务。任务信息中包含场景信息和规划目标信息。只规划其中第一个目标的喷涂路径
 *        目前规划一个面和一条边
 * @param generator
 * @param task_info
 * @param init_dof
 * @param res
 * @param mc_data
 * @param units
 * @param isIncrease
 * @param invert
 * @param plane_first     指示是否先喷面：true - 先喷面再喷焊缝， false -
 * 先喷焊缝再喷面
 * @return true
 * @return false
 */
bool TrajectoryProcess::planOneTask(TrajectoryGenerator *generator,
                                    PlanTask &task_info,
                                    const Eigen::VectorXd &init_dof,
                                    std::vector<TrajectoryInfo> &out_traj,
                                    std::vector<float> &mc_data, float units,
                                    bool isIncrease, bool invert,
                                    bool plane_first) {
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
      getPaintOrientation(task.face == 0, invert), 0, boxSize[2] / 2.0 - 50,
      task.face == 0, invert, 0, p, ori);
  generator->GenerateSeamPaintConstraint(
      boxCenter, boxSize, Eigen::Quaterniond(boxInfo.rotation()),
      getSeamPaintOrientation(task.face == 0, invert), 20, boxSize[2] - 200,
      task.face == 0, invert, p_weld, ori_weld);

  std::vector<Eigen::VectorXd> plan_path, plan_ori;
  if (plane_first) {
    plan_path.push_back(p);
    plan_ori.push_back(ori);
    plan_path.push_back(p_weld);
    plan_ori.push_back(ori_weld);
  } else {
    plan_path.push_back(p_weld);
    plan_ori.push_back(ori_weld);
    plan_path.push_back(p);
    plan_ori.push_back(ori);
  }

  auto init_pose = init_dof;
  bool ret;
  for (int i = 0; i < plan_path.size(); i++) {
    std::cout << "Gen Paint Path " << i << std::endl;
    ret = generator->GeneratePaintTrajectory(init_pose, plan_path[i],
                                             plan_ori[i], traj, ndof);
    if (ret) {
      logfile_ << "Paint Path dist " << traj.size() / ndof << " - "
               << generator->pathDist(traj, ndof) << std::endl;
      int N = traj.size() / ndof;
      Eigen::VectorXd paint_start, paint_end;
      paint_start = traj.block(0, 0, ndof, 1);
      std::cout << "Gen Entry Path " << i << std::endl;
      ret &= generator->GenerateEntryTrajectory(
          init_pose, traj.block(0, 0, ndof, 1), 4, entry_traj, ndof, 3);
      if (ret) {
        logfile_ << "Entry Path dist "
                 << " - " << generator->pathDist(entry_traj, ndof) << std::endl;
        single_traj.traj_ = entry_traj;
        single_traj.type_ = UnsafeTransitionTraj;
        out_traj.push_back(single_traj);
        single_traj.traj_ = traj;
        single_traj.type_ = SeamPaintTraj;
        out_traj.push_back(single_traj);
      } else {
        return false;
      }

      init_pose = traj.block(ndof * (N - 1), 0, ndof, 1);
    } else {
      return false;
    }
  }

  std::cout << "Gen Quit Path " << std::endl;
  ret = generator->GenerateEntryTrajectory(init_pose, init_dof, 4, quit_traj,
                                           ndof, 3, false);
  if (ret) {
    logfile_ << "Quit Path dist "
             << " - " << generator->pathDist(quit_traj, ndof) << std::endl
             << std::endl;
    single_traj.traj_ = quit_traj;
    single_traj.type_ = UnsafeTransitionTraj;
    out_traj.push_back(single_traj);
  }
  if (ret) {
    mc_data =
        generator->calChainZeroPoint(p1, 0, task.encoder, isIncrease, invert);
    mc_data[1] =
        task.face == 0
            ? -task.diff
            : task.diff; // 前提是机器人正方向和箱体允许方向相同，否则取负号
    std::cout << "task encoder: " << task.encoder << std::endl;
    std::cout << "p1 pos: " << p1[0] << ", " << p1[1] << ", " << p1[2]
              << std::endl;
    std::cout << "mc data: " << mc_data[0] << "," << mc_data[1] << std::endl;
  }

  return ret;
}

std::vector<RobotTask>
convertToRobotTask(const std::vector<TrajectoryInfo> &traj_info) {
  std::vector<RobotTask> rbttasks;
  bool paint_io = false;

  for (int n = 0; n < traj_info.size(); n++) {
    auto traj = traj_info[n];
    if (traj.type_ == SeamPaintTraj || traj.type_ == PlanePaintTraj) {
      if (paint_io == false) {
        RobotTask rbttask;
        // rbttask.track[0].clear();
        //  todo:: 开喷涂任务
        paint_io = true;
      }
    } else {
      if (paint_io == true) {
        RobotTask rbttask;
        // rbttask.track[0].clear();
        //  todo:: 关喷涂任务
        paint_io = false;
      }
    }

    RobotTask rbttask;
    // rbttask.task = VWSRobot::TaskType::MOVEABSJ;
    rbttask.speed[0] = vws::rbtspeed;
    rbttask.speed[1] = vws::rbtspeed;
    AbbRbt(traj.traj_, rbttask);
    rbttasks.push_back(rbttask);
  }
  return rbttasks;
}

std::vector<RobotTask>
convertToRobotTask(const std::vector<TrajectoryInfo> &traj_info,
                   PlanStragety &stragety) {
  std::vector<RobotTask> rbttasks;
  bool paint_io = false;
  double seam_speed, plane_speed, move_speed, speed;
  stragety.speedOf("plane_paint", plane_speed);
  stragety.speedOf("seam_paint", seam_speed);
  stragety.speedOf("move", move_speed);

  for (int n = 0; n < traj_info.size(); n++) {
    auto traj = traj_info[n];
    if (traj.type_ == SeamPaintTraj || traj.type_ == PlanePaintTraj) {
      if (paint_io == false) {
        RobotTask rbttask;
        // rbttask.track[0].clear();
        // todo:: 开喷涂任务
        paint_io = true;
      }
      speed = traj.type_ == SeamPaintTraj ? seam_speed : plane_speed;
    } else {
      if (paint_io == true) {
        RobotTask rbttask;
        // rbttask.track[0].clear();
        // todo:: 关喷涂任务
        paint_io = false;
      }
      speed = move_speed;
    }

    RobotTask rbttask;
    // rbttask.task = VWSRobot::TaskType::MOVEABSJ;
    rbttask.speed[0] = speed;
    rbttask.speed[1] = speed;
    AbbRbt(traj.traj_, rbttask);
    rbttasks.push_back(rbttask);
  }
  return rbttasks;
}

void convertToRobotTask(const std::vector<TrajectoryInfo> &traj_info,
                        PlanStragety &stragety, VWSRobot::TaskType taskType,
                        std::vector<RobotTask> &rbttasks) {
  // std::vector<RobotTask> rbttasks;
  bool paint_io = false;
  double seam_speed, plane_speed, move_speed, speed;
  stragety.speedOf("plane_paint", plane_speed);
  stragety.speedOf("seam_paint", seam_speed);
  stragety.speedOf("move", move_speed);

  for (int n = 0; n < traj_info.size(); n++) {
    auto traj = traj_info[n];
    if (traj.type_ == SeamPaintTraj || traj.type_ == PlanePaintTraj) {
      if (paint_io == false) {
        // todo:: 开喷涂任务
        paint_io = true;
      }
      speed = traj.type_ == SeamPaintTraj ? seam_speed : plane_speed;
    } else {
      if (paint_io == true) {
        // todo:: 关喷涂任务
        paint_io = false;
      }
      speed = move_speed;
    }

    Rbt(traj.traj_, speed, taskType, rbttasks);
  }
}

void clearQueue(std::vector<vws::PlanTaskInfo> &q, int64_t current_encoder,
                int64_t expire_range) {
  int N = q.size();
  std::vector<int> ind;
  for (int i = 0; i < N; i++) {
    if (llabs(q[i].encoder - current_encoder) > expire_range) {
      ind.push_back(i);
    }
  }

  for (int i = ind.size() - 1; i >= 0; i--) {
    q.erase(q.begin() + ind[i]);
  }
}

void TrajectoryProcess::begintraj_Slot(MainProcess *vdata) {
#ifdef PLAN_FAKE_DATA
  fakeData(vdata, vdata->getChainEncoder());
#endif

  TrajectoryGenerator *generator = TrajectoryGenerator::Instance();
  BaseStrategy *planstrategy;
  BaseStrategy *sevenstrategy;
  // auto context = std::make_shared<TrajectoryContext>(generator, planstrategy,
  //                                                    sevenstrategy);
  float valid_range = 6000; // mm
  int64_t current_encoder = vdata->getChainEncoder();
  bool isIncrease = vdata->getChainEncoderDir();
  float units = vdata->getChainUnits();
  int64_t expire_range = valid_range * units;
  int64_t plan_delay = 1200 * units;
  int equal_th = 400;
  auto bottom_task_q = vdata->GetPlanTaskInfo(0);
  auto upper_task_q = vdata->GetPlanTaskInfo(1);
#ifdef PLAN_FAKE_DATA
  int64_t bottom_2_upper = 0 * units;
#else
  int64_t bottom_2_upper = 270 * units;
#endif
  SortedTaskQ taskQ;

  //规划下层
  // taskQ =
  //     PrepareTaskInfoOneLayer(bottom_task_q, upper_task_q, current_encoder,
  //                             bottom_2_upper, 0, isIncrease, units,
  //                             plan_delay);

  upper_task_q->VectorMutex.lock();
  bottom_task_q->VectorMutex.lock();
  taskQ =
      PrepareTaskInfoTwoLayers(upper_task_q, bottom_task_q, current_encoder,
                               bottom_2_upper, isIncrease, units, plan_delay);
  upper_task_q->VectorMutex.unlock();
  bottom_task_q->VectorMutex.unlock();
  // 规划上层
  // PrepareTaskInfoOneLayer(upper_task_q, bottom_task_q, current_encoder, 0,
  //                          bottom_2_upper, isIncrease, units, plan_delay);

  //规划两层
  //  SortedTaskQ taskQ =
  //  PrepareTaskInfoTwoLayers(upper_task_q,bottom_task_q,current_encoder,bottom_2_upper,isIncrease,units,plan_delay);

  // todo:: 清理队列。把超过一定距离的task清除掉
  // clearQueue(*upper_task_q, current_encoder, expire_range);
  // clearQueue(*bottom_task_q, current_encoder, expire_range);

  //*************************************************************************
  //*                           规划任务
  //*       从队列中取出头部数据，根据箱体信息生成场景，然后依次规划路径
  //*************************************************************************
  bool invert = true;
#ifdef PLAN_FAKE_DATA
  auto stragety = stragety2_;
  invert = false;
#else
  auto stragety = stragety1_;
#endif
  while (!taskQ.empty()) {
    // std::cout << "开始规划" << std::endl;
    CLog::getInstance()->log("开始规划");
    CLog::getInstance()->log("当前编码器数值： " +
                             QString::number(current_encoder));
    QString str = "规划，下层队列信息： ";
    upper_task_q->VectorMutex.unlock();
    bottom_task_q->VectorMutex.unlock();
    for (auto &&i : bottom_task_q->getVector()) {
      auto val = i.encoder * vws::chainFactor;
      str += (QString::number(val) + ",");
    }
    CLog::getInstance()->log(str);
    str = "规划，上层队列信息： ";
    for (auto &&i : upper_task_q->getVector()) {
      auto val = i.encoder * vws::chainFactor;
      str += (QString::number(val) + ",");
    }
    CLog::getInstance()->log(str);
    upper_task_q->VectorMutex.unlock();
    bottom_task_q->VectorMutex.unlock();

    auto task_info = taskQ.top().second;
    auto task_priority = taskQ.top().first;

    CLog::getInstance()->log("规划， task_priority : " +
                             QString::number(task_priority));

    CLog::getInstance()->log("规划， task_info size: " +
                             QString::number(task_info.targets_.size()));

    taskQ.pop();
    std::vector<float> mc_data;
    Eigen::VectorXd init_dof;
    init_dof = vdata->getRobotWaitPose();
    // createPlanScene(generator, task_info, units, invert);

    createPlanSceneUsingTactic(generator, task_info, stragety, units);
    createSafeRegionUsingTactic(generator, task_info, stragety, units);

    std::vector<TrajectoryInfo> traj_info;
    std::vector<TrajectoryInfo> safe_traj, unsafe_traj, cancle_traj;
#ifdef PLAN_FAKE_DATA
    bool ret = planTaskUsingTactic(generator, task_info, stragety, safe_traj,
                                   unsafe_traj, cancle_traj, mc_data, units,
                                   isIncrease, 6);
#else
    bool ret = planTaskUsingTactic(generator, task_info, stragety, safe_traj,
                                   unsafe_traj, cancle_traj, mc_data, units,
                                   isIncrease, 6);
    // bool ret = planOneTask(generator, task_info, init_dof, traj_info,
    // mc_data, units,
    //                        isIncrease, invert,
    //                        task_info.targets_.front().face == 0);
#endif
    generator->clearEnv();
    if (!ret) {
      continue;
    }

    if (ret) {
      // std::cout << "规划成功" << std::endl;
      CLog::getInstance()->log("规划成功");
#ifdef PLAN_FAKE_DATA
      auto all_traj = safe_traj;
      for (int i = 0; i < unsafe_traj.size(); i++) {
        all_traj.push_back(unsafe_traj[i]);
      }
      auto rbttasks = convertToRobotTask(all_traj, stragety2_);
#else
      std::vector<RobotTask> rbttasks;
      convertToRobotTask(safe_traj, stragety1_, VWSRobot::TaskType::track_1,
                         rbttasks);

      convertToRobotTask(unsafe_traj, stragety1_, VWSRobot::TaskType::track_2,
                         rbttasks);

      // auto all_traj = safe_traj;
      // for (int i = 0; i < unsafe_traj.size(); i++){
      //   all_traj.push_back(unsafe_traj[i]);
      // }
      //   auto rbttasks = convertToRobotTask(all_traj, stragety);
      // auto rbttasks = convertToRobotTask(traj_info);
#endif
      vdata->SetRobotTaskInfo(mc_data, rbttasks);
    }
  }
}
