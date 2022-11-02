#include "userproperty.h"
#include <ostream>
#include <fstream>
#include "Include/Core/Config/jsonserializer.h"


UserProperty::UserProperty()
{

}

void UserProperty::save(Config c,QList<std::shared_ptr<User>> users) const
{
    auto t = c.mapMakeChild("Users");
    foreach(auto u , users)
    {
        Config config_child = t.listAppendNew();
        config_child.mapSetValue("Id",u->Id);
        config_child.mapSetValue("Name",u->Name);
        config_child.mapSetValue("Password",u->Password);
        config_child.mapSetValue("Role",u->Role);
        config_child.mapSetValue("Permissions",u->Permissions.join(","));
    }
}

QList<std::shared_ptr<User>>  UserProperty::load(const Config &c)
{
    QList<std::shared_ptr<User>> users;

    //c.mapGetString("Users",&users);
    auto vList= c.mapGetChild("Users");
    for(int i =0;i<vList.listLength();i++)
    {
        auto user = std::make_shared<User>();

        QString name;
        QString pwd;
        QString id;
        QString role;
        QString permissions;
        vList.listChildAt(i).mapGetString("Id",&id);
        vList.listChildAt(i).mapGetString("Name",&name);
        vList.listChildAt(i).mapGetString("Password",&pwd);
        vList.listChildAt(i).mapGetString("Role",&role);
        vList.listChildAt(i).mapGetString("Permissions",&permissions);

        user->Id = id;
        user->Name= name;
        user->Password = pwd;
        user->Role = role;

        user->Permissions = permissions.split(",");

        users.append(user);
    }

    return users;
}
