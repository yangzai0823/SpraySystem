#include "cameraproperty.h"

CameraProperty::CameraProperty()
{

}
void CameraProperty::save(Config c, QList<std::shared_ptr<Camera> > cameras) const
{
    auto t = c.mapMakeChild("Cameras");
    foreach(auto c , cameras)
    {
        Config config_child = t.listAppendNew();
        config_child.mapSetValue("Id",c->Id);
        config_child.mapSetValue("Name",c->Name);
        config_child.mapSetValue("Ip",c->Ip);
        config_child.mapSetValue("Port",c->Port);
        config_child.mapSetValue("Frame",c->Frame);
        config_child.mapSetValue("Exposure",c->Exposure);
    }
}

QList<std::shared_ptr<Camera> > CameraProperty::load(const Config &c)
{
    QList<std::shared_ptr<Camera>> cameras;

    //c.mapGetString("Users",&users);
    auto vList= c.mapGetChild("Cameras");
    for(int i =0;i<vList.listLength();i++)
    {
        auto camera = std::make_shared<Camera>();

        QString id;
        QString name;
        QString ip,port;
        float frame,exposure;
        vList.listChildAt(i).mapGetString("Id",&id);
        vList.listChildAt(i).mapGetString("Name",&name);
        vList.listChildAt(i).mapGetString("Ip",&ip);
        vList.listChildAt(i).mapGetString("Port",&port);
        vList.listChildAt(i).mapGetFloat("Frame",&frame);
        vList.listChildAt(i).mapGetFloat("Exposure",&exposure);

        camera->Id = id;
        camera->Name = name;
        camera->Ip = ip;
        camera ->Port = port;
        camera->Frame = frame;
        camera->Exposure = exposure;
        cameras.append(camera);
    }

    return cameras;
}
