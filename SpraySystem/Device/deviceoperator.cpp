#include "deviceoperator.h"

int DeviceOperator::start()
{
    return -1;
}

void DeviceOperator::close()
{

}

QString DeviceOperator::getName()
{
    return name;
}

QString DeviceOperator::getIP()
{
    return ip;
}

void DeviceOperator::setIP(QString ip)
{
    updated=true;
    this->ip = ip;
}

int DeviceOperator::getPort()
{
    return port;
}

void DeviceOperator::setPort(int port)
{
    updated=true;
    this->port = port;
}

int DeviceOperator::getState()
{
    return 1;
}
