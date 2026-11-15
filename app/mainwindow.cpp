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

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("El Buffet de Alejandro - Catálogo");
    ui->tabWidget->setCurrentIndex(0);


    // Con BDD: el stock y los precios se leen de la tabla Producto
    SistemaBuffet::cargarDesdeBDD();
    ui->lblPrecioMilanesa->setText("$" + QString::number(SistemaBuffet::productosPrecios[1], 'f', 0));
    ui->lblPrecioEmpanada->setText("$" + QString::number(SistemaBuffet::productosPrecios[2], 'f', 0));
    ui->lblPrecioPizza->setText("$" + QString::number(SistemaBuffet::productosPrecios[3], 'f', 0));
    ui->lblPrecioPebete->setText("$" + QString::number(SistemaBuffet::productosPrecios[4], 'f', 0));
    ui->lblPrecioMedialuna->setText("$" + QString::number(SistemaBuffet::productosPrecios[5], 'f', 0));
    ui->lblPrecioEnsalada->setText("$" + QString::number(SistemaBuffet::productosPrecios[6], 'f', 0));
    ui->lblPrecioManzana->setText("$" + QString::number(SistemaBuffet::productosPrecios[7], 'f', 0));
    ui->lblPrecioChocolatada->setText("$" + QString::number(SistemaBuffet::productosPrecios[8], 'f', 0));
    ui->lblPrecioYogur->setText("$" + QString::number(SistemaBuffet::productosPrecios[9], 'f', 0));
    ui->lblPrecioAgua->setText("$" + QString::number(SistemaBuffet::productosPrecios[10], 'f', 0));
    ui->lblPrecioJugo->setText("$" + QString::number(SistemaBuffet::productosPrecios[11], 'f', 0));
    ui->lblPrecioGaseosa->setText("$" + QString::number(SistemaBuffet::productosPrecios[12], 'f', 0));
    ui->lblPrecioCafe->setText("$" + QString::number(SistemaBuffet::productosPrecios[13], 'f', 0));
    ui->lblPrecioBarra->setText("$" + QString::number(SistemaBuffet::productosPrecios[14], 'f', 0));
    ui->lblPrecioGalletitas->setText("$" + QString::number(SistemaBuffet::productosPrecios[15], 'f', 0));
    ui->lblPrecioAlfajor->setText("$" + QString::number(SistemaBuffet::productosPrecios[16], 'f', 0));
    ui->lblPrecioTurron->setText("$" + QString::number(SistemaBuffet::productosPrecios[17], 'f', 0));
    ui->lblPrecioFrutosSecos->setText("$" + QString::number(SistemaBuffet::productosPrecios[18], 'f', 0));
    ui->lblPrecioPolloPure->setText("$" + QString::number(SistemaBuffet::productosPrecios[19], 'f', 0));
    ui->lblPrecioCombo->setText("$" + QString::number(SistemaBuffet::productosPrecios[20], 'f', 0));

    // Foto de cada producto: el archivo se llama igual que el producto (como en la guía Catálogo)
    cargarFoto(ui->lblFotoMilanesa, "Sándwich de Milanesa");
    cargarFoto(ui->lblFotoEmpanada, "Empanada de Carne");
    cargarFoto(ui->lblFotoPizza, "Porción de Pizza Muzzarella");
    cargarFoto(ui->lblFotoPebete, "Pebete de Queso");
    cargarFoto(ui->lblFotoMedialuna, "Medialuna de Manteca");
    cargarFoto(ui->lblFotoEnsalada, "Ensalada Mixta");
    cargarFoto(ui->lblFotoManzana, "Manzana");
    cargarFoto(ui->lblFotoChocolatada, "Chocolatada 250 ml");
    cargarFoto(ui->lblFotoYogur, "Yogur Bebible de Frutilla");
    cargarFoto(ui->lblFotoAgua, "Agua Mineral 500 ml");
    cargarFoto(ui->lblFotoJugo, "Jugo en Caja 200 ml");
    cargarFoto(ui->lblFotoGaseosa, "Gaseosa 500ml");
    cargarFoto(ui->lblFotoCafe, "Café con Leche");
    cargarFoto(ui->lblFotoBarra, "Barra de Cereal");
    cargarFoto(ui->lblFotoGalletitas, "Galletitas de Agua");
    cargarFoto(ui->lblFotoAlfajor, "Alfajor de Maicena");
    cargarFoto(ui->lblFotoTurron, "Turrón de Maní");
    cargarFoto(ui->lblFotoFrutosSecos, "Mix de Frutos Secos");
    cargarFoto(ui->lblFotoPolloPure, "Milanesa de Pollo con Puré");
    cargarFoto(ui->lblFotoCombo, "Combo Sándwich + Gaseosa");

    // Imagen de portada del cuerpo (portada.jpg en el .qrc); sin imagen queda la frase sobre bordó
    cargarFoto(ui->lblPortada, "portada");

    mostrarMenuDelDia();
    actualizarBotonCarrito();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::actualizarBotonCarrito()
{
    // Iniciales del estudiante en el círculo del perfil
    ui->btnPerfil->setText(SistemaBuffet::usuarioNombre.left(1) + SistemaBuffet::usuarioApellido.left(1));

    // Saludo con el saldo actual (cambia al pagar con la cuenta escolar)
    ui->lblUserGreeting->setText(QString("%1 %2 · %3 · Saldo $%4")
        .arg(SistemaBuffet::usuarioNombre)
        .arg(SistemaBuffet::usuarioApellido)
        .arg(SistemaBuffet::usuarioCurso)
        .arg(QString::number(SistemaBuffet::usuarioSaldo, 'f', 0)));

    int cant = SistemaBuffet::getCantidadTotalCarrito();
    double total = SistemaBuffet::getTotalCarrito();
    ui->btnCheckout->setText(QString("TU PEDIDO · %1 · $%2")
        .arg(cant)
        .arg(QString::number(total, 'f', 0)));
}

