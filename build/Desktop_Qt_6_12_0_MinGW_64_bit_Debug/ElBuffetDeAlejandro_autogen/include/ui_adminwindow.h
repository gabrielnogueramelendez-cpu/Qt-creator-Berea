/********************************************************************************
** Form generated from reading UI file 'adminwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ADMINWINDOW_H
#define UI_ADMINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AdminWindow
{
public:
    QVBoxLayout *verticalLayoutMain;
    QFrame *headerFrame;
    QHBoxLayout *layoutHeader;
    QLabel *lblAdminTitle;
    QPushButton *btnLogout;
    QTabWidget *tabWidgetAdmin;
    QWidget *tabAbm;
    QVBoxLayout *layoutAbm;
    QTableWidget *tableProductosAdmin;
    QFrame *cardFormAbm;
    QHBoxLayout *layoutForm;
    QLineEdit *txtNombreProd;
    QComboBox *comboCategoria;
    QLineEdit *txtPrecioProd;
    QLineEdit *txtStockProd;
    QPushButton *btnAgregarProd;
    QPushButton *btnGuardarEdit;
    QPushButton *btnEliminarProd;
    QWidget *tabLogs;
    QVBoxLayout *layoutLogs;
    QTableWidget *tableLogsAdmin;

    void setupUi(QWidget *AdminWindow)
    {
        if (AdminWindow->objectName().isEmpty())
            AdminWindow->setObjectName("AdminWindow");
        AdminWindow->resize(1000, 700);
        verticalLayoutMain = new QVBoxLayout(AdminWindow);
        verticalLayoutMain->setObjectName("verticalLayoutMain");
        headerFrame = new QFrame(AdminWindow);
        headerFrame->setObjectName("headerFrame");
        layoutHeader = new QHBoxLayout(headerFrame);
        layoutHeader->setObjectName("layoutHeader");
        lblAdminTitle = new QLabel(headerFrame);
        lblAdminTitle->setObjectName("lblAdminTitle");

        layoutHeader->addWidget(lblAdminTitle);

        btnLogout = new QPushButton(headerFrame);
        btnLogout->setObjectName("btnLogout");

        layoutHeader->addWidget(btnLogout);


        verticalLayoutMain->addWidget(headerFrame);

        tabWidgetAdmin = new QTabWidget(AdminWindow);
        tabWidgetAdmin->setObjectName("tabWidgetAdmin");
        tabAbm = new QWidget();
        tabAbm->setObjectName("tabAbm");
        layoutAbm = new QVBoxLayout(tabAbm);
        layoutAbm->setObjectName("layoutAbm");
        tableProductosAdmin = new QTableWidget(tabAbm);
        tableProductosAdmin->setObjectName("tableProductosAdmin");

        layoutAbm->addWidget(tableProductosAdmin);

        cardFormAbm = new QFrame(tabAbm);
        cardFormAbm->setObjectName("cardFormAbm");
        layoutForm = new QHBoxLayout(cardFormAbm);
        layoutForm->setObjectName("layoutForm");
        txtNombreProd = new QLineEdit(cardFormAbm);
        txtNombreProd->setObjectName("txtNombreProd");

        layoutForm->addWidget(txtNombreProd);

        comboCategoria = new QComboBox(cardFormAbm);
        comboCategoria->addItem(QString());
        comboCategoria->addItem(QString());
        comboCategoria->addItem(QString());
        comboCategoria->addItem(QString());
        comboCategoria->setObjectName("comboCategoria");

        layoutForm->addWidget(comboCategoria);

        txtPrecioProd = new QLineEdit(cardFormAbm);
        txtPrecioProd->setObjectName("txtPrecioProd");

        layoutForm->addWidget(txtPrecioProd);

        txtStockProd = new QLineEdit(cardFormAbm);
        txtStockProd->setObjectName("txtStockProd");

        layoutForm->addWidget(txtStockProd);

        btnAgregarProd = new QPushButton(cardFormAbm);
        btnAgregarProd->setObjectName("btnAgregarProd");

        layoutForm->addWidget(btnAgregarProd);

        btnGuardarEdit = new QPushButton(cardFormAbm);
        btnGuardarEdit->setObjectName("btnGuardarEdit");

        layoutForm->addWidget(btnGuardarEdit);

        btnEliminarProd = new QPushButton(cardFormAbm);
        btnEliminarProd->setObjectName("btnEliminarProd");

        layoutForm->addWidget(btnEliminarProd);


        layoutAbm->addWidget(cardFormAbm);

        tabWidgetAdmin->addTab(tabAbm, QString());
        tabLogs = new QWidget();
        tabLogs->setObjectName("tabLogs");
        layoutLogs = new QVBoxLayout(tabLogs);
        layoutLogs->setObjectName("layoutLogs");
        tableLogsAdmin = new QTableWidget(tabLogs);
        tableLogsAdmin->setObjectName("tableLogsAdmin");

        layoutLogs->addWidget(tableLogsAdmin);

        tabWidgetAdmin->addTab(tabLogs, QString());

        verticalLayoutMain->addWidget(tabWidgetAdmin);


        retranslateUi(AdminWindow);

        QMetaObject::connectSlotsByName(AdminWindow);
    } // setupUi

    void retranslateUi(QWidget *AdminWindow)
    {
        AdminWindow->setWindowTitle(QCoreApplication::translate("AdminWindow", "Panel de Administraci\303\263n - Gestor del Buffet", nullptr));
        lblAdminTitle->setText(QCoreApplication::translate("AdminWindow", "\360\237\233\240\357\270\217 PANEL DE ADMINISTRACI\303\223N - GESTOR DEL BUFFET", nullptr));
        btnLogout->setText(QCoreApplication::translate("AdminWindow", "\360\237\232\252 Cerrar Sesi\303\263n", nullptr));
        txtNombreProd->setPlaceholderText(QCoreApplication::translate("AdminWindow", "Nombre de Producto", nullptr));
        comboCategoria->setItemText(0, QCoreApplication::translate("AdminWindow", "Salados", nullptr));
        comboCategoria->setItemText(1, QCoreApplication::translate("AdminWindow", "Dulces", nullptr));
        comboCategoria->setItemText(2, QCoreApplication::translate("AdminWindow", "Bebidas", nullptr));
        comboCategoria->setItemText(3, QCoreApplication::translate("AdminWindow", "Men\303\272 del D\303\255a", nullptr));

        txtPrecioProd->setPlaceholderText(QCoreApplication::translate("AdminWindow", "Precio (ej: 2800)", nullptr));
        txtStockProd->setPlaceholderText(QCoreApplication::translate("AdminWindow", "Stock BDD", nullptr));
        btnAgregarProd->setText(QCoreApplication::translate("AdminWindow", "\342\236\225 Agregar", nullptr));
        btnGuardarEdit->setText(QCoreApplication::translate("AdminWindow", "\360\237\222\276 Guardar", nullptr));
        btnEliminarProd->setText(QCoreApplication::translate("AdminWindow", "\360\237\227\221\357\270\217 Eliminar", nullptr));
        tabWidgetAdmin->setTabText(tabWidgetAdmin->indexOf(tabAbm), QCoreApplication::translate("AdminWindow", "\360\237\223\246 Gesti\303\263n de Inventario & Precios (ABM)", nullptr));
        tabWidgetAdmin->setTabText(tabWidgetAdmin->indexOf(tabLogs), QCoreApplication::translate("AdminWindow", "\360\237\223\212 Reportes & Logs de Auditor\303\255a", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AdminWindow: public Ui_AdminWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADMINWINDOW_H
