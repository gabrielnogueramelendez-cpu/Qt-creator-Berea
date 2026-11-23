// caja.cpp: Mostrador: buscar un código y entregar o cancelar el pedido.  Diseño: cajerowindow.ui

#include "pantallas.h"
#include "ui_cajerowindow.h"
#include "datos.h"
#include "basedatos.h"
#include <QMessageBox>
#include <QTableWidgetItem>

// ===== BLOQUE 1: VARIABLES DE LA PANTALLA =====

static QWidget *ventana = NULL;
static Ui::CajeroWindow *ui = NULL;
static QString codigo_actual = "";

// Estado_Retiro: 1 PENDIENTE, 2 ENTREGADO, 3 CANCELADO
static const int ENTREGADO = 2;
static const int CANCELADO = 3;

// ===== BLOQUE 2: FUNCIONES DE LOS BOTONES Y LA TABLA =====

// Sirve para liberar el .ui cuando la ventana se cierra.
static void liberar()
{
    delete ui;
    ui = NULL;
    ventana = NULL;
}

// Sirve para leer los pedidos de la BDD y mostrarlos en la tabla.
static void mostrar_pedidos()
{
    cargar_pedidos();
    ui->tableColasPedidos->setRowCount(0);
    for (int i = 0; i < pedidos_codigos.size(); i++) {
        ui->tableColasPedidos->insertRow(i);
        ui->tableColasPedidos->setItem(i, 0, new QTableWidgetItem(pedidos_codigos[i]));
        ui->tableColasPedidos->setItem(i, 1, new QTableWidgetItem(pedidos_clientes[i] + " (" + pedidos_cursos[i] + ")"));
        ui->tableColasPedidos->setItem(i, 2, new QTableWidgetItem(pedidos_recreos[i]));
        ui->tableColasPedidos->setItem(i, 3, new QTableWidgetItem(pesos(pedidos_montos[i])));
        ui->tableColasPedidos->setItem(i, 4, new QTableWidgetItem(pedidos_estados[i]));
    }
}

// Sirve para borrar el detalle de la derecha y desactivar los botones, con un mensaje.
static void limpiar_detalle(const QString &mensaje)
{
    codigo_actual = "";
    ui->lblStatusPedido->setText(mensaje);
    ui->lblDetalleAlumno->setText("");
    ui->lblDetalleRecreo->setText("");
    ui->listDetalleItems->clear();
    ui->lblTotalMonto->setText("");
    ui->btnEntregarPedido->setEnabled(false);
    ui->btnCancelarPedido->setEnabled(false);
}

// Sirve para el botón BUSCAR (o Enter): muestra el pedido de ese código.
static void buscar_codigo()
{
    QString codigo = ui->txtCodigoRetiro->text().trimmed().toUpper();
    if (codigo.isEmpty()) {
        QMessageBox::warning(ventana, "Código Requerido", "Ingresá el código de retiro.");
        return;
    }

    int i = buscar_pedido(codigo);
    if (i == -1) {
        limpiar_detalle("No hay ningún pedido con el código " + codigo + ".");
        return;
    }

    codigo_actual = codigo;
    bool pendiente = (pedidos_estados[i] == "PENDIENTE");
    ui->btnEntregarPedido->setEnabled(pendiente);
    ui->btnCancelarPedido->setEnabled(pendiente);
    if (pendiente) {
        ui->lblStatusPedido->setText("PEDIDO " + codigo + " · PENDIENTE DE ENTREGA");
    } else {
        ui->lblStatusPedido->setText("PEDIDO " + codigo + " · " + pedidos_estados[i] + " · ya no se puede modificar");
    }

    ui->lblDetalleAlumno->setText(QString("Cliente: %1\nDNI %2 · %3").arg(pedidos_clientes[i], pedidos_dnis[i], pedidos_cursos[i]));
    ui->lblDetalleRecreo->setText(QString("Retiro: %1\nPago: %2").arg(pedidos_recreos[i], pedidos_metodos[i]));

    ui->listDetalleItems->clear();
    QStringList lineas = pedidos_detalles[i].split('\n', Qt::SkipEmptyParts);
    for (int j = 0; j < lineas.size(); j++) {
        ui->listDetalleItems->addItem(lineas[j].trimmed());
    }
    ui->lblTotalMonto->setText("Total: " + pesos(pedidos_montos[i]));
}

// Sirve para que, al tocar una fila de la tabla, se busque ese código.
static void elegir_pedido(int fila, int)
{
    ui->txtCodigoRetiro->setText(ui->tableColasPedidos->item(fila, 0)->text());
    buscar_codigo();
}

// Sirve para cambiar el estado del pedido elegido en la BDD.
static void cambiar_estado(int estadoID)
{
    int i = buscar_pedido(codigo_actual);
    if (i == -1) {
        return;
    }
    // Al cancelar, el SP devuelve el stock y el saldo
    MYSQL_RES *result = actualizar_estado_retiro(conexion, usuario_id, pedidos_retiro_ids[i], estadoID);
    if (numero_del_sp(result) == 0) {
        QMessageBox::warning(ventana, "Error", "No se pudo actualizar el pedido en la base de datos.");
    }
    mostrar_pedidos();
    buscar_codigo();
}

// Sirve para el botón MARCAR ENTREGADO.
static void entregar()
{
    cambiar_estado(ENTREGADO);
}

// Sirve para el botón CANCELAR PEDIDO.
static void cancelar()
{
    cambiar_estado(CANCELADO);
}

// Sirve para el botón SALIR: cierra la sesión y vuelve al login.
static void cerrar_sesion_caja()
{
    salir_de_la_cuenta();
    abrir_login();
    ventana->close();
}

// ===== BLOQUE 3: APERTURA DE LA PANTALLA =====

// Sirve para abrir el mostrador.
void abrir_caja()
{
    if (ventana != NULL) {
        ventana->raise();
        return;
    }
    ventana = new QWidget();
    ventana->setAttribute(Qt::WA_DeleteOnClose);
    ui = new Ui::CajeroWindow();
    ui->setupUi(ventana);
    ventana->setWindowTitle("Mostrador - El Buffet de Alejandro");
    QObject::connect(ventana, &QObject::destroyed, liberar);

    ui->btnEntregarPedido->setEnabled(false);
    ui->btnCancelarPedido->setEnabled(false);
    ui->txtCodigoRetiro->setMaxLength(5); // Codigo_Retiro CHAR(5)
    ui->tableColasPedidos->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableColasPedidos->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableColasPedidos->setColumnCount(5);
    ui->tableColasPedidos->setHorizontalHeaderLabels({"Código", "Cliente", "Retiro", "Total", "Estado"});
    ui->tableColasPedidos->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);

    QObject::connect(ui->btnBuscarCodigo, &QPushButton::clicked, buscar_codigo);
    QObject::connect(ui->txtCodigoRetiro, &QLineEdit::returnPressed, buscar_codigo);
    QObject::connect(ui->tableColasPedidos, &QTableWidget::cellClicked, elegir_pedido);
    QObject::connect(ui->btnEntregarPedido, &QPushButton::clicked, entregar);
    QObject::connect(ui->btnCancelarPedido, &QPushButton::clicked, cancelar);
    QObject::connect(ui->btnLogout, &QPushButton::clicked, cerrar_sesion_caja);

    mostrar_pedidos();
    ventana->show();
}
