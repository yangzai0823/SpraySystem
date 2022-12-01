#include "visioncontext.h"
#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <fstream>
#include "Data/StaticData.h"
#include <thread>
#include "Util/Log/clog.h"
//#include "halconcpp/HalconCpp.h"
//#include "halconcpp/HDevThread.h"

int saveToFile1(std::string fileName,const VWSCamera::ImageData &data){
    return 1;

    static int32_t index= 0;
    std::string ply = fileName+"_"+std::to_string(index)+".ply";
    std::ofstream outFile;
    //打开文件
    outFile.open(ply);
    int nPointNum = data.PointCloudImage.nDataLen / (sizeof(float) * 3);
    float* pSrcValue = (float*)(data.PointCloudImage.pData);
    outFile<<"ply"<<std::endl;
    outFile << "format ascii 1.0" << std::endl;
    outFile << "comment author:hik-robot" << std::endl;
    outFile << "element vertex "<< nPointNum << std::endl;
    outFile << "property float x" << std::endl;
    outFile << "property float y" << std::endl;
    outFile << "property float z" << std::endl;
    outFile << "end_header" << std::endl;
    for (int nPntIndex = 0; nPntIndex < nPointNum; ++nPntIndex) {
        outFile<<pSrcValue[nPntIndex * 3 + 0]<<" "<<pSrcValue[nPntIndex * 3 + 1]<<" "<<pSrcValue[nPntIndex * 3 + 2]<<std::endl;
    }
    outFile.close();

    index++;
}


VisionContext::VisionContext()
{
    hv_CamParam.Clear();
    hv_CamParam[0] = "area_scan_division";
    hv_CamParam[1] = 0.06;
    hv_CamParam[2] = 0;
    hv_CamParam[3] = 8.5e-06;
    hv_CamParam[4] = 8.5e-06;
    hv_CamParam[5] = 704;
    hv_CamParam[6] = 512;
    hv_CamParam[7] = 1408;
    hv_CamParam[8] = 1024;

}

