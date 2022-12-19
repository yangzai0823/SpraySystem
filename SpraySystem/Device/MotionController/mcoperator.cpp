#include "mcoperator.h"
#include <byteswap.h>
#include <QThread>
#include <unistd.h>
#include <QWaitCondition>
#include <QCoreApplication>
#include <QtEndian>
#include "Util/Log/clog.h"
#include <QDate>
#include <iostream>
#include <thread>

MCOperator::MCOperator(std::shared_ptr<MotionController> mc)
{
    this->ip = mc->Ip;
    this->port = mc->Port.toInt();
    this->port2 = mc->Port2.toInt();
    this->name = mc->Name;
}

MCOperator::~MCOperator()
{
    delete dataparser_master;
    delete master_socket;
}

int MCOperator::init()
{
    dataparser_master = new mcdatapaser();
    dataparser_slave = new mcdatapaser();
    dataparser_master->mcData = &data_master;
    dataparser_slave->mcData = &data_slave;
    // dataparser->semaphone_slave = &semaphone_slave;
    // dataparser->reply_order = replyOrder;
    master_socket = new QtSocketClient(dataparser_master);
    slave_socket = new QtSocketClient(dataparser_slave);

    // connect(dataparser, SIGNAL(getTrajParam_Signal(quint16)), this, SLOT(getTrajParam_Slot(quint16)));
    // connect(dataparser,SIGNAL(sendToRBT_Signal(quint16)),this,SLOT(sendToRBT_Slot(quint16)));
    connect(dataparser_slave, SIGNAL(mcWarning_Signal(quint16)), this, SLOT(mcWarning_Slot(quint16)));
    // connect(dataparser_master,SIGNAL(test_Signal()),this,SLOT(test_Slot()));

    // connect(socketclient, SIGNAL(readyRead_Signal(QByteArray)), this, SLOT(readyRead_Slot(QByteArray)), Qt::ConnectionType::QueuedConnection);
    connect(this, SIGNAL(connect_master_Signal(QString, int)), master_socket, SLOT(connect_Slot(QString, int)), Qt::ConnectionType::QueuedConnection);
    connect(this, SIGNAL(connect_slave_Signal(QString, int)), slave_socket, SLOT(connect_Slot(QString, int)), Qt::ConnectionType::QueuedConnection);

    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(checkState_Slot()));
    return 1;
}

int MCOperator::start()
{
    stop = false;
    // auto ret = socketclient->connectServer(ip,port);
    emit connect_master_Signal(ip, port);
    emit connect_slave_Signal(ip, port2);
    reset();

    // timer->start(1000);

    // 启动子线程监听被动信号
    std::thread t_slave([=]
                        {
        while(!stop){
            //semaphone_slave.acquire();
             dataparser_slave->semaphone_slave.acquire();
          
            QByteArray byArr;
            byArr.resize(dataparser_slave->N);
      
            memcpy(byArr.data(),dataparser_slave->reply_order,dataparser_slave->N);
            CLog::getInstance()->log("应答MC, order: "+ QString::number(data_slave.order));
            // QString current_date_time1 = QDateTime::currentDateTime().toString(Qt::ISODateWithMs);
            // std::cout<<"应答: "<<current_date_time1.toStdString()<<std::endl;
            slave_socket->send(byArr);
            if(data_slave.order == 33){

                CLog::getInstance()->log("MC Receive, 执行发送轨迹规划参数");
                getTrajParam_Slot(data_slave.axis_num);
            }else if(data_slave.order == 35){

                CLog::getInstance()->log("MC Receive, 执行发送轨迹给机器人");
                sendToRBT_Slot(data_slave.axis_num);
            }
       } });
    t_slave.detach();
    return 1;
}

void MCOperator::startReceive()
{
    CLog::getInstance()->log("MC, 启动运动控制器接收消息");

    // sendData(6, 0, 0);
    trySendData(6, 0, 0, data_master.b_receive_start);
}

void MCOperator::close()
{
    stop = true;
    master_socket->close();
}

int MCOperator::getState()
{
}

void MCOperator::sendTrajParam(float zeropoint, float offset, uint8_t axisNum)
{
    // sendData(0, zeropoint, offset);
    trySendData(0, zeropoint, offset, data_master.b_receive_traj_param, axisNum);
}

void MCOperator::reset()
{
    std::cout << "" << std::endl;
    CLog::getInstance()->log("运动控制器请报错复位");
    // sendData(9, 0, 0);
    trySendData(5, 0, 0, data_master.b_receive_reset);
}

void MCOperator::sendRbtResult(float success)
{
    trySendData(7, success, 0, data_master.b_receive_rbt_result);
}

