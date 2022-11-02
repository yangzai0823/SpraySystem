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
#include "planner/planning_utils.h"
#include "planner/utils.h"
namespace po = boost::program_options;
namespace ob = ompl::base;
namespace og = ompl::geometric;
namespace om = ompl::magic;

using namespace trajopt;
using namespace std;
using namespace OpenRAVE;
using namespace util;
using namespace boost::assign;

namespace {
bool plotting = true, verbose = false;

#if 0
OR::Transform randomReachablePose(RobotAndDOF& rad, KinBody::LinkPtr link) {
  // todo: save & restore
  DblVec dofvals = rad.RandomDOFValues();
  rad.SetDOFValues(dofvals);
  return link->GetTransform();
}
#endif

}  // namespace

Json::Value readJsonFile(const std::string& fname) {
  Json::Value root;
  Json::Reader reader;
  std::ifstream fh(fname.c_str());
  bool parse_success = reader.parse(fh, root);
  if (!parse_success) throw std::runtime_error("failed to parse " + fname);
  return root;
}
#define ASSERT_TRUE(x) assert((x))
#define ASSERT_EQ(x, y) assert(x == y)

#include <urdf2or/urdf_loader.h>
class URDFTest {
 public:
  EnvironmentBasePtr env;
  OSGViewerPtr viewer;
  URDFTest() {
    std::cout << "step 1" << std::endl;
    SetUpTestCase();

    SetUp();
    std::cout << "step 3" << std::endl;
  }
  ~URDFTest() { TearDownTestCase(); }
  /**
   * @brief 为机器人添加执行器信息
   *
   * @param robot 机器人
   * @param name  执行器名称
   * @param baselink    执行器规划起始link名称
   * @param ee_link     执行器规划末端link名称
   * @param q           相对于ee_link的姿态
   * @param t           相对于ee_link的偏移
   */
  void addManipulator(RobotBasePtr robot, const std::string& name,
                      const std::string& baselink, const std::string& ee_link,
                      const Eigen::Quaterniond& q, const Eigen::Vector3d& t) {
    OpenRAVE::RobotBase::ManipulatorInfo info;
    info._name = name;
    info._sBaseLinkName = baselink;
    info._sEffectorLinkName = ee_link;
    info._tLocalTool = OR::Transform(OR::Vector(q.w(), q.x(), q.y(), q.z()),
                                     OR::Vector(t[0], t[1], t[2]));
    robot->AddManipulator(info);
  }

  void SetUp() {
    RAVELOG_DEBUG("SetUp\n");
    RobotBasePtr robot = GetRobot(*env);
    robot->SetDOFValues(DblVec(robot->GetDOF(), 0));
    Transform I;
    I.identity();
    robot->SetTransform(I);
    Eigen::Quaterniond rot = Eigen::AngleAxisd(M_PI, Vector3d::UnitZ()) *
                             Eigen::AngleAxisd(-M_PI / 2, Vector3d::UnitY()) *
                             Eigen::AngleAxisd(M_PI / 2.0, Vector3d::UnitX());
    addManipulator(robot, "tool", "base", "link_flange", rot,
                   Eigen::Vector3d(0.00, 0.13651, 0.5));
    // addManipulator(robot, "toolA_6", "baseA", "linkA_6",
    //                Eigen::Quaterniond(1, 0, 0, 0), Eigen::Vector3d(0, 0,
    //                0.2));
    // addManipulator(robot, "toolB_6", "baseB", "linkB_flange",
    //                Eigen::Quaterniond(1, 0, 0, 0), Eigen::Vector3d(0, 0,
    //                0.2));
    // addManipulator(robot, "toolA_7", "world", "linkA_6",
    //                Eigen::Quaterniond(1, 0, 0, 0), Eigen::Vector3d(0, 0,
    //                0.2));
    // addManipulator(robot, "toolB_7", "world", "linkB_flange",
    //                Eigen::Quaterniond(1, 0, 0, 0), Eigen::Vector3d(0, 0,
    //                0.2));
    OpenRAVE::RobotBase::ManipulatorInfo info;
    // robot->SetActiveManipulator("toolB_6");
    // auto flange = robot->GetLink("linkB_flange");
    robot->SetActiveManipulator("tool");
    auto flange = robot->GetLink("link_flange");
    auto tf = flange->GetTransform();
    auto mnips = robot->GetManipulators();
    for (auto&& m : mnips) {
      std::cout << "Manip: " << m->GetName() << std::endl;
    }
    auto links = robot->GetLinks();
    for (int i = 0; i < links.size(); i++) {
      std::cout << links[i]->GetName() << ": " << links[i]->GetIndex()
                << std::endl;
    }
  }

