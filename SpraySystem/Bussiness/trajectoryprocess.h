#ifndef TRAJECTORYPROCESS_H
#define TRAJECTORYPROCESS_H

#include <vector>
#include <QObject>
#include <QQueue>
#include <memory>
#include <QVariant>
#include "VWSRobot/VWSRobot.h"
#include "VWSCamera/VWSCamera.h"
#include "Device/PLC/plcdatapaser.h"
//#include "Device/PLC/PLCData.h"
#include "Trajectory/trajectorycontext.h"
#include "Vision/visioncontext.h"
#include "Data/StructData.h"

using RobotTask = VWSRobot::RobotTask;
using ImageData = VWSCamera::ImageData;
using PLCData = vws::PLCData;
using VisionData = vws::VisionData;

Q_DECLARE_METATYPE(RobotTask) //注册结构体
class MainProcess;


template<typename _Tp>
    struct PairFirstLess : public std::binary_function<std::pair<int, _Tp>, std::pair<int, _Tp>, bool>
    {
      _GLIBCXX14_CONSTEXPR
      bool
      operator()(const std::pair<int, _Tp>& __x, const std::pair<int, _Tp>& __y) const
      { return __x.first < __y.first; }
    };

struct PlanTask{
    std::vector<vws::PlanTaskInfo> envs_;
    std::vector<vws::PlanTaskInfo> targets_;
};
typedef std::priority_queue<
    std::pair<int, PlanTask>,
    std::vector<std::pair<int, PlanTask>>,
    PairFirstLess<PlanTask>>
    SortedTaskQ;

enum TrajectoryType { 
  PaintTraj,
  TransitionTraj
};

struct TrajectoryInfo{
  Eigen::VectorXd traj_;
  TrajectoryType type_;
};
class TrajectoryProcess : public QObject
{
    Q_OBJECT
public:
    TrajectoryProcess();

    PlanTask tryGetPlanTask(vws::PlanTaskInfo *task,
                            std::vector<vws::PlanTaskInfo> &q1,
                            std::vector<vws::PlanTaskInfo> &q2,
                            int64_t encoder_off1, int64_t encoder_off2,
                            int64_t current_encoder, bool isIncrease,
                            float units);
    SortedTaskQ PrepareTaskInfoOneByOne(
        std::vector<vws::PlanTaskInfo> *upper_task_q,
        std::vector<vws::PlanTaskInfo> *bottom_task_q, int64_t current_encoder,
        int64_t bottom_2_upper, bool isIncrease, float units,
        int64_t plan_delay);


    
    PlanTask tryGetPlanTaskInTwoLayer(vws::PlanTaskInfo *task,
                                      std::vector<vws::PlanTaskInfo> &q1,
                                      std::vector<vws::PlanTaskInfo> &q2,
                                      int64_t encoder_off1,
                                      int64_t encoder_off2,
                                      int64_t current_encoder, bool isIncrease,
                                      float units);
    SortedTaskQ PrepareTaskInfoTwoLayers(
        std::vector<vws::PlanTaskInfo> *upper_task_q,
        std::vector<vws::PlanTaskInfo> *bottom_task_q, int64_t current_encoder,
        int64_t bottom_2_upper, bool isIncrease, float units,
        int64_t plan_delay);


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

   private:
    std::shared_ptr<VisionContext> visionContext;
private slots:
    void begintraj_Slot(MainProcess *vdata);
signals:
    void traj_Signal(QVariant varmc, QVariant varrbt);
};


#endif // TRAJECTORYPROCESS_H
