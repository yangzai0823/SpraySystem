#include "trajectorygenerator.h"

#include <openrave-core.h>
#include <openrave/openrave.h>

#include <boost/assign.hpp>
#include <boost/foreach.hpp>
#include <boost/format.hpp>
#include <boost/program_options.hpp>
#include <ctime>
#include <list>

#include "osgviewer/osgviewer.hpp"
#include "sco/optimizers.hpp"
#include "trajopt/collision_checker.hpp"
#include "trajopt/collision_terms.hpp"
#include "trajopt/common.hpp"
#include "trajopt/problem_description.hpp"
#include "trajopt/rave_utils.hpp"
#include "utils/clock.hpp"
#include "utils/config.hpp"
#include "utils/eigen_conversions.hpp"
#include "utils/stl_to_string.hpp"
// ompl includes
#include <ompl/base/ConstrainedSpaceInformation.h>
#include <ompl/base/Constraint.h>
#include <ompl/base/DiscreteMotionValidator.h>
#include <ompl/base/OptimizationObjective.h>
#include <ompl/base/objectives/PathLengthOptimizationObjective.h>
#include <ompl/base/spaces/RealVectorStateSpace.h>
#include <ompl/base/spaces/constraint/AtlasStateSpace.h>
#include <ompl/base/spaces/constraint/ConstrainedStateSpace.h>
#include <ompl/base/spaces/constraint/ProjectedStateSpace.h>
#include <ompl/base/spaces/constraint/TangentBundleStateSpace.h>
#include <ompl/geometric/PathGeometric.h>
#include <ompl/geometric/SimpleSetup.h>
#include <ompl/geometric/planners/est/BiEST.h>
#include <ompl/geometric/planners/est/EST.h>
#include <ompl/geometric/planners/est/ProjEST.h>
#include <ompl/geometric/planners/informedtrees/BITstar.h>
#include <ompl/geometric/planners/kpiece/BKPIECE1.h>
#include <ompl/geometric/planners/kpiece/KPIECE1.h>
#include <ompl/geometric/planners/prm/PRM.h>
#include <ompl/geometric/planners/prm/SPARS.h>
#include <ompl/geometric/planners/prm/SPARStwo.h>
#include <ompl/geometric/planners/rrt/RRT.h>
#include <ompl/geometric/planners/rrt/RRTConnect.h>
#include <ompl/geometric/planners/rrt/RRTstar.h>

#include <IK/utils.hpp>
#include <deque>

#include "planner/ompl_planning_class.h"
#include <mutex>
#include "Data/StaticData.h"

namespace po = boost::program_options;
namespace ob = ompl::base;
namespace og = ompl::geometric;
namespace om = ompl::magic;
using namespace trajopt;
using namespace std;
using namespace OpenRAVE;
using namespace util;
using namespace boost::assign;

TrajectoryGenerator *TrajectoryGenerator::_instance = NULL;
std::mutex TrajectoryGenerator::_mutex;

TrajectoryGenerator::TrajectoryGenerator()
{
    robotBeltDirection = Eigen::Vector3d( -0.00321569, -0.419444, -0.001071);

    extraAxisDirection = Eigen::Vector3d( 0, -0.0002153165405616164,0);
    GenerateEnvirInfo();
}

TrajectoryGenerator *TrajectoryGenerator::Instance()
{
    if(_instance==NULL)
    {
        _mutex.lock();
        if(_instance==NULL)
        {
            _instance = new TrajectoryGenerator();
        }
        _mutex.unlock();
    }

     return _instance;
}

TrajectoryGenerator::~TrajectoryGenerator()
{
    delete pt;
}

