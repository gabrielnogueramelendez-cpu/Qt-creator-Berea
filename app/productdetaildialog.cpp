#include "productdetaildialog.h"
#include "ui_productdetaildialog.h"
#include "sistemabuffet.h"
#include <QMessageBox>

ProductDetailDialog::ProductDetailDialog(int productoId, const QString &nombre, const QString &desc, double precio, int stockDisponible, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProductDetailDialog),
    m_productoId(productoId),
    m_nombre(nombre),
    m_precioUnitario(precio),
    m_cantidad(1),
    m_stockDisponible(stockDisponible)
{
    ui->setupUi(this);
    setWindowTitle("Detalle de Producto - " + nombre);

    ui->lblTitle->setText(nombre);

    // Foto del producto desde el .qrc (mismo nombre que el producto)
    QPixmap foto(":/imagenes/imagenes/" + nombre + ".jpg");
    if (!foto.isNull()) {
        foto = foto.scaled(ui->lblFotoProducto->size(), Qt::KeepAspectRatio);
        ui->lblFotoProducto->setPixmap(foto);
    }
    ui->lblDescription->setText(desc);
    ui->lblPriceTag->setText("$" + QString::number(precio, 'f', 0));
    ui->lblCantidad->setText(QString::number(m_cantidad));
    actualizarTotal();
}

ProductDetailDialog::~ProductDetailDialog()
{
    delete ui;
}

void ProductDetailDialog::actualizarTotal()
{
    double total = m_precioUnitario * m_cantidad;
    ui->lblCalculatedTotal->setText("Total: $" + QString::number(total, 'f', 0));
}

void ProductDetailDialog::on_btnPlus_clicked()
{
    // [NEG-02] Límite máximo razonable por pedido escolar (máx 10 unidades)
    const int LIMITE_MAXIMO_PEDIDO = 10;
    int enCarrito = SistemaBuffet::getCantidadEnCarrito(m_productoId);
    int topePermitido = qMin(LIMITE_MAXIMO_PEDIDO, m_stockDisponible) - enCarrito;

    // [NEG-03] Control de stock disponible
    if (m_cantidad >= topePermitido) {
        if (enCarrito > 0) {
            QMessageBox::warning(this, "Límite Alcanzado",
                QString("Ya tenés %1 unidad(es) en el carrito. No podés sumar más de este producto.").arg(enCarrito));
        } else if (m_stockDisponible < LIMITE_MAXIMO_PEDIDO) {
            QMessageBox::warning(this, "Stock Límite",
                QString("Solo quedan %1 unidad(es) disponibles en stock para este producto.").arg(m_stockDisponible));
        } else {
            QMessageBox::warning(this, "Límite por Pedido",
                QString("Por políticas del buffet escolar, el límite máximo es de %1 unidades por producto por pedido.").arg(LIMITE_MAXIMO_PEDIDO));
        }
        return;
    }

    m_cantidad++;
    ui->lblCantidad->setText(QString::number(m_cantidad));
    actualizarTotal();
}

void ProductDetailDialog::on_btnMinus_clicked()
{
    // [NEG-01] Cantidad mínima de compra no puede ser menor a 1
    if (m_cantidad > 1) {
        m_cantidad--;
        ui->lblCantidad->setText(QString::number(m_cantidad));
        actualizarTotal();
    }
}

void ProductDetailDialog::on_btnAddToCart_clicked()
{
    // [NEG-03] No superar stock ni el límite de 10 sumando lo que ya está en el carrito
    int enCarrito = SistemaBuffet::getCantidadEnCarrito(m_productoId);
    if (enCarrito + m_cantidad > qMin(10, m_stockDisponible)) {
        QMessageBox::warning(this, "Sin Stock Suficiente",
            QString("Ya tenés %1 en el carrito. Como máximo podés llevar %2 de este producto.")
                .arg(enCarrito).arg(qMin(10, m_stockDisponible)));
        return;
    }

    SistemaBuffet::agregarAlCarrito(m_productoId, m_nombre, m_precioUnitario, m_cantidad);
    accept();
}
