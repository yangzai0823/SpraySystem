#ifndef PLCDATAPASER_H
#define PLCDATAPASER_H

#include <QObject>
#include "Data/StructData.h"
#include "Data/basedatapaser.h"

class PLCDataPaser:public QObject, public BaseDataPaser
{
    Q_OBJECT
public:

    PLCDataPaser();

    void DataPaser(QByteArray buf);
    vws::PLCData *plcdata;
private:

    void DataPaser(QByteArray buf,vws::PLCData *data);

signals:
    void readyRead_Signal();
};

#endif // PLCDATAPASER_H
