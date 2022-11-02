#ifndef SYSTEMEDITFORM_H
#define SYSTEMEDITFORM_H

#include <QMainWindow>
#include <QDesktopWidget>

namespace Ui {
class SystemEditForm;
}

class SystemEditForm : public QMainWindow
{
    Q_OBJECT

public:
    explicit SystemEditForm(QWidget *parent = nullptr);
    ~SystemEditForm();

private slots:
    void on_btn_OK_clicked();

    void on_btn_Cancel_clicked();

private:
    Ui::SystemEditForm *ui;
    QDesktopWidget *m_pDeskdop;
};

#endif // SYSTEMEDITFORM_H
