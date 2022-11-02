#include "motioncontrollerproperty.h"

MotionControllerProperty::MotionControllerProperty()
{

}

void MotionControllerProperty::save(Config c, std::shared_ptr<MotionController> motionController) const
{
    c.mapSetValue("Id",motionController->Id);
    c.mapSetValue("Name",motionController->Name);
    c.mapSetValue("Ip",motionController->Ip);
    c.mapSetValue("Port",motionController->Port);
}

std::shared_ptr<MotionController> MotionControllerProperty::load(const Config &c)
{
    auto motionController = std::make_shared<MotionController>();

    QString id;
    QString name;
    QString ip,port;
    c.mapGetString("Id",&id);
    c.mapGetString("Name",&name);
    c.mapGetString("Ip",&ip);
    c.mapGetString("Port",&port);

    motionController->Id = id;
    motionController->Name = name;
    motionController->Ip = ip;
    motionController ->Port = port;

    return motionController;
}
