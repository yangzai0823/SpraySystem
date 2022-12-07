#include "mcoperator.h"
#include <byteswap.h>
#include <QThread>
#include <unistd.h>
#include <QWaitCondition>
#include <QCoreApplication>
#include <QtEndian>
#include "Util/Log/clog.h"
#include <QDate>
#include <QSemaphore>


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

    connect(dataparser, SIGNAL(getTrajParam_Signal(quint16)), this, SLOT(getTrajParam_Slot(quint16)));
    connect(dataparser,SIGNAL(sendToRBT_Signal(quint16)),this,SLOT(sendToRBT_Slot(quint16)));
    connect(dataparser,SIGNAL(mcWarning_Signal(quint16)),this,SLOT(mcWarning_Slot(quint16)));
    connect(dataparser,SIGNAL(test_Signal()),this,SLOT(test_Slot()));

    // connect(socketclient, SIGNAL(readyRead_Signal(QByteArray)), this, SLOT(readyRead_Slot(QByteArray)), Qt::ConnectionType::QueuedConnection);
    connect(this, SIGNAL(connect_Signal(QString, int)), socketclient, SLOT(connect_Slot(QString, int)), Qt::ConnectionType::QueuedConnection);

   timer = new QTimer();
    connect(timer,SIGNAL(timeout()),this,SLOT(checkState_Slot()));
    return 1;
}

int MCOperator::start()
{
    // auto ret = socketclient->connectServer(ip,port);
    emit connect_Signal(ip, port);

    reset();

     //timer->start(1000);
    return 1;
}


void MCOperator::startReceive()
{
    CLog::getInstance()->log("MC, 启动运动控制器接收消息");
    
    // sendData(6, 0, 0);
    trySendData(6,0,0,data.b_receive_start);
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
    // sendData(0, zeropoint, offset);
    trySendData(0,zeropoint,offset,data.b_receive_traj_param);
}

void MCOperator::reset()
{
     std::cout<<""<<std::endl;
     CLog::getInstance()->log("运动控制器请报错复位");
    // sendData(9, 0, 0);
    trySendData(5,0,0,data.b_receive_reset);
}

void MCOperator::sendRbtResult(float success)
{
    trySendData(7,success,0,data.b_receive_rbt_result);
}

std::vector<float> MCOperator::getChainEncoders(bool & success)
{
    success = true;

    // sendData(1, 0, 0);
    // if(!waitData(data.bchainencoder))
    // {
    //     success = false;
    // }
    success =  trySendData(1,0,0,data.b_chain_encoder);
    std::vector<float> val;
    val.push_back(data.encoder1);
    val.push_back(data.encoder2);

    return val;
}
std::vector<float> MCOperator::getRealTimeEncoder()
{
    // sendData(2, 0, 0);
    // waitData(data.brealtimeencoder);
    trySendData(2,0,0,data.b_realtime_encoder);
    std::vector<float> val;
    val.push_back(data.realtimeencoder1);
    val.push_back(data.realtimeencoder2);

    return val;
}


bool MCOperator::trySendData(uint8_t order,float v1,float v2, bool &flag){
    int N = 3;
    bool ret = false;
    for(int i=0;i<3;i++){ 
        sendData(order,v1,v2);
        ret = waitData(flag);
        if(ret == true){
            break;
        }
    }
    return ret;
}

void MCOperator::sendData(uint8_t order, float v1, float v2, u_int16_t num)
{
    QByteArray arry;

    //报头
    int16_t head = qToBigEndian<int16_t>(0xabcd);
    // arry.append((char*)&head,2);

    //编号
    if(num == 0){
        if(count>=9999){
            count = 0;
        }else{
            count++;
        }
    }
    else{
        count = num;
    }
    int16_t count_BigEndian= qToBigEndian<int16_t>(count);
    arry.append((char *)&count_BigEndian,2);

    //指令
    arry.append(order);

    //数据
    float d1 = qToBigEndian<float>(v1);
    float d2 = qToBigEndian<float>(v2);
    arry.append((char *)&d1, 4);
    arry.append((char *)&d2, 4);

    //报尾
    unsigned char * crcdata = (unsigned char*)arry.data();
    u_int16_t trail = dataparser->do_crc(crcdata,11);

    std::cout<<std::hex<<trail<<std::endl;

    auto trailToBingEndian = qToBigEndian(trail);
    arry.append((char *)&trailToBingEndian,2);

    uint ret;
    arry.insert(0,(char*)&head,2);
    ret = socketclient->send(arry);
}

bool MCOperator::waitData(bool &flag)
{
    int i = 0;
    while (!flag && i<30)
    {
        i++;
        //        QCoreApplication::processEvents();
        usleep(1e5);
    }
    //相应超时
    if(flag == false){
        return false;
    }
    else{
        flag = false;
        return true;
    }
}

void MCOperator::readyRead_Slot(QByteArray buf)
{
    std::thread::id id = std::this_thread::get_id();
    // emit getTrajParam_Signal();
}

void MCOperator::getTrajParam_Slot(quint16 num)
{
    // CLog::getInstance()->log("MC, 请求数据");
    
    QString current_date_time = QDateTime::currentDateTime().toString(Qt::ISODateWithMs);
        std::cout<<"MC, 请求数据: "<<current_date_time.toStdString()<<std::endl;
    sendData(33,0,0,num);

    emit getTrajParam_Signal();
}

void MCOperator::sendToRBT_Slot(quint16 num)
{
    //应答
    sendData(35,0,0,num);

    CLog::getInstance()->log("MC, 请求向机器人发送轨迹");
    emit sendToRBT_Signal();
}

void MCOperator::mcWarning_Slot(quint16 num){
    //应答
    sendData(36,0,0,num);
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