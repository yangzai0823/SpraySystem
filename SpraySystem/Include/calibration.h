#pragma once
#ifdef __GNUC__
#ifdef _EXPORT_
#define _API_ __attribute__((visibility("default")))
#else
#define _API_
#endif
#elif _MSC_VER
#ifdef _EXPORT_
#define _API_ __declspec(dllexport)
#else
#define _API_ __declspec(dllimport)
#endif
#endif

// #include <Eigen/Core>
#include <Eigen/Core>
#include <Eigen/Geometry>
#include <vector>
#include "calibrationdatas.h"

_API_ int getHandEyeMatrix(
	const std::vector<handEyeCaliData>& datas,
	size_t plateWidth,
	size_t plateHeight,
	float gridSize,
	const Eigen::Vector3f& beltDir,
	Eigen::Isometry3f& matrix);

_API_ int getMarkedGridPlateImage(
	size_t w, size_t h, void* srcBuff, size_t plateWidth, size_t plateHeight, void** dstBuff);

_API_ int getRobotBeltDirection(
	const std::vector<Eigen::Vector3f> points,
	const std::vector<float> beltPositions,
	Eigen::Vector3f& beltDirection);

_API_ int getExtraAxisDirection(
	const std::vector<Eigen::Vector3f> points,
	const std::vector<float> positions,
	Eigen::Vector3f& extraAxisDirection);

_API_ int getStationTransformMatrix(
	const std::vector<Eigen::Vector3f> src,
	const std::vector<Eigen::Vector3f> dst,
	Eigen::Isometry3f& transf);

_API_ int getLaserMarkerImage(size_t width, size_t height, void* rgb, void** markedImagePtr);

_API_ int getSensorOriginDirection(
	const std::vector<cameraSensorCaliData>& datas,
	Eigen::Vector3f& origin,
	Eigen::Vector3f& direction);
