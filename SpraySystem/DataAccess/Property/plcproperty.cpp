#include "plcproperty.h"

PLCProperty::PLCProperty()
{

}

void PLCProperty::save(Config c, std::shared_ptr<PLC> plc) const
{
    c.mapSetValue("Id",plc->Id);
    c.mapSetValue("Name",plc->Name);
    c.mapSetValue("Ip",plc->Ip);
    c.mapSetValue("Port",plc->Port);
}

std::shared_ptr<PLC> PLCProperty::load(const Config &c)
{
    auto plc = std::make_shared<PLC>();

    QString id;
    QString name;
    QString ip,port;
    c.mapGetString("Id",&id);
    c.mapGetString("Name",&name);
    c.mapGetString("Ip",&ip);
    c.mapGetString("Port",&port);

    plc->Id = id;
    plc->Name = name;
    plc->Ip = ip;
    plc ->Port = port;

    return plc;
}