void TrajectoryGenerator::GenerateEnvirInfo()
{
    //*******************************************************
    // 初始化环境
    // URDFTest pt;
      pt = new VwsPlanEnv(
        "../urdf/mpx2600/urdf/"
        "mpx2600_urdf.urdf",
        "../urdf/mpx2600/urdf/"
        "mpx2600_urdf.srdf");
    auto manip_rot =   Eigen::AngleAxisd(M_PI, Vector3d::UnitZ()) *
                               Eigen::AngleAxisd(-M_PI / 2, Vector3d::UnitY()) *
                               Eigen::AngleAxisd(M_PI / 2.0, Vector3d::UnitX());
    robot = GetRobot(*pt->env);
    //设置工具坐标系
    pt->addManipulator(robot, "tool", "base", "link_flange", manip_rot,
                       Eigen::Vector3d(0.00, 0.1, 0.8));
      //  // 添加墙面
   pt->addBox(Eigen::Vector3d(-1.5, 0, 1), Eigen::Vector3d(0.01, 5, 2),
   Eigen::Quaterniond(1,0,0,0), "wall");
         //  // 添加地面
   pt->addBox(Eigen::Vector3d(0, 0, -1.05), Eigen::Vector3d(5, 5, 0.01),
   Eigen::Quaterniond(1,0,0,0), "floor");
    // 添加滑轨
   pt->addBox(Eigen::Vector3d(0, 0, -1.005), Eigen::Vector3d(0.50, 5, 0.4),
   Eigen::Quaterniond(1,0,0,0), "slider");
       // 添加放碰撞块
   pt->addBox(Eigen::Vector3d(0.45, 0, -0.5), Eigen::Vector3d(0.01, 0.2, 0.2),
   Eigen::Quaterniond(1,0,0,0), "fixer");
}

void TrajectoryGenerator::AddBoxHookEnvirInfo(Eigen::Vector3d boxcenter,
                                          Eigen::Vector3d boxsize,
                                          Eigen::Quaterniond boxq,
                                          double hook_height,
                                          const std::string &name)
{
  AddBoxEnvirInfo(
          boxcenter + Eigen::Vector3d(0, 0, boxsize[2]/2 + hook_height/2),
          Eigen::Vector3d(50, boxsize[1] - 200, hook_height),
          boxq,
          name);
}


void TrajectoryGenerator::AddBoxEnvirInfo(Eigen::Vector3d boxcenter,
                                          Eigen::Vector3d boxsize,
                                          Eigen::Quaterniond boxq,
                                          const std::string &name)
{
  double box1centerx, box1centery, box1centerz;
  double box1height, box1width, box1length;
  double box1q1, box1q2, box1q3, box1q4;
  boxcenter = boxcenter / 1000.0;
  boxsize = boxsize / 1000.0;

  // 规划目标前表面
  boxes_.push_back(pt->addBox(boxcenter, boxsize / 2.0, boxq, name));
  
  //      pt.addBox(Eigen::Vector3d(box2centerx, box2centery, box2centerz),
  //      Eigen::Vector3d(box2length, box2width, box2height),
  //      Eigen::Quaterniond(box2q1,box2q2,box2q3,box2q4), "box2");
  //  // 添加目标上层箱体
  //  pt->addBox(Eigen::Vector3d(box1centerx, box1centery+0.4, box1centerz+1.2),
  //  Eigen::Vector3d(box1length, box1width, box1height),
  //  Eigen::Quaterniond(box1q1,box1q2,box1q3,box1q4), "box2");
  //  // 添加目标前方箱体
  //  pt->addBox(Eigen::Vector3d(box1centerx, box1centery+1.8, box1centerz),
  //  Eigen::Vector3d(box1length, box1width, box1height),
  //  Eigen::Quaterniond(box1q1,box1q2,box1q3,box1q4), "box3");
  //  // 添加目标前方箱体挂钩
  //  pt->addBox(Eigen::Vector3d(box1centerx, box1centery+1.8, box1centerz),
  //  Eigen::Vector3d(0.05, 0.2, box1height+2),
  //  Eigen::Quaterniond(box1q1,box1q2,box1q3,box1q4), "hook");
  //  // 添加墙面
  //  pt->addBox(Eigen::Vector3d(-1, 0, 1), Eigen::Vector3d(0.01, 5, 2),
  //  Eigen::Quaterniond(box1q1,box1q2,box1q3,box1q4), "wall");
}

void TrajectoryGenerator::clearEnv(){
    for(auto &&k : boxes_){
      pt->removeBox(k);
    }
    boxes_.clear();
}


