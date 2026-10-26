/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayoutMain;
    QFrame *headerFrame;
    QHBoxLayout *layoutHeader;
    QLabel *lblHeaderTitle;
    QHBoxLayout *layoutUserBox;
    QLabel *lblUserGreeting;
    QPushButton *btnLogout;
    QTabWidget *tabWidget;
    QWidget *tabMenuDia;
    QVBoxLayout *layoutTabMenuDiaContainer;
    QScrollArea *scrollMenuDia;
    QWidget *scrollMenuDiaContents;
    QVBoxLayout *layoutTabMenuDia;
    QFrame *cardMenuDia1;
    QVBoxLayout *layout_cardMenuDia1;
    QHBoxLayout *header_cardMenuDia1;
    QLabel *lblTitleMenuDia1;
    QLabel *lblPriceMenuDia1;
    QLabel *lblDescMenuDia1;
    QPushButton *btnVerDetalleMenuDia1;
    QFrame *cardMenuDia2;
    QVBoxLayout *layout_cardMenuDia2;
    QHBoxLayout *header_cardMenuDia2;
    QLabel *lblTitleMenuDia2;
    QLabel *lblPriceMenuDia2;
    QLabel *lblDescMenuDia2;
    QPushButton *btnVerDetalleMenuDia2;
    QWidget *tabSalados;
    QVBoxLayout *layoutTabSaladosContainer;
    QScrollArea *scrollSalados;
    QWidget *scrollSaladosContents;
    QVBoxLayout *layoutTabSalados;
    QFrame *cardMilanesa;
    QVBoxLayout *layout_cardMilanesa;
    QHBoxLayout *header_cardMilanesa;
    QLabel *lblTitleMilanesa;
    QLabel *lblPriceMilanesa;
    QLabel *lblDescMilanesa;
    QPushButton *btnVerDetalleMilanesa;
    QFrame *cardEmpanada;
    QVBoxLayout *layout_cardEmpanada;
    QHBoxLayout *header_cardEmpanada;
    QLabel *lblTitleEmpanada;
    QLabel *lblPriceEmpanada;
    QLabel *lblDescEmpanada;
    QPushButton *btnVerDetalleEmpanada;
    QFrame *cardTarta;
    QVBoxLayout *layout_cardTarta;
    QHBoxLayout *header_cardTarta;
    QLabel *lblTitleTarta;
    QLabel *lblPriceTarta;
    QLabel *lblDescTarta;
    QPushButton *btnVerDetalleTarta;
    QFrame *cardPebete;
    QVBoxLayout *layout_cardPebete;
    QHBoxLayout *header_cardPebete;
    QLabel *lblTitlePebete;
    QLabel *lblPricePebete;
    QLabel *lblDescPebete;
    QPushButton *btnVerDetallePebete;
    QWidget *tabDulces;
    QVBoxLayout *layoutTabDulcesContainer;
    QScrollArea *scrollDulces;
    QWidget *scrollDulcesContents;
    QVBoxLayout *layoutTabDulces;
    QFrame *cardAlfajor;
    QVBoxLayout *layout_cardAlfajor;
    QHBoxLayout *header_cardAlfajor;
    QLabel *lblTitleAlfajor;
    QLabel *lblPriceAlfajor;
    QLabel *lblDescAlfajor;
    QPushButton *btnVerDetalleAlfajor;
    QFrame *cardPastafrola;
    QVBoxLayout *layout_cardPastafrola;
    QHBoxLayout *header_cardPastafrola;
    QLabel *lblTitlePastafrola;
    QLabel *lblPricePastafrola;
    QLabel *lblDescPastafrola;
    QPushButton *btnVerDetallePastafrola;
    QFrame *cardFactura;
    QVBoxLayout *layout_cardFactura;
    QHBoxLayout *header_cardFactura;
    QLabel *lblTitleFactura;
    QLabel *lblPriceFactura;
    QLabel *lblDescFactura;
    QPushButton *btnVerDetalleFactura;
    QFrame *cardMuffin;
    QVBoxLayout *layout_cardMuffin;
    QHBoxLayout *header_cardMuffin;
    QLabel *lblTitleMuffin;
    QLabel *lblPriceMuffin;
    QLabel *lblDescMuffin;
    QPushButton *btnVerDetalleMuffin;
    QWidget *tabBebidas;
    QVBoxLayout *layoutTabBebidasContainer;
    QScrollArea *scrollBebidas;
    QWidget *scrollBebidasContents;
    QVBoxLayout *layoutTabBebidas;
    QFrame *cardGaseosa;
    QVBoxLayout *layout_cardGaseosa;
    QHBoxLayout *header_cardGaseosa;
    QLabel *lblTitleGaseosa;
    QLabel *lblPriceGaseosa;
    QLabel *lblDescGaseosa;
    QPushButton *btnVerDetalleGaseosa;
    QFrame *cardAgua;
    QVBoxLayout *layout_cardAgua;
    QHBoxLayout *header_cardAgua;
    QLabel *lblTitleAgua;
    QLabel *lblPriceAgua;
    QLabel *lblDescAgua;
    QPushButton *btnVerDetalleAgua;
    QFrame *cardJugo;
    QVBoxLayout *layout_cardJugo;
    QHBoxLayout *header_cardJugo;
    QLabel *lblTitleJugo;
    QLabel *lblPriceJugo;
    QLabel *lblDescJugo;
    QPushButton *btnVerDetalleJugo;
    QFrame *cardLicuado;
    QVBoxLayout *layout_cardLicuado;
    QHBoxLayout *header_cardLicuado;
    QLabel *lblTitleLicuado;
    QLabel *lblPriceLicuado;
    QLabel *lblDescLicuado;
    QPushButton *btnVerDetalleLicuado;
    QWidget *tabCarrito;
    QVBoxLayout *layoutTabCarrito;
    QFrame *cardNotice;
    QVBoxLayout *layoutNoticeCart;
    QLabel *lblRuleNoticeCart;
    QTableWidget *tableCart;
    QHBoxLayout *layoutCartBottom;
    QLabel *lblSelectRecreo;
    QComboBox *comboRecreo;
    QLabel *lblCartTotal;
    QPushButton *btnCheckout;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1050, 720);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayoutMain = new QVBoxLayout(centralwidget);
        verticalLayoutMain->setSpacing(12);
        verticalLayoutMain->setObjectName("verticalLayoutMain");
        verticalLayoutMain->setContentsMargins(15, 15, 15, 15);
        headerFrame = new QFrame(centralwidget);
        headerFrame->setObjectName("headerFrame");
        layoutHeader = new QHBoxLayout(headerFrame);
        layoutHeader->setObjectName("layoutHeader");
        lblHeaderTitle = new QLabel(headerFrame);
        lblHeaderTitle->setObjectName("lblHeaderTitle");

        layoutHeader->addWidget(lblHeaderTitle);

        layoutUserBox = new QHBoxLayout();
        layoutUserBox->setObjectName("layoutUserBox");
        lblUserGreeting = new QLabel(headerFrame);
        lblUserGreeting->setObjectName("lblUserGreeting");

        layoutUserBox->addWidget(lblUserGreeting);

        btnLogout = new QPushButton(headerFrame);
        btnLogout->setObjectName("btnLogout");

        layoutUserBox->addWidget(btnLogout);


        layoutHeader->addLayout(layoutUserBox);


        verticalLayoutMain->addWidget(headerFrame);

        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName("tabWidget");
        tabMenuDia = new QWidget();
        tabMenuDia->setObjectName("tabMenuDia");
        layoutTabMenuDiaContainer = new QVBoxLayout(tabMenuDia);
        layoutTabMenuDiaContainer->setObjectName("layoutTabMenuDiaContainer");
        scrollMenuDia = new QScrollArea(tabMenuDia);
        scrollMenuDia->setObjectName("scrollMenuDia");
        scrollMenuDia->setWidgetResizable(true);
        scrollMenuDiaContents = new QWidget();
        scrollMenuDiaContents->setObjectName("scrollMenuDiaContents");
        layoutTabMenuDia = new QVBoxLayout(scrollMenuDiaContents);
        layoutTabMenuDia->setSpacing(12);
        layoutTabMenuDia->setObjectName("layoutTabMenuDia");
        cardMenuDia1 = new QFrame(scrollMenuDiaContents);
        cardMenuDia1->setObjectName("cardMenuDia1");
        layout_cardMenuDia1 = new QVBoxLayout(cardMenuDia1);
        layout_cardMenuDia1->setSpacing(8);
        layout_cardMenuDia1->setObjectName("layout_cardMenuDia1");
        layout_cardMenuDia1->setContentsMargins(15, 15, 15, 15);
        header_cardMenuDia1 = new QHBoxLayout();
        header_cardMenuDia1->setObjectName("header_cardMenuDia1");
        lblTitleMenuDia1 = new QLabel(cardMenuDia1);
        lblTitleMenuDia1->setObjectName("lblTitleMenuDia1");

        header_cardMenuDia1->addWidget(lblTitleMenuDia1);

        lblPriceMenuDia1 = new QLabel(cardMenuDia1);
        lblPriceMenuDia1->setObjectName("lblPriceMenuDia1");

        header_cardMenuDia1->addWidget(lblPriceMenuDia1, 0, Qt::AlignRight);


        layout_cardMenuDia1->addLayout(header_cardMenuDia1);

        lblDescMenuDia1 = new QLabel(cardMenuDia1);
        lblDescMenuDia1->setObjectName("lblDescMenuDia1");
        lblDescMenuDia1->setWordWrap(true);

        layout_cardMenuDia1->addWidget(lblDescMenuDia1);

        btnVerDetalleMenuDia1 = new QPushButton(cardMenuDia1);
        btnVerDetalleMenuDia1->setObjectName("btnVerDetalleMenuDia1");

        layout_cardMenuDia1->addWidget(btnVerDetalleMenuDia1, 0, Qt::AlignRight);


        layoutTabMenuDia->addWidget(cardMenuDia1);

        cardMenuDia2 = new QFrame(scrollMenuDiaContents);
        cardMenuDia2->setObjectName("cardMenuDia2");
        layout_cardMenuDia2 = new QVBoxLayout(cardMenuDia2);
        layout_cardMenuDia2->setSpacing(8);
        layout_cardMenuDia2->setObjectName("layout_cardMenuDia2");
        layout_cardMenuDia2->setContentsMargins(15, 15, 15, 15);
        header_cardMenuDia2 = new QHBoxLayout();
        header_cardMenuDia2->setObjectName("header_cardMenuDia2");
        lblTitleMenuDia2 = new QLabel(cardMenuDia2);
        lblTitleMenuDia2->setObjectName("lblTitleMenuDia2");

        header_cardMenuDia2->addWidget(lblTitleMenuDia2);

        lblPriceMenuDia2 = new QLabel(cardMenuDia2);
        lblPriceMenuDia2->setObjectName("lblPriceMenuDia2");

        header_cardMenuDia2->addWidget(lblPriceMenuDia2, 0, Qt::AlignRight);


        layout_cardMenuDia2->addLayout(header_cardMenuDia2);

        lblDescMenuDia2 = new QLabel(cardMenuDia2);
        lblDescMenuDia2->setObjectName("lblDescMenuDia2");
        lblDescMenuDia2->setWordWrap(true);

        layout_cardMenuDia2->addWidget(lblDescMenuDia2);

        btnVerDetalleMenuDia2 = new QPushButton(cardMenuDia2);
        btnVerDetalleMenuDia2->setObjectName("btnVerDetalleMenuDia2");

        layout_cardMenuDia2->addWidget(btnVerDetalleMenuDia2, 0, Qt::AlignRight);


        layoutTabMenuDia->addWidget(cardMenuDia2);

        scrollMenuDia->setWidget(scrollMenuDiaContents);

        layoutTabMenuDiaContainer->addWidget(scrollMenuDia);

        tabWidget->addTab(tabMenuDia, QString());
        tabSalados = new QWidget();
        tabSalados->setObjectName("tabSalados");
        layoutTabSaladosContainer = new QVBoxLayout(tabSalados);
        layoutTabSaladosContainer->setObjectName("layoutTabSaladosContainer");
        scrollSalados = new QScrollArea(tabSalados);
        scrollSalados->setObjectName("scrollSalados");
        scrollSalados->setWidgetResizable(true);
        scrollSaladosContents = new QWidget();
        scrollSaladosContents->setObjectName("scrollSaladosContents");
        layoutTabSalados = new QVBoxLayout(scrollSaladosContents);
        layoutTabSalados->setSpacing(12);
        layoutTabSalados->setObjectName("layoutTabSalados");
        cardMilanesa = new QFrame(scrollSaladosContents);
        cardMilanesa->setObjectName("cardMilanesa");
        layout_cardMilanesa = new QVBoxLayout(cardMilanesa);
        layout_cardMilanesa->setSpacing(8);
        layout_cardMilanesa->setObjectName("layout_cardMilanesa");
        layout_cardMilanesa->setContentsMargins(15, 15, 15, 15);
        header_cardMilanesa = new QHBoxLayout();
        header_cardMilanesa->setObjectName("header_cardMilanesa");
        lblTitleMilanesa = new QLabel(cardMilanesa);
        lblTitleMilanesa->setObjectName("lblTitleMilanesa");

        header_cardMilanesa->addWidget(lblTitleMilanesa);

        lblPriceMilanesa = new QLabel(cardMilanesa);
        lblPriceMilanesa->setObjectName("lblPriceMilanesa");

        header_cardMilanesa->addWidget(lblPriceMilanesa, 0, Qt::AlignRight);


        layout_cardMilanesa->addLayout(header_cardMilanesa);

        lblDescMilanesa = new QLabel(cardMilanesa);
        lblDescMilanesa->setObjectName("lblDescMilanesa");
        lblDescMilanesa->setWordWrap(true);

        layout_cardMilanesa->addWidget(lblDescMilanesa);

        btnVerDetalleMilanesa = new QPushButton(cardMilanesa);
        btnVerDetalleMilanesa->setObjectName("btnVerDetalleMilanesa");

        layout_cardMilanesa->addWidget(btnVerDetalleMilanesa, 0, Qt::AlignRight);


        layoutTabSalados->addWidget(cardMilanesa);

        cardEmpanada = new QFrame(scrollSaladosContents);
        cardEmpanada->setObjectName("cardEmpanada");
        layout_cardEmpanada = new QVBoxLayout(cardEmpanada);
        layout_cardEmpanada->setSpacing(8);
        layout_cardEmpanada->setObjectName("layout_cardEmpanada");
        layout_cardEmpanada->setContentsMargins(15, 15, 15, 15);
        header_cardEmpanada = new QHBoxLayout();
        header_cardEmpanada->setObjectName("header_cardEmpanada");
        lblTitleEmpanada = new QLabel(cardEmpanada);
        lblTitleEmpanada->setObjectName("lblTitleEmpanada");

        header_cardEmpanada->addWidget(lblTitleEmpanada);

        lblPriceEmpanada = new QLabel(cardEmpanada);
        lblPriceEmpanada->setObjectName("lblPriceEmpanada");

        header_cardEmpanada->addWidget(lblPriceEmpanada, 0, Qt::AlignRight);


        layout_cardEmpanada->addLayout(header_cardEmpanada);

        lblDescEmpanada = new QLabel(cardEmpanada);
        lblDescEmpanada->setObjectName("lblDescEmpanada");
        lblDescEmpanada->setWordWrap(true);

        layout_cardEmpanada->addWidget(lblDescEmpanada);

        btnVerDetalleEmpanada = new QPushButton(cardEmpanada);
        btnVerDetalleEmpanada->setObjectName("btnVerDetalleEmpanada");

        layout_cardEmpanada->addWidget(btnVerDetalleEmpanada, 0, Qt::AlignRight);


        layoutTabSalados->addWidget(cardEmpanada);

        cardTarta = new QFrame(scrollSaladosContents);
        cardTarta->setObjectName("cardTarta");
        layout_cardTarta = new QVBoxLayout(cardTarta);
        layout_cardTarta->setSpacing(8);
        layout_cardTarta->setObjectName("layout_cardTarta");
        layout_cardTarta->setContentsMargins(15, 15, 15, 15);
        header_cardTarta = new QHBoxLayout();
        header_cardTarta->setObjectName("header_cardTarta");
        lblTitleTarta = new QLabel(cardTarta);
        lblTitleTarta->setObjectName("lblTitleTarta");

        header_cardTarta->addWidget(lblTitleTarta);

        lblPriceTarta = new QLabel(cardTarta);
        lblPriceTarta->setObjectName("lblPriceTarta");

        header_cardTarta->addWidget(lblPriceTarta, 0, Qt::AlignRight);


        layout_cardTarta->addLayout(header_cardTarta);

        lblDescTarta = new QLabel(cardTarta);
        lblDescTarta->setObjectName("lblDescTarta");
        lblDescTarta->setWordWrap(true);

        layout_cardTarta->addWidget(lblDescTarta);

        btnVerDetalleTarta = new QPushButton(cardTarta);
        btnVerDetalleTarta->setObjectName("btnVerDetalleTarta");

        layout_cardTarta->addWidget(btnVerDetalleTarta, 0, Qt::AlignRight);


        layoutTabSalados->addWidget(cardTarta);

        cardPebete = new QFrame(scrollSaladosContents);
        cardPebete->setObjectName("cardPebete");
        layout_cardPebete = new QVBoxLayout(cardPebete);
        layout_cardPebete->setSpacing(8);
        layout_cardPebete->setObjectName("layout_cardPebete");
        layout_cardPebete->setContentsMargins(15, 15, 15, 15);
        header_cardPebete = new QHBoxLayout();
        header_cardPebete->setObjectName("header_cardPebete");
        lblTitlePebete = new QLabel(cardPebete);
        lblTitlePebete->setObjectName("lblTitlePebete");

        header_cardPebete->addWidget(lblTitlePebete);

        lblPricePebete = new QLabel(cardPebete);
        lblPricePebete->setObjectName("lblPricePebete");

        header_cardPebete->addWidget(lblPricePebete, 0, Qt::AlignRight);


        layout_cardPebete->addLayout(header_cardPebete);

        lblDescPebete = new QLabel(cardPebete);
        lblDescPebete->setObjectName("lblDescPebete");
        lblDescPebete->setWordWrap(true);

        layout_cardPebete->addWidget(lblDescPebete);

        btnVerDetallePebete = new QPushButton(cardPebete);
        btnVerDetallePebete->setObjectName("btnVerDetallePebete");

        layout_cardPebete->addWidget(btnVerDetallePebete, 0, Qt::AlignRight);


        layoutTabSalados->addWidget(cardPebete);

        scrollSalados->setWidget(scrollSaladosContents);

        layoutTabSaladosContainer->addWidget(scrollSalados);

        tabWidget->addTab(tabSalados, QString());
        tabDulces = new QWidget();
        tabDulces->setObjectName("tabDulces");
        layoutTabDulcesContainer = new QVBoxLayout(tabDulces);
        layoutTabDulcesContainer->setObjectName("layoutTabDulcesContainer");
        scrollDulces = new QScrollArea(tabDulces);
        scrollDulces->setObjectName("scrollDulces");
        scrollDulces->setWidgetResizable(true);
        scrollDulcesContents = new QWidget();
        scrollDulcesContents->setObjectName("scrollDulcesContents");
        layoutTabDulces = new QVBoxLayout(scrollDulcesContents);
        layoutTabDulces->setSpacing(12);
        layoutTabDulces->setObjectName("layoutTabDulces");
        cardAlfajor = new QFrame(scrollDulcesContents);
        cardAlfajor->setObjectName("cardAlfajor");
        layout_cardAlfajor = new QVBoxLayout(cardAlfajor);
        layout_cardAlfajor->setSpacing(8);
        layout_cardAlfajor->setObjectName("layout_cardAlfajor");
        layout_cardAlfajor->setContentsMargins(15, 15, 15, 15);
        header_cardAlfajor = new QHBoxLayout();
        header_cardAlfajor->setObjectName("header_cardAlfajor");
        lblTitleAlfajor = new QLabel(cardAlfajor);
        lblTitleAlfajor->setObjectName("lblTitleAlfajor");

        header_cardAlfajor->addWidget(lblTitleAlfajor);

        lblPriceAlfajor = new QLabel(cardAlfajor);
        lblPriceAlfajor->setObjectName("lblPriceAlfajor");

        header_cardAlfajor->addWidget(lblPriceAlfajor, 0, Qt::AlignRight);


        layout_cardAlfajor->addLayout(header_cardAlfajor);

        lblDescAlfajor = new QLabel(cardAlfajor);
        lblDescAlfajor->setObjectName("lblDescAlfajor");
        lblDescAlfajor->setWordWrap(true);

        layout_cardAlfajor->addWidget(lblDescAlfajor);

        btnVerDetalleAlfajor = new QPushButton(cardAlfajor);
        btnVerDetalleAlfajor->setObjectName("btnVerDetalleAlfajor");

        layout_cardAlfajor->addWidget(btnVerDetalleAlfajor, 0, Qt::AlignRight);


        layoutTabDulces->addWidget(cardAlfajor);

        cardPastafrola = new QFrame(scrollDulcesContents);
        cardPastafrola->setObjectName("cardPastafrola");
        layout_cardPastafrola = new QVBoxLayout(cardPastafrola);
        layout_cardPastafrola->setSpacing(8);
        layout_cardPastafrola->setObjectName("layout_cardPastafrola");
        layout_cardPastafrola->setContentsMargins(15, 15, 15, 15);
        header_cardPastafrola = new QHBoxLayout();
        header_cardPastafrola->setObjectName("header_cardPastafrola");
        lblTitlePastafrola = new QLabel(cardPastafrola);
        lblTitlePastafrola->setObjectName("lblTitlePastafrola");

        header_cardPastafrola->addWidget(lblTitlePastafrola);

        lblPricePastafrola = new QLabel(cardPastafrola);
        lblPricePastafrola->setObjectName("lblPricePastafrola");

        header_cardPastafrola->addWidget(lblPricePastafrola, 0, Qt::AlignRight);


        layout_cardPastafrola->addLayout(header_cardPastafrola);

        lblDescPastafrola = new QLabel(cardPastafrola);
        lblDescPastafrola->setObjectName("lblDescPastafrola");
        lblDescPastafrola->setWordWrap(true);

        layout_cardPastafrola->addWidget(lblDescPastafrola);

        btnVerDetallePastafrola = new QPushButton(cardPastafrola);
        btnVerDetallePastafrola->setObjectName("btnVerDetallePastafrola");

        layout_cardPastafrola->addWidget(btnVerDetallePastafrola, 0, Qt::AlignRight);


        layoutTabDulces->addWidget(cardPastafrola);

        cardFactura = new QFrame(scrollDulcesContents);
        cardFactura->setObjectName("cardFactura");
        layout_cardFactura = new QVBoxLayout(cardFactura);
        layout_cardFactura->setSpacing(8);
        layout_cardFactura->setObjectName("layout_cardFactura");
        layout_cardFactura->setContentsMargins(15, 15, 15, 15);
        header_cardFactura = new QHBoxLayout();
        header_cardFactura->setObjectName("header_cardFactura");
        lblTitleFactura = new QLabel(cardFactura);
        lblTitleFactura->setObjectName("lblTitleFactura");

        header_cardFactura->addWidget(lblTitleFactura);

        lblPriceFactura = new QLabel(cardFactura);
        lblPriceFactura->setObjectName("lblPriceFactura");

        header_cardFactura->addWidget(lblPriceFactura, 0, Qt::AlignRight);


        layout_cardFactura->addLayout(header_cardFactura);

        lblDescFactura = new QLabel(cardFactura);
        lblDescFactura->setObjectName("lblDescFactura");
        lblDescFactura->setWordWrap(true);

        layout_cardFactura->addWidget(lblDescFactura);

        btnVerDetalleFactura = new QPushButton(cardFactura);
        btnVerDetalleFactura->setObjectName("btnVerDetalleFactura");

        layout_cardFactura->addWidget(btnVerDetalleFactura, 0, Qt::AlignRight);


        layoutTabDulces->addWidget(cardFactura);

        cardMuffin = new QFrame(scrollDulcesContents);
        cardMuffin->setObjectName("cardMuffin");
        layout_cardMuffin = new QVBoxLayout(cardMuffin);
        layout_cardMuffin->setSpacing(8);
        layout_cardMuffin->setObjectName("layout_cardMuffin");
        layout_cardMuffin->setContentsMargins(15, 15, 15, 15);
        header_cardMuffin = new QHBoxLayout();
        header_cardMuffin->setObjectName("header_cardMuffin");
        lblTitleMuffin = new QLabel(cardMuffin);
        lblTitleMuffin->setObjectName("lblTitleMuffin");

        header_cardMuffin->addWidget(lblTitleMuffin);

        lblPriceMuffin = new QLabel(cardMuffin);
        lblPriceMuffin->setObjectName("lblPriceMuffin");

        header_cardMuffin->addWidget(lblPriceMuffin, 0, Qt::AlignRight);


        layout_cardMuffin->addLayout(header_cardMuffin);

        lblDescMuffin = new QLabel(cardMuffin);
        lblDescMuffin->setObjectName("lblDescMuffin");
        lblDescMuffin->setWordWrap(true);

        layout_cardMuffin->addWidget(lblDescMuffin);

        btnVerDetalleMuffin = new QPushButton(cardMuffin);
        btnVerDetalleMuffin->setObjectName("btnVerDetalleMuffin");

        layout_cardMuffin->addWidget(btnVerDetalleMuffin, 0, Qt::AlignRight);


        layoutTabDulces->addWidget(cardMuffin);

        scrollDulces->setWidget(scrollDulcesContents);

        layoutTabDulcesContainer->addWidget(scrollDulces);

        tabWidget->addTab(tabDulces, QString());
        tabBebidas = new QWidget();
        tabBebidas->setObjectName("tabBebidas");
        layoutTabBebidasContainer = new QVBoxLayout(tabBebidas);
        layoutTabBebidasContainer->setObjectName("layoutTabBebidasContainer");
        scrollBebidas = new QScrollArea(tabBebidas);
        scrollBebidas->setObjectName("scrollBebidas");
        scrollBebidas->setWidgetResizable(true);
        scrollBebidasContents = new QWidget();
        scrollBebidasContents->setObjectName("scrollBebidasContents");
        layoutTabBebidas = new QVBoxLayout(scrollBebidasContents);
        layoutTabBebidas->setSpacing(12);
        layoutTabBebidas->setObjectName("layoutTabBebidas");
        cardGaseosa = new QFrame(scrollBebidasContents);
        cardGaseosa->setObjectName("cardGaseosa");
        layout_cardGaseosa = new QVBoxLayout(cardGaseosa);
        layout_cardGaseosa->setSpacing(8);
        layout_cardGaseosa->setObjectName("layout_cardGaseosa");
        layout_cardGaseosa->setContentsMargins(15, 15, 15, 15);
        header_cardGaseosa = new QHBoxLayout();
        header_cardGaseosa->setObjectName("header_cardGaseosa");
        lblTitleGaseosa = new QLabel(cardGaseosa);
        lblTitleGaseosa->setObjectName("lblTitleGaseosa");

        header_cardGaseosa->addWidget(lblTitleGaseosa);

        lblPriceGaseosa = new QLabel(cardGaseosa);
        lblPriceGaseosa->setObjectName("lblPriceGaseosa");

        header_cardGaseosa->addWidget(lblPriceGaseosa, 0, Qt::AlignRight);


        layout_cardGaseosa->addLayout(header_cardGaseosa);

        lblDescGaseosa = new QLabel(cardGaseosa);
        lblDescGaseosa->setObjectName("lblDescGaseosa");
        lblDescGaseosa->setWordWrap(true);

        layout_cardGaseosa->addWidget(lblDescGaseosa);

        btnVerDetalleGaseosa = new QPushButton(cardGaseosa);
        btnVerDetalleGaseosa->setObjectName("btnVerDetalleGaseosa");

        layout_cardGaseosa->addWidget(btnVerDetalleGaseosa, 0, Qt::AlignRight);


        layoutTabBebidas->addWidget(cardGaseosa);

        cardAgua = new QFrame(scrollBebidasContents);
        cardAgua->setObjectName("cardAgua");
        layout_cardAgua = new QVBoxLayout(cardAgua);
        layout_cardAgua->setSpacing(8);
        layout_cardAgua->setObjectName("layout_cardAgua");
        layout_cardAgua->setContentsMargins(15, 15, 15, 15);
        header_cardAgua = new QHBoxLayout();
        header_cardAgua->setObjectName("header_cardAgua");
        lblTitleAgua = new QLabel(cardAgua);
        lblTitleAgua->setObjectName("lblTitleAgua");

        header_cardAgua->addWidget(lblTitleAgua);

        lblPriceAgua = new QLabel(cardAgua);
        lblPriceAgua->setObjectName("lblPriceAgua");

        header_cardAgua->addWidget(lblPriceAgua, 0, Qt::AlignRight);


        layout_cardAgua->addLayout(header_cardAgua);

        lblDescAgua = new QLabel(cardAgua);
        lblDescAgua->setObjectName("lblDescAgua");
        lblDescAgua->setWordWrap(true);

        layout_cardAgua->addWidget(lblDescAgua);

        btnVerDetalleAgua = new QPushButton(cardAgua);
        btnVerDetalleAgua->setObjectName("btnVerDetalleAgua");

        layout_cardAgua->addWidget(btnVerDetalleAgua, 0, Qt::AlignRight);


        layoutTabBebidas->addWidget(cardAgua);

        cardJugo = new QFrame(scrollBebidasContents);
        cardJugo->setObjectName("cardJugo");
        layout_cardJugo = new QVBoxLayout(cardJugo);
        layout_cardJugo->setSpacing(8);
        layout_cardJugo->setObjectName("layout_cardJugo");
        layout_cardJugo->setContentsMargins(15, 15, 15, 15);
        header_cardJugo = new QHBoxLayout();
        header_cardJugo->setObjectName("header_cardJugo");
        lblTitleJugo = new QLabel(cardJugo);
        lblTitleJugo->setObjectName("lblTitleJugo");

        header_cardJugo->addWidget(lblTitleJugo);

        lblPriceJugo = new QLabel(cardJugo);
        lblPriceJugo->setObjectName("lblPriceJugo");

        header_cardJugo->addWidget(lblPriceJugo, 0, Qt::AlignRight);


        layout_cardJugo->addLayout(header_cardJugo);

        lblDescJugo = new QLabel(cardJugo);
        lblDescJugo->setObjectName("lblDescJugo");
        lblDescJugo->setWordWrap(true);

        layout_cardJugo->addWidget(lblDescJugo);

        btnVerDetalleJugo = new QPushButton(cardJugo);
        btnVerDetalleJugo->setObjectName("btnVerDetalleJugo");

        layout_cardJugo->addWidget(btnVerDetalleJugo, 0, Qt::AlignRight);


        layoutTabBebidas->addWidget(cardJugo);

        cardLicuado = new QFrame(scrollBebidasContents);
        cardLicuado->setObjectName("cardLicuado");
        layout_cardLicuado = new QVBoxLayout(cardLicuado);
        layout_cardLicuado->setSpacing(8);
        layout_cardLicuado->setObjectName("layout_cardLicuado");
        layout_cardLicuado->setContentsMargins(15, 15, 15, 15);
        header_cardLicuado = new QHBoxLayout();
        header_cardLicuado->setObjectName("header_cardLicuado");
        lblTitleLicuado = new QLabel(cardLicuado);
        lblTitleLicuado->setObjectName("lblTitleLicuado");

        header_cardLicuado->addWidget(lblTitleLicuado);

        lblPriceLicuado = new QLabel(cardLicuado);
        lblPriceLicuado->setObjectName("lblPriceLicuado");

        header_cardLicuado->addWidget(lblPriceLicuado, 0, Qt::AlignRight);


        layout_cardLicuado->addLayout(header_cardLicuado);

        lblDescLicuado = new QLabel(cardLicuado);
        lblDescLicuado->setObjectName("lblDescLicuado");
        lblDescLicuado->setWordWrap(true);

        layout_cardLicuado->addWidget(lblDescLicuado);

        btnVerDetalleLicuado = new QPushButton(cardLicuado);
        btnVerDetalleLicuado->setObjectName("btnVerDetalleLicuado");

        layout_cardLicuado->addWidget(btnVerDetalleLicuado, 0, Qt::AlignRight);


        layoutTabBebidas->addWidget(cardLicuado);

        scrollBebidas->setWidget(scrollBebidasContents);

        layoutTabBebidasContainer->addWidget(scrollBebidas);

        tabWidget->addTab(tabBebidas, QString());
        tabCarrito = new QWidget();
        tabCarrito->setObjectName("tabCarrito");
        layoutTabCarrito = new QVBoxLayout(tabCarrito);
        layoutTabCarrito->setSpacing(12);
        layoutTabCarrito->setObjectName("layoutTabCarrito");
        cardNotice = new QFrame(tabCarrito);
        cardNotice->setObjectName("cardNotice");
        layoutNoticeCart = new QVBoxLayout(cardNotice);
        layoutNoticeCart->setObjectName("layoutNoticeCart");
        lblRuleNoticeCart = new QLabel(cardNotice);
        lblRuleNoticeCart->setObjectName("lblRuleNoticeCart");
        lblRuleNoticeCart->setWordWrap(true);

        layoutNoticeCart->addWidget(lblRuleNoticeCart);


        layoutTabCarrito->addWidget(cardNotice);

        tableCart = new QTableWidget(tabCarrito);
        if (tableCart->columnCount() < 4)
            tableCart->setColumnCount(4);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableCart->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableCart->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableCart->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableCart->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        tableCart->setObjectName("tableCart");
        tableCart->setColumnCount(4);

        layoutTabCarrito->addWidget(tableCart);

        layoutCartBottom = new QHBoxLayout();
        layoutCartBottom->setObjectName("layoutCartBottom");
        lblSelectRecreo = new QLabel(tabCarrito);
        lblSelectRecreo->setObjectName("lblSelectRecreo");

        layoutCartBottom->addWidget(lblSelectRecreo);

        comboRecreo = new QComboBox(tabCarrito);
        comboRecreo->addItem(QString());
        comboRecreo->addItem(QString());
        comboRecreo->addItem(QString());
        comboRecreo->addItem(QString());
        comboRecreo->setObjectName("comboRecreo");

        layoutCartBottom->addWidget(comboRecreo);

        lblCartTotal = new QLabel(tabCarrito);
        lblCartTotal->setObjectName("lblCartTotal");

        layoutCartBottom->addWidget(lblCartTotal, 0, Qt::AlignRight);


        layoutTabCarrito->addLayout(layoutCartBottom);

        btnCheckout = new QPushButton(tabCarrito);
        btnCheckout->setObjectName("btnCheckout");

        layoutTabCarrito->addWidget(btnCheckout);

        tabWidget->addTab(tabCarrito, QString());

        verticalLayoutMain->addWidget(tabWidget);

        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "El Buffet de Alejandro - Men\303\272 Gourmet Escolar", nullptr));
        headerFrame->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: #3D1A24; border-radius: 10px; padding: 10px 15px;", nullptr));
        lblHeaderTitle->setText(QCoreApplication::translate("MainWindow", "\360\237\215\216 EL BUFFET DE ALEJANDRO \342\200\224 Carta Gourmet Escolar", nullptr));
        lblHeaderTitle->setStyleSheet(QCoreApplication::translate("MainWindow", "color: #FFFFFF; font-size: 18px; font-weight: bold;", nullptr));
        lblUserGreeting->setText(QCoreApplication::translate("MainWindow", "\360\237\216\223 Estudiante: Jorge Dahan (4\302\260 A\303\261o B)", nullptr));
        lblUserGreeting->setStyleSheet(QCoreApplication::translate("MainWindow", "color: #D4AF37; font-weight: bold; font-size: 13px;", nullptr));
        btnLogout->setText(QCoreApplication::translate("MainWindow", "\360\237\232\252 Cerrar Sesi\303\263n", nullptr));
        btnLogout->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: #522431; color: #FFFFFF; border-radius: 6px; padding: 6px 12px;", nullptr));
        cardMenuDia1->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: #FFFFFF; border: 1px solid #EAE3D2; border-radius: 10px;", nullptr));
        lblTitleMenuDia1->setText(QCoreApplication::translate("MainWindow", "\360\237\214\237 Combo N\302\260 1: Almuerzo Gourmet Completo", nullptr));
        lblTitleMenuDia1->setStyleSheet(QCoreApplication::translate("MainWindow", "font-size: 16px; font-weight: bold; color: #3D1A24;", nullptr));
        lblPriceMenuDia1->setText(QCoreApplication::translate("MainWindow", "$8.500", nullptr));
        lblPriceMenuDia1->setStyleSheet(QCoreApplication::translate("MainWindow", "font-size: 18px; font-weight: bold; color: #B8860B;", nullptr));
        lblDescMenuDia1->setText(QCoreApplication::translate("MainWindow", "Milanesa a la Napolitana con papas r\303\272sticas al horno + Bebida 500ml a elecci\303\263n + Postre o Fruta. (Encargar antes de las 09:00 hs).", nullptr));
        lblDescMenuDia1->setStyleSheet(QCoreApplication::translate("MainWindow", "font-size: 12px; color: #666666;", nullptr));
        btnVerDetalleMenuDia1->setText(QCoreApplication::translate("MainWindow", "\360\237\221\201\357\270\217 Ver Detalle Completo \342\236\224", nullptr));
        cardMenuDia2->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: #FFFFFF; border: 1px solid #EAE3D2; border-radius: 10px;", nullptr));
        lblTitleMenuDia2->setText(QCoreApplication::translate("MainWindow", "\360\237\245\227 Combo N\302\260 2: Almuerzo Express Saludable", nullptr));
        lblTitleMenuDia2->setStyleSheet(QCoreApplication::translate("MainWindow", "font-size: 16px; font-weight: bold; color: #3D1A24;", nullptr));
        lblPriceMenuDia2->setText(QCoreApplication::translate("MainWindow", "$5.200", nullptr));
        lblPriceMenuDia2->setStyleSheet(QCoreApplication::translate("MainWindow", "font-size: 18px; font-weight: bold; color: #B8860B;", nullptr));
        lblDescMenuDia2->setText(QCoreApplication::translate("MainWindow", "Tarta R\303\272stica de Verdura y Queso + Ensalada fresca de estaci\303\263n + Agua Mineral Villavicencio 500ml.", nullptr));
        lblDescMenuDia2->setStyleSheet(QCoreApplication::translate("MainWindow", "font-size: 12px; color: #666666;", nullptr));
        btnVerDetalleMenuDia2->setText(QCoreApplication::translate("MainWindow", "\360\237\221\201\357\270\217 Ver Detalle Completo \342\236\224", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabMenuDia), QCoreApplication::translate("MainWindow", "\360\237\214\237 Men\303\272 del D\303\255a", nullptr));
        cardMilanesa->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: #FFFFFF; border: 1px solid #EAE3D2; border-radius: 10px;", nullptr));
        lblTitleMilanesa->setText(QCoreApplication::translate("MainWindow", "\360\237\245\252 S\303\241ndwich de Milanesa Ciabatta Completo", nullptr));
        lblTitleMilanesa->setStyleSheet(QCoreApplication::translate("MainWindow", "font-size: 16px; font-weight: bold; color: #3D1A24;", nullptr));
        lblPriceMilanesa->setText(QCoreApplication::translate("MainWindow", "$2.800", nullptr));
        lblPriceMilanesa->setStyleSheet(QCoreApplication::translate("MainWindow", "font-size: 18px; font-weight: bold; color: #B8860B;", nullptr));
        lblDescMilanesa->setText(QCoreApplication::translate("MainWindow", "Milanesa tierna de ternera, lechuga fresca, tomate, queso tybo y aderezo artesanal en pan ciabatta crocante.", nullptr));
        lblDescMilanesa->setStyleSheet(QCoreApplication::translate("MainWindow", "font-size: 12px; color: #666666;", nullptr));
        btnVerDetalleMilanesa->setText(QCoreApplication::translate("MainWindow", "\360\237\221\201\357\270\217 Ver Detalle Completo \342\236\224", nullptr));
        cardEmpanada->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: #FFFFFF; border: 1px solid #EAE3D2; border-radius: 10px;", nullptr));
        lblTitleEmpanada->setText(QCoreApplication::translate("MainWindow", "\360\237\245\237 Empanada de Carne Cortada a Cuchillo", nullptr));
        lblTitleEmpanada->setStyleSheet(QCoreApplication::translate("MainWindow", "font-size: 16px; font-weight: bold; color: #3D1A24;", nullptr));
        lblPriceEmpanada->setText(QCoreApplication::translate("MainWindow", "$1.200", nullptr));
        lblPriceEmpanada->setStyleSheet(QCoreApplication::translate("MainWindow", "font-size: 18px; font-weight: bold; color: #B8860B;", nullptr));
        lblDescEmpanada->setText(QCoreApplication::translate("MainWindow", "Empanada tradicional horneada, rellena de ternera seleccionada cortada a cuchillo y especias criollas.", nullptr));
        lblDescEmpanada->setStyleSheet(QCoreApplication::translate("MainWindow", "font-size: 12px; color: #666666;", nullptr));
        btnVerDetalleEmpanada->setText(QCoreApplication::translate("MainWindow", "\360\237\221\201\357\270\217 Ver Detalle Completo \342\236\224", nullptr));
        cardTarta->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: #FFFFFF; border: 1px solid #EAE3D2; border-radius: 10px;", nullptr));
        lblTitleTarta->setText(QCoreApplication::translate("MainWindow", "\360\237\245\247 Tarta R\303\272stica de Jam\303\263n y Queso", nullptr));
        lblTitleTarta->setStyleSheet(QCoreApplication::translate("MainWindow", "font-size: 16px; font-weight: bold; color: #3D1A24;", nullptr));
        lblPriceTarta->setText(QCoreApplication::translate("MainWindow", "$2.400", nullptr));
        lblPriceTarta->setStyleSheet(QCoreApplication::translate("MainWindow", "font-size: 18px; font-weight: bold; color: #B8860B;", nullptr));
        lblDescTarta->setText(QCoreApplication::translate("MainWindow", "Porci\303\263n abundante de tarta casera con doble capa de queso mozzarella fundido y jam\303\263n cocido de primera.", nullptr));
        lblDescTarta->setStyleSheet(QCoreApplication::translate("MainWindow", "font-size: 12px; color: #666666;", nullptr));
        btnVerDetalleTarta->setText(QCoreApplication::translate("MainWindow", "\360\237\221\201\357\270\217 Ver Detalle Completo \342\236\224", nullptr));
        cardPebete->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: #FFFFFF; border: 1px solid #EAE3D2; border-radius: 10px;", nullptr));
        lblTitlePebete->setText(QCoreApplication::translate("MainWindow", "\360\237\245\226 Pebete Tradicional de Jam\303\263n y Queso", nullptr));
        lblTitlePebete->setStyleSheet(QCoreApplication::translate("MainWindow", "font-size: 16px; font-weight: bold; color: #3D1A24;", nullptr));
        lblPricePebete->setText(QCoreApplication::translate("MainWindow", "$1.800", nullptr));
        lblPricePebete->setStyleSheet(QCoreApplication::translate("MainWindow", "font-size: 18px; font-weight: bold; color: #B8860B;", nullptr));
        lblDescPebete->setText(QCoreApplication::translate("MainWindow", "Pan pebete suave artesanal, abundante jam\303\263n cocido, queso tybo en fetas y manteca de primera calidad.", nullptr));
        lblDescPebete->setStyleSheet(QCoreApplication::translate("MainWindow", "font-size: 12px; color: #666666;", nullptr));
        btnVerDetallePebete->setText(QCoreApplication::translate("MainWindow", "\360\237\221\201\357\270\217 Ver Detalle Completo \342\236\224", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabSalados), QCoreApplication::translate("MainWindow", "\360\237\245\220 Salados Gourmet", nullptr));
        cardAlfajor->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: #FFFFFF; border: 1px solid #EAE3D2; border-radius: 10px;", nullptr));
        lblTitleAlfajor->setText(QCoreApplication::translate("MainWindow", "\360\237\215\253 Alfajor Artesanal de Dulce de Leche", nullptr));
        lblTitleAlfajor->setStyleSheet(QCoreApplication::translate("MainWindow", "font-size: 16px; font-weight: bold; color: #3D1A24;", nullptr));
        lblPriceAlfajor->setText(QCoreApplication::translate("MainWindow", "$1.100", nullptr));
        lblPriceAlfajor->setStyleSheet(QCoreApplication::translate("MainWindow", "font-size: 18px; font-weight: bold; color: #B8860B;", nullptr));
        lblDescAlfajor->setText(QCoreApplication::translate("MainWindow", "Dos tapas suavemente horneadas con relleno abundante de dulce de leche repostero, ba\303\261ado en fino chocolate.", nullptr));
        lblDescAlfajor->setStyleSheet(QCoreApplication::translate("MainWindow", "font-size: 12px; color: #666666;", nullptr));
        btnVerDetalleAlfajor->setText(QCoreApplication::translate("MainWindow", "\360\237\221\201\357\270\217 Ver Detalle Completo \342\236\224", nullptr));
        cardPastafrola->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: #FFFFFF; border: 1px solid #EAE3D2; border-radius: 10px;", nullptr));
        lblTitlePastafrola->setText(QCoreApplication::translate("MainWindow", "\360\237\245\256 Porci\303\263n de Pasta Frola Artesanal", nullptr));
        lblTitlePastafrola->setStyleSheet(QCoreApplication::translate("MainWindow", "font-size: 16px; font-weight: bold; color: #3D1A24;", nullptr));
        lblPricePastafrola->setText(QCoreApplication::translate("MainWindow", "$1.500", nullptr));
        lblPricePastafrola->setStyleSheet(QCoreApplication::translate("MainWindow", "font-size: 18px; font-weight: bold; color: #B8860B;", nullptr));
        lblDescPastafrola->setText(QCoreApplication::translate("MainWindow", "Receta tradicional casera con dulce de membrillo especial y enrejado de masa sabl\303\251e de manteca.", nullptr));
        lblDescPastafrola->setStyleSheet(QCoreApplication::translate("MainWindow", "font-size: 12px; color: #666666;", nullptr));
        btnVerDetallePastafrola->setText(QCoreApplication::translate("MainWindow", "\360\237\221\201\357\270\217 Ver Detalle Completo \342\236\224", nullptr));
        cardFactura->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: #FFFFFF; border: 1px solid #EAE3D2; border-radius: 10px;", nullptr));
        lblTitleFactura->setText(QCoreApplication::translate("MainWindow", "\360\237\245\220 Factura de Manteca Seleccionada", nullptr));
        lblTitleFactura->setStyleSheet(QCoreApplication::translate("MainWindow", "font-size: 16px; font-weight: bold; color: #3D1A24;", nullptr));
        lblPriceFactura->setText(QCoreApplication::translate("MainWindow", "$600", nullptr));
        lblPriceFactura->setStyleSheet(QCoreApplication::translate("MainWindow", "font-size: 18px; font-weight: bold; color: #B8860B;", nullptr));
        lblDescFactura->setText(QCoreApplication::translate("MainWindow", "Medialuna u opci\303\263n hojaldrada artesanal horneada cada ma\303\261ana en el buffet del colegio.", nullptr));
        lblDescFactura->setStyleSheet(QCoreApplication::translate("MainWindow", "font-size: 12px; color: #666666;", nullptr));
        btnVerDetalleFactura->setText(QCoreApplication::translate("MainWindow", "\360\237\221\201\357\270\217 Ver Detalle Completo \342\236\224", nullptr));
        cardMuffin->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: #FFFFFF; border: 1px solid #EAE3D2; border-radius: 10px;", nullptr));
        lblTitleMuffin->setText(QCoreApplication::translate("MainWindow", "\360\237\247\201 Muffin de Chocolate con Coraz\303\263n Dulce", nullptr));
        lblTitleMuffin->setStyleSheet(QCoreApplication::translate("MainWindow", "font-size: 16px; font-weight: bold; color: #3D1A24;", nullptr));
        lblPriceMuffin->setText(QCoreApplication::translate("MainWindow", "$1.400", nullptr));
        lblPriceMuffin->setStyleSheet(QCoreApplication::translate("MainWindow", "font-size: 18px; font-weight: bold; color: #B8860B;", nullptr));
        lblDescMuffin->setText(QCoreApplication::translate("MainWindow", "Esponjoso muffin de cacao intenso con chispas de chocolate y coraz\303\263n de dulce de leche.", nullptr));
        lblDescMuffin->setStyleSheet(QCoreApplication::translate("MainWindow", "font-size: 12px; color: #666666;", nullptr));
        btnVerDetalleMuffin->setText(QCoreApplication::translate("MainWindow", "\360\237\221\201\357\270\217 Ver Detalle Completo \342\236\224", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabDulces), QCoreApplication::translate("MainWindow", "\360\237\215\252 Dulces & Reposter\303\255a", nullptr));
        cardGaseosa->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: #FFFFFF; border: 1px solid #EAE3D2; border-radius: 10px;", nullptr));
        lblTitleGaseosa->setText(QCoreApplication::translate("MainWindow", "\360\237\245\244 Gaseosa L\303\255nea Coca-Cola 500ml", nullptr));
        lblTitleGaseosa->setStyleSheet(QCoreApplication::translate("MainWindow", "font-size: 16px; font-weight: bold; color: #3D1A24;", nullptr));
        lblPriceGaseosa->setText(QCoreApplication::translate("MainWindow", "$1.300", nullptr));
        lblPriceGaseosa->setStyleSheet(QCoreApplication::translate("MainWindow", "font-size: 18px; font-weight: bold; color: #B8860B;", nullptr));
        lblDescGaseosa->setText(QCoreApplication::translate("MainWindow", "Botella de 500ml helada (Coca-Cola, Sprite o Fanta a elecci\303\263n al retirar en mostrador).", nullptr));
        lblDescGaseosa->setStyleSheet(QCoreApplication::translate("MainWindow", "font-size: 12px; color: #666666;", nullptr));
        btnVerDetalleGaseosa->setText(QCoreApplication::translate("MainWindow", "\360\237\221\201\357\270\217 Ver Detalle Completo \342\236\224", nullptr));
        cardAgua->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: #FFFFFF; border: 1px solid #EAE3D2; border-radius: 10px;", nullptr));
        lblTitleAgua->setText(QCoreApplication::translate("MainWindow", "\360\237\222\247 Agua Mineral Villavicencio 500ml", nullptr));
        lblTitleAgua->setStyleSheet(QCoreApplication::translate("MainWindow", "font-size: 16px; font-weight: bold; color: #3D1A24;", nullptr));
        lblPriceAgua->setText(QCoreApplication::translate("MainWindow", "$900", nullptr));
        lblPriceAgua->setStyleSheet(QCoreApplication::translate("MainWindow", "font-size: 18px; font-weight: bold; color: #B8860B;", nullptr));
        lblDescAgua->setText(QCoreApplication::translate("MainWindow", "Agua mineral de manantial sin gas de 500ml bien fr\303\255a.", nullptr));
        lblDescAgua->setStyleSheet(QCoreApplication::translate("MainWindow", "font-size: 12px; color: #666666;", nullptr));
        btnVerDetalleAgua->setText(QCoreApplication::translate("MainWindow", "\360\237\221\201\357\270\217 Ver Detalle Completo \342\236\224", nullptr));
        cardJugo->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: #FFFFFF; border: 1px solid #EAE3D2; border-radius: 10px;", nullptr));
        lblTitleJugo->setText(QCoreApplication::translate("MainWindow", "\360\237\215\212 Jugo Natural Exprimido de Naranja 350ml", nullptr));
        lblTitleJugo->setStyleSheet(QCoreApplication::translate("MainWindow", "font-size: 16px; font-weight: bold; color: #3D1A24;", nullptr));
        lblPriceJugo->setText(QCoreApplication::translate("MainWindow", "$1.600", nullptr));
        lblPriceJugo->setStyleSheet(QCoreApplication::translate("MainWindow", "font-size: 18px; font-weight: bold; color: #B8860B;", nullptr));
        lblDescJugo->setText(QCoreApplication::translate("MainWindow", "Jugo 100% natural de naranjas seleccionadas exprimidas en el d\303\255a, sin conservantes.", nullptr));
        lblDescJugo->setStyleSheet(QCoreApplication::translate("MainWindow", "font-size: 12px; color: #666666;", nullptr));
        btnVerDetalleJugo->setText(QCoreApplication::translate("MainWindow", "\360\237\221\201\357\270\217 Ver Detalle Completo \342\236\224", nullptr));
        cardLicuado->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: #FFFFFF; border: 1px solid #EAE3D2; border-radius: 10px;", nullptr));
        lblTitleLicuado->setText(QCoreApplication::translate("MainWindow", "\360\237\215\223 Licuado Frutal de Frutilla y Banana 400ml", nullptr));
        lblTitleLicuado->setStyleSheet(QCoreApplication::translate("MainWindow", "font-size: 16px; font-weight: bold; color: #3D1A24;", nullptr));
        lblPriceLicuado->setText(QCoreApplication::translate("MainWindow", "$2.200", nullptr));
        lblPriceLicuado->setStyleSheet(QCoreApplication::translate("MainWindow", "font-size: 18px; font-weight: bold; color: #B8860B;", nullptr));
        lblDescLicuado->setText(QCoreApplication::translate("MainWindow", "Licuado con leche o agua y fruta fresca de estaci\303\263n preparado al momento.", nullptr));
        lblDescLicuado->setStyleSheet(QCoreApplication::translate("MainWindow", "font-size: 12px; color: #666666;", nullptr));
        btnVerDetalleLicuado->setText(QCoreApplication::translate("MainWindow", "\360\237\221\201\357\270\217 Ver Detalle Completo \342\236\224", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabBebidas), QCoreApplication::translate("MainWindow", "\360\237\245\244 Bebidas & Refreshers", nullptr));
        cardNotice->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: #F8F4EF; border: 1px solid #E6DEC3; border-radius: 8px; padding: 10px;", nullptr));
        lblRuleNoticeCart->setText(QCoreApplication::translate("MainWindow", "\360\237\223\261 Cumplimiento de Norma Escolar: Los pedidos se realizan y retiran \303\272nicamente durante los recreos o fuera del horario de clases. Muestra tu c\303\263digo de 5 d\303\255gitos en mostrador.", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableCart->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("MainWindow", "Producto Seleccionado", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableCart->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("MainWindow", "Cant.", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableCart->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("MainWindow", "Precio Unit.", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tableCart->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("MainWindow", "Subtotal", nullptr));
        lblSelectRecreo->setText(QCoreApplication::translate("MainWindow", "Seleccionar Horario de Retiro:", nullptr));
        comboRecreo->setItemText(0, QCoreApplication::translate("MainWindow", "Primer Recreo (09:50 hs)", nullptr));
        comboRecreo->setItemText(1, QCoreApplication::translate("MainWindow", "Segundo Recreo (12:00 hs - Almuerzo)", nullptr));
        comboRecreo->setItemText(2, QCoreApplication::translate("MainWindow", "Tercer Recreo (14:30 hs)", nullptr));
        comboRecreo->setItemText(3, QCoreApplication::translate("MainWindow", "Salida (16:15 hs)", nullptr));

        lblCartTotal->setText(QCoreApplication::translate("MainWindow", "Total a Abonar: $2.800", nullptr));
        lblCartTotal->setStyleSheet(QCoreApplication::translate("MainWindow", "font-size: 18px; font-weight: bold; color: #3D1A24;", nullptr));
        btnCheckout->setText(QCoreApplication::translate("MainWindow", "\342\234\205 CONFIRMAR Y GENERAR C\303\223DIGO DE RETIRO EXPRESS \342\236\224", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabCarrito), QCoreApplication::translate("MainWindow", "\360\237\233\222 Mi Carrito Express", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
