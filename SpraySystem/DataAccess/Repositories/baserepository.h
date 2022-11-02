#ifndef BASEREPOSITORY_H
#define BASEREPOSITORY_H

#include <QString>
#include "Util/Log/clog.h"

class BaseRepository
{
public:
    BaseRepository();
protected:
    QString savepath;
    std::string filename;
    CLog *logger;
};

#endif // BASEREPOSITORY_H