void TrajectoryGenerator::GenerateSeamPaintConstraint(Eigen::Vector3d boxCenterPoint,
                                             Eigen::Vector3d boxSize,
                                             Eigen::Quaterniond boxq,
                                             Eigen::Quaterniond painter_ori,
                                             float weld_y_offset, float shrink_z,
                                             bool front, bool invert,
                                             Eigen::VectorXd &p, Eigen::VectorXd &ori){
      //计算喷涂的路径约束
 shrink_z =
      shrink_z > (boxSize[2] / 2.0) ? boxSize[0] / 2.0 : shrink_z;

  Eigen::Vector3d p1, p2;
  // 生成喷涂路线，这里通过 p1,p2,p3 三个点来控制位置，ori_dn来控制姿态，
  // 实际调试时需要更改
  // 最终需要通过柜体的位姿生成
  boxCenterPoint = boxCenterPoint / 1000.0;
  boxSize = boxSize / 1000.0;
  shrink_z /= 1000.0;
  weld_y_offset /= 1000.0;
  p1 = topnearpoint(boxCenterPoint, boxSize, invert);
  p1[1] -= weld_y_offset;
  p1[2] -= shrink_z;
  p2 = bottomnearpont(boxCenterPoint, boxSize, invert);
  p2[1] -= weld_y_offset;
  p2[2] += shrink_z;

  // 喷涂分为两种构型，参考构型为front = true/false， invert = false/true，
  // 当两个相同时，为另一种构型，需要对y进行偏移，对姿态进行调整（绕世界坐标系x轴旋转180度）
  if(!(front)){
    p1[1] = p1[1] - boxSize[1] + 2 * weld_y_offset;
    p2[1] = p2[1] - boxSize[1] + 2 * weld_y_offset;
  }
  p1 = boxq * (p1 - boxCenterPoint) + boxCenterPoint;
  p2 = boxq * (p2 - boxCenterPoint) + boxCenterPoint;
  // if(!(front ^ invert)){
  //   painter_ori =
  //        painter_ori* Eigen::AngleAxisd(M_PI/2.0, Eigen::Vector3d::UnitY()) 
  //        * Eigen::AngleAxisd(M_PI, Eigen::Vector3d::UnitX()) ;
  // }
  Eigen::Quaterniond quat(painter_ori);
  quat = boxq * quat;
  Eigen::Vector4d ori_dn(quat.w(), quat.x(), quat.y(), quat.z());

  
  // genInitPathForRect(p1, p2, p3, dn, p, dir);
  // genInitOrientedPathForRect(p1, p2, p3, ori_dn, p, ori);
  Eigen::Isometry3d tf = Eigen::Isometry3d::Identity();
  tf.rotate(Eigen::AngleAxisd(-M_PI / 4.0, Eigen::Vector3d::UnitY()));
  genInitOrientedPathForLine(p1, p2, ori_dn,
                             tf,  // 0: x, 1: y
                             0.2, 0.15, p, ori);
  p = p * 1000.0;
}



