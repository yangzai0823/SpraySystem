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
};

#endif // MOTIONCONTROLLER_H
