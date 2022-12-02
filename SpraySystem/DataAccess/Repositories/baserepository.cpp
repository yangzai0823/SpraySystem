#include "baserepository.h"
#include <QApplication>

BaseRepository::BaseRepository()
{
    savepath = qApp->applicationDirPath()+"/Data/";
    QDir dir(savepath);
    if(!dir.exists())
    {
        dir.mkdir(savepath);
    }
    logger = CLog::getInstance();
}
