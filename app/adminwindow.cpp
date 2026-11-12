#include "adminwindow.h"
#include "ui_adminwindow.h"
#include "loginwindow.h"
#include "sistemabuffet.h"
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QRegularExpression>
#include <QRegularExpressionValidator>

AdminWindow::AdminWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AdminWindow)
{
    ui->setupUi(this);
    setWindowTitle("Panel de Administración - Gestor del Buffet");

    // [ADM-02 y ADM-03] Validadores para precio y stock
    QRegularExpression regexPrecio("^[0-9]+(\\.[0-9]{1,2})?$");
    ui->txtPrecioProd->setValidator(new QRegularExpressionValidator(regexPrecio, this));

    QRegularExpression regexStock("^[0-9]{1,5}$");
    ui->txtStockProd->setValidator(new QRegularExpressionValidator(regexStock, this));

    cargarInventarioSimulado();
    cargarLogsSimulados();
}

AdminWindow::~AdminWindow()
{
    delete ui;
}

void AdminWindow::cargarInventarioSimulado()
{
    const auto prods = SistemaBuffet::instancia().getProductos();
    ui->tableProductosAdmin->setRowCount(0);
    ui->tableProductosAdmin->setColumnCount(5);
    QStringList headers;
    headers << "ID" << "Producto" << "Categoría" << "Precio" << "Stock";
    ui->tableProductosAdmin->setHorizontalHeaderLabels(headers);
    ui->tableProductosAdmin->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);

    for (int i = 0; i < prods.size(); ++i) {
        ui->tableProductosAdmin->insertRow(i);
        ui->tableProductosAdmin->setItem(i, 0, new QTableWidgetItem(QString::number(prods[i].id)));
        ui->tableProductosAdmin->setItem(i, 1, new QTableWidgetItem(prods[i].nombre));
        ui->tableProductosAdmin->setItem(i, 2, new QTableWidgetItem(prods[i].categoria));
        ui->tableProductosAdmin->setItem(i, 3, new QTableWidgetItem(QString::number(prods[i].precio, 'f', 0)));
        ui->tableProductosAdmin->setItem(i, 4, new QTableWidgetItem(QString::number(prods[i].stock)));
    }
}

void AdminWindow::cargarLogsSimulados()
{
    const auto logs = SistemaBuffet::instancia().getLogs();
    ui->tableLogsAdmin->setRowCount(0);
    ui->tableLogsAdmin->setColumnCount(4);
    QStringList headers;
    headers << "Fecha / Hora" << "Usuario / Rol" << "Acción" << "Detalle Evento";
    ui->tableLogsAdmin->setHorizontalHeaderLabels(headers);
    ui->tableLogsAdmin->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);

    for (int i = 0; i < logs.size(); ++i) {
        ui->tableLogsAdmin->insertRow(i);
        ui->tableLogsAdmin->setItem(i, 0, new QTableWidgetItem(logs[i].fechaHora.toString("yyyy-MM-dd HH:mm:ss")));
        ui->tableLogsAdmin->setItem(i, 1, new QTableWidgetItem(logs[i].usuario + " (" + logs[i].rol + ")"));
        ui->tableLogsAdmin->setItem(i, 2, new QTableWidgetItem(logs[i].accion));
        ui->tableLogsAdmin->setItem(i, 3, new QTableWidgetItem(logs[i].detalle));
    }
}

