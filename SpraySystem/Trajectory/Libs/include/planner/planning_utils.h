
#pragma once
#include <IK/abb_ik.h>
#include <math.h>
#include <openrave-core.h>
#include <openrave/environment.h>
#include <openrave/openrave.h>
#include <planner/ompl_planning_class.h>
//#include <planner/utils.h>

#include <Eigen/Eigen>
#include <deque>
#include <string>
#include <trajopt/configuration_space.hpp>
#include <trajopt/utils.hpp>
#include <vector>

// ompl includes
#include <ompl/base/spaces/RealVectorStateSpace.h>

typedef ompl::base::RealVectorStateSpace JointConfigSpace;
typedef std::shared_ptr<JointConfigSpace> JointConfigSpacePtr;

typedef Eigen::Map<Eigen::Matrix<double, -1, 3>> PointVec;
typedef Eigen::Map<Eigen::Matrix<double, -1, 3>> DirVec;
Eigen::Isometry3d toEigenTransform(const OpenRAVE::Transform& tf);

vws::robot::vIKJointSolution collisionFreeSearch(
    trajopt::RobotAndDOFPtr rad, OpenRAVE::EnvironmentBasePtr env,
    const std::vector<int>& search_indice, vws::robot::AbbIK* ik,
    const std::string& ik_base, RobotCollisionStateChecker* checker,
    const Eigen::Vector3d& pos, const Eigen::Vector4d& quat,
    std::vector<double>& start, const std::vector<double>& end, int level,
    std::vector<double>& search_res);

vws::robot::vIKJointSolution collisionFreeSearch(
    trajopt::RobotAndDOFPtr rad, OpenRAVE::EnvironmentBasePtr env,
    const std::vector<int>& search_indice, vws::robot::AbbIK* ik,
    const std::string& ik_base, RobotCollisionStateChecker* checker,
    const Eigen::Vector3d& pos, const Eigen::Vector4d& quat, int level,
    std::vector<double>& search_res);

Eigen::Isometry3d CalcInitPose(const Eigen::Isometry3d& tf_e,
                               const Eigen::Vector3d& zdir);
/**
 * @brief 对一个空间姿态tf，沿着给定轴axis旋转theta弧度
 *
 * @param tf          空间姿态表达
 * @param axis        旋转轴
 * @param theta       旋转角度 theta
 * @return Eigen::Isometry3d    旋转后的空间姿态
 */
Eigen::Isometry3d rotateAlongAxis(const Eigen::Isometry3d& tf,
                                  const Eigen::Vector3d& axis, double theta);
/**
 * @brief 给定位置和轴约束，搜索可行的机器人逆解。
 *        该函数依次按照搜索轴、末端姿态和末端轴方向的顺序进行搜索可行解
 *        1. 首先依照当前机器人姿态，计算出初始搜索参数
 *        2. 然后对搜索轴进行二分搜索（以level指定搜索深度），如果失败，
 *          保持末端轴方向不变，更改末端姿态，再次进行搜索
 *        3. 如果依旧没有搜索到，则调整末端轴方向，再次搜索。
 *
 * @param rad               机器人规划组，这里只需要传入6轴机器人的规划组
 * @param env               整体环境
 * @param search_indice     搜索轴索引
 * @param ik                逆解模块
 * @param pos               工具坐标需要达到的位置
 * @param dir               工具坐标需要达到的z轴方向
 * @param search_res        输出：搜索轴的搜索结果
 * @return vws::robot::vIKJointSolution
 */
vws::robot::vIKJointSolution searchApplicableSolution(
    trajopt::RobotAndDOFPtr rad, OpenRAVE::EnvironmentBasePtr env,
    const std::vector<int>& search_indice, vws::robot::AbbIK* ik,
    const std::string& ik_base, const Eigen::Vector3d& pos,
    const Eigen::Vector3d& dir, std::vector<double>& search_res,
    int search_level = 6);

JointConfigSpacePtr createBoundedJCSpace(trajopt::RobotAndDOFPtr rad);
ompl::base::ScopedState<> vec2RealVecState(ompl::base::StateSpacePtr ss,
                                           const std::vector<double>& j);

vws::robot::vIKJointSolution planWeldPathWithOMPL(
    trajopt::RobotAndDOFPtr rad, OpenRAVE::EnvironmentBasePtr env,
    OpenRAVE::RobotBase::ManipulatorPtr manip, vws::robot::AbbIKPtr ik,
    const Eigen::VectorXd& points, const Eigen::VectorXd& opt_dirs,
    const std::vector<double>& j1, const std::vector<double>& j2,
    double time = 1.0);

/**
 * @brief 使用OMPL规划自由空间中的路径
 *
 * @param rad       机器人规划组
 * @param env           环境
 * @param j1            机器人起始关节配置
 * @param j2            机器人目标关节配置
 * @param time          规划时间
 * @return vws::robot::vIKJointSolution
 */
