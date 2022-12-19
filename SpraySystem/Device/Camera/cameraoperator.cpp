#include "cameraoperator.h"
#include <iostream>
#include <unistd.h>

CameraOperator::CameraOperator(std::shared_ptr<Camera> camera)
{
    this->ip = camera->Ip;
    this->port = camera->Port.toUInt();
    this->name = camera->Name;
}

CameraOperator::~CameraOperator()
{
}

int CameraOperator::init()
{
    vwsCamera = std::make_shared<VWSCamera>();
    int ret = -1;
    for (int i = 0; i < 3; i++)
    {
        ret = vwsCamera->Init(ip.toStdString(), port);
        if (ret > 0)
        {
            break;
        }
        usleep(30);
    }
    return ret;
}

int CameraOperator::start()
{
    int ret = -1;
    for (int i = 0; i < 3; i++)
    {
        ret = vwsCamera->connect();
        if (ret > 0)
        {
            break;
        }
        usleep(30);
    }
    if (ret > 0)
    {
        // 设置硬触发
        vwsCamera->changeTriggerMode(1);
        vwsCamera->changeTriggerSource(1);
    }
    return ret;
}

void CameraOperator::close()
{
    if (vwsCamera == nullptr)
        return;
    vwsCamera->disConnect();
}

int CameraOperator::getState()
{
    if (vwsCamera == nullptr)
        return -1;
    auto ret = vwsCamera->state();
    return ret;
}

int CameraOperator::startGrab()
{
    auto ret = vwsCamera->startGrab();
    return ret;
}

int CameraOperator::stopGrab()
{
    auto ret = vwsCamera->stopGrab();
    return ret;
}
int CameraOperator::saveAllParam(const char *pOutFileName)
{
    auto ret = vwsCamera->saveAllParam(pOutFileName);
    return ret;
}
int CameraOperator::loadAllParam(const char *pOutFileName)
{
    auto ret = vwsCamera->loadAllParam(pOutFileName);
    return ret;
}
int CameraOperator::RegisterFrameCallBack(GetImageCallBack func, void *pUser)
{
    // std::cout <<"reg camera ip: " << vwsCamera->ip_ << std::endl;
    return vwsCamera->RegisterFrameCallBack(func, pUser);
}

int CameraOperator::deleteImage(const ImageData &data)
{
    auto ret = vwsCamera->deleteImage(data);
    return ret;
}
