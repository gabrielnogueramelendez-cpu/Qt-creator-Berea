#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "productdetaildialog.h"
#include "carritodialog.h"
#include "pagodialog.h"
#include "comprobantedialog.h"
#include "perfildialog.h"
#include "loginwindow.h"
#include "sistemabuffet.h"
#include <QMessageBox>
#include <QDate>

// Arma el texto del precio de un producto, por ejemplo "$2800"
static QString textoPrecio(int productoID)
{
    return "$" + QString::number(SistemaBuffet::productos[productoID].precio, 'f', 0);
}

// =====================================================================
// Constructor: arma el catálogo
// 1. Trae de la BDD los precios y el stock (si hay conexión)
// 2. Pone los precios y las fotos de cada producto
// 3. Muestra el menú del día y el botón del carrito
// =====================================================================
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("El Buffet de Alejandro - Catálogo");
    ui->tabWidget->setCurrentIndex(0);

    SistemaBuffet::cargarDesdeBDD();
    mostrarPrecios();

    // Foto de cada producto: el archivo se llama igual que el producto (como en la guía Catálogo)
    cargarFoto(ui->lblFotoMilanesa, SistemaBuffet::productos[1].nombre);
    cargarFoto(ui->lblFotoEmpanada, SistemaBuffet::productos[2].nombre);
    cargarFoto(ui->lblFotoPizza, SistemaBuffet::productos[3].nombre);
    cargarFoto(ui->lblFotoPebete, SistemaBuffet::productos[4].nombre);
    cargarFoto(ui->lblFotoMedialuna, SistemaBuffet::productos[5].nombre);
    cargarFoto(ui->lblFotoEnsalada, SistemaBuffet::productos[6].nombre);
    cargarFoto(ui->lblFotoManzana, SistemaBuffet::productos[7].nombre);
    cargarFoto(ui->lblFotoChocolatada, SistemaBuffet::productos[8].nombre);
    cargarFoto(ui->lblFotoYogur, SistemaBuffet::productos[9].nombre);
    cargarFoto(ui->lblFotoAgua, SistemaBuffet::productos[10].nombre);
    cargarFoto(ui->lblFotoJugo, SistemaBuffet::productos[11].nombre);
    cargarFoto(ui->lblFotoGaseosa, SistemaBuffet::productos[12].nombre);
    cargarFoto(ui->lblFotoCafe, SistemaBuffet::productos[13].nombre);
    cargarFoto(ui->lblFotoBarra, SistemaBuffet::productos[14].nombre);
    cargarFoto(ui->lblFotoGalletitas, SistemaBuffet::productos[15].nombre);
    cargarFoto(ui->lblFotoAlfajor, SistemaBuffet::productos[16].nombre);
    cargarFoto(ui->lblFotoTurron, SistemaBuffet::productos[17].nombre);
    cargarFoto(ui->lblFotoFrutosSecos, SistemaBuffet::productos[18].nombre);
    cargarFoto(ui->lblFotoPolloPure, SistemaBuffet::productos[19].nombre);
    cargarFoto(ui->lblFotoCombo, SistemaBuffet::productos[20].nombre);

    // Imagen de portada (portada.jpg). Sin imagen queda la frase sobre bordó
    cargarFoto(ui->lblPortada, "portada");

    mostrarMenuDelDia();
    actualizarBotonCarrito();
}

MainWindow::~MainWindow()
{
    delete ui;
}

