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

_API_ int getRobotBeltDirection(
	const std::vector<Eigen::Vector3f> points,
	const std::vector<float> beltPositions,
	Eigen::Vector3f& beltDirection);

_API_ int getCameraBeltDirection(
	const std::vector<cameraBeltDirectionCaliData>& datas,
	size_t plateWidth,
	size_t plateHeight,
	float gridSize,
	Eigen::Vector3f& beltDirection);

_API_ int getExtraAxisDirection(
	const std::vector<Eigen::Vector3f> points,
	const std::vector<float> positions,
	Eigen::Vector3f& extraAxisDirection);

_API_ int getStationTransformMatrix(
	const std::vector<Eigen::Vector3f> src,
	const std::vector<Eigen::Vector3f> dst,
	Eigen::Isometry3f& transf);

_API_ int getSensorRotationMatrix(
	const cameraSensorCaliData& data,
	size_t plateWidth,
	size_t plateHeight,
	float gridSize,
	Eigen::Matrix3f& rotation);

_API_ int getSensorDistance(float s1, float s2, float delta, float& dist);