void TrajectoryGenerator::GenerateShrinkedPaintConstraint(Eigen::Vector3d boxCenterPoint,
                                             Eigen::Vector3d boxSize,
                                             Eigen::Quaterniond boxq,
                                             Eigen::Quaterniond painter_ori,
                                             float shrink_size_x, float shrink_size_z,
                                             bool front, bool invert,
                                             int paint_dir, // 0: x, 1: z
                                             Eigen::VectorXd &p, Eigen::VectorXd &ori){
      //计算喷涂的路径约束

  Eigen::Vector3d p1, p2, p3;
  shrink_size_x =
      shrink_size_x > (boxSize[0] / 2.0) ? boxSize[0] / 2.0 : shrink_size_x;
  shrink_size_z =
      shrink_size_z > (boxSize[2] / 2.0) ? boxSize[2] / 2.0 : shrink_size_z;

  // 生成喷涂路线，这里通过 p1,p2,p3 三个点来控制位置，ori_dn来控制姿态，
  // 实际调试时需要更改
  // 最终需要通过柜体的位姿生成
  boxCenterPoint = boxCenterPoint / 1000.0;
  boxSize = boxSize / 1000.0;
  shrink_size_x /= 1000.0;
  shrink_size_z /= 1000.0;
  std::cout << "shrink x: " << shrink_size_x << std::endl;
  std::cout << "shrink z: " << shrink_size_z << std::endl;
  p1 = topfarpoint(boxCenterPoint, boxSize, invert);
  std::cout <<"p1 : " << p1.transpose() << std::endl;
  p2 = topnearpoint(boxCenterPoint, boxSize, invert);  
  std::cout <<"p2 : " << p2.transpose() << std::endl;
  p3 = bottomnearpont(boxCenterPoint, boxSize, invert);
  std::cout <<"p3 : " << p3.transpose() << std::endl;
  if(invert == false){
    p1[0] -= shrink_size_x;
    p1[2] -= shrink_size_z;
    p2[0] += shrink_size_x;
    p2[2] -= shrink_size_z;
    p3[0] += shrink_size_x;
    p3[2] += shrink_size_z;
  }else{
    p1[0] += shrink_size_x;
    p1[2] -= shrink_size_z;
    p2[0] -= shrink_size_x;
    p2[2] -= shrink_size_z;
    p3[0] -= shrink_size_x;
    p3[2] += shrink_size_z;
  }


  Eigen::Quaterniond box2world = boxq.inverse();
  p1 = boxq * (p1 - boxCenterPoint) + boxCenterPoint;
  p2 = boxq * (p2 - boxCenterPoint) + boxCenterPoint;
  p3 = boxq * (p3 - boxCenterPoint) + boxCenterPoint;
  std::cout <<"p1 : " << p1.transpose() << std::endl;
  std::cout <<"p2 : " << p2.transpose() << std::endl;
  std::cout <<"p3 : " << p3.transpose() << std::endl;

  // 喷涂分为两种构型，参考构型为front = true/false， invert = false/true，
  // 当两个相同时，为另一种构型，需要对y进行偏移，对姿态进行调整（绕世界坐标系x轴旋转180度）
  Eigen::Vector3d dy = Eigen::Vector3d(0, boxSize[1], 0);
  dy = boxq * dy;
  std::cout <<"dy : " << dy << std::endl;
  if (!(front)) {
    // p1[1] = p1[1] - boxSize[1];
    // p2[1] = p2[1] - boxSize[1];
    // p3[1] = p3[1] - boxSize[1];
    p1 = p1 - dy;
    p2 = p2 - dy;
    p3 = p3 - dy;
    // painter_ori =
    //     Eigen::AngleAxisd(M_PI, Eigen::Vector3d::UnitX()) * painter_ori;
  }
  Eigen::Quaterniond quat(painter_ori);
  quat = boxq * quat;
  Eigen::Vector4d ori_dn(quat.w(), quat.x(), quat.y(), quat.z());

  
  // genInitPathForRect(p1, p2, p3, dn, p, dir);
  // genInitOrientedPathForRect(p1, p2, p3, ori_dn, p, ori);
  genInitOrientedPathForBoxPlane(p1, p2, p3, ori_dn,
                                 paint_dir,  // 0: x, 1: y
                                 0.05, 0.15, 0.1, 0.1, 1, p, ori);
  p = p * 1000.0;
}