  void SetUpTestCase() {
    RAVELOG_DEBUG("SetupTestCase\n");
    util::gLogLevel = util::LevelError;
    RaveInitialize(false, verbose ? Level_Debug : Level_Info);
    std::cout << "step 2" << std::endl;
    env = RaveCreateEnvironment();
    std::cout << "step 4" << std::endl;
    env->StopSimulation();
    std::cout << "step 6" << std::endl;
    or_urdf::URDFLoader loader(env);
    std::stringstream ss;
    // ss << "/home/dq/workspace/weld_station_urdf/urdf/one.urdf" << std::endl
    //    << "/home/dq/workspace/weld_station_urdf/urdf/weld_station_urdf.srdf"
    //    << std::endl;
    ss << "/home/dq/workspace/trajopt_test/abb_irb1410_urdf/urdf/abb_irb1410_urdf.urdf"
       << std::endl
       << "/home/dq/workspace/trajopt_test/abb_irb1410_urdf/urdf/abb_irb1410_urdf.srdf"
       << std::endl;
    loader.loadURI(std::cout, ss);
    std::cout << "step 8" << std::endl;
    // env->Load("a.urdf");
    // env->Load(string(DATA_DIR) + "/workpiece.xml");
    std::cout << "step 10" << std::endl;
    Eigen::Isometry3d box_tf = Eigen::Isometry3d::Identity();
    box_tf.translate(Eigen::Vector3d(1.1, -0.7, 1.0));
    std::vector<OpenRAVE::KinBody::GeometryInfoConstPtr> infos;
    auto info = createBoxInfo(Eigen::Vector3d(0.4, 0.6, 0.4), box_tf, "box");
    box_tf.translate(Eigen::Vector3d(0, 1.2 + 0.75, 0));
    auto info2 = createBoxInfo(Eigen::Vector3d(0.4, 0.6, 0.4), box_tf, "box");
    infos.push_back(info);
    infos.push_back(info2);
    createKinBodyFromInfos(env, infos, "box");

    // TopoDS_Shape shape =
    //     readStepTopologies("/home/dq/workspace/trajopt_test/back_board.STEP");
    // auto kbody = createKinBodyFromOCCT(env, shape, "workpiece_shape");
    // kbody->SetTransform(
    //     OR::Transform(OR::Vector(1, 0, 0, 0), OR::Vector(0, 0, 0.5)));
    if (plotting) {
      std::cout << "step 11" << std::endl;
      viewer.reset(new OSGViewer(env));
      std::cout << "step 12" << std::endl;
      viewer->UpdateSceneData();
      std::cout << "step 13" << std::endl;
      viewer->PlotAxes(OpenRAVE::Transform(OR::Vector(1., 0., 0., 0.),
                                           OR::Vector(0., 0., 0.)),
                       1.0);
      std::cout << "step 14" << std::endl;
      env->AddViewer(viewer);
      std::cout << "step 15" << std::endl;
    }
  }

  void TearDownTestCase() {
    RAVELOG_DEBUG("TearDownTestCase\n");
    viewer.reset();
    env.reset();

    RaveDestroy();
  }
};

