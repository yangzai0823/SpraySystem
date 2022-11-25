#include "visioncontext.h"
#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <fstream>
#include "Data/StaticData.h"
//#include "halconcpp/HalconCpp.h"
//#include "halconcpp/HDevThread.h"

int saveToFile1(std::string fileName,const VWSCamera::ImageData &data){
    std::string ply = fileName+".ply";
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

void VisionContext::work(ImageData data,  vws::VisionData & visionData)
{
    //getPoseAndHeight(data,visionData);
    //RobotCenterPose(visionData, handEyeMatrix);
    double handEyeMatri[12];
    //RobotCenterPose(visionData,handEyeArry);
    visionProcessFunc(data, visionData);
}

void VisionContext::work_head(ImageData data,std::vector<float> senorNums, vws::VisionData & VisionData)
{   
    // std::cout<<"work_head: "<<std::to_string(senorNums.at(0))<<", "<<std::to_string(senorNums.at(1))<<std::endl;
    // std::string filename = "/home/vws/Demo/cloud/head_"+std::to_string(senorNums.at(0))+","+std::to_string(senorNums.at(1));
    // saveToFile1(filename,data);

     float senorDistance =  vws::senorDistance_b;
     getPoseAndHeight(data,VisionData);
     getWidth(senorNums,senorDistance,VisionData);
}
void VisionContext::work_headWithLength(vws::VisionData & VisionData)
{
    double * handEyeMatrix;
    if(VisionData.top_or_bottom == 0)
    {
        //顶
        handEyeMatrix = vws::handEyeMatrix_u_rbt1.data();
    }
    else
    {
        //底
        handEyeMatrix = vws::handEyeMatrix_b_rbt1.data();
    }

    RobotCenterPose(VisionData,handEyeMatrix, vws::HeadMoveMaxLength);
}
void VisionContext::work_trail(ImageData data,std::vector<float> senorNums, vws::VisionData & VisionData)
{   
    // std::cout<<"work_trail: "<<std::to_string(senorNums.at(0))<<", "<<std::to_string(senorNums.at(1))<<std::endl;
    // std::string filename = "/home/vws/Demo/cloud/trail_"+std::to_string(senorNums.at(0))+","+std::to_string(senorNums.at(1));
    // saveToFile1(filename,data);

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
     
     //getWidth(senorNums,senorDistance,VisionData);
    //  getLenght(vws::senorRotationMatrix_b.data(), senorNums,VisionData);
    //TODO: Length
    if(VisionData.top_or_bottom == 1)
    {
        VisionData.length = 800;
    }
    else{
        VisionData.length = 1000;
    }
     RobotCenterPose(VisionData, handEyeMatrix);
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


    std::cout<<"视觉结果： 箱体高度, "<<std::to_string(MedianHeight)<<std::endl;
    visionData.height = MedianHeight;
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


    std::cout<<"视觉结果： 箱体高度, "<<std::to_string(MedianHeight)<<std::endl;
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
    std::cout<<"视觉结果： 箱体深度, "<<std::to_string(visionData.width)<<std::endl;

}
void VisionContext::getLenght(std::vector<double> encoderVector, std::vector<float> encoderNums, VisionData &visionData)
{
    double encoderHead = encoderNums[0];
    double encoderTrail = encoderNums[1];

    double length;

    if(visionData.top_or_bottom==1)
    {
        PCLlibs::CalBoxLength(encoderVector.data(),encoderTrail,encoderHead,
        visionData.lefttop.data(),visionData.leftbottom.data(),visionData.righttop.data(),visionData.rightbottom.data(),length);

        visionData.length = length;
    }
    else
    {
        visionData.length = 1300;
    }
    std::cout<<"视觉结果： 箱体长度, "<<std::to_string(length)<<std::endl;

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
    visionData.robotpose.push_back(centerPoint1[0]);
    visionData.robotpose.push_back(centerPoint1[1]);
    visionData.robotpose.push_back(centerPoint1[2]);
    visionData.robotpose.push_back(Quater1[0]);
    visionData.robotpose.push_back(Quater1[1]);
    visionData.robotpose.push_back(Quater1[2]);
    visionData.robotpose.push_back(Quater1[3]);

    if(!visionData.head_done){
        std::cout<<"视觉: 头部未处理，真实箱体位姿传给头部"<<std::endl;
        PCLlibs::CalcuCameraRobot(visionData.normalvector_head.data(),visionData.righttop.data(),boxsize, handEyeMatrix, Quater1,centerPoint1);
        visionData.robotpose_head.push_back(centerPoint1[0]);
        visionData.robotpose_head.push_back(centerPoint1[1]);
        visionData.robotpose_head.push_back(centerPoint1[2]);
        visionData.robotpose_head.push_back(Quater1[0]);
        visionData.robotpose_head.push_back(Quater1[1]);
        visionData.robotpose_head.push_back(Quater1[2]);
        visionData.robotpose_head.push_back(Quater1[3]);
    }
}
void VisionContext::RobotCenterPose(vws::VisionData &visionData, double handEyeMatrix[12],double length)
{
    std::cout<<"视觉： 使用固定长度，{"<<std::to_string(length)<<"} 计算箱体头部信息"<<std::endl;

    double boxsize[] = {visionData.height,length,visionData.width};
    double Quater1[4] = {
        0,
        0,
        0,
        0,
    };
    double centerPoint1[3] = {0, 0, 0};
    PCLlibs::CalcuCameraRobot(visionData.normalvector_head.data(),visionData.righttop.data(),boxsize, handEyeMatrix, Quater1,centerPoint1);
    
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
void VisionContext::visionProcessFunc(const VWSCamera::ImageData &data, vws::VisionData &visionData)
{

    // static int index = 0;
    // saveToFile1("/home/vws/Demo/1/"+std::to_string(index),data);
    // index++;
    
    HTuple hv_CamParam, hv_ObjectModel3D, hv_Status;
    double hv_Result, hv_MedianHeight, hv_MedianWidth;
    double IntersecPonitUL[3], IntersecPonitDR[3], IntersecPonitDL[3], IntersecPonitUR[3];
    double LineQuali[4];
    double VectorPosition[9];
    double MedianHeight, MedianWidth;

    std::string Result = "0";
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
    double hv_SetParas[6] = {900, 1720, -1220, 880, 6, 1e5};


    int width = data.PointCloudImage.nWidth;
    int height = data.PointCloudImage.nHeight;
    int count = width * height;
    float *cloudData = (float *)(data.PointCloudImage.pData);
    std::vector<float> px, py, pz;
    for (int i = 0; i < count; i++)
    {
        px.push_back(cloudData[i * 3]);
        py.push_back(cloudData[i * 3 + 1]);
        pz.push_back(cloudData[i * 3 + 2]);
    }

    HImage imgx("real", width, height, (void *)px.data());
    HImage imgy("real", width, height, (void *)py.data());
    HImage imgz("real", width, height, (void *)pz.data());

    XyzToObjectModel3d(imgx, imgy, imgz, &hv_ObjectModel3D);


    PCLlibs::HeightAndPoseVector(hv_CamParam, hv_SetParas, hv_ObjectModel3D, MedianHeight, MedianWidth, VectorPosition, IntersecPonitUR,
                                 IntersecPonitUL, IntersecPonitDR, IntersecPonitDL, LineQuali, Result); ////

    double Quater[4];
    double RightUpPoint1[3];
    double boxSize[3] = {800, 500, 535};

    double Quater1[4] = {
        0,
        0,
        0,
        0,
    };
    double centerPoint1[3] = {0, 0, 0};
    // double handEyeMatrix[12] = {-0.03743671625852585,
    //                             -0.001139160362072289,
    //                             -0.9992983341217041,
    //                             2654.87060546875,
    //                             0.009748989716172219,
    //                             -0.9999521374702454,
    //                             0.0007746600895188749,
    //                             -1332.0638427734376,
    //                             -0.9992514252662659,
    //                             -0.009713122621178627,
    //                             0.03744605928659439,
    //                             481.78564453125};
       double handEyeMatrix[12] = {0.047927375882864,
                                0.029266173020005226,
                                0.9984219670295715,
                                469.2415771484375,
                                0.005496096797287464,
                                0.9995478391647339,
                                -0.029563141986727715,
                                2466.051025390625,
                                -0.9988358020782471,
                                0.006904320791363716,
                                0.04774487391114235,
                                -532.5537719726563};
    if (Result == "1, 运行成功")
    {
        if(visionData.top_or_bottom == 0){
            double handEyeMatrix_u[12] = {-0.0150250699,
                        -0.118890852,
                        0.992793739,
                        519.98938,
                        0.0130119231,
                        0.992798448,
                        0.11908827,
                        2632.92798,
                        -0.999802351,
                        0.0147074759,
                        -0.0133698536,
                        668.73468};
            std::cout<<"顶层手眼， "<<std::to_string(handEyeMatrix_u[0])<<std::endl;

            PCLlibs::CalcuCameraRobot(VectorPosition, IntersecPonitUR, boxSize, handEyeMatrix_u, Quater1, centerPoint1);

        }
        else
        {
              double handEyeMatrix_b[12] = {0.047927375882864,
                                0.029266173020005226,
                                0.9984219670295715,
                                469.2415771484375,
                                0.005496096797287464,
                                0.9995478391647339,
                                -0.029563141986727715,
                                2466.051025390625,
                                -0.9988358020782471,
                                0.006904320791363716,
                                0.04774487391114235,
                                -532.5537719726563};
            std::cout<<"低层手眼， "<<std::to_string(handEyeMatrix_b[0])<<std::endl;

            PCLlibs::CalcuCameraRobot(VectorPosition, IntersecPonitUR, boxSize, handEyeMatrix_b, Quater1, centerPoint1);

        }

   
        // PCLlibs::CalcuCameraRobot(VectorPosition, IntersecPonitUR, boxSize, handEyeMatrix, Quater1, centerPoint1);
        std::cout << "VectorUnit:  [  " << VectorPosition[0] << ",   " << VectorPosition[1] << ",   " << VectorPosition[2] << ",   " << VectorPosition[3] << ",   " << VectorPosition[4] << ",   " << VectorPosition[5] << ",   " << VectorPosition[6] << ",   " << VectorPosition[7] << ",   " << VectorPosition[8] << "]  " << std::endl; //左侧棱长向量
        std::cout << "LineQuality:  [  " << LineQuali[0] << ",   " << LineQuali[1] << ",   " << LineQuali[2] << ",   " << LineQuali[3] << "]  " << std::endl;                                                                                                                                                                               //法向量
    }
    else{
        std::cout<<"点云处理失败"<<std::endl;
        return;
    }

    std::cout << "Quater:  [  " << Quater1[0] << ",   " << Quater1[1] << ",   " << Quater1[2] << ",   " << Quater1[3] << "]  " << std::endl; //四元数
    std::cout << "centerPoint:  [  " << centerPoint1[0] << ",   " << centerPoint1[1] << ",   " << centerPoint1[2] << "] " << std::endl;      //中心坐标

    visionData.robotpose.push_back(centerPoint1[0]);
    visionData.robotpose.push_back(centerPoint1[1]);
    visionData.robotpose.push_back(centerPoint1[2]); //TODO： 
    visionData.robotpose.push_back(Quater1[0]);
    visionData.robotpose.push_back(Quater1[1]);
    visionData.robotpose.push_back(Quater1[2]);
    visionData.robotpose.push_back(Quater1[3]);
    visionData.robotpose.push_back(MedianHeight);

}
