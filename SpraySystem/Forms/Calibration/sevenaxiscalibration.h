#ifndef SEVENAXISCALIBRATION_H
#define SEVENAXISCALIBRATION_H

#include <QMainWindow>

namespace Ui {
class SevenAxisCalibration;
}

class SevenAxisCalibration : public QMainWindow
{
    Q_OBJECT

public:
    explicit SevenAxisCalibration(QWidget *parent = nullptr);
    ~SevenAxisCalibration();

private:
    Ui::SevenAxisCalibration *ui;
};

#endif // SEVENAXISCALIBRATION_H
