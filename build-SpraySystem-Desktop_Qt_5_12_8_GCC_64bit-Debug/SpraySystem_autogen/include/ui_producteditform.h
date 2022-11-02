/********************************************************************************
** Form generated from reading UI file 'producteditform.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PRODUCTEDITFORM_H
#define UI_PRODUCTEDITFORM_H

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

class Ui_ProductEditForm
{
public:
    QWidget *centralwidget;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout_4;
    QPushButton *btn_Cancel;
    QSpacerItem *horizontalSpacer;
    QPushButton *btn_OK;
    QWidget *layoutWidget1;
    QGridLayout *gridLayout;
    QLabel *label_2;
    QLabel *label_4;
    QHBoxLayout *horizontalLayout;
    QLineEdit *edit_Length;
    QLabel *label_6;
    QLabel *label_3;
    QHBoxLayout *horizontalLayout_3;
    QLineEdit *edit_Height;
    QLabel *label_8;
    QLabel *label;
    QHBoxLayout *horizontalLayout_2;
    QLineEdit *edit_Width;
    QLabel *label_7;
    QLineEdit *edit_Name;

    void setupUi(QMainWindow *ProductEditForm)
    {
        if (ProductEditForm->objectName().isEmpty())
            ProductEditForm->setObjectName(QString::fromUtf8("ProductEditForm"));
        ProductEditForm->resize(844, 603);
        centralwidget = new QWidget(ProductEditForm);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        layoutWidget = new QWidget(centralwidget);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(170, 430, 461, 62));
        horizontalLayout_4 = new QHBoxLayout(layoutWidget);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(0, 0, 0, 0);
        btn_Cancel = new QPushButton(layoutWidget);
        btn_Cancel->setObjectName(QString::fromUtf8("btn_Cancel"));
        btn_Cancel->setMinimumSize(QSize(150, 60));
        QFont font;
        font.setPointSize(18);
        btn_Cancel->setFont(font);

        horizontalLayout_4->addWidget(btn_Cancel);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer);

        btn_OK = new QPushButton(layoutWidget);
        btn_OK->setObjectName(QString::fromUtf8("btn_OK"));
        btn_OK->setMinimumSize(QSize(150, 60));
        btn_OK->setFont(font);

        horizontalLayout_4->addWidget(btn_OK);

        layoutWidget1 = new QWidget(centralwidget);
        layoutWidget1->setObjectName(QString::fromUtf8("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(100, 110, 611, 293));
        layoutWidget1->setFont(font);
        gridLayout = new QGridLayout(layoutWidget1);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        label_2 = new QLabel(layoutWidget1);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setMaximumSize(QSize(150, 45));
        label_2->setFont(font);

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        label_4 = new QLabel(layoutWidget1);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setMaximumSize(QSize(150, 45));
        label_4->setFont(font);

        gridLayout->addWidget(label_4, 3, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        edit_Length = new QLineEdit(layoutWidget1);
        edit_Length->setObjectName(QString::fromUtf8("edit_Length"));
        edit_Length->setMaximumSize(QSize(80, 16777215));
        edit_Length->setFont(font);

        horizontalLayout->addWidget(edit_Length);

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

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        edit_Height = new QLineEdit(layoutWidget1);
        edit_Height->setObjectName(QString::fromUtf8("edit_Height"));
        edit_Height->setMaximumSize(QSize(80, 16777215));
        edit_Height->setFont(font);

        horizontalLayout_3->addWidget(edit_Height);

        label_8 = new QLabel(layoutWidget1);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setFont(font);

        horizontalLayout_3->addWidget(label_8);


        gridLayout->addLayout(horizontalLayout_3, 3, 1, 1, 1);

        label = new QLabel(layoutWidget1);
        label->setObjectName(QString::fromUtf8("label"));
        label->setMaximumSize(QSize(150, 45));
        label->setFont(font);

        gridLayout->addWidget(label, 0, 0, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        edit_Width = new QLineEdit(layoutWidget1);
        edit_Width->setObjectName(QString::fromUtf8("edit_Width"));
        edit_Width->setMaximumSize(QSize(80, 16777215));
        edit_Width->setFont(font);

        horizontalLayout_2->addWidget(edit_Width);

        label_7 = new QLabel(layoutWidget1);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setFont(font);

        horizontalLayout_2->addWidget(label_7);


        gridLayout->addLayout(horizontalLayout_2, 2, 1, 1, 1);

        edit_Name = new QLineEdit(layoutWidget1);
        edit_Name->setObjectName(QString::fromUtf8("edit_Name"));
        edit_Name->setMaximumSize(QSize(200, 16777215));
        edit_Name->setFont(font);

        gridLayout->addWidget(edit_Name, 0, 1, 1, 1);

        ProductEditForm->setCentralWidget(centralwidget);
        QWidget::setTabOrder(edit_Name, edit_Length);
        QWidget::setTabOrder(edit_Length, edit_Width);
        QWidget::setTabOrder(edit_Width, edit_Height);
        QWidget::setTabOrder(edit_Height, btn_Cancel);
        QWidget::setTabOrder(btn_Cancel, btn_OK);

        retranslateUi(ProductEditForm);

        QMetaObject::connectSlotsByName(ProductEditForm);
    } // setupUi

    void retranslateUi(QMainWindow *ProductEditForm)
    {
        ProductEditForm->setWindowTitle(QApplication::translate("ProductEditForm", "\344\272\247\345\223\201\344\277\241\346\201\257", nullptr));
        btn_Cancel->setText(QApplication::translate("ProductEditForm", "\345\217\226\346\266\210", nullptr));
        btn_OK->setText(QApplication::translate("ProductEditForm", "\347\241\256\345\256\232", nullptr));
        label_2->setText(QApplication::translate("ProductEditForm", "\351\225\277", nullptr));
        label_4->setText(QApplication::translate("ProductEditForm", "\351\253\230", nullptr));
        label_6->setText(QApplication::translate("ProductEditForm", "/ m", nullptr));
        label_3->setText(QApplication::translate("ProductEditForm", "\345\256\275", nullptr));
        label_8->setText(QApplication::translate("ProductEditForm", "/ m", nullptr));
        label->setText(QApplication::translate("ProductEditForm", "\344\272\247\345\223\201\345\220\215\347\247\260", nullptr));
        label_7->setText(QApplication::translate("ProductEditForm", "/ m", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ProductEditForm: public Ui_ProductEditForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PRODUCTEDITFORM_H
