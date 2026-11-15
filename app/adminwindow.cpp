#include "adminwindow.h"
#include "ui_adminwindow.h"
#include "loginwindow.h"
#include "sistemabuffet.h"
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QIntValidator>

AdminWindow::AdminWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AdminWindow)
{
    ui->setupUi(this);
    setWindowTitle("Administración - El Buffet de Alejandro");

    // Solo números (guía Gestor de Inventario)
    ui->txtPrecioProd->setValidator(new QIntValidator(1, 999999, this));
    ui->txtStockProd->setValidator(new QIntValidator(0, 9999, this));
    ui->txtNombreProd->setMaxLength(100); // Nombre_Producto VARCHAR(100)

    ui->tableProductosAdmin->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableProductosAdmin->setSelectionMode(QAbstractItemView::SingleSelection);

    cargarInventario();
    cargarLogs();
}

AdminWindow::~AdminWindow()
{
    delete ui;
}

void AdminWindow::cargarInventario()
{
    // Con BDD: SELECT de Producto + Categoria
    ui->tableProductosAdmin->setRowCount(0);
    ui->tableProductosAdmin->setColumnCount(6);
    QStringList headers;
    headers << "ID" << "Producto" << "Categoría" << "Precio" << "Stock" << "Estado";
    ui->tableProductosAdmin->setHorizontalHeaderLabels(headers);
    ui->tableProductosAdmin->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);

    // Mismos datos que la tabla Producto de la BDD (04_DatosSQL)
    QStringList nombres = {"Sándwich de Milanesa", "Empanada de Carne", "Porción de Pizza Muzzarella", "Pebete de Queso",
                           "Medialuna de Manteca", "Ensalada Mixta", "Manzana", "Chocolatada 250 ml", "Yogur Bebible de Frutilla",
                           "Agua Mineral 500 ml", "Jugo en Caja 200 ml", "Gaseosa 500ml", "Café con Leche", "Barra de Cereal",
                           "Galletitas de Agua", "Alfajor de Maicena", "Turrón de Maní", "Mix de Frutos Secos",
                           "Milanesa de Pollo con Puré", "Combo Sándwich + Gaseosa"};
    QStringList precios = {"2800", "1100", "1800", "1600", "800", "2200", "700", "1100", "1200", "1000",
                           "900", "1300", "1400", "900", "700", "800", "600", "1500", "4200", "4100"};
    // ID_Categoria de cada producto (columna de Producto); es la posición en comboCategoria
    QList<int> categorias = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};

    for (int i = 0; i < nombres.size(); ++i) {
        ui->tableProductosAdmin->insertRow(i);
        ui->tableProductosAdmin->setItem(i, 0, new QTableWidgetItem(QString::number(i + 1)));
        ui->tableProductosAdmin->setItem(i, 1, new QTableWidgetItem(nombres[i]));
        // El item 0 del combo es "Seleccioná...", así que el ID_Categoria coincide con la posición
        ui->tableProductosAdmin->setItem(i, 2, new QTableWidgetItem(ui->comboCategoria->itemText(categorias[i])));
        ui->tableProductosAdmin->setItem(i, 3, new QTableWidgetItem(precios[i]));
        ui->tableProductosAdmin->setItem(i, 4, new QTableWidgetItem(QString::number(SistemaBuffet::productosStock[i + 1])));
        ui->tableProductosAdmin->setItem(i, 5, new QTableWidgetItem("Activo"));
    }
}

void AdminWindow::cargarLogs()
{
    // Columnas de Log_Auditoria (la Fecha y la Hora las pone la BDD con CURDATE() y CURTIME())
    ui->tableLogsAdmin->setRowCount(0);
    ui->tableLogsAdmin->setColumnCount(3);
    QStringList headers;
    headers << "Usuario" << "Evento" << "Detalle";
    ui->tableLogsAdmin->setHorizontalHeaderLabels(headers);
    ui->tableLogsAdmin->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);

    registrarLog("Inicio de Sesión Exitoso", "Ingreso al panel de administración");
}

void AdminWindow::registrarLog(QString evento, QString detalle)
{
    // Los nombres de evento son los de la tabla Tipo_Evento
    int fila = ui->tableLogsAdmin->rowCount();
    ui->tableLogsAdmin->insertRow(fila);
    ui->tableLogsAdmin->setItem(fila, 0, new QTableWidgetItem("alejandro.funes")); // el encargado (UserID 27)
    ui->tableLogsAdmin->setItem(fila, 1, new QTableWidgetItem(evento));
    ui->tableLogsAdmin->setItem(fila, 2, new QTableWidgetItem(detalle));
}

