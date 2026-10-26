/********************************************************************************
** Form generated from reading UI file 'carritodialog.ui'
**
** Created by: Qt User Interface Compiler version 6.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CARRITODIALOG_H
#define UI_CARRITODIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_CarritoDialog
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *lblTituloCarrito;
    QLabel *lblSubtituloCarrito;
    QTableWidget *tableCarrito;
    QHBoxLayout *layoutOperaciones;
    QPushButton *btnModificarCantidad;
    QPushButton *btnEliminarItem;
    QPushButton *btnVaciarCarrito;
    QSpacerItem *spacerOperaciones;
    QLabel *lblTotalPagar;
    QHBoxLayout *layoutBotonesFinales;
    QPushButton *btnSeguirComprando;
    QSpacerItem *spacerFinal;
    QPushButton *btnProcederPago;

    void setupUi(QDialog *CarritoDialog)
    {
        if (CarritoDialog->objectName().isEmpty())
            CarritoDialog->setObjectName("CarritoDialog");
        CarritoDialog->resize(700, 500);
        verticalLayout = new QVBoxLayout(CarritoDialog);
        verticalLayout->setSpacing(15);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(20, 20, 20, 20);
        lblTituloCarrito = new QLabel(CarritoDialog);
        lblTituloCarrito->setObjectName("lblTituloCarrito");
        QFont font;
        font.setPointSize(14);
        font.setBold(true);
        lblTituloCarrito->setFont(font);

        verticalLayout->addWidget(lblTituloCarrito);

        lblSubtituloCarrito = new QLabel(CarritoDialog);
        lblSubtituloCarrito->setObjectName("lblSubtituloCarrito");

        verticalLayout->addWidget(lblSubtituloCarrito);

        tableCarrito = new QTableWidget(CarritoDialog);
        tableCarrito->setObjectName("tableCarrito");
        tableCarrito->setAlternatingRowColors(true);
        tableCarrito->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableCarrito->horizontalHeader()->setStretchLastSection(true);

        verticalLayout->addWidget(tableCarrito);

        layoutOperaciones = new QHBoxLayout();
        layoutOperaciones->setObjectName("layoutOperaciones");
        btnModificarCantidad = new QPushButton(CarritoDialog);
        btnModificarCantidad->setObjectName("btnModificarCantidad");

        layoutOperaciones->addWidget(btnModificarCantidad);

        btnEliminarItem = new QPushButton(CarritoDialog);
        btnEliminarItem->setObjectName("btnEliminarItem");

        layoutOperaciones->addWidget(btnEliminarItem);

        btnVaciarCarrito = new QPushButton(CarritoDialog);
        btnVaciarCarrito->setObjectName("btnVaciarCarrito");

        layoutOperaciones->addWidget(btnVaciarCarrito);

        spacerOperaciones = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        layoutOperaciones->addItem(spacerOperaciones);

        lblTotalPagar = new QLabel(CarritoDialog);
        lblTotalPagar->setObjectName("lblTotalPagar");
        QFont font1;
        font1.setPointSize(13);
        font1.setBold(true);
        lblTotalPagar->setFont(font1);

        layoutOperaciones->addWidget(lblTotalPagar);


        verticalLayout->addLayout(layoutOperaciones);

        layoutBotonesFinales = new QHBoxLayout();
        layoutBotonesFinales->setObjectName("layoutBotonesFinales");
        btnSeguirComprando = new QPushButton(CarritoDialog);
        btnSeguirComprando->setObjectName("btnSeguirComprando");

        layoutBotonesFinales->addWidget(btnSeguirComprando);

        spacerFinal = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        layoutBotonesFinales->addItem(spacerFinal);

        btnProcederPago = new QPushButton(CarritoDialog);
        btnProcederPago->setObjectName("btnProcederPago");
        QFont font2;
        font2.setBold(true);
        btnProcederPago->setFont(font2);

        layoutBotonesFinales->addWidget(btnProcederPago);


        verticalLayout->addLayout(layoutBotonesFinales);


        retranslateUi(CarritoDialog);

        QMetaObject::connectSlotsByName(CarritoDialog);
    } // setupUi

    void retranslateUi(QDialog *CarritoDialog)
    {
        CarritoDialog->setWindowTitle(QCoreApplication::translate("CarritoDialog", "Mi Carrito de Compras - El Buffet de Alejandro", nullptr));
        lblTituloCarrito->setText(QCoreApplication::translate("CarritoDialog", "\360\237\233\222 Productos en tu Carrito de Compras", nullptr));
        lblSubtituloCarrito->setText(QCoreApplication::translate("CarritoDialog", "Revisa los art\303\255culos de tu merienda o almuerzo antes de confirmar tu pedido express.", nullptr));
        btnModificarCantidad->setText(QCoreApplication::translate("CarritoDialog", "\342\234\217\357\270\217 Modificar Cantidad", nullptr));
        btnEliminarItem->setText(QCoreApplication::translate("CarritoDialog", "\360\237\227\221\357\270\217 Eliminar Producto", nullptr));
        btnVaciarCarrito->setText(QCoreApplication::translate("CarritoDialog", "\342\232\240\357\270\217 Vaciar Carrito", nullptr));
        lblTotalPagar->setText(QCoreApplication::translate("CarritoDialog", "Total: $0", nullptr));
        btnSeguirComprando->setText(QCoreApplication::translate("CarritoDialog", "\342\254\205\357\270\217 Seguir Comprando", nullptr));
        btnProcederPago->setText(QCoreApplication::translate("CarritoDialog", "\360\237\222\263 Proceder al Pago y Verificaci\303\263n \342\236\224", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CarritoDialog: public Ui_CarritoDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CARRITODIALOG_H
