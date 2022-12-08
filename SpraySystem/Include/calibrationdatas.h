#pragma once
#include <Eigen/Core>
#include <vector>

struct handEyeCaliData {
	size_t width;
	size_t height;
	void* rgb;
	void* xyz;
	float pos1;
	float pos2;
	Eigen::Vector3f p1;
	Eigen::Vector3f p2;
	Eigen::Vector3f p3;
};

struct cameraBeltDirectionCaliData {
	size_t width;
	size_t height;
	void* rgb;
	void* xyz;
	float pos;
};

struct pointCloudData {
	size_t width;
	size_t height;
	void* xyz;
};

struct cameraSensorCaliData {
	size_t width;
	size_t height;
	void* rgb;
	void* xyz;
	float dist;
};

struct stationCaliData {
	float beltPos1;
	float beltPos2;
	std::vector<float> extraAxisPos1;
	std::vector<float> extraAxisPos2;
	std::vector<Eigen::Vector3f> robotPos1;
	std::vector<Eigen::Vector3f> robotPos2;
};