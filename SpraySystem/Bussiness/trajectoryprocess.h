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

using RobotTask = VWSRobot::RobotTask;
using ImageData = VWSCamera::ImageData;
using PLCData = PLCDataPaser::PLCData;
using VisionData = VisionContext::VisionData;
using TrajParam = TrajectoryGenerator::TrajParam;

Q_DECLARE_METATYPE(RobotTask)  //注册结构体
class TrajectoryProcess : public QObject
{
    Q_OBJECT
public:

    TrajectoryProcess();
private:
    std::shared_ptr<VisionContext> visionContext;
private slots:
    void begintraj_Slot(QVariant vdata);
signals:
    void traj_Signal(QVariant varmc,QVariant varrbt);
};

#endif // TRAJECTORYPROCESS_H
