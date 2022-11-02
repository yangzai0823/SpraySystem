#ifndef MOTIONCONTROLLERPROPERTY_H
#define MOTIONCONTROLLERPROPERTY_H
#include "DataAccess/Models/MotionController/motioncontroller.h"
#include "Include/Core/Config/jsonserializer.h"
#include "Include/Core/Config/config.hpp"

#include <memory>
using Config = vws::properties::Config;


class MotionControllerProperty
{
public:
    MotionControllerProperty();
    void save(Config c,std::shared_ptr<MotionController> mc) const;
    std::shared_ptr<MotionController> load(const Config& c);
};

#endif // MOTIONCONTROLLERPROPERTY_H
