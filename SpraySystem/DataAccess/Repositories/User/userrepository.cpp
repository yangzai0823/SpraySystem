#include "userrepository.h"
#include <Include/Core/Config/jsonserializer.h>


UserRepository *UserRepository::userRepository = NULL;

QMutex UserRepository::mutex;

UserRepository::UserRepository()
{
    userProperty = new UserProperty();

    filename = (savepath+"user.json").toUtf8().data();

    QFile file(savepath+"user.json");
    if(!file.exists()){
        file.open(QIODevice::ReadWrite);
        file.write("");
        file.close();
    }
}


UserRepository::~UserRepository()
{
    delete userProperty;
}

UserRepository *UserRepository::getInstance()
{
    if(userRepository==NULL)
    {
        mutex.lock();
        if(userRepository==NULL)
        {
            userRepository = new UserRepository();
        }
        mutex.unlock();
    }
    return userRepository;
}

QList<std::shared_ptr<User>> UserRepository::list()
{

    Config c;

    vws::properties::JsonSerializer::fromJson(c,filename);
    auto userList = userProperty->load(c);

  productdata = userList;
  return userList;
}

int UserRepository::add(std::shared_ptr<User> user)
{
    productdata<<user;

    return saveToFile();
}

int UserRepository::update(std::shared_ptr<User> user)
{
    int ret =1;
    foreach(std::shared_ptr<User> u , productdata){
        if(u->Id == user->Id){
            u->Name = user->Name;
            u->Password = user->Password;
            u->Role = user->Role;
            u->Permissions = user->Permissions;

            ret = saveToFile();
            break;
        }
    }
    return ret;;
}

int UserRepository::remove(QString id)
{
    int index = -1;
    for(int i =0;i<productdata.length();i++){
        if(productdata.at(i)->Id==id){
            index = i;
            break;
        }
    }
    if(index!=-1){
        productdata.removeAt(index);
        saveToFile();
    }
    return 1;
}

std::shared_ptr<User> UserRepository::query(QString id)
{
    std::shared_ptr<User> user;
    foreach (auto item, productdata) {
        if(item->Id == id){
            user = item;
            break;
        }
    }
    return user;
}

std::shared_ptr<User> UserRepository::identify(QString name, QString pwd)
{
    QFileInfo fileInfo(savepath+"user.json");
    if(!fileInfo.exists())
    {
        auto tmp = std::make_shared<User>();
        tmp->Name = "临时用户";
        tmp->Permissions.append("用户信息");
        return tmp;
    }

    if(productdata.count()==0){
        list();
    }
    foreach (auto item, productdata) {
        if(item->Name == name){
            if(item->Password==pwd)
            {
                return item;
            }
            break;
        }
    }
    return nullptr;
}


int UserRepository::saveToFile()
{
    int ret =1;
    try {
        Config c;
        userProperty->save(c,productdata);
        ret = vws::properties::JsonSerializer::toJson(c,filename);
    } catch (QString ex) {
        logger->log(CLog::REEROR, "用户添加失败， "+ex);
        return -1;
    }
    return ret;
}
