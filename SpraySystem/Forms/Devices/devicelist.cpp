#include "devicelist.h"
#include "ui_devicelist.h"
#include <QUuid>
#include <QVariant>

DeviceList::DeviceList(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DeviceList)
{
    ui->setupUi(this);

    m_pDeskdop = QApplication::desktop();
    move((m_pDeskdop->width() - this->width())/2, (m_pDeskdop->height() - this->height())/2);

    isrbtconnected = false;
    devicemanager = DeviceManager::getInstance();
    initCamera();
    initRobot();
    initPLC();
    initMC();
}

DeviceList::~DeviceList()
{
    delete ui;
}

#pragma region 相机控制{

void funcss(const VWSCamera::ImageData &data, void* pUser){
//    static int64_t q = data.RGB8PlanarImage.nTimeStamp;
//    static int i = 0;
//    std::cout<<++i<<"  "<<data.RGB8PlanarImage.nTimeStamp<<"      "<<data.RGB8PlanarImage.nTimeStamp - q<<std::endl;
//    q = data.RGB8PlanarImage.nTimeStamp;

//    ((VWSCamera *)pUser)->deleteImage(data);
}

//void DeviceList::on_btn_Start_clicked()
//{

//    int success =-1;

//    QString ip = ui->edit_CameraIP->text();
//    int port = ui->edit_CameraPort->text().toInt();
//    QString name = ui->lst_Camera->selectedItems().at(0)->text();
//    auto camera = devicemanager->getCamera(name);


//    if(iscamconnected){
//        camera->close();
//        success=1;
//    }
//    else
//    {
//        success = camera->init();
//        if(success>0){
//            success = camera->start();

//             camera->RegisterFrameCallBack(funcss,(void *)(&camera));
//        }
//    }

//    if(success>=0)
//    {
//        iscamconnected = !iscamconnected;
//        QIcon ico;
//        if(!iscamconnected)
//        {
//            ico.addFile("://Images/start.png");
//        }
//        else
//        {
//            ico.addFile("://Images/stop.png");
//        }
//        ui->btn_CameraStart->setIcon(ico);
//    }
//    else{
//        ShowRbtMessage("相机连接创建失败",false);
//    }
//}

void DeviceList::on_btn_CameraSave_clicked()
{
    auto item = ui->lst_Camera->selectedItems().at(0);
    QString name =  item->text();
    auto camera = devicemanager->getCamera(name);
    camera->setIP( ui->edit_CameraIP->text());
    camera->setPort(ui->edit_CameraPort->text().toInt());
    devicemanager->saveCamera();
}

void DeviceList::initCamera()
{
    auto lstCamera = devicemanager->cameraList();
    if(lstCamera.length()==0){
        devicemanager->addCamera("相机1","",0);
        devicemanager->addCamera("相加2","",0);
    }
    ui->lst_Camera->addItem("相机1");
    ui->lst_Camera->addItem("相机2");

    ui->lst_Camera->item(0)->setSelected(true);

}


void DeviceList::on_lst_Camera_itemSelectionChanged()
{
    QString name = ui->lst_Camera->selectedItems().at(0)->text();
    auto camera =  devicemanager->getCamera(name);
    if(camera!=nullptr){
        ui->edit_CameraIP->setText(camera->getIP());
        ui->edit_CameraPort->setText(QString::number(camera->getPort()));
        //        ui->edit_Exposure->setText(QString("%1").arg(camera->Exposure));
        //        ui->edit_Frame->setText(QString("%1").arg(camera->Frame));
    }
}



#pragma endregion}


#pragma region 机器人控制{

void DeviceList::initRobot()
{
    auto lstRobot = devicemanager->robotList();
    if(lstRobot.length()==0){
        devicemanager->addRobot("机器人1","",1);
        devicemanager->addRobot("机器人2","",2);
    }
    ui->lst_Robot->addItem("机器人1");
    ui->lst_Robot->addItem("机器人2");


    ui->lst_Robot->item(0)->setSelected(true);
}