void MainWindow::mostrarMenuDelDia()
{
    // Sin menú hoy (sábado o domingo): queda el texto del .ui y no se puede agregar
    ui->btnVerMenu->setEnabled(SistemaBuffet::menuHoyID != 0);
    if (SistemaBuffet::menuHoyID == 0) return;

    ui->lblFechaMenu->setText("Menú del día\n" + QDate::currentDate().toString("dd/MM/yyyy"));
    ui->lblPlatoMenu->setText(SistemaBuffet::menuHoyPlato);
    ui->lblIncluyeMenu->setText("Incluye: " + SistemaBuffet::menuHoyIncluye);
    ui->lblPrecioMenu->setText("$" + QString::number(SistemaBuffet::menuHoyPrecio, 'f', 0));
    ui->lblStockMenu->setText(QString("QUEDAN %1").arg(SistemaBuffet::productosStock[0]));
}

void MainWindow::cargarFoto(QLabel *lblFoto, QString nombreProducto)
{
    QPixmap foto(":/imagenes/imagenes/" + nombreProducto + ".jpg");
    if (foto.isNull()) {
        return; // todavía no hay foto: queda el texto "Sin foto" del .ui
    }
    foto = foto.scaled(lblFoto->size(), Qt::KeepAspectRatio);
    lblFoto->setPixmap(foto);
}

void MainWindow::abrirDetalleProducto(int id, const QString &nombre, const QString &desc, double precio, int stock)
{
    ProductDetailDialog dlg(id, nombre, desc, precio, stock, this);
    if (dlg.exec() == QDialog::Accepted) {
        actualizarBotonCarrito();
    }
}

void MainWindow::on_btnVerMilanesa_clicked()
{
    abrirDetalleProducto(1, "Sándwich de Milanesa", "Milanesa de carne en pan francés con lechuga y tomate", SistemaBuffet::productosPrecios[1], SistemaBuffet::productosStock[1]);
}

void MainWindow::on_btnVerEmpanada_clicked()
{
    abrirDetalleProducto(2, "Empanada de Carne", "Empanada al horno rellena de carne cortada a cuchillo", SistemaBuffet::productosPrecios[2], SistemaBuffet::productosStock[2]);
}

void MainWindow::on_btnVerPizza_clicked()
{
    abrirDetalleProducto(3, "Porción de Pizza Muzzarella", "Porción de pizza de muzzarella", SistemaBuffet::productosPrecios[3], SistemaBuffet::productosStock[3]);
}

void MainWindow::on_btnVerPebete_clicked()
{
    abrirDetalleProducto(4, "Pebete de Queso", "Pebete con queso cremoso", SistemaBuffet::productosPrecios[4], SistemaBuffet::productosStock[4]);
}

void MainWindow::on_btnVerMedialuna_clicked()
{
    abrirDetalleProducto(5, "Medialuna de Manteca", "Medialuna dulce de manteca", SistemaBuffet::productosPrecios[5], SistemaBuffet::productosStock[5]);
}

void MainWindow::on_btnVerEnsalada_clicked()
{
    abrirDetalleProducto(6, "Ensalada Mixta", "Lechuga, tomate y zanahoria", SistemaBuffet::productosPrecios[6], SistemaBuffet::productosStock[6]);
}

void MainWindow::on_btnVerManzana_clicked()
{
    abrirDetalleProducto(7, "Manzana", "Manzana roja por unidad", SistemaBuffet::productosPrecios[7], SistemaBuffet::productosStock[7]);
}

void MainWindow::on_btnVerChocolatada_clicked()
{
    abrirDetalleProducto(8, "Chocolatada 250 ml", "Leche chocolatada en envase individual", SistemaBuffet::productosPrecios[8], SistemaBuffet::productosStock[8]);
}

