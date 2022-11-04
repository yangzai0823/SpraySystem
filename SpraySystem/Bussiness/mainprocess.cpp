#include "mainprocess.h"
#include <Eigen/Core>
#include <Eigen/Geometry>
#include "VWSRobot/VWSRobot.h"


void imgFunc(const VWSCamera::ImageData &data, void* pUser)
{
    MainProcess::CameraCallbackData *cameraCallbackData = (MainProcess::CameraCallbackData*)pUser;
    auto mainProcess = cameraCallbackData->mainProcess;
    static int64_t q = data.RGB8PlanarImage.nTimeStamp;
    static int i = 0;
    std::cout<<++i<<"  "<<data.RGB8PlanarImage.nTimeStamp<<"      "<<data.RGB8PlanarImage.nTimeStamp - q<<std::endl;
    q = data.RGB8PlanarImage.nTimeStamp;

    mainProcess->currentData = data;

    Eigen::Isometry3d handEyeMatrix;
    handEyeMatrix.affine().block(0,0,3,4)
            <<  -0.03743671625852585,
            -0.001139160362072289,
            -0.9992983341217041,
            2654.87060546875,
            0.009748989716172219,
            -0.9999521374702454,
            0.0007746600895188749,
            -1332.0638427734376,
            -0.9992514252662659,
            -0.009713122621178627,
            0.03744605928659439,
            481.78564453125;

    VisionData v;

    if(cameraCallbackData->camera->getName()=="Camera 1"){
        mainProcess->mainData.image1 = data;
    }
    else{
       mainProcess->mainData.image2 = data;
    }
    mainProcess->visionContext->work(data,handEyeMatrix,v);

}

void MainProcess::recevieData_Slot(QVariant data)
{
    plcdata = data.value<PLCData>();

   auto ret = signalProcess->work(plcdata, mainData);
   //上层数据采集完毕
   if(ret==2){

   }
   //下层数据采集完毕
   else if(ret == 3){

   }
}


MainProcess::MainProcess()
{
    DeviceManager *deviceManager = DeviceManager::getInstance();
    //PLC
    auto plc = deviceManager->getPlc();
    plc->start();
    connect(plc,SIGNAL(recevieData_Signal(QVariant)),this,SLOT(recevieData_Slot(QVariant)));

    //运动控制器
    auto mc = deviceManager->getMC();
    mc->start();
    connect(mc,SIGNAL(getTrajParam_Signal()),this,SLOT(getTrajParam_Slot()));

    //机器人
    auto rbt = deviceManager->getRobot(0);
    rbt->init();
    rbt->start();

    //相机
    auto camera1 = deviceManager->getCamera(0);
    camera1CallbackData = new  CameraCallbackData();
    camera1CallbackData->camera = camera1;
    camera1CallbackData->mainProcess = this;

    auto ret = camera1->init();
    std::cout<<"camera init: "<<ret<<std::endl;
    ret = camera1->start();
    std::cout<<"camera start: "<<ret<<std::endl;
    camera1->RegisterFrameCallBack(imgFunc,(void *)(camera1CallbackData));
    //    auto camera2 = deviceManager->getCamera(1);
    //    camera2->RegisterFrameCallBack(imgFunc,(void *)(&camera2));



//    VWSRobot::RobotPosition p;
//        rbt->getRobotPosition(p);
//    //p.RobJoint[0],p.RobJoint[1],p.RobJoint[2],p.RobJoint[3],p.RobJoint[4],p.RobJoint[5]


    visionContext = new VisionContext();
    trajProc = new TrajectoryProcess();
    trajThread = new QThread;
    trajProc->moveToThread(trajThread);
    connect(this,SIGNAL(begintraj_Singal(QVariant)), trajProc,SLOT(begintraj_Slot(QVariant)));
    connect(trajProc,SIGNAL(traj_Signal(QVariant,QVariant)),this,SLOT(traj_Slot(QVariant,QVariant)));
    trajThread->start();

    signalProcess = new SignalProcess();
}

MainProcess::~MainProcess()
{  
    trajThread->quit();
    trajThread->wait();
    delete trajThread;
    delete trajProc;

    delete camera1CallbackData;
    delete camera2CallbackData;

    delete signalProcess;
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
