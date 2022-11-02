#ifndef CAMERAREPOSITORY_H
#define CAMERAREPOSITORY_H

#include "DataAccess/Models/Camera/camera.h"
#include "DataAccess/Property/cameraproperty.h"
#include "DataAccess/Repositories/baserepository.h"
#include <memory>
#include <QMutex>

class CameraRepository : public BaseRepository
{
public:
    ~CameraRepository();
    static CameraRepository *getInstance();
    QList<std::shared_ptr<Camera>> list();
    int add(std::shared_ptr<Camera> camera);
    int update(std::shared_ptr<Camera> camera);
    int remove(QString id);
    int save(QList<std::shared_ptr<Camera>> lstCamera);
    std::shared_ptr<Camera> query(QString name);

private:
    CameraRepository();
    int saveToFile();
    static CameraRepository *cameraRepository;
    static QMutex mutex;
    CameraProperty *cameraProperty;
    QList<std::shared_ptr<Camera>> data;
};

#endif // CAMERAREPOSITORY_H
