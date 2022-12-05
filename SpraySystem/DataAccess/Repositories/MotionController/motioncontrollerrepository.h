#ifndef MOTIONCONTROLLERREPOSITORY_H
#define MOTIONCONTROLLERREPOSITORY_H


#include "DataAccess/Models/MotionController/motioncontroller.h"
#include "DataAccess/Property/motioncontrollerproperty.h"
#include "DataAccess/Repositories/baserepository.h"
#include <memory>
#include <QMutex>


class MotionControllerRepository: public BaseRepository
{
public:
    static MotionControllerRepository *getInstance();
    QList<std::shared_ptr<MotionController>> list();
    int update(std::shared_ptr<MotionController> mc);
    int save(QList<std::shared_ptr<MotionController>> lstMC);
    int remove(QString id);
    std::shared_ptr<MotionController> query(QString name);
private:
    MotionControllerRepository();
    int saveToFile();
    static MotionControllerRepository *mcRepository;
    static QMutex mutex;
    MotionControllerProperty *mcProperty;
    QList<std::shared_ptr<MotionController>> data;
};

#endif // MOTIONCONTROLLERREPOSITORY_H
