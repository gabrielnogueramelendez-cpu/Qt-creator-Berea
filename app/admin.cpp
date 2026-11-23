// admin.cpp: Administración: alta, precio, stock, baja y auditoría.  Diseño: adminwindow.ui

#include "pantallas.h"
#include "ui_adminwindow.h"
#include "datos.h"
#include "basedatos.h"
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QIntValidator>

// ===== BLOQUE 1: VARIABLES DE LA PANTALLA =====

static QWidget *ventana = NULL;
static Ui::AdminWindow *ui = NULL;

// ===== BLOQUE 2: FUNCIONES QUE MUESTRAN DATOS =====

// Sirve para liberar el .ui cuando la ventana se cierra.
static void liberar()
{
    delete ui;
    ui = NULL;
    ventana = NULL;
}

// Sirve para mostrar todos los productos de la BDD en la tabla.
static void mostrar_inventario()
{
    ui->tableProductosAdmin->setRowCount(0);
    MYSQL_RES *result = obtener_productos(conexion);
    if (result == NULL) {
        return;
    }
    MYSQL_ROW fila;
    int i = 0;
    while ((fila = mysql_fetch_row(result)) != NULL) {
        ui->tableProductosAdmin->insertRow(i);
        for (int columna = 0; columna < 6; columna++) {
            QString texto = valor_fila(fila, columna);
            if (columna == 3) {
                texto = QString::number(texto.toDouble(), 'f', 0); // el precio sin los ,00
            }
            QTableWidgetItem *celda = new QTableWidgetItem(texto);
            celda->setData(Qt::UserRole, texto);
            ui->tableProductosAdmin->setItem(i, columna, celda);
        }
        i++;
    }
    mysql_free_result(result);
}

// Sirve para saber si el usuario editó una celda (texto actual distinto del original).
static bool cambio(int fila, int columna)
{
    QTableWidgetItem *celda = ui->tableProductosAdmin->item(fila, columna);
    return celda->text() != celda->data(Qt::UserRole).toString();
}

// Sirve para mostrar los últimos 50 eventos de auditoría.
static void mostrar_logs()
{
    ui->tableLogsAdmin->setRowCount(0);
    MYSQL_RES *result = obtener_logs(conexion);
    if (result == NULL) {
        return;
    }
    MYSQL_ROW fila;
    int i = 0;
    while ((fila = mysql_fetch_row(result)) != NULL) {
        ui->tableLogsAdmin->insertRow(i);
        for (int columna = 0; columna < 3; columna++) {
            ui->tableLogsAdmin->setItem(i, columna, new QTableWidgetItem(valor_fila(fila, columna)));
        }
        i++;
    }
    mysql_free_result(result);
}

// Sirve para dejar vacío el formulario de alta después de agregar.
static void limpiar_formulario()
{
    ui->txtNombreProd->clear();
    ui->txtPrecioProd->clear();
    ui->txtStockProd->clear();
    ui->txtDescripcionProd->clear();
    ui->comboCategoria->setCurrentIndex(0);
}

// ===== BLOQUE 3: FUNCIONES DE LOS BOTONES =====

// Sirve para dar de alta un producto nuevo con los datos del formulario.
static void agregar_producto()
{
    QString nombre = ui->txtNombreProd->text().trimmed();
    QString precio = ui->txtPrecioProd->text();
    QString stock = ui->txtStockProd->text();
    QString descripcion = ui->txtDescripcionProd->text().trimmed();

    if (nombre.isEmpty() || precio.isEmpty() || stock.isEmpty()) {
        QMessageBox::warning(ventana, "Campos Incompletos", "Completá nombre, precio y stock.");
        return;
    }
    if (ui->comboCategoria->currentIndex() == 0) {
        QMessageBox::warning(ventana, "Error", "Debe seleccionar una categoría.");
        return;
    }

    // La posición del combo es el ID_Categoria
    MYSQL_RES *result = alta_producto(conexion, usuario_id, ui->comboCategoria->currentIndex(),
                                      nombre, descripcion, stock.toInt(), precio.toInt());
    if (numero_del_sp(result) == 0) {
        QMessageBox::warning(ventana, "Error", "No se pudo dar de alta el producto en la base de datos.");
        return;
    }
    mostrar_inventario();
    mostrar_logs();
    limpiar_formulario();
    ui->lblEstadoAdmin->setText("Se agregó " + nombre + ".");
}

