#ifndef ROBOTPROPERTY_H
#define ROBOTPROPERTY_H

#include "DataAccess/Models/Robot/robot.h"
#include "Include/Core/Config/jsonserializer.h"
#include "Include/Core/Config/config.hpp"

#include <memory>
using Config = vws::properties::Config;


class RobotProperty
{
public:
    RobotProperty();
    void save(Config c,QList<std::shared_ptr<Robot>> robots) const;
    QList<std::shared_ptr<Robot>> load(const Config& c);
};

#endif // ROBOTPROPERTY_H