void TrajectoryGenerator::GeneratePaintConstraint(Eigen::Vector3d boxCenterPoint,
                                             Eigen::Vector3d boxSize,
                                             Eigen::Quaterniond boxq,
                                             Eigen::Quaterniond painter_ori,
                                             bool front, bool invert,
                                             Eigen::VectorXd &p, Eigen::VectorXd &ori){
    //计算喷涂的路径约束

  Eigen::Vector3d p1, p2, p3;

  // 生成喷涂路线，这里通过 p1,p2,p3 三个点来控制位置，ori_dn来控制姿态，
  // 实际调试时需要更改
  // 最终需要通过柜体的位姿生成
  //      Eigen::Vector3d c(1.0, -0.7, 0.6);
  //      double dx = 0.4, dy = 0.6, dz = 0.4;
  //      p1 = c + Eigen::Vector3d(dx, dy, dz);
  //      p2 = c + Eigen::Vector3d(-dx, dy, dz);
  //      p3 = c + Eigen::Vector3d(-dx, dy, -dz);
  boxCenterPoint = boxCenterPoint / 1000.0;
  boxSize = boxSize / 1000.0;
  p1 = topfarpoint(boxCenterPoint, boxSize, invert) ;
  p2 = topnearpoint(boxCenterPoint, boxSize, invert);
  p3 = bottomnearpont(boxCenterPoint, boxSize, invert);
  Eigen::Quaterniond box2world = boxq.inverse();
  p1 = boxq * (p1 - boxCenterPoint) + boxCenterPoint;
  p2 = boxq * (p2 - boxCenterPoint) + boxCenterPoint;
  p3 = boxq * (p3 - boxCenterPoint) + boxCenterPoint;

  // 喷涂分为两种构型，参考构型为front = true/false， invert = false/true，
  // 当两个相同时，为另一种构型，需要对y进行偏移，对姿态进行调整（绕世界坐标系x轴旋转180度）
  Eigen::Vector3d dy = Eigen::Vector3d(0, boxSize[1], 0);
  dy = boxq * dy;
  if(!(front)){
    p1 = p1 - dy;
    p2 = p2 - dy;
    p3 = p3 - dy;
    // painter_ori =
    //     Eigen::AngleAxisd(M_PI, Eigen::Vector3d::UnitX()) * painter_ori;
  }
  Eigen::Quaterniond quat(painter_ori);
  quat = boxq * quat;
  Eigen::Vector4d ori_dn(quat.w(), quat.x(), quat.y(), quat.z());

  
  // genInitPathForRect(p1, p2, p3, dn, p, dir);
  // genInitOrientedPathForRect(p1, p2, p3, ori_dn, p, ori);
  genInitOrientedPathForBoxPlane(p1, p2, p3, ori_dn,
                                 0,  // 0: x, 1: y
                                 0.2, 0.15, 0.1, 0.1, 1, p, ori);
  p = p * 1000.0;
}

float TrajectoryGenerator::pathDist(const Eigen::VectorXd &traj, int ndof){
  Eigen::VectorXd diff = traj.block(0, 0, traj.size() - ndof, 1) -
                             traj.block(ndof, 0, traj.size() - ndof, 1);
  float dist = diff.lpNorm<1>();
  return dist;
}


