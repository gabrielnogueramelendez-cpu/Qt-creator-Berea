/********************************************************************************
** Form generated from reading UI file 'loginwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGINWINDOW_H
#define UI_LOGINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LoginWindow
{
public:
    QVBoxLayout *verticalLayoutMain;
    QFrame *cardLogin;
    QVBoxLayout *verticalLayoutCard;
    QLabel *lblLogoTitle;
    QLabel *lblSubtitle;
    QLabel *lblRole;
    QComboBox *comboRole;
    QLabel *lblUser;
    QLineEdit *txtUser;
    QLabel *lblPass;
    QLineEdit *txtPass;
    QLabel *lblPinCajero;
    QLineEdit *txtPinCajero;
    QLabel *lblTokenDev;
    QLineEdit *txtTokenDev;
    QLabel *lblNoticeRol;
    QPushButton *btnLogin;
    QHBoxLayout *layoutRegisterPrompt;
    QLabel *lblNoAccount;
    QPushButton *btnGoRegister;

    void setupUi(QWidget *LoginWindow)
    {
        if (LoginWindow->objectName().isEmpty())
            LoginWindow->setObjectName("LoginWindow");
        LoginWindow->resize(920, 660);
        verticalLayoutMain = new QVBoxLayout(LoginWindow);
        verticalLayoutMain->setObjectName("verticalLayoutMain");
        cardLogin = new QFrame(LoginWindow);
        cardLogin->setObjectName("cardLogin");
        cardLogin->setMinimumSize(QSize(440, 520));
        cardLogin->setMaximumSize(QSize(480, 580));
        verticalLayoutCard = new QVBoxLayout(cardLogin);
        verticalLayoutCard->setSpacing(10);
        verticalLayoutCard->setObjectName("verticalLayoutCard");
        verticalLayoutCard->setContentsMargins(28, 25, 28, 25);
        lblLogoTitle = new QLabel(cardLogin);
        lblLogoTitle->setObjectName("lblLogoTitle");

        verticalLayoutCard->addWidget(lblLogoTitle, 0, Qt::AlignCenter);

        lblSubtitle = new QLabel(cardLogin);
        lblSubtitle->setObjectName("lblSubtitle");

        verticalLayoutCard->addWidget(lblSubtitle, 0, Qt::AlignCenter);

        lblRole = new QLabel(cardLogin);
        lblRole->setObjectName("lblRole");

        verticalLayoutCard->addWidget(lblRole);

        comboRole = new QComboBox(cardLogin);
        comboRole->addItem(QString());
        comboRole->addItem(QString());
        comboRole->addItem(QString());
        comboRole->addItem(QString());
        comboRole->setObjectName("comboRole");

        verticalLayoutCard->addWidget(comboRole);

        lblUser = new QLabel(cardLogin);
        lblUser->setObjectName("lblUser");

        verticalLayoutCard->addWidget(lblUser);

        txtUser = new QLineEdit(cardLogin);
        txtUser->setObjectName("txtUser");

        verticalLayoutCard->addWidget(txtUser);

        lblPass = new QLabel(cardLogin);
        lblPass->setObjectName("lblPass");

        verticalLayoutCard->addWidget(lblPass);

        txtPass = new QLineEdit(cardLogin);
        txtPass->setObjectName("txtPass");
        txtPass->setEchoMode(QLineEdit::Password);

        verticalLayoutCard->addWidget(txtPass);

        lblPinCajero = new QLabel(cardLogin);
        lblPinCajero->setObjectName("lblPinCajero");

        verticalLayoutCard->addWidget(lblPinCajero);

        txtPinCajero = new QLineEdit(cardLogin);
        txtPinCajero->setObjectName("txtPinCajero");
        txtPinCajero->setEchoMode(QLineEdit::Password);
        txtPinCajero->setMaxLength(4);

        verticalLayoutCard->addWidget(txtPinCajero);

        lblTokenDev = new QLabel(cardLogin);
        lblTokenDev->setObjectName("lblTokenDev");

        verticalLayoutCard->addWidget(lblTokenDev);

        txtTokenDev = new QLineEdit(cardLogin);
        txtTokenDev->setObjectName("txtTokenDev");

        verticalLayoutCard->addWidget(txtTokenDev);

        lblNoticeRol = new QLabel(cardLogin);
        lblNoticeRol->setObjectName("lblNoticeRol");
        lblNoticeRol->setWordWrap(true);

        verticalLayoutCard->addWidget(lblNoticeRol);

        btnLogin = new QPushButton(cardLogin);
        btnLogin->setObjectName("btnLogin");

        verticalLayoutCard->addWidget(btnLogin);

        layoutRegisterPrompt = new QHBoxLayout();
        layoutRegisterPrompt->setObjectName("layoutRegisterPrompt");
        lblNoAccount = new QLabel(cardLogin);
        lblNoAccount->setObjectName("lblNoAccount");

        layoutRegisterPrompt->addWidget(lblNoAccount);

        btnGoRegister = new QPushButton(cardLogin);
        btnGoRegister->setObjectName("btnGoRegister");

        layoutRegisterPrompt->addWidget(btnGoRegister);


        verticalLayoutCard->addLayout(layoutRegisterPrompt);


        verticalLayoutMain->addWidget(cardLogin, 0, Qt::AlignCenter);


        retranslateUi(LoginWindow);

        QMetaObject::connectSlotsByName(LoginWindow);
    } // setupUi

    void retranslateUi(QWidget *LoginWindow)
    {
        LoginWindow->setWindowTitle(QCoreApplication::translate("LoginWindow", "El Buffet de Alejandro - Iniciar Sesi\303\263n", nullptr));
        lblLogoTitle->setText(QCoreApplication::translate("LoginWindow", "\360\237\215\216 EL BUFFET DE ALEJANDRO", nullptr));
        lblSubtitle->setText(QCoreApplication::translate("LoginWindow", "Portal de Acceso Escolar & Gesti\303\263n", nullptr));
        lblRole->setText(QCoreApplication::translate("LoginWindow", "Tipo de Usuario / Rol:", nullptr));
        comboRole->setItemText(0, QCoreApplication::translate("LoginWindow", "\360\237\216\223 Estudiante / Alumno", nullptr));
        comboRole->setItemText(1, QCoreApplication::translate("LoginWindow", "\360\237\247\221\342\200\215\360\237\215\263 Personal de Buffet (Caja & Mostrador)", nullptr));
        comboRole->setItemText(2, QCoreApplication::translate("LoginWindow", "\360\237\233\240\357\270\217 Administrador del Sistema", nullptr));
        comboRole->setItemText(3, QCoreApplication::translate("LoginWindow", "\360\237\222\273 Desarrollador / Soporte T\303\251cnico (Modo Dev)", nullptr));

        lblUser->setText(QCoreApplication::translate("LoginWindow", "DNI o Correo Institucional:", nullptr));
        txtUser->setPlaceholderText(QCoreApplication::translate("LoginWindow", "Ej. 44123456 o alumno@bue.edu.ar", nullptr));
        lblPass->setText(QCoreApplication::translate("LoginWindow", "Contrase\303\261a de Acceso:", nullptr));
        txtPass->setPlaceholderText(QCoreApplication::translate("LoginWindow", "\342\200\242\342\200\242\342\200\242\342\200\242\342\200\242\342\200\242\342\200\242\342\200\242", nullptr));
        lblPinCajero->setText(QCoreApplication::translate("LoginWindow", "PIN de Seguridad de Turno (4 d\303\255gitos):", nullptr));
        txtPinCajero->setPlaceholderText(QCoreApplication::translate("LoginWindow", "Ej. 1234", nullptr));
        lblTokenDev->setText(QCoreApplication::translate("LoginWindow", "Token de Autorizaci\303\263n de Desarrollador (Key):", nullptr));
        txtTokenDev->setPlaceholderText(QCoreApplication::translate("LoginWindow", "DEV-BEREA-2026", nullptr));
        lblNoticeRol->setText(QCoreApplication::translate("LoginWindow", "\342\204\271\357\270\217 Los estudiantes deben registrarse con su correo @bue.edu.ar.", nullptr));
        lblNoticeRol->setStyleSheet(QCoreApplication::translate("LoginWindow", "color: #666; font-size: 11px;", nullptr));
        btnLogin->setText(QCoreApplication::translate("LoginWindow", "INGRESAR AL SISTEMA \342\236\224", nullptr));
        lblNoAccount->setText(QCoreApplication::translate("LoginWindow", "\302\277Eres alumno y no tienes cuenta?", nullptr));
        btnGoRegister->setText(QCoreApplication::translate("LoginWindow", "\302\241Reg\303\255strate aqu\303\255!", nullptr));
    } // retranslateUi

};

namespace Ui {
    class LoginWindow: public Ui_LoginWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGINWINDOW_H
