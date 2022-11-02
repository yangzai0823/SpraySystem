#ifndef PRODUCT_H
#define PRODUCT_H

#include <QObject>

class Product
{
public:
    Product();
    QString Id;
    QString Name;
    float Length;
    float Width;
    float Height;
};

#endif // PRODUCT_H
