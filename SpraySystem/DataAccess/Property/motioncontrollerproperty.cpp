#include "motioncontrollerproperty.h"

MotionControllerProperty::MotionControllerProperty()
{

}

void MotionControllerProperty::save(Config c, QList<std::shared_ptr<MotionController>> mcs) const
{
    auto t = c.mapMakeChild("MCs");
    foreach(auto motionController , mcs)
    {
        Config config_child = t.listAppendNew();
        c.mapSetValue("Id",motionController->Id);
        c.mapSetValue("Name",motionController->Name);
        c.mapSetValue("Ip",motionController->Ip);
        c.mapSetValue("Port",motionController->Port);
    }

}

QList<std::shared_ptr<MotionController>>  MotionControllerProperty::load(const Config &c)
{

 QList<std::shared_ptr<MotionController>> mcs;

    //c.mapGetString("Users",&users);
    auto vList= c.mapGetChild("MCs");
    for(int i =0;i<vList.listLength();i++)
    {
        auto mc = std::make_shared<MotionController>();

        QString id;
        QString name;
        QString ip,port;
        float frame,exposure;
        vList.listChildAt(i).mapGetString("Id",&id);
        vList.listChildAt(i).mapGetString("Name",&name);
        vList.listChildAt(i).mapGetString("Ip",&ip);
        vList.listChildAt(i).mapGetString("Port",&port);

        mc->Id = id;
        mc->Name = name;
        mc->Ip = ip;
        mc ->Port = port;
        mcs.append(mc);
    }

   

    return mcs;
}
