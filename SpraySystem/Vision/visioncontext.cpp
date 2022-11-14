#include "visioncontext.h"
//#include "halconcpp/HalconCpp.h"
//#include "halconcpp/HDevThread.h"

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

void VisionContext::work(ImageData data, Eigen::Isometry3d handEyeMatrix, vws::VisionData & visionData)
{
    //getPoseAndHeight(data,visionData);
    //RobotCenterPose(visionData, handEyeMatrix);
    double handEyeMatri[12];
    auto handEyeArry = handEyeMatrix.matrix().data();
    //RobotCenterPose(visionData,handEyeArry);
    visionProcessFunc(data, visionData);
}

void VisionContext::work_head(ImageData data, vws::VisionData & VisionData)
{   
     
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
     std::string Result = 0;
    hv_ObjectModel3D = ImageConver(data);
      PCLlibs::HeightAndPoseVector(hv_CamParam, hv_SetParas, hv_ObjectModel3D, MedianHeight, MedianWidth, VectorPosition, IntersecPonitUR,
                        IntersecPonitUL, IntersecPonitDR, IntersecPonitDL, LineQuali, Result);

    double v[3] = {VectorPosition[0], VectorPosition[1], VectorPosition[2]};
    visionData.normalvector = v;
}
void VisionContext:: getWidth(std::vector<float> senorNums, float senorDistance,double * normalvector)
{
    double sensorDis1 = senorNums[0];
    double sensorDis2 = senorNums[1];
    double holeDis;     //TODO： 标定工装孔据
    double matRot[9];  //TODO： 传感器标定矩阵, 方向向量

    double depth; //输出结果
    // PCLlibs::CalBoxDeepth(senorDistance,sensorDis1,sensorDis2,holeDis,matRot,&depth);
}

