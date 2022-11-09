#ifndef SIGNALSTATE_H
#define SIGNALSTATE
#include <QObject>

class SignalState
{
public:
    enum State{
        IDLE = 0,
        /* @brief 等待头部拍照信号*/
        WaitCameraSignalOn = 1,
        /* @brief 等待箱体感应信号*/
        WaitLaserSignalOn = 2,
        /* @brief 处理头部图像*/
        ProcessHeadImg = 3,
        /* @brief 等待头部图像*/
        WaitHeadImg = 4,
        /* @biref 头部图像处理完毕*/
        HeadImgDone = 5,
        /* @brief 处理尾部图像*/
        ProcessTrailImg = 6,
        /*@biref 等待尾部图像*/
        WaitTrailImg = 7,
    };
    enum Trigger
    {
        /* @brief 头部相机拍照信号*/
        CameraSignalOn = 0,
        /* @brief 箱体感应信号*/
        LaserSignalOnWithImg = 2,
        /* @brief 头部图像*/
        LaserSignalOnWithOutImg = 3,
        /* @brief 尾部相机拍照信号，有图像*/
        CameraSignalOffWithImg = 4,
        /* @brief 尾部相机拍照信号，无图像*/
        CameraSignalOffWithOutImg = 5,
        /* @brief 头部图像*/
        HeadImgReady = 6,
        /* @brief 尾部图像*/
        TrailImgReady = 7,
        /* @brief 头部处理完成*/
        HeadImgProcessDone = 8,
        /* @brief 尾部处理完成*/
        TrailImgProcessDone = 9
    };
    SignalState(/* args */);
    ~SignalState();
    static SignalState::State NextState(SignalState::State current, SignalState::Trigger trigger);
private:
};
Q_DECLARE_METATYPE(SignalState::State)
#endif