void genInitOrientedPathForRect(const Eigen::Vector3d& p1,
                                const Eigen::Vector3d& p2,
                                const Eigen::Vector3d& p3,
                                const Eigen::Vector4d& dn, Eigen::VectorXd& p,
                                Eigen::VectorXd& ori) {
  int step_h = 8, step_v = 8;
  Eigen::Vector3d vh = p2 - p1;
  Eigen::Vector3d vv = p3 - p2;
  double lv = vv.norm() / (step_v - 1);
  double lh = vh.norm() / (step_h - 1);
  vv.normalize();
  vh.normalize();
  int len = step_v * step_h;
  p.resize(len * 3);
  ori.resize(len * 4);

  for (int i = 0; i < step_v; i++) {
    for (int c = 0; c < step_h; c++) {
      p.block(3 * ((i * step_h) + c), 0, 3, 1) =
          p1 + (vv * i * lv) + vh * c * lh;
      ori.block(4 * ((i * step_h) + c), 0, 4, 1) = dn;
    }
  }
}

/**
 * @brief
 *
 * @param p1 上层远点
 * @param p2 上层近点
 * @param p3 下层近点
 * @param dn 工具喷涂姿态
 * @param weld_dist   焊缝距离p2的位置
 * @param tstep_dist  转弯步长
 * @param vstep_dist  水平方向规划的步长
 * @param hstep_dist  竖直方向规划的步长
 * @param p  输出，位置
 * @param ori 输出，姿态
 */
void genInitOrientedPathForRect(const Eigen::Vector3d& p1,
                                const Eigen::Vector3d& p2,
                                const Eigen::Vector3d& p3,
                                const Eigen::Vector4d& dn,
                                const Eigen::Vector4d& dn_weld,
                                int main_paint_dir,  // 0: x, 1: y
                                const Eigen::Vector3d& weld_dist,
                                double tstep_dist, double paint_dist,
                                double vstep_dist, double hstep_dist,
                                Eigen::VectorXd& p, Eigen::VectorXd& ori) {
  Eigen::Vector3d vh = p2 - p1;
  Eigen::Vector3d vv = p3 - p2;
  double dy = vh.norm();
  double dx = vv.norm();
  vv.normalize();
  vh.normalize();
  Eigen::Quaterniond rot(dn[0], dn[1], dn[2], dn[3]);
  Eigen::Quaterniond rot_weld(dn_weld[0], dn_weld[1], dn_weld[2], dn_weld[3]);
  Eigen::Vector3d main_step_dist, turn_step_dist, start_pos, start_pos_weld,
      weld_step_dist;
  int main_steps, turn_steps, weld_steps;

  // 计算转折的次数
  int Nturn = 0;
  if (main_paint_dir == 0) {
    // 沿x方向喷涂，转折方向为y方向
    Nturn = dy / tstep_dist;
    Nturn = (Nturn + 2) & 0xFFFFFFFE;
    tstep_dist = dy / Nturn;
    turn_steps = tstep_dist / hstep_dist + 1;
    main_steps = dx / hstep_dist + 1;
    main_step_dist = -(dx / main_steps) * vh;
    turn_step_dist = (tstep_dist / turn_steps) * vv;
    start_pos = p2;
    start_pos_weld = p3;
    weld_steps = dy / hstep_dist + 1;
    weld_step_dist = -(dy / weld_steps) * vv;
  } else {
    Nturn = dx / tstep_dist;
    Nturn = (Nturn + 2) & 0xFFFFFFFE;
    tstep_dist = dx / Nturn;
    turn_steps = tstep_dist / vstep_dist + 1;
    main_steps = dy / vstep_dist + 1;
    main_step_dist = (dy / main_steps) * vv;
    turn_step_dist = (tstep_dist / turn_steps) * vh;
    start_pos = p1;
    start_pos_weld = p2;
    weld_steps = dy / hstep_dist + 1;
    weld_step_dist = (dy / weld_steps) * vv;
  }

  p.resize(3 * (1 + main_steps + (Nturn - 1) * (turn_steps + main_steps) +
                weld_steps + 1));
  ori.resize(4 * (1 + main_steps + (Nturn - 1) * (turn_steps + main_steps) +
                  weld_steps + 1));
  auto current_pos = start_pos;
  current_pos = current_pos - rot.matrix().block(0, 2, 3, 1) * paint_dist;
  std::cout << current_pos.transpose() << std::endl;
  p.block(0, 0, 3, 1) = current_pos;
  ori.block(0, 0, 4, 1) = dn;

  int p_index = 3;
  int ori_index = 4;
  // 计算喷涂面的轨迹
  for (int m = 1; m <= main_steps; m++, p_index += 3, ori_index += 4) {
    current_pos = current_pos + main_step_dist;
    std::cout << current_pos.transpose() << std::endl;
    p.block(p_index, 0, 3, 1) = current_pos;
    ori.block(ori_index, 0, 4, 1) = dn;
  }
  for (int i = 1; i < Nturn; i++) {
    for (int t = 1; t <= turn_steps; t++, p_index += 3, ori_index += 4) {
      current_pos = current_pos + turn_step_dist;
      std::cout << current_pos.transpose() << std::endl;
      p.block(p_index, 0, 3, 1) = current_pos;
      ori.block(ori_index, 0, 4, 1) = dn;
    }
    main_step_dist = -main_step_dist;
    for (int m = 0; m < main_steps; m++, p_index += 3, ori_index += 4) {
      current_pos = current_pos + main_step_dist;
      std::cout << current_pos.transpose() << std::endl;
      p.block(p_index, 0, 3, 1) = current_pos;
      ori.block(ori_index, 0, 4, 1) = dn;
    }
  }

  // 焊缝的路径计算
  current_pos = start_pos_weld;
  current_pos = current_pos - rot_weld.matrix().block(0, 2, 3, 1) * paint_dist;
  p.block(p_index, 0, 3, 1) = current_pos;
  ori.block(ori_index, 0, 4, 1) = dn;
  p_index += 3;
  ori_index += 4;
  for (int m = 1; m <= weld_steps; m++, p_index += 3, ori_index += 4) {
    current_pos = current_pos + weld_step_dist;
    p.block(p_index, 0, 3, 1) = current_pos;
    ori.block(ori_index, 0, 4, 1) = dn_weld;
  }
}

