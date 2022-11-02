#include "mainprocess.h"
#include <Eigen/Core>
#include <Eigen/Geometry>
#include "VWSRobot/VWSRobot.h"


void imgFunc(const VWSCamera::ImageData &data, void* pUser)
{
//    static int64_t q = data.RGB8PlanarImage.nTimeStamp;
//    static int i = 0;
//    std::cout<<++i<<"  "<<data.RGB8PlanarImage.nTimeStamp<<"      "<<data.RGB8PlanarImage.nTimeStamp - q<<std::endl;
//    q = data.RGB8PlanarImage.nTimeStamp;
//    //saveToFile("/home/joker/testData/"+std::to_string(i),data);

//    //解析图像转换成视觉需要的格式


//    //销毁图像
//    ((VWSCamera *)pUser)->deleteImage(data);
}

void MainProcess::recevieData_Slot(QVariant data)
{
    plcdata = data.value<PLCData>();
    TrajectoryProcessing(true,false);
}


MainProcess::MainProcess()
{
    DeviceManager *deviceManager = DeviceManager::getInstance();
    //PLC
//    auto plc = deviceManager->getPlc();
//    plc->start();
//    connect(plc,SIGNAL(recevieData_Signal(QVariant)),this,SLOT(recevieData_Slot(QVariant)));

    //运动控制器
    auto mc = deviceManager->getMC();
    mc->start();
    connect(mc,SIGNAL(getTrajParam_Signal()),this,SLOT(getTrajParam_Slot()));

    //机器人
//    VWSRobot *vwsrbt = new VWSRobot();
//    auto ret1 = vwsrbt->Init("192.168.125.1");
//     vwsrbt->start();


    auto rbt = deviceManager->getRobot(0);
    rbt->init();
    rbt->start();

//    VWSRobot::RobotPosition p;
//        rbt->getRobotPosition(p);
//    //p.RobJoint[0],p.RobJoint[1],p.RobJoint[2],p.RobJoint[3],p.RobJoint[4],p.RobJoint[5]


//    std::vector<RobotTask> tasks;
//    VWSRobot::RobotTask tk;
//    tk.task = VWSRobot::TaskType::MOVEABSJ;  // 1:movel, 2:movej, 3:moveabsj
//    tk.speed[0] = 10;
//    tk.speed[1] = 10;

//    std::array<float,7> arr;
//    for(auto i =0;i<6;i++){
//        arr[i] = p.RobJoint[i];
//    }
//    tk.track.push_back(arr);
//    tasks.push_back(tk);
//    rbt->sendData(tasks);

//    auto camera1 = deviceManager->getCamera(0);
//    camera1->RegisterFrameCallBack(imgFunc,(void *)(&camera1));
//    auto camera2 = deviceManager->getCamera(1);
//    camera2->RegisterFrameCallBack(imgFunc,(void *)(&camera2));

    imgSocket = new QtSocketClient();
//    connect(imgSocket,SIGNAL(readyRead_Signal(QByteArrary)),this,SLOT(imgData_Slot(QByteArray)));
    connect(imgSocket,SIGNAL(readyRead_Signal(QByteArray)),this,SLOT(imgData_Slot(QByteArray)),Qt::ConnectionType::QueuedConnection);
    auto imgret = imgSocket->connectServer("192.168.125.66",6667);
//    imgSocket->connectServer("172.17.240.1",10000);

    trajProc = new TrajectoryProcess();
    trajThread = new QThread;
    trajProc->moveToThread(trajThread);
    connect(this,SIGNAL(begintraj_Singal(QVariant)), trajProc,SLOT(begintraj_Slot(QVariant)));
    connect(trajProc,SIGNAL(traj_Signal(QVariant,QVariant)),this,SLOT(traj_Slot(QVariant,QVariant)));
    trajThread->start();
}

MainProcess::~MainProcess()
{
    trajThread->quit();
    trajThread->wait();
    delete trajThread;
    delete trajProc;
}

bool range;
int intcamera =2 ;
void MainProcess::TrajectoryProcessing(bool brange = false, bool camera = false)
{
    if(brange)
        range = brange;
    if(camera){
        intcamera++;
    }

    //测距信号，上下相机采图完成，触发路径规划
//    if(range && ((plcdata.cell==1 && intcamera==1) || (plcdata.cell==2 && intcamera==2)))
//    {
//        TrajData trajData;
//        trajData.plcData =plcdata;
//        QVariant data;
//        data.setValue(trajData);
//        emit begintraj_Singal(data);
//        range =false;
//    }
}

void MainProcess::traj_Slot(QVariant varmc,QVariant varrbt)
{
    std::cout<<"反馈规划轨迹"<<std::endl;

    auto mcdata = varmc.value<std::vector<float>>();
    auto rbtdata= varrbt.value<RobotTask>();
    mcQueue.push_back(mcdata);
    trajQueue.push_back(rbtdata);
}


// p1 上层近点
// p2 下层近点
// p3 右侧上层
// p4 上层远点
void calBoxPose1(
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

void MainProcess::imgData_Slot(QByteArray imgData)
{
#pragma pack(1)
    struct DDD{
        double x;
        double y;
        double z;
        double q1;
        double q2;
        double q3;
        double q4;
    };
#pragma pack()
    auto buf = imgData.data();
    DDD* pd = (DDD*)buf;

    TrajParam param;
    param.boxCenterPoint = Eigen::Vector3d(pd->x,pd->y,pd->z);
    param.boxSize = Eigen::Vector3d(535,805,785);  //箱子尺寸lx,ly,lz
    param.boxQuat= Eigen::Quaterniond(pd->q1,pd->q2,pd->q3,pd->q4);

    auto mc = DeviceManager::getInstance()->getMC();
    auto encoders = mc->getChainEncoders();
    auto realtimeencoder = mc->getRealTimeEncoder();

    std::cout<<"拍照悬挂链： "<<encoders[0]<<std::endl;
    std::cout<<"机器人0点： "<<realtimeencoder[0]<<std::endl;

    param.offsetOfTraj = 300;
    param.sevenEncoder = realtimeencoder[0];
    param.encoder=encoders[0];

    QVariant data;
    data.setValue(param);
    emit begintraj_Singal(data);
}

void MainProcess::getTrajParam_Slot()
{
    auto mc= DeviceManager::getInstance()->getMC();


    if(mcQueue.count()>0){
        auto param = mcQueue.dequeue();
        auto zeropoint = param[0];
        auto offset = -param[1];
        offset = -1;

        std::cout<<"运动控制发送信息: "<<zeropoint<<std::endl;
        mc->sendTrajParam(zeropoint,offset);



    }
}
void MainProcess::sendtorbt()
{
    static VWSRobot::RobotTask staticTask;

    auto rbt = DeviceManager::getInstance()->getRobot(0);
    rbt->start();
     VWSRobot::RobotTask rbtparam;
    std::cout<<"机器人发送信息: "<<std::endl;
    if(trajQueue.count()>0){
         rbtparam = trajQueue.dequeue();
         staticTask = rbtparam;
    }
    else
    {
        rbtparam = staticTask;
    }

    std::vector<RobotTask> tasks;
    std::cout<<"规划总点数: "<< rbtparam.track.size() <<std::endl;
    size_t index__(0);
    for(auto&& i:rbtparam.track){
        std::cout << ++index__ << "\t";
        for(auto&&v : i){
            std::cout << v << ",";
        }
        std::cout << std::endl;
    }
    tasks.push_back(rbtparam);
    rbt->sendData(tasks);
    std::cout<<"发送结束"<<std::endl;
}
