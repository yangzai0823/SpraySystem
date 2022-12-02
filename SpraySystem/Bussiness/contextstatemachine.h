#ifndef CONTEXTSTATEMACHINE_H
#define CONTEXTSTATEMACHINE_H

#include <QObject>
#include <QStateMachine>
#include <QVariant>
#include <QTimer>
#include "mutex"
#include "VWSCamera/VWSCamera.h"
#include "Data/StructData.h"
#include <QThread>
#include "HeadDistanceMonitor.h"


class ContextStateMachine : public QStateMachine
{
    Q_OBJECT
public:
    struct SMContext
    {
        bool flag_camera = false;
        bool flag_laser = false;
        std::vector<float> laserCouple1;
        std::vector<float> laserCouple2;

        bool flag_img_head = false;
        bool flag_img_trail = false;
        VWSCamera::ImageData img_head;
        VWSCamera::ImageData img_trail;
        float encoder_img_head;
        float encoder_img_trail;

        vws::VisionData visionData;

        /** @brief 相机拍照编码器数值在寄存器中索引， 下相机：0，上相机：1*/
        int index = 0;
    };
    SMContext Context;
    ContextStateMachine();
    ~ContextStateMachine();

private:
    void checkHeadLaserAndImg();
    void checkTrailLaserAndImg();

    float getImgEncoder(bool init =false);
public:
    QString Name;

private:
    static std::mutex _mutex;
    //    VisionContext *visionContext;
    /**状态**/
    QState *parentState;
    QState *deviceAlarm;
    QState *stateIDLE;
    /** @brief 等待感应开关*/
    QState *waitLaserSignal;
    /** @brief 处理头部图像*/
    QState *processHeadImg;
    /** @brief 等待尾部相机事件*/
    QState *waitTrailProcess;
    /** @brief 处理尾部图像*/
    QState *processTrailImg;
    /**状态end**/

    /**Transition**/
    QSignalTransition *tranWaitSignal;
    QSignalTransition *tranProcessHeadImg;
    QSignalTransition *tranWaiTrailProcess;
    QSignalTransition *tranProcessTrailImg;
    QSignalTransition *tranIDLE;

    QSignalTransition *tranHeadImgTimeout;
    QSignalTransition *tranTrailImgTimeout;
    /**Transition end**/

    int interval = 100;  //计时器间隔时间 mm
    int timeout_head = 30*1000; //超时时间 30m
    int timeout_trail = 60*1000;
    QTimer *timer_img_head;
    int time_head =0;  //head计数
    QTimer *timer_img_trail;
    int time_trail = 0;  //trail计数

    std::vector<float> tmplaserdata; //暂存测距

    float pre_img_encoder;  //暂存编码器前一次拍照数值
signals:
    //外部信号
    void beginVision_Head_Signal(ContextStateMachine *context);
    void finishVision_Head_Signal(ContextStateMachine *context);
    void beginVision_Trail_Signal(ContextStateMachine *context);

    //内部信号
    void alarm();
    void cameraSignalOn();
    void laserSignalOnAndImgReady();
    void headDone();
    void cameraSignalOffAndImgReady();
    void trailDone();
    void headImgTimeout();
    void trailImgTimeout();
public slots:
    //外部事件
    void sendPlcData_Slot(QVariant vData);
    void sendImgData_Slot(QVariant vData);
    void finishVision_Slot(bool ishead);

    //内部事件
    void enteredParentState_Slot();
    void enteredAlarm_Slot();

    void enteredWaitLaserSignal_Slot();
    void exitWaitLaserSignal_Slot();

    void enteredProcessHeadImg_Slot();
    void enterdWaitTrailProcess_Slot();
    void exitWaitTrailProcess_Slot();
    void enteredProcessTrailImg_Slot();
    void enteredIDLE_Slot();

    void headTimer_Slot();
    void trailTimer_Slot();
};
Q_DECLARE_METATYPE(VWSCamera::ImageData)
Q_DECLARE_METATYPE(ContextStateMachine::SMContext)
#endif // CONTEXTSTATEMACHINE_H
