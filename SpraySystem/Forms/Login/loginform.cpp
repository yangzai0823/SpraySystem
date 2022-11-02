#include "loginform.h"
#include "ui_loginform.h"
#include "Forms/Main/mainwindow.h"
#include "qmessagebox.h"
#include <memory>
#include "DataAccess/Models/User/user.h"
#include <QtDebug>
#include <QMetaProperty>
#include "DataAccess/Property/userproperty.h"
#include "Include/Core/Config/jsonserializer.h"
#include <QApplication>
LoginForm::LoginForm(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LoginForm)
{
    ui->setupUi(this);

    m_pDeskdop = QApplication::desktop();
    move((m_pDeskdop->width() - this->width())/2, (m_pDeskdop->height() - this->height())/2);

    logger= CLog::getInstance();
     userRepository = UserRepository::getInstance();

    //auto users = userRepository->list();
//   UserProperty *userproperty =new UserProperty();
   //Config c;
   //userproperty->save(c,users);
   //QString path = qApp->applicationDirPath()+"/Data/User/user.json";
   //vws::properties::JsonSerializer::toJson(c,path.toUtf8().data());

//   Config c2;
//   vws::properties::JsonSerializer::fromJson(c2,path.toUtf8().data());
//   userproperty->load(c2);
}

LoginForm::~LoginForm()
{
    delete ui;
}

void LoginForm::on_btn_Login_clicked()
{
    QString txtUser = ui->edit_User->text();
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

    //验证用户名密码
    auto user  = userRepository->identify(txtUser,txtPwd);
    if(user!=nullptr){
        logger->log(CLog::CLOG_LEVEL::RINFO,QString("%1,登陆").arg(txtUser));

        //auto Main = std::make_shared<MainWindow>();
        MainWindow *Main = new MainWindow(user);
        Main->show();
        this-close();
    }
    else{
        QMessageBox::warning(this,"提示","用户名或密码错误",QMessageBox::Ok);
    }


    //this->hide();
}

void LoginForm::on_btn_Cancel_clicked()
{
    QApplication* app = nullptr;
    app->exit(0);
}

