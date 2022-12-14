#ifndef PLCOPERATOR_H
#define PLCOPERATOR_H
#include <memory>
#include "DataAccess/Models/PLC/plc.h"
#include "Device/deviceoperator.h"
#include "Util/Socket/qtsocketserver.h"
#include "Util/Socket/qtsocketclient.h"
#include "plcdatapaser.h"
#include <QVariant>

using PLCData = PLCDataPaser::PLCData;

Q_DECLARE_METATYPE(PLCData)  //注册结构体
class PLCOperator: public QObject, public DeviceOperator
{
    Q_OBJECT
public:
    PLCOperator(std::shared_ptr<PLC> plc);
    ~PLCOperator();
    int init();
    int start();
    void close();
    int getState();

    int sendData(QString data);
    QString getName();

private:
    QtSocketClient *socketclient;
    std::shared_ptr<PLCDataPaser> dataparser;
private slots:
    void readyRead_Slot(QByteArray buf);
signals:
    void recevieData_Signal(QVariant data);
    void newConnection_Signal(QString ip,int port);
};

#endif // PLCOPERATOR_H
