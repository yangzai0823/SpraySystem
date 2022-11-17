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


class TrajectoryProcess : public QObject
{
    Q_OBJECT
public:
    TrajectoryProcess();

    PlanTask tryGetPlanTask(MainProcess *vdata, vws::PlanTaskInfo *task,
                            std::vector<vws::PlanTaskInfo> &q1,
                            std::vector<vws::PlanTaskInfo> &q2,
                            int64_t encoder_off1, int64_t encoder_off2,
                            int64_t current_encoder, bool dir);

   private:
    std::shared_ptr<VisionContext> visionContext;
private slots:
    void begintraj_Slot(MainProcess *vdata);
signals:
    void traj_Signal(QVariant varmc, QVariant varrbt);
};


#endif // TRAJECTORYPROCESS_H
