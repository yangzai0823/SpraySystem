#ifndef HEADDISTANCEMONITOR_H
#define HEADDISTANCEMONITOR_H
#include <QObject>

class HeadDistanceMonitor: public QObject
{
    Q_OBJECT
public:
     void Stop();
private:
    double distance = 0;
    bool flag_stop = false;
signals:
    void outOfLimit_Signal();
public slots:
    void Work_Slot(float camera_encoder);
};
#endif // HEADDISTANCEMONITOR_H
