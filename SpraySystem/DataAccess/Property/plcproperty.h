#ifndef PLCPROPERTY_H
#define PLCPROPERTY_H

#include "DataAccess/Models/PLC/plc.h"
#include "Include/Core/Config/jsonserializer.h"
#include "Include/Core/Config/config.hpp"

#include <memory>
using Config = vws::properties::Config;

class PLCProperty
{
public:
    PLCProperty();
    void save(Config c,std::shared_ptr<PLC> plc) const;
    std::shared_ptr<PLC> load(const Config& c);
};

#endif // PLCPROPERTY_H
