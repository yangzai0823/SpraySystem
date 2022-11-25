#include "productrepository.h"
#include <Include/Core/Config/jsonserializer.h>



ProductRepository *ProductRepository::productRepository = NULL;

QMutex ProductRepository::mutex;

ProductRepository::ProductRepository()
{
    productProperty = new ProductProperty();
    filename = (savepath+"product.json").toUtf8().data();

    QFile file(savepath+"product.json");
    if(!file.exists()){
        file.open(QIODevice::ReadWrite);
        file.write("");
        file.close();
    }
}

ProductRepository::~ProductRepository()
{
   delete productProperty;
}

ProductRepository *ProductRepository::getInstance()
{
    if(productRepository==NULL)
    {
        mutex.lock();
        if(productRepository==NULL)
        {
            productRepository = new ProductRepository();
        }
        mutex.unlock();
    }
    return productRepository;

}

QList<std::shared_ptr<Product> > ProductRepository::list()
{
    Config c;

    vws::properties::JsonSerializer::fromJson(c,filename);
    auto productlist = productProperty->load(c);

  data = productlist;
  return productlist;
}

int ProductRepository::add(std::shared_ptr<Product> product)
{
    data<<product;

    return saveToFile();
}

int ProductRepository::update(std::shared_ptr<Product> product)
{
    int ret =1;
    foreach(std::shared_ptr<Product> u , data){
        if(u->Id == product->Id){
            u->Name = product->Name;
            u->Length = product->Length;
            u->Width = product->Width;
            u->Height = product->Height;
            ret = saveToFile();
            break;
        }
    }
    return ret;;
}

int ProductRepository::remove(QString id)
{
    int index = -1;
    for(int i =0;i<data.length();i++){
        if(data.at(i)->Id==id){
            index = i;
            break;
        }
    }
    if(index!=-1){
        data.removeAt(index);
        saveToFile();
    }
    return 1;
}

std::shared_ptr<Product> ProductRepository::query(QString id)
{
    std::shared_ptr<Product> product;
    foreach (auto item, data) {
        if(item->Id == id){
            product = item;
            break;
        }
    }
    return product;
}

int ProductRepository::saveToFile()
{
    int ret =1;
    try {
        Config c;
        productProperty->save(c,data);
        ret = vws::properties::JsonSerializer::toJson(c,filename);
    } catch (QString ex) {
        logger->log("产品添加失败， "+ex, CLog::REEROR);
        return -1;
    }
    return ret;
}
