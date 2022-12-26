#ifndef MCMOVE_H
#define MCMOVE_H

#include <QMainWindow>
#include <QDesktopWidget>

namespace Ui
{
    class MCMove;
}

class MCMove : public QMainWindow
{
    Q_OBJECT

public:
    explicit MCMove(QWidget *parent = nullptr);
    ~MCMove();

private slots:
    void on_btn_Move1_clicked();

    void on_btn_Move2_clicked();

private:
    Ui::MCMove *ui;
    QDesktopWidget *m_pDeskdop;
};

#endif // MCMOVE_H
