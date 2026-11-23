// catalogo.cpp: Catálogo del alumno: productos, menú del día, saldo y botón TU PEDIDO.  Diseño: mainwindow.ui

#include "pantallas.h"
#include "ui_mainwindow.h"
#include "datos.h"
#include <QMainWindow>
#include <QMessageBox>
#include <QDate>

// ===== BLOQUE 1: VARIABLES DE LA PANTALLA =====

static QMainWindow *ventana = NULL;
static Ui::MainWindow *ui = NULL;

// Los 20 productos del catálogo, en el orden de su ProductoID (1 a 20)
static const int CANTIDAD_PRODUCTOS = 20;
static QPushButton *botones[CANTIDAD_PRODUCTOS];
static QLabel *etiquetas_precio[CANTIDAD_PRODUCTOS];
static QLabel *etiquetas_foto[CANTIDAD_PRODUCTOS];

// ===== BLOQUE 2: FUNCIONES QUE MUESTRAN DATOS =====

// Sirve para liberar el .ui cuando la ventana se cierra.
static void liberar()
{
    delete ui;
    ui = NULL;
    ventana = NULL;
}

// Sirve para poner la foto de un producto en su etiqueta.
static void cargar_foto(QLabel *etiqueta, const QString &nombre)
{
    QPixmap foto(":/imagenes/imagenes/" + nombre + ".jpg");
    if (!foto.isNull()) {
        etiqueta->setPixmap(foto.scaled(etiqueta->size(), Qt::KeepAspectRatio));
    }
}

// Sirve para escribir el precio de la BDD debajo de cada producto.
static void mostrar_precios()
{
    for (int i = 0; i < CANTIDAD_PRODUCTOS; i++) {
        int id = i + 1;
        if (id < productos_precios.size()) {
            etiquetas_precio[i]->setText(pesos(productos_precios[id]));
        }
    }
}

// Sirve para completar la tarjeta del menú del día (plato, qué incluye, precio y stock).
static void mostrar_menu_del_dia()
{
    ui->btnVerMenu->setEnabled(menu_hoy_id != 0);
    if (menu_hoy_id == 0) {
        return;
    }
    ui->lblFechaMenu->setText("Menú del día\n" + QDate::currentDate().toString("dd/MM/yyyy"));
    ui->lblPlatoMenu->setText(menu_hoy_plato);
    ui->lblIncluyeMenu->setText("Incluye: " + menu_hoy_incluye);
    ui->lblPrecioMenu->setText(pesos(menu_hoy_precio));
    ui->lblStockMenu->setText(QString("QUEDAN %1").arg(productos_stock[0]));
}

// Sirve para actualizar la barra de arriba.
static void mostrar_usuario_y_carrito()
{
    ui->btnPerfil->setText(usuario_nombre.left(1) + usuario_apellido.left(1));
    ui->lblUserGreeting->setText(QString("%1 %2 · %3 · Saldo %4")
        .arg(usuario_nombre, usuario_apellido, usuario_curso, pesos(usuario_saldo)));
    ui->btnCheckout->setText(QString("TU PEDIDO · %1 · %2")
        .arg(cantidad_total_carrito())
        .arg(pesos(total_carrito())));
}

// ===== BLOQUE 3: FUNCIONES DE LOS BOTONES =====

// Sirve para abrir el detalle de un producto; si lo agregó, actualiza el botón del carrito.
static void ver_producto(int id)
{
    // id 0 = menú del día
    if (abrir_detalle(ventana, id)) {
        mostrar_usuario_y_carrito();
    }
}

// Sirve para el botón del menú del día: abre su detalle (es el id 0).
static void ver_menu()
{
    ver_producto(0);
}

// Sirve para el botón TU PEDIDO.
static void ir_a_pagar()
{
    if (carrito_ids.isEmpty()) {
        QMessageBox::warning(ventana, "Carrito Vacío",
            "Tu carrito está actualmente vacío.\nPor favor agrega al menos un producto o menú antes de proceder.");
        return;
    }

    // 1. Carrito  2. Pago  3. Comprobante
    if (abrir_carrito(ventana)) {
        QString codigo;
        double abonado = 0;
        double vuelto = 0;
        if (abrir_pago(ventana, codigo, abonado, vuelto)) {
            abrir_comprobante(ventana, codigo, abonado, vuelto);
        }
    }

    cargar_productos();
    mostrar_precios();
    mostrar_menu_del_dia();
    mostrar_usuario_y_carrito();
}

