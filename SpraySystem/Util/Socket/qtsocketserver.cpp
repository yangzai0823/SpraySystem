#include "qtsocketserver.h"

QtSocketServer::QtSocketServer(QObject *parent) : QObject(parent)
{
    tcpserver = new QTcpServer();
}

int QtSocketServer::startServer(QString ip,int port)
{
    try {
        tcpserver->listen((QHostAddress)ip,port);
        //tcpserver->listen(QHostAddress::Any,port);  //监听端口号

         connect(tcpserver,SIGNAL(newConnection()),this,SLOT(newConnection_Slot()));
    } catch (char* msg) {
        return -1;
    }
    return 1;
}

int QtSocketServer::send(QString msg){
    if(tcpsocket == NULL)
        return -1;
   return tcpsocket->write(msg.toUtf8().data());
}

void QtSocketServer::close()
{
//    if(tcpsocket!=nullptr && tcpsocket!=NULL)
////        tcpsocket->close();
//    if(tcpserver!=nullptr && tcpserver!=nullptr)
//        tcpserver->close();
}

void QtSocketServer::newConnection_Slot()
{
    tcpsocket=tcpserver->nextPendingConnection(); //获取已经连接的客户端套接字
    connect(tcpsocket,SIGNAL(readyRead()),this,SLOT(readyRead_Slot()));

    QString ip = tcpsocket->peerAddress().toString();
    int port = tcpsocket->peerPort();
    emit newConnection_Signal(ip,port);
}

void QtSocketServer::readyRead_Slot()
{
    QString buf;
    buf=tcpsocket->readAll();
    emit readyRead_Signal(buf);
}
