#include "motioncontrollerproperty.h"

MotionControllerProperty::MotionControllerProperty()
{
}

void MotionControllerProperty::save(Config c, QList<std::shared_ptr<MotionController>> mcs) const
{
    auto t = c.mapMakeChild("MCs");
    foreach (auto motionController, mcs)
    {
        Config config_child = t.listAppendNew();
        config_child.mapSetValue("Id", motionController->Id);
        config_child.mapSetValue("Name", motionController->Name);
        config_child.mapSetValue("Ip", motionController->Ip);
        config_child.mapSetValue("Port", motionController->Port);
        config_child.mapSetValue("Port2", motionController->Port2);
    }
}

QList<std::shared_ptr<MotionController>> MotionControllerProperty::load(const Config &c)
{

    QList<std::shared_ptr<MotionController>> mcs;

    // c.mapGetString("Users",&users);
    auto vList = c.mapGetChild("MCs");
    for (int i = 0; i < vList.listLength(); i++)
    {
        auto mc = std::make_shared<MotionController>();

        QString id;
        QString name;
        QString ip, port;
        QString port2;
        vList.listChildAt(i).mapGetString("Id", &id);
        vList.listChildAt(i).mapGetString("Name", &name);
        vList.listChildAt(i).mapGetString("Ip", &ip);
        vList.listChildAt(i).mapGetString("Port", &port);
        vList.listChildAt(i).mapGetString("Port2", &port2);

        mc->Id = id;
        mc->Name = name;
        mc->Ip = ip;
        mc->Port = port;
        mc->Port2 = port2;
        mcs.append(mc);
    }

    return mcs;
}
