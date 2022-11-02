#ifndef PLC_H
#define PLC_H
#include <QObject>

class PLC
{
public:
    PLC();
    QString Id;
    QString Name;
    QString Ip;
    QString Port;
};

#endif // PLC_H
