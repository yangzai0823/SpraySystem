#ifndef VWSCAMERA_H
#define VWSCAMERA_H
//#include "Include/common.hpp"
#include <string>
#include "Include/Mv3dRgbdApi.h"
# include <sys/time.h>
# include <unistd.h>
static inline char* getLocalTime() {
    static char local[26] = {0};
    time_t time;

    struct timeval tv;
    gettimeofday(&tv, NULL);

    time = tv.tv_sec;
    struct tm* p_time = localtime(&time);
    strftime(local, 26, "%Y-%m-%d %H:%M:%S", p_time);

    return local;
}
#define LOGE(fmt,...)  printf("(%s) Error: " fmt "\n", getLocalTime(), ##__VA_ARGS__) 
class VWSCamera
{
public:
    struct ImageMode {
        const static int DepthImage = 2;
        const static int ColorImage = 8;
    };

    struct ImageData {
        MV3D_RGBD_IMAGE_DATA PointCloudImage;
        MV3D_RGBD_IMAGE_DATA RGB8PlanarImage;
    };
    typedef void (*GetImageCallBack)  (const ImageData &data, void* pUser);
    void* pUser_;
    GetImageCallBack func_;
    void imageProcess(MV3D_RGBD_FRAME_DATA *stFrameData,ImageData &data);
//private:
    int netState_ = 0;
    std::string ip_ = "";
    MV3D_RGBD_DEVICE_INFO devInfo_;
    void* handle_ = NULL;
public:
    VWSCamera(/* args */);
    ~VWSCamera();
    int Init(std::string ip,int port);
    int connect();
    int disConnect();
    int softTrigger();
    int state();
    int startGrab();
    int stopGrab();
    int changeTriggerMode(bool flag);//true. on false off
    int getTriggerMode(int &mode);
    int FetchFrame(uint32_t nTimeOut,VWSCamera::ImageData &data);
    int changeTriggerSource(bool flag);//切换为硬件触发
    int getTriggerSource(int &mode);
    int RegisterFrameCallBack(GetImageCallBack func, void* pUser);
    int deleteImage(const ImageData &data);
    int setImageMode(int mode);
    int getImageMode(int &mode);
    int saveAllParam(const char* pOutFileName);
    int loadAllParam(const char* pOutFileName);
    
    int setFPS_Color(int frame);
    int setExposure_Color(float time);
    int setGain_Color(float dB);
    int getFPS_Color(int &frame);
    int getExposure_Color(float &time);
    int getGain_Color(float &dB);

    int setFPS_Depth(int frame);
    int setExposure_Depth(float time);
    int setGain_Depth(float dB);
    int getFPS_Depth(int &frame);
    int getExposure_Depth(float &time);
    int getGain_Depth(float &dB);
    
    int coordChanged();

};



#endif