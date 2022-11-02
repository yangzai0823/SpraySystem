#ifndef PLCDATAPASER_H
#define PLCDATAPASER_H

#include <QObject>
//#include "PLCData.h"


class PLCDataPaser
{
public:
    struct PLCData{
        int heart;
        float laser1;
        float laser2;
        float laser3;
        float laser4;
        int cell;
    };

    PLCDataPaser();
    void DataPaser(QByteArray buf,PLCData & data);
};

#endif // PLCDATAPASER_H
