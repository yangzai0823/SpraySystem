#include "userlistform.h"
#include "ui_userlistform.h"

#include <QAbstractTableModel>
#include <iostream>
#include <memory>
#include <QMessageBox>
#include "Forms/Users/Edit/usereditform.h"
#include "DataAccess/Models/User/user.h"
UserListForm::UserListForm(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::UserListForm)
{
    ui->setupUi(this);

    m_pDeskdop = QApplication::desktop();
    move((m_pDeskdop->width() - this->width())/2, (m_pDeskdop->height() - this->height())/2);

    userRepository = UserRepository::getInstance();

    SetUserTable();
}

UserListForm::~UserListForm()
{
    delete ui;
}

void UserListForm::SetUserTable()
{
    QStringList headerText;
    headerText <<"ID"<< "姓名"<<"角色"<<"权限";

    QFont font = ui->tableView->font();
    font.setBold(true);
    font.setPointSize(15);
    ui->tableView->setFont(font);
    ui->tableView->setAlternatingRowColors(true);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    theModel = new QStandardItemModel();

    //设置标题
    theModel->setHorizontalHeaderLabels(headerText);

    QList<std::shared_ptr<User>> userList = userRepository->list();

    for (int i=0; i<userList.length();i++)
    {
        //const User &u=userList.at(i);
        auto u = userList.at(i);
        //插入数据
        theModel->setItem(i,0,new QStandardItem(u->Id));
        theModel->setItem(i,1,new QStandardItem(u->Name));
        theModel->setItem(i,2,new QStandardItem(u->Role));
        theModel->setItem(i,3,new QStandardItem(u->Permissions.join(",")));
    }

    ui->tableView->setModel(theModel);
    ui->tableView->setColumnHidden(0,true);
}

void UserListForm::on_act_AddUser_triggered()
{
   UserEditForm *form = new UserEditForm(this);
   form->show();
}

void UserListForm::on_act_edit_triggered()
{
    int row = ui->tableView->currentIndex().row();
    QString id = theModel->item(row,0)->text();

    UserEditForm *form = new UserEditForm(this,id);
    form->show();
    this->hide();
}


void UserListForm::on_act_delete_triggered()
{
    int row = ui->tableView->currentIndex().row();
    QString id = theModel->item(row,0)->text();
    QString name = theModel->item(row,1)->text();
    QString message="删除用户"+name+"?";

    QMessageBox messageBox(QMessageBox::NoIcon, "删除",message,QMessageBox::Yes | QMessageBox::No, NULL);
    messageBox.setWindowFlag(Qt::WindowStaysOnTopHint);

    int reply=messageBox.exec();
    if(reply == QMessageBox::Yes)
    {
        userRepository->remove(id);

        theModel->removeRows(row,1);
    }
}

void UserListForm::save_slot(std::shared_ptr<User> u,bool iscreate)
{
    if(iscreate){
        int index = theModel->rowCount();
        theModel->setItem(index,0,new QStandardItem(u->Id));
        theModel->setItem(index,1,new QStandardItem(u->Name));
        theModel->setItem(index,2,new QStandardItem(u->Role));
        theModel->setItem(index,3,new QStandardItem(u->Permissions.join(",")));
    }else{
        for(int i=0;i<theModel->rowCount();i++){
            QString id = theModel->item(i,0)->text();
            if(id==u->Id){
                theModel->setItem(i,1,new QStandardItem(u->Name));
                theModel->setItem(i,2,new QStandardItem(u->Role));
                theModel->setItem(i,3,new QStandardItem(u->Permissions.join(",")));
                break;
            }
        }
    }
}

