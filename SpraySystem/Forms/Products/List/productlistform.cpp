#include "productlistform.h"
#include "ui_productlistform.h"
#include "Forms/Products/Edit/producteditform.h"
#include <QMessageBox>

ProductListForm::ProductListForm(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ProductListForm)
{
    ui->setupUi(this);

    m_pDeskdop = QApplication::desktop();
    move((m_pDeskdop->width() - this->width())/2, (m_pDeskdop->height() - this->height())/2);

    productRepository = ProductRepository::getInstance();

    setProductTable();
}

ProductListForm::~ProductListForm()
{
    delete ui;
}

void ProductListForm::on_act_EditProduct_triggered()
{
    int row = ui->tableView->currentIndex().row();
    QString id = theModel->item(row,0)->text();

    ProductEditForm *form = new ProductEditForm(this,id );
    form->show();
    this->hide();
}

void ProductListForm::on_act_AddProduct_triggered()
{
    ProductEditForm *form = new ProductEditForm(this);
    form->show();
    this->hide();
}

void ProductListForm::on_act_delete_triggered()
{
    int row = ui->tableView->currentIndex().row();
    QString id = theModel->item(row,0)->text();
    QString name = theModel->item(row,1)->text();
    QString message="删除产品"+name+"?";

    QMessageBox messageBox(QMessageBox::NoIcon, "删除",message,QMessageBox::Yes | QMessageBox::No, NULL);
    messageBox.setWindowFlag(Qt::WindowStaysOnTopHint);

    int reply=messageBox.exec();
    if(reply == QMessageBox::Yes)
    {
        productRepository->remove(id);

        theModel->removeRows(row,1);
    }
}

void ProductListForm::save_slot(std::shared_ptr<Product> p, bool iscreate)
{
    if(iscreate){
        int index = theModel->rowCount();
        theModel->setItem(index,0,new QStandardItem(p->Id));
        theModel->setItem(index,1,new QStandardItem(p->Name));
        theModel->setItem(index,2,new QStandardItem(QString("%1").arg(p->Length)));
        theModel->setItem(index,3,new QStandardItem(QString("%1").arg(p->Width)));
        theModel->setItem(index,4,new QStandardItem(QString("%1").arg(p->Height)));
    }else{
        for(int i=0;i<theModel->rowCount();i++){
            QString id = theModel->item(i,0)->text();
            if(id==p->Id){
                theModel->setItem(i,1,new QStandardItem(p->Name));
                theModel->setItem(i,2,new QStandardItem(QString("%1").arg(p->Length)));
                theModel->setItem(i,3,new QStandardItem(QString("%1").arg(p->Width)));
                theModel->setItem(i,4,new QStandardItem(QString("%1").arg(p->Height)));
                break;
            }
        }
    }
}


void ProductListForm::setProductTable()
{
    QStringList headerText;
    headerText <<"ID"<< "名称"<<"长"<<"宽"<<"高";

    QFont font = ui->tableView->font();
    font.setBold(true);
    font.setPointSize(15);
    ui->tableView->setFont(font);
    ui->tableView->setAlternatingRowColors(true);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    theModel = new QStandardItemModel();

    //设置标题
    theModel->setHorizontalHeaderLabels(headerText);

    QList<std::shared_ptr<Product>> productList = productRepository->list();

    for (int i=0; i<productList.length();i++)
    {
        //const User &u=userList.at(i);
        auto p = productList.at(i);
        //插入数据
        theModel->setItem(i,0,new QStandardItem(p->Id));
        theModel->setItem(i,1,new QStandardItem(p->Name));
        theModel->setItem(i,2,new QStandardItem(QString("%1").arg(p->Length)));
        theModel->setItem(i,3,new QStandardItem(QString("%1").arg(p->Width)));
        theModel->setItem(i,4,new QStandardItem(QString("%1").arg(p->Height)));
    }

    ui->tableView->setModel(theModel);
    ui->tableView->setColumnHidden(0,true);
}


