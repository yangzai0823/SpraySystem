#ifndef PROPERTYFRAMEINTERFACE_H
#define PROPERTYFRAMEINTERFACE_H

#include "Include/Core/abstractmodel.h"
#include "Include/Core/Config/jsonserializer.h"
#include "Include/Core/Config/config.hpp"

using Config = vws::core::Config;

class PropertyFrameInterface
{
public:
    PropertyFrameInterface();

    virtual void save(Config c) const{};
    virtual void load(const Config& c){};

};

#endif // PROPERTYFRAMEINTERFACE_H