// Sirve para el botón de las iniciales: abre el perfil.
static void ver_perfil()
{
    abrir_perfil(ventana);
    mostrar_usuario_y_carrito(); // por si cargó saldo
}

// Sirve para el botón SALIR: cierra la sesión y vuelve al login.
static void cerrar_sesion_catalogo()
{
    salir_de_la_cuenta();
    abrir_login();
    ventana->close();
}

// ===== BLOQUE 4: APERTURA DE LA PANTALLA =====

// Sirve para abrir el catálogo.
void abrir_catalogo()
{
    if (ventana != NULL) {
        ventana->raise();
        return;
    }
    ventana = new QMainWindow();
    ventana->setAttribute(Qt::WA_DeleteOnClose);
    ui = new Ui::MainWindow();
    ui->setupUi(ventana);
    ventana->setWindowTitle("El Buffet de Alejandro - Catálogo");
    ui->tabWidget->setCurrentIndex(0);
    QObject::connect(ventana, &QObject::destroyed, liberar);

    QPushButton *b[CANTIDAD_PRODUCTOS] = {
        ui->btnVerMilanesa, ui->btnVerEmpanada, ui->btnVerPizza, ui->btnVerPebete, ui->btnVerMedialuna,
        ui->btnVerEnsalada, ui->btnVerManzana, ui->btnVerChocolatada, ui->btnVerYogur, ui->btnVerAgua,
        ui->btnVerJugo, ui->btnVerGaseosa, ui->btnVerCafe, ui->btnVerBarra, ui->btnVerGalletitas,
        ui->btnVerAlfajor, ui->btnVerTurron, ui->btnVerFrutosSecos, ui->btnVerPolloPure, ui->btnVerCombo};
    QLabel *p[CANTIDAD_PRODUCTOS] = {
        ui->lblPrecioMilanesa, ui->lblPrecioEmpanada, ui->lblPrecioPizza, ui->lblPrecioPebete, ui->lblPrecioMedialuna,
        ui->lblPrecioEnsalada, ui->lblPrecioManzana, ui->lblPrecioChocolatada, ui->lblPrecioYogur, ui->lblPrecioAgua,
        ui->lblPrecioJugo, ui->lblPrecioGaseosa, ui->lblPrecioCafe, ui->lblPrecioBarra, ui->lblPrecioGalletitas,
        ui->lblPrecioAlfajor, ui->lblPrecioTurron, ui->lblPrecioFrutosSecos, ui->lblPrecioPolloPure, ui->lblPrecioCombo};
    QLabel *f[CANTIDAD_PRODUCTOS] = {
        ui->lblFotoMilanesa, ui->lblFotoEmpanada, ui->lblFotoPizza, ui->lblFotoPebete, ui->lblFotoMedialuna,
        ui->lblFotoEnsalada, ui->lblFotoManzana, ui->lblFotoChocolatada, ui->lblFotoYogur, ui->lblFotoAgua,
        ui->lblFotoJugo, ui->lblFotoGaseosa, ui->lblFotoCafe, ui->lblFotoBarra, ui->lblFotoGalletitas,
        ui->lblFotoAlfajor, ui->lblFotoTurron, ui->lblFotoFrutosSecos, ui->lblFotoPolloPure, ui->lblFotoCombo};

    cargar_productos();

    for (int i = 0; i < CANTIDAD_PRODUCTOS; i++) {
        botones[i] = b[i];
        etiquetas_precio[i] = p[i];
        etiquetas_foto[i] = f[i];
        int id = i + 1;
        QObject::connect(botones[i], &QPushButton::clicked, [id]() { ver_producto(id); });
        if (id < productos_nombres.size()) {
            cargar_foto(etiquetas_foto[i], productos_nombres[id]);
        }
    }
    cargar_foto(ui->lblPortada, "portada");

    QObject::connect(ui->btnVerMenu, &QPushButton::clicked, ver_menu);
    QObject::connect(ui->btnCheckout, &QPushButton::clicked, ir_a_pagar);
    QObject::connect(ui->btnPerfil, &QPushButton::clicked, ver_perfil);
    QObject::connect(ui->btnLogout, &QPushButton::clicked, cerrar_sesion_catalogo);

    mostrar_precios();
    mostrar_menu_del_dia();
    mostrar_usuario_y_carrito();
    ventana->show();
}
