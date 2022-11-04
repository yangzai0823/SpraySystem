#ifndef PLCDATAPASER_H
#define PLCDATAPASER_H

#include <QObject>
#include "Data/StructData.h"


class PLCDataPaser
{
public:

    PLCDataPaser();
    void DataPaser(QByteArray buf,vws::PLCData & data);
};

#endif // PLCDATAPASER_H