void VisionContext::work_head(ImageData data,std::vector<float> senorNums, vws::VisionData & VisionData)
{   
     float senorDistance =  vws::senorDistance_b;
     getPoseAndHeight(data,VisionData);
     if(VisionData.hasError){
        return;
     }
     getWidth(senorNums,senorDistance,VisionData);

    double * handEyeMatrix;
    if(VisionData.top_or_bottom == 0)
        {
            //顶
            std::cout<<"上相机手眼"<<std::endl;
            handEyeMatrix = vws::handEyeMatrix_u_rbt1.data();
        }
        else
        {
            //底
            std::cout<<"下相机手眼"<<std::endl;
            handEyeMatrix = vws::handEyeMatrix_b_rbt1.data();
        }
     RobotCenterPose(VisionData, handEyeMatrix, vws::BoxLenght);


    std::string toporbottom = VisionData.top_or_bottom==0?"上层":"下层";
    std::string filename = "/home/vws/Demo/cloud/"+toporbottom+"_head";
    std::cout<<"保存图像"<<std::endl;
    saveToFile1(filename,data);
    // std::thread newthread([](std::string filename,ImageData data){
  
    // },filename,data);
    // newthread.join();
}
void VisionContext::work_trail(ImageData data,std::vector<float> senorNums, vws::VisionData & VisionData)
{   
    // std::cout<<"work_trail: "<<std::to_string(senorNums.at(0))<<", "<<std::to_string(senorNums.at(1))<<std::endl;


    float senorDistance;
    double * handEyeMatrix;
    if(VisionData.top_or_bottom == 0)
    {
        //顶
        std::cout<<"上相机手眼"<<std::endl;
        handEyeMatrix = vws::handEyeMatrix_u_rbt1.data();
    }
    else
    {
        //底
        std::cout<<"下相机手眼"<<std::endl;
        handEyeMatrix = vws::handEyeMatrix_b_rbt1.data();
    }

     getPoseANdHeight_Trail(data,VisionData);
     if(VisionData.hasError){
        return;
     }
     
    std::string toporbottom = VisionData.top_or_bottom==0?"上层":"下层";
    auto strencoder = "encoder("+std::to_string(senorNums.at(0))+","+std::to_string(senorNums.at(1))+")";

    getLenght(senorNums,VisionData);

    RobotCenterPose(VisionData, handEyeMatrix);


    std::string filename = "/home/vws/Demo/cloud/"+toporbottom+"_trail_"+strencoder;
    std::cout<<"保存图像"<<std::endl;
    saveToFile1(filename,data);
    // std::thread newthread([](std::string filename,ImageData data){
     
    // },filename,data);
    // newthread.join();
}
void VisionContext::getPoseAndHeight(ImageData data, vws::VisionData &visionData)
{
    HTuple hv_ObjectModel3D;
    double hv_Result, hv_MedianHeight, hv_MedianWidth;
    double IntersecPonitUL[3], IntersecPonitDR[3], IntersecPonitDL[3], IntersecPonitUR[3];
    // double LineQuali, VectorPosition, MedianHeight, MedianWidth;
    double LineQuali[4];
    double VectorPosition[9];
    double MedianHeight, MedianWidth;
     std::string Result = "0";
    hv_ObjectModel3D = ImageConver(data);
      PCLlibs::HeightAndPoseVector(hv_CamParam, hv_SetParas, hv_ObjectModel3D, MedianHeight, MedianWidth, VectorPosition, IntersecPonitUR,
                        IntersecPonitUL, IntersecPonitDR, IntersecPonitDL, LineQuali, Result);

    if(Result!="1, 运行成功"){
        visionData.hasError = true;
        CLog::getInstance()->log_std("头部高度计算， "+Result,CLog::CLOG_LEVEL::REEROR);
         return;
    }

    CLog::getInstance()->log("视觉结果： 箱体高度, " + QString::number(MedianHeight));
    visionData.height = vws::BoxHeight; //使用固定高度 //MedianHeight;
    visionData.normalvector_head.resize(9);
    memcpy(visionData.normalvector_head.data(),VectorPosition,9*sizeof(double));
    //端点坐标
    //头部
    visionData.righttop.resize(3);
    memcpy(visionData.righttop.data(),IntersecPonitUR,3*sizeof(double));
    visionData.rightbottom.resize(3);
    memcpy(visionData.rightbottom.data(),IntersecPonitDR,3*sizeof(double));
}
void VisionContext::getPoseANdHeight_Trail(ImageData data, vws::VisionData &visionData)
{
    HTuple hv_ObjectModel3D;
    double hv_Result, hv_MedianHeight, hv_MedianWidth;
    double IntersecPonitUL[3], IntersecPonitDR[3], IntersecPonitDL[3], IntersecPonitUR[3];
    // double LineQuali, VectorPosition, MedianHeight, MedianWidth;
    double LineQuali[4];
    double VectorPosition[9];
    double MedianHeight, MedianWidth;
     std::string Result = "0";
    hv_ObjectModel3D = ImageConver(data);
      PCLlibs::HeightAndPoseVector(hv_CamParam, hv_SetParas, hv_ObjectModel3D, MedianHeight, MedianWidth, VectorPosition, IntersecPonitUR,
                        IntersecPonitUL, IntersecPonitDR, IntersecPonitDL, LineQuali, Result);

    if(Result!="1, 运行成功"){
        visionData.hasError = true;
        CLog::getInstance()->log_std("尾部高度计算， "+Result,CLog::CLOG_LEVEL::REEROR);
         return;
    }

    visionData.height = MedianHeight;
    visionData.normalvector.resize(9);
    memcpy(visionData.normalvector.data(),VectorPosition,9*sizeof(double));
    //端点坐标
    //尾部
    visionData.lefttop.resize(3);
    memcpy(visionData.lefttop.data(),IntersecPonitUL,3*sizeof(double));
    visionData.leftbottom.resize(3);
    memcpy(visionData.leftbottom.data(),IntersecPonitDL,3*sizeof(double));
}


