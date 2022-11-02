#include "plcoperator.h"

PLCOperator::PLCOperator(std::shared_ptr<PLC> plc)
{
    this->ip= plc->Ip;
    this->port=plc->Port.toInt();
    this->name = plc->Name;
}

PLCOperator::~PLCOperator()
{
    delete socketclient;
}

int PLCOperator::init()
{
    socketclient = new QtSocketClient();
    dataparser = std::make_shared<PLCDataPaser>();

    connect(socketclient,SIGNAL(readyRead_Signal(QByteArray)),this,SLOT(readyRead_Slot(QByteArray)));
    return 1;
}

int PLCOperator::start()
{
    auto ret = socketclient->connectServer(ip,port);
    return ret;
}

void PLCOperator::close()
{
    socketclient->close();
}

int PLCOperator::getState()
{
    return 1;
}

int PLCOperator::sendData(QString data)
{
    auto ret = socketclient->send(data);
    return ret;
}

void PLCOperator::readyRead_Slot(QByteArray buf)
{
    PLCData data;
    dataparser->DataPaser(buf,data);

    QVariant varData;
    varData.setValue(data);

    emit recevieData_Signal(varData);
}
