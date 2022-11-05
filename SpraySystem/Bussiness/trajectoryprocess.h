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
using TrajParam = vws::TrajParam;
Q_DECLARE_METATYPE(RobotTask)  //注册结构体
class MainProcess;
class TrajectoryProcess : public QObject
{
    Q_OBJECT
public:

    TrajectoryProcess();

    std::vector<vws::PlanTaskInfo> tryGetPlanTask(
        MainProcess *vdata, vws::PlanTaskInfo *task,
        std::vector<vws::PlanTaskInfo> &q1, std::vector<vws::PlanTaskInfo> &q2,
        uint64_t encoder_off1, uint64_t encoder_off2, bool dir);

   private:
    std::shared_ptr<VisionContext> visionContext;
private slots:
    void begintraj_Slot(MainProcess* vdata);
signals:
    void traj_Signal(QVariant varmc,QVariant varrbt);
};

#endif // TRAJECTORYPROCESS_H
