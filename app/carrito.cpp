#include "pantallas.h"
#include "ui_carritodialog.h"
#include "datos.h"
#include <QDialog>
#include <QMessageBox>
#include <QTableWidgetItem>

// Carrito de compras (carritodialog.ui)
static QDialog *dialogo = NULL;
static Ui::CarritoDialog *ui = NULL;
static bool hacia_el_pago = false;

static void mostrar_carrito()
{
    ui->tableCarrito->setRowCount(0);
    for (int i = 0; i < carrito_ids.size(); i++) {
        ui->tableCarrito->insertRow(i);
        // El id 0 es el menú del día
        QString id = (carrito_ids[i] == 0) ? QString("Menú") : QString::number(carrito_ids[i]);
        ui->tableCarrito->setItem(i, 0, new QTableWidgetItem(id));
        ui->tableCarrito->setItem(i, 1, new QTableWidgetItem(carrito_nombres[i]));
        ui->tableCarrito->setItem(i, 2, new QTableWidgetItem(pesos(carrito_precios[i])));
        ui->tableCarrito->setItem(i, 3, new QTableWidgetItem(QString::number(carrito_cantidades[i])));
        ui->tableCarrito->setItem(i, 4, new QTableWidgetItem(pesos(carrito_precios[i] * carrito_cantidades[i])));
    }

    ui->lblTotalPagar->setText(QString("Total (%1 ítems): %2").arg(cantidad_total_carrito()).arg(pesos(total_carrito())));

    bool hayItems = !carrito_ids.isEmpty();
    ui->btnProcederPago->setEnabled(hayItems);
    ui->btnVaciarCarrito->setEnabled(hayItems);
    ui->btnModificarCantidad->setEnabled(hayItems);
    ui->btnEliminarItem->setEnabled(hayItems);
}

static void elegir_fila(int fila, int)
{
    // El selector muestra la cantidad actual de ese producto
    ui->spinCantidad->setValue(carrito_cantidades[fila]);
}

static void modificar_cantidad()
{
    int fila = ui->tableCarrito->currentRow();
    if (fila < 0) {
        QMessageBox::warning(dialogo, "Selección Requerida", "Por favor selecciona un producto de la tabla para modificar su cantidad.");
        return;
    }
    int nueva = ui->spinCantidad->value();
    int maximo = productos_stock[carrito_ids[fila]];
    if (maximo > 10) {
        maximo = 10;
    }
    if (nueva > maximo) {
        QMessageBox::warning(dialogo, "Sin Stock Suficiente",
            QString("Solo podés llevar hasta %1 unidad(es) de '%2'.").arg(maximo).arg(carrito_nombres[fila]));
        return;
    }
    carrito_cantidades[fila] = nueva;
    mostrar_carrito();
}

static void eliminar_item()
{
    int fila = ui->tableCarrito->currentRow();
    if (fila < 0) {
        QMessageBox::warning(dialogo, "Selección Requerida", "Selecciona el producto que deseas quitar de tu carrito.");
        return;
    }
    int respuesta = QMessageBox::question(dialogo, "Confirmar Eliminación",
        QString("¿Deseas quitar '%1' de tu carrito de compras?").arg(carrito_nombres[fila]),
        QMessageBox::Yes | QMessageBox::No);
    if (respuesta == QMessageBox::Yes) {
        quitar_del_carrito(fila);
        mostrar_carrito();
    }
}

static void vaciar()
{
    int respuesta = QMessageBox::question(dialogo, "Vaciar Carrito",
        "¿Estás seguro de que deseas eliminar TODOS los productos de tu carrito?",
        QMessageBox::Yes | QMessageBox::No);
    if (respuesta == QMessageBox::Yes) {
        vaciar_carrito();
        mostrar_carrito();
    }
}

static void seguir_comprando()
{
    dialogo->reject();
}

static void ir_al_pago()
{
    if (carrito_ids.isEmpty()) {
        QMessageBox::warning(dialogo, "Carrito Vacío", "Tu carrito no contiene productos para pagar.");
        return;
    }
    hacia_el_pago = true;
    dialogo->accept();
}

bool abrir_carrito(QWidget *padre)
{
    hacia_el_pago = false;
    dialogo = new QDialog(padre);
    ui = new Ui::CarritoDialog();
    ui->setupUi(dialogo);
    dialogo->setWindowTitle("Mi Carrito - El Buffet de Alejandro");

    ui->tableCarrito->setColumnCount(5);
    ui->tableCarrito->setHorizontalHeaderLabels({"ID", "Producto", "Precio Unit.", "Cantidad", "Subtotal"});
    ui->tableCarrito->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->spinCantidad->setRange(1, 10);

    QObject::connect(ui->tableCarrito, &QTableWidget::cellClicked, elegir_fila);
    QObject::connect(ui->btnModificarCantidad, &QPushButton::clicked, modificar_cantidad);
    QObject::connect(ui->btnEliminarItem, &QPushButton::clicked, eliminar_item);
    QObject::connect(ui->btnVaciarCarrito, &QPushButton::clicked, vaciar);
    QObject::connect(ui->btnSeguirComprando, &QPushButton::clicked, seguir_comprando);
    QObject::connect(ui->btnProcederPago, &QPushButton::clicked, ir_al_pago);

    mostrar_carrito();
    dialogo->exec();

    delete ui;
    delete dialogo;
    ui = NULL;
    dialogo = NULL;
    return hacia_el_pago;
}
