/********************************************************************************
** Form generated from reading UI file 'registerdialog.ui'
**
** Created by: Qt User Interface Compiler version 6.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REGISTERDIALOG_H
#define UI_REGISTERDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_RegisterDialog
{
public:
    QVBoxLayout *verticalLayout;
    QFrame *cardRegister;
    QVBoxLayout *layoutRegForm;
    QLabel *lblRegTitle;
    QLabel *lblRegNombre;
    QLineEdit *txtRegNombre;
    QLabel *lblRegApellido;
    QLineEdit *txtRegApellido;
    QLabel *lblRegDni;
    QLineEdit *txtRegDni;
    QLabel *lblRegCurso;
    QLineEdit *txtRegCurso;
    QLabel *lblRegEmail;
    QLineEdit *txtRegEmail;
    QLabel *lblRegPass;
    QLineEdit *txtRegPass;
    QPushButton *btnRegisterConfirm;

    void setupUi(QDialog *RegisterDialog)
    {
        if (RegisterDialog->objectName().isEmpty())
            RegisterDialog->setObjectName("RegisterDialog");
        RegisterDialog->resize(500, 550);
        verticalLayout = new QVBoxLayout(RegisterDialog);
        verticalLayout->setObjectName("verticalLayout");
        cardRegister = new QFrame(RegisterDialog);
        cardRegister->setObjectName("cardRegister");
        layoutRegForm = new QVBoxLayout(cardRegister);
        layoutRegForm->setSpacing(10);
        layoutRegForm->setObjectName("layoutRegForm");
        layoutRegForm->setContentsMargins(20, 20, 20, 20);
        lblRegTitle = new QLabel(cardRegister);
        lblRegTitle->setObjectName("lblRegTitle");

        layoutRegForm->addWidget(lblRegTitle);

        lblRegNombre = new QLabel(cardRegister);
        lblRegNombre->setObjectName("lblRegNombre");

        layoutRegForm->addWidget(lblRegNombre);

        txtRegNombre = new QLineEdit(cardRegister);
        txtRegNombre->setObjectName("txtRegNombre");

        layoutRegForm->addWidget(txtRegNombre);

        lblRegApellido = new QLabel(cardRegister);
        lblRegApellido->setObjectName("lblRegApellido");

        layoutRegForm->addWidget(lblRegApellido);

        txtRegApellido = new QLineEdit(cardRegister);
        txtRegApellido->setObjectName("txtRegApellido");

        layoutRegForm->addWidget(txtRegApellido);

        lblRegDni = new QLabel(cardRegister);
        lblRegDni->setObjectName("lblRegDni");

        layoutRegForm->addWidget(lblRegDni);

        txtRegDni = new QLineEdit(cardRegister);
        txtRegDni->setObjectName("txtRegDni");

        layoutRegForm->addWidget(txtRegDni);

        lblRegCurso = new QLabel(cardRegister);
        lblRegCurso->setObjectName("lblRegCurso");

        layoutRegForm->addWidget(lblRegCurso);

        txtRegCurso = new QLineEdit(cardRegister);
        txtRegCurso->setObjectName("txtRegCurso");

        layoutRegForm->addWidget(txtRegCurso);

        lblRegEmail = new QLabel(cardRegister);
        lblRegEmail->setObjectName("lblRegEmail");

        layoutRegForm->addWidget(lblRegEmail);

        txtRegEmail = new QLineEdit(cardRegister);
        txtRegEmail->setObjectName("txtRegEmail");

        layoutRegForm->addWidget(txtRegEmail);

        lblRegPass = new QLabel(cardRegister);
        lblRegPass->setObjectName("lblRegPass");

        layoutRegForm->addWidget(lblRegPass);

        txtRegPass = new QLineEdit(cardRegister);
        txtRegPass->setObjectName("txtRegPass");
        txtRegPass->setEchoMode(QLineEdit::Password);

        layoutRegForm->addWidget(txtRegPass);

        btnRegisterConfirm = new QPushButton(cardRegister);
        btnRegisterConfirm->setObjectName("btnRegisterConfirm");

        layoutRegForm->addWidget(btnRegisterConfirm);


        verticalLayout->addWidget(cardRegister);


        retranslateUi(RegisterDialog);

        QMetaObject::connectSlotsByName(RegisterDialog);
    } // setupUi

    void retranslateUi(QDialog *RegisterDialog)
    {
        RegisterDialog->setWindowTitle(QCoreApplication::translate("RegisterDialog", "Registro de Alumno - El Buffet de Alejandro", nullptr));
        lblRegTitle->setText(QCoreApplication::translate("RegisterDialog", "\360\237\223\235 REGISTRO DE NUEVO ESTUDIANTE", nullptr));
        lblRegNombre->setText(QCoreApplication::translate("RegisterDialog", "Nombre del Alumno:", nullptr));
        txtRegNombre->setPlaceholderText(QCoreApplication::translate("RegisterDialog", "Ej. Jorge", nullptr));
        lblRegApellido->setText(QCoreApplication::translate("RegisterDialog", "Apellido del Alumno:", nullptr));
        txtRegApellido->setPlaceholderText(QCoreApplication::translate("RegisterDialog", "Ej. Dahan", nullptr));
        lblRegDni->setText(QCoreApplication::translate("RegisterDialog", "DNI / Documento:", nullptr));
        txtRegDni->setPlaceholderText(QCoreApplication::translate("RegisterDialog", "Ej. 45123890", nullptr));
        lblRegCurso->setText(QCoreApplication::translate("RegisterDialog", "Curso y Divisi\303\263n:", nullptr));
        txtRegCurso->setPlaceholderText(QCoreApplication::translate("RegisterDialog", "Ej. 4\302\260 A\303\261o B", nullptr));
        lblRegEmail->setText(QCoreApplication::translate("RegisterDialog", "Correo Institucional:", nullptr));
        txtRegEmail->setPlaceholderText(QCoreApplication::translate("RegisterDialog", "alumno@colegio.edu.ar", nullptr));
        lblRegPass->setText(QCoreApplication::translate("RegisterDialog", "Contrase\303\261a:", nullptr));
        txtRegPass->setPlaceholderText(QCoreApplication::translate("RegisterDialog", "\342\200\242\342\200\242\342\200\242\342\200\242\342\200\242\342\200\242\342\200\242\342\200\242", nullptr));
        btnRegisterConfirm->setText(QCoreApplication::translate("RegisterDialog", "\342\234\205 COMPLETAR REGISTRO", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RegisterDialog: public Ui_RegisterDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REGISTERDIALOG_H
