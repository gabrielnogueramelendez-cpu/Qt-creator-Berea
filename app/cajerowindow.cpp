#include "cajerowindow.h"
#include "ui_cajerowindow.h"
#include "loginwindow.h"
#include "sistemabuffet.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QTableWidgetItem>
#include <QRegularExpression>
#include <QRegularExpressionValidator>

CajeroWindow::CajeroWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CajeroWindow)
{
    ui->setupUi(this);
    setWindowTitle("Panel del Personal de Buffet - Caja & Retiros");

    ui->btnEntregarPedido->setEnabled(false);
    ui->btnCancelarPedido->setEnabled(false);

    // [CAJ-01] Validador alfanumérico estricto de 5 caracteres
    QRegularExpression regexCodigo("^[a-zA-Z0-9]{5}$");
    ui->txtCodigoRetiro->setValidator(new QRegularExpressionValidator(regexCodigo, this));
    ui->txtCodigoRetiro->setMaxLength(5);

    cargarPedidosDelSistema();
}

CajeroWindow::~CajeroWindow()
{
    delete ui;
}

void CajeroWindow::cargarPedidosDelSistema()
{
    const auto pedidos = SistemaBuffet::instancia().getPedidos();
    ui->tableColasPedidos->setRowCount(0);
    ui->tableColasPedidos->setColumnCount(6);
    QStringList headers;
    headers << "Código" << "Alumno" << "Recreo" << "Método Pago" << "Monto Total" << "Estado";
    ui->tableColasPedidos->setHorizontalHeaderLabels(headers);
    ui->tableColasPedidos->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);

    for (int i = 0; i < pedidos.size(); ++i) {
        const auto &p = pedidos[i];
        ui->tableColasPedidos->insertRow(i);
        ui->tableColasPedidos->setItem(i, 0, new QTableWidgetItem(p.codigoRetiro));
        ui->tableColasPedidos->setItem(i, 1, new QTableWidgetItem(p.nombreAlumno + " (" + p.cursoAlumno + ")"));
        ui->tableColasPedidos->setItem(i, 2, new QTableWidgetItem(p.recreoRetiro));
        ui->tableColasPedidos->setItem(i, 3, new QTableWidgetItem(p.metodoPago));
        ui->tableColasPedidos->setItem(i, 4, new QTableWidgetItem("$" + QString::number(p.montoTotal, 'f', 0)));

        QTableWidgetItem *itemEstado = new QTableWidgetItem(p.estado);
        if (p.estado == "PENDIENTE") {
            itemEstado->setForeground(QBrush(QColor(180, 100, 0)));
        } else if (p.estado == "ENTREGADO") {
            itemEstado->setForeground(QBrush(QColor(40, 140, 40)));
        } else if (p.estado == "CANCELADO") {
            itemEstado->setForeground(QBrush(QColor(180, 40, 40)));
        }
        ui->tableColasPedidos->setItem(i, 5, itemEstado);
    }
}

void CajeroWindow::on_btnBuscarCodigo_clicked()
{
    QString codigo = ui->txtCodigoRetiro->text().trimmed().toUpper();

    // [CAJ-01] Validación de código requerido y formato
    if (codigo.isEmpty()) {
        QMessageBox::warning(this, "Código Requerido", "Por favor ingresa el código de 5 dígitos del comprobante del alumno.");
        return;
    }

    if (codigo.length() != 5) {
        QMessageBox::warning(this, "Formato Inválido", "El código de retiro debe tener exactamente 5 caracteres alfanuméricos.");
        return;
    }

    PedidoBuffet *p = SistemaBuffet::instancia().buscarPedidoPorCodigo(codigo);

    if (!p) {
        ui->lblStatusPedido->setText("❌ PEDIDO NO ENCONTRADO");
        ui->btnEntregarPedido->setEnabled(false);
        ui->btnCancelarPedido->setEnabled(false);
        QMessageBox::critical(this, "No Encontrado",
            QString("No se encontró ningún pedido registrado con el código '%1'.").arg(codigo));
        return;
    }

    m_codigoActual = codigo;

    // [CAJ-03 y CAJ-04] Control de Estados (Evitar Doble Entrega / Anulaciones)
    if (p->estado == "ENTREGADO") {
        ui->lblStatusPedido->setText("⚠️ ATENCIÓN: ESTE PEDIDO YA FUE ENTREGADO");
        ui->lblStatusPedido->setStyleSheet("color: #b78103; font-weight: bold;");
        ui->btnEntregarPedido->setEnabled(false);
        ui->btnCancelarPedido->setEnabled(false);
        QMessageBox::warning(this, "Doble Entrega Bloqueada",
            QString("El pedido '%1' ya fue entregado y cobrado por %2.\nNo es posible volver a entregarlo.")
                .arg(codigo, p->cajeroEntrega.isEmpty() ? "Personal de Buffet" : p->cajeroEntrega));
    } else if (p->estado == "CANCELADO") {
        ui->lblStatusPedido->setText("❌ ATENCIÓN: ESTE PEDIDO FUE ANULADO");
        ui->lblStatusPedido->setStyleSheet("color: #c62828; font-weight: bold;");
        ui->btnEntregarPedido->setEnabled(false);
        ui->btnCancelarPedido->setEnabled(false);
        QMessageBox::warning(this, "Pedido Cancelado",
            QString("El pedido '%1' fue cancelado y no puede ser entregado.").arg(codigo));
    } else {
        ui->lblStatusPedido->setText("✅ PEDIDO ENCONTRADO - PENDIENTE DE ENTREGA");
        ui->lblStatusPedido->setStyleSheet("color: #2e7d32; font-weight: bold;");
        ui->btnEntregarPedido->setEnabled(true);
        ui->btnCancelarPedido->setEnabled(true);
    }

    ui->lblDetalleAlumno->setText(QString("<b>Alumno:</b> %1 (DNI: %2 - %3)")
        .arg(p->nombreAlumno, p->dniAlumno, p->cursoAlumno));
    ui->lblDetalleRecreo->setText(QString("<b>Horario:</b> %1 | <b>Pago:</b> %2")
        .arg(p->recreoRetiro, p->metodoPago));

    // Cargar ítems comprados reales
    ui->tableDetalleItems->setRowCount(0);
    ui->tableDetalleItems->setColumnCount(3);
    QStringList headers;
    headers << "Producto" << "Cant." << "Subtotal";
    ui->tableDetalleItems->setHorizontalHeaderLabels(headers);
    ui->tableDetalleItems->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);

    for (int i = 0; i < p->items.size(); ++i) {
        ui->tableDetalleItems->insertRow(i);
        ui->tableDetalleItems->setItem(i, 0, new QTableWidgetItem(p->items[i].nombre));
        ui->tableDetalleItems->setItem(i, 1, new QTableWidgetItem(QString::number(p->items[i].cantidad)));
        ui->tableDetalleItems->setItem(i, 2, new QTableWidgetItem("$" + QString::number(p->items[i].subtotal(), 'f', 0)));
    }

    QString textoTotal = QString("Total a Cobrar: $%1").arg(QString::number(p->montoTotal, 'f', 0));
    if (p->metodoPago.contains("Efectivo", Qt::CaseInsensitive)) {
        textoTotal += QString(" (Abona con $%1 - Dar Vuelto: $%2)")
            .arg(QString::number(p->montoAbonado, 'f', 0))
            .arg(QString::number(p->vuelto, 'f', 0));
    }
    ui->lblTotalMonto->setText(textoTotal);
}

