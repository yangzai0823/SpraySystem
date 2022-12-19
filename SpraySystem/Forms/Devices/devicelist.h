#ifndef DEVICELIST_H
#define DEVICELIST_H

#include <QMainWindow>
#include <QDesktopWidget>
#include <iostream>
#include <QTime>
#include <QDate>
#include <QDateTime>
#include <memory>
#include <QListWidgetItem>
#include "DataAccess/Models/Camera/camera.h"
#include "DataAccess/Repositories/Camera/camerarepository.h"
#include "DataAccess/Repositories/Robot/robotrepository.h"
#include "DataAccess/Repositories/PLC/plcrepository.h"

#include "Device/Camera/cameraoperator.h"
#include "Device/Robot/robotoperator.h"
#include "Device/devicemanager.h"


namespace Ui {
class DeviceList;
}

class DeviceList : public QMainWindow
{
    Q_OBJECT

public:
    explicit DeviceList(QWidget *parent = nullptr);
    ~DeviceList();

private slots:

    void on_btn_RobotSend_clicked();

    void on_btn_RobotReset_clicked();

    void on_btn_RobotSave_clicked();

    void on_btn_RobotConnect_clicked();


//    void on_btn_Start_clicked();

    void on_btn_CameraSave_clicked();

    void on_lst_Robot_itemSelectionChanged();

    void on_lst_Camera_itemSelectionChanged();

    void on_btn_PLCSave_clicked();

    void on_btn_PLCConnect_clicked();

    void on_tabWidget_currentChanged(int index);

    void receiveData_Slot(QVariant varData);
    void newConnection_Slot(QString ip,int port);

    void on_btn_MCSave_clicked();

    void on_btn_MCConnect_clicked();

    void on_btn_MCSend_clicked();

    void on_btn_MCReset_clicked();
signals:
    void client_Singal(QString msg);
private:
    Ui::DeviceList *ui;
    QDesktopWidget *m_pDeskdop;
    bool isrbtconnected;
    bool iscamconnected;
    bool ismcconnected;
    bool isplcconnected;
    DeviceManager *devicemanager;


    void initCamera();
    void initRobot();
    void initPLC();
    void initMC();
    void ShowRbtMessage(QString msg,bool recv);
    void ShowPlcMessage(QString msg, bool recv);
    void ShowMcMessage(QString msg,bool recv);
};

#endif // DEVICELIST_H
