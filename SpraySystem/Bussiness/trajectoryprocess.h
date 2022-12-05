#ifndef TRAJECTORYPROCESS_H
#define TRAJECTORYPROCESS_H

#include "Device/PLC/plcdatapaser.h"
#include "VWSCamera/VWSCamera.h"
#include "VWSRobot/VWSRobot.h"
#include <QObject>
#include <QQueue>
#include <QThread>
#include <QVariant>
#include <memory>
#include <vector>
#include <QThread>
#include "VWSRobot/VWSRobot.h"
#include "VWSCamera/VWSCamera.h"
#include "Device/PLC/plcdatapaser.h"
#include "Data/StructData.h"
#include "Include/Core/Config/config.hpp"
#include "Include/Core/Config/jsonserializer.h"
#include "Util/threadsafevector.h"
#include "Trajectory/trajectorygenerator.h"
#include <queue>
using RobotTask = VWSRobot::RobotTask;
using ImageData = VWSCamera::ImageData;
using PLCData = vws::PLCData;
using VisionData = vws::VisionData;

Q_DECLARE_METATYPE(RobotTask) // 注册结构体
class MainProcess;

template <typename _Tp, bool increase>
struct PairFirstLess : public std::binary_function<std::pair<int, _Tp>,
                                                   std::pair<int, _Tp>, bool>
{
    _GLIBCXX14_CONSTEXPR
    bool operator()(const std::pair<int, _Tp> &__x,
                    const std::pair<int, _Tp> &__y) const
    {
        return increase ? __x.first >= __y.first : __x.first < __y.first;
    }
};

struct PlanTask
{
    std::vector<vws::PlanTaskInfo> envs_;
    std::vector<vws::PlanTaskInfo> targets_;
};
typedef std::priority_queue<std::pair<int, PlanTask>,
                            std::vector<std::pair<int, PlanTask>>,
                            PairFirstLess<PlanTask, false>>
    SortedTaskQ;

enum TrajectoryType
{
    SeamPaintTraj,       // 喷涂焊缝的轨迹
    PlanePaintTraj,      // 喷涂面的轨迹
    SafeTransitionTraj,  // 安全转移轨迹
    UnsafeTransitionTraj // 不安全转移轨迹
};

struct TrajectoryInfo
{
    Eigen::VectorXd traj_;
    TrajectoryType type_;
};

using Config = vws::properties::Config;

class PlanStragety
{
public:
    PlanStragety(const std::string &filename);
    // void save(Config c) const;
    void load();
    bool speedOf(const std::string &key, double &s) const;
    /**
     * @brief 获取从from到to的转移轨迹，其中轨迹包括to的关节角，不包括from的关节角
     *
     * @param from
     * @param to
     * @param t
     * @return true
     * @return false
     */
    bool transitionOf(const std::string &from, const std::string &to,
                      Eigen::VectorXd &t) const;
    bool teachPoseOf(const std::string &key, Eigen::VectorXd &pose) const;
    bool tacticOf(const std::string &name, std::string &work_tactic,
                  std::string &cancle_tactic, std::string &follow,
                  float &follow_offset) const;
    bool strategyOf(bool issingle, bool isup, bool isfront,
                    std::string &strategy) const;
    bool isInvert() const { return invert_; }
    bool isYIncrease() const { return y_increase_; }
    int getNdof() const { return ndof; }
    double getFrontSeamExtraPaintSpace() const { return front_seam_space_; }
    double getBackSeamExtraPaintSpace() const { return back_seam_space_; }
    double getPaintExtremePos() const { return paint_extreme_pos_; }
    double getFrontPlanePaintSpace() const { return front_plane_space_; }
    double getBackPlanePaintSpace() const { return back_plane_space_; }
    double getPosOfPaintHouse() const { return pos_of_paint_house_; }
    double getGuardDist() const { return guard_dist_; }
    double getTransitionRadius() const { return transition_radius_; }
    double getExtraAxisMax() const { return extra_axis_max_; }
    double getExtraAxisMin() const { return extra_axis_min_; }
    double getSeamOffset() const { return seam_offset_; }

private:
    std::map<std::string, Eigen::VectorXd> teachPoseMap_;
    std::map<std::string, Eigen::VectorXd> transitionsMap_;
    std::map<std::string, double> speedMap_;
    std::map<std::string, std::string> strategiesMap_;
    Config teachPose_;
    Config transition_;
    Config speed_;
    Config tactic_;
    Config strategies_;
    int ndof;
    double front_seam_space_;
    double back_seam_space_;
    double front_plane_space_;
    double back_plane_space_;
    double pos_of_paint_house_;
    double guard_dist_;
    double transition_radius_;
    double paint_extreme_pos_;
    double extra_axis_min_;
    double extra_axis_max_;
    double seam_offset_;
    std::string filename_;
    bool invert_;
    bool y_increase_;
};

