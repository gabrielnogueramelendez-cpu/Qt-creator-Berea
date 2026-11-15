#include "cajerowindow.h"
#include "ui_cajerowindow.h"
#include "loginwindow.h"
#include "sistemabuffet.h"
#include "basedatos.h"
#include <QMessageBox>
#include <QTableWidgetItem>

CajeroWindow::CajeroWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CajeroWindow)
{
    ui->setupUi(this);
    setWindowTitle("Mostrador - El Buffet de Alejandro");

    ui->btnEntregarPedido->setEnabled(false);
    ui->btnCancelarPedido->setEnabled(false);

    // Codigo_Retiro es CHAR(5) en la BDD
    ui->txtCodigoRetiro->setMaxLength(5);

    ui->tableColasPedidos->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableColasPedidos->setSelectionMode(QAbstractItemView::SingleSelection);

    cargarPedidosDelSistema();
}

CajeroWindow::~CajeroWindow()
{
    delete ui;
}

void CajeroWindow::cargarPedidosDelSistema()
{
    // Con BDD: los pedidos salen de Retiro + Venta + Carrito (ver obtener_pedidos)
    SistemaBuffet::cargarDesdeBDD();
    ui->tableColasPedidos->setRowCount(0);
    ui->tableColasPedidos->setColumnCount(5);
    QStringList headers;
    headers << "Código" << "Cliente" << "Retiro" << "Total" << "Estado";
    ui->tableColasPedidos->setHorizontalHeaderLabels(headers);
    ui->tableColasPedidos->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);

    for (int i = 0; i < SistemaBuffet::pedidosCodigos.size(); ++i) {
        ui->tableColasPedidos->insertRow(i);
        ui->tableColasPedidos->setItem(i, 0, new QTableWidgetItem(SistemaBuffet::pedidosCodigos[i]));
        ui->tableColasPedidos->setItem(i, 1, new QTableWidgetItem(SistemaBuffet::pedidosAlumnos[i] + " (" + SistemaBuffet::pedidosCursos[i] + ")"));
        ui->tableColasPedidos->setItem(i, 2, new QTableWidgetItem(SistemaBuffet::pedidosRecreos[i]));
        ui->tableColasPedidos->setItem(i, 3, new QTableWidgetItem("$" + QString::number(SistemaBuffet::pedidosMontos[i], 'f', 0)));
        ui->tableColasPedidos->setItem(i, 4, new QTableWidgetItem(SistemaBuffet::pedidosEstados[i]));
    }
}

void CajeroWindow::on_tableColasPedidos_cellClicked(int row, int)
{
    ui->txtCodigoRetiro->setText(ui->tableColasPedidos->item(row, 0)->text());
    on_btnBuscarCodigo_clicked();
}

