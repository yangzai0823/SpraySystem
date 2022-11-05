#ifndef QTSOCKETCLIENT_H
#define QTSOCKETCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>
#include <iostream>
#include <QThread>

class QtSocketClient : public QThread
{
    Q_OBJECT
public:
     QtSocketClient();
     ~QtSocketClient();
    
    int connectServer(QString ip,int port);
    int send(QString msg);
//    int send(char* msg);
    int send(QByteArray msg);
    void close();
public:
    QTcpSocket *tcpsocket= NULL;
    QThread *socketThread;
public slots:
     void readyRead_Slot();
public slots:
      void connect_Slot(QString ip,int port);
      void send_Slot(QByteArray msg);
signals:
     void readyRead_Signal(QByteArray buf);
     void send_Signal(QByteArray msg);

};

#endif // QTSOCKETCLIENT_H
