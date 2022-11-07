#ifndef BASEDATAPASER_H
#define BASEDATAPASER_H
#include <QObject>

class BaseDataPaser
{
public:
    virtual ~BaseDataPaser(){};
    virtual void DataPaser(QByteArray buf) = 0;
};

#endif // BASEDATAPASER_H
