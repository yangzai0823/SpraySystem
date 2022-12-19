#ifndef MOTIONCONTROLLER_H
#define MOTIONCONTROLLER_H
#include <QObject>

class MotionController
{
public:
    MotionController();
    QString Id;
    QString Name;
    QString Ip;
    QString Port;
    QString Port2;
};

#endif // MOTIONCONTROLLER_H
