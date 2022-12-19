#include "motioncontrollerrepository.h"

MotionControllerRepository *MotionControllerRepository::mcRepository = NULL;

QMutex MotionControllerRepository::mutex;

MotionControllerRepository::MotionControllerRepository()
{
    mcProperty = new MotionControllerProperty();
    filename = (savepath + "mc.json").toUtf8().data();
    QFile file(savepath + "mc.json");
    if (!file.exists())
    {
        file.open(QIODevice::ReadWrite);
        file.close();
    }
}

int MotionControllerRepository::saveToFile()
{
    int ret = 1;
    try
    {
        Config c;
        mcProperty->save(c, data);
        ret = vws::properties::JsonSerializer::toJson(c, filename);
    }
    catch (QString ex)
    {
        logger->log("plc修改失败， " + ex, CLog::REEROR);
        return -1;
    }
    return ret;
}

MotionControllerRepository *MotionControllerRepository::getInstance()
{
    if (mcRepository == NULL)
    {
        mutex.lock();
        if (mcRepository == NULL)
        {
            mcRepository = new MotionControllerRepository();
        }
        mutex.unlock();
    }
    return mcRepository;
}

QList<std::shared_ptr<MotionController>> MotionControllerRepository::list()
{
    Config c;

    vws::properties::JsonSerializer::fromJson(c, filename);
    auto mclist = mcProperty->load(c);

    data = mclist;
    return mclist;
}

int MotionControllerRepository::update(std::shared_ptr<MotionController> mc)
{
    int ret = 1;
    foreach (std::shared_ptr<MotionController> u, data)
    {
        if (u->Id == mc->Id)
        {
            u->Id = mc->Id;
            u->Name = mc->Name;
            u->Ip = mc->Ip;
            u->Port = mc->Port;
            u->Port2 = mc->Port2;
            break;
        }
    }
    ret = saveToFile();
    return ret;
}

int MotionControllerRepository::remove(QString id)
{
    int index = -1;
    for (int i = 0; i < data.length(); i++)
    {
        if (data.at(i)->Id == id)
        {
            index = i;
            break;
        }
    }
    if (index != -1)
    {
        data.removeAt(index);
        saveToFile();
    }
    return 1;
}

int MotionControllerRepository::save(QList<std::shared_ptr<MotionController>> lstMC)
{
    data = lstMC;
    saveToFile();
}

std::shared_ptr<MotionController> MotionControllerRepository::query(QString name)
{
    std::shared_ptr<MotionController> mc;
    foreach (auto item, data)
    {
        if (item->Name == name)
        {
            mc = item;
            break;
        }
    }
    return mc;
}