std::vector<float> MCOperator::getChainEncoders(bool &success)
{
    success = true;

    // sendData(1, 0, 0);
    // if(!waitData(data.bchainencoder))
    // {
    //     success = false;
    // }
    success = trySendData(1, 0, 0, data_master.b_chain_encoder);
    std::vector<float> val;
    val.push_back(data_master.encoder1);
    val.push_back(data_master.encoder2);

    std::cout << "获取拍照时刻悬挂链值" << std::endl;
    return val;
}
std::vector<float> MCOperator::getRealTimeEncoder(bool &success)
{
    success = true;
    // sendData(2, 0, 0);
    // waitData(data.brealtimeencoder);
    success = trySendData(2, 0, 0, data_master.b_realtime_encoder);
    std::vector<float> val;
    val.push_back(data_master.realtimeencoder1);
    val.push_back(data_master.realtimeencoder2);

    std::cout << "获取实施编码器数值" << std::endl;
    return val;
}

bool MCOperator::trySendData(uint8_t order, float v1, float v2, bool &flag, uint8_t axisNum)
{
    int N = 3;
    bool ret = false;
    send_mutex.lock();
    for (int i = 0; i < 3; i++)
    {
        sendData(order, v1, v2, axisNum);
        ret = waitData(flag);
        if (ret == true)
        {
            break;
        }
    }
    send_mutex.unlock();
    return ret;
}

void MCOperator::sendData(uint8_t order, float v1, float v2, uint8_t axisNum)
{
    QByteArray arry;
    // 报头
    int16_t head = qToBigEndian<int16_t>(0xabcd);
    // arry.append((char*)&head,2);
    // 编号
    if (count >= 9999)
    {
        count = 0;
    }
    else
    {
        count++;
    }

    int16_t count_BigEndian = qToBigEndian<int16_t>(count);
    arry.append((char *)&count_BigEndian, 2);

    // 轴号
    arry.append((char *)&axisNum, 1);

    // 指令
    arry.append(order);

    // 数据
    float d1 = qToBigEndian<float>(v1);
    float d2 = qToBigEndian<float>(v2);
    float d3 = qToBigEndian<float>(0);
    float d4 = qToBigEndian<float>(0);
    float d5 = qToBigEndian<float>(0);
    float d6 = qToBigEndian<float>(0);
    arry.append((char *)&d1, 4);
    arry.append((char *)&d2, 4);
    arry.append((char *)&d3, 4);
    arry.append((char *)&d4, 4);
    arry.append((char *)&d5, 4);
    arry.append((char *)&d6, 4);

    // 报尾
    unsigned char *crcdata = (unsigned char *)arry.data();
    u_int16_t trail = dataparser_master->do_crc(crcdata, dataparser_master->N_Data);

    // std::cout<<std::hex<<trail<<std::endl;

    auto trailToBingEndian = qToBigEndian(trail);
    arry.append((char *)&trailToBingEndian, 2);

    uint ret;
    arry.insert(0, (char *)&head, 2);
    ret = master_socket->send(arry);
}

// bool MCOperator::waitData(bool &flag)
// {
//     int i = 0;
//     while (!flag && i<300)
//     {
//         i++;
//         //        QCoreApplication::processEvents();
//         usleep(1e5);
//     }
//     //相应超时
//     if(flag == false){
//         return false;
//     }
//     else{
//         flag = false;
//         return true;
//     }
// }

bool MCOperator::waitData(bool &flag)
{
    QString strflag;
    if (&flag == &data_master.b_chain_encoder)
    {
        strflag = "b_chain_encoder";
    }
    else if (&flag == &data_master.b_realtime_encoder)
    {
        strflag = "b_realtime_encoder";
    }
    else if (&flag == &data_master.b_receive_rbt_result)
    {
        strflag = "b_receive_rbt_result";
    }
    else if (&flag == &data_master.b_receive_reset)
    {
        strflag = "b_receive_reset";
    }
    else if (&flag == &data_master.b_receive_traj_param)
    {
        strflag = "b_receive_traj_param";
    }

    bool success = false;
    for (int i = 0; i < 3; i++)
    {

        CLog::getInstance()->log("错误重试, " + strflag);
        if (dataparser_master->semaphore_master.tryAcquire(1, 400))
        {
            CLog::getInstance()->log("获得资源, " + strflag);
            if (flag)
            {
                CLog::getInstance()->log("标志成功, " + strflag);
                flag = false;
                success = true;
                break;
            }
        }
        // std::cout<<"等待超时"<<std::endl;
    }
    return success;
}

void MCOperator::readyRead_Slot(QByteArray buf)
{
    std::thread::id id = std::this_thread::get_id();
    // emit getTrajParam_Signal();
}

void MCOperator::getTrajParam_Slot(quint8 axisNum)
{
    emit getTrajParam_Signal(axisNum);
}

void MCOperator::sendToRBT_Slot(quint8 axisNum)
{
    emit sendToRBT_Signal(axisNum);
}

void MCOperator::mcWarning_Slot(quint16 num)
{
    // 应答
    sendData(36, 0, 0, num);
}

void MCOperator::checkState_Slot()
{
    if (preheart != data_slave.heart)
    {
        preheart = data_slave.heart;
        state = true;
        if (failcount != 0)
        {
            failcount = 0;
        }
    }
    else
    {
        if (++failcount > 3)
        {
            state = false;
        }
    }
}