// =====================================================================
// mostrarPrecios: pone el precio debajo de cada producto
// =====================================================================
void MainWindow::mostrarPrecios()
{
    ui->lblPrecioMilanesa->setText(textoPrecio(1));
    ui->lblPrecioEmpanada->setText(textoPrecio(2));
    ui->lblPrecioPizza->setText(textoPrecio(3));
    ui->lblPrecioPebete->setText(textoPrecio(4));
    ui->lblPrecioMedialuna->setText(textoPrecio(5));
    ui->lblPrecioEnsalada->setText(textoPrecio(6));
    ui->lblPrecioManzana->setText(textoPrecio(7));
    ui->lblPrecioChocolatada->setText(textoPrecio(8));
    ui->lblPrecioYogur->setText(textoPrecio(9));
    ui->lblPrecioAgua->setText(textoPrecio(10));
    ui->lblPrecioJugo->setText(textoPrecio(11));
    ui->lblPrecioGaseosa->setText(textoPrecio(12));
    ui->lblPrecioCafe->setText(textoPrecio(13));
    ui->lblPrecioBarra->setText(textoPrecio(14));
    ui->lblPrecioGalletitas->setText(textoPrecio(15));
    ui->lblPrecioAlfajor->setText(textoPrecio(16));
    ui->lblPrecioTurron->setText(textoPrecio(17));
    ui->lblPrecioFrutosSecos->setText(textoPrecio(18));
    ui->lblPrecioPolloPure->setText(textoPrecio(19));
    ui->lblPrecioCombo->setText(textoPrecio(20));
}

// =====================================================================
// actualizarBotonCarrito: iniciales, saludo con saldo y total del carrito
// =====================================================================
void MainWindow::actualizarBotonCarrito()
{
    UsuarioActivo usuario = SistemaBuffet::usuario;

    // Iniciales en el círculo del perfil (ej. "DM")
    ui->btnPerfil->setText(usuario.nombre.left(1) + usuario.apellido.left(1));

    // Saludo con el saldo actual (cambia al pagar con la cuenta escolar)
    ui->lblUserGreeting->setText(usuario.nombre + " " + usuario.apellido + " · " + usuario.curso
                                 + " · Saldo $" + QString::number(usuario.saldo, 'f', 0));

    int cantidadTotal = SistemaBuffet::getCantidadTotalCarrito();
    double total = SistemaBuffet::getTotalCarrito();
    ui->btnCheckout->setText("TU PEDIDO · " + QString::number(cantidadTotal)
                             + " · $" + QString::number(total, 'f', 0));
}

// =====================================================================
// mostrarMenuDelDia: completa la tarjeta del menú del día
// Sin menú (sábado o domingo) queda el texto del .ui y no se puede agregar
// =====================================================================
void MainWindow::mostrarMenuDelDia()
{
    if (SistemaBuffet::menuHoy.id == 0) {
        ui->btnVerMenu->setEnabled(false);
        return;
    }

    ui->btnVerMenu->setEnabled(true);
    ui->lblFechaMenu->setText("Menú del día\n" + QDate::currentDate().toString("dd/MM/yyyy"));
    ui->lblPlatoMenu->setText(SistemaBuffet::menuHoy.plato);
    ui->lblIncluyeMenu->setText("Incluye: " + SistemaBuffet::menuHoy.incluye);
    ui->lblPrecioMenu->setText("$" + QString::number(SistemaBuffet::menuHoy.precio, 'f', 0));
    ui->lblStockMenu->setText("QUEDAN " + QString::number(SistemaBuffet::productos[0].stock));
}

// =====================================================================
// cargarFoto: pone en una etiqueta la foto que está en recursos.qrc
// etiquetaFoto es la etiqueta de la pantalla donde va la foto
// (ui->lblFoto... ya es un puntero a esa etiqueta)
// =====================================================================
void MainWindow::cargarFoto(QLabel *etiquetaFoto, QString nombreArchivo)
{
    QPixmap foto(":/imagenes/imagenes/" + nombreArchivo + ".jpg");

    // Si todavía no hay foto, queda el texto "Sin foto" del .ui
    if (!foto.isNull()) {
        foto = foto.scaled(etiquetaFoto->size(), Qt::KeepAspectRatio);
        etiquetaFoto->setPixmap(foto);
    }
}

// =====================================================================
// abrirDetalleProducto: abre la ventana de detalle de un producto
// productoID 0 es el menú del día
// =====================================================================
void MainWindow::abrirDetalleProducto(int productoID)
{
    Producto elegido = SistemaBuffet::productos[productoID];

    // El menú del día toma el plato y el precio de hoy
    if (productoID == 0) {
        elegido.nombre = SistemaBuffet::menuHoy.plato;
        elegido.descripcion = "Menú del día. Incluye: " + SistemaBuffet::menuHoy.incluye;
        elegido.precio = SistemaBuffet::menuHoy.precio;
    }

    ProductDetailDialog ventanaDetalle(elegido, this);
    if (ventanaDetalle.exec() == QDialog::Accepted) {
        actualizarBotonCarrito();
    }
}

