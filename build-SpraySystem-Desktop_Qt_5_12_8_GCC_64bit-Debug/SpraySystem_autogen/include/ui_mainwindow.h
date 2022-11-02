/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actQuit;
    QAction *actChangeUser;
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QFrame *frame_Nav;
    QVBoxLayout *verticalLayout;
    QToolButton *btn_User;
    QToolButton *btn_Product;
    QToolButton *btn_Device;
    QToolButton *btn_System;
    QToolButton *btn_Calibration;
    QSpacerItem *verticalSpacer;
    QFrame *frame_Right;
    QVBoxLayout *verticalLayout_3;
    QFrame *frame_StartStop;
    QHBoxLayout *horizontalLayout;
    QPushButton *btn_Start;
    QPushButton *btn_EStop;
    QSpacerItem *horizontalSpacer_2;
    QFrame *frame_Device;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *btn_Camera1;
    QPushButton *btn_Camera2;
    QPushButton *btn_Robot1;
    QPushButton *btn_Robot2;
    QPushButton *btn_PLC;
    QSpacerItem *horizontalSpacer_3;
    QFrame *frame_Info;
    QVBoxLayout *verticalLayout_2;
    QLabel *label;
    QLabel *label_2;
    QFrame *frame_2;
    QHBoxLayout *horizontalLayout_2;
    QLabel *lbl_Title;
    QSpacerItem *horizontalSpacer;
    QLabel *lbl_Time;
    QSpacerItem *horizontalSpacer_4;
    QToolButton *btn_LoginUser;
    QMenuBar *menuBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1679, 1014);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        QPalette palette;
        QBrush brush(QColor(255, 255, 255, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Active, QPalette::Text, brush);
        QBrush brush1(QColor(0, 0, 0, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::ButtonText, brush1);
        palette.setBrush(QPalette::Active, QPalette::Base, brush);
        QBrush brush2(QColor(48, 47, 47, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Window, brush2);
        palette.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        QBrush brush3(QColor(255, 255, 255, 128));
        brush3.setStyle(Qt::SolidPattern);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette.setBrush(QPalette::Active, QPalette::PlaceholderText, brush3);
#endif
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette.setBrush(QPalette::Inactive, QPalette::ButtonText, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush2);
        palette.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette.setBrush(QPalette::Inactive, QPalette::PlaceholderText, brush3);
#endif
        QBrush brush4(QColor(190, 190, 190, 255));
        brush4.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush4);
        palette.setBrush(QPalette::Disabled, QPalette::Text, brush4);
        palette.setBrush(QPalette::Disabled, QPalette::ButtonText, brush4);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        QBrush brush5(QColor(0, 0, 0, 128));
        brush5.setStyle(Qt::SolidPattern);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette.setBrush(QPalette::Disabled, QPalette::PlaceholderText, brush5);
#endif
        MainWindow->setPalette(palette);
        actQuit = new QAction(MainWindow);
        actQuit->setObjectName(QString::fromUtf8("actQuit"));
        actChangeUser = new QAction(MainWindow);
        actChangeUser->setObjectName(QString::fromUtf8("actChangeUser"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        frame_Nav = new QFrame(centralwidget);
        frame_Nav->setObjectName(QString::fromUtf8("frame_Nav"));
        frame_Nav->setStyleSheet(QString::fromUtf8("QToolButton{border:2px groove gray;border-radius:10px;padding:2px 4px;border-style: outset;background-color:#404040;border-color:#404040}\n"
"QToolButton:hover{background-color:rgb(211, 211, 211); color: black;}\n"
"QToolButton:pressed{background-color:#404040;border-style: inset;}"));
        frame_Nav->setFrameShape(QFrame::Panel);
        frame_Nav->setFrameShadow(QFrame::Raised);
        verticalLayout = new QVBoxLayout(frame_Nav);
        verticalLayout->setSpacing(20);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        btn_User = new QToolButton(frame_Nav);
        btn_User->setObjectName(QString::fromUtf8("btn_User"));
        btn_User->setMaximumSize(QSize(150, 130));
        QPalette palette1;
        QBrush brush6(QColor(64, 64, 64, 255));
        brush6.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Button, brush6);
        palette1.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette1.setBrush(QPalette::Active, QPalette::Base, brush6);
        palette1.setBrush(QPalette::Active, QPalette::Window, brush6);
        palette1.setBrush(QPalette::Inactive, QPalette::Button, brush6);
        palette1.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::Base, brush6);
        palette1.setBrush(QPalette::Inactive, QPalette::Window, brush6);
        palette1.setBrush(QPalette::Disabled, QPalette::Button, brush6);
        palette1.setBrush(QPalette::Disabled, QPalette::ButtonText, brush4);
        palette1.setBrush(QPalette::Disabled, QPalette::Base, brush6);
        palette1.setBrush(QPalette::Disabled, QPalette::Window, brush6);
        btn_User->setPalette(palette1);
        QFont font;
        font.setPointSize(20);
        font.setBold(true);
        font.setWeight(75);
        btn_User->setFont(font);
        btn_User->setMouseTracking(false);
        btn_User->setStyleSheet(QString::fromUtf8("border:2px groove gray;border-radius:10px;padding:2px 4px;border-style: outset;background-color:#404040;border-color:#404040"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/Images/user_manager.png"), QSize(), QIcon::Normal, QIcon::Off);
        btn_User->setIcon(icon);
        btn_User->setIconSize(QSize(100, 100));
        btn_User->setAutoRepeat(false);
        btn_User->setAutoExclusive(false);
        btn_User->setPopupMode(QToolButton::DelayedPopup);
        btn_User->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        btn_User->setAutoRaise(false);
        btn_User->setArrowType(Qt::NoArrow);

        verticalLayout->addWidget(btn_User);

        btn_Product = new QToolButton(frame_Nav);
        btn_Product->setObjectName(QString::fromUtf8("btn_Product"));
        btn_Product->setMaximumSize(QSize(130, 150));
        QPalette palette2;
        palette2.setBrush(QPalette::Active, QPalette::Button, brush6);
        palette2.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette2.setBrush(QPalette::Active, QPalette::Base, brush6);
        palette2.setBrush(QPalette::Active, QPalette::Window, brush6);
        palette2.setBrush(QPalette::Inactive, QPalette::Button, brush6);
        palette2.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette2.setBrush(QPalette::Inactive, QPalette::Base, brush6);
        palette2.setBrush(QPalette::Inactive, QPalette::Window, brush6);
        palette2.setBrush(QPalette::Disabled, QPalette::Button, brush6);
        palette2.setBrush(QPalette::Disabled, QPalette::ButtonText, brush4);
        palette2.setBrush(QPalette::Disabled, QPalette::Base, brush6);
        palette2.setBrush(QPalette::Disabled, QPalette::Window, brush6);
        btn_Product->setPalette(palette2);
        btn_Product->setFont(font);
        btn_Product->setMouseTracking(false);
        btn_Product->setStyleSheet(QString::fromUtf8("border:2px groove gray;border-radius:10px;padding:2px 4px;border-style: outset;background-color:#404040;border-color:#404040"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/Images/product.png"), QSize(), QIcon::Normal, QIcon::Off);
        btn_Product->setIcon(icon1);
        btn_Product->setIconSize(QSize(100, 100));
        btn_Product->setAutoRepeat(false);
        btn_Product->setAutoExclusive(false);
        btn_Product->setPopupMode(QToolButton::DelayedPopup);
        btn_Product->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        btn_Product->setAutoRaise(false);
        btn_Product->setArrowType(Qt::NoArrow);

        verticalLayout->addWidget(btn_Product);

        btn_Device = new QToolButton(frame_Nav);
        btn_Device->setObjectName(QString::fromUtf8("btn_Device"));
        btn_Device->setMaximumSize(QSize(130, 150));
        QPalette palette3;
        palette3.setBrush(QPalette::Active, QPalette::Button, brush6);
        palette3.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette3.setBrush(QPalette::Active, QPalette::Base, brush6);
        palette3.setBrush(QPalette::Active, QPalette::Window, brush6);
        palette3.setBrush(QPalette::Inactive, QPalette::Button, brush6);
        palette3.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette3.setBrush(QPalette::Inactive, QPalette::Base, brush6);
        palette3.setBrush(QPalette::Inactive, QPalette::Window, brush6);
        palette3.setBrush(QPalette::Disabled, QPalette::Button, brush6);
        palette3.setBrush(QPalette::Disabled, QPalette::ButtonText, brush4);
        palette3.setBrush(QPalette::Disabled, QPalette::Base, brush6);
        palette3.setBrush(QPalette::Disabled, QPalette::Window, brush6);
        btn_Device->setPalette(palette3);
        btn_Device->setFont(font);
        btn_Device->setMouseTracking(false);
        btn_Device->setStyleSheet(QString::fromUtf8("border:2px groove gray;border-radius:10px;padding:2px 4px;border-style: outset;background-color:#404040;border-color:#404040"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/Images/device.png"), QSize(), QIcon::Normal, QIcon::Off);
        btn_Device->setIcon(icon2);
        btn_Device->setIconSize(QSize(100, 100));
        btn_Device->setAutoRepeat(false);
        btn_Device->setAutoExclusive(false);
        btn_Device->setPopupMode(QToolButton::DelayedPopup);
        btn_Device->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        btn_Device->setAutoRaise(false);
        btn_Device->setArrowType(Qt::NoArrow);

        verticalLayout->addWidget(btn_Device);

        btn_System = new QToolButton(frame_Nav);
        btn_System->setObjectName(QString::fromUtf8("btn_System"));
        btn_System->setMaximumSize(QSize(130, 150));
        QPalette palette4;
        palette4.setBrush(QPalette::Active, QPalette::Button, brush6);
        palette4.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette4.setBrush(QPalette::Active, QPalette::Base, brush6);
        palette4.setBrush(QPalette::Active, QPalette::Window, brush6);
        palette4.setBrush(QPalette::Inactive, QPalette::Button, brush6);
        palette4.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette4.setBrush(QPalette::Inactive, QPalette::Base, brush6);
        palette4.setBrush(QPalette::Inactive, QPalette::Window, brush6);
        palette4.setBrush(QPalette::Disabled, QPalette::Button, brush6);
        palette4.setBrush(QPalette::Disabled, QPalette::ButtonText, brush4);
        palette4.setBrush(QPalette::Disabled, QPalette::Base, brush6);
        palette4.setBrush(QPalette::Disabled, QPalette::Window, brush6);
        btn_System->setPalette(palette4);
        btn_System->setFont(font);
        btn_System->setMouseTracking(false);
        btn_System->setStyleSheet(QString::fromUtf8("border:2px groove gray;border-radius:10px;padding:2px 4px;border-style: outset;background-color:#404040;border-color:#404040"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/Images/system.png"), QSize(), QIcon::Normal, QIcon::Off);
        btn_System->setIcon(icon3);
        btn_System->setIconSize(QSize(100, 100));
        btn_System->setAutoRepeat(false);
        btn_System->setAutoExclusive(false);
        btn_System->setPopupMode(QToolButton::DelayedPopup);
        btn_System->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        btn_System->setAutoRaise(false);
        btn_System->setArrowType(Qt::NoArrow);

        verticalLayout->addWidget(btn_System);

        btn_Calibration = new QToolButton(frame_Nav);
        btn_Calibration->setObjectName(QString::fromUtf8("btn_Calibration"));
        btn_Calibration->setMaximumSize(QSize(130, 150));
        QPalette palette5;
        palette5.setBrush(QPalette::Active, QPalette::Button, brush6);
        palette5.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette5.setBrush(QPalette::Active, QPalette::Base, brush6);
        palette5.setBrush(QPalette::Active, QPalette::Window, brush6);
        palette5.setBrush(QPalette::Inactive, QPalette::Button, brush6);
        palette5.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette5.setBrush(QPalette::Inactive, QPalette::Base, brush6);
        palette5.setBrush(QPalette::Inactive, QPalette::Window, brush6);
        palette5.setBrush(QPalette::Disabled, QPalette::Button, brush6);
        palette5.setBrush(QPalette::Disabled, QPalette::ButtonText, brush4);
        palette5.setBrush(QPalette::Disabled, QPalette::Base, brush6);
        palette5.setBrush(QPalette::Disabled, QPalette::Window, brush6);
        btn_Calibration->setPalette(palette5);
        btn_Calibration->setFont(font);
        btn_Calibration->setMouseTracking(false);
        btn_Calibration->setStyleSheet(QString::fromUtf8("border:2px groove gray;border-radius:10px;padding:2px 4px;border-style: outset;background-color:#404040;border-color:#404040"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/Images/calibration.png"), QSize(), QIcon::Normal, QIcon::Off);
        btn_Calibration->setIcon(icon4);
        btn_Calibration->setIconSize(QSize(100, 100));
        btn_Calibration->setAutoRepeat(false);
        btn_Calibration->setAutoExclusive(false);
        btn_Calibration->setPopupMode(QToolButton::DelayedPopup);
        btn_Calibration->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        btn_Calibration->setAutoRaise(false);
        btn_Calibration->setArrowType(Qt::NoArrow);

        verticalLayout->addWidget(btn_Calibration);

        verticalSpacer = new QSpacerItem(20, 44, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        gridLayout->addWidget(frame_Nav, 1, 0, 1, 1);

        frame_Right = new QFrame(centralwidget);
        frame_Right->setObjectName(QString::fromUtf8("frame_Right"));
        frame_Right->setFrameShape(QFrame::StyledPanel);
        frame_Right->setFrameShadow(QFrame::Raised);
        verticalLayout_3 = new QVBoxLayout(frame_Right);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(-1, -1, -1, 0);
        frame_StartStop = new QFrame(frame_Right);
        frame_StartStop->setObjectName(QString::fromUtf8("frame_StartStop"));
        frame_StartStop->setStyleSheet(QString::fromUtf8(""));
        frame_StartStop->setFrameShape(QFrame::StyledPanel);
        frame_StartStop->setFrameShadow(QFrame::Raised);
        horizontalLayout = new QHBoxLayout(frame_StartStop);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        btn_Start = new QPushButton(frame_StartStop);
        btn_Start->setObjectName(QString::fromUtf8("btn_Start"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(btn_Start->sizePolicy().hasHeightForWidth());
        btn_Start->setSizePolicy(sizePolicy1);
        btn_Start->setMinimumSize(QSize(100, 100));
        btn_Start->setMaximumSize(QSize(100, 100));
        btn_Start->setStyleSheet(QString::fromUtf8(""));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/Images/start.png"), QSize(), QIcon::Normal, QIcon::Off);
        btn_Start->setIcon(icon5);
        btn_Start->setIconSize(QSize(300, 300));
        btn_Start->setFlat(true);

        horizontalLayout->addWidget(btn_Start);

        btn_EStop = new QPushButton(frame_StartStop);
        btn_EStop->setObjectName(QString::fromUtf8("btn_EStop"));
        sizePolicy1.setHeightForWidth(btn_EStop->sizePolicy().hasHeightForWidth());
        btn_EStop->setSizePolicy(sizePolicy1);
        btn_EStop->setMinimumSize(QSize(100, 100));
        btn_EStop->setMaximumSize(QSize(100, 100));
        btn_EStop->setStyleSheet(QString::fromUtf8(""));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/Images/e-Stop.png"), QSize(), QIcon::Normal, QIcon::Off);
        btn_EStop->setIcon(icon6);
        btn_EStop->setIconSize(QSize(300, 300));
        btn_EStop->setFlat(true);

        horizontalLayout->addWidget(btn_EStop);

        horizontalSpacer_2 = new QSpacerItem(1239, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);


        verticalLayout_3->addWidget(frame_StartStop);

        frame_Device = new QFrame(frame_Right);
        frame_Device->setObjectName(QString::fromUtf8("frame_Device"));
        QPalette palette6;
        palette6.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette6.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette6.setBrush(QPalette::Disabled, QPalette::ButtonText, brush4);
        frame_Device->setPalette(palette6);
        QFont font1;
        font1.setBold(true);
        font1.setWeight(75);
        frame_Device->setFont(font1);
        frame_Device->setStyleSheet(QString::fromUtf8(""));
        frame_Device->setFrameShape(QFrame::StyledPanel);
        frame_Device->setFrameShadow(QFrame::Raised);
        horizontalLayout_3 = new QHBoxLayout(frame_Device);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        btn_Camera1 = new QPushButton(frame_Device);
        btn_Camera1->setObjectName(QString::fromUtf8("btn_Camera1"));
        btn_Camera1->setMaximumSize(QSize(150, 100));
        QFont font2;
        font2.setPointSize(25);
        font2.setBold(true);
        font2.setWeight(75);
        btn_Camera1->setFont(font2);
        btn_Camera1->setStyleSheet(QString::fromUtf8(""));
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/Images/warning.png"), QSize(), QIcon::Normal, QIcon::Off);
        btn_Camera1->setIcon(icon7);
        btn_Camera1->setIconSize(QSize(150, 150));
        btn_Camera1->setAutoExclusive(false);
        btn_Camera1->setAutoRepeatDelay(200);
        btn_Camera1->setFlat(true);

        horizontalLayout_3->addWidget(btn_Camera1);

        btn_Camera2 = new QPushButton(frame_Device);
        btn_Camera2->setObjectName(QString::fromUtf8("btn_Camera2"));
        btn_Camera2->setMaximumSize(QSize(150, 100));
        btn_Camera2->setFont(font2);
        btn_Camera2->setStyleSheet(QString::fromUtf8(""));
        btn_Camera2->setIcon(icon7);
        btn_Camera2->setIconSize(QSize(150, 150));
        btn_Camera2->setAutoExclusive(false);
        btn_Camera2->setAutoRepeatDelay(200);
        btn_Camera2->setFlat(true);

        horizontalLayout_3->addWidget(btn_Camera2);

        btn_Robot1 = new QPushButton(frame_Device);
        btn_Robot1->setObjectName(QString::fromUtf8("btn_Robot1"));
        btn_Robot1->setMaximumSize(QSize(170, 100));
        btn_Robot1->setFont(font2);
        btn_Robot1->setStyleSheet(QString::fromUtf8(""));
        btn_Robot1->setIcon(icon7);
        btn_Robot1->setIconSize(QSize(150, 150));
        btn_Robot1->setAutoExclusive(false);
        btn_Robot1->setAutoRepeatDelay(200);
        btn_Robot1->setFlat(true);

        horizontalLayout_3->addWidget(btn_Robot1);

        btn_Robot2 = new QPushButton(frame_Device);
        btn_Robot2->setObjectName(QString::fromUtf8("btn_Robot2"));
        btn_Robot2->setMaximumSize(QSize(170, 100));
        btn_Robot2->setFont(font2);
        btn_Robot2->setStyleSheet(QString::fromUtf8(""));
        btn_Robot2->setIcon(icon7);
        btn_Robot2->setIconSize(QSize(150, 150));
        btn_Robot2->setAutoExclusive(false);
        btn_Robot2->setAutoRepeatDelay(200);
        btn_Robot2->setFlat(true);

        horizontalLayout_3->addWidget(btn_Robot2);

        btn_PLC = new QPushButton(frame_Device);
        btn_PLC->setObjectName(QString::fromUtf8("btn_PLC"));
        btn_PLC->setMaximumSize(QSize(150, 100));
        btn_PLC->setFont(font2);
        btn_PLC->setStyleSheet(QString::fromUtf8(""));
        btn_PLC->setIcon(icon7);
        btn_PLC->setIconSize(QSize(150, 150));
        btn_PLC->setAutoExclusive(false);
        btn_PLC->setAutoRepeatDelay(200);
        btn_PLC->setFlat(true);

        horizontalLayout_3->addWidget(btn_PLC);

        horizontalSpacer_3 = new QSpacerItem(983, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_3);


        verticalLayout_3->addWidget(frame_Device);

        frame_Info = new QFrame(frame_Right);
        frame_Info->setObjectName(QString::fromUtf8("frame_Info"));
        frame_Info->setFrameShape(QFrame::Panel);
        frame_Info->setFrameShadow(QFrame::Raised);
        frame_Info->setLineWidth(1);
        verticalLayout_2 = new QVBoxLayout(frame_Info);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        label = new QLabel(frame_Info);
        label->setObjectName(QString::fromUtf8("label"));
        QFont font3;
        font3.setPointSize(30);
        font3.setBold(true);
        font3.setWeight(75);
        label->setFont(font3);

        verticalLayout_2->addWidget(label);

        label_2 = new QLabel(frame_Info);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        QFont font4;
        font4.setPointSize(12);
        label_2->setFont(font4);
        label_2->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);

        verticalLayout_2->addWidget(label_2);

        verticalLayout_2->setStretch(0, 1);
        verticalLayout_2->setStretch(1, 10);

        verticalLayout_3->addWidget(frame_Info);

        verticalLayout_3->setStretch(0, 1);
        verticalLayout_3->setStretch(1, 1);
        verticalLayout_3->setStretch(2, 10);

        gridLayout->addWidget(frame_Right, 1, 1, 1, 1);

        frame_2 = new QFrame(centralwidget);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        horizontalLayout_2 = new QHBoxLayout(frame_2);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        lbl_Title = new QLabel(frame_2);
        lbl_Title->setObjectName(QString::fromUtf8("lbl_Title"));
        lbl_Title->setMaximumSize(QSize(500, 300));
        QFont font5;
        font5.setPointSize(50);
        font5.setBold(true);
        font5.setWeight(75);
        lbl_Title->setFont(font5);

        horizontalLayout_2->addWidget(lbl_Title);

        horizontalSpacer = new QSpacerItem(735, 17, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        lbl_Time = new QLabel(frame_2);
        lbl_Time->setObjectName(QString::fromUtf8("lbl_Time"));
        lbl_Time->setMaximumSize(QSize(180, 16777215));
        QFont font6;
        font6.setPointSize(30);
        lbl_Time->setFont(font6);

        horizontalLayout_2->addWidget(lbl_Time);

        horizontalSpacer_4 = new QSpacerItem(30, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_4);

        btn_LoginUser = new QToolButton(frame_2);
        btn_LoginUser->setObjectName(QString::fromUtf8("btn_LoginUser"));
        sizePolicy.setHeightForWidth(btn_LoginUser->sizePolicy().hasHeightForWidth());
        btn_LoginUser->setSizePolicy(sizePolicy);
        btn_LoginUser->setMaximumSize(QSize(150, 16777215));
        QPalette palette7;
        palette7.setBrush(QPalette::Active, QPalette::Button, brush2);
        palette7.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette7.setBrush(QPalette::Active, QPalette::Base, brush4);
        palette7.setBrush(QPalette::Inactive, QPalette::Button, brush2);
        palette7.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette7.setBrush(QPalette::Inactive, QPalette::Base, brush4);
        palette7.setBrush(QPalette::Disabled, QPalette::Button, brush2);
        palette7.setBrush(QPalette::Disabled, QPalette::ButtonText, brush4);
        palette7.setBrush(QPalette::Disabled, QPalette::Base, brush2);
        btn_LoginUser->setPalette(palette7);
        btn_LoginUser->setFont(font2);
        btn_LoginUser->setStyleSheet(QString::fromUtf8(":hover{background-color:rgb(211, 211, 211); color: black;border-radius:10px}"));
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/Images/user.png"), QSize(), QIcon::Normal, QIcon::Off);
        btn_LoginUser->setIcon(icon8);
        btn_LoginUser->setIconSize(QSize(80, 80));
        btn_LoginUser->setPopupMode(QToolButton::InstantPopup);
        btn_LoginUser->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

        horizontalLayout_2->addWidget(btn_LoginUser);


        gridLayout->addWidget(frame_2, 0, 0, 1, 3);

        MainWindow->setCentralWidget(centralwidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1679, 22));
        MainWindow->setMenuBar(menuBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "\344\270\273\351\241\265\351\235\242", nullptr));
        actQuit->setText(QApplication::translate("MainWindow", "\351\200\200\345\207\272", nullptr));
#ifndef QT_NO_TOOLTIP
        actQuit->setToolTip(QApplication::translate("MainWindow", "\351\200\200\345\207\272", nullptr));
#endif // QT_NO_TOOLTIP
        actChangeUser->setText(QApplication::translate("MainWindow", "\345\210\207\346\215\242\350\264\246\345\217\267", nullptr));
#ifndef QT_NO_TOOLTIP
        actChangeUser->setToolTip(QApplication::translate("MainWindow", "\345\210\207\346\215\242\350\264\246\345\217\267", nullptr));
#endif // QT_NO_TOOLTIP
        btn_User->setText(QApplication::translate("MainWindow", "\347\224\250\346\210\267\344\277\241\346\201\257", nullptr));
        btn_Product->setText(QApplication::translate("MainWindow", "\344\272\247\345\223\201\344\277\241\346\201\257", nullptr));
        btn_Device->setText(QApplication::translate("MainWindow", "\350\256\276\345\244\207\347\273\264\346\212\244", nullptr));
        btn_System->setText(QApplication::translate("MainWindow", "\347\263\273\347\273\237\344\277\241\346\201\257", nullptr));
        btn_Calibration->setText(QApplication::translate("MainWindow", "\350\256\276\345\244\207\346\240\207\345\256\232", nullptr));
        btn_Start->setText(QString());
        btn_EStop->setText(QString());
        btn_Camera1->setText(QApplication::translate("MainWindow", "\347\233\270\346\234\2721", nullptr));
        btn_Camera2->setText(QApplication::translate("MainWindow", "\347\233\270\346\234\2722", nullptr));
        btn_Robot1->setText(QApplication::translate("MainWindow", "\346\234\272\345\231\250\344\272\2721", nullptr));
        btn_Robot2->setText(QApplication::translate("MainWindow", "\346\234\272\345\231\250\344\272\2722", nullptr));
        btn_PLC->setText(QApplication::translate("MainWindow", "PLC", nullptr));
        label->setText(QApplication::translate("MainWindow", "\350\277\220\350\241\214\344\277\241\346\201\257", nullptr));
        label_2->setText(QApplication::translate("MainWindow", "<html><head/><body><p>10\357\274\23210\357\274\23210</p><p>\350\257\206\345\210\253\344\272\247\345\223\201</p><p>\344\272\247\345\223\201\345\236\213\345\217\267\357\274\23201</p><p>\345\260\272\345\257\270\357\274\232 10*10*10</p><p>10\357\274\23212\357\274\23211</p><p>\345\274\200\345\247\213\345\226\267\346\266\202</p></body></html>", nullptr));
        lbl_Title->setText(QApplication::translate("MainWindow", "\350\207\252\345\212\250\345\226\267\346\266\202\347\263\273\347\273\237", nullptr));
        lbl_Time->setText(QApplication::translate("MainWindow", "10:51:51", nullptr));
        btn_LoginUser->setText(QApplication::translate("MainWindow", "\347\256\241\347\220\206\345\221\230", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
