#include "carritodialog.h"
#include "ui_carritodialog.h"
#include "sistemabuffet.h"
#include <QMessageBox>
#include <QTableWidgetItem>

CarritoDialog::CarritoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CarritoDialog),
    m_haciaElPago(false)
{
    ui->setupUi(this);
    setWindowTitle("Mi Carrito - El Buffet de Alejandro");

    ui->tableCarrito->setColumnCount(5);
    QStringList headers;
    headers << "ID" << "Producto" << "Precio Unit." << "Cantidad" << "Subtotal";
    ui->tableCarrito->setHorizontalHeaderLabels(headers);
    ui->tableCarrito->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);

    // Cantidad por producto: mínimo 1, máximo 10 (el stock se valida al actualizar)
    ui->spinCantidad->setRange(1, 10);

    recargarTablaCarrito();
}

CarritoDialog::~CarritoDialog()
{
    delete ui;
}

void CarritoDialog::recargarTablaCarrito()
{
    ui->tableCarrito->setRowCount(0);

    for (int i = 0; i < SistemaBuffet::carritoIds.size(); ++i) {
        ui->tableCarrito->insertRow(i);
        // El id 0 es el menú del día
        QString id = SistemaBuffet::carritoIds[i] == 0 ? QString("Menú") : QString::number(SistemaBuffet::carritoIds[i]);
        ui->tableCarrito->setItem(i, 0, new QTableWidgetItem(id));
        ui->tableCarrito->setItem(i, 1, new QTableWidgetItem(SistemaBuffet::carritoNombres[i]));
        ui->tableCarrito->setItem(i, 2, new QTableWidgetItem("$" + QString::number(SistemaBuffet::carritoPrecios[i], 'f', 0)));
        ui->tableCarrito->setItem(i, 3, new QTableWidgetItem(QString::number(SistemaBuffet::carritoCantidades[i])));
        double subtotal = SistemaBuffet::carritoPrecios[i] * SistemaBuffet::carritoCantidades[i];
        ui->tableCarrito->setItem(i, 4, new QTableWidgetItem("$" + QString::number(subtotal, 'f', 0)));
    }

    double total = SistemaBuffet::getTotalCarrito();
    int cantTotal = SistemaBuffet::getCantidadTotalCarrito();
    ui->lblTotalPagar->setText(QString("Total (%1 ítems): $%2")
        .arg(cantTotal)
        .arg(QString::number(total, 'f', 0)));

    bool hayItems = !SistemaBuffet::carritoIds.isEmpty();
    ui->btnProcederPago->setEnabled(hayItems);
    ui->btnVaciarCarrito->setEnabled(hayItems);
    ui->btnModificarCantidad->setEnabled(hayItems);
    ui->btnEliminarItem->setEnabled(hayItems);
}

void CarritoDialog::on_btnModificarCantidad_clicked()
{
    int fila = ui->tableCarrito->currentRow();
    if (fila < 0) {
        QMessageBox::warning(this, "Selección Requerida", "Por favor selecciona un producto de la tabla para modificar su cantidad.");
        return;
    }

    int nuevaCant = ui->spinCantidad->value();
    int maximo = qMin(10, SistemaBuffet::productosStock[SistemaBuffet::carritoIds[fila]]);
    if (nuevaCant > maximo) {
        QMessageBox::warning(this, "Sin Stock Suficiente",
            QString("Solo podés llevar hasta %1 unidad(es) de '%2'.").arg(maximo).arg(SistemaBuffet::carritoNombres[fila]));
        return;
    }

    SistemaBuffet::carritoCantidades[fila] = nuevaCant;
    recargarTablaCarrito();
}

void CarritoDialog::on_tableCarrito_cellClicked(int row, int column)
{
    // Al elegir una fila, el selector muestra la cantidad actual de ese producto
    ui->spinCantidad->setValue(SistemaBuffet::carritoCantidades[row]);
}

void CarritoDialog::on_btnEliminarItem_clicked()
{
    int fila = ui->tableCarrito->currentRow();
    if (fila < 0) {
        QMessageBox::warning(this, "Selección Requerida", "Selecciona el producto que deseas quitar de tu carrito.");
        return;
    }

    QString nombre = SistemaBuffet::carritoNombres[fila];

    QMessageBox::StandardButton resp = QMessageBox::question(this, "Confirmar Eliminación",
        QString("¿Deseas quitar '%1' de tu carrito de compras?").arg(nombre),
        QMessageBox::Yes | QMessageBox::No);

    if (resp == QMessageBox::Yes) {
        SistemaBuffet::carritoIds.removeAt(fila);
        SistemaBuffet::carritoNombres.removeAt(fila);
        SistemaBuffet::carritoPrecios.removeAt(fila);
        SistemaBuffet::carritoCantidades.removeAt(fila);
        recargarTablaCarrito();
    }
}

void CarritoDialog::on_btnVaciarCarrito_clicked()
{
    if (SistemaBuffet::carritoIds.isEmpty()) {
        return;
    }

    QMessageBox::StandardButton resp = QMessageBox::question(this, "Vaciar Carrito",
        "¿Estás seguro de que deseas eliminar TODOS los productos de tu carrito?",
        QMessageBox::Yes | QMessageBox::No);

    if (resp == QMessageBox::Yes) {
        SistemaBuffet::vaciarCarrito();
        recargarTablaCarrito();
    }
}

void CarritoDialog::on_btnSeguirComprando_clicked()
{
    reject();
}

void CarritoDialog::on_btnProcederPago_clicked()
{
    if (SistemaBuffet::carritoIds.isEmpty()) {
        QMessageBox::warning(this, "Carrito Vacío", "Tu carrito no contiene productos para pagar.");
        return;
    }
    m_haciaElPago = true;
    accept();
}