void AdminWindow::on_btnAgregarProd_clicked()
{
    QString nombre = ui->txtNombreProd->text().trimmed();
    QString precio = ui->txtPrecioProd->text();
    QString stock = ui->txtStockProd->text();
    QString descripcion = ui->txtDescripcionProd->text().trimmed(); // Descripcion_Producto puede quedar vacía

    if (nombre.isEmpty() || precio.isEmpty() || stock.isEmpty()) {
        QMessageBox::warning(this, "Campos Incompletos", "Completá nombre, precio y stock.");
        return;
    }
    if (ui->comboCategoria->currentIndex() == 0) {
        QMessageBox::warning(this, "Error", "Debe seleccionar una categoría.");
        return;
    }

    // Con BDD: CALL sp_alta_producto(UserID, categoría, nombre, descripcion, stock, precio); el ID lo da AUTO_INCREMENT
    int fila = ui->tableProductosAdmin->rowCount();
    ui->tableProductosAdmin->insertRow(fila);
    ui->tableProductosAdmin->setItem(fila, 0, new QTableWidgetItem(QString::number(fila + 1)));
    ui->tableProductosAdmin->setItem(fila, 1, new QTableWidgetItem(nombre));
    ui->tableProductosAdmin->setItem(fila, 2, new QTableWidgetItem(ui->comboCategoria->currentText()));
    ui->tableProductosAdmin->setItem(fila, 3, new QTableWidgetItem(precio));
    ui->tableProductosAdmin->setItem(fila, 4, new QTableWidgetItem(stock));
    ui->tableProductosAdmin->setItem(fila, 5, new QTableWidgetItem("Activo"));

    registrarLog("Alta de Producto", "Alta de producto: " + nombre);
    ui->lblEstadoAdmin->setText("Se agregó " + nombre + ".");

    ui->txtNombreProd->clear();
    ui->txtPrecioProd->clear();
    ui->txtStockProd->clear();
    ui->txtDescripcionProd->clear();
    ui->comboCategoria->setCurrentIndex(0);
}

void AdminWindow::on_btnGuardarEdit_clicked()
{
    int fila = ui->tableProductosAdmin->currentRow();
    if (fila == -1) {
        QMessageBox::warning(this, "Error", "No seleccionó ninguna fila para modificar.");
        return;
    }

    int id = ui->tableProductosAdmin->item(fila, 0)->text().toInt();
    QString nombre = ui->tableProductosAdmin->item(fila, 1)->text();

    bool okPrecio = false;
    int precio = ui->tableProductosAdmin->item(fila, 3)->text().toInt(&okPrecio);
    if (!okPrecio || precio <= 0) {
        QMessageBox::warning(this, "Precio Inválido", "El precio tiene que ser un número entero mayor a 0.");
        return;
    }

    bool okStock = false;
    int stock = ui->tableProductosAdmin->item(fila, 4)->text().toInt(&okStock);
    if (!okStock || stock < 0) {
        QMessageBox::warning(this, "Stock Inválido", "El stock tiene que ser un número entero mayor o igual a 0.");
        return;
    }

    // Con BDD: CALL sp_modificar_stock(UserID, ProductoID, stock)
    if (id < SistemaBuffet::productosStock.size()) {
        SistemaBuffet::productosStock[id] = stock; // el catálogo usa este mismo stock
    }

    registrarLog("Modificación de Stock de Producto", QString("Producto %1: stock %2").arg(id).arg(stock));

    // Con BDD: CALL sp_modificar_precio(UserID, ProductoID, precio)
    registrarLog("Modificación de Precio de Producto", QString("Producto %1: precio %2").arg(id).arg(precio));
    ui->lblEstadoAdmin->setText("Se guardaron los cambios de " + nombre + ".");
}

void AdminWindow::on_btnEliminarProd_clicked()
{
    int fila = ui->tableProductosAdmin->currentRow();
    if (fila == -1) {
        QMessageBox::warning(this, "Error", "No seleccionó ninguna fila para dar de baja.");
        return;
    }

    QString id = ui->tableProductosAdmin->item(fila, 0)->text();
    QString nombre = ui->tableProductosAdmin->item(fila, 1)->text();

    // Baja lógica como sp_baja_producto (Activo = FALSE): el producto queda en la tabla
    ui->tableProductosAdmin->item(fila, 5)->setText("Inactivo");

    registrarLog("Baja de Producto", "Producto " + id + ": " + nombre);
    ui->lblEstadoAdmin->setText(nombre + " quedó inactivo.");
}

void AdminWindow::on_btnLogout_clicked()
{
    LoginWindow *loginWin = new LoginWindow();
    loginWin->show();
    this->close();
}
