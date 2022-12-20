#ifndef MCDATAPASER_H
#define MCDATAPASER_H
#include <QObject>
#include <iostream>
#include <mutex>
#include "Data/StructData.h"
#include "Data/basedatapaser.h"
#include <QSemaphore>

using MCData = vws::MCData;

class mcdatapaser : public QObject, public BaseDataPaser
{
  Q_OBJECT
public:
  mcdatapaser();

  void DataPaser(QByteArray buf);

  u_int16_t do_crc(unsigned char *ptr, int len);
  MCData *mcData;
  std::mutex parse_mutex;
  QSemaphore semaphone_slave;
  QSemaphore semaphore_master;
  int N = 32;
  int N_Data = 28;
  char reply_order[32];

private:
  QByteArray wholeBuf;

  void DataPaser(QByteArray buf, MCData *data);
  void parseData(char *v1, vws::MCData *data);
};

#endif // MCDATAPASER_H
