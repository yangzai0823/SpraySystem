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

using MCData = mcdatapaser::MCData;


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

    void sendTrajParam(float zeropoint, float offset);
    std::vector<float> getChainEncoders();
    std::vector<float> getRealTimeEncoder();
    void SevenAxisReset();

private:
    MCData data;
    QtSocketClient *socketclient;
     std::shared_ptr<mcdatapaser> dataparser;

    void sendData(uint8_t head,float v1, float v2);
    void waitData(bool &flag);
private slots:
    void readyRead_Slot(QByteArray buf);

signals:
    void getTrajParam_Signal();
};

#endif // MCOPERATOR_H
