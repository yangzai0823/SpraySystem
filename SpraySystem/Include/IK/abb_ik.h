#ifndef VWS_ROBOT_ABB_IK_H
#define VWS_ROBOT_ABB_IK_H

#include <math.h>

#include <Eigen/Eigen>
#include <IK/visibility_control.hpp>
#include <memory>
#include <vector>
namespace vws {
namespace robot {
typedef std::vector<std::vector<double>> vIKJointSolution;
typedef Eigen::Matrix<double, 6, 6> Jacobi6Axis;
struct DHParam {
  std::vector<double> alpha;
  std::vector<double> a;
  std::vector<double> d;
  std::vector<double> theta;
};
class ROBOTLIB_MODULE IKInterface {
 public:
  IKInterface(){};
  virtual void loadDH(const DHParam &dh) = 0;
  virtual vIKJointSolution ikSlove(const Eigen::Matrix4d &m) = 0;
  virtual vIKJointSolution ikSlove(const Eigen::Vector3d &t,
                                   const Eigen::Vector3d &rpy) = 0;
  virtual vIKJointSolution ikSlove(const Eigen::Vector3d &t,
                                   const Eigen::Vector4d &quat) = 0;
  virtual Eigen::Matrix4d forward(std::vector<double> joint_value) = 0;
};

class ROBOTLIB_MODULE AbbIK : public IKInterface {
 public:
  AbbIK()
      : flange_2_joint6_tf_(Eigen::Matrix4d::Identity()),
        dh_2_base_tf_(Eigen::Matrix4d::Identity()),
        tool_2_flange_tf_(Eigen::Matrix4d::Identity()) {}
  /**
   * @brief 加载DH参数，计算中间矩阵
   *
   * @param dh
   */
  virtual void loadDH(const DHParam &dh) override;

  /**
   * @brief 求解逆运动学参数
   *
   * @param m
   * @return vIKJointSolution
   */
  virtual vIKJointSolution ikSlove(const Eigen::Matrix4d &m) override;
  virtual vIKJointSolution ikSlove(const Eigen::Vector3d &t,
                                   const Eigen::Vector3d &rpy) override;
  virtual vIKJointSolution ikSlove(const Eigen::Vector3d &t,
                                   const Eigen::Vector4d &quat) override;
  vIKJointSolution ikSlove(const Eigen::Vector3d &t,
                           const Eigen::Quaterniond &quat);
  void setLimit(const std::vector<double> &lower,
                const std::vector<double> &upper);

  /**
   * @brief
   * 由于dh坐标系的4、5、6三个坐标系原点位于同一点（关节5原点），与实际关节位置不同，因此最后需要对法兰位置进行修正。
   *          在机器人配置文件中的 Flanges 字段有描述0位的空间坐标，可以使用
   * @ref flangeCorrection 计算修正矩阵。
   * @param h
   */
  void setFlangeTransform(const Eigen::Matrix4d &h) { flange_2_joint6_tf_ = h; }
  /**
   * @brief Set the Tool Transform object
   *
   * @param pos
   * @param rpy rpy[0] = rx, rpy[1] = ry, rpy[2] = rz
   */
  void setToolTransform(const Eigen::Vector3d &pos, const Eigen::Vector3d &rpy);

  Eigen::Matrix4d getFlangeTransform() { return flange_2_joint6_tf_; }

  /**
   * @brief
   * 根据机器人位于0位时的空间姿态，计算法兰修正矩阵，在调用此函数前需要先调用
   * @ref setBaseTransform
   * @ref flangeCorrection "computeFlangeTransform"
   * @param h  机器人0位的空间位姿，可以通过机器人配置文件中的 Flanges
   * 字段获得。
   */
  void computeFlangeTransform(const Eigen::Matrix4d &h);

  /**
   * @brief
   * 设置dh坐标系到base坐标系的转换，由于ABB机器人的DH坐标系的joint1的原点与机器人基座不重叠，在Z方向有个偏差，
   *          所以需要通过该矩阵进行修正。该参数在ABB配置文件中的
   * KinematicBaseFrames 字段有描述
   *
   * @param h
   */
  void setBaseTransform(const Eigen::Matrix4d &h) { dh_2_base_tf_ = h; }
  /**
   * @brief 计算正运动学结果
   * @ref setBaseTransform "setBaseTransform"
   * @param joint_value
   * @return Eigen::Matrix4d
   */
  virtual Eigen::Matrix4d forward(std::vector<double> joint_value);

