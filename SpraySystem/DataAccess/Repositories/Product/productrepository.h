#ifndef PRODUCTREPOSITORY_H
#define PRODUCTREPOSITORY_H

#include "DataAccess/Models/Product/product.h"
#include "DataAccess/Property/productproperty.h"
#include "DataAccess/Repositories/baserepository.h"
#include <memory>
#include <QMutex>

class ProductRepository : public BaseRepository
{
public:

    ~ProductRepository();
    static ProductRepository *getInstance();
    QList<std::shared_ptr<Product>> list();
    int add(std::shared_ptr<Product> product);
    int update(std::shared_ptr<Product> product);
    int remove(QString id);
    std::shared_ptr<Product> query(QString id);

private:
    ProductRepository();
    int saveToFile();
    static ProductRepository *productRepository;
    static QMutex mutex;
    ProductProperty *productProperty;
    QList<std::shared_ptr<Product>> data;
};

#endif // PRODUCTREPOSITORY_H
