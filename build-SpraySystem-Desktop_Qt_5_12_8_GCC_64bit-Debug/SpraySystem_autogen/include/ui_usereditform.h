/********************************************************************************
** Form generated from reading UI file 'usereditform.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_USEREDITFORM_H
#define UI_USEREDITFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_UserEditForm
{
public:
    QWidget *centralwidget;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *btn_Cancel;
    QSpacerItem *horizontalSpacer;
    QPushButton *btn_OK;
    QWidget *layoutWidget1;
    QGridLayout *gridLayout_2;
    QLineEdit *edit_Pwd;
    QLabel *label;
    QLabel *label_4;
    QLineEdit *edit_Name;
    QLabel *label_2;
    QLabel *label_3;
    QGridLayout *gridLayout;
    QCheckBox *cb_User;
    QCheckBox *cb_Product;
    QCheckBox *cb_Device;
    QCheckBox *cb_Cali;
    QCheckBox *cb_Sys;
    QHBoxLayout *horizontalLayout;
    QRadioButton *rb_Operator;
    QRadioButton *rb_Tec;
    QRadioButton *rb_Admin;

    void setupUi(QMainWindow *UserEditForm)
    {
        if (UserEditForm->objectName().isEmpty())
            UserEditForm->setObjectName(QString::fromUtf8("UserEditForm"));
        UserEditForm->resize(800, 600);
        UserEditForm->setMinimumSize(QSize(800, 600));
        UserEditForm->setMaximumSize(QSize(800, 600));
        centralwidget = new QWidget(UserEditForm);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        layoutWidget = new QWidget(centralwidget);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(180, 500, 461, 62));
        horizontalLayout_2 = new QHBoxLayout(layoutWidget);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        btn_Cancel = new QPushButton(layoutWidget);
        btn_Cancel->setObjectName(QString::fromUtf8("btn_Cancel"));
        btn_Cancel->setMinimumSize(QSize(150, 60));
        QFont font;
        font.setPointSize(18);
        btn_Cancel->setFont(font);

        horizontalLayout_2->addWidget(btn_Cancel);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        btn_OK = new QPushButton(layoutWidget);
        btn_OK->setObjectName(QString::fromUtf8("btn_OK"));
        btn_OK->setMinimumSize(QSize(150, 60));
        btn_OK->setFont(font);

        horizontalLayout_2->addWidget(btn_OK);

        layoutWidget1 = new QWidget(centralwidget);
        layoutWidget1->setObjectName(QString::fromUtf8("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(90, 60, 631, 421));
        gridLayout_2 = new QGridLayout(layoutWidget1);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        edit_Pwd = new QLineEdit(layoutWidget1);
        edit_Pwd->setObjectName(QString::fromUtf8("edit_Pwd"));
        edit_Pwd->setMaximumSize(QSize(200, 45));
        edit_Pwd->setFont(font);

        gridLayout_2->addWidget(edit_Pwd, 1, 1, 1, 1);

        label = new QLabel(layoutWidget1);
        label->setObjectName(QString::fromUtf8("label"));
        label->setMaximumSize(QSize(100, 45));
        label->setFont(font);

        gridLayout_2->addWidget(label, 0, 0, 1, 1);

        label_4 = new QLabel(layoutWidget1);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setMaximumSize(QSize(100, 45));
        label_4->setFont(font);

        gridLayout_2->addWidget(label_4, 3, 0, 1, 1);

        edit_Name = new QLineEdit(layoutWidget1);
        edit_Name->setObjectName(QString::fromUtf8("edit_Name"));
        edit_Name->setMaximumSize(QSize(200, 45));
        edit_Name->setFont(font);

        gridLayout_2->addWidget(edit_Name, 0, 1, 1, 1);

        label_2 = new QLabel(layoutWidget1);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setMaximumSize(QSize(100, 45));
        label_2->setFont(font);

        gridLayout_2->addWidget(label_2, 1, 0, 1, 1);

        label_3 = new QLabel(layoutWidget1);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setMaximumSize(QSize(100, 45));
        label_3->setFont(font);

        gridLayout_2->addWidget(label_3, 2, 0, 1, 1);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        cb_User = new QCheckBox(layoutWidget1);
        cb_User->setObjectName(QString::fromUtf8("cb_User"));
        cb_User->setFont(font);

        gridLayout->addWidget(cb_User, 0, 0, 1, 1);

        cb_Product = new QCheckBox(layoutWidget1);
        cb_Product->setObjectName(QString::fromUtf8("cb_Product"));
        cb_Product->setFont(font);

        gridLayout->addWidget(cb_Product, 0, 1, 1, 1);

        cb_Device = new QCheckBox(layoutWidget1);
        cb_Device->setObjectName(QString::fromUtf8("cb_Device"));
        cb_Device->setFont(font);

        gridLayout->addWidget(cb_Device, 0, 2, 1, 1);

        cb_Cali = new QCheckBox(layoutWidget1);
        cb_Cali->setObjectName(QString::fromUtf8("cb_Cali"));
        cb_Cali->setFont(font);

        gridLayout->addWidget(cb_Cali, 1, 0, 1, 1);

        cb_Sys = new QCheckBox(layoutWidget1);
        cb_Sys->setObjectName(QString::fromUtf8("cb_Sys"));
        cb_Sys->setFont(font);

        gridLayout->addWidget(cb_Sys, 1, 1, 1, 1);


        gridLayout_2->addLayout(gridLayout, 3, 1, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        rb_Operator = new QRadioButton(layoutWidget1);
        rb_Operator->setObjectName(QString::fromUtf8("rb_Operator"));
        rb_Operator->setFont(font);
        rb_Operator->setChecked(true);

        horizontalLayout->addWidget(rb_Operator);

        rb_Tec = new QRadioButton(layoutWidget1);
        rb_Tec->setObjectName(QString::fromUtf8("rb_Tec"));
        rb_Tec->setFont(font);

        horizontalLayout->addWidget(rb_Tec);

        rb_Admin = new QRadioButton(layoutWidget1);
        rb_Admin->setObjectName(QString::fromUtf8("rb_Admin"));
        rb_Admin->setFont(font);

        horizontalLayout->addWidget(rb_Admin);


        gridLayout_2->addLayout(horizontalLayout, 2, 1, 1, 1);

        UserEditForm->setCentralWidget(centralwidget);
        QWidget::setTabOrder(edit_Name, edit_Pwd);
        QWidget::setTabOrder(edit_Pwd, rb_Operator);
        QWidget::setTabOrder(rb_Operator, rb_Tec);
        QWidget::setTabOrder(rb_Tec, rb_Admin);
        QWidget::setTabOrder(rb_Admin, cb_User);
        QWidget::setTabOrder(cb_User, cb_Product);
        QWidget::setTabOrder(cb_Product, cb_Device);
        QWidget::setTabOrder(cb_Device, cb_Cali);
        QWidget::setTabOrder(cb_Cali, cb_Sys);
        QWidget::setTabOrder(cb_Sys, btn_Cancel);
        QWidget::setTabOrder(btn_Cancel, btn_OK);

        retranslateUi(UserEditForm);
        QObject::connect(btn_Cancel, SIGNAL(clicked()), UserEditForm, SLOT(close()));

        QMetaObject::connectSlotsByName(UserEditForm);
    } // setupUi

    void retranslateUi(QMainWindow *UserEditForm)
    {
        UserEditForm->setWindowTitle(QApplication::translate("UserEditForm", "\347\224\250\346\210\267\344\277\241\346\201\257", nullptr));
        btn_Cancel->setText(QApplication::translate("UserEditForm", "\345\217\226\346\266\210", nullptr));
        btn_OK->setText(QApplication::translate("UserEditForm", "\347\241\256\345\256\232", nullptr));
        label->setText(QApplication::translate("UserEditForm", "\347\224\250\346\210\267\345\220\215\357\274\232", nullptr));
        label_4->setText(QApplication::translate("UserEditForm", "\346\235\203   \351\231\220\357\274\232", nullptr));
        label_2->setText(QApplication::translate("UserEditForm", "\345\257\206    \347\240\201\357\274\232", nullptr));
        label_3->setText(QApplication::translate("UserEditForm", "\350\247\222    \350\211\262\357\274\232", nullptr));
        cb_User->setText(QApplication::translate("UserEditForm", "\347\224\250\346\210\267\344\277\241\346\201\257", nullptr));
        cb_Product->setText(QApplication::translate("UserEditForm", "\344\272\247\345\223\201\344\277\241\346\201\257", nullptr));
        cb_Device->setText(QApplication::translate("UserEditForm", "\350\256\276\345\244\207\347\273\264\346\212\244", nullptr));
        cb_Cali->setText(QApplication::translate("UserEditForm", "\350\256\276\345\244\207\346\240\207\345\256\232", nullptr));
        cb_Sys->setText(QApplication::translate("UserEditForm", "\347\263\273\347\273\237\344\277\241\346\201\257", nullptr));
        rb_Operator->setText(QApplication::translate("UserEditForm", "\346\223\215\344\275\234\345\221\230", nullptr));
        rb_Tec->setText(QApplication::translate("UserEditForm", "\346\212\200\346\234\257\345\221\230", nullptr));
        rb_Admin->setText(QApplication::translate("UserEditForm", "\347\256\241\347\220\206\345\221\230", nullptr));
    } // retranslateUi

};

namespace Ui {
    class UserEditForm: public Ui_UserEditForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_USEREDITFORM_H
