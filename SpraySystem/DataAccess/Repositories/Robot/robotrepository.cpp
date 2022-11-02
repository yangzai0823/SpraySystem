#include "robotrepository.h"

RobotRepository *RobotRepository::robotRepository = NULL;

QMutex RobotRepository::mutex;

RobotRepository::RobotRepository()
{
    filename = (savepath+"robot.json").toUtf8().data();

    QFile file(savepath+"robot.json");
    if(!file.exists()){
        file.open(QIODevice::ReadWrite);
        file.write("");
        file.close();
    }
}

int RobotRepository::saveToFile()
{
    int ret =1;
    try {
        Config c;
        robotProperty->save(c,data);
        ret = vws::properties::JsonSerializer::toJson(c,filename);
    } catch (QString ex) {
        logger->log(CLog::REEROR, "机器人添加失败， "+ex);
        return -1;
    }
    return ret;
}

RobotRepository *RobotRepository::getInstance()
{
    if(robotRepository==NULL)
    {
        mutex.lock();
        if(robotRepository==NULL)
        {
            robotRepository = new RobotRepository();
        }
        mutex.unlock();
    }
    return robotRepository;
}

QList<std::shared_ptr<Robot> > RobotRepository::list()
{
    Config c;

    vws::properties::JsonSerializer::fromJson(c,filename);
    auto robotlist = robotProperty->load(c);

  data = robotlist;
  return robotlist;
}

int RobotRepository::save(QList<std::shared_ptr<Robot> > lstRobot)
{
    data = lstRobot;
    saveToFile();
}

std::shared_ptr<Robot> RobotRepository::query(QString name)
{
    std::shared_ptr<Robot> robot;
    foreach (auto item, data) {
        if(item->Name == name){
            robot = item;
            break;
        }
    }
    return robot;
}
