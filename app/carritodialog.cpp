#include "carritodialog.h"
#include "ui_carritodialog.h"
#include "sistemabuffet.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QTableWidgetItem>

CarritoDialog::CarritoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CarritoDialog),
    m_haciaElPago(false)
{
    ui->setupUi(this);
    setWindowTitle("🛒 Mi Carrito - El Buffet de Alejandro");

    ui->tableCarrito->setColumnCount(5);
    QStringList headers;
    headers << "ID" << "Producto" << "Precio Unit." << "Cantidad" << "Subtotal";
    ui->tableCarrito->setHorizontalHeaderLabels(headers);
    ui->tableCarrito->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);

    recargarTablaCarrito();
}

CarritoDialog::~CarritoDialog()
{
    delete ui;
}

void CarritoDialog::recargarTablaCarrito()
{
    const auto items = SistemaBuffet::instancia().getCarrito();
    ui->tableCarrito->setRowCount(0);

    for (int i = 0; i < items.size(); ++i) {
        ui->tableCarrito->insertRow(i);
        ui->tableCarrito->setItem(i, 0, new QTableWidgetItem(QString::number(items[i].productoId)));
        ui->tableCarrito->setItem(i, 1, new QTableWidgetItem(items[i].nombre));
        ui->tableCarrito->setItem(i, 2, new QTableWidgetItem("$" + QString::number(items[i].precioUnitario, 'f', 0)));
        ui->tableCarrito->setItem(i, 3, new QTableWidgetItem(QString::number(items[i].cantidad)));
        ui->tableCarrito->setItem(i, 4, new QTableWidgetItem("$" + QString::number(items[i].subtotal(), 'f', 0)));
    }

    double total = SistemaBuffet::instancia().getTotalCarrito();
    int cantTotal = SistemaBuffet::instancia().getCantidadTotalCarrito();
    ui->lblTotalPagar->setText(QString("Total (%1 ítems): $%2")
        .arg(cantTotal)
        .arg(QString::number(total, 'f', 0)));

    ui->btnProcederPago->setEnabled(!items.isEmpty());
    ui->btnVaciarCarrito->setEnabled(!items.isEmpty());
    ui->btnModificarCantidad->setEnabled(!items.isEmpty());
    ui->btnEliminarItem->setEnabled(!items.isEmpty());
}

void CarritoDialog::on_btnModificarCantidad_clicked()
{
    int fila = ui->tableCarrito->currentRow();
    if (fila < 0) {
        QMessageBox::warning(this, "Selección Requerida", "Por favor selecciona un producto de la tabla para modificar su cantidad.");
        return;
    }

    int id = ui->tableCarrito->item(fila, 0)->text().toInt();
    QString nombre = ui->tableCarrito->item(fila, 1)->text();
    int cantActual = ui->tableCarrito->item(fila, 3)->text().toInt();

    bool ok = false;
    int nuevaCant = QInputDialog::getInt(this, "Modificar Cantidad",
        QString("Ingresa la nueva cantidad para '%1':\n(Mínimo 1, Máximo 10 unidades)").arg(nombre),
        cantActual, 1, 10, 1, &ok);

    if (ok) {
        QString errorMsg;
        if (!SistemaBuffet::instancia().modificarCantidadCarrito(id, nuevaCant, errorMsg)) {
            QMessageBox::warning(this, "No se pudo actualizar", errorMsg);
            return;
        }
        recargarTablaCarrito();
    }
}

void CarritoDialog::on_btnEliminarItem_clicked()
{
    int fila = ui->tableCarrito->currentRow();
    if (fila < 0) {
        QMessageBox::warning(this, "Selección Requerida", "Selecciona el producto que deseas quitar de tu carrito.");
        return;
    }

    int id = ui->tableCarrito->item(fila, 0)->text().toInt();
    QString nombre = ui->tableCarrito->item(fila, 1)->text();

    QMessageBox::StandardButton resp = QMessageBox::question(this, "Confirmar Eliminación",
        QString("¿Deseas quitar '%1' de tu carrito de compras?").arg(nombre),
        QMessageBox::Yes | QMessageBox::No);

    if (resp == QMessageBox::Yes) {
        SistemaBuffet::instancia().eliminarDelCarrito(id);
        recargarTablaCarrito();
    }
}

void CarritoDialog::on_btnVaciarCarrito_clicked()
{
    if (SistemaBuffet::instancia().getCarrito().isEmpty()) {
        return;
    }

    QMessageBox::StandardButton resp = QMessageBox::question(this, "Vaciar Carrito",
        "¿Estás seguro de que deseas eliminar TODOS los productos de tu carrito?",
        QMessageBox::Yes | QMessageBox::No);

    if (resp == QMessageBox::Yes) {
        SistemaBuffet::instancia().vaciarCarrito();
        recargarTablaCarrito();
    }
}

void CarritoDialog::on_btnSeguirComprando_clicked()
{
    reject();
}

void CarritoDialog::on_btnProcederPago_clicked()
{
    if (SistemaBuffet::instancia().getCarrito().isEmpty()) {
        QMessageBox::warning(this, "Carrito Vacío", "Tu carrito no contiene productos para pagar.");
        return;
    }
    m_haciaElPago = true;
    accept();
}
