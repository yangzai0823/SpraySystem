#include "mcoperator.h"
#include <byteswap.h>
#include <QThread>
#include <unistd.h>
#include <QWaitCondition>
#include <QCoreApplication>
#include <QtEndian>

MCOperator::MCOperator(std::shared_ptr<MotionController> mc)
{
    this->ip = mc->Ip;
    this->port = mc->Port.toInt();
    this->name = mc->Name;
}

MCOperator::~MCOperator()
{
    delete dataparser;
    delete socketclient;
}

int MCOperator::init()
{
    dataparser = new mcdatapaser();
    dataparser->mcData = &data;
    socketclient = new QtSocketClient(dataparser);

    connect(dataparser, SIGNAL(getTrajParam_Signal()), this, SLOT(getTrajParam_Slot()));

    connect(socketclient, SIGNAL(readyRead_Signal(QByteArray)), this, SLOT(readyRead_Slot(QByteArray)), Qt::ConnectionType::QueuedConnection);
    connect(this, SIGNAL(connect_Signal(QString, int)), socketclient, SLOT(connect_Slot(QString, int)), Qt::ConnectionType::QueuedConnection);

    return 1;
}

int MCOperator::start()
{
    // auto ret = socketclient->connectServer(ip,port);
    emit connect_Signal(ip, port);

   
    return 1;
}


void MCOperator::startReceive()
{
    std::cout<<"启动运动控制器接收消息"<<std::endl;
    sendData(8, 0, 0);
}

void MCOperator::close()
{
    socketclient->close();
}

int MCOperator::getState()
{
}

void MCOperator::sendTrajParam(float zeropoint, float offset)
{
    sendData(0, zeropoint, offset);
}

void MCOperator::reset()
{
     std::cout<<"重置运动控制器状态"<<std::endl;
    sendData(-1, 0, 0);
}

std::vector<float> MCOperator::getChainEncoders()
{
    // std::vector<float> v = {1000, 1000};
    // return v;

    sendData(1, 0, 0);
    waitData(data.bchainencoder);

    std::vector<float> val;
    val.push_back(data.encoder1);
    val.push_back(data.encoder2);

    return val;
}
std::vector<float> MCOperator::getRealTimeEncoder()
{
    sendData(2, 0, 0);
    waitData(data.brealtimeencoder);
    std::vector<float> val;
    val.push_back(data.realtimeencoder1);
    val.push_back(data.realtimeencoder2);

    return val;
}

void MCOperator::sendData(uint8_t head, float v1, float v2)
{

    std::thread::id id = std::this_thread::get_id();
    //    std::cout << "mcoperator sendData 线程ID: "<< id << std::endl;

    QByteArray arry;
    arry.append(head);

    float d1 = qToBigEndian<float>(v1);
    float d2 = qToBigEndian<float>(v2);
    //    float d1 = bswap_32(v1);
    //    float d2 = bswap_32(v2);

    arry.append((char *)&d1, 4);
    arry.append((char *)&d2, 4);

    uint ret;
    ret = socketclient->send(arry);
}

void MCOperator::waitData(bool &flag)
{
    int i = 0;
    while (!flag)
    {
        i++;
        //        QCoreApplication::processEvents();
        usleep(10);
    }
    flag = false;
}

void MCOperator::readyRead_Slot(QByteArray buf)
{
    std::thread::id id = std::this_thread::get_id();
    emit getTrajParam_Signal();
    //    std::cout << "mcoperator slot 线程ID: "<< id << std::endl;

    //    dataparser->DataPaser(buf,data);

    //获取规划数据
    // if(data.btrajparam){
    //     data.btrajparam=false;
    //     emit getTrajParam_Signal();

    // }
}

void MCOperator::getTrajParam_Slot()
{
    std::cout<<"MC, 请求数据"<<std::endl;
    emit getTrajParam_Signal();
}

void MCOperator::checkState_Slot()
{
    if(preheart!=data.heart){
        preheart = data.heart;
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