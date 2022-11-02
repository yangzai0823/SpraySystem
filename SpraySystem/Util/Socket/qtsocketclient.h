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
private:
    QTcpSocket *tcpsocket= NULL;
    QThread *socketThread;
private slots:
     void readyRead_Slot();

signals:
     void readyRead_Signal(QByteArray buf);

};

#endif // QTSOCKETCLIENT_H
