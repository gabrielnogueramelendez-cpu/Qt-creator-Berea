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
    ui->lblCalculatedTotal->setText("Total a pagar: $" + QString::number(total, 'f', 0));
}

void ProductDetailDialog::on_btnPlus_clicked()
{
    // [NEG-02] Límite máximo razonable por pedido escolar (máx 10 unidades)
    const int LIMITE_MAXIMO_PEDIDO = 10;
    int topePermitido = qMin(LIMITE_MAXIMO_PEDIDO, m_stockDisponible);

    // [NEG-03] Control de stock disponible
    if (m_cantidad >= topePermitido) {
        if (m_stockDisponible < LIMITE_MAXIMO_PEDIDO) {
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
    QString errorMsg;
    bool agregado = SistemaBuffet::instancia().agregarAlCarrito(m_productoId, m_nombre, m_precioUnitario, m_cantidad, m_stockDisponible, errorMsg);

    if (!agregado) {
        QMessageBox::warning(this, "No se pudo agregar al carrito", errorMsg);
        return;
    }

    QMessageBox::information(this, "Añadido al Carrito",
        QString("Se agregaron %1 unidad(es) de '%2' a tu Carrito Express.").arg(m_cantidad).arg(m_nombre));
    accept();
}