  /**
   * @brief 计算末端tool的雅克比矩阵
   *
   * @param joint_value   关节值，单位：弧度
   * @return Jacobi6Axis
   */
  virtual Jacobi6Axis jacobi(const std::vector<double> &joint_value);
  /**
   * @brief 计算空间中某一点P相对于机器人关节的雅克比矩阵
   *
   * @param joint_value     关节值，单位：弧度
   * @param p
   * @return Jacobi6Axis
   */
  virtual Jacobi6Axis jacobi(const std::vector<double> &joint_value,
                             const Eigen::Vector3d &p);

  /**
   * @brief 计算各个关节到Base的转换关系。
   *
   * @param joint_value   关节值，单位：弧度
   * @return std::vector<Eigen::Matrix4d>
   */
  std::vector<Eigen::Matrix4d> computeJointsToBase(
      const std::vector<double> &joint_value);

 protected:
  /**
   * @brief 计算由axis+1到axis的转换矩阵
   *
   * @param axis 从0开始的axis下标
   * @param v
   * @return Eigen::Matrix4d
   */
  Eigen::Matrix4d computeT(int axis, double v);
  void prepareT(int axis, Eigen::Matrix4d &T);

  Jacobi6Axis jacobi(const std::vector<Eigen::Matrix4d> &vTbase,
                     const Eigen::Vector3d &p);

  /**
   * @brief 计算允许的$\theta_1$, 可能有两个值
   *
   * @param slt
   * @param n
   * @param o
   * @param a
   * @param p
   */
  void computeJoint1(vIKJointSolution &slt, const Eigen::Vector3d &n,
                     const Eigen::Vector3d &o, const Eigen::Vector3d &a,
                     const Eigen::Vector3d &p);
  /**
   * @brief 计算$\theta_2$和$\theta_3$， 每个$\theta_1$对应两个$\theta_2$，
   * 每个$\theta_2$ 对应一个$\theta_3$
   *
   *          | vector 0 |                    $\theta_1$ | $\theta_1$ | | vector
   * 1 |          $\theta_2$      |        $\theta_2$          | $\theta_2$ |
   * $\theta_2$          | | vector 2 |          $\theta_3$      | $\theta_3$ |
   * $\theta_3$     |      $\theta_3$          | | vector 4 | $\theta_5$ |
   * $\theta_5$  |   $\theta_5$ | $\theta_5$  |  $\theta_5$ | $\theta_5$  |
   * $\theta_5$ | $\theta_5$  |
   *
   *          注意，这里计算的theta都是不包含offset的结果
   * @param slt  轴角度
   * @param n
   * @param o
   * @param a
   * @param p
   */
  void computeJoint23(vIKJointSolution &slt, const Eigen::Vector3d &n,
                      const Eigen::Vector3d &o, const Eigen::Vector3d &a,
                      const Eigen::Vector3d &p);
  void computeJoint46(vIKJointSolution &slt, const Eigen::Vector3d &n,
                      const Eigen::Vector3d &o, const Eigen::Vector3d &a,
                      const Eigen::Vector3d &p);
  void computeJoint5(vIKJointSolution &slt, const Eigen::Vector3d &n,
                     const Eigen::Vector3d &o, const Eigen::Vector3d &a,
                     const Eigen::Vector3d &p);
  vIKJointSolution ik(const Eigen::Vector3d &n, const Eigen::Vector3d &o,
                      const Eigen::Vector3d &a, const Eigen::Vector3d &p);

  bool toBoundary(int axis, double &v);
  std::vector<double> searchInBoundary(int axis, double &jv);
  /**
   * @brief 计算偏置后的伺服角度
   *
   * @param axis
   * @param v
   * @return double
   */
  double offsetTheta(int axis, double v);

 protected:
  Eigen::Matrix4d T_[6];
  Eigen::Matrix4d T_tool;
  double upper_[6];
  double lower_[6];
  DHParam dh_;
  Eigen::Matrix4d flange_2_joint6_tf_;
  Eigen::Matrix4d dh_2_base_tf_;
  Eigen::Matrix4d tool_2_flange_tf_;
};

typedef std::shared_ptr<AbbIK> AbbIKPtr;

}  // namespace robot

}  // namespace vws

std::ostream &operator<<(std::ostream &s,
                         const vws::robot::vIKJointSolution &j);

#endif