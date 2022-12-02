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

    void sendTrajParam(float zeropoint, float offset);
    std::vector<float> getChainEncoders(bool & success);
    std::vector<float> getRealTimeEncoder();
    void SevenAxisReset();
private:
    MCData data;
    QtSocketClient *socketclient;
    mcdatapaser* dataparser;
    QTimer timer;
    int preheart;
    int failcount=0;
    bool state;

    void sendData(uint8_t head,float v1, float v2);
    bool waitData(bool &flag);
public slots:
    void readyRead_Slot(QByteArray buf);
    void getTrajParam_Slot();
    void sendToRBT_Slot();
    void checkState_Slot();
signals:
    void getTrajParam_Signal();
    void sendToRBT_Signal();
    void connect_Signal(QString ip,int port);
};

#endif // MCOPERATOR_H
