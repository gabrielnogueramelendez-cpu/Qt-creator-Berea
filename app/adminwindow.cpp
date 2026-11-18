// =====================================================================
// ARCHIVO: adminwindow.cpp (ADMINISTRACIÓN)
// QUÉ HACE: inventario (alta, cambio de precio y stock, baja) y la tabla
//           de movimientos de Log_Auditoria.
// SE LLEGA: login de "Administración".
// BDD:      obtener_productos, sp_alta_producto, sp_modificar_stock,
//           sp_modificar_precio, sp_baja_producto y obtener_logs.
//           Los SP escriben el log; la app solo lo muestra.
// =====================================================================

#include "adminwindow.h"
#include "ui_adminwindow.h"
#include "loginwindow.h"
#include "sistemabuffet.h"
#include "basedatos.h"
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QIntValidator>

// =====================================================================
// Constructor: prepara el panel de administración
// =====================================================================
AdminWindow::AdminWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AdminWindow)
{
    ui->setupUi(this);
    setWindowTitle("Administración - El Buffet de Alejandro");

    // Solo números en precio y stock (guía Gestor de Inventario)
    ui->txtPrecioProd->setValidator(new QIntValidator(1, 999999, this));
    ui->txtStockProd->setValidator(new QIntValidator(0, 9999, this));
    ui->txtNombreProd->setMaxLength(100); // Nombre_Producto VARCHAR(100)

    // En la tabla se elige una fila entera por vez
    ui->tableProductosAdmin->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableProductosAdmin->setSelectionMode(QAbstractItemView::SingleSelection);

    // Las dos tablas de la pantalla se llenan con datos de la BDD
    cargarInventario();
    prepararTablaLogs();
    cargarLogsDesdeBDD();
}

AdminWindow::~AdminWindow()
{
    delete ui;
}

// =====================================================================
// cargarInventario: llena la tabla de productos con la tabla Producto
// (consulta obtener_productos en basedatos.cpp)
// Columnas de la pantalla: ID, Producto, Categoría, Precio, Stock, Estado
// =====================================================================
void AdminWindow::cargarInventario()
{
    ui->tableProductosAdmin->setRowCount(0);
    ui->tableProductosAdmin->setColumnCount(6);
    QStringList titulos;
    titulos << "ID" << "Producto" << "Categoría" << "Precio" << "Stock" << "Estado";
    ui->tableProductosAdmin->setHorizontalHeaderLabels(titulos);
    ui->tableProductosAdmin->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);

    MYSQL_RES *resultado = obtener_productos(SistemaBuffet::conexion);
    if (resultado == NULL) {
        return;
    }

    // Una fila de la pantalla por cada fila que devolvió MySQL
    int numeroFila = 0;
    MYSQL_ROW fila = mysql_fetch_row(resultado);
    while (fila != NULL) {
        ui->tableProductosAdmin->insertRow(numeroFila);
        for (int columna = 0; columna < 6; columna++) {
            QString texto = valor_fila(fila, columna);
            if (columna == 3) {
                texto = QString::number(texto.toDouble(), 'f', 0); // el precio sin los ,00
            }
            ui->tableProductosAdmin->setItem(numeroFila, columna, new QTableWidgetItem(texto));
        }
        numeroFila++;
        fila = mysql_fetch_row(resultado);
    }
    mysql_free_result(resultado);
}

// =====================================================================
// prepararTablaLogs: columnas de la tabla de movimientos (Log_Auditoria)
// La fecha y la hora las pone la BDD con CURDATE() y CURTIME()
// =====================================================================
void AdminWindow::prepararTablaLogs()
{
    ui->tableLogsAdmin->setRowCount(0);
    ui->tableLogsAdmin->setColumnCount(3);
    QStringList titulos;
    titulos << "Usuario" << "Evento" << "Detalle";
    ui->tableLogsAdmin->setHorizontalHeaderLabels(titulos);
    ui->tableLogsAdmin->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
}

// =====================================================================
// cargarLogsDesdeBDD: los últimos 50 movimientos de Log_Auditoria
// Los movimientos los escriben los SP (alta, stock, precio, baja...);
// la app nunca hace un INSERT en Log_Auditoria, solo los muestra
// =====================================================================
void AdminWindow::cargarLogsDesdeBDD()
{
    ui->tableLogsAdmin->setRowCount(0);

    MYSQL_RES *resultado = obtener_logs(SistemaBuffet::conexion);
    if (resultado == NULL) {
        return;
    }

    int numeroFila = 0;
    MYSQL_ROW fila = mysql_fetch_row(resultado);
    while (fila != NULL) {
        ui->tableLogsAdmin->insertRow(numeroFila);
        for (int columna = 0; columna < 3; columna++) {
            ui->tableLogsAdmin->setItem(numeroFila, columna, new QTableWidgetItem(valor_fila(fila, columna)));
        }
        numeroFila++;
        fila = mysql_fetch_row(resultado);
    }
    mysql_free_result(resultado);
}

// =====================================================================
// respuestaCorrecta: true si el SP no dio error y cambió una fila
// (Filas_Actualizadas distinto de 0)
// =====================================================================
bool AdminWindow::respuestaCorrecta(QStringList respuesta)
{
    if (respuesta.size() == 0) {
        return false;
    }
    if (respuesta[0] == "ERROR" || respuesta[0] == "0") {
        return false;
    }
    return true;
}

// =====================================================================
// limpiarFormulario: deja vacíos los campos de "nuevo producto"
// =====================================================================
void AdminWindow::limpiarFormulario()
{
    ui->txtNombreProd->clear();
    ui->txtPrecioProd->clear();
    ui->txtStockProd->clear();
    ui->txtDescripcionProd->clear();
    ui->comboCategoria->setCurrentIndex(0);
}

