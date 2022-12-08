#include "VWSCamera.h"
#include <iostream>
#include <regex>
#include "Include/Mv3dRgbdImgProc.h"
//#include "Include/RenderImage.hpp"
#include <thread>

void __stdcall  ExceptionCallBackFunc(MV3D_RGBD_EXCEPTION_INFO* pstExceptionData, void* userdata)
{
    if (NULL != pstExceptionData)
    {
        if (DevException_Disconnect == pstExceptionData->enExceptionId)
        {
            *(int*)userdata = -1;
            LOGE("设备断开！！  %s",pstExceptionData->chExceptionDes);
        }
    }
}

void __stdcall GetImageCallBackFunc(MV3D_RGBD_FRAME_DATA* pstFrameData, void* pUser) {
    VWSCamera::ImageData data;
    ((VWSCamera*)pUser)->imageProcess(pstFrameData, data);
    ((VWSCamera*)pUser)->func_(data, ((VWSCamera*)pUser)->pUser_);
}

VWSCamera::VWSCamera(/* args */) {
}

VWSCamera::~VWSCamera() {
    if(handle_){
        disConnect();
    }
}

int VWSCamera::Init(std::string ip, int port) {
    unsigned int nDevNum = 0;
    int nRet = MV3D_RGBD_OK;
    nRet = MV3D_RGBD_Initialize();
    if (nRet != MV3D_RGBD_OK) {
        LOGE("MV3D_RGBD_GetDeviceNumber failed: error %#x at %s:%d", nRet, __FILE__, __LINE__);
        return -1;
    }
    nRet = MV3D_RGBD_GetDeviceNumber(DeviceType_Ethernet, &nDevNum);
    if (nRet != MV3D_RGBD_OK) {
        LOGE("MV3D_RGBD_GetDeviceNumber failed: error %#x at %s:%d", nRet, __FILE__, __LINE__);
        return -1;
    }

    std::vector<MV3D_RGBD_DEVICE_INFO> devs(nDevNum);
    nRet = MV3D_RGBD_GetDeviceList(DeviceType_Ethernet, &devs[0], nDevNum, &nDevNum);
    if (nRet != MV3D_RGBD_OK || nDevNum <= 0) {
        LOGE("MV3D_RGBD_GetDeviceList failed: error %#x at %s:%d", nRet, __FILE__, __LINE__);
        return -1;
    }
    for (unsigned int i = 0; i < nDevNum; i++) {
        if (strcmp(devs[i].SpecialInfo.stNetInfo.chCurrentIp, ip.c_str()) == 0) {
            devInfo_ = devs[i];
            this->ip_ = ip;
            return 1;
        }
    }
    LOGE("IP failed: error %#x at %s:%d", nRet, __FILE__, __LINE__);
    return -1;
}
int VWSCamera::connect() {
    int nRet;
    nRet = MV3D_RGBD_OpenDevice(&handle_,&devInfo_);
    if(nRet != MV3D_RGBD_OK){
        LOGE("MV3D_RGBD_OpenDevice failed: error %#x at %s:%d", nRet, __FILE__, __LINE__);
        return -1;
    }
    nRet = MV3D_RGBD_RegisterFrameCallBack(handle_, GetImageCallBackFunc, this);
    if(nRet != MV3D_RGBD_OK){
        LOGE("RegisterFrameCallBack failed: error %#x at %s:%d", nRet, __FILE__, __LINE__);
        return -1;
    }
    nRet = MV3D_RGBD_RegisterExceptionCallBack(handle_, ExceptionCallBackFunc, &netState_);
    if(nRet != MV3D_RGBD_OK){
        LOGE("MV3D_RGBD_RegisterExceptionCallBack failed: error %#x at %s:%d", nRet, __FILE__, __LINE__);
        return -1;
    }
    nRet = MV3D_RGBD_Start(handle_);
    if(nRet != MV3D_RGBD_OK){
        LOGE("MV3D_RGBD_Start failed: error %#x at %s:%d", nRet, __FILE__, __LINE__);
        return -1;
    }
    netState_ = 1;
    return 1;
}
int VWSCamera::disConnect() {
    int nRet = MV3D_RGBD_OK;
    nRet = MV3D_RGBD_Stop(handle_);
    if(nRet != MV3D_RGBD_OK){
        LOGE("MV3D_RGBD_Stop failed: error %#x at %s:%d", nRet, __FILE__, __LINE__);
        return -1;
    }
    nRet = MV3D_RGBD_CloseDevice(&handle_);
    if(nRet != MV3D_RGBD_OK){
        LOGE("MV3D_RGBD_CloseDevice failed: error %#x at %s:%d", nRet, __FILE__, __LINE__);
        return -1;
    }
    nRet = MV3D_RGBD_Release();
    if(nRet != MV3D_RGBD_OK){
        LOGE("MV3D_RGBD_Release failed: error %#x at %s:%d", nRet, __FILE__, __LINE__);
        return -1;
    }
    handle_ = NULL;
    netState_ = 0;
    return 1;
}
// int VWSCamera::getImage(VWSCamera::ImageData & data) {
//     nRet = MV3D_RGBD_Stop(handle_);
//     if(nRet != MV3D_RGBD_OK){
//         LOGE("MV3D_RGBD_Stop failed: error %#x at %s:%d", nRet, __FILE__, __LINE__);
//         return -1;
//     }
//     changeTriggerMode(false);
//     nRet = MV3D_RGBD_Start(handle_);
//     if(nRet != MV3D_RGBD_OK){
//         LOGE("MV3D_RGBD_Stop failed: error %#x at %s:%d", nRet, __FILE__, __LINE__);
//         return -1;
//     }

