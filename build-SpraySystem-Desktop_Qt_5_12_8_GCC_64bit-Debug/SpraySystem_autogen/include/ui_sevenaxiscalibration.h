/********************************************************************************
** Form generated from reading UI file 'sevenaxiscalibration.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SEVENAXISCALIBRATION_H
#define UI_SEVENAXISCALIBRATION_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SevenAxisCalibration
{
public:
    QWidget *centralwidget;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_2;
    QGridLayout *gridLayout;
    QLineEdit *lineEdit_4;
    QSpacerItem *horizontalSpacer;
    QPushButton *btn_Point1;
    QSpacerItem *horizontalSpacer_2;
    QHBoxLayout *horizontalLayout;
    QLineEdit *lineEdit;
    QLineEdit *lineEdit_2;
    QLineEdit *lineEdit_3;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_5;
    QGridLayout *gridLayout_6;
    QLineEdit *lineEdit_9;
    QSpacerItem *horizontalSpacer_5;
    QPushButton *btn_Point1_3;
    QSpacerItem *horizontalSpacer_6;
    QHBoxLayout *horizontalLayout_3;
    QLineEdit *lineEdit_10;
    QLineEdit *lineEdit_11;
    QLineEdit *lineEdit_12;
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout_7;
    QGridLayout *gridLayout_8;
    QLineEdit *lineEdit_13;
    QSpacerItem *horizontalSpacer_7;
    QPushButton *btn_Point1_4;
    QSpacerItem *horizontalSpacer_8;
    QHBoxLayout *horizontalLayout_4;
    QLineEdit *lineEdit_14;
    QLineEdit *lineEdit_15;
    QLineEdit *lineEdit_16;
    QTextEdit *textEdit;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *SevenAxisCalibration)
    {
        if (SevenAxisCalibration->objectName().isEmpty())
            SevenAxisCalibration->setObjectName(QString::fromUtf8("SevenAxisCalibration"));
        SevenAxisCalibration->resize(937, 648);
        centralwidget = new QWidget(SevenAxisCalibration);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        groupBox = new QGroupBox(centralwidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(40, 50, 450, 160));
        groupBox->setMinimumSize(QSize(450, 160));
        groupBox->setMaximumSize(QSize(450, 160));
        gridLayout_2 = new QGridLayout(groupBox);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        lineEdit_4 = new QLineEdit(groupBox);
        lineEdit_4->setObjectName(QString::fromUtf8("lineEdit_4"));
        lineEdit_4->setMaximumSize(QSize(200, 16777215));

        gridLayout->addWidget(lineEdit_4, 1, 0, 1, 1);

        horizontalSpacer = new QSpacerItem(188, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 1, 1, 1, 2);

        btn_Point1 = new QPushButton(groupBox);
        btn_Point1->setObjectName(QString::fromUtf8("btn_Point1"));

        gridLayout->addWidget(btn_Point1, 2, 2, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(188, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 2, 0, 1, 2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        lineEdit = new QLineEdit(groupBox);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        horizontalLayout->addWidget(lineEdit);

        lineEdit_2 = new QLineEdit(groupBox);
        lineEdit_2->setObjectName(QString::fromUtf8("lineEdit_2"));

        horizontalLayout->addWidget(lineEdit_2);

        lineEdit_3 = new QLineEdit(groupBox);
        lineEdit_3->setObjectName(QString::fromUtf8("lineEdit_3"));

        horizontalLayout->addWidget(lineEdit_3);


        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 3);


        gridLayout_2->addLayout(gridLayout, 0, 0, 1, 1);

        groupBox_2 = new QGroupBox(centralwidget);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(40, 210, 450, 160));
        groupBox_2->setMinimumSize(QSize(450, 160));
        groupBox_2->setMaximumSize(QSize(450, 160));
        gridLayout_5 = new QGridLayout(groupBox_2);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        gridLayout_6 = new QGridLayout();
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        lineEdit_9 = new QLineEdit(groupBox_2);
        lineEdit_9->setObjectName(QString::fromUtf8("lineEdit_9"));
        lineEdit_9->setMaximumSize(QSize(200, 16777215));

        gridLayout_6->addWidget(lineEdit_9, 1, 0, 1, 1);

        horizontalSpacer_5 = new QSpacerItem(188, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_6->addItem(horizontalSpacer_5, 1, 1, 1, 2);

        btn_Point1_3 = new QPushButton(groupBox_2);
        btn_Point1_3->setObjectName(QString::fromUtf8("btn_Point1_3"));

        gridLayout_6->addWidget(btn_Point1_3, 2, 2, 1, 1);

        horizontalSpacer_6 = new QSpacerItem(188, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_6->addItem(horizontalSpacer_6, 2, 0, 1, 2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        lineEdit_10 = new QLineEdit(groupBox_2);
        lineEdit_10->setObjectName(QString::fromUtf8("lineEdit_10"));

        horizontalLayout_3->addWidget(lineEdit_10);

        lineEdit_11 = new QLineEdit(groupBox_2);
        lineEdit_11->setObjectName(QString::fromUtf8("lineEdit_11"));

        horizontalLayout_3->addWidget(lineEdit_11);

        lineEdit_12 = new QLineEdit(groupBox_2);
        lineEdit_12->setObjectName(QString::fromUtf8("lineEdit_12"));

        horizontalLayout_3->addWidget(lineEdit_12);


        gridLayout_6->addLayout(horizontalLayout_3, 0, 0, 1, 3);


        gridLayout_5->addLayout(gridLayout_6, 0, 0, 1, 1);

        groupBox_3 = new QGroupBox(centralwidget);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setGeometry(QRect(40, 380, 450, 160));
        groupBox_3->setMinimumSize(QSize(450, 160));
        groupBox_3->setMaximumSize(QSize(450, 160));
        gridLayout_7 = new QGridLayout(groupBox_3);
        gridLayout_7->setObjectName(QString::fromUtf8("gridLayout_7"));
        gridLayout_8 = new QGridLayout();
        gridLayout_8->setObjectName(QString::fromUtf8("gridLayout_8"));
        lineEdit_13 = new QLineEdit(groupBox_3);
        lineEdit_13->setObjectName(QString::fromUtf8("lineEdit_13"));
        lineEdit_13->setMaximumSize(QSize(200, 16777215));

        gridLayout_8->addWidget(lineEdit_13, 1, 0, 1, 1);

        horizontalSpacer_7 = new QSpacerItem(188, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_8->addItem(horizontalSpacer_7, 1, 1, 1, 2);

        btn_Point1_4 = new QPushButton(groupBox_3);
        btn_Point1_4->setObjectName(QString::fromUtf8("btn_Point1_4"));

        gridLayout_8->addWidget(btn_Point1_4, 2, 2, 1, 1);

        horizontalSpacer_8 = new QSpacerItem(188, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_8->addItem(horizontalSpacer_8, 2, 0, 1, 2);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        lineEdit_14 = new QLineEdit(groupBox_3);
        lineEdit_14->setObjectName(QString::fromUtf8("lineEdit_14"));

        horizontalLayout_4->addWidget(lineEdit_14);

        lineEdit_15 = new QLineEdit(groupBox_3);
        lineEdit_15->setObjectName(QString::fromUtf8("lineEdit_15"));

        horizontalLayout_4->addWidget(lineEdit_15);

        lineEdit_16 = new QLineEdit(groupBox_3);
        lineEdit_16->setObjectName(QString::fromUtf8("lineEdit_16"));

        horizontalLayout_4->addWidget(lineEdit_16);


        gridLayout_8->addLayout(horizontalLayout_4, 0, 0, 1, 3);


        gridLayout_7->addLayout(gridLayout_8, 0, 0, 1, 1);

        textEdit = new QTextEdit(centralwidget);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));
        textEdit->setGeometry(QRect(590, 90, 291, 211));
        SevenAxisCalibration->setCentralWidget(centralwidget);
        menubar = new QMenuBar(SevenAxisCalibration);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 937, 22));
        SevenAxisCalibration->setMenuBar(menubar);
        statusbar = new QStatusBar(SevenAxisCalibration);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        SevenAxisCalibration->setStatusBar(statusbar);

        retranslateUi(SevenAxisCalibration);

        QMetaObject::connectSlotsByName(SevenAxisCalibration);
    } // setupUi

    void retranslateUi(QMainWindow *SevenAxisCalibration)
    {
        SevenAxisCalibration->setWindowTitle(QApplication::translate("SevenAxisCalibration", "MainWindow", nullptr));
        groupBox->setTitle(QApplication::translate("SevenAxisCalibration", "\347\202\2711", nullptr));
        btn_Point1->setText(QApplication::translate("SevenAxisCalibration", "\350\216\267\345\217\226\345\235\220\346\240\207", nullptr));
        groupBox_2->setTitle(QApplication::translate("SevenAxisCalibration", "\347\202\2712", nullptr));
        btn_Point1_3->setText(QApplication::translate("SevenAxisCalibration", "\350\216\267\345\217\226\345\235\220\346\240\207", nullptr));
        groupBox_3->setTitle(QApplication::translate("SevenAxisCalibration", "\347\202\2713", nullptr));
        btn_Point1_4->setText(QApplication::translate("SevenAxisCalibration", "\350\216\267\345\217\226\345\235\220\346\240\207", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SevenAxisCalibration: public Ui_SevenAxisCalibration {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SEVENAXISCALIBRATION_H