void AdminWindow::on_btnAgregarProd_clicked()
{
    QString nombre = ui->txtNombreProd->text().trimmed();
    QString cat = ui->comboCategoria->currentText();
    QString precioStr = ui->txtPrecioProd->text().trimmed();
    QString stockStr = ui->txtStockProd->text().trimmed();

    // [ADM-01] Validar campos incompletos
    if (nombre.isEmpty() || precioStr.isEmpty() || stockStr.isEmpty()) {
        QMessageBox::warning(this, "Campos Incompletos", "Por favor completa todos los campos para agregar un nuevo producto.");
        return;
    }

    // [ADM-01] Validar longitud mínima de nombre
    if (nombre.length() < 3) {
        QMessageBox::warning(this, "Nombre Demasiado Corto", "El nombre del producto debe contener al menos 3 caracteres.");
        return;
    }

    // [ADM-02] Validar precio numérico estrictamente positivo
    bool okPrecio = false;
    double precio = precioStr.toDouble(&okPrecio);
    if (!okPrecio || precio <= 0.0) {
        QMessageBox::warning(this, "Precio Inválido", "El precio debe ser un valor numérico estrictamente mayor a $0.");
        return;
    }

    // [ADM-03] Validar stock entero no negativo
    bool okStock = false;
    int stock = stockStr.toInt(&okStock);
    if (!okStock || stock < 0) {
        QMessageBox::warning(this, "Stock Inválido", "El stock debe ser un número entero mayor o igual a 0.");
        return;
    }

    QString errorMsg;
    bool agregado = SistemaBuffet::instancia().agregarProducto(nombre, "Producto agregado por administración escolar", cat, precio, stock, errorMsg);

    if (!agregado) {
        QMessageBox::warning(this, "No se pudo agregar", errorMsg);
        return;
    }

    QMessageBox::information(this, "Producto Agregado",
        QString("El producto '%1' fue registrado exitosamente con precio $%2 y stock %3.")
            .arg(nombre).arg(QString::number(precio, 'f', 0)).arg(stock));

    ui->txtNombreProd->clear();
    ui->txtPrecioProd->clear();
    ui->txtStockProd->clear();

    cargarInventarioSimulado();
    cargarLogsSimulados();
}

void AdminWindow::on_btnGuardarEdit_clicked()
{
    int fila = ui->tableProductosAdmin->currentRow();
    if (fila < 0) {
        QMessageBox::warning(this, "Selección Requerida", "Selecciona un producto de la tabla para guardar sus modificaciones de precio o stock.");
        return;
    }

    int id = ui->tableProductosAdmin->item(fila, 0)->text().toInt();
    QString precioStr = ui->tableProductosAdmin->item(fila, 3)->text().replace("$", "").trimmed();
    QString stockStr = ui->tableProductosAdmin->item(fila, 4)->text().trimmed();

    bool okP = false;
    double precio = precioStr.toDouble(&okP);
    if (!okP || precio <= 0) {
        QMessageBox::warning(this, "Precio Inválido", "El precio en la celda debe ser un número mayor a 0.");
        return;
    }

    bool okS = false;
    int stock = stockStr.toInt(&okS);
    if (!okS || stock < 0) {
        QMessageBox::warning(this, "Stock Inválido", "El stock en la celda debe ser un entero mayor o igual a 0.");
        return;
    }

    QString errorMsg;
    if (!SistemaBuffet::instancia().actualizarProducto(id, precio, stock, errorMsg)) {
        QMessageBox::critical(this, "Error al Actualizar", errorMsg);
        return;
    }

    QMessageBox::information(this, "Cambios Guardados",
        QString("El producto ID %1 se actualizó con éxito (Precio: $%2, Stock: %3).")
            .arg(id).arg(QString::number(precio, 'f', 0)).arg(stock));

    cargarInventarioSimulado();
    cargarLogsSimulados();
}

void AdminWindow::on_btnEliminarProd_clicked()
{
    int fila = ui->tableProductosAdmin->currentRow();
    if (fila < 0) {
        QMessageBox::warning(this, "Selección Requerida", "Selecciona un producto de la tabla para eliminarlo del inventario.");
        return;
    }

    int id = ui->tableProductosAdmin->item(fila, 0)->text().toInt();
    QString nombre = ui->tableProductosAdmin->item(fila, 1)->text();

    QMessageBox::StandardButton resp = QMessageBox::question(this, "Confirmar Baja",
        QString("¿Estás seguro de que deseas eliminar '%1' del catálogo del buffet escolar?").arg(nombre),
        QMessageBox::Yes | QMessageBox::No);

    if (resp == QMessageBox::Yes) {
        QString errorMsg;
        if (!SistemaBuffet::instancia().eliminarProducto(id, errorMsg)) {
            QMessageBox::critical(this, "Error", errorMsg);
            return;
        }
        cargarInventarioSimulado();
        cargarLogsSimulados();
        QMessageBox::information(this, "Producto Eliminado", "El producto fue removido correctamente.");
    }
}

void AdminWindow::on_btnLogout_clicked()
{
    SistemaBuffet::instancia().cerrarSesion();
    LoginWindow *loginWin = new LoginWindow();
    loginWin->show();
    this->close();
}