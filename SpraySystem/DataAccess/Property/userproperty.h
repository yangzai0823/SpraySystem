#ifndef USERPROPERTY_H
#define USERPROPERTY_H
#include "DataAccess/Models/User/user.h"
#include "Include/Core/Config/jsonserializer.h"
#include "Include/Core/Config/config.hpp"
#include <memory>
using Config = vws::properties::Config;
class UserProperty
{
public:
    UserProperty();
    void save(Config c,QList<std::shared_ptr<User>> user) const;
    QList<std::shared_ptr<User>> load(const Config& c);
};

#endif // USERPROPERTY_H