bool TrajectoryGenerator::GenerateEntryTrajectory(
    Eigen::VectorXd start, Eigen::VectorXd end, int nsteps, Eigen::VectorXd &entry_traj, 
    int ndof, int plan_total_num, bool vis){
    int plan_no = 0;
    std::vector<Eigen::VectorXd> trajs;
    float min_dist = 1E10;
    Eigen::VectorXd goal;

    int collision_cnt, cnst_voil_cnt;
    double elapsed_time = 0;
    int try_no = 0;
    bool init_loop = true;
    while (try_no < 5) {
      if(plan_no == plan_total_num){
        break;
      }
      plan_no++;
      auto t1 = clock();

      Eigen::VectorXd init_pos(nsteps * ndof);
      init_pos = Eigen::VectorXd::Zero(nsteps * ndof);
      Eigen::VectorXd ddof = (end - start) / (nsteps - 1);
      for (int i = 0; i < nsteps; i++) {
        for (int n = 0; n < ndof; n++){
          init_pos[ndof * i + n] = start[n] + i * ddof[n];
        }
      }

      // pt.viewer->Idle();
      std::cout << "stage 1" << std::endl;
      ProblemConstructionInfo pci(pt->env);

      if (init_loop){
        init_loop = false;

        // step 2.0: 生成优化后的路径

        auto free_traj = planFreePathJoint(pt->env, "tool", end, nsteps, 5, 0.1, init_pos, collision_cnt,
                    cnst_voil_cnt, elapsed_time, vis);
        // for (int i = 1; i < 10; i ++){
        //   free_traj = planFreePathJoint(pt.env, "tool", goal, nsteps, i*10, 0.2, free_traj, collision_cnt,
        //             cnst_voil_cnt, elapsed_time, false);
        // }
        free_traj = planFreePathJoint(pt->env, "tool", end, nsteps, 100, 0.1, free_traj, collision_cnt,
                    cnst_voil_cnt, elapsed_time, false);
        
        float dist =pathDist(free_traj, ndof);
        if(cnst_voil_cnt == 0 && collision_cnt ==0){
          if(min_dist > dist){
            min_dist = dist;
            trajs.push_back(free_traj);
          }
        }
      }


  #if 1
      
      bool valid = false;
      // step 2：取喷涂轨迹的两个端点，生成进入和退出轨迹
      // step 2.1：使用ompl生成初始点位
      std::vector<double> j1, j2;
      for (int i = 0; i < 6; i++){
        j1.push_back(start[i]);
        j2.push_back(end[i]);
      }


      // 重试最多5次，找到最优随机解
      auto traj = planFreePathWithOMPL(robot, pt->env, "tool", j1, j2,1);
      auto interp_traj = jointInterplot(traj, 30 / 180.0 * M_PI, ndof);
      int retry_cnt = 0;
      int opt_joint_steps = interp_traj.size();
      auto opt_traj = traj;
      while (interp_traj.size() / ndof > 20 && retry_cnt < 5) {
        traj = planFreePathWithOMPL(robot, pt->env, "tool", j1, j2, 1);
        interp_traj = jointInterplot(traj, 30 / 180.0 * M_PI, ndof);
        retry_cnt++;
        int joint_steps = interp_traj.size();
        if(joint_steps < opt_joint_steps){
          opt_joint_steps = joint_steps;
          opt_traj = traj;
        }
      }
      traj = opt_traj;
      int N = traj.size() * traj[0].size();
      Eigen::VectorXd init_pos_ompl;
      init_pos_ompl.resize(N);
      int current_n = 0;
      for (int i = 0; i < traj.size(); i++){
        for (int n = 0; n < traj[0].size(); n++){
          init_pos_ompl[current_n++] = traj[i][n];
        }
      }
      // step 2.2: 生成优化后的路径

      auto free_traj = planFreePathJoint(pt->env, "tool", end, nsteps, 5, 0.1, init_pos_ompl, collision_cnt,
                  cnst_voil_cnt, elapsed_time, false);
      // for (int i = 1; i < 10; i ++){
      //   free_traj = planFreePathJoint(pt.env, "tool", goal, nsteps, i*10, 0.2, free_traj, collision_cnt,
      //             cnst_voil_cnt, elapsed_time, false);
      // }
      free_traj = planFreePathJoint(pt->env, "tool", end, nsteps, 100, 0.1, free_traj, collision_cnt,
                  cnst_voil_cnt, elapsed_time, vis);
      
      float dist =pathDist(free_traj, ndof);
      if(cnst_voil_cnt == 0 && collision_cnt ==0){
        if(min_dist > dist){
          min_dist = dist;
          trajs.push_back(free_traj);
        }
      }else{
        plan_no--;
        try_no++;
      }
      auto t2 = clock();

#endif
    }

    int cnt = 0;
    for (auto&& traj : trajs) {
      float dist = pathDist(traj, 6);
      if(abs(dist - min_dist) < 1e-6){
        entry_traj = traj;
        return true;
      }
    }
    return false;
}