void CajeroWindow::on_btnBuscarCodigo_clicked()
{
    QString codigo = ui->txtCodigoRetiro->text().trimmed().toUpper();

    if (codigo.isEmpty()) {
        QMessageBox::warning(this, "Código Requerido", "Ingresá el código de retiro.");
        return;
    }

    int idx = SistemaBuffet::pedidosCodigos.indexOf(codigo);
    if (idx == -1) {
        m_codigoActual = "";
        ui->lblStatusPedido->setText("No hay ningún pedido con el código " + codigo + ".");
        ui->lblDetalleAlumno->setText("");
        ui->lblDetalleRecreo->setText("");
        ui->listDetalleItems->clear();
        ui->lblTotalMonto->setText("");
        ui->btnEntregarPedido->setEnabled(false);
        ui->btnCancelarPedido->setEnabled(false);
        return;
    }

    m_codigoActual = codigo;
    QString estado = SistemaBuffet::pedidosEstados[idx];

    // Solo se puede cambiar un pedido PENDIENTE (igual que sp_actualizar_estado_retiro)
    bool pendiente = (estado == "PENDIENTE");
    ui->btnEntregarPedido->setEnabled(pendiente);
    ui->btnCancelarPedido->setEnabled(pendiente);
    if (pendiente) {
        ui->lblStatusPedido->setText("PEDIDO " + codigo + " · PENDIENTE DE ENTREGA");
    } else {
        ui->lblStatusPedido->setText("PEDIDO " + codigo + " · " + estado + " · ya no se puede modificar");
    }

    ui->lblDetalleAlumno->setText(QString("Cliente: %1\nDNI %2 · %3")
        .arg(SistemaBuffet::pedidosAlumnos[idx])
        .arg(SistemaBuffet::pedidosDnis[idx])
        .arg(SistemaBuffet::pedidosCursos[idx]));
    ui->lblDetalleRecreo->setText(QString("Retiro: %1\nPago: %2")
        .arg(SistemaBuffet::pedidosRecreos[idx])
        .arg(SistemaBuffet::pedidosMetodos[idx]));

    // Un renglón por producto (en la BDD: tabla Carrito de esa venta)
    ui->listDetalleItems->clear();
    QStringList lineas = SistemaBuffet::pedidosDetalles[idx].split('\n', Qt::SkipEmptyParts);
    for (int i = 0; i < lineas.size(); ++i) {
        ui->listDetalleItems->addItem(lineas[i].trimmed());
    }

    QString textoTotal = "Total: $" + QString::number(SistemaBuffet::pedidosMontos[idx], 'f', 0);
    if (SistemaBuffet::pedidosMetodos[idx].contains("Efectivo") && SistemaBuffet::pedidosVueltos[idx] > 0) {
        textoTotal += QString("\nPaga con $%1 · Vuelto $%2")
            .arg(QString::number(SistemaBuffet::pedidosAbonados[idx], 'f', 0))
            .arg(QString::number(SistemaBuffet::pedidosVueltos[idx], 'f', 0));
    }
    ui->lblTotalMonto->setText(textoTotal);
}

void CajeroWindow::on_btnEntregarPedido_clicked()
{
    int idx = SistemaBuffet::pedidosCodigos.indexOf(m_codigoActual);
    if (idx == -1) return;

    if (SistemaBuffet::conn != NULL) {
        cambiarEstadoEnBDD(idx, 2); // 2 = ENTREGADO
    } else {
        SistemaBuffet::pedidosEstados[idx] = "ENTREGADO";
    }
    cargarPedidosDelSistema();
    on_btnBuscarCodigo_clicked();
}

void CajeroWindow::on_btnCancelarPedido_clicked()
{
    int idx = SistemaBuffet::pedidosCodigos.indexOf(m_codigoActual);
    if (idx == -1) return;

    if (SistemaBuffet::conn != NULL) {
        cambiarEstadoEnBDD(idx, 3); // 3 = CANCELADO: el SP devuelve el stock y el saldo
        cargarPedidosDelSistema();
        on_btnBuscarCodigo_clicked();
        return;
    }
    SistemaBuffet::pedidosEstados[idx] = "CANCELADO";

    // shortcut: sin BDD no se devuelve el stock y el saldo solo existe para el estudiante logueado; con BDD lo hace el SP
    if (SistemaBuffet::pedidosMetodos[idx] == "Saldo de Cuenta Escolar" && SistemaBuffet::pedidosDnis[idx] == SistemaBuffet::usuarioDni) {
        SistemaBuffet::usuarioSaldo += SistemaBuffet::pedidosMontos[idx];
    }

    cargarPedidosDelSistema();
    on_btnBuscarCodigo_clicked();
}

void CajeroWindow::on_btnLogout_clicked()
{
    SistemaBuffet::cerrarSesion();
    LoginWindow *loginWin = new LoginWindow();
    loginWin->show();
    this->close();
}

void CajeroWindow::cambiarEstadoEnBDD(int idx, int estadoID)
{
    MYSQL_RES *result = actualizar_estado_retiro(SistemaBuffet::conn, SistemaBuffet::usuarioID, SistemaBuffet::pedidosRetiroIds[idx], estadoID);
    if (result == NULL || error_del_sp(result) != 0) {
        QMessageBox::warning(this, "Error", "No se pudo actualizar el pedido en la base de datos.");
    }
    if (result != NULL) {
        mysql_free_result(result);
    }
}