// Sirve para guardar el precio y/o el stock editados en la tabla.
static void guardar_cambios()
{
    int fila = ui->tableProductosAdmin->currentRow();
    if (fila == -1) {
        QMessageBox::warning(ventana, "Error", "No seleccionó ninguna fila para modificar.");
        return;
    }

    int id = ui->tableProductosAdmin->item(fila, 0)->text().toInt();
    QString nombre = ui->tableProductosAdmin->item(fila, 1)->text();

    bool precioValido = false;
    int precio = ui->tableProductosAdmin->item(fila, 3)->text().toInt(&precioValido);
    if (!precioValido || precio <= 0) {
        QMessageBox::warning(ventana, "Precio Inválido", "El precio tiene que ser un número entero mayor a 0.");
        return;
    }
    bool stockValido = false;
    int stock = ui->tableProductosAdmin->item(fila, 4)->text().toInt(&stockValido);
    if (!stockValido || stock < 0) {
        QMessageBox::warning(ventana, "Stock Inválido", "El stock tiene que ser un número entero mayor o igual a 0.");
        return;
    }

    bool cambioStock = cambio(fila, 4);
    bool cambioPrecio = cambio(fila, 3);
    if (!cambioStock && !cambioPrecio) {
        QMessageBox::information(ventana, "Sin Cambios", "Editá el precio o el stock en la tabla antes de guardar.");
        return;
    }
    bool correcto = true;
    if (cambioStock && numero_del_sp(modificar_stock(conexion, usuario_id, id, stock)) == 0) {
        correcto = false;
    }
    if (cambioPrecio && numero_del_sp(modificar_precio(conexion, usuario_id, id, precio)) == 0) {
        correcto = false;
    }
    if (!correcto) {
        QMessageBox::warning(ventana, "Error", "No se pudieron guardar los cambios en la base de datos.");
    }
    mostrar_inventario();
    mostrar_logs();
    ui->lblEstadoAdmin->setText("Se guardaron los cambios de " + nombre + ".");
}

// Sirve para dar de baja (lógica) el producto elegido: queda inactivo, no se borra.
static void dar_de_baja()
{
    int fila = ui->tableProductosAdmin->currentRow();
    if (fila == -1) {
        QMessageBox::warning(ventana, "Error", "No seleccionó ninguna fila para dar de baja.");
        return;
    }
    int id = ui->tableProductosAdmin->item(fila, 0)->text().toInt();
    QString nombre = ui->tableProductosAdmin->item(fila, 1)->text();

    if (numero_del_sp(baja_producto(conexion, usuario_id, id)) == 0) {
        QMessageBox::warning(ventana, "Error", nombre + " ya estaba inactivo o no se pudo dar de baja.");
        return;
    }
    mostrar_inventario();
    mostrar_logs();
    ui->lblEstadoAdmin->setText(nombre + " quedó inactivo.");
}

// Sirve para el botón SALIR: cierra la sesión y vuelve al login.
static void cerrar_sesion_admin()
{
    salir_de_la_cuenta();
    abrir_login();
    ventana->close();
}

// ===== BLOQUE 4: APERTURA DE LA PANTALLA =====

// Sirve para abrir la administración.
void abrir_admin()
{
    if (ventana != NULL) {
        ventana->raise();
        return;
    }
    ventana = new QWidget();
    ventana->setAttribute(Qt::WA_DeleteOnClose);
    ui = new Ui::AdminWindow();
    ui->setupUi(ventana);
    ventana->setWindowTitle("Administración - El Buffet de Alejandro");
    QObject::connect(ventana, &QObject::destroyed, liberar);

    ui->txtPrecioProd->setValidator(new QIntValidator(1, 999999, ventana));
    ui->txtStockProd->setValidator(new QIntValidator(0, 9999, ventana));
    ui->txtNombreProd->setMaxLength(100); // Nombre_Producto VARCHAR(100)

    ui->tableProductosAdmin->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableProductosAdmin->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableProductosAdmin->setColumnCount(6);
    ui->tableProductosAdmin->setHorizontalHeaderLabels({"ID", "Producto", "Categoría", "Precio", "Stock", "Estado"});
    ui->tableProductosAdmin->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->tableLogsAdmin->setColumnCount(3);
    ui->tableLogsAdmin->setHorizontalHeaderLabels({"Usuario", "Evento", "Detalle"});
    ui->tableLogsAdmin->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);

    QObject::connect(ui->btnAgregarProd, &QPushButton::clicked, agregar_producto);
    QObject::connect(ui->btnGuardarEdit, &QPushButton::clicked, guardar_cambios);
    QObject::connect(ui->btnEliminarProd, &QPushButton::clicked, dar_de_baja);
    QObject::connect(ui->btnLogout, &QPushButton::clicked, cerrar_sesion_admin);

    mostrar_inventario();
    mostrar_logs();
    ventana->show();
}