void DeviceList::on_btn_RobotSave_clicked()
{
    auto item = ui->lst_Robot->selectedItems().at(0);
    QString name =  item->text();
    auto robot = devicemanager->getRobot(name);


    robot->setIP(ui->edit_RobotIP->text());
    robot->setPort(ui->edit_RobotPort->text().toInt());

    devicemanager->saveRobot();
}

void DeviceList::on_lst_Robot_itemSelectionChanged()
{
    QString name = ui->lst_Robot->selectedItems().at(0)->text();
    auto robot = devicemanager->getRobot(name);
    if(robot!=nullptr){
        ui->edit_RobotIP->setText(robot->getIP());
        ui->edit_RobotPort->setText(QString::number( robot->getPort()));
    }
}

void DeviceList::ShowRbtMessage(QString msg, bool recv)
{
    QString type;
    if(recv){
        type="接收消息";
    }else{
        type = "发送消息";
    }

    QString strTime = QTime::currentTime().toString("hh:mm:ss");
    QString formatMsg =ui->lbl_RobotMsg->text() + strTime+"，"+type+"：\n"+msg +"\n";
    ui->lbl_RobotMsg->setText(formatMsg);
}

void DeviceList::on_btn_RobotSend_clicked()
{
    QString name = ui->lst_Robot->selectedItems().at(0)->text();
    auto robot = devicemanager->getRobot(name);

    if(!isrbtconnected)
    {
        return;
    }
    QString msg = "";
    int success;



    ShowRbtMessage(msg,false);

}

void DeviceList::on_btn_RobotReset_clicked()
{
    ui->lbl_RobotMsg->clear();
}

void DeviceList::on_btn_RobotConnect_clicked()
{


    int success =-1;

    QString ip = ui->edit_RobotIP->text();
    int port = ui->edit_RobotPort->text().toInt();
    QString name = ui->lst_Robot->selectedItems().at(0)->text();
    auto robot = devicemanager->getRobot(name);

    if(isrbtconnected){
        robot->close();
        success=1;
    }
    else
    {
        success = robot->init();
        if(success>0){
            success = robot->start();

            VWSRobot::RobotPosition p;
            robot->getRobotPosition(p);

            QString msg;
            msg+=QString("Joint: %f %f %f %f %f %f \n").arg(p.RobJoint[0]).arg(p.RobJoint[1]).arg(p.RobJoint[2]).arg(p.RobJoint[3]).arg(p.RobJoint[4]).arg(p.RobJoint[5]);
            msg+=QString("Pos: %f %f %f \n").arg(p.pos[0]).arg(p.pos[1]).arg(p.pos[2]);
            msg+=QString("Orient: %f %f %f %f \n").arg(p.orient[0]).arg(p.orient[1]).arg(p.orient[2]).arg(p.orient[3]);
            ShowRbtMessage(msg,true);
        }
    }

    if(success>=0)
    {
        isrbtconnected = !isrbtconnected;
        QIcon ico;
        if(!isrbtconnected)
        {
            ico.addFile("://Images/start.png");
        }
        else
        {
            ico.addFile("://Images/stop.png");
        }
        ui->btn_RobotConnect->setIcon(ico);
    }
    else{
        ShowRbtMessage("机器人连接创建失败",false);
    }
}

#pragma endregion}


#pragma region PLC{

void DeviceList::initPLC(){
    auto plc = devicemanager->getPlc();
    if(plc==nullptr){
        devicemanager->addPLC("PLC","",0);
    }

    ui->edit_PLCIP->setText(plc->getIP());
    ui->edit_PLCPort->setText(QString::number(plc->getPort()));


    connect(plc,SIGNAL(recevieData_Signal(QVariant)),this,SLOT(receiveData_Slot(QVariant)));
//    connect(plc,SIGNAL(newConnection_Signal(QString, int)),this,SLOT(newConnection_Slot(QString,int)));
}

