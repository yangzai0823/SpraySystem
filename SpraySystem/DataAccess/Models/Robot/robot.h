#ifndef ROBOT_H
#define ROBOT_H

#include <QObject>

class Robot
{
public:
    Robot();
    QString Id;
    QString Name;
    QString Ip;
    QString Port;
};

#endif // ROBOT_H
