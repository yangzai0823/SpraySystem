#ifndef LOGINFORM_H
#define LOGINFORM_H

#include <QMainWindow>
#include <QDesktopWidget>
#include "Util/Log/clog.h"
#include "DataAccess/Repositories/User/userrepository.h"

namespace Ui {
class LoginForm;
}

class LoginForm : public QMainWindow
{
    Q_OBJECT

public:
    explicit LoginForm(QWidget *parent = nullptr);
    ~LoginForm();

private slots:
    void on_btn_Login_clicked();

    void on_btn_Cancel_clicked();

private:
    Ui::LoginForm *ui;
    QDesktopWidget *m_pDeskdop;
    CLog *logger;
    UserRepository *userRepository;
};

#endif // LOGINFORM_H
