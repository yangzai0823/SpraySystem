/********************************************************************************
** Form generated from reading UI file 'userlistform.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_USERLISTFORM_H
#define UI_USERLISTFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QTableView>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_UserListForm
{
public:
    QAction *act_AddUser;
    QAction *act_edit;
    QAction *act_delete;
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout;
    QTableView *tableView;
    QToolBar *toolBar;

    void setupUi(QMainWindow *UserListForm)
    {
        if (UserListForm->objectName().isEmpty())
            UserListForm->setObjectName(QString::fromUtf8("UserListForm"));
        UserListForm->resize(800, 600);
        UserListForm->setMinimumSize(QSize(800, 600));
        UserListForm->setMaximumSize(QSize(800, 600));
        act_AddUser = new QAction(UserListForm);
        act_AddUser->setObjectName(QString::fromUtf8("act_AddUser"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/Images/add.png"), QSize(), QIcon::Normal, QIcon::Off);
        act_AddUser->setIcon(icon);
        act_edit = new QAction(UserListForm);
        act_edit->setObjectName(QString::fromUtf8("act_edit"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/Images/edit.png"), QSize(), QIcon::Normal, QIcon::Off);
        act_edit->setIcon(icon1);
        act_delete = new QAction(UserListForm);
        act_delete->setObjectName(QString::fromUtf8("act_delete"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/Images/delete.png"), QSize(), QIcon::Normal, QIcon::Off);
        act_delete->setIcon(icon2);
        centralwidget = new QWidget(UserListForm);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        horizontalLayout = new QHBoxLayout(centralwidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        tableView = new QTableView(centralwidget);
        tableView->setObjectName(QString::fromUtf8("tableView"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(tableView->sizePolicy().hasHeightForWidth());
        tableView->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(tableView);

        UserListForm->setCentralWidget(centralwidget);
        toolBar = new QToolBar(UserListForm);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        QFont font;
        font.setPointSize(18);
        toolBar->setFont(font);
        toolBar->setIconSize(QSize(30, 30));
        toolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        UserListForm->addToolBar(Qt::TopToolBarArea, toolBar);

        toolBar->addAction(act_AddUser);
        toolBar->addAction(act_delete);
        toolBar->addAction(act_edit);

        retranslateUi(UserListForm);

        QMetaObject::connectSlotsByName(UserListForm);
    } // setupUi

    void retranslateUi(QMainWindow *UserListForm)
    {
        UserListForm->setWindowTitle(QApplication::translate("UserListForm", "\347\224\250\346\210\267\345\210\227\350\241\250", nullptr));
        act_AddUser->setText(QApplication::translate("UserListForm", "\346\226\260\345\242\236\347\224\250\346\210\267", nullptr));
#ifndef QT_NO_TOOLTIP
        act_AddUser->setToolTip(QApplication::translate("UserListForm", "\346\226\260\345\242\236\347\224\250\346\210\267", nullptr));
#endif // QT_NO_TOOLTIP
        act_edit->setText(QApplication::translate("UserListForm", "\347\274\226\350\276\221\347\224\250\346\210\267", nullptr));
#ifndef QT_NO_TOOLTIP
        act_edit->setToolTip(QApplication::translate("UserListForm", "\347\274\226\350\276\221\347\224\250\346\210\267", nullptr));
#endif // QT_NO_TOOLTIP
        act_delete->setText(QApplication::translate("UserListForm", "\345\210\240\351\231\244\347\224\250\346\210\267", nullptr));
        toolBar->setWindowTitle(QApplication::translate("UserListForm", "toolBar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class UserListForm: public Ui_UserListForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_USERLISTFORM_H
