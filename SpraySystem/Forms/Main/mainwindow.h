#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <memory>
//#include "Device/Robot/robot.h"
//#include "Device/Camera/camera.h"
//#include "Device/PLC/plc.h"
#include "devicemonitor.h"
#include "./ui_mainwindow.h"
#include "DataAccess/Models/User/user.h"
#include "Bussiness/mainprocess.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(std::shared_ptr<User> user);
    ~MainWindow();
    void closeEvent(QCloseEvent *event);
private slots:
    void on_actQuit_triggered();

    void on_actChangeUser_triggered();

    void on_btn_Start_clicked();

    void on_btn_User_clicked();

    void on_btn_Product_clicked();

    void update();

    void on_btn_Device_clicked();

    void on_btn_System_clicked();

    void deviceConnectError_slot(QString device, int state);
    void on_btn_EStop_clicked();
    void on_btn_Camera1_clicked();
    void on_btn_Camera2_clicked();
    void on_btn_Robot1_clicked();
    void on_btn_Robot2_clicked();
signals:
    void startMonitorDevice_signal();

private:
    Ui::MainWindow *ui;
    bool isRun;
    QTimer *timer;
    MainProcess::CameraCallbackData *camera1CallbackData;

    //    Robot *robot1;
    //    Robot *robot2;
    //    Camera *camera1;
    //    Camera *camera2;
    //    PLC *plc;

       DeviceMonitor *devicemonitor; //监听所有设备状态
       QThread *devicemonitorThread;
    MainProcess *mainprocess = nullptr;

    void startDevices();
    void updateDeviceState(QPushButton *sender, int state);
    void judgeAuthority();
    void showMsg(QString msg);
};
#endif // MAINWINDOW_H
