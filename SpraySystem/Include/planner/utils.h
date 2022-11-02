
// 求可行的机器人configure
std::ostream& operator<<(std::ostream& s, const ompl::base::State* st);
std::ostream& operator<<(std::ostream& s, const trajopt::DblVec& pvalue);

class RobotCollisionStateChecker {
 public:
  RobotCollisionStateChecker(OpenRAVE::EnvironmentBasePtr env,
                             trajopt::RobotAndDOFPtr rad);
  bool isCollideFree(const std::vector<double>& state) const;
  bool isCollideFree(const std::vector<double>& js_from,
                     const std::vector<double>& js_to) const;
  trajopt::Link2Int m_link2ind;
  std::vector<OpenRAVE::KinBody::LinkPtr> m_links;
  OpenRAVE::EnvironmentBasePtr env_;
  boost::shared_ptr<trajopt::CollisionChecker> cc_;
  trajopt::RobotAndDOFPtr rad_;
};
vws::robot::vIKJointSolution getApplicableConfigures(
    const RobotCollisionStateChecker* checker, const Eigen::Isometry3d& tf,
    vws::robot::AbbIK* ik);

trajopt::RobotAndDOFPtr GetRADFromName(const std::string& name,
                                       OpenRAVE::RobotBasePtr robot);
float RadicalInverse(uint bits);


#if USE_OCCT
/**
 * @brief Create a Kin Body From O C C T object
 *        从shape中提取所有的面，创建一个完整的KinBody
 * @param env
 * @param aShape    完整的对象Shape
 * @param name      KinBody的名称
 * @return OpenRAVE::KinBodyPtr
 */
OpenRAVE::KinBodyPtr createKinBodyFromOCCT(OpenRAVE::EnvironmentBasePtr env,
                                           const TopoDS_Shape& aShape,
                                           const std::string& name);

/**
 * @brief Create a Geometry From O C C T object
 *        创建一个面对象的几何信息对象
 * @param env
 * @param aShape    一个面的形状
 * @param name      面名称
 * @return OpenRAVE::KinBody::GeometryInfoPtr   几何信息对象
 */
OpenRAVE::KinBody::GeometryInfoPtr createGeometryFromOCCT(
    OpenRAVE::EnvironmentBasePtr env, const TopoDS_Shape& aShape,
    const std::string& name);

TopoDS_Shape readStepTopologies(const std::string& fileName);
#endif


OpenRAVE::KinBodyPtr createKinBodyFromInfos(
    OpenRAVE::EnvironmentBasePtr env,
    std::vector<OpenRAVE::KinBody::GeometryInfoConstPtr> geoms,
    const std::string& name);

OpenRAVE::KinBody::GeometryInfoPtr createBoxInfo(const Eigen::Vector3d& size,
                                                 const Eigen::Isometry3d& tf,
                                                 const std::string& name);

void genInitOrientedPathForBoxPlane(
    const Eigen::Vector3d& p1, const Eigen::Vector3d& p2,
    const Eigen::Vector3d& p3, const Eigen::Vector4d& dn,
    int main_paint_dir,  // 0: x, 1: y
    double tstep_dist, double paint_dist, double vstep_dist, double hstep_dist,
    int Nturn, Eigen::VectorXd& p, Eigen::VectorXd& ori);

/**
 * @brief
 *
 * @param p1 上层点
 * @param p2 下层点
 * @param dn 工具喷涂姿态
 * @param painter_tf  喷枪口和tcp之间的转换关系，该tf右乘dn为tcp位姿
 * @param step_dist  竖直方向规划的步长
 * @param paint_offset  喷涂距离
 * @param p  输出，位置
 * @param ori 输出，姿态
 */
void genInitOrientedPathForLine(const Eigen::Vector3d& p1,
                                const Eigen::Vector3d& p2,
                                const Eigen::Vector4d& dn,
                                const Eigen::Isometry3d& painter_tf,
                                double step_dist, double paint_dist,
                                Eigen::VectorXd& p, Eigen::VectorXd& ori);
class VwsPlanEnv {
 public:
  OpenRAVE::EnvironmentBasePtr env;
  VwsPlanEnv(const std::string& urdf, const std::string& srdf);
  virtual ~VwsPlanEnv();
  void addManipulator(OpenRAVE::RobotBasePtr robot, const std::string& name,
                      const std::string& baselink, const std::string& ee_link,
                      const Eigen::Quaterniond& q, const Eigen::Vector3d& t);

  virtual void SetUpRobot();

  void SetUpEnv();

  OpenRAVE::KinBodyPtr addBox(const Eigen::Vector3d& pos,
                              const Eigen::Vector3d& size,
                              const Eigen::Quaterniond& rot,
                              const std::string& name="box");

  void removeBox(OpenRAVE::KinBodyPtr box);

 public:
  bool verbose_ = false;
  std::string urdf_path_;
  std::string srdf_path_;
};
