#ifndef USERREPOSITORY_H
#define USERREPOSITORY_H


#include "DataAccess/Models/User/user.h"
#include "DataAccess/Property/userproperty.h"
#include "DataAccess/Repositories/baserepository.h"
#include <memory>
#include <QMutex>

class UserRepository : public BaseRepository
{
public:
    ~UserRepository();
    static UserRepository *getInstance();
    QList<std::shared_ptr<User>> list();
    int add(std::shared_ptr<User> user);
    int update(std::shared_ptr<User> user);
    int remove(QString id);
    std::shared_ptr<User> query(QString id);
    std::shared_ptr<User> identify(QString name,QString pwd);

private:
    UserRepository();
    int saveToFile();
    static UserRepository *userRepository;
    static QMutex mutex;
    UserProperty *userProperty;
    QList<std::shared_ptr<User>> productdata;
};

#endif // USERREPOSITORY_H
