#ifndef USERLISTFORM_H
#define USERLISTFORM_H

#include <QMainWindow>
#include <QDesktopWidget>
#include <QStandardItemModel>
#include <memory>

#include "DataAccess/Repositories/User/userrepository.h"

namespace Ui {
class UserListForm;
}

class UserListForm : public QMainWindow
{
    Q_OBJECT

public:
    explicit UserListForm(QWidget *parent = nullptr);

    ~UserListForm();

private slots:
    void on_act_AddUser_triggered();

    void on_act_edit_triggered();
    void on_act_delete_triggered();

public slots:
    void save_slot(std::shared_ptr<User> user,bool iscreate);

private:
    Ui::UserListForm *ui;
    QDesktopWidget *m_pDeskdop;
    QStandardItemModel *theModel;
    UserRepository *userRepository;
    void SetUserTable();

};

#endif // USERLISTFORM_H
