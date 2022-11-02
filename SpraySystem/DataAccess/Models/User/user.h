#ifndef USER_H
#define USER_H

#include <QObject>

class User
{
public:
    User();
    //User(const User &user);
    QString Id;
    QString Name;
    QString Password;
    QString Role;
    QStringList Permissions;
};

#endif // USER_H
