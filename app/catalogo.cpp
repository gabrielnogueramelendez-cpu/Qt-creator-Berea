// catalogo.cpp: Catálogo del alumno: productos, menú del día, saldo y botón TU PEDIDO.  Diseño: mainwindow.ui

#include "pantallas.h"
#include "ui_mainwindow.h"
#include "datos.h"
#include <QMainWindow>
#include <QMessageBox>
#include <QDate>
#include <QPainter>
#include <QScrollArea>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>

// ===== BLOQUE 1: VARIABLES DE LA PANTALLA =====

static QMainWindow *ventana = NULL;
static Ui::MainWindow *ui = NULL;

// Los 20 productos del catálogo, en el orden de su ProductoID (1 a 20)
static const int CANTIDAD_PRODUCTOS = 20;
static QPushButton *botones[CANTIDAD_PRODUCTOS];
static QLabel *etiquetas_precio[CANTIDAD_PRODUCTOS];
static QLabel *etiquetas_foto[CANTIDAD_PRODUCTOS];

// Pestaña NUEVOS: tiene los productos que el administrador agregó después de los 20 del diseño
static QWidget *pestana_nuevos = NULL;

static void ver_producto(int id);

// ===== BLOQUE 2: FUNCIONES QUE MUESTRAN DATOS =====

// Sirve para liberar el .ui cuando la ventana se cierra.
static void liberar()
{
    delete ui;
    ui = NULL;
    ventana = NULL;
    pestana_nuevos = NULL; // se borró junto con la ventana
}

