/********************************************************************************
** Form generated from reading UI file 'cajerowindow.ui'
**
** Created by: Qt User Interface Compiler version 6.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CAJEROWINDOW_H
#define UI_CAJEROWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CajeroWindow
{
public:
    QVBoxLayout *verticalLayoutMain;
    QFrame *headerFrame;
    QHBoxLayout *layoutHeader;
    QLabel *lblCajeroTitle;
    QPushButton *btnLogout;
    QFrame *cardSearch;
    QHBoxLayout *layoutSearch;
    QLabel *lblSearchPrompt;
    QLineEdit *txtCodigoRetiro;
    QPushButton *btnBuscarCodigo;
    QFrame *cardDetail;
    QVBoxLayout *layoutDetail;
    QLabel *lblStatusPedido;
    QLabel *lblDetalleAlumno;
    QLabel *lblDetalleRecreo;
    QTableWidget *tableDetalleItems;
    QHBoxLayout *layoutActions;
    QLabel *lblTotalMonto;
    QPushButton *btnEntregarPedido;
    QPushButton *btnCancelarPedido;
    QTableWidget *tableColasPedidos;

    void setupUi(QWidget *CajeroWindow)
    {
        if (CajeroWindow->objectName().isEmpty())
            CajeroWindow->setObjectName("CajeroWindow");
        CajeroWindow->resize(950, 650);
        verticalLayoutMain = new QVBoxLayout(CajeroWindow);
        verticalLayoutMain->setObjectName("verticalLayoutMain");
        headerFrame = new QFrame(CajeroWindow);
        headerFrame->setObjectName("headerFrame");
        layoutHeader = new QHBoxLayout(headerFrame);
        layoutHeader->setObjectName("layoutHeader");
        lblCajeroTitle = new QLabel(headerFrame);
        lblCajeroTitle->setObjectName("lblCajeroTitle");

        layoutHeader->addWidget(lblCajeroTitle);

        btnLogout = new QPushButton(headerFrame);
        btnLogout->setObjectName("btnLogout");

        layoutHeader->addWidget(btnLogout);


        verticalLayoutMain->addWidget(headerFrame);

        cardSearch = new QFrame(CajeroWindow);
        cardSearch->setObjectName("cardSearch");
        layoutSearch = new QHBoxLayout(cardSearch);
        layoutSearch->setObjectName("layoutSearch");
        lblSearchPrompt = new QLabel(cardSearch);
        lblSearchPrompt->setObjectName("lblSearchPrompt");

        layoutSearch->addWidget(lblSearchPrompt);

        txtCodigoRetiro = new QLineEdit(cardSearch);
        txtCodigoRetiro->setObjectName("txtCodigoRetiro");

        layoutSearch->addWidget(txtCodigoRetiro);

        btnBuscarCodigo = new QPushButton(cardSearch);
        btnBuscarCodigo->setObjectName("btnBuscarCodigo");

        layoutSearch->addWidget(btnBuscarCodigo);


        verticalLayoutMain->addWidget(cardSearch);

        cardDetail = new QFrame(CajeroWindow);
        cardDetail->setObjectName("cardDetail");
        layoutDetail = new QVBoxLayout(cardDetail);
        layoutDetail->setObjectName("layoutDetail");
        lblStatusPedido = new QLabel(cardDetail);
        lblStatusPedido->setObjectName("lblStatusPedido");

        layoutDetail->addWidget(lblStatusPedido);

        lblDetalleAlumno = new QLabel(cardDetail);
        lblDetalleAlumno->setObjectName("lblDetalleAlumno");

        layoutDetail->addWidget(lblDetalleAlumno);

        lblDetalleRecreo = new QLabel(cardDetail);
        lblDetalleRecreo->setObjectName("lblDetalleRecreo");

        layoutDetail->addWidget(lblDetalleRecreo);

        tableDetalleItems = new QTableWidget(cardDetail);
        tableDetalleItems->setObjectName("tableDetalleItems");

        layoutDetail->addWidget(tableDetalleItems);

        layoutActions = new QHBoxLayout();
        layoutActions->setObjectName("layoutActions");
        lblTotalMonto = new QLabel(cardDetail);
        lblTotalMonto->setObjectName("lblTotalMonto");

        layoutActions->addWidget(lblTotalMonto);

        btnEntregarPedido = new QPushButton(cardDetail);
        btnEntregarPedido->setObjectName("btnEntregarPedido");

        layoutActions->addWidget(btnEntregarPedido);

        btnCancelarPedido = new QPushButton(cardDetail);
        btnCancelarPedido->setObjectName("btnCancelarPedido");

        layoutActions->addWidget(btnCancelarPedido);


        layoutDetail->addLayout(layoutActions);


        verticalLayoutMain->addWidget(cardDetail);

        tableColasPedidos = new QTableWidget(CajeroWindow);
        tableColasPedidos->setObjectName("tableColasPedidos");

        verticalLayoutMain->addWidget(tableColasPedidos);


        retranslateUi(CajeroWindow);

        QMetaObject::connectSlotsByName(CajeroWindow);
    } // setupUi

    void retranslateUi(QWidget *CajeroWindow)
    {
        CajeroWindow->setWindowTitle(QCoreApplication::translate("CajeroWindow", "Panel del Personal de Buffet - Caja & Retiros", nullptr));
        lblCajeroTitle->setText(QCoreApplication::translate("CajeroWindow", "\360\237\247\221\342\200\215\360\237\215\263 PANEL DE MOSTRADOR - CAJA Y RETIROS EXPRESS", nullptr));
        btnLogout->setText(QCoreApplication::translate("CajeroWindow", "\360\237\232\252 Cerrar Sesi\303\263n", nullptr));
        lblSearchPrompt->setText(QCoreApplication::translate("CajeroWindow", "C\303\263digo de Retiro (5 d\303\255gitos):", nullptr));
        txtCodigoRetiro->setPlaceholderText(QCoreApplication::translate("CajeroWindow", "Ej: 3C6MB", nullptr));
        btnBuscarCodigo->setText(QCoreApplication::translate("CajeroWindow", "\360\237\224\215 Buscar Pedido", nullptr));
        lblStatusPedido->setText(QCoreApplication::translate("CajeroWindow", "Esperando ingreso de c\303\263digo...", nullptr));
        lblDetalleAlumno->setText(QCoreApplication::translate("CajeroWindow", "Alumno: -", nullptr));
        lblDetalleRecreo->setText(QCoreApplication::translate("CajeroWindow", "Horario de Retiro: -", nullptr));
        lblTotalMonto->setText(QCoreApplication::translate("CajeroWindow", "Total a Cobrar: $0", nullptr));
        btnEntregarPedido->setText(QCoreApplication::translate("CajeroWindow", "\342\234\205 Confirmar Entrega y Cobro", nullptr));
        btnCancelarPedido->setText(QCoreApplication::translate("CajeroWindow", "\342\235\214 Cancelar Pedido", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CajeroWindow: public Ui_CajeroWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CAJEROWINDOW_H
