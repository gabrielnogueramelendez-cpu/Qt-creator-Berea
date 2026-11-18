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

    cargarInventario();
    prepararTablaLogs();
    registrarLog("Inicio de Sesión Exitoso", "Ingreso al panel de administración");
}

AdminWindow::~AdminWindow()
{
    delete ui;
}

// =====================================================================
// cargarInventario: llena la tabla de productos
// Columnas: ID, Producto, Categoría, Precio, Stock, Estado
// =====================================================================
void AdminWindow::cargarInventario()
{
    ui->tableProductosAdmin->setRowCount(0);
    ui->tableProductosAdmin->setColumnCount(6);
    QStringList titulos;
    titulos << "ID" << "Producto" << "Categoría" << "Precio" << "Stock" << "Estado";
    ui->tableProductosAdmin->setHorizontalHeaderLabels(titulos);
    ui->tableProductosAdmin->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);

    if (SistemaBuffet::conexion != NULL) {
        // ----- MODO CON BDD: Producto + Categoria -----
        MYSQL_RES *resultado = obtener_productos(SistemaBuffet::conexion);
        if (resultado == NULL) {
            return;
        }

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
    } else {
        // ----- MODO SIN BDD: la lista de productos (la posición 0 es el menú, se saltea) -----
        for (int id = 1; id < SistemaBuffet::productos.size(); id++) {
            Producto producto = SistemaBuffet::productos[id];
            int numeroFila = id - 1;

            ui->tableProductosAdmin->insertRow(numeroFila);
            ui->tableProductosAdmin->setItem(numeroFila, 0, new QTableWidgetItem(QString::number(producto.id)));
            ui->tableProductosAdmin->setItem(numeroFila, 1, new QTableWidgetItem(producto.nombre));
            ui->tableProductosAdmin->setItem(numeroFila, 2, new QTableWidgetItem(producto.categoria));
            ui->tableProductosAdmin->setItem(numeroFila, 3, new QTableWidgetItem(QString::number(producto.precio, 'f', 0)));
            ui->tableProductosAdmin->setItem(numeroFila, 4, new QTableWidgetItem(QString::number(producto.stock)));
            ui->tableProductosAdmin->setItem(numeroFila, 5, new QTableWidgetItem("Activo"));
        }
    }
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
// cargarLogsDesdeBDD (MODO CON BDD)
// Los movimientos los escriben los SP; acá solo se muestran los últimos
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
// registrarLog: agrega un movimiento a la tabla de logs
// Con BDD el SP ya lo guardó: solo se recarga la tabla
// =====================================================================
void AdminWindow::registrarLog(QString evento, QString detalle)
{
    if (SistemaBuffet::conexion != NULL) {
        cargarLogsDesdeBDD();
        return;
    }

    // Sin BDD: se agrega una fila (los nombres de evento son los de la tabla Tipo_Evento)
    int numeroFila = ui->tableLogsAdmin->rowCount();
    ui->tableLogsAdmin->insertRow(numeroFila);
    ui->tableLogsAdmin->setItem(numeroFila, 0, new QTableWidgetItem("alejandro.funes")); // el encargado (UserID 27)
    ui->tableLogsAdmin->setItem(numeroFila, 1, new QTableWidgetItem(evento));
    ui->tableLogsAdmin->setItem(numeroFila, 2, new QTableWidgetItem(detalle));
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
// Botón AGREGAR PRODUCTO
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

    if (SistemaBuffet::conexion != NULL) {
        // ----- MODO CON BDD: el ProductoID lo da AUTO_INCREMENT -----
        QStringList respuesta = leer_primera_fila(alta_producto(SistemaBuffet::conexion, SistemaBuffet::usuario.id,
                                                                categoriaID, nombre, descripcion, stock.toInt(), precio.toInt()));
        if (!respuestaCorrecta(respuesta)) {
            QMessageBox::warning(this, "Error", "No se pudo dar de alta el producto en la base de datos.");
            return;
        }
        cargarInventario();
        cargarLogsDesdeBDD();
    } else {
        // ----- MODO SIN BDD: se agrega una fila a la tabla -----
        int numeroFila = ui->tableProductosAdmin->rowCount();
        ui->tableProductosAdmin->insertRow(numeroFila);
        ui->tableProductosAdmin->setItem(numeroFila, 0, new QTableWidgetItem(QString::number(numeroFila + 1)));
        ui->tableProductosAdmin->setItem(numeroFila, 1, new QTableWidgetItem(nombre));
        ui->tableProductosAdmin->setItem(numeroFila, 2, new QTableWidgetItem(ui->comboCategoria->currentText()));
        ui->tableProductosAdmin->setItem(numeroFila, 3, new QTableWidgetItem(precio));
        ui->tableProductosAdmin->setItem(numeroFila, 4, new QTableWidgetItem(stock));
        ui->tableProductosAdmin->setItem(numeroFila, 5, new QTableWidgetItem("Activo"));
        registrarLog("Alta de Producto", "Alta de producto: " + nombre);
    }

    ui->lblEstadoAdmin->setText("Se agregó " + nombre + ".");
    limpiarFormulario();
}

// =====================================================================
// Botón GUARDAR CAMBIOS: guarda el precio y el stock que se editaron
// directamente en la tabla
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

    if (SistemaBuffet::conexion != NULL) {
        // ----- MODO CON BDD: un SP para el stock y otro para el precio -----
        bool stockGuardado = respuestaCorrecta(leer_primera_fila(modificar_stock(SistemaBuffet::conexion, SistemaBuffet::usuario.id, productoID, stock)));
        bool precioGuardado = respuestaCorrecta(leer_primera_fila(modificar_precio(SistemaBuffet::conexion, SistemaBuffet::usuario.id, productoID, precio)));
        if (!stockGuardado || !precioGuardado) {
            QMessageBox::warning(this, "Error", "No se pudieron guardar los cambios en la base de datos.");
        }
        cargarInventario();
        cargarLogsDesdeBDD();
    } else {
        // ----- MODO SIN BDD: el catálogo usa este mismo stock -----
        if (productoID < SistemaBuffet::productos.size()) {
            SistemaBuffet::productos[productoID].stock = stock;
        }
        registrarLog("Modificación de Stock de Producto", "Producto " + QString::number(productoID) + ": stock " + QString::number(stock));
        registrarLog("Modificación de Precio de Producto", "Producto " + QString::number(productoID) + ": precio " + QString::number(precio));
    }

    ui->lblEstadoAdmin->setText("Se guardaron los cambios de " + nombre + ".");
}

// =====================================================================
// Botón DAR DE BAJA: el producto queda inactivo (no se borra)
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

    if (SistemaBuffet::conexion != NULL) {
        // ----- MODO CON BDD -----
        QStringList respuesta = leer_primera_fila(baja_producto(SistemaBuffet::conexion, SistemaBuffet::usuario.id, productoID.toInt()));
        if (!respuestaCorrecta(respuesta)) {
            QMessageBox::warning(this, "Error", nombre + " ya estaba inactivo o no se pudo dar de baja.");
            return;
        }
        cargarInventario();
        cargarLogsDesdeBDD();
    } else {
        // ----- MODO SIN BDD: baja lógica, como sp_baja_producto (Activo = FALSE) -----
        ui->tableProductosAdmin->item(filaElegida, 5)->setText("Inactivo");
        registrarLog("Baja de Producto", "Producto " + productoID + ": " + nombre);
    }

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
