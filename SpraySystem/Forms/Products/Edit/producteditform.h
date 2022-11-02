#ifndef PRODUCTEDITFORM_H
#define PRODUCTEDITFORM_H

#include <QMainWindow>
#include <QDesktopWidget>
#include "Forms/Products/List/productlistform.h"
#include "DataAccess/Repositories/Product/productrepository.h"
#include "DataAccess/Models/Product/product.h"

namespace Ui {
class ProductEditForm;
}

class ProductEditForm : public QMainWindow
{
    Q_OBJECT

public:
    explicit ProductEditForm(ProductListForm *listform,QString id="");
    ~ProductEditForm();
     void closeEvent(QCloseEvent* event);
private slots:
     void on_btn_OK_clicked();

     void on_btn_Cancel_clicked();

signals:
    void save_signal(std::shared_ptr<Product> u,bool iscreate);

private:
    Ui::ProductEditForm *ui;
    QDesktopWidget *m_pDeskdop;
    ProductListForm *listform;
    bool iscreate;
    QString id;
    ProductRepository *productRepository;
};

#endif // PRODUCTEDITFORM_H
