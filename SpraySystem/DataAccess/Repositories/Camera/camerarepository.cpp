#include "camerarepository.h"


CameraRepository *CameraRepository::cameraRepository = NULL;

QMutex CameraRepository::mutex;

CameraRepository::CameraRepository()
{
    cameraProperty = new CameraProperty();
    filename = (savepath+"camera.json").toUtf8().data();

    QFile file(savepath+"camera.json");
    if(!file.exists()){
        file.open(QIODevice::ReadWrite);
        file.write("");
        file.close();
    }
}

CameraRepository::~CameraRepository()
{
   delete cameraProperty;
}

CameraRepository *CameraRepository::getInstance()
{
    if(cameraRepository==NULL)
    {
        mutex.lock();
        if(cameraRepository==NULL)
        {
            cameraRepository = new CameraRepository();
        }
        mutex.unlock();
    }
    return cameraRepository;

}

QList<std::shared_ptr<Camera> > CameraRepository::list()
{
    Config c;

    vws::properties::JsonSerializer::fromJson(c,filename);
    auto cameralist = cameraProperty->load(c);

  data = cameralist;
  return cameralist;
}

int CameraRepository::add(std::shared_ptr<Camera> camera)
{
    data<<camera;

    return saveToFile();
}

int CameraRepository::update(std::shared_ptr<Camera> camera)
{
    int ret =1;
    foreach(std::shared_ptr<Camera> u , data){
        if(u->Id == camera->Id){
            u->Name = camera->Name;
            u->Ip = camera->Ip;
            u->Port = camera->Port;
            u->Frame = 0; //camera->Frame;
            u->Exposure = 0; // camera->Exposure;
            ret = saveToFile();
            break;
        }
    }
    return ret;;
}

int CameraRepository::remove(QString id)
{
    int index = -1;
    for(int i =0;i<data.length();i++){
        if(data.at(i)->Id==id){
            index = i;
            break;
        }
    }
    if(index!=-1){
        data.removeAt(index);
        saveToFile();
    }
    return 1;
}

int CameraRepository::save(QList<std::shared_ptr<Camera> > lstCamera)
{
    data = lstCamera;
    saveToFile();
}

std::shared_ptr<Camera> CameraRepository::query(QString name)
{
    std::shared_ptr<Camera> camera;
    foreach (auto item, data) {
        if(item->Name == name){
            camera = item;
            break;
        }
    }
    return camera;
}

int CameraRepository::saveToFile()
{
    int ret =1;
    try {
        Config c;
        cameraProperty->save(c,data);
        ret = vws::properties::JsonSerializer::toJson(c,filename);
    } catch (QString ex) {
        logger->log("相机添加失败， "+ex,CLog::REEROR);
        return -1;
    }
    return ret;
}
