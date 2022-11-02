#ifndef USEREDITFORM_H
#define USEREDITFORM_H

#include <QMainWindow>
#include <QDesktopWidget>
#include <QButtonGroup>

#include "DataAccess/Repositories/User/userrepository.h"
#include "Forms/Users/List/userlistform.h"

namespace Ui {
class UserEditForm;
}

class UserEditForm : public QMainWindow
{
    Q_OBJECT

public:

    UserEditForm(UserListForm *listform, QString id = "");
    void closeEvent(QCloseEvent* event);
    ~UserEditForm();

private slots:
    void on_rb_Tec_clicked(bool checked);

    void on_rb_Admin_clicked(bool checked);

    void on_rb_Operator_clicked(bool checked);

    void on_btn_OK_clicked();

    void permissionChange_slot(int state);

signals:
    void save_signal(std::shared_ptr<User> u,bool iscreate);

private:
    Ui::UserEditForm *ui;
    QDesktopWidget *m_pDeskdop;
    bool iscreate = true;
    QString id;
    UserRepository *userrepository;
    QButtonGroup *bg;
    QStringList permissionList;
    UserListForm *listform;

    void setProductTable();
};

#endif // USEREDITFORM_H