// =====================================================================
// Botón AGREGAR PRODUCTO: sp_alta_producto
// El ProductoID lo da AUTO_INCREMENT y el SP deja el alta en Log_Auditoria
// =====================================================================
void AdminWindow::on_btnAgregarProd_clicked()
{
    QString nombre = ui->txtNombreProd->text().trimmed();
    QString precio = ui->txtPrecioProd->text();
    QString stock = ui->txtStockProd->text();
    QString descripcion = ui->txtDescripcionProd->text().trimmed(); // puede quedar vacía
    int categoriaID = ui->comboCategoria->currentIndex();          // la posición en el combo es el ID_Categoria

    if (nombre.isEmpty() || precio.isEmpty() || stock.isEmpty()) {
        QMessageBox::warning(this, "Campos Incompletos", "Completá nombre, precio y stock.");
        return;
    }
    if (categoriaID == 0) {
        QMessageBox::warning(this, "Error", "Debe seleccionar una categoría.");
        return;
    }

    QStringList respuesta = leer_primera_fila(alta_producto(SistemaBuffet::conexion, SistemaBuffet::usuario.id,
                                                            categoriaID, nombre, descripcion, stock.toInt(), precio.toInt()));
    if (!respuestaCorrecta(respuesta)) {
        QMessageBox::warning(this, "Error", "No se pudo dar de alta el producto en la base de datos.");
        return;
    }

    // Se vuelven a leer las dos tablas para ver el producto nuevo y su log
    cargarInventario();
    cargarLogsDesdeBDD();

    ui->lblEstadoAdmin->setText("Se agregó " + nombre + ".");
    limpiarFormulario();
}

// =====================================================================
// Botón GUARDAR CAMBIOS: guarda el precio y el stock que se editaron
// directamente en la tabla (sp_modificar_stock y sp_modificar_precio)
// =====================================================================
void AdminWindow::on_btnGuardarEdit_clicked()
{
    int filaElegida = ui->tableProductosAdmin->currentRow();
    if (filaElegida == -1) {
        QMessageBox::warning(this, "Error", "No seleccionó ninguna fila para modificar.");
        return;
    }

    // Se leen las celdas de la fila elegida (item(fila, columna) es la celda)
    int productoID = ui->tableProductosAdmin->item(filaElegida, 0)->text().toInt();
    QString nombre = ui->tableProductosAdmin->item(filaElegida, 1)->text();
    QString textoPrecio = ui->tableProductosAdmin->item(filaElegida, 3)->text();
    QString textoStock = ui->tableProductosAdmin->item(filaElegida, 4)->text();

    // toInt(&esNumero) deja esNumero en false si el texto no es un número
    bool precioEsNumero = false;
    int precio = textoPrecio.toInt(&precioEsNumero);
    if (!precioEsNumero || precio <= 0) {
        QMessageBox::warning(this, "Precio Inválido", "El precio tiene que ser un número entero mayor a 0.");
        return;
    }

    bool stockEsNumero = false;
    int stock = textoStock.toInt(&stockEsNumero);
    if (!stockEsNumero || stock < 0) {
        QMessageBox::warning(this, "Stock Inválido", "El stock tiene que ser un número entero mayor o igual a 0.");
        return;
    }

    // Un SP para el stock y otro para el precio. Cada uno deja su log.
    // Ojo: si el precio no cambió, MySQL no cuenta la fila como modificada
    // y sp_modificar_precio devuelve 0, así que sale el aviso aunque el stock sí se guardó
    bool stockGuardado = respuestaCorrecta(leer_primera_fila(modificar_stock(SistemaBuffet::conexion, SistemaBuffet::usuario.id, productoID, stock)));
    bool precioGuardado = respuestaCorrecta(leer_primera_fila(modificar_precio(SistemaBuffet::conexion, SistemaBuffet::usuario.id, productoID, precio)));
    if (!stockGuardado || !precioGuardado) {
        QMessageBox::warning(this, "Error", "No se pudieron guardar los cambios en la base de datos.");
    }
    cargarInventario();
    cargarLogsDesdeBDD();

    ui->lblEstadoAdmin->setText("Se guardaron los cambios de " + nombre + ".");
}

// =====================================================================
// Botón DAR DE BAJA: sp_baja_producto. Es una baja lógica: el producto
// queda con Activo = FALSE (no se borra, porque hay ventas que lo usan)
// =====================================================================
void AdminWindow::on_btnEliminarProd_clicked()
{
    int filaElegida = ui->tableProductosAdmin->currentRow();
    if (filaElegida == -1) {
        QMessageBox::warning(this, "Error", "No seleccionó ninguna fila para dar de baja.");
        return;
    }

    QString productoID = ui->tableProductosAdmin->item(filaElegida, 0)->text();
    QString nombre = ui->tableProductosAdmin->item(filaElegida, 1)->text();

    QStringList respuesta = leer_primera_fila(baja_producto(SistemaBuffet::conexion, SistemaBuffet::usuario.id, productoID.toInt()));
    if (!respuestaCorrecta(respuesta)) {
        QMessageBox::warning(this, "Error", nombre + " ya estaba inactivo o no se pudo dar de baja.");
        return;
    }
    cargarInventario();
    cargarLogsDesdeBDD();

    ui->lblEstadoAdmin->setText(nombre + " quedó inactivo.");
}

// =====================================================================
// Botón SALIR: vuelve al login
// =====================================================================
void AdminWindow::on_btnLogout_clicked()
{
    SistemaBuffet::cerrarSesion();
    LoginWindow *ventanaLogin = new LoginWindow();
    ventanaLogin->show();
    this->close();
}
