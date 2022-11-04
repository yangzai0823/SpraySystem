#ifndef CAMERAOPERATOR_H
#define CAMERAOPERATOR_H

#include <QObject>
#include <memory>

#include "Device/deviceoperator.h"
#include "DataAccess/Models/Camera/camera.h"
#include "VWSCamera/VWSCamera.h"

using ImageData = VWSCamera::ImageData;
using GetImageCallBack = VWSCamera::GetImageCallBack;


class CameraOperator : public DeviceOperator
{
public:
    CameraOperator(std::shared_ptr<Camera> camera);
    ~CameraOperator();
    int init();
    int start();
    void close();
    int getState();
    //int getImage(ImageData & data);
    int RegisterFrameCallBack(GetImageCallBack func, void* pUser);
    int deleteImage(const ImageData &data);

private:
    std::shared_ptr<VWSCamera> vwsCamera;
};

#endif // CAMERAOPERATOR_H
