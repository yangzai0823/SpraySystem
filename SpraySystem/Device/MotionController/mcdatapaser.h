#ifndef MCDATAPASER_H
#define MCDATAPASER_H
#include <QObject>
#include <iostream>
#include "Data/StructData.h"

using MCData = vws::MCData;

class mcdatapaser
{
public:

    mcdatapaser();
    void DataPaser(QByteArray buf,MCData & data);
};

#endif // MCDATAPASER_H