//     MV3D_RGBD_FRAME_DATA stFrameData = {0};
//     // while (MV3D_RGBD_FetchFrame(handle_, &stFrameData, 5000) !=MV3D_RGBD_OK )
//     // {
//     //     LOGE("MV3D_RGBD_FetchFrame failed: error %#x at %s:%d", 0, __FILE__, __LINE__);
//     //     /* code */
//     // }
//     int nRet = MV3D_RGBD_FetchFrame(handle_, &stFrameData, 5000);
//     if(nRet != MV3D_RGBD_OK){
//         LOGE("MV3D_RGBD_FetchFrame failed: error %#x at %s:%d", nRet, __FILE__, __LINE__);
//         //changeTriggerMode(true);
//         return -1;
//     }
//     int nRet = MV3D_RGBD_OK;
//     if (MV3D_RGBD_OK == nRet) {
//         imageProcess(&stFrameData,data);
//     }
//     if(nRet != MV3D_RGBD_OK){
//         LOGE("MV3D_RGBD_MapDepthToPointCloud failed: error %#x at %s:%d", nRet, __FILE__, __LINE__);
//         //changeTriggerMode(true);
//         return -1;
//     }
//         nRet = MV3D_RGBD_Stop(handle_);
//     if(nRet != MV3D_RGBD_OK){
//         LOGE("MV3D_RGBD_Stop failed: error %#x at %s:%d", nRet, __FILE__, __LINE__);
//         return -1;
//     }
//     changeTriggerMode(true);
//     nRet = MV3D_RGBD_Start(handle_);
//     if(nRet != MV3D_RGBD_OK){
//         LOGE("MV3D_RGBD_Stop failed: error %#x at %s:%d", nRet, __FILE__, __LINE__);
//         return -1;
//     }
//     //changeTriggerMode(true);
//     return 1;
// }
int VWSCamera::state() {
    return netState_;
}

void VWSCamera::imageProcess(MV3D_RGBD_FRAME_DATA *stFrameData,VWSCamera::ImageData &data){
    for (int i = 0; i < stFrameData->nImageCount; i++) {
        if (ImageType_Depth == stFrameData->stImageData[i].enImageType) {
            MV3D_RGBD_IMAGE_DATA stPointCloudImage;
            int nRet = MV3D_RGBD_MapDepthToPointCloud(handle_, &stFrameData->stImageData[i], &stPointCloudImage);
            if (MV3D_RGBD_OK != nRet) {
                break;
            }
            uint8_t* PointCloudImage = new uint8_t[stPointCloudImage.nDataLen];
            memcpy(PointCloudImage,stPointCloudImage.pData,stPointCloudImage.nDataLen);
            stPointCloudImage.pData = PointCloudImage;
            data.PointCloudImage = stPointCloudImage;
        }
        if (ImageType_RGB8_Planar == stFrameData->stImageData[i].enImageType) {
            int len = stFrameData->stImageData[i].nDataLen;
            uint8_t* pdata = stFrameData->stImageData[i].pData;
            uint8_t* RGBImage = new uint8_t[len];
            for (int i = 0; i < len / 3; ++i) {
                RGBImage[i * 3 + 0] = pdata[i];
                RGBImage[i * 3 + 1] = pdata[len / 3 + i];
                RGBImage[i * 3 + 2] = pdata[(len / 3) * 2 + i];
            }
            stFrameData->stImageData[i].pData = RGBImage;
            data.RGB8PlanarImage = stFrameData->stImageData[i];
        }
    }
}

int VWSCamera::RegisterFrameCallBack(GetImageCallBack func, void* pUser){
    func_ = func;
    pUser_ = pUser;
    return 0;
}

int VWSCamera::deleteImage(const VWSCamera::ImageData &data){
    delete []data.PointCloudImage.pData;
    delete []data.RGB8PlanarImage.pData;
}

int VWSCamera::softTrigger(){
    changeTriggerSource(false);
    int nRet = MV3D_RGBD_SoftTrigger(handle_);
    if(nRet != MV3D_RGBD_OK){
        LOGE("MV3D_RGBD_SoftTrigger failed: error %#x at %s:%d", nRet, __FILE__, __LINE__);
        return -1;
    }
    changeTriggerSource(true);
}

int VWSCamera::changeTriggerSource(bool flag){
    MV3D_RGBD_PARAM para;
    if (flag)
        para.ParamInfo.stEnumParam.nCurValue = 0;
    else
        para.ParamInfo.stEnumParam.nCurValue = 7;
    para.enParamType = Mv3dRgbdParamType::ParamType_Enum;
    int nRet = MV3D_RGBD_SetParam(handle_,MV3D_RGBD_ENUM_TRIGGERSOURCE,&para);
    if(nRet != MV3D_RGBD_OK){
        LOGE("MV3D_RGBD_SetParam failed: error %#x at %s:%d", nRet, __FILE__, __LINE__);
        return -1;
    }
    return 1;
}