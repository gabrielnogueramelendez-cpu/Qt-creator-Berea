/********************************************************************************
** Form generated from reading UI file 'comprobantedialog.ui'
**
** Created by: Qt User Interface Compiler version 6.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_COMPROBANTEDIALOG_H
#define UI_COMPROBANTEDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ComprobanteDialog
{
public:
    QVBoxLayout *verticalLayout;
    QFrame *cardReceipt;
    QVBoxLayout *layoutReceipt;
    QLabel *lblLogoTitle;
    QLabel *lblSubtitle;
    QFrame *frameCode;
    QVBoxLayout *layoutCode;
    QLabel *lblCodeLabel;
    QLabel *lblCode;
    QLabel *lblDetails;
    QFrame *frameNotice;
    QVBoxLayout *layoutNotice;
    QLabel *lblRuleNotice;
    QPushButton *btnClose;

    void setupUi(QDialog *ComprobanteDialog)
    {
        if (ComprobanteDialog->objectName().isEmpty())
            ComprobanteDialog->setObjectName("ComprobanteDialog");
        ComprobanteDialog->resize(500, 550);
        verticalLayout = new QVBoxLayout(ComprobanteDialog);
        verticalLayout->setObjectName("verticalLayout");
        cardReceipt = new QFrame(ComprobanteDialog);
        cardReceipt->setObjectName("cardReceipt");
        layoutReceipt = new QVBoxLayout(cardReceipt);
        layoutReceipt->setSpacing(12);
        layoutReceipt->setObjectName("layoutReceipt");
        layoutReceipt->setContentsMargins(20, 20, 20, 20);
        lblLogoTitle = new QLabel(cardReceipt);
        lblLogoTitle->setObjectName("lblLogoTitle");

        layoutReceipt->addWidget(lblLogoTitle, 0, Qt::AlignCenter);

        lblSubtitle = new QLabel(cardReceipt);
        lblSubtitle->setObjectName("lblSubtitle");

        layoutReceipt->addWidget(lblSubtitle, 0, Qt::AlignCenter);

        frameCode = new QFrame(cardReceipt);
        frameCode->setObjectName("frameCode");
        layoutCode = new QVBoxLayout(frameCode);
        layoutCode->setObjectName("layoutCode");
        lblCodeLabel = new QLabel(frameCode);
        lblCodeLabel->setObjectName("lblCodeLabel");

        layoutCode->addWidget(lblCodeLabel, 0, Qt::AlignCenter);

        lblCode = new QLabel(frameCode);
        lblCode->setObjectName("lblCode");

        layoutCode->addWidget(lblCode, 0, Qt::AlignCenter);


        layoutReceipt->addWidget(frameCode, 0, Qt::AlignCenter);

        lblDetails = new QLabel(cardReceipt);
        lblDetails->setObjectName("lblDetails");
        lblDetails->setWordWrap(true);

        layoutReceipt->addWidget(lblDetails);

        frameNotice = new QFrame(cardReceipt);
        frameNotice->setObjectName("frameNotice");
        layoutNotice = new QVBoxLayout(frameNotice);
        layoutNotice->setObjectName("layoutNotice");
        lblRuleNotice = new QLabel(frameNotice);
        lblRuleNotice->setObjectName("lblRuleNotice");
        lblRuleNotice->setWordWrap(true);

        layoutNotice->addWidget(lblRuleNotice);


        layoutReceipt->addWidget(frameNotice);

        btnClose = new QPushButton(cardReceipt);
        btnClose->setObjectName("btnClose");

        layoutReceipt->addWidget(btnClose, 0, Qt::AlignCenter);


        verticalLayout->addWidget(cardReceipt);


        retranslateUi(ComprobanteDialog);

        QMetaObject::connectSlotsByName(ComprobanteDialog);
    } // setupUi

    void retranslateUi(QDialog *ComprobanteDialog)
    {
        ComprobanteDialog->setWindowTitle(QCoreApplication::translate("ComprobanteDialog", "Comprobante de Pedido - El Buffet de Alejandro", nullptr));
        lblLogoTitle->setText(QCoreApplication::translate("ComprobanteDialog", "\360\237\216\211 \302\241PEDIDO CONFIRMADO CON \303\211XITO!", nullptr));
        lblSubtitle->setText(QCoreApplication::translate("ComprobanteDialog", "Muestra o dicta tu c\303\263digo en caja para retirar sin demoras", nullptr));
        frameCode->setStyleSheet(QCoreApplication::translate("ComprobanteDialog", "border: 2px dashed #B8860B; background-color: #FFFDF7; border-radius: 10px; padding: 15px;", nullptr));
        lblCodeLabel->setText(QCoreApplication::translate("ComprobanteDialog", "C\303\223DIGO \303\232NICO DE RETIRO EXPRESS:", nullptr));
        lblCode->setText(QCoreApplication::translate("ComprobanteDialog", "3C6MB", nullptr));
        lblCode->setStyleSheet(QCoreApplication::translate("ComprobanteDialog", "font-size: 32px; font-weight: bold; color: #3D1A24; letter-spacing: 4px;", nullptr));
        lblDetails->setText(QCoreApplication::translate("ComprobanteDialog", "\360\237\223\213 RESUMEN DE COMPRA:\n"
"- Estudiante: Jorge Dahan (4\302\260 A\303\261o B)\n"
"- Horario de Retiro: Primer Recreo (09:50 hs)\n"
"- Estado: PENDIENTE DE RETIRO EN MOSTRADOR\n"
"- Total Abonado: $2.800", nullptr));
        frameNotice->setStyleSheet(QCoreApplication::translate("ComprobanteDialog", "background-color: #F8F4EF; border: 1px solid #E6DEC3; border-radius: 8px; padding: 10px;", nullptr));
        lblRuleNotice->setText(QCoreApplication::translate("ComprobanteDialog", "\360\237\223\261 Cumplimiento de Norma Escolar: No utilices el celular en clase. Solo necesitas dictar el c\303\263digo \"3C6MB\" en la caja para retirar tu pedido al instante.", nullptr));
        btnClose->setText(QCoreApplication::translate("ComprobanteDialog", "\302\241Entendido! Volver al Inicio \342\236\224", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ComprobanteDialog: public Ui_ComprobanteDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_COMPROBANTEDIALOG_H