void VisionContext::getLenght(std::vector<float> encoderVector, std::vector<float> encoderNums, VisionData &visionData)
{
    double encoderHead = encoderNums[0];
    double encoderTrail = encoderNums[1];
    // PCLlibs::CalBoxLength(encoderVector,encoderTrail,encoderHead,)
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

    // HTuple hv_CamParam, hv_ObjectModel3D, hv_Status;
    // double hv_Result, hv_MedianHeight, hv_MedianWidth;
    // double IntersecPonitUL[3], IntersecPonitDR[3], IntersecPonitDL[3], IntersecPonitUR[3];
    // // double LineQuali, VectorPosition, MedianHeight, MedianWidth;
    // double LineQuali[4];
    // double VectorPosition[9];
    // double MedianHeight, MedianWidth;

    // int Result = 0;
    // hv_CamParam.Clear();
    // hv_CamParam[0] = "area_scan_division";
    // hv_CamParam[1] = 0.06;
    // hv_CamParam[2] = 0;
    // hv_CamParam[3] = 8.5e-06;
    // hv_CamParam[4] = 8.5e-06;
    // hv_CamParam[5] = 704;
    // hv_CamParam[6] = 512;
    // hv_CamParam[7] = 1408;
    // hv_CamParam[8] = 1024;
    // //*��ȡ��������
    // // ReadObjectModel3d("", "m", HTuple(), HTuple(), &hv_ObjectModel3D, &hv_Status);
    // double hv_SetParas[6] = {900, 1720, -1220, 880, 6, 1e5};

    // int width = data.PointCloudImage.nWidth;
    // int height = data.PointCloudImage.nHeight;
    // int count = width * height;
    // float *cloudData = (float*)(data.PointCloudImage.pData);
    // std::vector<float> px, py, pz;
    // for (int i = 0; i < count; i++)
    // {
    //     px.push_back(cloudData[i*3]);
    //     py.push_back(cloudData[i*3+1]);
    //     pz.push_back(cloudData[i*3+2]);
    // }

    // HImage ds;
    // HImage imgx("real", width, height, (void *)(px.data()));
    // HImage imgy("real", width, height, (void *)py.data());
    // HImage imgz("real", width, height, (void *)pz.data());

    // // HTuple obj3d;
    // XyzToObjectModel3d(imgx, imgy, imgz, &hv_ObjectModel3D);

    // // ReadObjectModel3d(obj3d, "m", HTuple(), HTuple(), &hv_ObjectModel3D, &hv_Status);

    // PCLlibs::HeightAndPoseVector(hv_CamParam, hv_SetParas, hv_ObjectModel3D, MedianHeight, MedianWidth, VectorPosition, IntersecPonitUR,
    //                     IntersecPonitUL, IntersecPonitDR, IntersecPonitDL, LineQuali, &Result); ////

    // double VectorX[3] = {VectorPosition[0], VectorPosition[1], VectorPosition[2]};
    // double VectorY[3] = {VectorPosition[3], VectorPosition[4], VectorPosition[5]};
    // double VectorZ[3] = {VectorPosition[6], VectorPosition[7], VectorPosition[8]};
    // double RightUpPoint[3] = {IntersecPonitUR[0], IntersecPonitUR[1], IntersecPonitUR[2]};
    // double Quater[4];
    // double RightUpPoint1[3];
    // // double boxSize[3] = {MedianHeight, 805, 535};
    //    double boxSize[3] = {800, 500, 535};
 
    // double Quater1[4];
    // double centerPoint1[3];
    // double handEyeMatrix[12]={
    //     -0.03743671625852585,
    //     -0.001139160362072289,
    //     -0.9992983341217041,
    //     2654.87060546875,
    //     0.009748989716172219,
    //     -0.9999521374702454,
    //     0.0007746600895188749,
    //     -1332.0638427734376,
    //     -0.9992514252662659,
    //     -0.009713122621178627,
    //     0.03744605928659439,
    //     481.78564453125
    // };
    // // double CalibPose[12] = {-0.04470646008849144,    0.024665797129273416,    -0.9986956119537354,
    // //                                                     2662.4287109375,             -0.013045716099441052,   -0.9996243119239808,
    // //                                                    -0.02410474978387356,   -2946.929443359375,         -0.9989149570465088,
    // //                                                    0.011951121501624585,   0.045011427253484729,     466.8827819824219};
    // PCLlibs::CalcuCameraRobot(VectorX, VectorY, VectorZ, RightUpPoint, boxSize, handEyeMatrix, Quater1, centerPoint1);

    // // double BoxSize0[3]={0, 0, 1070};
    // // PCLlibs::CalcuCameraRobot(VectorX, VectorY, VectorZ, RightUpPoint, BoxSize0, handEyeMatrix, Quater, RightUpPoint1);

    // std::cout << "VectorUnit:  [  " << VectorPosition[0] << ",   " << VectorPosition[1] << ",   " << VectorPosition[2] << ",   " << VectorPosition[3] << ",   " << VectorPosition[4] << ",   " << VectorPosition[5] << ",   " << VectorPosition[6] << ",   " << VectorPosition[7] << ",   " << VectorPosition[8] << "]  " << std::endl; //左侧棱长向量
    // std::cout << "LineQuality:  [  " << LineQuali[0] << ",   " << LineQuali[1] << ",   " << LineQuali[2] << ",   " << LineQuali[3] << "]  " << std::endl;                                                                                                                                                                               //法向量

    // std::cout << "Quater:  [  " << Quater1[0] << ",   " << Quater1[1] << ",   " << Quater1[2] << ",   " << Quater1[3] << "]  " << std::endl; //四元数
    // std::cout << "centerPoint:  [  " << centerPoint1[0] << ",   " << centerPoint1[1] << ",   " << centerPoint1[2] << "] " << std::endl;       //中心坐标
    

    // visionData.robotpose.push_back(centerPoint1[0]);
    // visionData.robotpose.push_back(centerPoint1[1]);
    // visionData.robotpose.push_back(centerPoint1[2]);
    // visionData.robotpose.push_back(Quater1[0]);
    // visionData.robotpose.push_back(Quater1[1]);
    // visionData.robotpose.push_back(Quater1[2]);
    // visionData.robotpose.push_back(Quater1[3]);
    // visionData.robotpose.push_back(MedianHeight);

    // // std::cout << "Quater:  [  " << Quater[0] << ",   " << Quater[1] << ",   " << Quater[2] << ",   " << Quater[3] << "]  " << std::endl; //四元数
    // // std::cout << "RightUpPoint1:  [  " << RightUpPoint1[0] << ",   " << RightUpPoint1[1] << ",   " << RightUpPoint1[2] << "] " << std::endl;       //中心坐标
   
    // // double * buf = new double[8];
    // // buf[0] = centerPoint1[0];
    // // buf[1] = centerPoint1[1];
    // // buf[2] = centerPoint1[2];
    // // buf[3] = Quater1[0];
    // // buf[4] = Quater1[1];
    // // buf[5] = Quater1[2];
    // // buf[6] = Quater1[3];
    // // buf[7] = MedianHeight;

    // //write(connect_fd, (void *)buf, sizeof(double)*7);
    // // write(serverFD, (void *)buf, sizeof(double)*8);
    // // delete []buf;
    
}

