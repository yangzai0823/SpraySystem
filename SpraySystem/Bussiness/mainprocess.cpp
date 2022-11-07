#include "mainprocess.h"
#include <Eigen/Core>
#include <Eigen/Geometry>
#include "VWSRobot/VWSRobot.h"
#include "Data/StaticData.h"

std::mutex MainProcess::_mutex;
void imgFunc(const VWSCamera::ImageData &data, void* pUser)
{
    MainProcess::CameraCallbackData *cameraCallbackData = (MainProcess::CameraCallbackData*)pUser;
    auto mainProcess = cameraCallbackData->mainProcess;
    auto cameraOperator = cameraCallbackData->camera;
    static int64_t q = data.RGB8PlanarImage.nTimeStamp;
    static int i = 0;
    std::cout<<++i<<"  "<<data.RGB8PlanarImage.nTimeStamp<<"      "<<data.RGB8PlanarImage.nTimeStamp - q<<std::endl;
    q = data.RGB8PlanarImage.nTimeStamp;

    mainProcess->currentData = data;

    VisionData v;

    if(cameraCallbackData->camera->getName()=="Camera 2"){

    }
    else{
        std::cout<<"获得下相机图像"<<std::endl;
        mainProcess->mainData.currentBottom.image = data;
        mainProcess->mainData.currentBottom.imgFlag =true;
        mainProcess->UpdateCurrentData(mainProcess->mainData.currentBottom, 0);
    }
//    mainProcess->visionContext->work(data,handEyeMatrix,v);

}

void MainProcess::recevieData_Slot(QVariant data)
{
    plcdata = data.value<PLCData>();

   auto ret = signalProcess->work(plcdata, mainData);
   //上层数据采集完毕
   if(ret==1){

   }
   else if(ret == 2){
    std::cout<<"下层数据采集完毕,处理第一张图片"<<std::endl;
      UpdateCurrentData(mainData.currentBottom,0);
   }
}


MainProcess::MainProcess()
{
    vws::DataInit::Init();

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
    connect(this,SIGNAL(begintraj_Singal(MainProcess* )), trajProc,SLOT(begintraj_Slot(MainProcess* )));
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

std::vector<vws::PlanTaskInfo> *MainProcess::GetPlanTaskInfo(int upper_or_bottom)
{
    if(upper_or_bottom == 0){
         return &qPlanTaskInfoBottom;
    }else
    {
        return &qPlanTaskInfoTop;
    }
}

float MainProcess::getChainSpeed() const
{
    return 80;
}

uint64_t MainProcess::getChainEncoder() const
{
    // auto val = DeviceManager::getInstance()->getMC()->getChainEncoders();
    // return val[0];
    static uint64_t en = 1e8;
    en -= 50;
    return en;
}

float MainProcess::getChainUnits() const
{
    return 1/0.4198727819755431f;
}

Eigen::VectorXd MainProcess::getRobotWaitPose() const
{
    Eigen::VectorXd val(6);
    val << 1.5, 0, 0, 0, 0, 0;
    std::cout<<"getRobotWaitPose: "<<val[0]<<std::endl;
    return val;
}

bool MainProcess::getChainEncoderDir() const
{
    //TODO: 根据实际情况
    return false;
}

void MainProcess::VisionProcessing(vws::ProcessData data,bool upper_or_bottom)
{
    std::cout<<"视觉处理"<<std::endl;
    VisionData visionData;
    visionContext->work(data.image,vws::handEyeMatrix, visionData);
    DeviceManager::getInstance()->getCamera(0)->deleteImage(data.image);
    data.imgFlag =false;
    vws::PlanTaskInfo planTaskInfo;
    planTaskInfo.diff =  vws::diff;
    planTaskInfo.lx = visionData.width;
    planTaskInfo.ly = vws::BoxLenght;
    planTaskInfo.lz = vws::BoxHeight;
    planTaskInfo.encoder = data.imgEncoder;
    planTaskInfo.face =data.face;
    planTaskInfo.boxInfo = Eigen::Isometry3d::Identity();
    planTaskInfo.boxInfo.prerotate(Eigen::Quaterniond(1,2,3,4));
    planTaskInfo.boxInfo.pretranslate(Eigen::Vector3d(1,2,3));
    if(upper_or_bottom){
       std::cout<<"顶层箱子参数进入队列"<<std::endl;
        qPlanTaskInfoTop.push_back(planTaskInfo);
    }
    else{
          std::cout<<"底层箱子参数进入队列"<<std::endl;
        qPlanTaskInfoBottom.push_back(planTaskInfo);
    }

    std::cout<<"触发轨迹规划信号"<<std::endl;

    emit begintraj_Singal(this);
}

void MainProcess::UpdateCurrentData(vws::ProcessData data, bool upper_or_bottom)
{
    _mutex.lock();
    if(data.flag ==2 && data.imgFlag){
        data.flag = 4;
    }
    _mutex.unlock();

   if(data.flag==4){
        //视觉处理
        data.flag =6;
        VisionProcessing(data,upper_or_bottom);
        data.flag  =5;
   }
}



// void MainProcess::imgData_Slot(QByteArray imgData)
// {
// #pragma pack(1)
//     struct DDD{
//         double x;
//         double y;
//         double z;
//         double q1;
//         double q2;
//         double q3;
//         double q4;
//     };
// #pragma pack()
//     auto buf = imgData.data();
//     DDD* pd = (DDD*)buf;

//     TrajParam param;
//     param.boxCenterPoint = Eigen::Vector3d(pd->x,pd->y,pd->z);
//     param.boxSize = Eigen::Vector3d(535,805,785);  //箱子尺寸lx,ly,lz
//     param.boxQuat= Eigen::Quaterniond(pd->q1,pd->q2,pd->q3,pd->q4);

//     auto mc = DeviceManager::getInstance()->getMC();
//     auto encoders = mc->getChainEncoders();
//     auto realtimeencoder = mc->getRealTimeEncoder();

//     std::cout<<"拍照悬挂链： "<<encoders[0]<<std::endl;
//     std::cout<<"机器人0点： "<<realtimeencoder[0]<<std::endl;

//     param.offsetOfTraj = 300;
//     param.sevenEncoder = realtimeencoder[0];
//     param.encoder=encoders[0];

//     QVariant data;
//     data.setValue(param);
//     emit begintraj_Singal(data);
// }


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

// void MainProcess::sendtorbt()
// {
//     static VWSRobot::RobotTask staticTask;

//     auto rbt = DeviceManager::getInstance()->getRobot(0);
//     rbt->start();
//      VWSRobot::RobotTask rbtparam;
//     std::cout<<"机器人发送信息: "<<std::endl;
//     if(trajQueue.count()>0){
//          rbtparam = trajQueue.dequeue();
//          staticTask = rbtparam;
//     }
//     else
//     {
//         rbtparam = staticTask;
//     }

//     std::vector<RobotTask> tasks;
//     std::cout<<"规划总点数: "<< rbtparam.track.size() <<std::endl;
//     size_t index__(0);
//     for(auto&& i:rbtparam.track){
//         std::cout << ++index__ << "\t";
//         for(auto&&v : i){
//             std::cout << v << ",";
//         }
//         std::cout << std::endl;
//     }
//     tasks.push_back(rbtparam);
//     rbt->sendData(tasks);
//     std::cout<<"发送结束"<<std::endl;
// }
