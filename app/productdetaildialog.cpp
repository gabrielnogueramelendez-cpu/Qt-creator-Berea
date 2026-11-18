#include "productdetaildialog.h"
#include "ui_productdetaildialog.h"
#include <QMessageBox>

// [NEG-02] Máximo de unidades de un mismo producto por pedido
const int LIMITE_POR_PEDIDO = 10;

// =====================================================================
// Constructor: muestra los datos del producto elegido
// =====================================================================
ProductDetailDialog::ProductDetailDialog(Producto productoElegido, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProductDetailDialog)
{
    ui->setupUi(this);
    setWindowTitle("Detalle de Producto - " + productoElegido.nombre);

    producto = productoElegido;
    cantidadElegida = 1;

    ui->lblTitle->setText(producto.nombre);
    ui->lblDescription->setText(producto.descripcion);
    ui->lblPriceTag->setText("$" + QString::number(producto.precio, 'f', 0));
    ui->lblCantidad->setText(QString::number(cantidadElegida));

    // Foto del producto (el archivo se llama igual que el producto)
    QPixmap foto(":/imagenes/imagenes/" + producto.nombre + ".jpg");
    if (!foto.isNull()) {
        foto = foto.scaled(ui->lblFotoProducto->size(), Qt::KeepAspectRatio);
        ui->lblFotoProducto->setPixmap(foto);
    }

    actualizarTotal();
}

ProductDetailDialog::~ProductDetailDialog()
{
    delete ui;
}

// =====================================================================
// maximoPermitido: cuántas unidades se pueden llevar en total
// Es el límite por pedido (10), o el stock si queda menos que eso
// =====================================================================
int ProductDetailDialog::maximoPermitido()
{
    if (producto.stock < LIMITE_POR_PEDIDO) {
        return producto.stock;
    } else {
        return LIMITE_POR_PEDIDO;
    }
}

// =====================================================================
// actualizarTotal: precio x cantidad
// =====================================================================
void ProductDetailDialog::actualizarTotal()
{
    double total = producto.precio * cantidadElegida;
    ui->lblCalculatedTotal->setText("Total: $" + QString::number(total, 'f', 0));
}

// =====================================================================
// Botón +: suma una unidad, sin pasar el máximo ni el stock
// (también cuenta lo que ya está en el carrito)
// =====================================================================
void ProductDetailDialog::on_btnPlus_clicked()
{
    int yaEnCarrito = SistemaBuffet::getCantidadEnCarrito(producto.id);
    int todaviaPuede = maximoPermitido() - yaEnCarrito;

    // [NEG-03] Control de stock disponible
    if (cantidadElegida >= todaviaPuede) {
        if (yaEnCarrito > 0) {
            QMessageBox::warning(this, "Límite Alcanzado",
                "Ya tenés " + QString::number(yaEnCarrito) + " unidad(es) en el carrito. No podés sumar más de este producto.");
        } else if (producto.stock < LIMITE_POR_PEDIDO) {
            QMessageBox::warning(this, "Stock Límite",
                "Solo quedan " + QString::number(producto.stock) + " unidad(es) disponibles en stock para este producto.");
        } else {
            QMessageBox::warning(this, "Límite por Pedido",
                "Por políticas del buffet escolar, el límite máximo es de " + QString::number(LIMITE_POR_PEDIDO) + " unidades por producto por pedido.");
        }
        return;
    }

    cantidadElegida++;
    ui->lblCantidad->setText(QString::number(cantidadElegida));
    actualizarTotal();
}

// =====================================================================
// Botón -: resta una unidad
// [NEG-01] La cantidad mínima es 1
// =====================================================================
void ProductDetailDialog::on_btnMinus_clicked()
{
    if (cantidadElegida > 1) {
        cantidadElegida--;
        ui->lblCantidad->setText(QString::number(cantidadElegida));
        actualizarTotal();
    }
}

// =====================================================================
// Botón AGREGAR AL CARRITO
// [NEG-03] No superar el stock ni el límite de 10 sumando lo del carrito
// =====================================================================
void ProductDetailDialog::on_btnAddToCart_clicked()
{
    int yaEnCarrito = SistemaBuffet::getCantidadEnCarrito(producto.id);

    if (yaEnCarrito + cantidadElegida > maximoPermitido()) {
        QMessageBox::warning(this, "Sin Stock Suficiente",
            "Ya tenés " + QString::number(yaEnCarrito) + " en el carrito. Como máximo podés llevar "
            + QString::number(maximoPermitido()) + " de este producto.");
        return;
    }

    SistemaBuffet::agregarAlCarrito(producto.id, producto.nombre, producto.precio, cantidadElegida);
    accept();
}
