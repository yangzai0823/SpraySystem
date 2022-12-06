#ifndef MCDATAPASER_H
#define MCDATAPASER_H
#include <QObject>
#include <iostream>
#include <mutex>
#include "Data/StructData.h"
#include "Data/basedatapaser.h"

using MCData = vws::MCData;

class mcdatapaser:public QObject ,public BaseDataPaser
{
  Q_OBJECT
public:

    mcdatapaser();

    void DataPaser(QByteArray buf);
    u_int16_t do_crc(unsigned char *ptr, int len);
    MCData *mcData;
    std::mutex parse_mutex;

private:
  QByteArray wholeBuf;

  void DataPaser(QByteArray buf,MCData * data);
  void parseData(unsigned char* v1, vws::MCData *data);
signals:
    void getTrajParam_Signal();
    /** @brief 应答并执行发送操作 */
    void sendToRBT_Signal(u_int16_t num);
    /** @brief 应答并执行发送操作 */
    void mcWarning_Signal(u_int16_t num);
};

#endif // MCDATAPASER_H
