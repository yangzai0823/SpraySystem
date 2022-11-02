#include "usereditform.h"
#include "ui_usereditform.h"
#include <QMessageBox>
#include "DataAccess/Models/User/user.h"
#include <memory>
#include <QUuid>

UserEditForm::UserEditForm( UserListForm* listform, QString id):
    ui(new Ui::UserEditForm)
{
    ui->setupUi(this);
    this->listform = listform;

    m_pDeskdop = QApplication::desktop();
    move((m_pDeskdop->width() - this->width())/2, (m_pDeskdop->height() - this->height())/2);

    bg= new QButtonGroup();
    bg->addButton(ui->rb_Tec);
    bg->addButton(ui->rb_Operator);
    bg->addButton(ui->rb_Admin);

    //权限选项
    QList<QCheckBox*> items =  ui->centralwidget->findChildren<QCheckBox*>();
    foreach(auto item,items){
       connect(item,SIGNAL(stateChanged(int)),this,SLOT(permissionChange_slot(int)));
    }
    //更新用户信息
    connect(this,SIGNAL(save_signal(std::shared_ptr<User>,bool)),listform,SLOT(save_slot(std::shared_ptr<User>,bool)));

    iscreate = true;
    userrepository = UserRepository::getInstance();

    if(id!=""){

        iscreate = false;
        this->id = id;
        std::shared_ptr<User> user = userrepository->query(id);
        QStringList permissions = user->Permissions;
        QString role = user->Role;

        ui->edit_Name->setText(user->Name);
        ui->edit_Pwd->setText(user->Password);
        if(role=="Tec"){
           ui->rb_Tec->setChecked(true);
        }else if(role=="Operator"){
            ui->rb_Operator->setChecked(true);
        }else{
            ui->rb_Admin->setChecked(true);
        }

       QList<QCheckBox*> items =  ui->centralwidget->findChildren<QCheckBox*>();
       for(int i = 0;i<user->Permissions.length();i++){
           QString permission = user->Permissions[i];
            foreach (auto item, items) {
               if(item->text()==permission){
                   item->setChecked(true);
                   break;
               }
           }
       }
    }
}

void UserEditForm::closeEvent(QCloseEvent *event)
{
    listform->show();
}

UserEditForm::~UserEditForm()
{
    delete ui;
}

void UserEditForm::on_rb_Tec_clicked(bool checked)
{
    if(checked){
        ui->cb_User->setChecked(true);
        ui->cb_Product->setChecked(true);
        ui->cb_Device->setChecked(true);
        ui->cb_Cali->setChecked(false);
        ui->cb_Sys->setChecked(false);
    }
}

void UserEditForm::on_rb_Admin_clicked(bool checked)
{
    if(checked){
        ui->cb_User->setChecked(true);
        ui->cb_Product->setChecked(true);
        ui->cb_Device->setChecked(true);
        ui->cb_Cali->setChecked(true);
        ui->cb_Sys->setChecked(true);
    }
}

void UserEditForm::on_rb_Operator_clicked(bool checked)
{
    if(checked){
        ui->cb_User->setChecked(false);
        ui->cb_Product->setChecked(false);
        ui->cb_Device->setChecked(false);
        ui->cb_Cali->setChecked(false);
        ui->cb_Sys->setChecked(false);
    }

}

void UserEditForm::on_btn_OK_clicked()
{
    QString txtUser = ui->edit_Name->text();
    QString txtPwd = ui->edit_Pwd->text();
    if(txtUser.isEmpty())
    {
        QMessageBox::warning(this,"警告","请输入用户名",QMessageBox::Ok);
        return;
    }
    if(txtPwd.isEmpty())
    {
        QMessageBox::warning(this,"警告","请输入密码",QMessageBox::Ok);
        return;
    }
    //保存数据/更新List
    std::shared_ptr<User> user = std::make_shared<User>();
    user->Name = txtUser;
    user->Password = txtPwd;

    QRadioButton* pbtn = qobject_cast<QRadioButton*>(bg->checkedButton());
     QString role = pbtn->text();

     user->Role = role;
     delete  pbtn;
     user->Permissions = this->permissionList;

    if(iscreate){
        user->Id = QUuid::createUuid().toString().remove("{").remove("}");
        userrepository->add(user);
    }else{
        user->Id = this->id;
        userrepository->update(user);
    }

    emit save_signal(user,iscreate);
    this->close();
}

void UserEditForm::permissionChange_slot(int state)
{
    QCheckBox *cb =dynamic_cast<QCheckBox *>(QObject::sender());

    QString per = cb->text();
    if(state==2){
        this->permissionList.append(per);
    }
    else{
        this->permissionList.removeOne(per);
    }
}

