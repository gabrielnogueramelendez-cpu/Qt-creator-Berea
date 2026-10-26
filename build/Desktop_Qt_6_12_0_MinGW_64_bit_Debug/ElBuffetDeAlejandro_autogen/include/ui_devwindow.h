/********************************************************************************
** Form generated from reading UI file 'devwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DEVWINDOW_H
#define UI_DEVWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DevWindow
{
public:
    QVBoxLayout *verticalLayout;
    QFrame *frameHeaderDev;
    QHBoxLayout *layoutHeaderDev;
    QLabel *lblTitleDev;
    QSpacerItem *spacerDevHeader;
    QPushButton *btnLogoutDev;
    QTabWidget *tabWidgetDev;
    QWidget *tabMySQL;
    QVBoxLayout *layoutTabMySQL;
    QGroupBox *groupParamsMySQL;
    QHBoxLayout *layoutParams;
    QLabel *lblHost;
    QLabel *lblPort;
    QLabel *lblDB;
    QLabel *lblUser;
    QPushButton *btnTestConexion;
    QTextEdit *txtSalidaMySQL;
    QWidget *tabSPs;
    QVBoxLayout *layoutTabSPs;
    QHBoxLayout *layoutSelectorSP;
    QLabel *lblSPLabel;
    QComboBox *comboSP;
    QLineEdit *txtParam1;
    QLineEdit *txtParam2;
    QPushButton *btnEjecutarSP;
    QTextEdit *txtSalidaSP;
    QWidget *tabCoddModel;
    QVBoxLayout *layoutTabCodd;
    QTextEdit *txtModeloCodd;
    QHBoxLayout *layoutAccesoDirecto;
    QLabel *lblAtajosDev;
    QPushButton *btnAbrirVistaAlumno;
    QPushButton *btnAbrirVistaCaja;
    QPushButton *btnAbrirVistaAdmin;
    QSpacerItem *spacerAtajos;

    void setupUi(QWidget *DevWindow)
    {
        if (DevWindow->objectName().isEmpty())
            DevWindow->setObjectName("DevWindow");
        DevWindow->resize(950, 680);
        verticalLayout = new QVBoxLayout(DevWindow);
        verticalLayout->setSpacing(12);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(18, 18, 18, 18);
        frameHeaderDev = new QFrame(DevWindow);
        frameHeaderDev->setObjectName("frameHeaderDev");
        layoutHeaderDev = new QHBoxLayout(frameHeaderDev);
        layoutHeaderDev->setObjectName("layoutHeaderDev");
        lblTitleDev = new QLabel(frameHeaderDev);
        lblTitleDev->setObjectName("lblTitleDev");

        layoutHeaderDev->addWidget(lblTitleDev);

        spacerDevHeader = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        layoutHeaderDev->addItem(spacerDevHeader);

        btnLogoutDev = new QPushButton(frameHeaderDev);
        btnLogoutDev->setObjectName("btnLogoutDev");

        layoutHeaderDev->addWidget(btnLogoutDev);


        verticalLayout->addWidget(frameHeaderDev);

        tabWidgetDev = new QTabWidget(DevWindow);
        tabWidgetDev->setObjectName("tabWidgetDev");
        tabMySQL = new QWidget();
        tabMySQL->setObjectName("tabMySQL");
        layoutTabMySQL = new QVBoxLayout(tabMySQL);
        layoutTabMySQL->setSpacing(10);
        layoutTabMySQL->setObjectName("layoutTabMySQL");
        groupParamsMySQL = new QGroupBox(tabMySQL);
        groupParamsMySQL->setObjectName("groupParamsMySQL");
        layoutParams = new QHBoxLayout(groupParamsMySQL);
        layoutParams->setObjectName("layoutParams");
        lblHost = new QLabel(groupParamsMySQL);
        lblHost->setObjectName("lblHost");

        layoutParams->addWidget(lblHost);

        lblPort = new QLabel(groupParamsMySQL);
        lblPort->setObjectName("lblPort");

        layoutParams->addWidget(lblPort);

        lblDB = new QLabel(groupParamsMySQL);
        lblDB->setObjectName("lblDB");

        layoutParams->addWidget(lblDB);

        lblUser = new QLabel(groupParamsMySQL);
        lblUser->setObjectName("lblUser");

        layoutParams->addWidget(lblUser);

        btnTestConexion = new QPushButton(groupParamsMySQL);
        btnTestConexion->setObjectName("btnTestConexion");

        layoutParams->addWidget(btnTestConexion);


        layoutTabMySQL->addWidget(groupParamsMySQL);

        txtSalidaMySQL = new QTextEdit(tabMySQL);
        txtSalidaMySQL->setObjectName("txtSalidaMySQL");
        txtSalidaMySQL->setReadOnly(true);
        QFont font;
        font.setFamilies({QString::fromUtf8("Consolas")});
        font.setPointSize(10);
        txtSalidaMySQL->setFont(font);

        layoutTabMySQL->addWidget(txtSalidaMySQL);

        tabWidgetDev->addTab(tabMySQL, QString());
        tabSPs = new QWidget();
        tabSPs->setObjectName("tabSPs");
        layoutTabSPs = new QVBoxLayout(tabSPs);
        layoutTabSPs->setObjectName("layoutTabSPs");
        layoutSelectorSP = new QHBoxLayout();
        layoutSelectorSP->setObjectName("layoutSelectorSP");
        lblSPLabel = new QLabel(tabSPs);
        lblSPLabel->setObjectName("lblSPLabel");

        layoutSelectorSP->addWidget(lblSPLabel);

        comboSP = new QComboBox(tabSPs);
        comboSP->addItem(QString());
        comboSP->addItem(QString());
        comboSP->addItem(QString());
        comboSP->addItem(QString());
        comboSP->addItem(QString());
        comboSP->setObjectName("comboSP");

        layoutSelectorSP->addWidget(comboSP);

        txtParam1 = new QLineEdit(tabSPs);
        txtParam1->setObjectName("txtParam1");

        layoutSelectorSP->addWidget(txtParam1);

        txtParam2 = new QLineEdit(tabSPs);
        txtParam2->setObjectName("txtParam2");

        layoutSelectorSP->addWidget(txtParam2);

        btnEjecutarSP = new QPushButton(tabSPs);
        btnEjecutarSP->setObjectName("btnEjecutarSP");

        layoutSelectorSP->addWidget(btnEjecutarSP);


        layoutTabSPs->addLayout(layoutSelectorSP);

        txtSalidaSP = new QTextEdit(tabSPs);
        txtSalidaSP->setObjectName("txtSalidaSP");
        txtSalidaSP->setReadOnly(true);
        txtSalidaSP->setFont(font);

        layoutTabSPs->addWidget(txtSalidaSP);

        tabWidgetDev->addTab(tabSPs, QString());
        tabCoddModel = new QWidget();
        tabCoddModel->setObjectName("tabCoddModel");
        layoutTabCodd = new QVBoxLayout(tabCoddModel);
        layoutTabCodd->setObjectName("layoutTabCodd");
        txtModeloCodd = new QTextEdit(tabCoddModel);
        txtModeloCodd->setObjectName("txtModeloCodd");
        txtModeloCodd->setReadOnly(true);
        txtModeloCodd->setFont(font);

        layoutTabCodd->addWidget(txtModeloCodd);

        tabWidgetDev->addTab(tabCoddModel, QString());

        verticalLayout->addWidget(tabWidgetDev);

        layoutAccesoDirecto = new QHBoxLayout();
        layoutAccesoDirecto->setObjectName("layoutAccesoDirecto");
        lblAtajosDev = new QLabel(DevWindow);
        lblAtajosDev->setObjectName("lblAtajosDev");
        QFont font1;
        font1.setBold(true);
        lblAtajosDev->setFont(font1);

        layoutAccesoDirecto->addWidget(lblAtajosDev);

        btnAbrirVistaAlumno = new QPushButton(DevWindow);
        btnAbrirVistaAlumno->setObjectName("btnAbrirVistaAlumno");

        layoutAccesoDirecto->addWidget(btnAbrirVistaAlumno);

        btnAbrirVistaCaja = new QPushButton(DevWindow);
        btnAbrirVistaCaja->setObjectName("btnAbrirVistaCaja");

        layoutAccesoDirecto->addWidget(btnAbrirVistaCaja);

        btnAbrirVistaAdmin = new QPushButton(DevWindow);
        btnAbrirVistaAdmin->setObjectName("btnAbrirVistaAdmin");

        layoutAccesoDirecto->addWidget(btnAbrirVistaAdmin);

        spacerAtajos = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        layoutAccesoDirecto->addItem(spacerAtajos);


        verticalLayout->addLayout(layoutAccesoDirecto);


        retranslateUi(DevWindow);

        QMetaObject::connectSlotsByName(DevWindow);
    } // setupUi

    void retranslateUi(QWidget *DevWindow)
    {
        DevWindow->setWindowTitle(QCoreApplication::translate("DevWindow", "Consola de Desarrollador & Diagn\303\263stico BDD - El Buffet de Alejandro", nullptr));
        frameHeaderDev->setStyleSheet(QCoreApplication::translate("DevWindow", "background-color: #1A1A24; border-radius: 8px; padding: 10px;", nullptr));
        lblTitleDev->setText(QCoreApplication::translate("DevWindow", "\360\237\222\273 CONSOLA DE DESARROLLADOR \342\200\224 Entorno de Testing y BDD", nullptr));
        lblTitleDev->setStyleSheet(QCoreApplication::translate("DevWindow", "color: #58D68D; font-size: 16px; font-weight: bold; font-family: monospace;", nullptr));
        btnLogoutDev->setText(QCoreApplication::translate("DevWindow", "\360\237\232\252 Salir del Modo Dev", nullptr));
        btnLogoutDev->setStyleSheet(QCoreApplication::translate("DevWindow", "background-color: #C0392B; color: white;", nullptr));
        groupParamsMySQL->setTitle(QCoreApplication::translate("DevWindow", "Par\303\241metros de Conexi\303\263n del Gestor (Laboratorio 4to A\303\261o)", nullptr));
        lblHost->setText(QCoreApplication::translate("DevWindow", "Host: localhost", nullptr));
        lblPort->setText(QCoreApplication::translate("DevWindow", "Puerto: 3306", nullptr));
        lblDB->setText(QCoreApplication::translate("DevWindow", "BDD: ElBuffetDeAlejandro", nullptr));
        lblUser->setText(QCoreApplication::translate("DevWindow", "Usuario: root", nullptr));
        btnTestConexion->setText(QCoreApplication::translate("DevWindow", "\360\237\224\215 Probar Conexi\303\263n con Gestor", nullptr));
        tabWidgetDev->setTabText(tabWidgetDev->indexOf(tabMySQL), QCoreApplication::translate("DevWindow", "\360\237\224\214 Diagn\303\263stico y Conexi\303\263n MySQL", nullptr));
        lblSPLabel->setText(QCoreApplication::translate("DevWindow", "Seleccionar Stored Procedure:", nullptr));
        comboSP->setItemText(0, QCoreApplication::translate("DevWindow", "sp_reporte_ventas_por_fecha", nullptr));
        comboSP->setItemText(1, QCoreApplication::translate("DevWindow", "sp_reporte_auditoria", nullptr));
        comboSP->setItemText(2, QCoreApplication::translate("DevWindow", "sp_productos_mas_vendidos", nullptr));
        comboSP->setItemText(3, QCoreApplication::translate("DevWindow", "sp_stock_critico", nullptr));
        comboSP->setItemText(4, QCoreApplication::translate("DevWindow", "sp_reporte_metricas_tuneado", nullptr));

        txtParam1->setPlaceholderText(QCoreApplication::translate("DevWindow", "Par\303\241metro 1 (ej. 2026-10-01)", nullptr));
        txtParam2->setPlaceholderText(QCoreApplication::translate("DevWindow", "Par\303\241metro 2 (ej. 2026-10-25)", nullptr));
        btnEjecutarSP->setText(QCoreApplication::translate("DevWindow", "\342\226\266\357\270\217 Ejecutar Prueba", nullptr));
        tabWidgetDev->setTabText(tabWidgetDev->indexOf(tabSPs), QCoreApplication::translate("DevWindow", "\360\237\247\252 Consola de Stored Procedures (SPs)", nullptr));
        tabWidgetDev->setTabText(tabWidgetDev->indexOf(tabCoddModel), QCoreApplication::translate("DevWindow", "\360\237\223\232 Modelo Relacional Codd 3FN (20 Tablas)", nullptr));
        lblAtajosDev->setText(QCoreApplication::translate("DevWindow", "\342\232\241 Acceso R\303\241pido de Testing:", nullptr));
        btnAbrirVistaAlumno->setText(QCoreApplication::translate("DevWindow", "\360\237\216\223 Probar Vista Alumno", nullptr));
        btnAbrirVistaCaja->setText(QCoreApplication::translate("DevWindow", "\360\237\247\221\342\200\215\360\237\215\263 Probar Vista Caja", nullptr));
        btnAbrirVistaAdmin->setText(QCoreApplication::translate("DevWindow", "\360\237\233\240\357\270\217 Probar Vista Admin", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DevWindow: public Ui_DevWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DEVWINDOW_H
