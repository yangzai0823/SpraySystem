//函数的声明
// #ifndef _HeightAndPose_h
// #define _HeightAndPose_h
#ifndef _SIZEANDPOSE_H
#define _SIZEANDPOSE_H
#ifndef __APPLE__
#include "halconcpp/HalconCpp.h"

#include "halconcpp/HDevThread.h"
#else
#ifndef HC_LARGE_IMAGES
#include <HALCONCpp/HalconCpp.h>
#include <HALCONCpp/HDevThread.h>
#else
#include <HALCONCppxl/HalconCpp.h>
#include <HALCONCppxl/HDevThread.h>
#endif
#endif
#include <iostream>
// #include <Eigen/Dense>
using namespace HalconCpp;
// using namespace Eigen;
using namespace std;

namespace PCLlibs
{

    /// @brief  点云处理计算箱体高度、箱体侧面四个角点坐标和相机坐标系
    /// @param hv_CamParam 相机参数，一般默认输入值
    /// @param hv_SetParas 设置参数数组，设置顺序为：Z轴最小参数、Z轴最大参数、X轴最小参数、X轴最大参数、点云最小距离、目标区域的最小点云数量
    /// @param hv_ObjectModel3D 输入点云
    /// @param MedianHeight 目标高度结果
    /// @param MedianWidth 目标宽度结果
    /// @param VectorPosition 坐标系三个方向单位向量数组，顺序为：X轴方向向量3个参数(箱体表面法向量)、Y轴方向向量3个参数、Z轴方向向量3个参数、
    /// @param IntersecPonitUR 箱体侧面右上角坐标
    /// @param IntersecPonitUL 箱体侧面左上角坐标
    /// @param IntersecPonitDR 箱体侧面右下角坐标
    /// @param IntersecPonitDL 箱体侧面左下角坐标
    /// @param LineQuali 四条边直线质量的方差结果：上侧边、下侧边、右侧边、左侧边
    /// @param Result 是否运行成功
    void HeightAndPoseVector(HTuple hv_CamParam, double hv_SetParas[6], HTuple hv_ObjectModel3D, double &MedianHeight, double &MedianWidth, double *VectorPosition, double *IntersecPonitUR,
                             double *IntersecPonitUL, double *IntersecPonitDR, double *IntersecPonitDL, double *LineQuali, int *Result);

    /// @brief 箱体长度测量算法
    /// @param Vector 编码器标定方向
    /// @param s2 箱体尾端触发拍照时的编码器值
    /// @param s1 箱体首端触发拍照时的编码器值
    /// @param LeftUpPoint 左上角坐标
    /// @param LeftDownPoint 左下角坐标
    /// @param RightUpPoint 右上角坐标
    /// @param RightDownPoint 右下角坐标
    /// @param boxLength 长度测量结果
    void CalBoxLength(double Vector[3], double s2, double s1, double LeftUpPoint[3],
                      double LeftDownPoint[3], double RightUpPoint[3], double RightDownPoint[3], double &boxLength);

    /// @brief 根据标定矩阵计算箱体在机械手下的姿态
    /// @param VectorX 输入箱体在相机下X轴单位向量
    /// @param VectorY 输入箱体在相机下Y轴单位向量
    /// @param VectorZ 输入箱体在相机下Z轴单位向量
    /// @param RightUpPoint 箱体右上角坐标
    /// @param boxSize 箱体尺寸：高度、长度、深度
    /// @param VectorX1 相机和机械手的标定矩阵：X轴方向向量
    /// @param VectorY1 相机和机械手的标定矩阵：Y轴方向向量
    /// @param VectorZ1 相机和机械手的标定矩阵：Z轴方向向量
    /// @param Quater 输出箱体在机械手下的四元数
    /// @param CenterPoint 输出箱体中心坐标

    /// @brief 根据标定矩阵计算箱体在机械手下的姿态
    /// @param VectorX 输入箱体在相机下X轴单位向量
    /// @param VectorY 输入箱体在相机下Y轴单位向量
    /// @param VectorZ 输入箱体在相机下Z轴单位向量
    /// @param RightUpPoint 箱体右上角坐标
    /// @param boxSize 箱体尺寸：高度、长度、深度
    /// @param VectorX1 相机和机械手的标定矩阵：X轴方向向量
    /// @param VectorY1 相机和机械手的标定矩阵：Y轴方向向量
    /// @param VectorZ1 相机和机械手的标定矩阵：Z轴方向向量
    /// @param CalibPoint  相机和机械手的标定矩阵：标定矩阵偏移量
    /// @param Quater 输出箱体在机械手下的四元数
    /// @param CenterPoint 输出箱体中心坐标
    void CalcuCameraRobot(double VectorX[3], double VectorY[3], double VectorZ[3], double RightUpPoint[3],
                          double boxSize[3], double VectorX1[3], double VectorY1[3], double VectorZ1[3], double CalibPoint[3], double *Quater, double *CenterPoint);


    /// @brief 根据标定矩阵计算箱体在机械手下的姿态
    /// @param VectorX 输入箱体在相机下X轴单位向量
    /// @param VectorY 输入箱体在相机下Y轴单位向量
    /// @param VectorZ 输入箱体在相机下Z轴单位向量
    /// @param RightUpPoint 箱体右上角坐标
    /// @param boxSize 箱体尺寸：高度、长度、深度
    /// @param CalibPose 相机和机械手的标定矩阵
    /// @param Quater 输出箱体在机械手下的四元数
    /// @param CenterPoint 输出箱体中心坐标
    void CalcuCameraRobot(double VectorX[3], double VectorY[3], double VectorZ[3], double RightUpPoint[3],
                          double boxSize[3], double CalibPose[12], double *Quater, double *CenterPoint);


     /// @brief 测量箱体深度
    /// @param SensorDis  一对传感器的距离值
    /// @param SensorDis1  实际测量时，传感器1的测量值
    /// @param SensorDis2  实际测量时，传感器2的测量值
    /// @param holeDis   标定工装孔距
    /// @param MatRot  标定矩阵
    /// @param NormalVec  点云法向量
    /// @param deepth 输出箱体深度
    void CalBoxDeepth(double SensorDis, double SensorDis1, double SensorDis2, double holeDis, double MatRot[9], double NormalVec[3], double &deepth);

 
}

#endif
