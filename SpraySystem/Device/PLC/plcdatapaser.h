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
    bool init = false;  //是否初始化，第一次接收plc信号，初始化相机高低位信号
    bool pre_camera_b = false;
    bool pre_camera_u = false;
    bool pre_laser_b = false;
    bool pre_laser_u = false;

    bool isemit; //是否触发信号
private:
    void DataPaser(QByteArray buf,vws::PLCData *data);

signals:
    void readyRead_Signal();
};

#endif // PLCDATAPASER_H