void genInitPathForRect(const Eigen::Vector3d& p1, const Eigen::Vector3d& p2,
                        const Eigen::Vector3d& p3, const Eigen::Vector3d& dn,
                        Eigen::VectorXd& p, Eigen::VectorXd& dir) {
  int step_h = 8, step_v = 8;
  Eigen::Vector3d vh = p2 - p1;
  Eigen::Vector3d vv = p3 - p2;
  double lv = vv.norm() / (step_v - 1);
  double lh = vh.norm() / (step_h - 1);
  vv.normalize();
  vh.normalize();
  p.resize(step_v * step_h * 3);
  dir.resize(step_v * step_h * 3);

  for (int i = 0; i < step_v; i++) {
    for (int c = 0; c < step_h; c++) {
      p.block(3 * ((i * step_h) + c), 0, 3, 1) =
          p1 + (vv * i * lv) + vh * c * lh;
      dir.block(3 * ((i * step_h) + c), 0, 3, 1) = dn;
    }
  }
}

struct PainterInfo {
  Eigen::Vector3d pos;
  Eigen::Vector3d pos2;
  Eigen::Quaterniond rot;
};

struct InitPosePlanInfo {
  std::string manip_name;
  Eigen::Vector3d conveyor_dir;  // 机器人坐标系下传送带运动方向
  double conveyor_dist;  // 机器人基坐标到传送带的水平距离
  double z;         // 目标物体中心到机器人基坐标系下的高度
  double box_dist;  // 后一个箱子的距离
  Eigen::Vector3d box_size;

