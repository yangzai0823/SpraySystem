#include "qtsocketclient.h"


QtSocketClient::QtSocketClient()
{
//    socketThread = new QThread();
    //主线程
    socketThread = new QThread(this);
//    connect(socketThread, SIGNAL(started()), this, SLOT(OnThreadStarted()));
//    connect(socketThread, SIGNAL(finished()), this, SLOT(OnThreadFinished()));
    moveToThread(socketThread);
    socketThread->start();
}

QtSocketClient::~QtSocketClient()
{
    socketThread->quit();
    socketThread->wait();
    delete  socketThread;
}

int QtSocketClient::connectServer(QString ip,int port)
{
    bool ret;
    tcpsocket = new QTcpSocket();
    try {
        tcpsocket->connectToHost((QHostAddress)ip, port);

        ret = tcpsocket->waitForConnected(3000);
        if(!ret)
        {
            return -1;
        }

        connect(tcpsocket,SIGNAL(readyRead()),this,SLOT(readyRead_Slot()));
    } catch (char* msg) {
        return -1;
    }
    //主动和服务器进行连接

    return 1;
}

int QtSocketClient::send(QString msg)
{
    return tcpsocket->write(msg.toUtf8().data());
}

int QtSocketClient::send(QByteArray msg)
{
    auto ret = tcpsocket->write(msg);
//    std::cout<<"发送成功，字节书: "<<ret<<std::endl;
}


void QtSocketClient::close()
{
    if(tcpsocket==nullptr)
        return;
    tcpsocket->close();
}

void QtSocketClient::readyRead_Slot()//定义接收信号的槽
{
    std::thread::id id = std::this_thread::get_id();
//    std::cout << "socketclient slot 线程ID: "<< id << std::endl;

    QByteArray buf=tcpsocket->readAll();

    emit readyRead_Signal(buf);

}
