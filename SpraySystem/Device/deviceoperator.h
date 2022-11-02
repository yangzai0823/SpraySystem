#ifndef DEVICEOPERATOR_H
#define DEVICEOPERATOR_H

#include <QObject>

//设备控制接口
class DeviceOperator
{
public:
    DeviceOperator(){};
    virtual int start();
    virtual void close();
    QString getName();
    QString getIP();
    void setIP(QString ip);
    int getPort();
    void setPort(int port);
    int getState();

    bool updated=false;
protected:
    QString ip;
    int port;
    QString name;
    int state =-1;
};

#endif // DEVICEOPERATOR_H
