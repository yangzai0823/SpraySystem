#ifndef QTSOCKETSERVER_H
#define QTSOCKETSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QHostAddress>
#include <iostream>

class QtSocketServer: public QObject
{
    Q_OBJECT
public:
     explicit QtSocketServer(QObject *parent = nullptr);
    int startServer(QString ip,int port);
    int send(QString msg);
    void close();
//    QTcpSocket *tcpsocket = NULL;
private:
    QTcpSocket *tcpsocket = NULL;
    QTcpServer *tcpserver = nullptr;
private slots:
    void readyRead_Slot();
    void newConnection_Slot();
signals:
    void newConnection_Signal(QString ip,int port);
    void readyRead_Signal(QString msg);
};

#endif // QTSOCKETSERVER_H
