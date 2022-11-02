#ifndef CAMERA_H
#define CAMERA_H
#include <QObject>

class Camera
{
public:
    Camera();
    QString Id;
    QString Name;
    QString Ip;
    QString Port;
    float Exposure;
    float Frame;
};

#endif // CAMERA_H
