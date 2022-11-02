/********************************************************************************
** Form generated from reading UI file 'systemeditform.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SYSTEMEDITFORM_H
#define UI_SYSTEMEDITFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SystemEditForm
{
public:
    QWidget *centralwidget;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout_5;
    QPushButton *btn_Cancel;
    QSpacerItem *horizontalSpacer;
    QPushButton *btn_OK;
    QWidget *layoutWidget1;
    QGridLayout *gridLayout;
    QLabel *label;
    QHBoxLayout *horizontalLayout_4;
    QLineEdit *lineEdit;
    QLabel *label_9;
    QLabel *label_2;
    QHBoxLayout *horizontalLayout;
    QLineEdit *lineEdit_2;
    QLabel *label_6;
    QLabel *label_3;
    QHBoxLayout *horizontalLayout_2;
    QLineEdit *lineEdit_3;
    QLabel *label_7;
    QLabel *label_4;
    QHBoxLayout *horizontalLayout_3;
    QLineEdit *lineEdit_4;
    QLabel *label_8;

    void setupUi(QMainWindow *SystemEditForm)
    {
        if (SystemEditForm->objectName().isEmpty())
            SystemEditForm->setObjectName(QString::fromUtf8("SystemEditForm"));
        SystemEditForm->resize(800, 600);
        centralwidget = new QWidget(SystemEditForm);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        layoutWidget = new QWidget(centralwidget);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(210, 430, 421, 62));
        horizontalLayout_5 = new QHBoxLayout(layoutWidget);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        horizontalLayout_5->setContentsMargins(0, 0, 0, 0);
        btn_Cancel = new QPushButton(layoutWidget);
        btn_Cancel->setObjectName(QString::fromUtf8("btn_Cancel"));
        btn_Cancel->setMinimumSize(QSize(150, 60));
        QFont font;
        font.setPointSize(18);
        btn_Cancel->setFont(font);

        horizontalLayout_5->addWidget(btn_Cancel);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer);

        btn_OK = new QPushButton(layoutWidget);
        btn_OK->setObjectName(QString::fromUtf8("btn_OK"));
        btn_OK->setMinimumSize(QSize(150, 60));
        btn_OK->setFont(font);

        horizontalLayout_5->addWidget(btn_OK);

        layoutWidget1 = new QWidget(centralwidget);
        layoutWidget1->setObjectName(QString::fromUtf8("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(170, 120, 491, 261));
        gridLayout = new QGridLayout(layoutWidget1);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(layoutWidget1);
        label->setObjectName(QString::fromUtf8("label"));
        label->setMaximumSize(QSize(150, 45));
        label->setFont(font);

        gridLayout->addWidget(label, 0, 0, 1, 1);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        lineEdit = new QLineEdit(layoutWidget1);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setMaximumSize(QSize(80, 16777215));
        lineEdit->setFont(font);

        horizontalLayout_4->addWidget(lineEdit);

        label_9 = new QLabel(layoutWidget1);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setFont(font);

        horizontalLayout_4->addWidget(label_9);


        gridLayout->addLayout(horizontalLayout_4, 0, 1, 1, 1);

        label_2 = new QLabel(layoutWidget1);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setMaximumSize(QSize(150, 45));
        label_2->setFont(font);

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        lineEdit_2 = new QLineEdit(layoutWidget1);
        lineEdit_2->setObjectName(QString::fromUtf8("lineEdit_2"));
        lineEdit_2->setMaximumSize(QSize(80, 16777215));
        lineEdit_2->setFont(font);

        horizontalLayout->addWidget(lineEdit_2);

        label_6 = new QLabel(layoutWidget1);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setFont(font);

        horizontalLayout->addWidget(label_6);


        gridLayout->addLayout(horizontalLayout, 1, 1, 1, 1);

        label_3 = new QLabel(layoutWidget1);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setMaximumSize(QSize(150, 45));
        label_3->setFont(font);

        gridLayout->addWidget(label_3, 2, 0, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        lineEdit_3 = new QLineEdit(layoutWidget1);
        lineEdit_3->setObjectName(QString::fromUtf8("lineEdit_3"));
        lineEdit_3->setMaximumSize(QSize(80, 16777215));
        lineEdit_3->setFont(font);

        horizontalLayout_2->addWidget(lineEdit_3);

        label_7 = new QLabel(layoutWidget1);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setFont(font);

        horizontalLayout_2->addWidget(label_7);


        gridLayout->addLayout(horizontalLayout_2, 2, 1, 1, 1);

        label_4 = new QLabel(layoutWidget1);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setMaximumSize(QSize(150, 45));
        label_4->setFont(font);

        gridLayout->addWidget(label_4, 3, 0, 1, 1);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        lineEdit_4 = new QLineEdit(layoutWidget1);
        lineEdit_4->setObjectName(QString::fromUtf8("lineEdit_4"));
        lineEdit_4->setMaximumSize(QSize(80, 16777215));
        lineEdit_4->setFont(font);

        horizontalLayout_3->addWidget(lineEdit_4);

        label_8 = new QLabel(layoutWidget1);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setFont(font);

        horizontalLayout_3->addWidget(label_8);


        gridLayout->addLayout(horizontalLayout_3, 3, 1, 1, 1);

        SystemEditForm->setCentralWidget(centralwidget);
        QWidget::setTabOrder(lineEdit, lineEdit_2);
        QWidget::setTabOrder(lineEdit_2, lineEdit_3);
        QWidget::setTabOrder(lineEdit_3, lineEdit_4);
        QWidget::setTabOrder(lineEdit_4, btn_Cancel);
        QWidget::setTabOrder(btn_Cancel, btn_OK);

        retranslateUi(SystemEditForm);

        QMetaObject::connectSlotsByName(SystemEditForm);
    } // setupUi

    void retranslateUi(QMainWindow *SystemEditForm)
    {
        SystemEditForm->setWindowTitle(QApplication::translate("SystemEditForm", "\347\263\273\347\273\237\344\277\241\346\201\257", nullptr));
        btn_Cancel->setText(QApplication::translate("SystemEditForm", "\345\217\226\346\266\210", nullptr));
        btn_OK->setText(QApplication::translate("SystemEditForm", "\347\241\256\345\256\232", nullptr));
        label->setText(QApplication::translate("SystemEditForm", "\345\220\216\346\226\271\347\256\261\344\275\223\350\267\235\347\246\273", nullptr));
        label_9->setText(QApplication::translate("SystemEditForm", "/ m", nullptr));
        label_2->setText(QApplication::translate("SystemEditForm", "\345\226\267\346\266\202\351\200\237\345\272\246", nullptr));
        label_6->setText(QApplication::translate("SystemEditForm", "/ (m/s)", nullptr));
        label_3->setText(QApplication::translate("SystemEditForm", "\345\226\267\346\266\202\346\227\266\351\227\264", nullptr));
        label_7->setText(QApplication::translate("SystemEditForm", "/ s", nullptr));
        label_4->setText(QApplication::translate("SystemEditForm", "7\350\275\264\350\265\267\347\202\271", nullptr));
        label_8->setText(QApplication::translate("SystemEditForm", "/ m", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SystemEditForm: public Ui_SystemEditForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SYSTEMEDITFORM_H
