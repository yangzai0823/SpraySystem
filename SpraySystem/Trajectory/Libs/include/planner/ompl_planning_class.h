
#pragma once
#include <openrave-core.h>
#include <openrave/openrave.h>

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
// trajopt includes
#include "IK/abb_ik.h"
#include "trajopt/collision_checker.hpp"
#include "trajopt/collision_terms.hpp"
#include "trajopt/common.hpp"
#include "trajopt/problem_description.hpp"
#include "trajopt/rave_utils.hpp"

// deprecated 目前使用discrete motion validator
class MotionChecker : public ompl::base::MotionValidator {
 public:
  MotionChecker(ompl::base::SpaceInformationPtr si,
                OpenRAVE::EnvironmentBasePtr env, trajopt::RobotAndDOFPtr rad);

  bool checkMotion(const ompl::base::State* s1,
                   const ompl::base::State* s2) const override;

  virtual bool checkMotion(
      const ompl::base::State* s1, const ompl::base::State* s2,
      std::pair<ompl::base::State*, double>& lastValid) const override;

  OpenRAVE::EnvironmentBasePtr env_;
  boost::shared_ptr<trajopt::CollisionChecker> cc_;
  trajopt::RobotAndDOFPtr rad_;
};

/**
 * @brief 关节空间的投影
 *
 */
class ProjectionEvalJoint : public ompl::base::ProjectionEvaluator {
 public:
  ProjectionEvalJoint(ompl::base::StateSpacePtr ss);

  unsigned int getDimension() const override;

  virtual void defaultCellSize();

  void project(const ompl::base::State* state,
               Eigen::Ref<Eigen::VectorXd> projection) const override;
};
/**
 * @brief R3空间的投影，将关节状态转换为R3空间坐标
 *
 */
class ProjectionEvalPosition : public ompl::base::ProjectionEvaluator {
 public:
  ProjectionEvalPosition(ompl::base::StateSpacePtr ss,
                         OpenRAVE::EnvironmentBasePtr env,
                         trajopt::RobotAndDOFPtr rad);

  unsigned int getDimension() const override;

  virtual void defaultCellSize();

  void project(const ompl::base::State* state,
               Eigen::Ref<Eigen::VectorXd> projection) const override;

  std::vector<OpenRAVE::KinBody::LinkPtr> m_links;
  OpenRAVE::EnvironmentBasePtr env_;
  boost::shared_ptr<trajopt::CollisionChecker> cc_;
  trajopt::RobotAndDOFPtr rad_;
};
/**
 * @brief 状态碰撞合法性检查，判断一个状态是否产生了碰撞
 *      ！！！注意，所有虚函数中传入的状态值，均为
 *        WrapperStateSpace空间下的封装state，需要解一层包之后
 *        再转换到实际空间中。
 */
class CollisionStateChecker : public ompl::base::StateValidityChecker {
 public:
  CollisionStateChecker(ompl::base::SpaceInformationPtr si,
                        OpenRAVE::EnvironmentBasePtr env,
                        trajopt::RobotAndDOFPtr rad);
  bool isValid(const ompl::base::State* state) const override;

  trajopt::Link2Int m_link2ind;
  std::vector<OpenRAVE::KinBody::LinkPtr> m_links;
  OpenRAVE::EnvironmentBasePtr env_;
  boost::shared_ptr<trajopt::CollisionChecker> cc_;
  trajopt::RobotAndDOFPtr rad_;
};

/**
 * @brief OMPL 直线约束，
 *        ！！！注意，所有虚函数中传入的状态值，均为
 *        WrapperStateSpace空间下的封装state，需要解一层包之后
 *        再转换到实际空间中。
 *
 */
class LinePathConstraint : public ompl::base::Constraint {
 public:
  LinePathConstraint(OpenRAVE::EnvironmentBasePtr env,
                     trajopt::RobotAndDOFPtr rad,
                     OpenRAVE::RobotBase::ManipulatorPtr manip,
                     std::shared_ptr<vws::robot::AbbIK> ik);
  LinePathConstraint(OpenRAVE::EnvironmentBasePtr env,
                     trajopt::RobotAndDOFPtr rad,
                     OpenRAVE::RobotBase::ManipulatorPtr manip,
                     std::shared_ptr<vws::robot::AbbIK> ik,
                     const Eigen::Vector3d& start, const Eigen::Vector3d& end);
  virtual ~LinePathConstraint() {}
  Eigen::Isometry3d getTipTransform(
      const Eigen::Ref<const Eigen::VectorXd>& x) const;
  void setRobotTipOffset(double x, double y, double z);

  virtual double distance(
      const Eigen::Ref<const Eigen::VectorXd>& x) const override;

