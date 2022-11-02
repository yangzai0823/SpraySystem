#ifndef ROBOTREPOSITORY_H
#define ROBOTREPOSITORY_H

#include "DataAccess/Models/Robot/robot.h"
#include "DataAccess/Property/robotproperty.h"
#include "DataAccess/Repositories/baserepository.h"
#include <memory>
#include <QMutex>


class RobotRepository: public BaseRepository
{
public:
    ~RobotRepository();
    static RobotRepository *getInstance();
    QList<std::shared_ptr<Robot>> list();
    int save(QList<std::shared_ptr<Robot>> lstRobot);
    std::shared_ptr<Robot> query(QString name);

private:
    RobotRepository();
    int saveToFile();
    static RobotRepository *robotRepository;
    static QMutex mutex;
    RobotProperty *robotProperty;
    QList<std::shared_ptr<Robot>> data;
};

#endif // ROBOTREPOSITORY_H