// Sirve para poner la foto de un producto en su etiqueta: entera, centrada y con fondo blanco alrededor.
// Devuelve false si no hay foto con ese nombre.
static bool cargar_foto(QLabel *etiqueta, const QString &nombre)
{
    QPixmap foto(":/imagenes/imagenes/" + nombre + ".jpg");
    if (foto.isNull()) {
        return false;
    }
    QSize tam = etiqueta->minimumSize();
    QPixmap chica = foto.scaled(tam, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QPixmap fondo(tam);
    fondo.fill(Qt::white);
    QPainter pintor(&fondo);
    pintor.drawPixmap((tam.width() - chica.width()) / 2, (tam.height() - chica.height()) / 2, chica);
    pintor.end();
    etiqueta->setPixmap(fondo);
    return true;
}

// Sirve para escribir el precio de la BDD debajo de cada producto
// y esconder la tarjeta de los que el administrador dio de baja.
static void mostrar_precios()
{
    for (int i = 0; i < CANTIDAD_PRODUCTOS; i++) {
        int id = i + 1;
        bool activo = id < productos_activos.size() && productos_activos[id];
        botones[i]->parentWidget()->setVisible(activo); // el padre del botón es la tarjeta
        if (activo) {
            etiquetas_precio[i]->setText(pesos(productos_precios[id]));
        }
    }
}

// Sirve para armar una tarjeta igual a las del diseño para un producto agregado por el administrador.
static QFrame *crear_tarjeta(int id)
{
    QFrame *tarjeta = new QFrame();
    tarjeta->setObjectName("tarjetaNueva");
    tarjeta->setFixedSize(352, 370);
    tarjeta->setStyleSheet("#tarjetaNueva { background-color: #FFFDF8; border: 1px solid #D9CBB5; }");
    QVBoxLayout *capa = new QVBoxLayout(tarjeta);
    capa->setContentsMargins(1, 1, 1, 14);
    capa->setSpacing(8);

    QLabel *foto = new QLabel("Sin foto");
    foto->setFixedSize(350, 180);
    foto->setAlignment(Qt::AlignCenter);
    foto->setStyleSheet("background-color: #EDE3D3; color: #7A6A60;");
    cargar_foto(foto, productos_nombres[id]);
    capa->addWidget(foto);

    QVBoxLayout *texto = new QVBoxLayout();
    texto->setContentsMargins(16, 4, 16, 0);
    texto->setSpacing(6);
    QLabel *nombre = new QLabel(productos_nombres[id]);
    nombre->setWordWrap(true);
    nombre->setStyleSheet("font-family: 'Sitka Banner', 'Palatino Linotype', Georgia; font-size: 20px; color: #2B1A1A;");
    texto->addWidget(nombre);
    QLabel *descripcion = new QLabel(productos_descripciones[id]);
    descripcion->setWordWrap(true);
    descripcion->setStyleSheet("font-size: 12px; color: #7A6A60;");
    texto->addWidget(descripcion);
    texto->addStretch();

    QFrame *linea = new QFrame();
    linea->setFixedHeight(1);
    linea->setStyleSheet("background-color: #D9CBB5;");
    texto->addWidget(linea);

    QHBoxLayout *pie = new QHBoxLayout();
    QLabel *precio = new QLabel(pesos(productos_precios[id]));
    precio->setStyleSheet("font-size: 17px; font-weight: bold; color: #6B1B2A;");
    pie->addWidget(precio);
    QPushButton *boton = new QPushButton("AGREGAR");
    QObject::connect(boton, &QPushButton::clicked, [id]() { ver_producto(id); });
    pie->addWidget(boton, 0, Qt::AlignRight);
    texto->addLayout(pie);

    capa->addLayout(texto);
    return tarjeta;
}

// Sirve para mostrar en la pestaña NUEVOS los productos con ID mayor a 20 (no tienen tarjeta en el diseño).
// Si no hay ninguno, la pestaña no aparece.
static void mostrar_productos_nuevos()
{
    if (pestana_nuevos != NULL) {
        ui->tabWidget->removeTab(ui->tabWidget->indexOf(pestana_nuevos));
        delete pestana_nuevos;
        pestana_nuevos = NULL;
    }

    QWidget *contenido = new QWidget();
    contenido->setObjectName("contenidoNuevos");
    contenido->setStyleSheet("#contenidoNuevos { background-color: #EDE3D3; }");
    QGridLayout *grilla = new QGridLayout(contenido);
    grilla->setContentsMargins(40, 22, 24, 22);
    grilla->setSpacing(16);
    grilla->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    int cantidad = 0;
    for (int id = CANTIDAD_PRODUCTOS + 1; id < productos_nombres.size(); id++) {
        if (!productos_activos[id]) {
            continue; // no existe en la BDD o está dado de baja
        }
        grilla->addWidget(crear_tarjeta(id), cantidad / 3, cantidad % 3);
        cantidad++;
    }
    if (cantidad == 0) {
        delete contenido;
        return;
    }

    QScrollArea *scroll = new QScrollArea();
    scroll->setStyleSheet("QScrollArea { border: none; background-color: #EDE3D3; }");
    scroll->setWidgetResizable(true);
    scroll->setWidget(contenido);
    pestana_nuevos = scroll;
    ui->tabWidget->addTab(pestana_nuevos, "NUEVOS");
}

// Sirve para completar la tarjeta del menú del día (plato, qué incluye, precio y stock).
static void mostrar_menu_del_dia()
{
    ui->btnVerMenu->setEnabled(menu_hoy_id != 0);
    if (menu_hoy_id == 0) {
        return;
    }
    QString fecha = QDate::currentDate().toString("dd/MM/yyyy");
    ui->lblFechaMenu->setText("Menú del día\n" + fecha);
    ui->lblPlatoMenu->setText(menu_hoy_plato);
    ui->lblIncluyeMenu->setText("Incluye: " + menu_hoy_incluye);

    // Si el plato tiene foto, ocupa el recuadro de arriba y la fecha baja al texto.
    if (cargar_foto(ui->lblFechaMenu, menu_hoy_plato)) {
        ui->lblIncluyeMenu->setText("Menú del día " + fecha + " · Incluye: " + menu_hoy_incluye);
    }
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
    mostrar_productos_nuevos();
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

    QObject::connect(ui->btnVerMenu, &QPushButton::clicked, ver_menu);
    QObject::connect(ui->btnCheckout, &QPushButton::clicked, ir_a_pagar);
    QObject::connect(ui->btnPerfil, &QPushButton::clicked, ver_perfil);
    QObject::connect(ui->btnLogout, &QPushButton::clicked, cerrar_sesion_catalogo);

    mostrar_precios();
    mostrar_productos_nuevos();
    mostrar_menu_del_dia();
    mostrar_usuario_y_carrito();
    ventana->show();
}