bool TrajectoryGenerator::GeneratePaintTrajectory(Eigen::VectorXd init_dof,
                                                  Eigen::VectorXd p,
                                                  Eigen::VectorXd ori,
                                                  Eigen::VectorXd &traj,
                                                  int ndof)
{
  Eigen::VectorXd goal;
  int nsteps;
  int collision_cnt, cnst_voil_cnt;
  double elapsed_time = 0;

  auto t1 = clock();
  p = p / 1000.0;
  nsteps = p.size() / 3;
  Eigen::VectorXd init_pos(nsteps * ndof);
  init_pos = Eigen::VectorXd::Zero(nsteps * ndof);
  for (int i = 0; i < nsteps; i++) {
    for (int n = 0; n < ndof; n++) {
      init_pos[ndof * i + n] = init_dof[n];
    }
  }

  // pt.viewer->Idle();
  std::cout << "stage 1" << std::endl;
  ProblemConstructionInfo pci(pt->env);

#if 1

  bool valid = false;

  //************************************************************************
  // step 1: 生成喷涂轨迹
  // init_pos =
  //     planWeldPathFixed(pt.env, "toolB_6", ik, p, dir, 0, 0.1, init_pos,
  //                       collision_cnt, cnst_voil_cnt, elapsed_time, true);
  // init_pos =
  //     planWeldPathFixed(pt.env, "toolB_6", ik, p, dir, 100, 0.1, init_pos,
  //                       collision_cnt, cnst_voil_cnt, elapsed_time, true);
  std::vector<double> dofvalues;
  auto init_pos2 =
      planOrientedPathFixed(pt->env, "tool", p, ori, 0, 0.1, init_pos,
                            collision_cnt, cnst_voil_cnt, elapsed_time, false);
  traj =
      planOrientedPathFixed(pt->env, "tool", p, ori, 100, 0.1, init_pos2,
                            collision_cnt, cnst_voil_cnt, elapsed_time, false);
  std::cout << "voil_cnt " << cnst_voil_cnt << ", collision cnt "
            << collision_cnt << std::endl;
  if (collision_cnt ||
      !checkPath(robot, "tool", traj, p, ori, 0.010, 5.0 / 180.0 * M_PI)) {
    return false;
  } else {
    return true;
  }
  


#endif
}

VWSRobot::RobotTask TrajectoryGenerator::GenerateSprayTask()
{
    //    std::vector<VWSRobot::RobotTask> tasks;
    VWSRobot::RobotTask tk;
    // tk.task = VWSRobot::TaskType::MOVEABSJ;  // 1:movel, 2:movej, 3:moveabsj
    tk.speed[0] = 500;
    tk.speed[1] = 10;
    int cnt = 0;
    std::cout << planRet.transpose() << std::endl;
    for(int i = 0; i < planRet.size() / 6; i++){
        std::cout << i << ": ";
        std::array<float,7> jv;
        for(int n = 0; n < 6; n++){
            jv[n] = planRet[cnt++] / M_PI * 180.0;
            // IRB 1410机器人3轴和2轴耦合，因此需要添加下面的补偿
            if(n == 2){
                jv[n] += jv[n-1];
            }
            std::cout << jv[n] << ", ";
        }
        std::cout << std::endl;
        // tk.track.push_back(jv);  //TODO： 机器人数据结构已修改，需要调整
    }
//    //        tasks.push_back(tk);
    return tk;
}
std::vector<float> TrajectoryGenerator::calChainZeroPoint(Eigen::Vector3d p1,
  float follow_offset, int64_t encoder, bool is_increase, bool invert){

    // 编码器系数，单位：mm/unit
    // chainFactor = 0.4198727819755431f;
    chainFactor = vws::chainFactor;
//    std::cout<<"系数： "<<chainFactor<<std::endl;

    //1. 相机坐标系下箱子中心坐标转机器人坐标系
    //auto rbtBoxCenter = handEyeMatrix*param.boxCenterPoint;
    //2. 计算箱子左下角（靠近机器人方向）顶点（P1）坐标
    //3. 计算（p1）距离机器人原点（p2）距离 l2
    float offsetOfTraj = 0;
    // if (front ^ invert) {
    //   // 构型1, 机器人坐标系下，跟踪位置向y正方向偏移
    //   offsetOfTraj = offsetOfTraj;
    // } else {
    //   offsetOfTraj = -offsetOfTraj;
    // }
    // Eigen::Vector3d robotZeroPoint = extraAxisDirection*sevenEncoder;
    double conveyor_dest = invert ? follow_offset : -follow_offset;

    double diffL = conveyor_dest - p1[1];
    //5. diffL转悬挂链移动距离， e1
    double e1 = abs(diffL) / chainFactor;
    //6. 悬挂链0点编码器数值
    double ret = is_increase ? e1+encoder : encoder - e1;   //注意机器人正方向与编码器正方向是否相同

    std::vector<float> result;
    result.push_back(ret);
    result.push_back(offsetOfTraj);
//    result.push_back(0);
    return result;
}

