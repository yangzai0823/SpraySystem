#include "plcoperator.h"

PLCOperator::PLCOperator(std::shared_ptr<PLC> plc)
{
    this->ip= plc->Ip;
    this->port=plc->Port.toInt();
    this->name = plc->Name;
}

PLCOperator::~PLCOperator()
{
    delete dataparser;
    delete socketclient;
}

int PLCOperator::init()
{
    dataparser = new PLCDataPaser();
    dataparser->plcdata = &plcData;
    socketclient = new QtSocketClient(dataparser);


    // connect(socketclient,SIGNAL(readyRead_Signal(QByteArray)),this,SLOT(readyRead_Slot(QByteArray)));
    connect(dataparser,SIGNAL(readyRead_Signal()),this,SLOT(readyRead_Slot()));
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

void PLCOperator::readyRead_Slot()
{
    QVariant varData;
    varData.setValue(plcData);

    emit recevieData_Signal(varData);
}