vws::robot::vIKJointSolution planFreePathWithOMPL(
    OpenRAVE::RobotBasePtr robot,
    OpenRAVE::EnvironmentBasePtr env,const std::string& plan_manip_name,
    const std::vector<double>& j1, const std::vector<double>& j2, double time);

vws::robot::vIKJointSolution findShortestConfig(
    const vws::robot::vIKJointSolution& j1,
    const vws::robot::vIKJointSolution& j2);

/**
 * @brief 给定7轴情况下规划焊接路径
 *
 * @param env             规划环境
 * @param plan_manip_name   进行plan的关节组
 * @param ik              ik解算模块
 * @param line            焊接的直线端点
 * @param dir             理想无遮挡情况下的焊接方向
 * @param nsteps               轨迹点数
 * @param collision_penalty_coeffs  碰撞惩罚系数
 * @param init_pos                  初始位置
 * @param collision_cnt             最终结果的碰撞个数
 * @param cnst_voil_cnt             最终结果的约束违反个数
 * @param elapsed_time              用时
 * @return Eigen::VectorXd          输出的轨迹
 */
Eigen::VectorXd planWeldLinePathFixed(
    OpenRAVE::EnvironmentBasePtr env, const std::string& plan_manip_name,
    vws::robot::AbbIKPtr ik, const Eigen::VectorXd& line,
    const Eigen::Vector3d& dir, int nsteps, double collision_penalty_coeffs,
    double safety_dist, const Eigen::VectorXd& init_pos, int& collision_cnt,
    int& cnst_voil_cnt, double& elapsed_time, bool& valid,
    bool plotting = false);

/**
 * @brief 给定7轴情况下规划焊接路径
 *
 * @param env               规划环境
 * @param plan_manip_name   进行plan的关节组
 * @param ik                ik解算模块
 * @param positions         焊接路径
 * @param dirs              理想无遮挡情况下的焊接方向
 * @param collision_penalty_coeffs 碰撞惩罚系数
 * @param init_pos                  初始位置
 * @param collision_cnt             最终结果的碰撞个数
 * @param cnst_voil_cnt             最终结果的约束违反个数
 * @param elapsed_time              用时
 * @return Eigen::VectorXd          输出的轨迹
 */
Eigen::VectorXd planWeldPathFixed(
    OpenRAVE::EnvironmentBasePtr env, const std::string& plan_manip_name,
    vws::robot::AbbIKPtr ik, const Eigen::VectorXd& positions,
    const Eigen::VectorXd& dirs, double collision_penalty_coeffs,
    double safety_dist, const Eigen::VectorXd& init_pos, int& collision_cnt,
    int& cnst_voil_cnt, double& elapsed_time, bool plotting = false);

/**
 * @brief 给定7轴情况下规划指定末端位姿的路径
 *
 * @param env               规划环境
 * @param plan_manip_name   进行plan的关节组
 * @param ik                ik解算模块
 * @param positions         末端位置
 * @param ori             末端姿态
 * @param collision_penalty_coeffs 碰撞惩罚系数
 * @param init_pos                  初始位置
 * @param collision_cnt             最终结果的碰撞个数
 * @param cnst_voil_cnt             最终结果的约束违反个数
 * @param elapsed_time              用时
 * @return Eigen::VectorXd          输出的轨迹
 */
Eigen::VectorXd planOrientedPathFixed(
    OpenRAVE::EnvironmentBasePtr env, const std::string& plan_manip_name,
    const Eigen::VectorXd& positions,
    const Eigen::VectorXd& ori, double collision_penalty_coeffs,
    double safety_dist, const Eigen::VectorXd& init_pos, int& collision_cnt,
    int& cnst_voil_cnt, double& elapsed_time, bool plotting);

/**
 * @brief 计算从p点到p1-p2连线的线段之间的距离以及线段上距离p点最近的点
 *
 */
void distance_(const Eigen::Vector3d& p, const Eigen::Vector3d& p1,
               const Eigen::Vector3d& p2, double& d, Eigen::Vector3d& point);

bool checkLineMovePathCollisionFixed(OpenRAVE::EnvironmentBasePtr env,
                                     const std::string& plan_manip_name,
                                     vws::robot::AbbIKPtr ik,
                                     const Eigen::VectorXd& positions,
                                     const Eigen::VectorXd& traj,
                                     double interp_mm);

Eigen::VectorXd planFreePath(
    OpenRAVE::EnvironmentBasePtr env, const std::string& plan_manip_name,
    const Eigen::VectorXd& goal, int nsteps,
    double collision_penalty_coeffs, double safety_dist,
    const Eigen::VectorXd& init_pos, int& collision_cnt, int& cnst_voil_cnt,
    double& elapsed_time, bool plotting);
Eigen::VectorXd planFreePathJoint(
    OpenRAVE::EnvironmentBasePtr env, const std::string& plan_manip_name,
    const Eigen::VectorXd& goal, int nsteps, double collision_penalty_coeffs,
    double safety_dist, const Eigen::VectorXd& init_pos, int& collision_cnt,
    int& cnst_voil_cnt, double& elapsed_time, bool plotting);
