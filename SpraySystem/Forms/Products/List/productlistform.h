#ifndef PRODUCTLISTFORM_H
#define PRODUCTLISTFORM_H

#include <QMainWindow>
#include <QDesktopWidget>
#include <QStandardItemModel>
#include <memory>
#include <memory>
#include "DataAccess/Repositories/Product/productrepository.h"

namespace Ui {
class ProductListForm;
}

class ProductListForm : public QMainWindow
{
    Q_OBJECT

public:
    explicit ProductListForm(QWidget *parent = nullptr);
    ~ProductListForm();

private slots:
    void on_act_EditProduct_triggered();

    void on_act_AddProduct_triggered();

    void on_act_delete_triggered();

public slots:
    void save_slot(std::shared_ptr<Product> user,bool iscreate);

private:
     Ui::ProductListForm *ui;
     QDesktopWidget *m_pDeskdop;
     QStandardItemModel *theModel;
     ProductRepository *productRepository;
     void setProductTable();
};

#endif // PRODUCTLISTFORM_H