  double seam_offset;  // 焊缝距离边缘的距离

  int nsteps;
  double paint_dist;   //
  double paint_angle;  //
  double seam_paint_angle;
};

struct PlanInfo {
  Eigen::Vector3d
      conveyor_dir;  // 机器人坐标系下传送带运动方向，必须为xyz三个方向之一
  Eigen::Vector3d box_size;  // 柜体尺寸
  double box_dist;  // 柜体距离，根据传送带运动方向可以确定柜体的水平位置
  double box_height;       // 柜体高度
  Eigen::Vector3d pos;     // 柜体位置
  Eigen::Quaterniond rot;  // 柜体姿态
  double seam_offset;      // 焊缝距离边缘的距离
  int nsteps;
  double paint_dist;   //     喷涂距离
  double paint_angle;  //     喷涂方向与物体表面法线角度
  Eigen::Vector3d main_paint_dir;  // 喷涂主方向
  double turn_dist;                // 转向距离
  double plan_dist;                // 规划距离
  double seam_paint_angle;
};

struct PaintTraject {
  int nsteps;
  int ndof;
  Eigen::VectorXd joints1;  // 前面的喷涂路径
  Eigen::VectorXd joints2;  // 后面的喷涂路径
};

// p1 上层近点
// p2 下层近点
// p3 右侧上层
// p4 上层远点
void calBoxPose(
    const Eigen::Vector3d& p1,
    const Eigen::Vector3d& p2,
    const Eigen::Vector3d& p3,
    const Eigen::Vector3d& p4,
    float& lx,
    float& ly,
    float& lz,
    Eigen::Vector3d& center,
    Eigen::Quaterniond& quat) {
    Eigen::Vector3d ix = p4 - p1;
    lx				   = ix.norm();
    ix.normalize();
    Eigen::Vector3d iy = p1 - p3;
    ly				   = iy.norm();
    iy.normalize();
    Eigen::Vector3d iz = ix.cross(iy);
    lz				   = (p1 - p2).norm();
    ix				   = iy.cross(iz);
    center			   = p1 + 0.5f * lx * ix - 0.5f * ly * iy - 0.5f * lz * iz;
    Eigen::Matrix3d rotation;
    rotation.col(0) = ix;
    rotation.col(1) = iy;
    rotation.col(2) = iz;
    quat			= Eigen::Quaterniond(rotation);
}

#include <VWSRobot/VWSRobot.h>

