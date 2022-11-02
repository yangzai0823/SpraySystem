#ifndef CAMERAPROPERTY_H
#define CAMERAPROPERTY_H

#include "DataAccess/Models/Camera/camera.h"
#include "Include/Core/Config/jsonserializer.h"
#include "Include/Core/Config/config.hpp"

#include <memory>
using Config = vws::properties::Config;

class CameraProperty
{
public:
    CameraProperty();
    void save(Config c,QList<std::shared_ptr<Camera>> robots) const;
    QList<std::shared_ptr<Camera>> load(const Config& c);
};

#endif // CAMERAPROPERTY_H
