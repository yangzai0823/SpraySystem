#include "VWSCamera.h"

#include <iostream>
#include <regex>

#include "Include/Mv3dRgbdImgProc.h"
// #include "Include/RenderImage.hpp"
#include <thread>

void __stdcall ExceptionCallBackFunc(MV3D_RGBD_EXCEPTION_INFO* pstExceptionData, void* userdata) {
    if (NULL != pstExceptionData) {
        if (DevException_Disconnect == pstExceptionData->enExceptionId) {
            *(int*)userdata = -1;
            LOGE("设备断开！！  %s", pstExceptionData->chExceptionDes);
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
    if (handle_) {
        disConnect();
    }
}

int VWSCamera::Init(std::string ip, int port) {
#ifdef LOCALTEST
    return 1;
#endif
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
        std::cout << "ip   " << devs[i].SpecialInfo.stNetInfo.chCurrentIp << std::endl;
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
#ifdef LOCALTEST
    return 1;
#endif
    int nRet;
    nRet = MV3D_RGBD_OpenDevice(&handle_, &devInfo_);
    if (nRet != MV3D_RGBD_OK) {
        LOGE("MV3D_RGBD_OpenDevice failed: error %#x at %s:%d", nRet, __FILE__, __LINE__);
        return -1;
    }
    nRet = MV3D_RGBD_RegisterExceptionCallBack(handle_, ExceptionCallBackFunc, &netState_);
    if (nRet != MV3D_RGBD_OK) {
        LOGE("MV3D_RGBD_RegisterExceptionCallBack failed: error %#x at %s:%d", nRet, __FILE__, __LINE__);
        return -1;
    }
    // nRet = MV3D_RGBD_Start(handle_);
    // if(nRet != MV3D_RGBD_OK){
    //     LOGE("MV3D_RGBD_Start failed: error %#x at %s:%d", nRet, __FILE__, __LINE__);
    //     return -1;
    // }
    netState_ = 1;
    return 1;
}
int VWSCamera::disConnect() {
#ifdef LOCALTEST
    return 1;
#endif
    int nRet = MV3D_RGBD_OK;
    // nRet = MV3D_RGBD_Stop(handle_);
    // if(nRet != MV3D_RGBD_OK){
    //     LOGE("MV3D_RGBD_Stop failed: error %#x at %s:%d", nRet, __FILE__, __LINE__);
    //     return -1;
    // }
    nRet = MV3D_RGBD_CloseDevice(&handle_);
    if (nRet != MV3D_RGBD_OK) {
        LOGE("MV3D_RGBD_CloseDevice failed: error %#x at %s:%d", nRet, __FILE__, __LINE__);
        return -1;
    }
    nRet = MV3D_RGBD_Release();
    if (nRet != MV3D_RGBD_OK) {
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
#ifdef LOCALTEST
    return 1;
#endif
    return netState_;
}
int VWSCamera::startGrab() {
#ifdef LOCALTEST
    return 1;
#endif
    int nRet = MV3D_RGBD_OK;

    nRet = MV3D_RGBD_Start(handle_);
    if (nRet != MV3D_RGBD_OK) {
        LOGE("MV3D_RGBD_Start failed: error %#x at %s:%d", nRet, __FILE__, __LINE__);
        return -1;
    }
    grabState = true;
    return 1;
}
int VWSCamera::stopGrab() {
#ifdef LOCALTEST
    return 1;
#endif
    int nRet = MV3D_RGBD_OK;
    nRet = MV3D_RGBD_Stop(handle_);
    if (nRet != MV3D_RGBD_OK) {
        LOGE("MV3D_RGBD_Stop failed: error %#x at %s:%d", nRet, __FILE__, __LINE__);
        return -1;
    }
    grabState = false;
    return 1;
}
void VWSCamera::imageProcess(MV3D_RGBD_FRAME_DATA* stFrameData, VWSCamera::ImageData& data) {
    for (int i = 0; i < stFrameData->nImageCount; i++) {
        if (ImageType_Depth == stFrameData->stImageData[i].enImageType) {
            MV3D_RGBD_IMAGE_DATA stPointCloudImage;
            int nRet = MV3D_RGBD_MapDepthToPointCloud(handle_, &stFrameData->stImageData[i], &stPointCloudImage);
            if (MV3D_RGBD_OK != nRet) {
                break;
            }
            uint8_t* PointCloudImage = new uint8_t[stPointCloudImage.nDataLen];
            memcpy(PointCloudImage, stPointCloudImage.pData, stPointCloudImage.nDataLen);
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

int VWSCamera::RegisterFrameCallBack(GetImageCallBack func, void* pUser) {
#ifdef LOCALTEST
    return 1;
#endif
    int nRet = MV3D_RGBD_RegisterFrameCallBack(handle_, GetImageCallBackFunc, this);
    if (nRet != MV3D_RGBD_OK) {
        LOGE("RegisterFrameCallBack failed: error %#x at %s:%d", nRet, __FILE__, __LINE__);
        return -1;
    }
    func_ = func;
    pUser_ = pUser;
    return 0;
}

int VWSCamera::deleteImage(const VWSCamera::ImageData& data) {
    delete[] data.PointCloudImage.pData;
    delete[] data.RGB8PlanarImage.pData;
    return 1;
}

int VWSCamera::softTrigger() {
#ifdef LOCALTEST
    return 1;
#endif
    // changeTriggerSource(false);
    int nRet = MV3D_RGBD_SoftTrigger(handle_);
    if (nRet != MV3D_RGBD_OK) {
        LOGE("MV3D_RGBD_SoftTrigger failed: error %#x at %s:%d", nRet, __FILE__, __LINE__);
        return -1;
    }
    // changeTriggerSource(true);
    return 1;
}
/********
 * falg = true on
 * flag = false off
 */
int VWSCamera::changeTriggerMode(bool flag) {
#ifdef LOCALTEST
    return 1;
#endif
    MV3D_RGBD_PARAM para;
    if (flag)
        para.ParamInfo.stEnumParam.nCurValue = 1;
    else
        para.ParamInfo.stEnumParam.nCurValue = 0;
    para.enParamType = Mv3dRgbdParamType::ParamType_Enum;
    int nRet = MV3D_RGBD_SetParam(handle_, MV3D_RGBD_ENUM_TRIGGERMODE, &para);
    if (nRet != MV3D_RGBD_OK) {
        LOGE("changeTriggerMode failed: error %#x at %s:%d", nRet, __FILE__, __LINE__);
        return -1;
    }
    return 1;
}
int VWSCamera::getTriggerMode(int& mode) {
#ifdef LOCALTEST
    return 1;
#endif
    MV3D_RGBD_PARAM para;
    para.enParamType = Mv3dRgbdParamType::ParamType_Enum;
    int nRet = MV3D_RGBD_GetParam(handle_, MV3D_RGBD_ENUM_TRIGGERMODE, &para);
    if (nRet != MV3D_RGBD_OK) {
        LOGE("MV3D_RGBD_SetParam failed: error %#x at %s:%d", nRet, __FILE__, __LINE__);
        return -1;
    }
    mode = para.ParamInfo.stEnumParam.nCurValue;
    return 1;
}
int VWSCamera::FetchFrame(uint32_t nTimeOut, VWSCamera::ImageData& data) {
#ifdef LOCALTEST
    return 1;
#endif
    MV3D_RGBD_FRAME_DATA pstFrameData = {0};
    int nRet = MV3D_RGBD_FetchFrame(handle_, &pstFrameData, nTimeOut);
    if (nRet != MV3D_RGBD_OK) {
        LOGE("FetchFrame failed: error %#x at %s:%d", nRet, __FILE__, __LINE__);
        return -1;
    }
    imageProcess(&pstFrameData, data);
    return 1;
}
/********
 * falg = true 硬件触发
 * flag = false 软件触发
 */
int VWSCamera::changeTriggerSource(bool flag) {
#ifdef LOCALTEST
    return 1;
#endif
    MV3D_RGBD_PARAM para;
    if (flag)
        para.ParamInfo.stEnumParam.nCurValue = 0;
    else
        para.ParamInfo.stEnumParam.nCurValue = 7;
    para.enParamType = Mv3dRgbdParamType::ParamType_Enum;
    int nRet = MV3D_RGBD_SetParam(handle_, MV3D_RGBD_ENUM_TRIGGERSOURCE, &para);
    if (nRet != MV3D_RGBD_OK) {
        LOGE("MV3D_RGBD_SetParam failed: error %#x at %s:%d", nRet, __FILE__, __LINE__);
        return -1;
    }
    return 1;
}
int VWSCamera::getTriggerSource(int& mode) {
#ifdef LOCALTEST
    return 1;
#endif
    MV3D_RGBD_PARAM para;
    para.enParamType = Mv3dRgbdParamType::ParamType_Enum;
    int nRet = MV3D_RGBD_GetParam(handle_, MV3D_RGBD_ENUM_TRIGGERSOURCE, &para);
    if (nRet != MV3D_RGBD_OK) {
        LOGE("MV3D_RGBD_SetParam failed: error %#x at %s:%d", nRet, __FILE__, __LINE__);
        return -1;
    }
    mode = para.ParamInfo.stEnumParam.nCurValue;
    return 1;
}
int VWSCamera::setImageMode(int mode) {
#ifdef LOCALTEST
    return 1;
#endif
    MV3D_RGBD_PARAM para;
    para.enParamType = Mv3dRgbdParamType::ParamType_Enum;
    para.ParamInfo.stEnumParam.nCurValue = mode;
    int nRet = MV3D_RGBD_SetParam(handle_, MV3D_RGBD_ENUM_IMAGEMODE, &para);
    if (nRet != MV3D_RGBD_OK) {
        LOGE("MV3D_RGBD_GetParam failed: error %#x at %s:%d", nRet, __FILE__, __LINE__);
        return -1;
    }
    return 1;
}
int VWSCamera::getImageMode(int& mode) {
#ifdef LOCALTEST
    return 1;
#endif
    MV3D_RGBD_PARAM para;
    para.enParamType = Mv3dRgbdParamType::ParamType_Enum;
    int nRet = MV3D_RGBD_GetParam(handle_, MV3D_RGBD_ENUM_IMAGEMODE, &para);
    if (nRet != MV3D_RGBD_OK) {
        LOGE("MV3D_RGBD_GetParam failed: error %#x at %s:%d", nRet, __FILE__, __LINE__);
        return -1;
    }
    mode = para.ParamInfo.stEnumParam.nCurValue;
    return 1;
}

int VWSCamera::saveAllParam(const char* pOutFileName) {
#ifdef LOCALTEST
    return 1;
#endif
    int nRet = MV3D_RGBD_ExportAllParam(handle_, pOutFileName);
    if (nRet != MV3D_RGBD_OK) {
        LOGE("MV3D_RGBD_GetParam failed: error %#x at %s:%d", nRet, __FILE__, __LINE__);
        return -1;
    }
    return 1;
}

int VWSCamera::loadAllParam(const char* pOutFileName) {
#ifdef LOCALTEST
    return 1;
#endif
    int nRet = MV3D_RGBD_ImportAllParam(handle_, pOutFileName);
    if (nRet != MV3D_RGBD_OK) {
        LOGE("MV3D_RGBD_GetParam failed: error %#x at %s:%d", nRet, __FILE__, __LINE__);
        return -1;
    }
    return 1;
}
/********
Type: Float
Name Space: Standard
Visibility: Beginner
Streamable: Yes
Min: 1
Max: 8
*/
int VWSCamera::setFPS_Color(int frame) {
#ifdef LOCALTEST
    return 1;
#endif
    MV3D_RGBD_PARAM para;
    int mode;
    bool needTrans = false;
    if (getImageMode(mode) != 1)
        return -1;
    if (mode == VWSCamera::ImageMode::DepthImage) {
        if (setImageMode(VWSCamera::ImageMode::ColorImage) != 1)
            return -1;
        needTrans = true;
    }
    para.ParamInfo.stFloatParam.fCurValue = frame;
    para.enParamType = Mv3dRgbdParamType::ParamType_Float;
    int nRet = MV3D_RGBD_SetParam(handle_, MV3D_RGBD_FLOAT_FRAMERATE, &para);
    if (nRet != MV3D_RGBD_OK) {
        LOGE("MV3D_RGBD_SetParam failed: error %#x at %s:%d", nRet, __FILE__, __LINE__);
        return -1;
    }
    if (needTrans) {
        if (setImageMode(VWSCamera::ImageMode::DepthImage) != 1)
            return -1;
    }
    return 1;
}
/******
 * Node Name: ExposureTime
Type: Float
Name Space: Custom
Visibility: Beginner
Streamable: Yes
Min: 100
Max: 998584
*/
int VWSCamera::setExposure_Color(float time) {
#ifdef LOCALTEST
    return 1;
#endif
    int mode;
    bool needTrans = false;
    if (getImageMode(mode) != 1)
        return -1;
    if (mode == VWSCamera::ImageMode::DepthImage) {
        if (setImageMode(VWSCamera::ImageMode::ColorImage) != 1)
            return -1;
        needTrans = true;
    }
    MV3D_RGBD_PARAM para;
    para.ParamInfo.stFloatParam.fCurValue = time;
    para.enParamType = Mv3dRgbdParamType::ParamType_Float;
    int nRet = MV3D_RGBD_SetParam(handle_, MV3D_RGBD_FLOAT_EXPOSURETIME, &para);
    if (nRet != MV3D_RGBD_OK) {
        LOGE("MV3D_RGBD_SetParam failed: error %#x at %s:%d", nRet, __FILE__, __LINE__);
        return -1;
    }
    if (needTrans) {
        if (setImageMode(VWSCamera::ImageMode::DepthImage) != 1)
            return -1;
    }
    return 1;

} /*************
 应用于图像的增益，单位为dB

 Node Name: Gain
 Type: Float
 Name Space: Custom
 Visibility: Beginner
 Streamable: Yes
 Min: 0
 Max: 39.9926
 */
int VWSCamera::setGain_Color(float dB) {
#ifdef LOCALTEST
    return 1;
#endif
    int mode;
    bool needTrans = false;
    if (getImageMode(mode) != 1)
        return -1;
    if (mode == VWSCamera::ImageMode::DepthImage) {
        if (setImageMode(VWSCamera::ImageMode::ColorImage) != 1)
            return -1;
        needTrans = true;
    }
    MV3D_RGBD_PARAM para;
    para.ParamInfo.stFloatParam.fCurValue = dB;
    para.enParamType = Mv3dRgbdParamType::ParamType_Float;
    int nRet = MV3D_RGBD_SetParam(handle_, MV3D_RGBD_FLOAT_GAIN, &para);
    if (nRet != MV3D_RGBD_OK) {
        LOGE("MV3D_RGBD_SetParam failed: error %#x at %s:%d", nRet, __FILE__, __LINE__);
        return -1;
    }
    if (needTrans) {
        if (setImageMode(VWSCamera::ImageMode::DepthImage) != 1)
            return -1;
    }
    return 1;
}
int VWSCamera::getFPS_Color(int& frame) {
#ifdef LOCALTEST
    return 1;
#endif
    int mode;
    bool needTrans = false;
    if (getImageMode(mode) != 1)
        return -1;
    if (mode == VWSCamera::ImageMode::DepthImage) {
        if (setImageMode(VWSCamera::ImageMode::ColorImage) != 1)
            return -1;
        needTrans = true;
    }
    MV3D_RGBD_PARAM para;
    para.enParamType = Mv3dRgbdParamType::ParamType_Float;
    int nRet = MV3D_RGBD_GetParam(handle_, MV3D_RGBD_FLOAT_FRAMERATE, &para);
    if (nRet != MV3D_RGBD_OK) {
        LOGE("MV3D_RGBD_GetParam failed: error %#x at %s:%d", nRet, __FILE__, __LINE__);
        return -1;
    }
    frame = para.ParamInfo.stFloatParam.fCurValue;
    if (needTrans) {
        if (setImageMode(VWSCamera::ImageMode::DepthImage) != 1)
            return -1;
    }
    return 1;
}
int VWSCamera::getExposure_Color(float& time) {
#ifdef LOCALTEST
    return 1;
#endif
    int mode;
    bool needTrans = false;
    if (getImageMode(mode) != 1)
        return -1;
    if (mode == VWSCamera::ImageMode::DepthImage) {
        if (setImageMode(VWSCamera::ImageMode::ColorImage) != 1)
            return -1;
        needTrans = true;
    }
    MV3D_RGBD_PARAM para;
    para.enParamType = Mv3dRgbdParamType::ParamType_Float;
    int nRet = MV3D_RGBD_GetParam(handle_, MV3D_RGBD_FLOAT_EXPOSURETIME, &para);
    if (nRet != MV3D_RGBD_OK) {
        LOGE("MV3D_RGBD_GetParam failed: error %#x at %s:%d", nRet, __FILE__, __LINE__);
        return -1;
    }
    time = para.ParamInfo.stFloatParam.fCurValue;
    if (needTrans) {
        if (setImageMode(VWSCamera::ImageMode::DepthImage) != 1)
            return -1;
    }
    return 1;
}
int VWSCamera::getGain_Color(float& dB) {
#ifdef LOCALTEST
    return 1;
#endif
    int mode;
    bool needTrans = false;
    if (getImageMode(mode) != 1)
        return -1;
    if (mode == VWSCamera::ImageMode::DepthImage) {
        if (setImageMode(VWSCamera::ImageMode::ColorImage) != 1)
            return -1;
        needTrans = true;
    }
    MV3D_RGBD_PARAM para;
    para.enParamType = Mv3dRgbdParamType::ParamType_Float;
    int nRet = MV3D_RGBD_GetParam(handle_, MV3D_RGBD_FLOAT_GAIN, &para);
    if (nRet != MV3D_RGBD_OK) {
        LOGE("MV3D_RGBD_GetParam failed: error %#x at %s:%d", nRet, __FILE__, __LINE__);
        return -1;
    }
    dB = para.ParamInfo.stFloatParam.fCurValue;
    if (needTrans) {
        if (setImageMode(VWSCamera::ImageMode::DepthImage) != 1)
            return -1;
    }
    return 1;
}

/********
Type: Float
Name Space: Standard
Visibility: Beginner
Streamable: Yes
Min: 1
Max: 8
*/
int VWSCamera::setFPS_Depth(int frame) {
#ifdef LOCALTEST
    return 1;
#endif
    int mode;
    bool needTrans = false;
    if (getImageMode(mode) != 1)
        return -1;
    if (mode == VWSCamera::ImageMode::ColorImage) {
        if (setImageMode(VWSCamera::ImageMode::DepthImage) != 1)
            return -1;
        needTrans = true;
    }
    MV3D_RGBD_PARAM para;
    para.ParamInfo.stFloatParam.fCurValue = frame;
    para.enParamType = Mv3dRgbdParamType::ParamType_Float;
    int nRet = MV3D_RGBD_SetParam(handle_, MV3D_RGBD_FLOAT_FRAMERATE, &para);
    if (nRet != MV3D_RGBD_OK) {
        LOGE("MV3D_RGBD_SetParam failed: error %#x at %s:%d", nRet, __FILE__, __LINE__);
        return -1;
    }
    if (needTrans) {
        if (setImageMode(VWSCamera::ImageMode::ColorImage) != 1)
            return -1;
    }
    return 1;
}
/******
 * Node Name: ExposureTime
Type: Float
Name Space: Custom
Visibility: Beginner
Streamable: Yes
Min: 100
Max: 998584
*/
int VWSCamera::setExposure_Depth(float time) {
#ifdef LOCALTEST
    return 1;
#endif
    int mode;
    bool needTrans = false;
    if (getImageMode(mode) != 1)
        return -1;
    if (mode == VWSCamera::ImageMode::ColorImage) {
        if (setImageMode(VWSCamera::ImageMode::DepthImage) != 1)
            return -1;
        needTrans = true;
    }
    MV3D_RGBD_PARAM para;
    para.ParamInfo.stFloatParam.fCurValue = time;
    para.enParamType = Mv3dRgbdParamType::ParamType_Float;
    int nRet = MV3D_RGBD_SetParam(handle_, MV3D_RGBD_FLOAT_EXPOSURETIME, &para);
    if (nRet != MV3D_RGBD_OK) {
        LOGE("MV3D_RGBD_SetParam failed: error %#x at %s:%d", nRet, __FILE__, __LINE__);
        return -1;
    }

    if (needTrans) {
        if (setImageMode(VWSCamera::ImageMode::ColorImage) != 1)
            return -1;
    }
    return 1;

} /*************
 应用于图像的增益，单位为dB

 Node Name: Gain
 Type: Float
 Name Space: Custom
 Visibility: Beginner
 Streamable: Yes
 Min: 0
 Max: 39.9926
 */
int VWSCamera::setGain_Depth(float dB) {
#ifdef LOCALTEST
    return 1;
#endif
    int mode;
    bool needTrans = false;
    if (getImageMode(mode) != 1)
        return -1;
    if (mode == VWSCamera::ImageMode::ColorImage) {
        if (setImageMode(VWSCamera::ImageMode::DepthImage) != 1)
            return -1;
        needTrans = true;
    }
    MV3D_RGBD_PARAM para;
    para.ParamInfo.stFloatParam.fCurValue = dB;
    para.enParamType = Mv3dRgbdParamType::ParamType_Float;
    int nRet = MV3D_RGBD_SetParam(handle_, MV3D_RGBD_FLOAT_GAIN, &para);
    if (nRet != MV3D_RGBD_OK) {
        LOGE("MV3D_RGBD_SetParam failed: error %#x at %s:%d", nRet, __FILE__, __LINE__);
        return -1;
    }
    if (needTrans) {
        if (setImageMode(VWSCamera::ImageMode::ColorImage) != 1)
            return -1;
    }
    return 1;
}
int VWSCamera::getFPS_Depth(int& frame) {
#ifdef LOCALTEST
    return 1;
#endif
    int mode;
    bool needTrans = false;
    if (getImageMode(mode) != 1)
        return -1;
    if (mode == VWSCamera::ImageMode::ColorImage) {
        if (setImageMode(VWSCamera::ImageMode::DepthImage) != 1)
            return -1;
        needTrans = true;
    }
    MV3D_RGBD_PARAM para;
    para.enParamType = Mv3dRgbdParamType::ParamType_Float;
    int nRet = MV3D_RGBD_GetParam(handle_, MV3D_RGBD_FLOAT_FRAMERATE, &para);
    if (nRet != MV3D_RGBD_OK) {
        LOGE("MV3D_RGBD_GetParam failed: error %#x at %s:%d", nRet, __FILE__, __LINE__);
        return -1;
    }
    frame = para.ParamInfo.stFloatParam.fCurValue;
    if (needTrans) {
        if (setImageMode(VWSCamera::ImageMode::ColorImage) != 1)
            return -1;
    }
    return 1;
}
int VWSCamera::getExposure_Depth(float& time) {
#ifdef LOCALTEST
    return 1;
#endif
    int mode;
    bool needTrans = false;
    if (getImageMode(mode) != 1)
        return -1;
    if (mode == VWSCamera::ImageMode::ColorImage) {
        if (setImageMode(VWSCamera::ImageMode::DepthImage) != 1)
            return -1;
        needTrans = true;
    }
    MV3D_RGBD_PARAM para;
    para.enParamType = Mv3dRgbdParamType::ParamType_Float;
    int nRet = MV3D_RGBD_GetParam(handle_, MV3D_RGBD_FLOAT_EXPOSURETIME, &para);
    if (nRet != MV3D_RGBD_OK) {
        LOGE("MV3D_RGBD_GetParam failed: error %#x at %s:%d", nRet, __FILE__, __LINE__);
        return -1;
    }
    time = para.ParamInfo.stFloatParam.fCurValue;
    if (needTrans) {
        if (setImageMode(VWSCamera::ImageMode::ColorImage) != 1)
            return -1;
    }
    return 1;
}
int VWSCamera::getGain_Depth(float& dB) {
#ifdef LOCALTEST
    return 1;
#endif
    int mode;
    bool needTrans = false;
    if (getImageMode(mode) != 1)
        return -1;
    if (mode == VWSCamera::ImageMode::ColorImage) {
        if (setImageMode(VWSCamera::ImageMode::DepthImage) != 1)
            return -1;
        needTrans = true;
    }
    MV3D_RGBD_PARAM para;
    para.enParamType = Mv3dRgbdParamType::ParamType_Float;
    int nRet = MV3D_RGBD_GetParam(handle_, MV3D_RGBD_FLOAT_GAIN, &para);
    if (nRet != MV3D_RGBD_OK) {
        LOGE("MV3D_RGBD_GetParam failed: error %#x at %s:%d", nRet, __FILE__, __LINE__);
        return -1;
    }
    dB = para.ParamInfo.stFloatParam.fCurValue;
    if (needTrans) {
        if (setImageMode(VWSCamera::ImageMode::ColorImage) != 1)
            return -1;
    }
    return 1;
}
/// @brief
/// @return ture:正在取流，false：不在取流
bool VWSCamera::getGrabState() {
#ifdef LOCALTEST
    return 1;
#endif
    return grabState;
}

int VWSCamera::coordChanged() {
#ifdef LOCALTEST
    return 1;
#endif
    MV3D_RGBD_PARAM stParm;
    stParm.enParamType = ParamType_Int;
    stParm.ParamInfo.stIntParam.nCurValue = 0;
    int nRet;
    nRet = MV3D_RGBD_SetParam(handle_, MV3D_RGBD_INT_IMAGEALIGN, &stParm);
    if (nRet != MV3D_RGBD_OK) {
        LOGE("MV3D_RGBD_SetParam failed: error %#x at %s:%d", nRet, __FILE__, __LINE__);
        return -1;
    }
    stParm.enParamType = ParamType_Int;
    stParm.ParamInfo.stIntParam.nCurValue = 0;
    nRet = MV3D_RGBD_SetParam(handle_, MV3D_RGBD_INT_RESOLUTIONALIGN, &stParm);
    if (nRet != MV3D_RGBD_OK) {
        LOGE("MV3D_RGBD_SetParam failed: error %#x at %s:%d", nRet, __FILE__, __LINE__);
        return -1;
    }
    return 1;
}