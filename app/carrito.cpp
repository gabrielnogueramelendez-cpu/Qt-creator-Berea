// carrito.cpp: Carrito: cambiar cantidades, quitar productos e ir al pago.  Diseño: carritodialog.ui

#include "pantallas.h"
#include "ui_carritodialog.h"
#include "datos.h"
#include <QDialog>
#include <QMessageBox>
#include <QTableWidgetItem>

// ===== BLOQUE 1: VARIABLES DE LA PANTALLA =====

static QDialog *dialogo = NULL;
static Ui::CarritoDialog *ui = NULL;
static bool hacia_el_pago = false;

// ===== BLOQUE 2: FUNCIONES DE LOS BOTONES Y LA TABLA =====

// Sirve para dibujar la tabla del carrito (un renglón por producto) y el total.
static void mostrar_carrito()
{
    ui->tableCarrito->setRowCount(0);
    for (int i = 0; i < carrito_ids.size(); i++) {
        ui->tableCarrito->insertRow(i);
        // id 0 = menú del día
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

// Sirve para que, al tocar una fila, el selector muestre la cantidad de ese producto.
static void elegir_fila(int fila, int)
{
    ui->spinCantidad->setValue(carrito_cantidades[fila]);
}

// Sirve para cambiar la cantidad del producto elegido (máximo 10 y no más que el stock).
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

// Sirve para quitar del carrito el producto elegido (pide confirmación).
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

// Sirve para borrar todos los productos del carrito (pide confirmación).
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

// Sirve para volver al catálogo sin pagar.
static void seguir_comprando()
{
    dialogo->reject();
}

// Sirve para cerrar el carrito avisando que hay que abrir el pago.
static void ir_al_pago()
{
    if (carrito_ids.isEmpty()) {
        QMessageBox::warning(dialogo, "Carrito Vacío", "Tu carrito no contiene productos para pagar.");
        return;
    }
    hacia_el_pago = true;
    dialogo->accept();
}

// ===== BLOQUE 3: APERTURA DE LA PANTALLA =====

// Sirve para abrir el carrito: arma la tabla, conecta los botones y espera.
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
