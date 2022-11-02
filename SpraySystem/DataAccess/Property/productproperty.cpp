#include "productproperty.h"

ProductProperty::ProductProperty()
{

}

void ProductProperty::save(Config c, QList<std::shared_ptr<Product> > products) const
{
    auto t = c.mapMakeChild("Products");
    foreach(auto u , products)
    {
        Config config_child = t.listAppendNew();
        config_child.mapSetValue("Id",u->Id);
        config_child.mapSetValue("Name",u->Name);
        config_child.mapSetValue("Length",u->Length);
        config_child.mapSetValue("Width",u->Width);
        config_child.mapSetValue("Height",u->Height);
    }
}

QList<std::shared_ptr<Product> > ProductProperty::load(const Config &c)
{
    QList<std::shared_ptr<Product>> products;

    //c.mapGetString("Users",&users);
    auto vList= c.mapGetChild("Products");
    for(int i =0;i<vList.listLength();i++)
    {
        auto product = std::make_shared<Product>();

        QString id;
        QString name;
        float length,width,height;
        vList.listChildAt(i).mapGetString("Id",&id);
        vList.listChildAt(i).mapGetString("Name",&name);
        vList.listChildAt(i).mapGetFloat("Length",&length);
        vList.listChildAt(i).mapGetFloat("Width",&width);
        vList.listChildAt(i).mapGetFloat("Height",&height);

        product->Id = id;
        product->Name = name;
        product->Width = width;
        product->Length = length;
        product->Height= height;

        products.append(product);
    }

    return products;
}
