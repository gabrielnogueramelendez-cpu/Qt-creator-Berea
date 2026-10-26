/********************************************************************************
** Form generated from reading UI file 'pagodialog.ui'
**
** Created by: Qt User Interface Compiler version 6.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PAGODIALOG_H
#define UI_PAGODIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PagoDialog
{
public:
    QVBoxLayout *verticalLayoutMain;
    QLabel *lblTituloPago;
    QGroupBox *groupDatosAlumno;
    QFormLayout *formLayoutAlumno;
    QLabel *lblEtiquetaDni;
    QLineEdit *txtPagoDni;
    QLabel *lblEtiquetaNombre;
    QLineEdit *txtPagoNombre;
    QLabel *lblEtiquetaCurso;
    QLineEdit *txtPagoCurso;
    QGroupBox *groupHorario;
    QVBoxLayout *verticalLayoutHorario;
    QComboBox *comboRecreo;
    QGroupBox *groupMetodoPago;
    QVBoxLayout *verticalLayoutMetodo;
    QHBoxLayout *layoutTotalPedido;
    QLabel *lblResumenTotal;
    QComboBox *comboMetodoPago;
    QStackedWidget *stackedDetallePago;
    QWidget *pageEfectivo;
    QVBoxLayout *layoutPageEfectivo;
    QLabel *lblIndicacionEfectivo;
    QLineEdit *txtMontoAbonado;
    QLabel *lblVueltoCalculado;
    QWidget *pageSaldoCuenta;
    QVBoxLayout *layoutPageSaldo;
    QLabel *lblSaldoDisponible;
    QLabel *lblEstadoSaldo;
    QWidget *pageMercadoPago;
    QVBoxLayout *layoutPageMP;
    QLabel *lblDatosMP;
    QLineEdit *txtComprobanteMP;
    QCheckBox *chkConfirmarTerminos;
    QHBoxLayout *layoutBotones;
    QPushButton *btnCancelarPago;
    QSpacerItem *spacerBotones;
    QPushButton *btnConfirmarPedido;

    void setupUi(QDialog *PagoDialog)
    {
        if (PagoDialog->objectName().isEmpty())
            PagoDialog->setObjectName("PagoDialog");
        PagoDialog->resize(620, 660);
        verticalLayoutMain = new QVBoxLayout(PagoDialog);
        verticalLayoutMain->setSpacing(12);
        verticalLayoutMain->setObjectName("verticalLayoutMain");
        verticalLayoutMain->setContentsMargins(24, 20, 24, 20);
        lblTituloPago = new QLabel(PagoDialog);
        lblTituloPago->setObjectName("lblTituloPago");
        QFont font;
        font.setPointSize(14);
        font.setBold(true);
        lblTituloPago->setFont(font);

        verticalLayoutMain->addWidget(lblTituloPago);

        groupDatosAlumno = new QGroupBox(PagoDialog);
        groupDatosAlumno->setObjectName("groupDatosAlumno");
        formLayoutAlumno = new QFormLayout(groupDatosAlumno);
        formLayoutAlumno->setSpacing(10);
        formLayoutAlumno->setObjectName("formLayoutAlumno");
        lblEtiquetaDni = new QLabel(groupDatosAlumno);
        lblEtiquetaDni->setObjectName("lblEtiquetaDni");

        formLayoutAlumno->setWidget(0, QFormLayout::ItemRole::LabelRole, lblEtiquetaDni);

        txtPagoDni = new QLineEdit(groupDatosAlumno);
        txtPagoDni->setObjectName("txtPagoDni");
        txtPagoDni->setReadOnly(false);

        formLayoutAlumno->setWidget(0, QFormLayout::ItemRole::FieldRole, txtPagoDni);

        lblEtiquetaNombre = new QLabel(groupDatosAlumno);
        lblEtiquetaNombre->setObjectName("lblEtiquetaNombre");

        formLayoutAlumno->setWidget(1, QFormLayout::ItemRole::LabelRole, lblEtiquetaNombre);

        txtPagoNombre = new QLineEdit(groupDatosAlumno);
        txtPagoNombre->setObjectName("txtPagoNombre");

        formLayoutAlumno->setWidget(1, QFormLayout::ItemRole::FieldRole, txtPagoNombre);

        lblEtiquetaCurso = new QLabel(groupDatosAlumno);
        lblEtiquetaCurso->setObjectName("lblEtiquetaCurso");

        formLayoutAlumno->setWidget(2, QFormLayout::ItemRole::LabelRole, lblEtiquetaCurso);

        txtPagoCurso = new QLineEdit(groupDatosAlumno);
        txtPagoCurso->setObjectName("txtPagoCurso");

        formLayoutAlumno->setWidget(2, QFormLayout::ItemRole::FieldRole, txtPagoCurso);


        verticalLayoutMain->addWidget(groupDatosAlumno);

        groupHorario = new QGroupBox(PagoDialog);
        groupHorario->setObjectName("groupHorario");
        verticalLayoutHorario = new QVBoxLayout(groupHorario);
        verticalLayoutHorario->setObjectName("verticalLayoutHorario");
        comboRecreo = new QComboBox(groupHorario);
        comboRecreo->addItem(QString());
        comboRecreo->addItem(QString());
        comboRecreo->addItem(QString());
        comboRecreo->addItem(QString());
        comboRecreo->addItem(QString());
        comboRecreo->setObjectName("comboRecreo");

        verticalLayoutHorario->addWidget(comboRecreo);


        verticalLayoutMain->addWidget(groupHorario);

        groupMetodoPago = new QGroupBox(PagoDialog);
        groupMetodoPago->setObjectName("groupMetodoPago");
        verticalLayoutMetodo = new QVBoxLayout(groupMetodoPago);
        verticalLayoutMetodo->setSpacing(10);
        verticalLayoutMetodo->setObjectName("verticalLayoutMetodo");
        layoutTotalPedido = new QHBoxLayout();
        layoutTotalPedido->setObjectName("layoutTotalPedido");
        lblResumenTotal = new QLabel(groupMetodoPago);
        lblResumenTotal->setObjectName("lblResumenTotal");
        QFont font1;
        font1.setPointSize(12);
        font1.setBold(true);
        lblResumenTotal->setFont(font1);

        layoutTotalPedido->addWidget(lblResumenTotal);


        verticalLayoutMetodo->addLayout(layoutTotalPedido);

        comboMetodoPago = new QComboBox(groupMetodoPago);
        comboMetodoPago->addItem(QString());
        comboMetodoPago->addItem(QString());
        comboMetodoPago->addItem(QString());
        comboMetodoPago->setObjectName("comboMetodoPago");

        verticalLayoutMetodo->addWidget(comboMetodoPago);

        stackedDetallePago = new QStackedWidget(groupMetodoPago);
        stackedDetallePago->setObjectName("stackedDetallePago");
        pageEfectivo = new QWidget();
        pageEfectivo->setObjectName("pageEfectivo");
        layoutPageEfectivo = new QVBoxLayout(pageEfectivo);
        layoutPageEfectivo->setObjectName("layoutPageEfectivo");
        lblIndicacionEfectivo = new QLabel(pageEfectivo);
        lblIndicacionEfectivo->setObjectName("lblIndicacionEfectivo");

        layoutPageEfectivo->addWidget(lblIndicacionEfectivo);

        txtMontoAbonado = new QLineEdit(pageEfectivo);
        txtMontoAbonado->setObjectName("txtMontoAbonado");

        layoutPageEfectivo->addWidget(txtMontoAbonado);

        lblVueltoCalculado = new QLabel(pageEfectivo);
        lblVueltoCalculado->setObjectName("lblVueltoCalculado");
        QFont font2;
        font2.setBold(true);
        lblVueltoCalculado->setFont(font2);

        layoutPageEfectivo->addWidget(lblVueltoCalculado);

        stackedDetallePago->addWidget(pageEfectivo);
        pageSaldoCuenta = new QWidget();
        pageSaldoCuenta->setObjectName("pageSaldoCuenta");
        layoutPageSaldo = new QVBoxLayout(pageSaldoCuenta);
        layoutPageSaldo->setObjectName("layoutPageSaldo");
        lblSaldoDisponible = new QLabel(pageSaldoCuenta);
        lblSaldoDisponible->setObjectName("lblSaldoDisponible");

        layoutPageSaldo->addWidget(lblSaldoDisponible);

        lblEstadoSaldo = new QLabel(pageSaldoCuenta);
        lblEstadoSaldo->setObjectName("lblEstadoSaldo");
        lblEstadoSaldo->setFont(font2);

        layoutPageSaldo->addWidget(lblEstadoSaldo);

        stackedDetallePago->addWidget(pageSaldoCuenta);
        pageMercadoPago = new QWidget();
        pageMercadoPago->setObjectName("pageMercadoPago");
        layoutPageMP = new QVBoxLayout(pageMercadoPago);
        layoutPageMP->setObjectName("layoutPageMP");
        lblDatosMP = new QLabel(pageMercadoPago);
        lblDatosMP->setObjectName("lblDatosMP");

        layoutPageMP->addWidget(lblDatosMP);

        txtComprobanteMP = new QLineEdit(pageMercadoPago);
        txtComprobanteMP->setObjectName("txtComprobanteMP");

        layoutPageMP->addWidget(txtComprobanteMP);

        stackedDetallePago->addWidget(pageMercadoPago);

        verticalLayoutMetodo->addWidget(stackedDetallePago);


        verticalLayoutMain->addWidget(groupMetodoPago);

        chkConfirmarTerminos = new QCheckBox(PagoDialog);
        chkConfirmarTerminos->setObjectName("chkConfirmarTerminos");

        verticalLayoutMain->addWidget(chkConfirmarTerminos);

        layoutBotones = new QHBoxLayout();
        layoutBotones->setObjectName("layoutBotones");
        btnCancelarPago = new QPushButton(PagoDialog);
        btnCancelarPago->setObjectName("btnCancelarPago");

        layoutBotones->addWidget(btnCancelarPago);

        spacerBotones = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        layoutBotones->addItem(spacerBotones);

        btnConfirmarPedido = new QPushButton(PagoDialog);
        btnConfirmarPedido->setObjectName("btnConfirmarPedido");
        btnConfirmarPedido->setFont(font2);

        layoutBotones->addWidget(btnConfirmarPedido);


        verticalLayoutMain->addLayout(layoutBotones);


        retranslateUi(PagoDialog);

        stackedDetallePago->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(PagoDialog);
    } // setupUi

    void retranslateUi(QDialog *PagoDialog)
    {
        PagoDialog->setWindowTitle(QCoreApplication::translate("PagoDialog", "Verificaci\303\263n de Datos y Pago - El Buffet de Alejandro", nullptr));
        lblTituloPago->setText(QCoreApplication::translate("PagoDialog", "\360\237\222\263 Confirmaci\303\263n de Pedido y M\303\251todo de Pago", nullptr));
        groupDatosAlumno->setTitle(QCoreApplication::translate("PagoDialog", "\360\237\223\213 1. Verificaci\303\263n de Datos del Estudiante", nullptr));
        lblEtiquetaDni->setText(QCoreApplication::translate("PagoDialog", "DNI del Alumno:", nullptr));
        lblEtiquetaNombre->setText(QCoreApplication::translate("PagoDialog", "Nombre y Apellido:", nullptr));
        lblEtiquetaCurso->setText(QCoreApplication::translate("PagoDialog", "Curso y Divisi\303\263n:", nullptr));
        groupHorario->setTitle(QCoreApplication::translate("PagoDialog", "\342\217\260 2. Horario / Recreo de Retiro en el Buffet", nullptr));
        comboRecreo->setItemText(0, QCoreApplication::translate("PagoDialog", "\360\237\224\224 1er Recreo Turno Ma\303\261ana (10:35 hs)", nullptr));
        comboRecreo->setItemText(1, QCoreApplication::translate("PagoDialog", "\360\237\224\224 2do Recreo Turno Ma\303\261ana (12:00 hs)", nullptr));
        comboRecreo->setItemText(2, QCoreApplication::translate("PagoDialog", "\360\237\215\275\357\270\217 Almuerzo Buffet Escolar (12:45 hs - 13:30 hs)", nullptr));
        comboRecreo->setItemText(3, QCoreApplication::translate("PagoDialog", "\360\237\224\224 1er Recreo Turno Tarde (15:35 hs)", nullptr));
        comboRecreo->setItemText(4, QCoreApplication::translate("PagoDialog", "\360\237\216\222 Salida de Talleres T\303\251cnicos (17:45 hs)", nullptr));

        groupMetodoPago->setTitle(QCoreApplication::translate("PagoDialog", "\360\237\222\260 3. Selecci\303\263n y Validaci\303\263n del M\303\251todo de Pago", nullptr));
        lblResumenTotal->setText(QCoreApplication::translate("PagoDialog", "Total a Pagar: $0", nullptr));
        comboMetodoPago->setItemText(0, QCoreApplication::translate("PagoDialog", "\360\237\222\265 Efectivo en Mostrador (Caja Buffet)", nullptr));
        comboMetodoPago->setItemText(1, QCoreApplication::translate("PagoDialog", "\360\237\222\263 Saldo de Cuenta Escolar Buffet", nullptr));
        comboMetodoPago->setItemText(2, QCoreApplication::translate("PagoDialog", "\360\237\223\261 Mercado Pago / Transferencia Institucional", nullptr));

        lblIndicacionEfectivo->setText(QCoreApplication::translate("PagoDialog", "Indica el billete o monto con el que vas a abonar en caja:", nullptr));
        txtMontoAbonado->setPlaceholderText(QCoreApplication::translate("PagoDialog", "Ej. 5000 o 10000", nullptr));
        lblVueltoCalculado->setText(QCoreApplication::translate("PagoDialog", "Vuelto a recibir en caja: $0", nullptr));
        lblSaldoDisponible->setText(QCoreApplication::translate("PagoDialog", "Saldo disponible en tu cuenta escolar: $15.000", nullptr));
        lblEstadoSaldo->setText(QCoreApplication::translate("PagoDialog", "\342\234\205 Saldo suficiente para cubrir esta compra.", nullptr));
        lblDatosMP->setText(QCoreApplication::translate("PagoDialog", "<b>Alias:</b> BUFFET.ESCUELA.MP <br><b>CVU:</b> 00000031000987654321", nullptr));
        txtComprobanteMP->setPlaceholderText(QCoreApplication::translate("PagoDialog", "Ingresa los 4 d\303\255gitos finales del comprobante de transferencia", nullptr));
        chkConfirmarTerminos->setText(QCoreApplication::translate("PagoDialog", "Declaro que los datos son correctos y me comprometo a retirar el pedido en el horario seleccionado.", nullptr));
        btnCancelarPago->setText(QCoreApplication::translate("PagoDialog", "Volver al Carrito", nullptr));
        btnConfirmarPedido->setText(QCoreApplication::translate("PagoDialog", "\360\237\216\211 CONFIRMAR PEDIDO Y OBTENER C\303\223DIGO \342\236\224", nullptr));
    } // retranslateUi

};

namespace Ui {
    class PagoDialog: public Ui_PagoDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PAGODIALOG_H
