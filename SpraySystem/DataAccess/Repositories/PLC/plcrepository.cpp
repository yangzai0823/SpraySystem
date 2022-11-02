#include "plcrepository.h"



PLCRepository *PLCRepository::plcRepository = NULL;

QMutex PLCRepository::mutex;

PLCRepository::PLCRepository()
{
    plcProperty = new PLCProperty();
    filename = (savepath+"plc.json").toUtf8().data();
    QFile file(savepath+"plc.json");
    if(!file.exists()){
        file.open(QIODevice::ReadWrite);
        file.close();
    }
}

int PLCRepository::saveToFile()
{
    int ret =1;
    try {
        Config c;
        plcProperty->save(c,data);
        ret = vws::properties::JsonSerializer::toJson(c,filename);
    } catch (QString ex) {
        logger->log(CLog::REEROR, "plc修改失败， "+ex);
        return -1;
    }
    return ret;
}

PLCRepository *PLCRepository::getInstance()
{
    if(plcRepository==NULL)
    {
        mutex.lock();
        if(plcRepository==NULL)
        {
            plcRepository = new PLCRepository();
        }
        mutex.unlock();
    }
    return plcRepository;
}

int PLCRepository::update(std::shared_ptr<PLC> plc)
{
    if(data==nullptr){
        data = std::make_shared<PLC>();
    }
    data->Id = plc->Id;
    data->Name = plc->Name;
    data->Ip = plc->Ip;
    data->Port = plc->Port;

    saveToFile();
}

std::shared_ptr<PLC> PLCRepository::query()
{
    Config c;

    vws::properties::JsonSerializer::fromJson(c,filename);
    data = plcProperty->load(c);

    return data;
}