std::vector<float> TrajectoryGenerator::calChainZeroPoint(TrajParam param, bool invert)
{

    // chainFactor = 0.4198727819755431f;
    chainFactor = vws::chainFactor;
//    std::cout<<"系数： "<<chainFactor<<std::endl;

    //1. 相机坐标系下箱子中心坐标转机器人坐标系
    //auto rbtBoxCenter = handEyeMatrix*param.boxCenterPoint;
    //2. 计算箱子左下角（靠近机器人方向）顶点（P1）坐标
    auto p1= bottomnearpont(param.boxCenterPoint, param.boxSize, invert);
    //3. 计算（p1）距离机器人原点（p2）距离 l2
    Eigen::Vector3d robotZeroPoint = extraAxisDirection*param.sevenEncoder;
    double l2 = robotZeroPoint[1] - p1[1];
    //4. 计算箱子拍摄位到起喷点距离(diffL)，l2-跟踪偏移， mm
    double diffL = l2 - param.offsetOfTraj;
    //5. diffL转悬挂链移动距离， e1
    double e1 = diffL / chainFactor;
    //6. 悬挂链0点编码器数值
    double ret = -e1+param.encoder;   //注意机器人正方向与编码器正方向是否相同

    std::vector<float> result;
    result.push_back(ret);
    result.push_back(param.offsetOfTraj);
//    result.push_back(0);
    return result;
}

Eigen::Vector3d TrajectoryGenerator::topfarpoint(Eigen::Vector3d boxCenterPoint,Eigen::Vector3d boxSize, bool invert)
{
  if(invert == false){
    double x= boxCenterPoint[0] + boxSize[0]/2;
    double y =boxCenterPoint[1] + boxSize[1]/2;
    double z= boxCenterPoint[2] + boxSize[2]/2;
    Eigen::Vector3d p1(x,y,z);

    return p1;    
  }else{
    double x= boxCenterPoint[0] - boxSize[0]/2;
    double y =boxCenterPoint[1] + boxSize[1]/2;
    double z= boxCenterPoint[2] + boxSize[2]/2;
    Eigen::Vector3d p1(x,y,z);

    return p1;   
  }

}

Eigen::Vector3d TrajectoryGenerator::topnearpoint(Eigen::Vector3d boxCenterPoint,Eigen::Vector3d boxSize, bool invert)
{
  if(invert == false){
    double x= boxCenterPoint[0] - boxSize[0]/2;
    double y =boxCenterPoint[1] + boxSize[1]/2;
    double z= boxCenterPoint[2] + boxSize[2]/2;
    Eigen::Vector3d p1(x,y,z);

    return p1;    
  }else{
    double x= boxCenterPoint[0] + boxSize[0]/2;
    double y =boxCenterPoint[1] + boxSize[1]/2;
    double z= boxCenterPoint[2] + boxSize[2]/2;
    Eigen::Vector3d p1(x,y,z);

    return p1;    
  }

}

Eigen::Vector3d TrajectoryGenerator::bottomnearpont(Eigen::Vector3d boxCenterPoint,Eigen::Vector3d boxSize, bool invert)
{
  if(invert == false){
    double x= boxCenterPoint[0] - boxSize[0]/2;
    double y =boxCenterPoint[1] + boxSize[1]/2;
    double z= boxCenterPoint[2] - boxSize[2]/2;
    Eigen::Vector3d p1(x,y,z);

    return p1;    
  }else{
    double x= boxCenterPoint[0] + boxSize[0]/2;
    double y =boxCenterPoint[1] + boxSize[1]/2;
    double z= boxCenterPoint[2] - boxSize[2]/2;
    Eigen::Vector3d p1(x,y,z);

    return p1;  
  }

}