class TrajectoryProcess : public QObject
{
    Q_OBJECT
public:
    TrajectoryProcess();
    ~TrajectoryProcess();
    PlanTask tryGetPlanTask(vws::PlanTaskInfo *task,
                            std::vector<vws::PlanTaskInfo> &q1,
                            std::vector<vws::PlanTaskInfo> &q2,
                            int64_t encoder_off1, int64_t encoder_off2,
                            int64_t current_encoder, bool isIncrease,
                            float units);
    SortedTaskQ
    PrepareTaskInfoOneByOne(std::vector<vws::PlanTaskInfo> *upper_task_q,
                            std::vector<vws::PlanTaskInfo> *bottom_task_q,
                            int64_t current_encoder, int64_t bottom_2_upper,
                            bool isIncrease, float units, int64_t plan_delay);

    PlanTask tryGetPlanTaskInTwoLayer(vws::PlanTaskInfo *task,
                                      std::vector<vws::PlanTaskInfo> &q1,
                                      std::vector<vws::PlanTaskInfo> &q2,
                                      int64_t encoder_off1, int64_t encoder_off2,
                                      int64_t current_encoder, bool isIncrease,
                                      float units);
    SortedTaskQ
    PrepareTaskInfoTwoLayers(ThreadSafeVector<vws::PlanTaskInfo> *upper_task_q,
                             ThreadSafeVector<vws::PlanTaskInfo> *bottom_task_q,
                             int64_t current_encoder, int64_t bottom_2_upper,
                             bool isIncrease, float units, int64_t plan_delay);

    /**
     * @brief 对一层进行规划信息的准备。
     *
     * @param q1                    需要规划的队列
     * @param q2                    另一层队列
     * @param current_encoder       当前编码器值
     * @param q1_offset             q1队列编码器修正值
     * @param q2_offset             q2队列编码器修正值
     * @param isIncrease
     * @param units
     * @param plan_delay
     * @return SortedTaskQ
     */
    SortedTaskQ PrepareTaskInfoOneLayer(std::vector<vws::PlanTaskInfo> *q1,
                                        std::vector<vws::PlanTaskInfo> *q2,
                                        int64_t current_encoder,
                                        int64_t q1_offset, int64_t q2_offset,
                                        bool isIncrease, float units,
                                        int64_t plan_delay);

    bool planOneTask(TrajectoryGenerator *generator, PlanTask &task_info,
                     const Eigen::VectorXd &init_dof,
                     std::vector<TrajectoryInfo> &out_traj,
                     std::vector<float> &mc_data, float units, bool isIncrease,
                     bool invert, bool plane_first = true);

private:
    PlanStragety stragety1_;
    PlanStragety stragety2_;
private slots:
    void begintraj_Slot(MainProcess *vdata);
signals:
    void traj_Signal(QVariant varmc, QVariant varrbt);

private:
    std::ofstream logfile_;
};

#endif // TRAJECTORYPROCESS_H
