#ifndef DEVICEOPERATOR_H
#define DEVICEOPERATOR_H

#include <QObject>

// 设备控制接口
class DeviceOperator
{
public:
    DeviceOperator(){};
    virtual int start();
    virtual void close();
    QString getName();
    QString getIP();
    void setIP(QString ip);
    int getPort(int index = 0);
    void setPort(int port, int index = 0);
    int getState();

    bool updated = false;

protected:
    QString ip;
    int port;
    int port2;
    QString name;
    int state = -1;
};

#endif // DEVICEOPERATOR_H
