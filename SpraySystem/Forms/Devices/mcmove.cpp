#include "mcmove.h"
#include "ui_mcmove.h"
#include "Device/devicemanager.h"

MCMove::MCMove(QWidget *parent) : QMainWindow(parent),
                                  ui(new Ui::MCMove)
{
    ui->setupUi(this);

    m_pDeskdop = QApplication::desktop();
    move((m_pDeskdop->width() - this->width()) / 2, (m_pDeskdop->height() - this->height()) / 2);
}

MCMove::~MCMove()
{
    delete ui;
}

void MCMove::on_btn_Move1_clicked()
{
    int axisNum = ui->rb_1->isChecked() ? 0 : 1;
    float step = ui->txt_Step->text().toFloat();
    DeviceManager::getInstance()->getMC(axisNum)->move(step, axisNum);
}

void MCMove::on_btn_Move2_clicked()
{
    int axisNum = ui->rb_1->isChecked() ? 0 : 1;
    float step = ui->txt_Step->text().toFloat();
    DeviceManager::getInstance()->getMC(axisNum)->move(-step, axisNum);
}
