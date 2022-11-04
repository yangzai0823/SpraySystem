#include "cameraoperator.h"


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
    vwsCamera  = std::make_shared<VWSCamera>();
    int ret = vwsCamera->Init(ip.toStdString(),port);
    return ret;
}

int CameraOperator::start()
{
    auto ret = vwsCamera->connect();
    return ret;
}

void CameraOperator::close()
{
    if(vwsCamera==nullptr)
        return;
    vwsCamera->disConnect();
}

int CameraOperator::getState()
{
//    if(vwsCamera==nullptr)
//        return -1;
//   auto ret = vwsCamera->state();
//   return ret;
}

int CameraOperator::RegisterFrameCallBack(GetImageCallBack func, void *pUser)
{
    vwsCamera->RegisterFrameCallBack(func,pUser);
}

int CameraOperator::deleteImage(const ImageData &data)
{
    auto ret = vwsCamera->deleteImage(data);
    return ret;
}
