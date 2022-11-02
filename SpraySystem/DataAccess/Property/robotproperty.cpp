#include "robotproperty.h"

RobotProperty::RobotProperty()
{

}

void RobotProperty::save(Config c, QList<std::shared_ptr<Robot> > robots) const
{
    auto t = c.mapMakeChild("Robots");
    foreach(auto c , robots)
    {
        Config config_child = t.listAppendNew();
        config_child.mapSetValue("Id",c->Id);
        config_child.mapSetValue("Name",c->Name);
        config_child.mapSetValue("Ip",c->Ip);
        config_child.mapSetValue("Port",c->Port);
    }
}

QList<std::shared_ptr<Robot> > RobotProperty::load(const Config &c)
{
    QList<std::shared_ptr<Robot>> robots;

    auto vList= c.mapGetChild("Robots");
    for(int i =0;i<vList.listLength();i++)
    {
        auto robot = std::make_shared<Robot>();

        QString id;
        QString name;
        QString ip,port;
        vList.listChildAt(i).mapGetString("Id",&id);
        vList.listChildAt(i).mapGetString("Name",&name);
        vList.listChildAt(i).mapGetString("Ip",&ip);
        vList.listChildAt(i).mapGetString("Port",&port);

        robot->Id = id;
        robot->Name = name;
        robot->Ip = ip;
        robot ->Port = port;

        robots.append(robot);
    }

    return robots;
}