void CajeroWindow::on_btnEntregarPedido_clicked()
{
    if (m_codigoActual.isEmpty()) return;

    UsuarioSistema cajeroActual = SistemaBuffet::instancia().getUsuarioActual();
    QString nombreCajero = cajeroActual.nombre.isEmpty() ? "Cajero Mostrador" : (cajeroActual.nombre + " " + cajeroActual.apellido);

    QString errorMsg;
    bool ok = SistemaBuffet::instancia().entregarPedido(m_codigoActual, nombreCajero, errorMsg);

    if (!ok) {
        QMessageBox::critical(this, "Error de Entrega", errorMsg);
        return;
    }

    ui->lblStatusPedido->setText("🎉 PEDIDO ENTREGADO Y COBRADO EXITOSAMENTE");
    ui->lblStatusPedido->setStyleSheet("color: #2e7d32; font-weight: bold;");
    ui->btnEntregarPedido->setEnabled(false);
    ui->btnCancelarPedido->setEnabled(false);

    cargarPedidosDelSistema();

    QMessageBox::information(this, "Entrega Confirmada",
        QString("El pedido '%1' ha sido marcado como ENTREGADO y registrado en la auditoría.").arg(m_codigoActual));
}

void CajeroWindow::on_btnCancelarPedido_clicked()
{
    if (m_codigoActual.isEmpty()) return;

    bool ok = false;
    QString motivo = QInputDialog::getText(this, "Cancelar Pedido",
        "Ingresa el motivo de anulación del pedido:", QLineEdit::Normal, "Cancelado a solicitud del alumno", &ok);

    if (!ok || motivo.trimmed().isEmpty()) {
        QMessageBox::warning(this, "Motivo Requerido", "Es obligatorio ingresar un motivo para anular un pedido.");
        return;
    }

    UsuarioSistema cajeroActual = SistemaBuffet::instancia().getUsuarioActual();
    QString nombreCajero = cajeroActual.nombre.isEmpty() ? "Cajero Mostrador" : (cajeroActual.nombre + " " + cajeroActual.apellido);

    QString errorMsg;
    bool cancelado = SistemaBuffet::instancia().cancelarPedido(m_codigoActual, nombreCajero, motivo, errorMsg);

    if (!cancelado) {
        QMessageBox::critical(this, "Error al Cancelar", errorMsg);
        return;
    }

    ui->lblStatusPedido->setText("❌ PEDIDO CANCELADO");
    ui->lblStatusPedido->setStyleSheet("color: #c62828; font-weight: bold;");
    ui->btnEntregarPedido->setEnabled(false);
    ui->btnCancelarPedido->setEnabled(false);

    cargarPedidosDelSistema();

    QMessageBox::warning(this, "Pedido Cancelado",
        QString("El pedido '%1' ha sido anulado. El stock ha sido reincorporado al inventario.").arg(m_codigoActual));
}

void CajeroWindow::on_btnLogout_clicked()
{
    SistemaBuffet::instancia().cerrarSesion();
    LoginWindow *loginWin = new LoginWindow();
    loginWin->show();
    this->close();
}