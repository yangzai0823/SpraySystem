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
    int update(std::shared_ptr<MotionController> mc);
    std::shared_ptr<MotionController> query();
private:
   MotionControllerRepository();
   int saveToFile();
   static MotionControllerRepository *mcRepository;
   static QMutex mutex;
   MotionControllerProperty *mcProperty;
   std::shared_ptr<MotionController> data;
};

#endif // MOTIONCONTROLLERREPOSITORY_H