int test_traj_line_ik() {

    VWSRobot robot_interf;
    robot_interf.Init("192.168.125.1");
    robot_interf.start();


    Eigen::Vector3d p11, p22, p33, p44;
        p11 << 784.79, -533.38, 879.24;
        p22 << 801.65, -495.94, 96.10;
        p33 << 696.44, -1331.72, 837.84;
        p44 << 1310.26, -578.21, 888.59;
        p11 = p11 / 1000.0; p33 = p33 / 1000.0;
        p22 = p22 / 1000.0; p44 = p44 / 1000.0;
        float lx(-1), ly(-1), lz(-1);
        Eigen::Vector3d center(0, 0, 0);
        Eigen::Quaterniond q;
        q.setIdentity();
        calBoxPose(p11, p22, p33, p44, lx, ly, lz, center, q);

        Eigen::AngleAxisd aa(3.1415926535f / 2.f, Eigen::Vector3d::UnitZ());
        Eigen::Quaterniond q_(aa);


      double box1centerx,box1centery,box1centerz;
      double box1height,box1width,box1length;
      double box1q1,box1q2,box1q3,box1q4;
      box1centerx =center[0];
      box1centery = center[1];
      box1centerz = center[2];
      box1length = lx/2;
      box1width = ly/2;
      box1height = lz/2;
      box1q1=1;
      box1q2=0;
      box1q3=0;
      box1q4=0;


      double box2centerx,box2centery,box2centerz;
      double box2height,box2width,box2length;
      double box2q1,box2q2,box2q3,box2q4;
      box2centerx =1;
      box2centery = -0.7;
      box2centerz = 0.6;
      box2length = 0.4;
      box2width = 0.6;
      box2height = 0.4;
      box2q1=1;
      box2q2=0;
      box2q3=0;
      box2q4=0;

      //箱体三个点
      Eigen::Vector3d point1, point2, point3;
      point1 = Eigen::Vector3d(0,0,0);
      point2 = Eigen::Vector3d(0,0,0);
      point3 = Eigen::Vector3d(0,0,0);


      RAVELOG_DEBUG("TEST\n");
      //*******************************************************
      // 初始化环境
      // URDFTest pt;
      VwsPlanEnv pt(
          "/home/vws/Demo/QT/Spray/build-SpraySystem-Desktop_Qt_5_12_8_GCC_64bit-Debug/urdf/abb_irb1410_urdf/urdf/"
          "abb_irb1410_urdf.urdf",
          "/home/vws/Demo/QT/Spray/build-SpraySystem-Desktop_Qt_5_12_8_GCC_64bit-Debug/urdf/abb_irb1410_urdf/urdf/"
          "abb_irb1410_urdf.srdf");
      auto manip_rot =   Eigen::AngleAxisd(M_PI, Vector3d::UnitZ()) *
                                 Eigen::AngleAxisd(-M_PI / 2, Vector3d::UnitY()) *
                                 Eigen::AngleAxisd(M_PI / 2.0, Vector3d::UnitX());
      auto robot = GetRobot(*pt.env);
      //设置工具坐标系
      pt.addManipulator(robot, "tool", "base", "link_flange", manip_rot,
                     Eigen::Vector3d(0.00, 0.2, 0.285));
      //箱子（立方体）中心点坐标， 箱子尺寸（长框高/2），姿态（四元数）
//      pt.addBox(Eigen::Vector3d(1, -0.7, 0.6), Eigen::Vector3d(0.4, 0.6, 0.4), Eigen::Quaterniond(1,0,0,0), "box1");
//      pt.addBox(Eigen::Vector3d(1, -0.7 + 0.75+1.2, 0.6), Eigen::Vector3d(0.4, 0.6, 0.4), Eigen::Quaterniond(1,0,0,0), "box2");
      // 规划目标前表面
      pt.addBox(Eigen::Vector3d(box1centerx, box1centery, box1centerz), Eigen::Vector3d(box1length, box1width, box1height), Eigen::Quaterniond(box1q1,box1q2,box1q3,box1q4), "box1");
//      pt.addBox(Eigen::Vector3d(box2centerx, box2centery, box2centerz), Eigen::Vector3d(box2length, box2width, box2height), Eigen::Quaterniond(box2q1,box2q2,box2q3,box2q4), "box2");
      // 添加目标上层箱体
      pt.addBox(Eigen::Vector3d(box1centerx, box1centery+0.4, box1centerz+1.2), Eigen::Vector3d(box1length, box1width, box1height), Eigen::Quaterniond(box1q1,box1q2,box1q3,box1q4), "box2");
      // 添加目标前方箱体
      pt.addBox(Eigen::Vector3d(box1centerx, box1centery+1.8, box1centerz), Eigen::Vector3d(box1length, box1width, box1height), Eigen::Quaterniond(box1q1,box1q2,box1q3,box1q4), "box3");
      // 添加目标前方箱体挂钩
      pt.addBox(Eigen::Vector3d(box1centerx, box1centery+1.8, box1centerz), Eigen::Vector3d(0.05, 0.2, box1height+2), Eigen::Quaterniond(box1q1,box1q2,box1q3,box1q4), "hook");
      // 添加墙面
      pt.addBox(Eigen::Vector3d(-1, 0, 1), Eigen::Vector3d(0.01, 5, 2), Eigen::Quaterniond(box1q1,box1q2,box1q3,box1q4), "wall");


      //********************************************************
    #if 1
      //计算喷涂的路径约束
      Eigen::Matrix3d rot;
      // rot 对应矩阵为旋转矩阵，每一列对应
      rot << 1, 0, 0, //
      0, 0, -1, //
      0, 1, 0;
      Eigen::Quaterniond quat(rot);
      quat = Eigen::Quaterniond(box1q1,box1q2,box1q3,box1q4) * quat;
      Eigen::Vector3d p1, p2, p3, dn(0, -1, 0);
      Eigen::Vector4d ori_dn(quat.w(), quat.x(), quat.y(), quat.z());
      rot = (Eigen::AngleAxisd(-M_PI / 4.0, Eigen::Vector3d::UnitZ()) * Eigen::AngleAxisd(M_PI, Eigen::Vector3d::UnitX()) * rot);
      // rot = (Eigen::AngleAxisd(3 * M_PI / 4.0, Eigen::Vector3d::UnitZ()) * rot);
      quat = Eigen::Quaterniond(rot);
      Eigen::Vector4d dn_weld(quat.w(), quat.x(), quat.y(), quat.z());

      // 生成喷涂路线，这里通过 p1,p2,p3 三个点来控制位置，ori_dn来控制姿态，
      // 实际调试时需要更改
      // 最终需要通过柜体的位姿生成
//      Eigen::Vector3d c(1.0, -0.7, 0.6);
//      double dx = 0.4, dy = 0.6, dz = 0.4;
//      p1 = c + Eigen::Vector3d(dx, dy, dz);
//      p2 = c + Eigen::Vector3d(-dx, dy, dz);
//      p3 = c + Eigen::Vector3d(-dx, dy, -dz);



      p1= p44;
      p2=p11;
      p3=p22;

      Eigen::VectorXd p, dir, ori;
      // genInitPathForRect(p1, p2, p3, dn, p, dir);
      // genInitOrientedPathForRect(p1, p2, p3, ori_dn, p, ori);
      genInitOrientedPathForBoxPlane(p1, p2, p3, ori_dn ,
                                     0,  // 0: x, 1: y
                                     0.2, 0.15, 0.1,
                                     0.1, 0, p, ori);


      // Eigen::Isometry3d painter_tf = Eigen::Isometry3d::Identity();
      // Eigen::Quaterniond tcp_2_painter(Eigen::AngleAxisd(3 / 4.0 * M_PI, Eigen::Vector3d::UnitY()));
      // painter_tf.prerotate(tcp_2_painter);
      // painter_tf.translate(Eigen::Vector3d(0, 0, 0.2));
      // genInitOrientedPathForLine(p2, p3, dn_weld, painter_tf, 0.2, 0.2, p, ori);

      int nsteps = p.size() / 3;
      Eigen::VectorXd init_pos(nsteps * 6);
      init_pos = Eigen::VectorXd::Zero(nsteps * 6);
      for (int i = 0; i < nsteps; i++) {
        init_pos[6 * i + 0] = 1.5;
        init_pos[6 * i + 1] = 0;
        init_pos[6 * i + 2] = 0;
        init_pos[6 * i + 3] = 0;
        init_pos[6 * i + 4] = 0;
        init_pos[6 * i + 5] = 0;
      }
    #endif


      // pt.viewer->Idle();
      std::cout << "stage 1" << std::endl;
      ProblemConstructionInfo pci(pt.env);

    #if 1
      int collision_cnt, cnst_voil_cnt;
      double elapsed_time = 0;
      bool valid = false;


      //************************************************************************
      // step 1: 生成喷涂轨迹
      // init_pos =
      //     planWeldPathFixed(pt.env, "toolB_6", ik, p, dir, 0, 0.1, init_pos,
      //                       collision_cnt, cnst_voil_cnt, elapsed_time, true);
      // init_pos =
      //     planWeldPathFixed(pt.env, "toolB_6", ik, p, dir, 100, 0.1, init_pos,
      //                       collision_cnt, cnst_voil_cnt, elapsed_time, true);
      init_pos =
          planOrientedPathFixed(pt.env, "tool", p, ori, 0, 0.1, init_pos,
                                collision_cnt, cnst_voil_cnt, elapsed_time, false);
      init_pos =
          planOrientedPathFixed(pt.env, "tool", p, ori, 100, 0.1, init_pos,
                                collision_cnt, cnst_voil_cnt, elapsed_time, false);
      std::cout << "voil_cnt " << cnst_voil_cnt << ", collision cnt "
                << collision_cnt << std::endl;

      // step 2：取喷涂轨迹的两个端点，生成进入和退出轨迹
      // step 2.1：使用ompl生成初始点位
      std::vector<double> j1, j2;
      for (int i = 0; i < 6; i++){
        j1.push_back(0);
        j2.push_back(init_pos[i]);
      }
      j1[0] = 1.5;

      if(robot==nullptr){
          std::cout<<"robot 为空"<<std::endl;
      }

      auto traj = planFreePathWithOMPL(robot, pt.env, "tool", j1, j2,1);
      int N = traj.size() * traj[0].size();
//      while(traj.size() < nsteps){
//        traj = planFreePathWithOMPL(robot, pt.env, "tool", j1, j2,1);
//        N = traj.size() * traj[0].size();
//      }
      Eigen::VectorXd init_pos_ompl;
      init_pos_ompl.resize(N);
      int current_n = 0;
      for (int i = 0; i < traj.size(); i++){
        for (int n = 0; n < traj[0].size(); n++){
          init_pos_ompl[current_n++] = traj[i][n];
        }
      }
      // step 2.2: 生成优化后的路径
      Eigen::VectorXd goal;
      goal.resize(6);
      goal[0] = init_pos[0];
      goal[1] = init_pos[1];
      goal[2] = init_pos[2];
      goal[3] = init_pos[3];
      goal[4] = init_pos[4];
      goal[5] = init_pos[5];
      if(nsteps <= init_pos_ompl.size() / 6){
          std::cout << "track" << std::endl;
      }
      auto ret = planFreePathJoint(pt.env, "tool", goal, 20, 100, 0.2, init_pos_ompl, collision_cnt,
                   cnst_voil_cnt, elapsed_time, false);

      // init_pos = planWeldLinePathFixed(
      //     pt.env, "toolB_6", ik, line, Eigen::Vector3d(1, 0, -1), nsteps, 0,
      //     init_pos, collision_cnt, cnst_voil_cnt, elapsed_time, valid, true);

      // init_pos = planWeldLinePathFixed(
      //     pt.env, "toolB_6", ik, line, Eigen::Vector3d(1, 0, -1), nsteps, 100,
      //     init_pos, collision_cnt, cnst_voil_cnt, elapsed_time, valid, true);

    #endif

    std::vector<VWSRobot::RobotTask> tasks;
    VWSRobot::RobotTask tk;
    tk.task = VWSRobot::TaskType::MOVEABSJ;  // 1:movel, 2:movej, 3:moveabsj
    tk.speed[0] = 100;
    tk.speed[1] = 10;
    int cnt = 0;
    for(int i = 0; i < ret.size() / 6; i++){
        std::cout << i << ": ";
        std::array<float,7> jv;
        for(int n = 0; n < 6; n++){
            jv[n] = ret[cnt++] / M_PI * 180.0;
            // IRB 1410机器人3轴和2轴耦合，因此需要添加下面的补偿
            if(n == 2){
                jv[n] += jv[n-1];
            }
            std::cout << jv[n] << ", ";
        }
        std::cout << std::endl;
        tk.track.push_back(jv);
    }
    tasks.push_back(tk);
    robot_interf.sendData(tasks);
    while(1);

    return 0;
}

//int main() { test_traj_line_ik(); }
