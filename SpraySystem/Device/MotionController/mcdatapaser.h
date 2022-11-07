#ifndef MCDATAPASER_H
#define MCDATAPASER_H
#include <QObject>
#include <iostream>
#include "Data/StructData.h"
#include "Data/basedatapaser.h"

using MCData = vws::MCData;

class mcdatapaser:public BaseDataPaser
{
public:

    mcdatapaser();

    void DataPaser(QByteArray buf);
    MCData *mcData;
private:
  void DataPaser(QByteArray buf,MCData * data);
};

#endif // MCDATAPASER_H