void MainWindow::on_btnVerYogur_clicked()
{
    abrirDetalleProducto(9, "Yogur Bebible de Frutilla", "Yogur bebible 200 ml", SistemaBuffet::productosPrecios[9], SistemaBuffet::productosStock[9]);
}

void MainWindow::on_btnVerAgua_clicked()
{
    abrirDetalleProducto(10, "Agua Mineral 500 ml", "Agua mineral sin gas", SistemaBuffet::productosPrecios[10], SistemaBuffet::productosStock[10]);
}

void MainWindow::on_btnVerJugo_clicked()
{
    abrirDetalleProducto(11, "Jugo en Caja 200 ml", "Jugo de frutas en envase individual", SistemaBuffet::productosPrecios[11], SistemaBuffet::productosStock[11]);
}

void MainWindow::on_btnVerGaseosa_clicked()
{
    abrirDetalleProducto(12, "Gaseosa 500ml", "Gaseosa línea cola 500 ml", SistemaBuffet::productosPrecios[12], SistemaBuffet::productosStock[12]);
}

void MainWindow::on_btnVerCafe_clicked()
{
    abrirDetalleProducto(13, "Café con Leche", "Vaso de café con leche", SistemaBuffet::productosPrecios[13], SistemaBuffet::productosStock[13]);
}

void MainWindow::on_btnVerBarra_clicked()
{
    abrirDetalleProducto(14, "Barra de Cereal", "Barra de cereal con frutas", SistemaBuffet::productosPrecios[14], SistemaBuffet::productosStock[14]);
}

void MainWindow::on_btnVerGalletitas_clicked()
{
    abrirDetalleProducto(15, "Galletitas de Agua", "Paquete individual de galletitas de agua", SistemaBuffet::productosPrecios[15], SistemaBuffet::productosStock[15]);
}

void MainWindow::on_btnVerAlfajor_clicked()
{
    abrirDetalleProducto(16, "Alfajor de Maicena", "Alfajor de maicena con dulce de leche y coco", SistemaBuffet::productosPrecios[16], SistemaBuffet::productosStock[16]);
}

void MainWindow::on_btnVerTurron_clicked()
{
    abrirDetalleProducto(17, "Turrón de Maní", "Turrón de maní individual", SistemaBuffet::productosPrecios[17], SistemaBuffet::productosStock[17]);
}

void MainWindow::on_btnVerFrutosSecos_clicked()
{
    abrirDetalleProducto(18, "Mix de Frutos Secos", "Mix de almendras, pasas y maní sin sal", SistemaBuffet::productosPrecios[18], SistemaBuffet::productosStock[18]);
}

void MainWindow::on_btnVerPolloPure_clicked()
{
    abrirDetalleProducto(19, "Milanesa de Pollo con Puré", "Milanesa al horno con puré de papas", SistemaBuffet::productosPrecios[19], SistemaBuffet::productosStock[19]);
}

void MainWindow::on_btnVerCombo_clicked()
{
    abrirDetalleProducto(20, "Combo Sándwich + Gaseosa", "Sándwich de milanesa + gaseosa 500 ml", SistemaBuffet::productosPrecios[20], SistemaBuffet::productosStock[20]);
}

void MainWindow::on_btnVerMenu_clicked()
{
    abrirDetalleProducto(0, SistemaBuffet::menuHoyPlato, "Menú del día. Incluye: " + SistemaBuffet::menuHoyIncluye, SistemaBuffet::menuHoyPrecio, SistemaBuffet::productosStock[0]);
}

void MainWindow::on_btnCheckout_clicked()
{
    // [NEG-04] Carrito no vacío
    if (SistemaBuffet::carritoNombres.isEmpty()) {
        QMessageBox::warning(this, "Carrito Vacío",
            "Tu carrito está actualmente vacío.\nPor favor agrega al menos un producto o menú antes de proceder.");
        return;
    }

    // 1. Abrir diálogo interactivo para ver los productos en el carrito
    CarritoDialog carritoDlg(this);
    if (carritoDlg.exec() == QDialog::Accepted && carritoDlg.fueHaciaElPago()) {
        // 2. Abrir pantalla de verificación de datos y método de pago
        PagoDialog pagoDlg(this);
        if (pagoDlg.exec() == QDialog::Accepted) {
            // 3. Abrir comprobante final con el código único generado
            QString codigo = pagoDlg.getCodigoGenerado();
            ComprobanteDialog compDlg(codigo, this);
            compDlg.exec();
        }
    }

    SistemaBuffet::cargarDesdeBDD(); // con BDD: stock y menú actualizados después de la compra
    mostrarMenuDelDia();
    actualizarBotonCarrito();
}

void MainWindow::on_btnLogout_clicked()
{
    SistemaBuffet::cerrarSesion();
    LoginWindow *loginWin = new LoginWindow();
    loginWin->show();
    this->close();
}

void MainWindow::on_btnPerfil_clicked()
{
    PerfilDialog perfil(this);
    perfil.exec();
    actualizarBotonCarrito(); // por si cargó saldo
}
