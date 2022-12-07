#ifndef MCOPERATOR_H
#define MCOPERATOR_H

#include <memory>
#include <QTcpSocket>
#include <QHostAddress>

#include "DataAccess/Models/MotionController/motioncontroller.h"
#include "Device/deviceoperator.h"
#include "Util/Socket/qtsocketclient.h"
#include "Device/MotionController/mcdatapaser.h"
#include <QVariant>
#include <QTimer>

using MCData = vws::MCData;


Q_DECLARE_METATYPE(MCData)  //注册结构体
class MCOperator : public QObject,public DeviceOperator
{
    Q_OBJECT
public:
    MCOperator(std::shared_ptr<MotionController> mc);
    virtual ~MCOperator();
    int init();
    int start();
    void close();
    int getState();
    /** @brief 开始接受运动控制器信号*/
    void startReceive();

    /** @brief 运动控制器请报错复位*/
    void reset();

    /** @brief 向机器人发送运动轨迹*/
    void sendTrajParam(float zeropoint, float offset);
    
    /** @brief 反馈轨迹发送结果
     * @param success: 1成功，-1失败
    */
    void sendRbtResult(float success);
    std::vector<float> getChainEncoders(bool & success);
    std::vector<float> getRealTimeEncoder();
    void SevenAxisReset();
private:
    MCData data;
    QtSocketClient *socketclient;
    mcdatapaser* dataparser;
    QTimer *timer;
    int preheart;
    int failcount=0;
    bool state;
    int count; //0-10000;

    bool trySendData(uint8_t order,float v1,float v2,bool &flag);
    void sendData(uint8_t order,float v1, float v2, u_int16_t num =0);
    bool waitData(bool &flag);
public slots:
    void readyRead_Slot(QByteArray buf);
    void getTrajParam_Slot(quint16 num); 
    void sendToRBT_Slot(quint16 num);
    void mcWarning_Slot(quint16 num);
    void checkState_Slot();
signals:
    void getTrajParam_Signal();
    void sendToRBT_Signal();
    void connect_Signal(QString ip,int port);
};

#endif // MCOPERATOR_H
