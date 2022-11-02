/********************************************************************************
** Form generated from reading UI file 'devicelist.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DEVICELIST_H
#define UI_DEVICELIST_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DeviceList
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout_7;
    QTabWidget *tabWidget;
    QWidget *tab_Camrea;
    QGridLayout *gridLayout_5;
    QHBoxLayout *horizontalLayout_9;
    QListWidget *lst_Camera;
    QGridLayout *gridLayout_3;
    QHBoxLayout *horizontalLayout_8;
    QPushButton *btn_CameraStart;
    QPushButton *btn_CameraSave;
    QSpacerItem *horizontalSpacer;
    QFrame *frame;
    QLabel *label_15;
    QGridLayout *gridLayout;
    QLineEdit *edit_CameraPort;
    QLabel *label;
    QLabel *label_4;
    QLineEdit *edit_Exposure;
    QLineEdit *edit_CameraIP;
    QLabel *label_2;
    QLabel *label_3;
    QLineEdit *edit_Frame;
    QSpacerItem *verticalSpacer;
    QWidget *tab_Robot;
    QGridLayout *gridLayout_14;
    QHBoxLayout *horilayout_robot;
    QListWidget *lst_Robot;
    QVBoxLayout *verilayout_rbtsave;
    QHBoxLayout *horizontalLayout_4;
    QPushButton *btn_RobotSave;
    QSpacerItem *horizontalSpacer_3;
    QGridLayout *gridLayout_2;
    QLabel *label_16;
    QLineEdit *edit_RobotIP;
    QLabel *label_18;
    QLineEdit *edit_RobotPort;
    QSpacerItem *verticalSpacer_3;
    QGroupBox *gpb_rbtdebug;
    QGridLayout *gridLayout_16;
    QLabel *lbl_RobotMsg;
    QHBoxLayout *horizontalLayout_2;
    QRadioButton *radioButton;
    QRadioButton *radioButton_2;
    QRadioButton *radioButton_3;
    QRadioButton *radioButton_4;
    QGridLayout *gridLayout_4;
    QLabel *label_5;
    QLineEdit *edit_RobotSpeed;
    QLabel *label_6;
    QHBoxLayout *horizontalLayout_5;
    QLineEdit *edit_l1;
    QLineEdit *edit_l2;
    QLineEdit *edit_l3;
    QLineEdit *edit_l4;
    QLineEdit *edit_l5;
    QLineEdit *edit_l6;
    QLineEdit *edit_l7;
    QSpacerItem *horizontalSpacer_4;
    QLabel *label_7;
    QLineEdit *edit_IO;
    QLabel *label_8;
    QLineEdit *edit_IOSingal;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *btn_RobotConnect;
    QPushButton *btn_RobotSend;
    QPushButton *btn_RobotReset;
    QWidget *tab_PLC;
    QGridLayout *gridLayout_20;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout_6;
    QPushButton *btn_PLCSave;
    QSpacerItem *horizontalSpacer_5;
    QGridLayout *gridLayout_6;
    QLabel *label_20;
    QLineEdit *edit_PLCIP;
    QLabel *label_21;
    QLineEdit *edit_PLCPort;
    QGroupBox *gpb_plcdebug;
    QVBoxLayout *verticalLayout;
    QLabel *lbl_PLCMsg;
    QHBoxLayout *horizontalLayout_10;
    QRadioButton *rbt_OK;
    QRadioButton *rbt_NG;
    QSpacerItem *horizontalSpacer_7;
    QHBoxLayout *horizontalLayout_7;
    QSpacerItem *horizontalSpacer_6;
    QRadioButton *rb_PLC;
    QRadioButton *rb_MC;
    QPushButton *btn_PLCConnect;
    QPushButton *btn_PLCSend;
    QPushButton *btn_PLCReset;
    QGroupBox *gb_MC;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_20;
    QPushButton *btn_MCSave;
    QSpacerItem *horizontalSpacer_16;
    QGridLayout *gridLayout_19;
    QLabel *label_29;
    QLineEdit *edit_MCIP;
    QLabel *label_30;
    QLineEdit *edit_MCPort;
    QSpacerItem *verticalSpacer_4;

    void setupUi(QMainWindow *DeviceList)
    {
        if (DeviceList->objectName().isEmpty())
            DeviceList->setObjectName(QString::fromUtf8("DeviceList"));
        DeviceList->resize(1166, 736);
        DeviceList->setMinimumSize(QSize(800, 600));
        DeviceList->setMaximumSize(QSize(1600, 1200));
        centralwidget = new QWidget(DeviceList);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        QFont font;
        font.setPointSize(18);
        centralwidget->setFont(font);
        gridLayout_7 = new QGridLayout(centralwidget);
        gridLayout_7->setObjectName(QString::fromUtf8("gridLayout_7"));
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tab_Camrea = new QWidget();
        tab_Camrea->setObjectName(QString::fromUtf8("tab_Camrea"));
        gridLayout_5 = new QGridLayout(tab_Camrea);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        lst_Camera = new QListWidget(tab_Camrea);
        lst_Camera->setObjectName(QString::fromUtf8("lst_Camera"));
        lst_Camera->setMinimumSize(QSize(100, 0));
        lst_Camera->setMaximumSize(QSize(200, 16777215));

        horizontalLayout_9->addWidget(lst_Camera);

        gridLayout_3 = new QGridLayout();
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        btn_CameraStart = new QPushButton(tab_Camrea);
        btn_CameraStart->setObjectName(QString::fromUtf8("btn_CameraStart"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(btn_CameraStart->sizePolicy().hasHeightForWidth());
        btn_CameraStart->setSizePolicy(sizePolicy);
        btn_CameraStart->setMinimumSize(QSize(40, 40));
        btn_CameraStart->setMaximumSize(QSize(40, 40));
        btn_CameraStart->setStyleSheet(QString::fromUtf8(""));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/Images/start.png"), QSize(), QIcon::Normal, QIcon::Off);
        btn_CameraStart->setIcon(icon);
        btn_CameraStart->setIconSize(QSize(40, 40));
        btn_CameraStart->setAutoRepeat(false);
        btn_CameraStart->setAutoExclusive(false);
        btn_CameraStart->setFlat(true);

        horizontalLayout_8->addWidget(btn_CameraStart);

        btn_CameraSave = new QPushButton(tab_Camrea);
        btn_CameraSave->setObjectName(QString::fromUtf8("btn_CameraSave"));
        sizePolicy.setHeightForWidth(btn_CameraSave->sizePolicy().hasHeightForWidth());
        btn_CameraSave->setSizePolicy(sizePolicy);
        btn_CameraSave->setMinimumSize(QSize(40, 40));
        btn_CameraSave->setMaximumSize(QSize(40, 40));
        btn_CameraSave->setStyleSheet(QString::fromUtf8(""));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/Images/save.png"), QSize(), QIcon::Normal, QIcon::Off);
        btn_CameraSave->setIcon(icon1);
        btn_CameraSave->setIconSize(QSize(40, 40));
        btn_CameraSave->setAutoRepeat(false);
        btn_CameraSave->setAutoExclusive(false);
        btn_CameraSave->setFlat(true);

        horizontalLayout_8->addWidget(btn_CameraSave);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer);


        gridLayout_3->addLayout(horizontalLayout_8, 0, 0, 1, 2);

        frame = new QFrame(tab_Camrea);
        frame->setObjectName(QString::fromUtf8("frame"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(frame->sizePolicy().hasHeightForWidth());
        frame->setSizePolicy(sizePolicy1);
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        label_15 = new QLabel(frame);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        label_15->setGeometry(QRect(290, 250, 121, 71));

        gridLayout_3->addWidget(frame, 1, 0, 2, 1);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        edit_CameraPort = new QLineEdit(tab_Camrea);
        edit_CameraPort->setObjectName(QString::fromUtf8("edit_CameraPort"));
        edit_CameraPort->setMinimumSize(QSize(80, 0));
        edit_CameraPort->setMaximumSize(QSize(100, 16777215));

        gridLayout->addWidget(edit_CameraPort, 1, 1, 1, 1);

        label = new QLabel(tab_Camrea);
        label->setObjectName(QString::fromUtf8("label"));
        label->setMaximumSize(QSize(120, 16777215));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        label_4 = new QLabel(tab_Camrea);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setMaximumSize(QSize(120, 16777215));

        gridLayout->addWidget(label_4, 3, 0, 1, 1);

        edit_Exposure = new QLineEdit(tab_Camrea);
        edit_Exposure->setObjectName(QString::fromUtf8("edit_Exposure"));
        edit_Exposure->setMinimumSize(QSize(80, 0));
        edit_Exposure->setMaximumSize(QSize(100, 16777215));

        gridLayout->addWidget(edit_Exposure, 2, 1, 1, 1);

        edit_CameraIP = new QLineEdit(tab_Camrea);
        edit_CameraIP->setObjectName(QString::fromUtf8("edit_CameraIP"));
        edit_CameraIP->setMinimumSize(QSize(200, 0));
        edit_CameraIP->setMaximumSize(QSize(200, 16777215));

        gridLayout->addWidget(edit_CameraIP, 0, 1, 1, 1);

        label_2 = new QLabel(tab_Camrea);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setMaximumSize(QSize(120, 16777215));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        label_3 = new QLabel(tab_Camrea);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setMaximumSize(QSize(120, 16777215));

        gridLayout->addWidget(label_3, 2, 0, 1, 1);

        edit_Frame = new QLineEdit(tab_Camrea);
        edit_Frame->setObjectName(QString::fromUtf8("edit_Frame"));
        edit_Frame->setMinimumSize(QSize(80, 0));
        edit_Frame->setMaximumSize(QSize(100, 16777215));

        gridLayout->addWidget(edit_Frame, 3, 1, 1, 1);


        gridLayout_3->addLayout(gridLayout, 1, 1, 1, 1);

        verticalSpacer = new QSpacerItem(20, 118, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_3->addItem(verticalSpacer, 2, 1, 1, 1);

        gridLayout_3->setColumnStretch(0, 4);
        gridLayout_3->setColumnStretch(1, 1);

        horizontalLayout_9->addLayout(gridLayout_3);

        horizontalLayout_9->setStretch(0, 1);
        horizontalLayout_9->setStretch(1, 4);

        gridLayout_5->addLayout(horizontalLayout_9, 0, 0, 1, 1);

        tabWidget->addTab(tab_Camrea, QString());
        tab_Robot = new QWidget();
        tab_Robot->setObjectName(QString::fromUtf8("tab_Robot"));
        gridLayout_14 = new QGridLayout(tab_Robot);
        gridLayout_14->setObjectName(QString::fromUtf8("gridLayout_14"));
        horilayout_robot = new QHBoxLayout();
        horilayout_robot->setObjectName(QString::fromUtf8("horilayout_robot"));
        lst_Robot = new QListWidget(tab_Robot);
        lst_Robot->setObjectName(QString::fromUtf8("lst_Robot"));
        lst_Robot->setMinimumSize(QSize(100, 0));
        lst_Robot->setMaximumSize(QSize(200, 16777215));

        horilayout_robot->addWidget(lst_Robot);

        verilayout_rbtsave = new QVBoxLayout();
        verilayout_rbtsave->setObjectName(QString::fromUtf8("verilayout_rbtsave"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        btn_RobotSave = new QPushButton(tab_Robot);
        btn_RobotSave->setObjectName(QString::fromUtf8("btn_RobotSave"));
        sizePolicy.setHeightForWidth(btn_RobotSave->sizePolicy().hasHeightForWidth());
        btn_RobotSave->setSizePolicy(sizePolicy);
        btn_RobotSave->setMinimumSize(QSize(40, 40));
        btn_RobotSave->setMaximumSize(QSize(40, 40));
        btn_RobotSave->setStyleSheet(QString::fromUtf8(""));
        btn_RobotSave->setIcon(icon1);
        btn_RobotSave->setIconSize(QSize(40, 40));
        btn_RobotSave->setAutoRepeat(false);
        btn_RobotSave->setAutoExclusive(false);
        btn_RobotSave->setFlat(true);

        horizontalLayout_4->addWidget(btn_RobotSave);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_3);


        verilayout_rbtsave->addLayout(horizontalLayout_4);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        label_16 = new QLabel(tab_Robot);
        label_16->setObjectName(QString::fromUtf8("label_16"));
        label_16->setMaximumSize(QSize(120, 16777215));

        gridLayout_2->addWidget(label_16, 0, 0, 1, 1);

        edit_RobotIP = new QLineEdit(tab_Robot);
        edit_RobotIP->setObjectName(QString::fromUtf8("edit_RobotIP"));
        edit_RobotIP->setMinimumSize(QSize(200, 0));
        edit_RobotIP->setMaximumSize(QSize(300, 16777215));

        gridLayout_2->addWidget(edit_RobotIP, 0, 1, 1, 1);

        label_18 = new QLabel(tab_Robot);
        label_18->setObjectName(QString::fromUtf8("label_18"));
        label_18->setMaximumSize(QSize(120, 16777215));

        gridLayout_2->addWidget(label_18, 1, 0, 1, 1);

        edit_RobotPort = new QLineEdit(tab_Robot);
        edit_RobotPort->setObjectName(QString::fromUtf8("edit_RobotPort"));
        edit_RobotPort->setMinimumSize(QSize(80, 0));
        edit_RobotPort->setMaximumSize(QSize(100, 16777215));

        gridLayout_2->addWidget(edit_RobotPort, 1, 1, 1, 1);


        verilayout_rbtsave->addLayout(gridLayout_2);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verilayout_rbtsave->addItem(verticalSpacer_3);


        horilayout_robot->addLayout(verilayout_rbtsave);

        gpb_rbtdebug = new QGroupBox(tab_Robot);
        gpb_rbtdebug->setObjectName(QString::fromUtf8("gpb_rbtdebug"));
        gridLayout_16 = new QGridLayout(gpb_rbtdebug);
        gridLayout_16->setObjectName(QString::fromUtf8("gridLayout_16"));
        lbl_RobotMsg = new QLabel(gpb_rbtdebug);
        lbl_RobotMsg->setObjectName(QString::fromUtf8("lbl_RobotMsg"));
        lbl_RobotMsg->setMinimumSize(QSize(0, 0));
        QFont font1;
        font1.setPointSize(12);
        lbl_RobotMsg->setFont(font1);
        lbl_RobotMsg->setLayoutDirection(Qt::LeftToRight);
        lbl_RobotMsg->setStyleSheet(QString::fromUtf8(""));
        lbl_RobotMsg->setFrameShape(QFrame::StyledPanel);
        lbl_RobotMsg->setFrameShadow(QFrame::Raised);
        lbl_RobotMsg->setLineWidth(1);
        lbl_RobotMsg->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);

        gridLayout_16->addWidget(lbl_RobotMsg, 0, 0, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        radioButton = new QRadioButton(gpb_rbtdebug);
        radioButton->setObjectName(QString::fromUtf8("radioButton"));

        horizontalLayout_2->addWidget(radioButton);

        radioButton_2 = new QRadioButton(gpb_rbtdebug);
        radioButton_2->setObjectName(QString::fromUtf8("radioButton_2"));

        horizontalLayout_2->addWidget(radioButton_2);

        radioButton_3 = new QRadioButton(gpb_rbtdebug);
        radioButton_3->setObjectName(QString::fromUtf8("radioButton_3"));

        horizontalLayout_2->addWidget(radioButton_3);

        radioButton_4 = new QRadioButton(gpb_rbtdebug);
        radioButton_4->setObjectName(QString::fromUtf8("radioButton_4"));

        horizontalLayout_2->addWidget(radioButton_4);


        gridLayout_16->addLayout(horizontalLayout_2, 1, 0, 1, 1);

        gridLayout_4 = new QGridLayout();
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        label_5 = new QLabel(gpb_rbtdebug);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout_4->addWidget(label_5, 0, 0, 1, 1);

        edit_RobotSpeed = new QLineEdit(gpb_rbtdebug);
        edit_RobotSpeed->setObjectName(QString::fromUtf8("edit_RobotSpeed"));
        edit_RobotSpeed->setMaximumSize(QSize(50, 16777215));

        gridLayout_4->addWidget(edit_RobotSpeed, 0, 1, 1, 1);

        label_6 = new QLabel(gpb_rbtdebug);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout_4->addWidget(label_6, 1, 0, 1, 1);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        edit_l1 = new QLineEdit(gpb_rbtdebug);
        edit_l1->setObjectName(QString::fromUtf8("edit_l1"));
        edit_l1->setMaximumSize(QSize(50, 16777215));

        horizontalLayout_5->addWidget(edit_l1);

        edit_l2 = new QLineEdit(gpb_rbtdebug);
        edit_l2->setObjectName(QString::fromUtf8("edit_l2"));
        edit_l2->setMaximumSize(QSize(50, 16777215));

        horizontalLayout_5->addWidget(edit_l2);

        edit_l3 = new QLineEdit(gpb_rbtdebug);
        edit_l3->setObjectName(QString::fromUtf8("edit_l3"));
        edit_l3->setMaximumSize(QSize(50, 16777215));

        horizontalLayout_5->addWidget(edit_l3);

        edit_l4 = new QLineEdit(gpb_rbtdebug);
        edit_l4->setObjectName(QString::fromUtf8("edit_l4"));
        edit_l4->setMaximumSize(QSize(50, 16777215));

        horizontalLayout_5->addWidget(edit_l4);

        edit_l5 = new QLineEdit(gpb_rbtdebug);
        edit_l5->setObjectName(QString::fromUtf8("edit_l5"));
        edit_l5->setMaximumSize(QSize(50, 16777215));

        horizontalLayout_5->addWidget(edit_l5);

        edit_l6 = new QLineEdit(gpb_rbtdebug);
        edit_l6->setObjectName(QString::fromUtf8("edit_l6"));
        edit_l6->setMaximumSize(QSize(50, 16777215));

        horizontalLayout_5->addWidget(edit_l6);

        edit_l7 = new QLineEdit(gpb_rbtdebug);
        edit_l7->setObjectName(QString::fromUtf8("edit_l7"));
        edit_l7->setMaximumSize(QSize(50, 16777215));

        horizontalLayout_5->addWidget(edit_l7);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_4);


        gridLayout_4->addLayout(horizontalLayout_5, 1, 1, 1, 1);

        label_7 = new QLabel(gpb_rbtdebug);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        gridLayout_4->addWidget(label_7, 2, 0, 1, 1);

        edit_IO = new QLineEdit(gpb_rbtdebug);
        edit_IO->setObjectName(QString::fromUtf8("edit_IO"));
        edit_IO->setMaximumSize(QSize(50, 16777215));

        gridLayout_4->addWidget(edit_IO, 2, 1, 1, 1);

        label_8 = new QLabel(gpb_rbtdebug);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        gridLayout_4->addWidget(label_8, 3, 0, 1, 1);

        edit_IOSingal = new QLineEdit(gpb_rbtdebug);
        edit_IOSingal->setObjectName(QString::fromUtf8("edit_IOSingal"));
        edit_IOSingal->setMaximumSize(QSize(50, 16777215));

        gridLayout_4->addWidget(edit_IOSingal, 3, 1, 1, 1);


        gridLayout_16->addLayout(gridLayout_4, 2, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        btn_RobotConnect = new QPushButton(gpb_rbtdebug);
        btn_RobotConnect->setObjectName(QString::fromUtf8("btn_RobotConnect"));
        sizePolicy.setHeightForWidth(btn_RobotConnect->sizePolicy().hasHeightForWidth());
        btn_RobotConnect->setSizePolicy(sizePolicy);
        btn_RobotConnect->setMinimumSize(QSize(40, 40));
        btn_RobotConnect->setMaximumSize(QSize(40, 40));
        btn_RobotConnect->setStyleSheet(QString::fromUtf8(""));
        btn_RobotConnect->setIcon(icon);
        btn_RobotConnect->setIconSize(QSize(40, 40));
        btn_RobotConnect->setAutoRepeat(false);
        btn_RobotConnect->setAutoExclusive(false);
        btn_RobotConnect->setFlat(true);

        horizontalLayout->addWidget(btn_RobotConnect);

        btn_RobotSend = new QPushButton(gpb_rbtdebug);
        btn_RobotSend->setObjectName(QString::fromUtf8("btn_RobotSend"));
        sizePolicy.setHeightForWidth(btn_RobotSend->sizePolicy().hasHeightForWidth());
        btn_RobotSend->setSizePolicy(sizePolicy);
        btn_RobotSend->setMinimumSize(QSize(40, 40));
        btn_RobotSend->setMaximumSize(QSize(40, 40));
        btn_RobotSend->setStyleSheet(QString::fromUtf8(""));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/Images/send.png"), QSize(), QIcon::Normal, QIcon::Off);
        btn_RobotSend->setIcon(icon2);
        btn_RobotSend->setIconSize(QSize(40, 40));
        btn_RobotSend->setAutoRepeat(false);
        btn_RobotSend->setAutoExclusive(false);
        btn_RobotSend->setFlat(true);

        horizontalLayout->addWidget(btn_RobotSend);

        btn_RobotReset = new QPushButton(gpb_rbtdebug);
        btn_RobotReset->setObjectName(QString::fromUtf8("btn_RobotReset"));
        sizePolicy.setHeightForWidth(btn_RobotReset->sizePolicy().hasHeightForWidth());
        btn_RobotReset->setSizePolicy(sizePolicy);
        btn_RobotReset->setMinimumSize(QSize(40, 40));
        btn_RobotReset->setMaximumSize(QSize(40, 40));
        btn_RobotReset->setStyleSheet(QString::fromUtf8(""));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/Images/reset.png"), QSize(), QIcon::Normal, QIcon::Off);
        btn_RobotReset->setIcon(icon3);
        btn_RobotReset->setIconSize(QSize(40, 40));
        btn_RobotReset->setAutoRepeat(false);
        btn_RobotReset->setAutoExclusive(false);
        btn_RobotReset->setFlat(true);

        horizontalLayout->addWidget(btn_RobotReset);


        gridLayout_16->addLayout(horizontalLayout, 3, 0, 1, 1);

        gridLayout_16->setRowStretch(0, 10);
        gridLayout_16->setRowStretch(1, 1);
        gridLayout_16->setRowStretch(2, 2);
        gridLayout_16->setRowStretch(3, 1);

        horilayout_robot->addWidget(gpb_rbtdebug);

        horilayout_robot->setStretch(0, 1);
        horilayout_robot->setStretch(1, 1);
        horilayout_robot->setStretch(2, 6);

        gridLayout_14->addLayout(horilayout_robot, 0, 0, 1, 1);

        tabWidget->addTab(tab_Robot, QString());
        tab_PLC = new QWidget();
        tab_PLC->setObjectName(QString::fromUtf8("tab_PLC"));
        gridLayout_20 = new QGridLayout(tab_PLC);
        gridLayout_20->setObjectName(QString::fromUtf8("gridLayout_20"));
        groupBox = new QGroupBox(tab_PLC);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        verticalLayout_4 = new QVBoxLayout(groupBox);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        btn_PLCSave = new QPushButton(groupBox);
        btn_PLCSave->setObjectName(QString::fromUtf8("btn_PLCSave"));
        sizePolicy.setHeightForWidth(btn_PLCSave->sizePolicy().hasHeightForWidth());
        btn_PLCSave->setSizePolicy(sizePolicy);
        btn_PLCSave->setMinimumSize(QSize(40, 40));
        btn_PLCSave->setMaximumSize(QSize(40, 40));
        btn_PLCSave->setStyleSheet(QString::fromUtf8(""));
        btn_PLCSave->setIcon(icon1);
        btn_PLCSave->setIconSize(QSize(40, 40));
        btn_PLCSave->setAutoRepeat(false);
        btn_PLCSave->setAutoExclusive(false);
        btn_PLCSave->setFlat(true);

        horizontalLayout_6->addWidget(btn_PLCSave);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_5);


        verticalLayout_4->addLayout(horizontalLayout_6);

        gridLayout_6 = new QGridLayout();
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        label_20 = new QLabel(groupBox);
        label_20->setObjectName(QString::fromUtf8("label_20"));
        label_20->setMaximumSize(QSize(120, 16777215));

        gridLayout_6->addWidget(label_20, 0, 0, 1, 1);

        edit_PLCIP = new QLineEdit(groupBox);
        edit_PLCIP->setObjectName(QString::fromUtf8("edit_PLCIP"));
        edit_PLCIP->setMinimumSize(QSize(200, 0));
        edit_PLCIP->setMaximumSize(QSize(180, 16777215));

        gridLayout_6->addWidget(edit_PLCIP, 0, 1, 1, 1);

        label_21 = new QLabel(groupBox);
        label_21->setObjectName(QString::fromUtf8("label_21"));
        label_21->setMaximumSize(QSize(120, 16777215));

        gridLayout_6->addWidget(label_21, 1, 0, 1, 1);

        edit_PLCPort = new QLineEdit(groupBox);
        edit_PLCPort->setObjectName(QString::fromUtf8("edit_PLCPort"));
        edit_PLCPort->setMinimumSize(QSize(80, 0));
        edit_PLCPort->setMaximumSize(QSize(100, 16777215));

        gridLayout_6->addWidget(edit_PLCPort, 1, 1, 1, 1);


        verticalLayout_4->addLayout(gridLayout_6);


        gridLayout_20->addWidget(groupBox, 0, 0, 1, 1);

        gpb_plcdebug = new QGroupBox(tab_PLC);
        gpb_plcdebug->setObjectName(QString::fromUtf8("gpb_plcdebug"));
        verticalLayout = new QVBoxLayout(gpb_plcdebug);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        lbl_PLCMsg = new QLabel(gpb_plcdebug);
        lbl_PLCMsg->setObjectName(QString::fromUtf8("lbl_PLCMsg"));
        lbl_PLCMsg->setMinimumSize(QSize(0, 0));
        lbl_PLCMsg->setFont(font1);
        lbl_PLCMsg->setLayoutDirection(Qt::LeftToRight);
        lbl_PLCMsg->setStyleSheet(QString::fromUtf8(""));
        lbl_PLCMsg->setFrameShape(QFrame::StyledPanel);
        lbl_PLCMsg->setFrameShadow(QFrame::Raised);
        lbl_PLCMsg->setLineWidth(1);
        lbl_PLCMsg->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);

        verticalLayout->addWidget(lbl_PLCMsg);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setObjectName(QString::fromUtf8("horizontalLayout_10"));
        rbt_OK = new QRadioButton(gpb_plcdebug);
        rbt_OK->setObjectName(QString::fromUtf8("rbt_OK"));
        rbt_OK->setChecked(true);

        horizontalLayout_10->addWidget(rbt_OK);

        rbt_NG = new QRadioButton(gpb_plcdebug);
        rbt_NG->setObjectName(QString::fromUtf8("rbt_NG"));

        horizontalLayout_10->addWidget(rbt_NG);

        horizontalSpacer_7 = new QSpacerItem(78, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_10->addItem(horizontalSpacer_7);


        verticalLayout->addLayout(horizontalLayout_10);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        horizontalSpacer_6 = new QSpacerItem(218, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_6);

        rb_PLC = new QRadioButton(gpb_plcdebug);
        rb_PLC->setObjectName(QString::fromUtf8("rb_PLC"));
        rb_PLC->setChecked(true);

        horizontalLayout_7->addWidget(rb_PLC);

        rb_MC = new QRadioButton(gpb_plcdebug);
        rb_MC->setObjectName(QString::fromUtf8("rb_MC"));
        rb_MC->setChecked(false);

        horizontalLayout_7->addWidget(rb_MC);

        btn_PLCConnect = new QPushButton(gpb_plcdebug);
        btn_PLCConnect->setObjectName(QString::fromUtf8("btn_PLCConnect"));
        sizePolicy.setHeightForWidth(btn_PLCConnect->sizePolicy().hasHeightForWidth());
        btn_PLCConnect->setSizePolicy(sizePolicy);
        btn_PLCConnect->setMinimumSize(QSize(40, 40));
        btn_PLCConnect->setMaximumSize(QSize(40, 40));
        btn_PLCConnect->setStyleSheet(QString::fromUtf8(""));
        btn_PLCConnect->setIcon(icon);
        btn_PLCConnect->setIconSize(QSize(40, 40));
        btn_PLCConnect->setAutoRepeat(false);
        btn_PLCConnect->setAutoExclusive(false);
        btn_PLCConnect->setFlat(true);

        horizontalLayout_7->addWidget(btn_PLCConnect);

        btn_PLCSend = new QPushButton(gpb_plcdebug);
        btn_PLCSend->setObjectName(QString::fromUtf8("btn_PLCSend"));
        sizePolicy.setHeightForWidth(btn_PLCSend->sizePolicy().hasHeightForWidth());
        btn_PLCSend->setSizePolicy(sizePolicy);
        btn_PLCSend->setMinimumSize(QSize(40, 40));
        btn_PLCSend->setMaximumSize(QSize(40, 40));
        btn_PLCSend->setStyleSheet(QString::fromUtf8(""));
        btn_PLCSend->setIcon(icon2);
        btn_PLCSend->setIconSize(QSize(40, 40));
        btn_PLCSend->setAutoRepeat(false);
        btn_PLCSend->setAutoExclusive(false);
        btn_PLCSend->setFlat(true);

        horizontalLayout_7->addWidget(btn_PLCSend);

        btn_PLCReset = new QPushButton(gpb_plcdebug);
        btn_PLCReset->setObjectName(QString::fromUtf8("btn_PLCReset"));
        sizePolicy.setHeightForWidth(btn_PLCReset->sizePolicy().hasHeightForWidth());
        btn_PLCReset->setSizePolicy(sizePolicy);
        btn_PLCReset->setMinimumSize(QSize(40, 40));
        btn_PLCReset->setMaximumSize(QSize(40, 40));
        btn_PLCReset->setStyleSheet(QString::fromUtf8(""));
        btn_PLCReset->setIcon(icon3);
        btn_PLCReset->setIconSize(QSize(40, 40));
        btn_PLCReset->setAutoRepeat(false);
        btn_PLCReset->setAutoExclusive(false);
        btn_PLCReset->setFlat(true);

        horizontalLayout_7->addWidget(btn_PLCReset);


        verticalLayout->addLayout(horizontalLayout_7);

        verticalLayout->setStretch(0, 5);
        verticalLayout->setStretch(1, 1);
        verticalLayout->setStretch(2, 1);

        gridLayout_20->addWidget(gpb_plcdebug, 0, 1, 3, 1);

        gb_MC = new QGroupBox(tab_PLC);
        gb_MC->setObjectName(QString::fromUtf8("gb_MC"));
        verticalLayout_3 = new QVBoxLayout(gb_MC);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        horizontalLayout_20 = new QHBoxLayout();
        horizontalLayout_20->setObjectName(QString::fromUtf8("horizontalLayout_20"));
        btn_MCSave = new QPushButton(gb_MC);
        btn_MCSave->setObjectName(QString::fromUtf8("btn_MCSave"));
        sizePolicy.setHeightForWidth(btn_MCSave->sizePolicy().hasHeightForWidth());
        btn_MCSave->setSizePolicy(sizePolicy);
        btn_MCSave->setMinimumSize(QSize(40, 40));
        btn_MCSave->setMaximumSize(QSize(40, 40));
        btn_MCSave->setStyleSheet(QString::fromUtf8(""));
        btn_MCSave->setIcon(icon1);
        btn_MCSave->setIconSize(QSize(40, 40));
        btn_MCSave->setAutoRepeat(false);
        btn_MCSave->setAutoExclusive(false);
        btn_MCSave->setFlat(true);

        horizontalLayout_20->addWidget(btn_MCSave);

        horizontalSpacer_16 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_20->addItem(horizontalSpacer_16);


        verticalLayout_3->addLayout(horizontalLayout_20);

        gridLayout_19 = new QGridLayout();
        gridLayout_19->setObjectName(QString::fromUtf8("gridLayout_19"));
        label_29 = new QLabel(gb_MC);
        label_29->setObjectName(QString::fromUtf8("label_29"));
        label_29->setMaximumSize(QSize(120, 16777215));

        gridLayout_19->addWidget(label_29, 0, 0, 1, 1);

        edit_MCIP = new QLineEdit(gb_MC);
        edit_MCIP->setObjectName(QString::fromUtf8("edit_MCIP"));
        edit_MCIP->setMinimumSize(QSize(200, 0));
        edit_MCIP->setMaximumSize(QSize(180, 16777215));

        gridLayout_19->addWidget(edit_MCIP, 0, 1, 1, 1);

        label_30 = new QLabel(gb_MC);
        label_30->setObjectName(QString::fromUtf8("label_30"));
        label_30->setMaximumSize(QSize(120, 16777215));

        gridLayout_19->addWidget(label_30, 1, 0, 1, 1);

        edit_MCPort = new QLineEdit(gb_MC);
        edit_MCPort->setObjectName(QString::fromUtf8("edit_MCPort"));
        edit_MCPort->setMinimumSize(QSize(80, 0));
        edit_MCPort->setMaximumSize(QSize(100, 16777215));

        gridLayout_19->addWidget(edit_MCPort, 1, 1, 1, 1);


        verticalLayout_3->addLayout(gridLayout_19);


        gridLayout_20->addWidget(gb_MC, 1, 0, 1, 1);

        verticalSpacer_4 = new QSpacerItem(20, 277, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_20->addItem(verticalSpacer_4, 2, 0, 1, 1);

        gridLayout_20->setColumnStretch(0, 1);
        gridLayout_20->setColumnStretch(1, 3);
        tabWidget->addTab(tab_PLC, QString());

        gridLayout_7->addWidget(tabWidget, 0, 0, 1, 1);

        DeviceList->setCentralWidget(centralwidget);
        QWidget::setTabOrder(lst_Camera, edit_CameraIP);
        QWidget::setTabOrder(edit_CameraIP, edit_CameraPort);
        QWidget::setTabOrder(edit_CameraPort, edit_Exposure);
        QWidget::setTabOrder(edit_Exposure, edit_Frame);
        QWidget::setTabOrder(edit_Frame, btn_CameraStart);
        QWidget::setTabOrder(btn_CameraStart, btn_CameraSave);
        QWidget::setTabOrder(btn_CameraSave, lst_Robot);
        QWidget::setTabOrder(lst_Robot, edit_RobotIP);
        QWidget::setTabOrder(edit_RobotIP, edit_RobotPort);
        QWidget::setTabOrder(edit_RobotPort, btn_RobotSave);
        QWidget::setTabOrder(btn_RobotSave, btn_RobotConnect);
        QWidget::setTabOrder(btn_RobotConnect, btn_RobotSend);
        QWidget::setTabOrder(btn_RobotSend, btn_RobotReset);
        QWidget::setTabOrder(btn_RobotReset, edit_Exposure);
        QWidget::setTabOrder(edit_Exposure, edit_CameraPort);
        QWidget::setTabOrder(edit_CameraPort, edit_CameraPort);
        QWidget::setTabOrder(edit_CameraPort, edit_Frame);
        QWidget::setTabOrder(edit_Frame, edit_Exposure);
        QWidget::setTabOrder(edit_Exposure, edit_CameraIP);
        QWidget::setTabOrder(edit_CameraIP, edit_Frame);
        QWidget::setTabOrder(edit_Frame, edit_CameraIP);

        retranslateUi(DeviceList);

        tabWidget->setCurrentIndex(2);


        QMetaObject::connectSlotsByName(DeviceList);
    } // setupUi

    void retranslateUi(QMainWindow *DeviceList)
    {
        DeviceList->setWindowTitle(QApplication::translate("DeviceList", "\350\256\276\345\244\207\345\210\227\350\241\250", nullptr));
        btn_CameraStart->setText(QString());
        btn_CameraSave->setText(QString());
        label_15->setText(QApplication::translate("DeviceList", "\345\233\276\345\203\217\346\230\276\347\244\272", nullptr));
        label->setText(QApplication::translate("DeviceList", "IP", nullptr));
        label_4->setText(QApplication::translate("DeviceList", "\345\270\247\347\216\207", nullptr));
        label_2->setText(QApplication::translate("DeviceList", "\347\253\257\345\217\243\345\217\267", nullptr));
        label_3->setText(QApplication::translate("DeviceList", "\346\233\235\345\205\211\346\227\266\351\227\264", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_Camrea), QApplication::translate("DeviceList", "\347\233\270\346\234\272", nullptr));
        btn_RobotSave->setText(QString());
        label_16->setText(QApplication::translate("DeviceList", "IP", nullptr));
        edit_RobotIP->setText(QApplication::translate("DeviceList", "192.168.32.129", nullptr));
        label_18->setText(QApplication::translate("DeviceList", "\347\253\257\345\217\243\345\217\267", nullptr));
        edit_RobotPort->setText(QApplication::translate("DeviceList", "10086", nullptr));
        gpb_rbtdebug->setTitle(QApplication::translate("DeviceList", "\350\260\203\350\257\225", nullptr));
        lbl_RobotMsg->setText(QString());
        radioButton->setText(QApplication::translate("DeviceList", "\345\226\267\346\236\252", nullptr));
        radioButton_2->setText(QApplication::translate("DeviceList", "MoveL", nullptr));
        radioButton_3->setText(QApplication::translate("DeviceList", "MoveJ", nullptr));
        radioButton_4->setText(QApplication::translate("DeviceList", "MoveAbsJ", nullptr));
        label_5->setText(QApplication::translate("DeviceList", "\351\200\237\345\272\246", nullptr));
        label_6->setText(QApplication::translate("DeviceList", "\350\275\250\350\277\271", nullptr));
        label_7->setText(QApplication::translate("DeviceList", "IO", nullptr));
        label_8->setText(QApplication::translate("DeviceList", "IO\344\277\241\345\217\267", nullptr));
#ifndef QT_NO_TOOLTIP
        btn_RobotConnect->setToolTip(QApplication::translate("DeviceList", "\350\277\236\346\216\245", nullptr));
#endif // QT_NO_TOOLTIP
        btn_RobotConnect->setText(QString());
#ifndef QT_NO_TOOLTIP
        btn_RobotSend->setToolTip(QApplication::translate("DeviceList", "\345\217\221\351\200\201", nullptr));
#endif // QT_NO_TOOLTIP
        btn_RobotSend->setText(QString());
#ifndef QT_NO_TOOLTIP
        btn_RobotReset->setToolTip(QApplication::translate("DeviceList", "\346\270\205\347\251\272", nullptr));
#endif // QT_NO_TOOLTIP
        btn_RobotReset->setText(QString());
        tabWidget->setTabText(tabWidget->indexOf(tab_Robot), QApplication::translate("DeviceList", "\346\234\272\345\231\250\344\272\272", nullptr));
        groupBox->setTitle(QApplication::translate("DeviceList", "PLC", nullptr));
        btn_PLCSave->setText(QString());
        label_20->setText(QApplication::translate("DeviceList", "IP", nullptr));
        edit_PLCIP->setText(QApplication::translate("DeviceList", "192.168.32.129", nullptr));
        label_21->setText(QApplication::translate("DeviceList", "\347\253\257\345\217\243\345\217\267", nullptr));
        edit_PLCPort->setText(QApplication::translate("DeviceList", "10086", nullptr));
        gpb_plcdebug->setTitle(QApplication::translate("DeviceList", "\350\260\203\350\257\225", nullptr));
        lbl_PLCMsg->setText(QString());
        rbt_OK->setText(QApplication::translate("DeviceList", "\346\210\220\345\212\237", nullptr));
        rbt_NG->setText(QApplication::translate("DeviceList", "\345\244\261\350\264\245", nullptr));
        rb_PLC->setText(QApplication::translate("DeviceList", "PLC", nullptr));
        rb_MC->setText(QApplication::translate("DeviceList", "\350\277\220\345\212\250\346\216\247\345\210\266\345\231\250", nullptr));
#ifndef QT_NO_TOOLTIP
        btn_PLCConnect->setToolTip(QApplication::translate("DeviceList", "\350\277\236\346\216\245", nullptr));
#endif // QT_NO_TOOLTIP
        btn_PLCConnect->setText(QString());
#ifndef QT_NO_TOOLTIP
        btn_PLCSend->setToolTip(QApplication::translate("DeviceList", "\345\217\221\351\200\201", nullptr));
#endif // QT_NO_TOOLTIP
        btn_PLCSend->setText(QString());
#ifndef QT_NO_TOOLTIP
        btn_PLCReset->setToolTip(QApplication::translate("DeviceList", "\346\270\205\347\251\272", nullptr));
#endif // QT_NO_TOOLTIP
        btn_PLCReset->setText(QString());
        gb_MC->setTitle(QApplication::translate("DeviceList", "\350\277\220\345\212\250\346\216\247\345\210\266\345\231\250", nullptr));
        btn_MCSave->setText(QString());
        label_29->setText(QApplication::translate("DeviceList", "IP", nullptr));
        edit_MCIP->setText(QApplication::translate("DeviceList", "192.168.32.129", nullptr));
        label_30->setText(QApplication::translate("DeviceList", "\347\253\257\345\217\243\345\217\267", nullptr));
        edit_MCPort->setText(QApplication::translate("DeviceList", "10086", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_PLC), QApplication::translate("DeviceList", "PLC +\350\277\220\345\212\250\346\216\247\345\210\266\345\231\250", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DeviceList: public Ui_DeviceList {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DEVICELIST_H
