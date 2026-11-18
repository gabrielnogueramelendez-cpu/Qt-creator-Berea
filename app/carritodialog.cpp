#include "carritodialog.h"
#include "ui_carritodialog.h"
#include "sistemabuffet.h"
#include <QMessageBox>
#include <QTableWidgetItem>

// =====================================================================
// Constructor: arma la tabla del carrito
// =====================================================================
CarritoDialog::CarritoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CarritoDialog)
{
    ui->setupUi(this);
    setWindowTitle("Mi Carrito - El Buffet de Alejandro");

    eligioPagar = false;

    // Columnas de la tabla
    ui->tableCarrito->setColumnCount(5);
    QStringList titulos;
    titulos << "ID" << "Producto" << "Precio Unit." << "Cantidad" << "Subtotal";
    ui->tableCarrito->setHorizontalHeaderLabels(titulos);
    ui->tableCarrito->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);

    // Cantidad por producto: mínimo 1, máximo 10 (el stock se controla al actualizar)
    ui->spinCantidad->setRange(1, 10);

    recargarTablaCarrito();
}

CarritoDialog::~CarritoDialog()
{
    delete ui;
}

// =====================================================================
// recargarTablaCarrito: vuelve a llenar la tabla con lo que hay en el carrito
// Cada celda es un QTableWidgetItem nuevo; la tabla lo guarda y lo borra sola
// =====================================================================
void CarritoDialog::recargarTablaCarrito()
{
    ui->tableCarrito->setRowCount(0);

    for (int i = 0; i < SistemaBuffet::carrito.size(); i++) {
        ItemCarrito item = SistemaBuffet::carrito[i];

        // El id 0 es el menú del día
        QString textoID;
        if (item.id == 0) {
            textoID = "Menú";
        } else {
            textoID = QString::number(item.id);
        }
        double subtotal = item.precio * item.cantidad;

        ui->tableCarrito->insertRow(i);
        ui->tableCarrito->setItem(i, 0, new QTableWidgetItem(textoID));
        ui->tableCarrito->setItem(i, 1, new QTableWidgetItem(item.nombre));
        ui->tableCarrito->setItem(i, 2, new QTableWidgetItem("$" + QString::number(item.precio, 'f', 0)));
        ui->tableCarrito->setItem(i, 3, new QTableWidgetItem(QString::number(item.cantidad)));
        ui->tableCarrito->setItem(i, 4, new QTableWidgetItem("$" + QString::number(subtotal, 'f', 0)));
    }

    double total = SistemaBuffet::getTotalCarrito();
    int cantidadTotal = SistemaBuffet::getCantidadTotalCarrito();
    ui->lblTotalPagar->setText("Total (" + QString::number(cantidadTotal) + " ítems): $" + QString::number(total, 'f', 0));

    // Con el carrito vacío no se puede pagar ni modificar nada
    bool hayProductos = !SistemaBuffet::carrito.isEmpty();
    ui->btnProcederPago->setEnabled(hayProductos);
    ui->btnVaciarCarrito->setEnabled(hayProductos);
    ui->btnModificarCantidad->setEnabled(hayProductos);
    ui->btnEliminarItem->setEnabled(hayProductos);
}

// =====================================================================
// Botón ACTUALIZAR CANTIDAD del producto elegido en la tabla
// =====================================================================
void CarritoDialog::on_btnModificarCantidad_clicked()
{
    int filaElegida = ui->tableCarrito->currentRow();
    if (filaElegida < 0) {
        QMessageBox::warning(this, "Selección Requerida", "Por favor selecciona un producto de la tabla para modificar su cantidad.");
        return;
    }

    int nuevaCantidad = ui->spinCantidad->value();

    // Máximo: 10 por pedido, o el stock si queda menos
    int productoID = SistemaBuffet::carrito[filaElegida].id;
    int maximo = SistemaBuffet::productos[productoID].stock;
    if (maximo > 10) {
        maximo = 10;
    }

    if (nuevaCantidad > maximo) {
        QMessageBox::warning(this, "Sin Stock Suficiente",
            "Solo podés llevar hasta " + QString::number(maximo) + " unidad(es) de '"
            + SistemaBuffet::carrito[filaElegida].nombre + "'.");
        return;
    }

    SistemaBuffet::carrito[filaElegida].cantidad = nuevaCantidad;
    recargarTablaCarrito();
}

// =====================================================================
// Clic en una fila: el selector muestra la cantidad de ese producto
// =====================================================================
void CarritoDialog::on_tableCarrito_cellClicked(int fila, int columna)
{
    ui->spinCantidad->setValue(SistemaBuffet::carrito[fila].cantidad);
}

// =====================================================================
// Botón QUITAR: saca el producto elegido del carrito
// =====================================================================
void CarritoDialog::on_btnEliminarItem_clicked()
{
    int filaElegida = ui->tableCarrito->currentRow();
    if (filaElegida < 0) {
        QMessageBox::warning(this, "Selección Requerida", "Selecciona el producto que deseas quitar de tu carrito.");
        return;
    }

    QString nombre = SistemaBuffet::carrito[filaElegida].nombre;
    int respuesta = QMessageBox::question(this, "Confirmar Eliminación",
        "¿Deseas quitar '" + nombre + "' de tu carrito de compras?",
        QMessageBox::Yes | QMessageBox::No);

    if (respuesta == QMessageBox::Yes) {
        SistemaBuffet::carrito.removeAt(filaElegida);
        recargarTablaCarrito();
    }
}

// =====================================================================
// Botón VACIAR CARRITO
// =====================================================================
void CarritoDialog::on_btnVaciarCarrito_clicked()
{
    if (SistemaBuffet::carrito.isEmpty()) {
        return;
    }

    int respuesta = QMessageBox::question(this, "Vaciar Carrito",
        "¿Estás seguro de que deseas eliminar TODOS los productos de tu carrito?",
        QMessageBox::Yes | QMessageBox::No);

    if (respuesta == QMessageBox::Yes) {
        SistemaBuffet::vaciarCarrito();
        recargarTablaCarrito();
    }
}

// =====================================================================
// Botón SEGUIR COMPRANDO: vuelve al catálogo
// =====================================================================
void CarritoDialog::on_btnSeguirComprando_clicked()
{
    reject();
}

// =====================================================================
// Botón PROCEDER AL PAGO
// =====================================================================
void CarritoDialog::on_btnProcederPago_clicked()
{
    if (SistemaBuffet::carrito.isEmpty()) {
        QMessageBox::warning(this, "Carrito Vacío", "Tu carrito no contiene productos para pagar.");
        return;
    }
    eligioPagar = true;
    accept();
}
