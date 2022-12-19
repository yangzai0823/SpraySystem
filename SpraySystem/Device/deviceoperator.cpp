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
    updated = true;
    this->ip = ip;
}

int DeviceOperator::getPort(int index)
{
    if (index == 0)
    {
        return port;
    }
    else
    {
        return port2;
    }
}

void DeviceOperator::setPort(int port, int index)
{
    updated = true;
    if (index == 0)
    {
        this->port = port;
    }
    else
    {
        this->port2 = port;
    }
}

int DeviceOperator::getState()
{
    return 1;
}
