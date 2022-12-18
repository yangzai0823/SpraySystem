#include "mainwindow.h"

#include "Forms/Login/loginform.h"
#include "Forms/Users/List/userlistform.h"
#include "Forms/Products/List/productlistform.h"
#include "Forms/Devices/devicelist.h"
#include "Forms/Systems/systemeditform.h"
#include <QTime>
#include <QTimer>

#include "Forms/global.h"
#include <QAbstractButton>
#include "Data/StaticData.h"

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

    vws::DataInit::Init();

    updateRealTime_Slot();
    timer = new QTimer(this);
    timer->start(1000);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateRealTime_Slot()));

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

    // 断开所有设备连接
}

void imgfunc(const VWSCamera::ImageData &data, void *pUser)
{
}

void MainWindow::on_actQuit_triggered()
{
    QApplication *applicaiton;
    applicaiton->exit(0);
}

void MainWindow::on_actChangeUser_triggered()
{
    LoginForm *loginform = new LoginForm();

    this->close();
    loginform->show();
}

void MainWindow::on_btn_Start_clicked()
{
    QIcon ico;
    if (isRun)
    {
        delete mainprocess;
        ico.addFile("://Images/start.png");
    }
    else
    {
        if (!mainprocess)
        {
            showMsg("开始运行");
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
    // this->hide();
}

void MainWindow::on_btn_Product_clicked()
{
    ProductListForm *form = new ProductListForm();
    form->show();
    // this->hide();
}

void MainWindow::on_btn_Device_clicked()
{
    DeviceList *form = new DeviceList();
    form->show();
}

void MainWindow::on_btn_System_clicked()
{
    SystemEditForm *form = new SystemEditForm();
    form->show();
}

void MainWindow::deviceConnectError_slot(QString device, int state)
{
    QStringList deviceList;
    // deviceList << "robot1"<<"robot2"<<"camera1"<<"camera2"<<"plc";

    auto rbtlist = DeviceManager::getInstance()->robotList();
    foreach (auto item, rbtlist)
    {
        deviceList.append(item->getName());
    }
    auto camlist = DeviceManager::getInstance()->cameraList();
    foreach (auto item, camlist)
    {
        deviceList.append(item->getName());
    }
    deviceList << vws::PLC;

    switch (deviceList.indexOf(device))
    {
    case 0:
        updateDeviceState(ui->btn_Robot1, state);
        break;
    case 1:
        updateDeviceState(ui->btn_Robot2, state);
        break;
    case 2:
        updateDeviceState(ui->btn_Camera1, state);
        break;
    case 3:
        updateDeviceState(ui->btn_Camera2, state);
        break;
    case 4:
        updateDeviceState(ui->btn_PLC, state);
        break;
    default:
        break;
    }
}

int i = 0;
void MainWindow::on_btn_EStop_clicked()
{

    mainprocess->triggerTest();
    return;
    // mainprocess->Test(mainprocess->plcdata);

    // if (i == 0)
    // {
    //     mainprocess->plcdata.laser_up_head = mainprocess->plcdata.laser_up_behind = mainprocess->plcdata.laser_bottom_head = mainprocess->plcdata.laser_bottom_behind = 100;
    //     mainprocess->plcdata.flag_camera_b = 1;
    //     mainprocess->Test(mainprocess->plcdata);
    // }
    // else if (i == 1)
    // {
    //     mainprocess->plcdata.flag_laser_b = 1;
    //     mainprocess->Test(mainprocess->plcdata);
    // }
    // else if (i == 2)
    // {
    //     mainprocess->TestImg();
    // }
    // else if (i == 3)
    // {
    //     mainprocess->plcdata.flag_camera_b = 0;
    //     mainprocess->Test(mainprocess->plcdata);
    // }
    // else if (i == 4)
    // {
    //     mainprocess->TestImg();
    // }

    // // std::cout << "i: " << i << std::endl;
    // if (i < 4)
    // {
    //     i++;
    // }
    // else
    // {
    //     i = 0;
    // }
    // //   mainprocess->sendtorbt();
}

void MainWindow::on_btn_Camera1_clicked()
{
    DeviceManager::getInstance()->getCamera(vws::Camera_top)->init();
    DeviceManager::getInstance()->getCamera(vws::Camera_top)->start();
}

void MainWindow::on_btn_Camera2_clicked()
{
    DeviceManager::getInstance()->getCamera(vws::Camera_bottom)->init();
    DeviceManager::getInstance()->getCamera(vws::Camera_bottom)->start();
}

void MainWindow::on_btn_Robot1_clicked()
{
    DeviceManager::getInstance()->getRobot(vws::Robot1)->init();
    DeviceManager::getInstance()->getRobot(vws::Robot1)->start();
}

void MainWindow::on_btn_Robot2_clicked()
{
    DeviceManager::getInstance()->getRobot(vws::Robot2)->init();
    DeviceManager::getInstance()->getRobot(vws::Robot2)->start();
}

void MainWindow::on_btn_PLC_clicked()
{
    DeviceManager::getInstance()->getPlc()->init();
    DeviceManager::getInstance()->getPlc()->start();
}

void MainWindow::startDevices()
{
    QString msg;
    DeviceManager *deviceManager = DeviceManager::getInstance();
    // PLC
    auto plc = deviceManager->getPlc();
    auto retplc = plc->start();
    // msg = retplc>0?"PLC启动成功":"PLC启动失败";
    // showMsg(msg);

    // // 运动控制器
    // auto mc = deviceManager->getMC();
    // auto retmc = mc->init();
    // retmc = mc->start();

    // // msg = retmc>0?"运动控制器启动成功":"运动控制器启动失败";
    // // showMsg(msg);

    // // 机器人
    // auto rbt = deviceManager->getRobot(0);
    // auto retrbt = rbt->init();
    // if (retrbt > 0)
    // {
    //     std::cout << "robot1 init: " << retrbt << std::endl;
    //     retrbt = rbt->start();
    //     std::cout << "robot1 start: " << retrbt << std::endl;
    // }
    // else
    // {
    //     msg = rbt->getName() + ", 启动失败";
    //     showMsg(msg);
    // }

    // // 相机
    // auto camera1 = deviceManager->getCamera(0);

    // auto ret = camera1->init();
    // if (ret > 0)
    // {
    //     std::cout << "camera1 init: " << ret << std::endl;
    //     ret = camera1->start();
    //     std::cout << "camera1 start: " << ret << std::endl;
    // }
    // else
    // {
    //     msg = camera1->getName() + "， 启动失败";
    //     showMsg(msg);
    // }

    // auto camera2 = deviceManager->getCamera(1);
    // ret = camera2->init();
    // if (ret > 0)
    // {
    //     std::cout << "camera2 init: " << ret << std::endl;
    //     ret = camera2->start();
    //     std::cout << "camera2 start: " << ret << std::endl;
    // }
    // else
    // {
    //     msg = camera2->getName() + "， 启动失败";
    //     showMsg(msg);
    // }

    /***设备状态监控***/
    devicemonitor = new DeviceMonitor();
    devicemonitorThread = new QThread();

    devicemonitor->moveToThread(devicemonitorThread);
    if (!devicemonitorThread->isRunning())
    {
        devicemonitorThread->start();
    }
    connect(this, SIGNAL(startMonitorDevice_signal()), devicemonitor, SLOT(startMonitorDevice_slot()));
    connect(devicemonitor, SIGNAL(deviceConnectError_signal(QString, int)), this, SLOT(deviceConnectError_slot(QString, int)));
    emit this->startMonitorDevice_signal();
    /***设备状态监控***/
}

void MainWindow::updateDeviceState(QPushButton *sender, int state)
{
    QIcon ico;
    if (state >= 0)
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
    if (permissions.length() == 0)
    { // 无任任何权限隐藏左侧导航兰
        ui->frame_Nav->setVisible(false);
    }
    else
    {
        QList<QToolButton *> navBtns = ui->frame_Nav->findChildren<QToolButton *>(); // 导航栏按钮
        for (int i = 0; i < navBtns.length(); i++)
        {
            QToolButton *var = navBtns[i];
            if (permissions.indexOf(var->text()) > -1)
            {
                var->setVisible(true);
            }
            else
            {
                var->setVisible(false);
            }
        }
    }
}

void MainWindow::showMsg(QString msg)
{
    QDateTime current_date_time = QDateTime::currentDateTime();
    QString current_date = current_date_time.toString("yyyy.MM.dd hh:mm:ss");
    msg = "<p>" + current_date + "</p>" + "<p>" + msg + "</p>";
    msg = ui->lbl_Msg->text() + msg;

    ui->lbl_Msg->setText(msg);
    ui->lbl_Msg->setTextFormat(Qt::RichText);
}

void MainWindow::updateRealTime_Slot()
{
    QTime timenow = QTime::currentTime();
    ui->lbl_Time->setText(tr("%1").arg(timenow.toString()));
}

void MainWindow::alarm_Slot()
{
    showMsg("设备异常");
}