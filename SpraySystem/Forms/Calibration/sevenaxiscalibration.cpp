#include "sevenaxiscalibration.h"
#include "ui_sevenaxiscalibration.h"

SevenAxisCalibration::SevenAxisCalibration(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SevenAxisCalibration)
{
    ui->setupUi(this);
}

SevenAxisCalibration::~SevenAxisCalibration()
{
    delete ui;
}
