#ifndef PROPERTYMODEL_H
#define PROPERTYMODEL_H

#include "Include/Core/abstractmodel.h"
#include "Include/Core/Config/config.hpp"


class PropertyModel
{
    public:
        PropertyModel();
//        virtual void save(Config) const override;
//        virtual void load(const Config&) override;
        virtual std::string getClassName() const{ return "Property";}
        //virtual std::string defaultName() const {return "Property"；}
};

#endif // PROPERTYMODEL_H
