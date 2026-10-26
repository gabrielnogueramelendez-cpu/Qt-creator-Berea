/********************************************************************************
** Form generated from reading UI file 'productdetaildialog.ui'
**
** Created by: Qt User Interface Compiler version 6.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PRODUCTDETAILDIALOG_H
#define UI_PRODUCTDETAILDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ProductDetailDialog
{
public:
    QVBoxLayout *verticalLayout;
    QFrame *cardDetail;
    QVBoxLayout *layoutDetail;
    QLabel *lblBadge;
    QLabel *lblTitle;
    QLabel *lblPriceTag;
    QLabel *lblDescription;
    QFrame *framePrepNotice;
    QVBoxLayout *layoutNotice;
    QLabel *lblPrepNotice;
    QHBoxLayout *layoutQuantity;
    QLabel *lblCantLabel;
    QPushButton *btnMinus;
    QLabel *lblCantidad;
    QPushButton *btnPlus;
    QLabel *lblCalculatedTotal;
    QPushButton *btnAddToCart;

    void setupUi(QDialog *ProductDetailDialog)
    {
        if (ProductDetailDialog->objectName().isEmpty())
            ProductDetailDialog->setObjectName("ProductDetailDialog");
        ProductDetailDialog->resize(550, 480);
        verticalLayout = new QVBoxLayout(ProductDetailDialog);
        verticalLayout->setObjectName("verticalLayout");
        cardDetail = new QFrame(ProductDetailDialog);
        cardDetail->setObjectName("cardDetail");
        layoutDetail = new QVBoxLayout(cardDetail);
        layoutDetail->setSpacing(12);
        layoutDetail->setObjectName("layoutDetail");
        layoutDetail->setContentsMargins(25, 25, 25, 25);
        lblBadge = new QLabel(cardDetail);
        lblBadge->setObjectName("lblBadge");

        layoutDetail->addWidget(lblBadge, 0, Qt::AlignCenter);

        lblTitle = new QLabel(cardDetail);
        lblTitle->setObjectName("lblTitle");
        lblTitle->setWordWrap(true);

        layoutDetail->addWidget(lblTitle);

        lblPriceTag = new QLabel(cardDetail);
        lblPriceTag->setObjectName("lblPriceTag");

        layoutDetail->addWidget(lblPriceTag, 0, Qt::AlignCenter);

        lblDescription = new QLabel(cardDetail);
        lblDescription->setObjectName("lblDescription");
        lblDescription->setWordWrap(true);

        layoutDetail->addWidget(lblDescription);

        framePrepNotice = new QFrame(cardDetail);
        framePrepNotice->setObjectName("framePrepNotice");
        layoutNotice = new QVBoxLayout(framePrepNotice);
        layoutNotice->setObjectName("layoutNotice");
        lblPrepNotice = new QLabel(framePrepNotice);
        lblPrepNotice->setObjectName("lblPrepNotice");
        lblPrepNotice->setWordWrap(true);

        layoutNotice->addWidget(lblPrepNotice);


        layoutDetail->addWidget(framePrepNotice);

        layoutQuantity = new QHBoxLayout();
        layoutQuantity->setObjectName("layoutQuantity");
        lblCantLabel = new QLabel(cardDetail);
        lblCantLabel->setObjectName("lblCantLabel");

        layoutQuantity->addWidget(lblCantLabel);

        btnMinus = new QPushButton(cardDetail);
        btnMinus->setObjectName("btnMinus");

        layoutQuantity->addWidget(btnMinus);

        lblCantidad = new QLabel(cardDetail);
        lblCantidad->setObjectName("lblCantidad");
        lblCantidad->setAlignment(Qt::AlignCenter);

        layoutQuantity->addWidget(lblCantidad);

        btnPlus = new QPushButton(cardDetail);
        btnPlus->setObjectName("btnPlus");

        layoutQuantity->addWidget(btnPlus);


        layoutDetail->addLayout(layoutQuantity);

        lblCalculatedTotal = new QLabel(cardDetail);
        lblCalculatedTotal->setObjectName("lblCalculatedTotal");

        layoutDetail->addWidget(lblCalculatedTotal, 0, Qt::AlignRight);

        btnAddToCart = new QPushButton(cardDetail);
        btnAddToCart->setObjectName("btnAddToCart");

        layoutDetail->addWidget(btnAddToCart);


        verticalLayout->addWidget(cardDetail);


        retranslateUi(ProductDetailDialog);

        QMetaObject::connectSlotsByName(ProductDetailDialog);
    } // setupUi

    void retranslateUi(QDialog *ProductDetailDialog)
    {
        ProductDetailDialog->setWindowTitle(QCoreApplication::translate("ProductDetailDialog", "Detalle de Producto - El Buffet de Alejandro", nullptr));
        lblBadge->setText(QCoreApplication::translate("ProductDetailDialog", "\342\234\250 ESPECIALIDAD GOURMET DEL BUFFET", nullptr));
        lblBadge->setStyleSheet(QCoreApplication::translate("ProductDetailDialog", "color: #B8860B; font-weight: bold; font-size: 13px;", nullptr));
        lblTitle->setText(QCoreApplication::translate("ProductDetailDialog", "T\303\255tulo del Producto", nullptr));
        lblTitle->setStyleSheet(QCoreApplication::translate("ProductDetailDialog", "font-size: 20px; font-weight: bold; color: #3D1A24;", nullptr));
        lblPriceTag->setText(QCoreApplication::translate("ProductDetailDialog", "$0", nullptr));
        lblPriceTag->setStyleSheet(QCoreApplication::translate("ProductDetailDialog", "font-size: 26px; font-weight: bold; color: #B8860B;", nullptr));
        lblDescription->setText(QCoreApplication::translate("ProductDetailDialog", "Descripci\303\263n detallada del producto...", nullptr));
        lblDescription->setStyleSheet(QCoreApplication::translate("ProductDetailDialog", "font-size: 13px; color: #555555; line-height: 1.4;", nullptr));
        framePrepNotice->setStyleSheet(QCoreApplication::translate("ProductDetailDialog", "background-color: #FFF9E6; border: 1px solid #FFE58F; border-radius: 8px; padding: 8px;", nullptr));
        lblPrepNotice->setText(QCoreApplication::translate("ProductDetailDialog", "\342\217\261\357\270\217 Tiempo de elaboraci\303\263n en cocina: 15 mins. Encargar antes de las 09:00 hs para asegurarte tu combo en el recreo del almuerzo.", nullptr));
        lblCantLabel->setText(QCoreApplication::translate("ProductDetailDialog", "Cantidad a encargar:", nullptr));
        btnMinus->setText(QCoreApplication::translate("ProductDetailDialog", " - ", nullptr));
        lblCantidad->setText(QCoreApplication::translate("ProductDetailDialog", "1", nullptr));
        btnPlus->setText(QCoreApplication::translate("ProductDetailDialog", " + ", nullptr));
        lblCalculatedTotal->setText(QCoreApplication::translate("ProductDetailDialog", "Total a pagar: $0", nullptr));
        lblCalculatedTotal->setStyleSheet(QCoreApplication::translate("ProductDetailDialog", "font-size: 16px; font-weight: bold; color: #3D1A24;", nullptr));
        btnAddToCart->setText(QCoreApplication::translate("ProductDetailDialog", "\360\237\233\222 A\303\221ADIR AL CARRITO EXPRESS \342\236\224", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ProductDetailDialog: public Ui_ProductDetailDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PRODUCTDETAILDIALOG_H
