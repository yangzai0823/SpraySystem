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
    delete timer;
}

int PLCOperator::init()
{
    dataparser = new PLCDataPaser();
    dataparser->plcdata = &plcData;
    socketclient = new QtSocketClient(dataparser);

    connect(dataparser,SIGNAL(readyRead_Signal()),this,SLOT(readyRead_Slot()));

    timer = new QTimer();
    connect(timer,SIGNAL(timeout()),this,SLOT(checkState_Slot()));
    return 1;
}

int PLCOperator::start()
{
    auto ret = socketclient->connectServer(ip,port);

    timer->start(1000);
    return ret;
}

void PLCOperator::close()
{
    socketclient->close();
}

int PLCOperator::getState()
{
    auto ret = state?1:-1;
    return ret;
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

void PLCOperator::checkState_Slot()
{
    if(preheart!=plcData.heart){
        preheart = plcData.heart;
        state=true;
        if(failcount!=0){
            failcount=0;
        }
    }else
    {
        if(++failcount>3){
            state = false;
        }
    }
}