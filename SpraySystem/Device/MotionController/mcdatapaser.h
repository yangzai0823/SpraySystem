#ifndef MCDATAPASER_H
#define MCDATAPASER_H
#include <QObject>
#include <iostream>
class mcdatapaser
{
public:
    struct MCData{
        std::vector<uint8_t> headvect;
        int encoder1;
        int encoder2;
        int realtimeencoder1;
        int realtimeencoder2;
        int getparam;
        int heart;

        bool bchainencoder = false;
        bool brealtimeencoder = false;
        bool btrajparam = false;
    };
    mcdatapaser();
    void DataPaser(QByteArray buf,MCData & data);
};

#endif // MCDATAPASER_H
