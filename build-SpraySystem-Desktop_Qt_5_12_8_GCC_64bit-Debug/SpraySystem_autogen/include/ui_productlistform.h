/********************************************************************************
** Form generated from reading UI file 'productlistform.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PRODUCTLISTFORM_H
#define UI_PRODUCTLISTFORM_H

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

class Ui_ProductListForm
{
public:
    QAction *act_EditProduct;
    QAction *act_AddProduct;
    QAction *act_delete;
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout;
    QTableView *tableView;
    QToolBar *toolBar;
    QToolBar *toolBar_2;

    void setupUi(QMainWindow *ProductListForm)
    {
        if (ProductListForm->objectName().isEmpty())
            ProductListForm->setObjectName(QString::fromUtf8("ProductListForm"));
        ProductListForm->resize(800, 600);
        ProductListForm->setMinimumSize(QSize(800, 600));
        ProductListForm->setMaximumSize(QSize(800, 600));
        act_EditProduct = new QAction(ProductListForm);
        act_EditProduct->setObjectName(QString::fromUtf8("act_EditProduct"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/Images/edit.png"), QSize(), QIcon::Normal, QIcon::Off);
        act_EditProduct->setIcon(icon);
        act_AddProduct = new QAction(ProductListForm);
        act_AddProduct->setObjectName(QString::fromUtf8("act_AddProduct"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/Images/add.png"), QSize(), QIcon::Normal, QIcon::Off);
        act_AddProduct->setIcon(icon1);
        act_delete = new QAction(ProductListForm);
        act_delete->setObjectName(QString::fromUtf8("act_delete"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/Images/delete.png"), QSize(), QIcon::Normal, QIcon::Off);
        act_delete->setIcon(icon2);
        centralwidget = new QWidget(ProductListForm);
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

        ProductListForm->setCentralWidget(centralwidget);
        toolBar = new QToolBar(ProductListForm);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        ProductListForm->addToolBar(Qt::TopToolBarArea, toolBar);
        toolBar_2 = new QToolBar(ProductListForm);
        toolBar_2->setObjectName(QString::fromUtf8("toolBar_2"));
        QFont font;
        font.setPointSize(18);
        toolBar_2->setFont(font);
        toolBar_2->setIconSize(QSize(30, 30));
        toolBar_2->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        ProductListForm->addToolBar(Qt::TopToolBarArea, toolBar_2);

        toolBar_2->addAction(act_AddProduct);
        toolBar_2->addAction(act_delete);
        toolBar_2->addAction(act_EditProduct);

        retranslateUi(ProductListForm);

        QMetaObject::connectSlotsByName(ProductListForm);
    } // setupUi

    void retranslateUi(QMainWindow *ProductListForm)
    {
        ProductListForm->setWindowTitle(QApplication::translate("ProductListForm", "\344\272\247\345\223\201\345\210\227\350\241\250", nullptr));
        act_EditProduct->setText(QApplication::translate("ProductListForm", "\347\274\226\350\276\221\344\272\247\345\223\201", nullptr));
#ifndef QT_NO_TOOLTIP
        act_EditProduct->setToolTip(QApplication::translate("ProductListForm", "\347\274\226\350\276\221\347\224\250\346\210\267", nullptr));
#endif // QT_NO_TOOLTIP
        act_AddProduct->setText(QApplication::translate("ProductListForm", "\346\226\260\345\242\236\344\272\247\345\223\201", nullptr));
#ifndef QT_NO_TOOLTIP
        act_AddProduct->setToolTip(QApplication::translate("ProductListForm", "\346\226\260\345\242\236\347\224\250\346\210\267", nullptr));
#endif // QT_NO_TOOLTIP
        act_delete->setText(QApplication::translate("ProductListForm", "\345\210\240\351\231\244\344\272\247\345\223\201", nullptr));
#ifndef QT_NO_TOOLTIP
        act_delete->setToolTip(QApplication::translate("ProductListForm", "\345\210\240\351\231\244\344\272\247\345\223\201", nullptr));
#endif // QT_NO_TOOLTIP
        toolBar->setWindowTitle(QApplication::translate("ProductListForm", "toolBar", nullptr));
        toolBar_2->setWindowTitle(QApplication::translate("ProductListForm", "toolBar_2", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ProductListForm: public Ui_ProductListForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PRODUCTLISTFORM_H
