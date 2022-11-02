#ifndef PLCREPOSITORY_H
#define PLCREPOSITORY_H

#include "DataAccess/Models/PLC/plc.h"
#include "DataAccess/Property/plcproperty.h"
#include "DataAccess/Repositories/baserepository.h"
#include <memory>
#include <QMutex>


class PLCRepository: public BaseRepository
{
public:
     ~PLCRepository();
     static PLCRepository *getInstance();
     int update(std::shared_ptr<PLC> plc);
     std::shared_ptr<PLC> query();
private:
    PLCRepository();
    int saveToFile();
    static PLCRepository *plcRepository;
    static QMutex mutex;
    PLCProperty *plcProperty;
    std::shared_ptr<PLC> data;
};

#endif // PLCREPOSITORY_H