// =====================================================================
// Botones VER de cada producto
// =====================================================================
void MainWindow::on_btnVerMilanesa_clicked()    { abrirDetalleProducto(1); }
void MainWindow::on_btnVerEmpanada_clicked()    { abrirDetalleProducto(2); }
void MainWindow::on_btnVerPizza_clicked()       { abrirDetalleProducto(3); }
void MainWindow::on_btnVerPebete_clicked()      { abrirDetalleProducto(4); }
void MainWindow::on_btnVerMedialuna_clicked()   { abrirDetalleProducto(5); }
void MainWindow::on_btnVerEnsalada_clicked()    { abrirDetalleProducto(6); }
void MainWindow::on_btnVerManzana_clicked()     { abrirDetalleProducto(7); }
void MainWindow::on_btnVerChocolatada_clicked() { abrirDetalleProducto(8); }
void MainWindow::on_btnVerYogur_clicked()       { abrirDetalleProducto(9); }
void MainWindow::on_btnVerAgua_clicked()        { abrirDetalleProducto(10); }
void MainWindow::on_btnVerJugo_clicked()        { abrirDetalleProducto(11); }
void MainWindow::on_btnVerGaseosa_clicked()     { abrirDetalleProducto(12); }
void MainWindow::on_btnVerCafe_clicked()        { abrirDetalleProducto(13); }
void MainWindow::on_btnVerBarra_clicked()       { abrirDetalleProducto(14); }
void MainWindow::on_btnVerGalletitas_clicked()  { abrirDetalleProducto(15); }
void MainWindow::on_btnVerAlfajor_clicked()     { abrirDetalleProducto(16); }
void MainWindow::on_btnVerTurron_clicked()      { abrirDetalleProducto(17); }
void MainWindow::on_btnVerFrutosSecos_clicked() { abrirDetalleProducto(18); }
void MainWindow::on_btnVerPolloPure_clicked()   { abrirDetalleProducto(19); }
void MainWindow::on_btnVerCombo_clicked()       { abrirDetalleProducto(20); }
void MainWindow::on_btnVerMenu_clicked()        { abrirDetalleProducto(0); }

// =====================================================================
// Botón TU PEDIDO: carrito -> pago -> comprobante
// =====================================================================
void MainWindow::on_btnCheckout_clicked()
{
    // [NEG-04] El carrito no puede estar vacío
    if (SistemaBuffet::carrito.isEmpty()) {
        QMessageBox::warning(this, "Carrito Vacío",
            "Tu carrito está actualmente vacío.\nPor favor agrega al menos un producto o menú antes de proceder.");
        return;
    }

    // 1. Ventana del carrito
    CarritoDialog ventanaCarrito(this);
    if (ventanaCarrito.exec() == QDialog::Accepted && ventanaCarrito.fueHaciaElPago()) {

        // 2. Ventana del pago
        PagoDialog ventanaPago(this);
        if (ventanaPago.exec() == QDialog::Accepted) {

            // 3. Comprobante con el código de retiro
            ComprobanteDialog ventanaComprobante(ventanaPago.getCodigoGenerado(), this);
            ventanaComprobante.exec();
        }
    }

    // Con BDD: stock y menú actualizados después de la compra
    SistemaBuffet::cargarDesdeBDD();
    mostrarMenuDelDia();
    actualizarBotonCarrito();
}

// =====================================================================
// Botón SALIR: vuelve al login
// =====================================================================
void MainWindow::on_btnLogout_clicked()
{
    SistemaBuffet::cerrarSesion();
    LoginWindow *ventanaLogin = new LoginWindow();
    ventanaLogin->show();
    this->close();
}

// =====================================================================
// Botón del perfil (círculo con las iniciales)
// =====================================================================
void MainWindow::on_btnPerfil_clicked()
{
    PerfilDialog ventanaPerfil(this);
    ventanaPerfil.exec();
    actualizarBotonCarrito(); // por si cargó saldo
}
