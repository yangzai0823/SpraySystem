#ifndef CONTEXTSTATEMACHINE_H
#define CONTEXTSTATEMACHINE_H

#include <QObject>
#include <QStateMachine>
#include <QVariant>
#include <QTimer>
#include "mutex"
#include "VWSCamera/VWSCamera.h"
#include "Data/StructData.h"

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
    };
    SMContext Context;
    ContextStateMachine();
    ~ContextStateMachine();

private:
    void checkHeadLaserAndImg();
    void checkTrailLaserAndImg();

    
public:
    QString Name;

private:
    static std::mutex _mutex;
    //    VisionContext *visionContext;
    /**状态**/
    QState *stateIDLE;
    QState *waitLaserSignal;
    QState *processHeadImg;
    QState *headProcessDone;
    QState *processTrailImg;
    /**状态end**/

    /**Transition**/
    QSignalTransition *tranWaitSignal;
    QSignalTransition *tranProcessHeadImg;
    QSignalTransition *tranHeadProcessDone;
    QSignalTransition *tranProcessTrailImg;
    QSignalTransition *tranIDLE;

    QSignalTransition *tranHeadImgTimeout;
    QSignalTransition *tranTrailImgTimeout;
    /**Transition end**/

    int interval = 100;  //计时器间隔时间 mm
    int timeout = 30*1000; //超时时间 30m
    QTimer *timer_img_head;
    int time_head =0;  //head计数
    QTimer *timer_img_trail;
    int time_trail = 0;  //trail计数
signals:
    //外部信号
    void beginVision_Singal(ContextStateMachine *context, bool ishead);

    //内部信号
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
    void enteredWaitLaserSignal_Slot();
    void enteredProcessHeadImg_Slot();
    void enterdHeadProcessDone_Slot();
    void enteredProcessTrailImg_Slot();
    void enteredIDLE_Slot();

    void headTimer_Slot();
    void timeoutHeadImg_Slot();
    void trailTimer_Slot();
    void timeoutTrailImg_Slot();
};
Q_DECLARE_METATYPE(VWSCamera::ImageData)
Q_DECLARE_METATYPE(ContextStateMachine::SMContext)
#endif // CONTEXTSTATEMACHINE_H
