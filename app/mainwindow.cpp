#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "productdetaildialog.h"
#include "carritodialog.h"
#include "pagodialog.h"
#include "comprobantedialog.h"
#include "loginwindow.h"
#include "sistemabuffet.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("El Buffet de Alejandro - Menú Gourmet Escolar");
    ui->tabWidget->setCurrentIndex(0);

    // Saludo personalizado con el usuario activo
    UsuarioSistema actual = SistemaBuffet::instancia().getUsuarioActual();
    if (!actual.nombre.isEmpty()) {
        ui->lblUserGreeting->setText(QString("🎓 %1 %2 (%3) | Saldo: $%4")
            .arg(actual.nombre)
            .arg(actual.apellido)
            .arg(actual.curso)
            .arg(QString::number(actual.saldoCuenta, 'f', 0)));
    }

    actualizarBotonCarrito();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::actualizarBotonCarrito()
{
    int cant = SistemaBuffet::instancia().getCantidadTotalCarrito();
    double total = SistemaBuffet::instancia().getTotalCarrito();
    ui->btnCheckout->setText(QString("🛒 VER MI PEDIDO (%1 ítems - $%2) ➔")
        .arg(cant)
        .arg(QString::number(total, 'f', 0)));
}

void MainWindow::abrirDetalleProducto(int id, const QString &nombre, const QString &desc, double precio, int stock)
{
    // Obtener stock actualizado del sistema si existe
    bool encontrado = false;
    ProductoBuffet p = SistemaBuffet::instancia().getProductoPorId(id, encontrado);
    if (encontrado) {
        stock = p.stock;
        precio = p.precio;
    }

    ProductDetailDialog dlg(id, nombre, desc, precio, stock, this);
    if (dlg.exec() == QDialog::Accepted) {
        actualizarBotonCarrito();
    }
}

void MainWindow::on_btnVerDetalleMenuDia1_clicked()
{
    abrirDetalleProducto(1,
        "🌟 Menu del Dia N° 1: Combo Almuerzo Gourmet",
        "Incluye: Milanesa a la Napolitana con papas rústicas al horno + Bebida 500ml a elección + Postre o Fruta de estación.",
        8500.0, 20
    );
}

void MainWindow::on_btnVerDetalleMenuDia2_clicked()
{
    abrirDetalleProducto(2,
        "🥗 Menu del Dia N° 2: Combo Express Saludable",
        "Incluye: Porción de Tarta Rústica de Verdura y Queso + Ensalada fresca de estación + Agua Mineral 500ml.",
        5200.0, 15
    );
}

void MainWindow::on_btnVerDetalleMilanesa_clicked()
{
    abrirDetalleProducto(3,
        "🥪 Sándwich de Milanesa Ciabatta Completo",
        "Milanesa tierna de ternera, lechuga fresca, tomate, queso tybo y aderezo artesanal en pan ciabatta crocante horneado en el día.",
        2800.0, 35
    );
}

void MainWindow::on_btnVerDetalleEmpanada_clicked()
{
    abrirDetalleProducto(4,
        "🥟 Empanada de Carne Cortada a Cuchillo",
        "Empanada tradicional horneada, rellena de ternera seleccionada cortada a cuchillo, cebolla de verdeo y especias criollas.",
        1200.0, 40
    );
}

void MainWindow::on_btnVerDetalleTarta_clicked()
{
    abrirDetalleProducto(5,
        "🥧 Tarta Rústica de Jamón y Queso",
        "Porción abundante de tarta casera con doble capa de queso mozzarella fundido, jamón cocido de primera y masa hojaldrada.",
        2400.0, 18
    );
}

void MainWindow::on_btnVerDetallePebete_clicked()
{
    abrirDetalleProducto(6,
        "🥖 Pebete Tradicional de Jamón y Queso",
        "Pan pebete artesanal suave, abundante jamón cocido, queso tybo en fetas y manteca de primera calidad.",
        1800.0, 25
    );
}

void MainWindow::on_btnVerDetalleAlfajor_clicked()
{
    abrirDetalleProducto(7,
        "🍫 Alfajor Artesanal de Dulce de Leche",
        "Dos tapas suavemente horneadas con relleno abundante de dulce de leche repostero, bañado en fino chocolate semi-amargo.",
        1100.0, 50
    );
}

void MainWindow::on_btnVerDetallePastafrola_clicked()
{
    abrirDetalleProducto(8,
        "🥮 Porción de Pasta Frola Artesanal",
        "Receta tradicional casera con dulce de membrillo especial y enrejado de masa sablée de manteca.",
        1500.0, 22
    );
}

void MainWindow::on_btnVerDetalleFactura_clicked()
{
    abrirDetalleProducto(9,
        "🥐 Factura de Manteca Seleccionada",
        "Medialuna u opción hojaldrada artesanal horneada cada mañana en el buffet.",
        600.0, 60
    );
}

void MainWindow::on_btnVerDetalleMuffin_clicked()
{
    abrirDetalleProducto(10,
        "🧁 Muffin de Chocolate con Corazón Dulce",
        "Esponjoso muffin de cacao intenso con chispas de chocolate y corazón de dulce de leche.",
        1400.0, 30
    );
}

void MainWindow::on_btnVerDetalleGaseosa_clicked()
{
    abrirDetalleProducto(11,
        "🥤 Gaseosa Línea Coca-Cola 500ml",
        "Botella de 500ml bien fría (Coca-Cola, Sprite o Fanta a elección al retirar).",
        1300.0, 45
    );
}

void MainWindow::on_btnVerDetalleAgua_clicked()
{
    abrirDetalleProducto(12,
        "💧 Agua Mineral Villavicencio 500ml",
        "Agua mineral de manantial sin gas de 500ml.",
        900.0, 50
    );
}

void MainWindow::on_btnVerDetalleJugo_clicked()
{
    abrirDetalleProducto(13,
        "🍊 Jugo Natural Exprimido de Naranja 350ml",
        "Jugo 100% natural de naranjas seleccionadas exprimidas en el día, sin conservantes.",
        1600.0, 25
    );
}

void MainWindow::on_btnVerDetalleLicuado_clicked()
{
    abrirDetalleProducto(14,
        "🍓 Licuado Frutal de Frutilla y Banana 400ml",
        "Licuado con leche o agua y fruta fresca de estación preparado en el momento.",
        2200.0, 20
    );
}

void MainWindow::on_btnCheckout_clicked()
{
    // [NEG-04] Carrito no vacío
    if (SistemaBuffet::instancia().getCarrito().isEmpty()) {
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

    actualizarBotonCarrito();
}

void MainWindow::on_btnLogout_clicked()
{
    SistemaBuffet::instancia().cerrarSesion();
    LoginWindow *loginWin = new LoginWindow();
    loginWin->show();
    this->close();
}
