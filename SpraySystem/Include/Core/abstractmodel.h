#ifndef ABSTRACT_MODEL_H
#define ABSTRACT_MODEL_H

#include <QUuid>
#include <Include/Core/visibility_control.hpp>
#include <Include/Core/Config/config.hpp>
#include <Include/Core/Properties/entity_property.hpp>
#include "Include/Core/abstractmanagerbase.h"
namespace vws{
namespace core{
    using Config = vws::properties::Config;
    using EntityProperty = vws::properties::EntityProperty;
    class CORE_MODULE AbstractModel
    {

    public:
        AbstractModel();
        virtual ~AbstractModel() {}
        virtual std::string getClassName() const = 0;
        virtual void save(Config) const = 0;
        virtual void load(const Config&) = 0;
        QUuid id() const;

        virtual std::string getModelName() const;
        virtual std::string defaultName() const = 0;
        virtual void setModelNameSpace(std::string ns);
        void setManager(AbstractManagerBase * m){manager_ = m;}
        virtual bool trySetModelName(std::string name);
        virtual void setModelName(std::string name);
    protected:

        QUuid id_;
        EntityProperty model_;
        AbstractManagerBase * manager_;
    };

}
}

#endif
