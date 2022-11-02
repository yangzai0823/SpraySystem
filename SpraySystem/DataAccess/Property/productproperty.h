#ifndef PRODUCTPROPERTY_H
#define PRODUCTPROPERTY_H
#include "DataAccess/Models/Product/product.h"
#include "Include/Core/Config/jsonserializer.h"
#include "Include/Core/Config/config.hpp"

#include <memory>
using Config = vws::properties::Config;

class ProductProperty
{
public:
    ProductProperty();
    void save(Config c,QList<std::shared_ptr<Product>> products) const;
    QList<std::shared_ptr<Product>> load(const Config& c);
};

#endif // PRODUCTPROPERTY_H