void VisionContext:: getWidth(std::vector<float> senorNums, float senorDistance, VisionData &visionData)
{
    double sensorDis1 = senorNums[0];
    double sensorDis2 = senorNums[1];
    double holeDis = vws::holeDistance; 
    double * matRot = vws::senorRotationMatrix_b.data(); //传感器标定矩阵, 方向向量
    std::cout<<"matrot: "<<std::to_string(matRot[0])<<std::endl;
    double v1[] = {visionData.normalvector_head[0],visionData.normalvector_head[1],visionData.normalvector_head[2]};
    double* normalvector = v1;
    double depth; //输出结果
    if(visionData.top_or_bottom==1){
        PCLlibs::CalBoxDeepth(senorDistance,sensorDis1,sensorDis2,holeDis,matRot,normalvector,depth);
        visionData.width = depth;
    }
    else{
        visionData.width = 640;
    }
    CLog::getInstance()->log("视觉结果： 箱体深度, " + QString::number(visionData.width));

}
void VisionContext::getLenght(std::vector<float> encoderNums, VisionData &visionData)
{

    Eigen::Map<Eigen::Matrix<double,3,4,Eigen::RowMajor>> tran(vws::handEyeMatrix_b_rbt1.data());
    Eigen::Map<Eigen::Vector3d> vec(vws::robotBeltDirection.data());
    Eigen::Vector3d encoderVector = tran.block(0,0,3,3).inverse()*vec;

    double encoderHead = encoderNums[0];
    double encoderTrail = encoderNums[1];

    double length;

    PCLlibs::CalBoxLength(encoderVector.data(),encoderTrail,encoderHead,
    visionData.lefttop.data(),visionData.leftbottom.data(),visionData.righttop.data(),visionData.rightbottom.data(),length);

    visionData.length = length*0.8;

    CLog::getInstance()->log("视觉结果： 箱体长度, " + QString::number(visionData.length));
}
void VisionContext::RobotCenterPose(vws::VisionData &visionData, double handEyeMatrix[12])
{
    double boxsize[] = {visionData.height,visionData.length,visionData.width};
    double Quater1[4] = {
        0,
        0,
        0,
        0,
    };
    double centerPoint1[3] = {0, 0, 0};

    std::cout<<"视觉: 处理尾部处理"<<std::endl;
    PCLlibs::CalcuEndCameraRobot(visionData.normalvector.data(),visionData.lefttop.data(),boxsize, handEyeMatrix, Quater1,centerPoint1);
    visionData.robotpose.clear();
    visionData.robotpose.push_back(centerPoint1[0]);
    visionData.robotpose.push_back(centerPoint1[1]);
    visionData.robotpose.push_back(centerPoint1[2]);
    visionData.robotpose.push_back(Quater1[0]);
    visionData.robotpose.push_back(Quater1[1]);
    visionData.robotpose.push_back(Quater1[2]);
    visionData.robotpose.push_back(Quater1[3]);
}
void VisionContext::RobotCenterPose(vws::VisionData &visionData, double handEyeMatrix[12],double length)
{
    CLog::getInstance()->log("视觉： 使用固定长度，{"+QString::number(length)+"} 计算箱体头部信息");
    double boxsize[] = {visionData.height,length,visionData.width};
    double Quater1[4] = {
        0,
        0,
        0,
        0,
    };
    double centerPoint1[3] = {0, 0, 0};
    PCLlibs::CalcuCameraRobot(visionData.normalvector_head.data(),visionData.righttop.data(),boxsize, handEyeMatrix, Quater1,centerPoint1);
    
    visionData.robotpose_head.clear();
    visionData.length_head = length;
    visionData.robotpose_head.push_back(centerPoint1[0]);
    visionData.robotpose_head.push_back(centerPoint1[1]);
    visionData.robotpose_head.push_back(centerPoint1[2]);
    visionData.robotpose_head.push_back(Quater1[0]);
    visionData.robotpose_head.push_back(Quater1[1]);
    visionData.robotpose_head.push_back(Quater1[2]);
    visionData.robotpose_head.push_back(Quater1[3]);
    visionData.head_done = true;
}
HTuple VisionContext::ImageConver(ImageData data)
{
    HTuple hv_ObjectModel3D;
    int width = data.PointCloudImage.nWidth;
    int height = data.PointCloudImage.nHeight;
    int count = width * height;
    float *cloudData = (float*)(data.PointCloudImage.pData);
    std::vector<float> px, py, pz;
    for (int i = 0; i < count; i++)
    {
        px.push_back(cloudData[i*3]);
        py.push_back(cloudData[i*3+1]);
        pz.push_back(cloudData[i*3+2]);
    }

    HImage imgx("real", width, height, (void *)px.data());
    HImage imgy("real", width, height, (void *)py.data());
    HImage imgz("real", width, height, (void *)pz.data());

    // HTuple obj3d;
    XyzToObjectModel3d(imgx, imgy, imgz, &hv_ObjectModel3D);

    return hv_ObjectModel3D;
}