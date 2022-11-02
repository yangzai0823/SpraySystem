#include "producteditform.h"
#include "ui_producteditform.h"
#include <QMessageBox>
#include <QUuid>

ProductEditForm::ProductEditForm(ProductListForm *listform,QString id) :
    ui(new Ui::ProductEditForm)
{
    ui->setupUi(this);
    this->listform = listform;

    m_pDeskdop = QApplication::desktop();
    move((m_pDeskdop->width() - this->width())/2, (m_pDeskdop->height() - this->height())/2);

    //更新产品信息
    connect(this,SIGNAL(save_signal(std::shared_ptr<Product>,bool)),listform,SLOT(save_slot(std::shared_ptr<Product>,bool)));

    iscreate = true;
    productRepository = ProductRepository::getInstance();

    if(id!=""){

        iscreate = false;
        this->id = id;
        std::shared_ptr<Product> product = productRepository->query(id);

        ui->edit_Name->setText(product->Name);
        ui->edit_Length->setText(QString("%1").arg(product->Length));
        ui->edit_Width->setText(QString("%1").arg(product->Width));
        ui->edit_Height->setText(QString("%1").arg(product->Height));
    }
}

ProductEditForm::~ProductEditForm()
{
    delete ui;
}

void ProductEditForm::closeEvent(QCloseEvent *event)
{
    listform->show();
}


void ProductEditForm::on_btn_OK_clicked()
{
    QString name = ui->edit_Name->text();
    QString length = ui->edit_Length->text();
    QString width = ui->edit_Width->text();
    QString height = ui->edit_Height->text();

    if(name=="" || length=="" || width=="" || height==""){
        QMessageBox::warning(this,"警告","请将信息填写完整",QMessageBox::Ok);
        return;
    }
    std::shared_ptr<Product> product = std::make_shared<Product>();
    product->Name = name;
    product->Length = length.toFloat();
    product->Width = width.toFloat();
    product->Height = height.toFloat();

    if(iscreate){
        product ->Id =QUuid::createUuid().toString().remove("{").remove("}");
        productRepository->add(product);
    }else{
        product->Id = id;
        productRepository->update(product);
    }

    emit save_signal(product,iscreate);

    this->close();
}

void ProductEditForm::on_btn_Cancel_clicked()
{
    this->close();
}
