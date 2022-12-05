#ifndef MCDATAPASER_H
#define MCDATAPASER_H
#include <QObject>
#include <iostream>
#include "Data/StructData.h"
#include "Data/basedatapaser.h"

using MCData = vws::MCData;

class mcdatapaser:public QObject ,public BaseDataPaser
{
  Q_OBJECT
public:

    mcdatapaser();

    void DataPaser(QByteArray buf);
    int16_t do_crc(unsigned char *ptr, int len);
    MCData *mcData;
private:
  void DataPaser(QByteArray buf,MCData * data);
  void parseData(unsigned char* v1, vws::MCData *data);
signals:
    void getTrajParam_Signal();
    void sendToRBT_Signal();
};

#endif // MCDATAPASER_H
