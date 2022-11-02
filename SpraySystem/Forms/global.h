#ifndef GLOBAL_H
#define GLOBAL_H

#include "DataAccess/Models/User/user.h"
#include <memory>

extern std::shared_ptr<User> loginUser;     //登陆用户

class Global
{
public:
    Global();
};

#endif // GLOBAL_H