// void visionProcessFunc(const VWSCamera::ImageData &data, vws::VisionData &visionData)
// {

//     HTuple hv_CamParam, hv_ObjectModel3D, hv_Status;
//     double hv_Result, hv_MedianHeight, hv_MedianWidth;
//     double IntersecPonitUL[3], IntersecPonitDR[3], IntersecPonitDL[3], IntersecPonitUR[3];
//     double LineQuali[4];
//     double VectorPosition[9];
//     double MedianHeight, MedianWidth;

//     std::string Result = 0;
//     hv_CamParam.Clear();
//     hv_CamParam[0] = "area_scan_division";
//     hv_CamParam[1] = 0.06;
//     hv_CamParam[2] = 0;
//     hv_CamParam[3] = 8.5e-06;
//     hv_CamParam[4] = 8.5e-06;
//     hv_CamParam[5] = 704;
//     hv_CamParam[6] = 512;
//     hv_CamParam[7] = 1408;
//     hv_CamParam[8] = 1024;
//     double hv_SetParas[6] = {900, 1720, -1220, 880, 6, 1e5};


//     int width = data.PointCloudImage.nWidth;
//     int height = data.PointCloudImage.nHeight;
//     int count = width * height;
//     float *cloudData = (float *)(data.PointCloudImage.pData);
//     std::vector<float> px, py, pz;
//     for (int i = 0; i < count; i++)
//     {
//         px.push_back(cloudData[i * 3]);
//         py.push_back(cloudData[i * 3 + 1]);
//         pz.push_back(cloudData[i * 3 + 2]);
//     }

//     HImage imgx("real", width, height, (void *)px.data());
//     HImage imgy("real", width, height, (void *)py.data());
//     HImage imgz("real", width, height, (void *)pz.data());

//     XyzToObjectModel3d(imgx, imgy, imgz, &hv_ObjectModel3D);


//     PCLlibs::HeightAndPoseVector(hv_CamParam, hv_SetParas, hv_ObjectModel3D, MedianHeight, MedianWidth, VectorPosition, IntersecPonitUR,
//                                  IntersecPonitUL, IntersecPonitDR, IntersecPonitDL, LineQuali, Result); ////

//     double Quater[4];
//     double RightUpPoint1[3];
//     double boxSize[3] = {MedianHeight, 805, 535};

//     double Quater1[4] = {
//         0,
//         0,
//         0,
//         0,
//     };
//     double centerPoint1[3] = {0, 0, 0};
//     double handEyeMatrix[12] = {-0.03743671625852585,
//                                 -0.001139160362072289,
//                                 -0.9992983341217041,
//                                 2654.87060546875,
//                                 0.009748989716172219,
//                                 -0.9999521374702454,
//                                 0.0007746600895188749,
//                                 -1332.0638427734376,
//                                 -0.9992514252662659,
//                                 -0.009713122621178627,
//                                 0.03744605928659439,
//                                 481.78564453125};
//     if (Result == "1, 运行成功")
//     {
//         PCLlibs::CalcuCameraRobot(VectorPosition, IntersecPonitUR, boxSize, handEyeMatrix, Quater1, centerPoint1);

//         std::cout << "VectorUnit:  [  " << VectorPosition[0] << ",   " << VectorPosition[1] << ",   " << VectorPosition[2] << ",   " << VectorPosition[3] << ",   " << VectorPosition[4] << ",   " << VectorPosition[5] << ",   " << VectorPosition[6] << ",   " << VectorPosition[7] << ",   " << VectorPosition[8] << "]  " << std::endl; //左侧棱长向量
//         std::cout << "LineQuality:  [  " << LineQuali[0] << ",   " << LineQuali[1] << ",   " << LineQuali[2] << ",   " << LineQuali[3] << "]  " << std::endl;                                                                                                                                                                               //法向量
//     }

//     std::cout << "Quater:  [  " << Quater1[0] << ",   " << Quater1[1] << ",   " << Quater1[2] << ",   " << Quater1[3] << "]  " << std::endl; //四元数
//     std::cout << "centerPoint:  [  " << centerPoint1[0] << ",   " << centerPoint1[1] << ",   " << centerPoint1[2] << "] " << std::endl;      //中心坐标