  void distance(const Eigen::Ref<const Eigen::VectorXd>& x,
                const Eigen::Vector3d& p1, const Eigen::Vector3d& p2, double& d,
                Eigen::Vector3d& point) const;
  void distance(const Eigen::Ref<const Eigen::VectorXd>& x, double& d,
                Eigen::Vector3d& point) const;
  static void distance_(const Eigen::Vector3d& p, const Eigen::Vector3d& p1,
                        const Eigen::Vector3d& p2, double& d,
                        Eigen::Vector3d& point);
  virtual void function(const Eigen::Ref<const Eigen::VectorXd>& x,
                        Eigen::Ref<Eigen::VectorXd> out) const override;
  void function(const Eigen::Ref<const Eigen::VectorXd>& x,
                const Eigen::Vector3d& p1, const Eigen::Vector3d& p2,
                Eigen::Ref<Eigen::VectorXd> out) const;
  virtual void jacobian(const Eigen::Ref<const Eigen::VectorXd>& x,
                        Eigen::Ref<Eigen::MatrixXd> out) const override;
  virtual void jacobian(const ompl::base::State* state,
                        Eigen::Ref<Eigen::MatrixXd> out) const override;
  virtual bool project(Eigen::Ref<Eigen::VectorXd> x) const override;
  virtual bool project(ompl::base::State* state) const override;

  void setFreeDOF(const std::vector<int>& dofindice) { free_dof_ = dofindice; }

  void setLine(Eigen::Vector3d p1, Eigen::Vector3d p2) {
    p1_ = p1;
    p2_ = p2;
  }
  Eigen::Vector3d p1_, p2_;
  std::vector<OpenRAVE::KinBody::LinkPtr> m_links;
  OpenRAVE::EnvironmentBasePtr env_;
  trajopt::RobotAndDOFPtr rad_;
  OpenRAVE::RobotBase::ManipulatorPtr manip_;
  Eigen::Vector3d tool_offset_;
  std::vector<int> link_inds_;
  std::vector<int> free_dof_;
  std::shared_ptr<vws::robot::AbbIK> ik_;
  int NDOF_;
};

typedef Eigen::Map<Eigen::Matrix<double, -1, 3>> PolyLineData;
class PolyLinePathConstraint : public LinePathConstraint {
 public:
  PolyLinePathConstraint(OpenRAVE::EnvironmentBasePtr env,
                         trajopt::RobotAndDOFPtr rad,
                         OpenRAVE::RobotBase::ManipulatorPtr manip,
                         std::shared_ptr<vws::robot::AbbIK> ik);
  PolyLinePathConstraint(OpenRAVE::EnvironmentBasePtr env,
                         trajopt::RobotAndDOFPtr rad,
                         OpenRAVE::RobotBase::ManipulatorPtr manip,
                         std::shared_ptr<vws::robot::AbbIK> ik,
                         const std::vector<double>& points);
  PolyLinePathConstraint(OpenRAVE::EnvironmentBasePtr env,
                         trajopt::RobotAndDOFPtr rad,
                         OpenRAVE::RobotBase::ManipulatorPtr manip,
                         std::shared_ptr<vws::robot::AbbIK> ik,
                         const Eigen::VectorXd& points);
  PolyLinePathConstraint(OpenRAVE::EnvironmentBasePtr env,
                         trajopt::RobotAndDOFPtr rad,
                         OpenRAVE::RobotBase::ManipulatorPtr manip,
                         std::shared_ptr<vws::robot::AbbIK> ik, double* points,
                         int npoints);
  virtual ~PolyLinePathConstraint() {}

  void distance(const Eigen::Ref<const Eigen::VectorXd>& x, double& d,
                Eigen::Vector3d& point) const;
  void distance(Eigen::Vector3d& p, double& d, Eigen::Vector3d& point) const;

  PolyLineData getPolyLine() const;
  void setPolyLine(const Eigen::VectorXd& points);
  void setPolyLine(const std::vector<double>& points);
  void setPolyLine(double* points, int npoints);
  void function(const Eigen::Ref<const Eigen::VectorXd>& x,
                Eigen::Ref<Eigen::VectorXd> out) const override;
  void jacobian(const Eigen::Ref<const Eigen::VectorXd>& x,
                Eigen::Ref<Eigen::MatrixXd> out) const override;

  std::vector<double> polyline_data_;
};

void setInitState(ompl::geometric::PathGeometric& p, trajopt::TrajArray& ta,
                  int trajN);

ompl::base::OptimizationObjectivePtr getPathLengthObjective(
    const ompl::base::SpaceInformationPtr& si);

// class ConstraintInterface{
//   public:
//   ConstraintInterface(){}

// }

// class PlannerInterface {
//   public:
//   PlannerInterface(){}

// };