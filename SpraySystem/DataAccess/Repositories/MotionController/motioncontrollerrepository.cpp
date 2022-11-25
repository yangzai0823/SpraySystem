#include "motioncontrollerrepository.h"


MotionControllerRepository *MotionControllerRepository::mcRepository = NULL;

QMutex MotionControllerRepository::mutex;

MotionControllerRepository::MotionControllerRepository()
{
    mcProperty = new MotionControllerProperty();
    filename = (savepath+"mc.json").toUtf8().data();
    QFile file(savepath+"mc.json");
    if(!file.exists()){
        file.open(QIODevice::ReadWrite);
        file.close();
    }
}

int MotionControllerRepository::saveToFile()
{
    int ret =1;
    try {
        Config c;
        mcProperty->save(c,data);
        ret = vws::properties::JsonSerializer::toJson(c,filename);
    } catch (QString ex) {
        logger->log( "plc修改失败， "+ex,CLog::REEROR);
        return -1;
    }
    return ret;
}

MotionControllerRepository *MotionControllerRepository::getInstance()
{
    if(mcRepository==NULL)
    {
        mutex.lock();
        if(mcRepository==NULL)
        {
            mcRepository = new MotionControllerRepository();
        }
        mutex.unlock();
    }
    return mcRepository;
}

int MotionControllerRepository::update(std::shared_ptr<MotionController> plc)
{
    if(data==nullptr){
        data = std::make_shared<MotionController>();
    }
    data->Id = plc->Id;
    data->Name = plc->Name;
    data->Ip = plc->Ip;
    data->Port = plc->Port;

    saveToFile();
}

std::shared_ptr<MotionController> MotionControllerRepository::query()
{
    Config c;

    vws::properties::JsonSerializer::fromJson(c,filename);
    data = mcProperty->load(c);

    return data;
}
