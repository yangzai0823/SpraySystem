#include "qtsocketclient.h"

QtSocketClient::QtSocketClient(BaseDataPaser *paser) {
  _paser = paser;
  //    socketThread = new QThread();
  //主线程
  socketThread = new QThread(this);
  //    connect(socketThread, SIGNAL(started()), this, SLOT(OnThreadStarted()));
  //    connect(socketThread, SIGNAL(finished()), this,
  //    SLOT(OnThreadFinished()));
  moveToThread(socketThread);
  socketThread->start();

  connect(this, SIGNAL(send_Signal(QByteArray)), this,
          SLOT(send_Slot(QByteArray)));
}

QtSocketClient::~QtSocketClient() {
  socketThread->quit();
  socketThread->wait();
  delete socketThread;
}

int QtSocketClient::connectServer(QString ip, int port) {
  bool ret;
  tcpsocket = new QTcpSocket();
  try {
    tcpsocket->connectToHost((QHostAddress)ip, port);

    ret = tcpsocket->waitForConnected(3000);
    if (!ret) {
      return -1;
    }

    connect(tcpsocket, SIGNAL(readyRead()), this, SLOT(readyRead_Slot()));
  } catch (char *msg) {
    return -1;
  }
  //主动和服务器进行连接

  return 1;
}

void QtSocketClient::connect_Slot(QString ip, int port) {
  bool ret;
  tcpsocket = new QTcpSocket();
  try {
    tcpsocket->connectToHost((QHostAddress)ip, port);

    ret = tcpsocket->waitForConnected(3000);

    connect(tcpsocket, SIGNAL(readyRead()), this, SLOT(readyRead_Slot()));
  } catch (char *msg) {
  }
}

int QtSocketClient::send(QString msg) {
  if (!tcpsocket) {
    return -1;
  }
  return tcpsocket->write(msg.toUtf8().data());
}

int QtSocketClient::send(QByteArray msg) {
  emit send_Signal(msg);
  return 0;
}

int QtSocketClient::send(char *msg) {
  emit send_Signal(msg);
  return 0;
}

void QtSocketClient::close() {
  if (tcpsocket == nullptr)
    return;
  tcpsocket->close();
}

void QtSocketClient::readyRead_Slot() //定义接收信号的槽
{
  if (!tcpsocket) {
    return;
  }
  std::thread::id id = std::this_thread::get_id();

  QByteArray buf = tcpsocket->readAll();

  //调用相应的数据解释
  _paser->DataPaser(buf);
}

void QtSocketClient::send_Slot(QByteArray msg) {
  if (!tcpsocket) {
    return;
  }
  auto ret = tcpsocket->write(msg);
  if (ret < 0) {
    std::cout << "socket 发送失败" << std::endl;
  }
}