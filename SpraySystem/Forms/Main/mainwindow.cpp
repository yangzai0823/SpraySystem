#include "mainwindow.h"

#include "Forms/Login/loginform.h"
#include "Forms/Users/List/userlistform.h"
#include "Forms/Products/List/productlistform.h"
#include "Forms/Devices/devicelist.h"
#include "Forms/Systems/systemeditform.h"
#include <QTime>
#include <QTimer>

#include "Util/Socket/socketclient.h"
#include "Forms/global.h"
#include <QAbstractButton>

MainWindow::MainWindow(std::shared_ptr<User> user)
    : ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->showMaximized();

    loginUser = user;
    this->judgeAuthority();

    QMenu *userMenu = new QMenu;
    userMenu->addAction(ui->actQuit);
    userMenu->addAction(ui->actChangeUser);
    userMenu->setStyleSheet("QMenu{background-color:rgb(48,47,47); color:rgb(255,255,255); font:25px}");
    ui->btn_LoginUser->setText(loginUser->Name);
    ui->btn_LoginUser->setMenu(userMenu);



    isRun = false;

    update();
    timer=new QTimer(this);
    timer->start(1000);
    connect(timer,SIGNAL(timeout()),this,SLOT(update()));

    startDevices();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
//    devicemonitor->stopWork();
//    devicemonitorThread->quit();
    //    devicemonitorThread->wait();
    //    devicemonitorThread = nullptr;

    //断开所有设备连接

}

void MainWindow::update()
{
    QTime timenow = QTime::currentTime();
    ui->lbl_Time->setText(tr("%1").arg(timenow.toString()));
}

void MainWindow::startDevices()
{
//    devicemonitor = new DeviceMonitor();
//    devicemonitorThread = new QThread();

//    devicemonitor->moveToThread(devicemonitorThread);
//    if(!devicemonitorThread->isRunning()){
//        devicemonitorThread->start();
//    }
//    connect(this,SIGNAL(startMonitorDevice_signal()),devicemonitor,SLOT(startMonitorDevice_slot()));
//    connect(devicemonitor,SIGNAL(deviceConnectError_signal(QString,int)),this, SLOT(deviceConnectError_slot(QString,int)));
//    emit this->startMonitorDevice_signal();
}

void MainWindow::updateDeviceState(QPushButton* sender, int state)
{
    QIcon ico;
    if(state==1)
    {
        ico.addFile("://Images/running.png");
    }
    else
    {
        ico.addFile("://Images/warning.png");
    }
    sender->setIcon(ico);
}

void MainWindow::judgeAuthority()
{

    QStringList permissions = loginUser->Permissions;
    if(permissions.length()==0){    //无任任何权限隐藏左侧导航兰
        ui->frame_Nav->setVisible(false);
    }
    else{
        QList<QToolButton*> navBtns = ui->frame_Nav->findChildren<QToolButton*>();  //导航栏按钮
        for(int i=0;i<navBtns.length();i++){
            QToolButton *var = navBtns[i];
            if(permissions.indexOf(var->text())>-1){
                var->setVisible(true);
            }
            else{
                var -> setVisible(false);
            }
        }
    }
}



void MainWindow::on_actQuit_triggered()
{
    QApplication* applicaiton;
    applicaiton->exit(0);
}

void MainWindow::on_actChangeUser_triggered()
{
    LoginForm *loginform = new LoginForm() ;

    this->close();
    loginform->show();
}

void MainWindow::on_btn_Start_clicked()
{
    QIcon ico;
    if(isRun)
    {
      mainprocess->triggerTest();
      return;
      // delete mainprocess;
      ico.addFile("://Images/start.png");
    }
    else
    {
        if(!mainprocess){
            mainprocess = new MainProcess();
        }

        ico.addFile("://Images/stop.png");
    }
    isRun = !isRun;
    ui->btn_Start->setIcon(ico);
}

void MainWindow::on_btn_User_clicked()
{
    UserListForm *form = new UserListForm();
    form->show();
    //this->hide();
}

void MainWindow::on_btn_Product_clicked()
{
    ProductListForm *form = new ProductListForm();
    form->show();
    //this->hide();
}

void MainWindow::on_btn_Device_clicked()
{
    DeviceList *form = new DeviceList();
    form->show();
}

void MainWindow::on_btn_System_clicked()
{
    SystemEditForm *form =new SystemEditForm();
    form->show();
}

void MainWindow::deviceConnectError_slot(QString device, int state)
{
    QStringList deviceList ;
    //deviceList << "robot1"<<"robot2"<<"camera1"<<"camera2"<<"plc";

    auto rbtlist = DeviceManager::getInstance()->robotList();
    foreach (auto item, rbtlist) {
         deviceList.append(item->getName());
    }
    auto camlist = DeviceManager::getInstance()->cameraList();
    foreach(auto item,camlist){
        deviceList.append(item->getName());
    }
    deviceList<<"plc";

    switch (deviceList.indexOf(device)) {
    case 0:
        updateDeviceState(ui->btn_Robot1,state);
        break;
    case 1:
        updateDeviceState(ui->btn_Robot2,state);
        break;
    case 2:
        updateDeviceState(ui->btn_Camera1,state);
        break;
    case 3:
        updateDeviceState(ui->btn_Camera2,state);
        break;
    case 4:
        updateDeviceState(ui->btn_PLC,state);
        break;
    default:
        break;
    }
}



void MainWindow::on_btn_EStop_clicked()
{
    // mainprocess->sendtorbt();
}