//     visionData.robotpose.push_back(centerPoint1[0]);
//     visionData.robotpose.push_back(centerPoint1[1]);
//     visionData.robotpose.push_back(centerPoint1[2]);
//     visionData.robotpose.push_back(Quater1[0]);
//     visionData.robotpose.push_back(Quater1[1]);
//     visionData.robotpose.push_back(Quater1[2]);
//     visionData.robotpose.push_back(Quater1[3]);
//     visionData.robotpose.push_back(MedianHeight);

// }

void VisionContext::RobotCenterPose(vws::VisionData &visionData, double handEyeMatrix[12])
{
//     double LineQuali[4];
//     double VectorPosition[9];
//     double MedianHeight, MedianWidth;

//     double VectorX[3] = {VectorPosition[0], VectorPosition[1], VectorPosition[2]};
//     double VectorY[3] = {VectorPosition[3], VectorPosition[4], VectorPosition[5]};
//     double VectorZ[3] = {VectorPosition[6], VectorPosition[7], VectorPosition[8]};
//     double RightUpPoint[3] = {visionData.righttop[0], visionData.righttop[1], visionData.righttop[2]};
//     double Quater[4];
//     double RightUpPoint1[3];
//        double boxSize[3] = {785, 805, 535};

//     double Quater1[4];
//     double centerPoint1[3];
//    double handEyeMatrix1[12]={
//        -0.03743671625852585,
//        -0.001139160362072289,
//        -0.9992983341217041,
//        2654.87060546875,
//        0.009748989716172219,
//        -0.9999521374702454,
//        0.0007746600895188749,
//        -1332.0638427734376,
//        -0.9992514252662659,
//        -0.009713122621178627,
//        0.03744605928659439,
//        481.78564453125
//    };
//     // double CalibPose[12] = {-0.04470646008849144,    0.024665797129273416,    -0.9986956119537354,
//     //                                                     2662.4287109375,             -0.013045716099441052,   -0.9996243119239808,
//     //                                                    -0.02410474978387356,   -2946.929443359375,         -0.9989149570465088,
//     //                                                    0.011951121501624585,   0.045011427253484729,     466.8827819824219};
//     PCLlibs::CalcuCameraRobot(VectorX, VectorY, VectorZ, RightUpPoint, boxSize, handEyeMatrix1, Quater1, centerPoint1);

//     // double BoxSize0[3]={0, 0, 1070};
//     // PCLlibs::CalcuCameraRobot(VectorX, VectorY, VectorZ, RightUpPoint, BoxSize0, handEyeMatrix1, Quater, RightUpPoint1);

//     std::cout << "VectorUnit:  [  " << VectorPosition[0] << ",   " << VectorPosition[1] << ",   " << VectorPosition[2] << ",   " << VectorPosition[3] << ",   " << VectorPosition[4] << ",   " << VectorPosition[5] << ",   " << VectorPosition[6] << ",   " << VectorPosition[7] << ",   " << VectorPosition[8] << "]  " << std::endl; //左侧棱长向量
//     std::cout << "LineQuality:  [  " << LineQuali[0] << ",   " << LineQuali[1] << ",   " << LineQuali[2] << ",   " << LineQuali[3] << "]  " << std::endl;                                                                                                                                                                               //法向量

//     std::cout << "Quater:  [  " << Quater1[0] << ",   " << Quater1[1] << ",   " << Quater1[2] << ",   " << Quater1[3] << "]  " << std::endl; //四元数
//     std::cout << "centerPoint:  [  " << centerPoint1[0] << ",   " << centerPoint1[1] << ",   " << centerPoint1[2] << "] " << std::endl;       //中心坐标

//     // std::cout << "Quater:  [  " << Quater[0] << ",   " << Quater[1] << ",   " << Quater[2] << ",   " << Quater[3] << "]  " << std::endl; //四元数
//     // std::cout << "RightUpPoint1:  [  " << RightUpPoint1[0] << ",   " << RightUpPoint1[1] << ",   " << RightUpPoint1[2] << "] " << std::endl;       //中心坐标


//     visionData.robotpose.push_back(centerPoint1[0]);
//     visionData.robotpose.push_back(centerPoint1[1]);
//     visionData.robotpose.push_back(centerPoint1[2]);
//     visionData.robotpose.push_back(Quater1[0]);
//     visionData.robotpose.push_back(Quater1[1]);
//     visionData.robotpose.push_back(Quater1[2]);
//     visionData.robotpose.push_back(Quater1[3]);
//     visionData.robotpose.push_back(MedianHeight);
}
