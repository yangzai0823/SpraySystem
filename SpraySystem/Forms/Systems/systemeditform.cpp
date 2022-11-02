#include "systemeditform.h"
#include "ui_systemeditform.h"

SystemEditForm::SystemEditForm(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SystemEditForm)
{
    ui->setupUi(this);

    m_pDeskdop = QApplication::desktop();
    move((m_pDeskdop->width() - this->width())/2, (m_pDeskdop->height() - this->height())/2);
}

SystemEditForm::~SystemEditForm()
{
    delete ui;
}

void SystemEditForm::on_btn_OK_clicked()
{

}

void SystemEditForm::on_btn_Cancel_clicked()
{
    this->close();
}