void DeviceList::on_btn_PLCSave_clicked()
{
    auto plc = devicemanager->getPlc();
    plc->setIP(ui->edit_PLCIP->text());
    plc->setPort(ui->edit_PLCPort->text().toInt());
    devicemanager->savePLC();
}

void DeviceList::on_btn_PLCConnect_clicked()
{
    int success=-1;

    if(ui->rb_PLC->isChecked()){
        auto plc = devicemanager->getPlc();
        success = plc->start();
    }
    else{
        auto mc = devicemanager->getMC();
        success = mc->start();
    }
    if(success>=0){
        isrbtconnected = !isrbtconnected;
        QIcon ico;
        if(!isrbtconnected)
        {
            ico.addFile("://Images/start.png");
        }
        else
        {
            ico.addFile("://Images/stop.png");
        }
        ui->btn_PLCConnect->setIcon(ico);
    }else
    {
        ShowRbtMessage("TCP 连接创建失败",false);
    }
}

void DeviceList::ShowPlcMessage(QString msg, bool recv)
{
    QString type;
    if(recv){
        type="接收消息";
    }else{
        type = "发送消息";
    }

    QString strTime = QTime::currentTime().toString("hh:mm:ss");
    QString formatMsg =ui->lbl_PLCMsg->text() + strTime+"，"+type+"：\n"+msg +"\n";
    ui->lbl_PLCMsg->setText(formatMsg);
}

void DeviceList::on_btn_PLCSend_clicked()
{
    if(ui->rb_PLC->isChecked()){
        auto data = ui->rbt_OK->isChecked()?"1":"0";
        devicemanager->getPlc()->sendData(data);
        ShowPlcMessage(data,false);
    }
    else{
        devicemanager->getMC()->sendTrajParam(5799,-300);
//        auto ret = devicemanager->getMC()->getChainEncoders();
//        std::cout<<"返回至："<<ret.at(0)<<std::endl;
//        auto ret1 = devicemanager->getMC()->getChainEncoders();
//        auto ret2 = devicemanager->getMC()->getRealTimeEncoder();

    }
}

void DeviceList::receiveData_Slot(QVariant varData)
{
    PLCData data= varData.value<PLCData>();
    QString msg= QString("测距仪1： %1,  测距仪2： %2， 测距仪3：%3， 测距仪4：%4 \n").arg(data.laser1).arg(data.laser2).arg(data.laser3).arg(data.laser4);
    ShowPlcMessage(msg,true);
}

void DeviceList::newConnection_Slot(QString ip, int port)
{
    QString msg=QString("新客户端链接： IP： %1， Port： %2 \n" ).arg(ip).arg(port);
    ShowPlcMessage(msg,true);
}

#pragma endregion}

#pragma region 运动控制器{
void DeviceList::initMC(){
    auto mc = devicemanager->getMC();
    if(mc==nullptr){
        devicemanager->addMC("MC","",0);
    }

    ui->edit_MCIP->setText(mc->getIP());
    ui->edit_MCPort->setText(QString::number(mc->getPort()));


   // connect(mc,SIGNAL(recevieData_Signal(QVariant)),this,SLOT(receiveData_Slot(QVariant)));
//    connect(plc,SIGNAL(newConnection_Signal(QString, int)),this,SLOT(newConnection_Slot(QString,int)));
}

void DeviceList::on_btn_MCSave_clicked()
{
    auto mc = devicemanager->getMC();
    mc->setIP(ui->edit_MCIP->text());
    mc->setPort(ui->edit_MCPort->text().toInt());
    devicemanager->saveMC();


}
#pragma endregion}

void DeviceList::on_tabWidget_currentChanged(int index)
{
//    devicemanager->disconnectDevices();
    QIcon ico;

    ico.addFile("://Images/start.png");


    ui->btn_RobotConnect->setIcon(ico);
    ui->btn_PLCConnect->setIcon(ico);
}




