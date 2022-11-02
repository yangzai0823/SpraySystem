#ifndef ABSTRACTDEVICEMANAGERBASE_H
#define ABSTRACTDEVICEMANAGERBASE_H

#include <string>
#include "Include/Core/visibility_control.hpp"
namespace vws {
namespace core {

class CORE_MODULE AbstractManagerBase {
public:
    AbstractManagerBase()=default;
    virtual ~AbstractManagerBase(){}
    virtual bool ChangeModelName(std::string oldName, std::string newName) = 0;

};
}
}

#endif
