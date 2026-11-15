#include "pagodialog.h"
#include "ui_pagodialog.h"
#include "sistemabuffet.h"
#include <QMessageBox>
#include <QIntValidator>
#include <QRandomGenerator>

PagoDialog::PagoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PagoDialog),
    m_totalPagar(0.0)
{
    ui->setupUi(this);
    setWindowTitle("Pago - El Buffet de Alejandro");

    // Agrupador lógico de los métodos de pago (guía Configurador de Pedidos)
    grupoMetodo = new QButtonGroup(this);
    grupoMetodo->addButton(ui->rbEfectivo, 1);
    grupoMetodo->addButton(ui->rbMercadoPago, 2);
    grupoMetodo->addButton(ui->rbSaldo, 3);

    // Solo números en monto y comprobante (guía Gestor de Inventario)
    ui->txtMontoAbonado->setValidator(new QIntValidator(0, 999999, this));
    ui->txtComprobanteMP->setValidator(new QIntValidator(0, 9999, this));
    ui->txtComprobanteMP->setMaxLength(4);

    m_totalPagar = SistemaBuffet::getTotalCarrito();
    ui->lblResumenTotal->setText("Total: $" + QString::number(m_totalPagar, 'f', 0));

    cargarDatosAlumno();
    habilitarCamposMetodo();
}

PagoDialog::~PagoDialog()
{
    delete ui;
}

void PagoDialog::cargarDatosAlumno()
{
    ui->txtPagoDni->setText(SistemaBuffet::usuarioDni);
    ui->txtPagoNombre->setText(SistemaBuffet::usuarioNombre + " " + SistemaBuffet::usuarioApellido);
    ui->txtPagoCurso->setText(SistemaBuffet::usuarioCurso);
    ui->lblSaldoDisponible->setText("Saldo disponible: $" + QString::number(SistemaBuffet::usuarioSaldo, 'f', 0));

    if (SistemaBuffet::usuarioSaldo >= m_totalPagar) {
        ui->lblEstadoSaldo->setText(QString("Te quedarán $%1 después de pagar.")
            .arg(QString::number(SistemaBuffet::usuarioSaldo - m_totalPagar, 'f', 0)));
    } else {
        ui->lblEstadoSaldo->setText("El saldo no alcanza para cubrir el total.");
    }
}

void PagoDialog::habilitarCamposMetodo()
{
    // Cada método enciende solo los campos que necesita
    int metodo = grupoMetodo->checkedId();
    ui->txtMontoAbonado->setEnabled(metodo == 1);
    ui->lblVueltoCalculado->setEnabled(metodo == 1);
    ui->txtComprobanteMP->setEnabled(metodo == 2);
    ui->lblSaldoDisponible->setEnabled(metodo == 3);
    ui->lblEstadoSaldo->setEnabled(metodo == 3);
}

void PagoDialog::on_rbEfectivo_clicked()
{
    habilitarCamposMetodo();
}

void PagoDialog::on_rbMercadoPago_clicked()
{
    habilitarCamposMetodo();
}

void PagoDialog::on_rbSaldo_clicked()
{
    habilitarCamposMetodo();
}

void PagoDialog::on_txtMontoAbonado_textChanged(const QString &text)
{
    double abonado = text.toDouble();
    if (text.isEmpty()) {
        ui->lblVueltoCalculado->setText("Vuelto: $0");
    } else if (abonado >= m_totalPagar) {
        ui->lblVueltoCalculado->setText("Vuelto: $" + QString::number(abonado - m_totalPagar, 'f', 0));
    } else {
        ui->lblVueltoCalculado->setText("Faltan $" + QString::number(m_totalPagar - abonado, 'f', 0));
    }
}

void PagoDialog::on_btnCancelarPago_clicked()
{
    reject();
}

void PagoDialog::on_btnConfirmarPedido_clicked()
{
    QString dni = ui->txtPagoDni->text().trimmed();
    QString nombre = ui->txtPagoNombre->text().trimmed();
    QString curso = ui->txtPagoCurso->text().trimmed();

    if (dni.isEmpty() || nombre.isEmpty() || curso.isEmpty()) {
        QMessageBox::warning(this, "Datos Incompletos", "Completá tus datos.");
        return;
    }

    // El primer ítem del combo es el texto "Seleccioná un horario..."
    if (ui->comboRecreo->currentIndex() == 0) {
        QMessageBox::warning(this, "Error", "Debe seleccionar un horario de retiro.");
        return;
    }

    int selectMetodo = grupoMetodo->checkedId();
    if (selectMetodo == -1) {
        QMessageBox::warning(this, "Error", "Debe seleccionar un método de pago.");
        return;
    }

    if (!ui->chkConfirmarTerminos->isChecked()) {
        QMessageBox::warning(this, "Confirmación Requerida",
            "Marcá la casilla para confirmar los datos y el horario de retiro.");
        return;
    }

    QString metodo;
    double montoAbonado = m_totalPagar;
    double vuelto = 0.0;

    switch (selectMetodo) {
    case 1: // Efectivo
        metodo = ui->rbEfectivo->text();
        montoAbonado = ui->txtMontoAbonado->text().toDouble();
        if (ui->txtMontoAbonado->text().isEmpty() || montoAbonado < m_totalPagar) {
            QMessageBox::warning(this, "Monto Insuficiente", "El monto ingresado no alcanza para cubrir el total.");
            return;
        }
        vuelto = montoAbonado - m_totalPagar;
        break;
    case 2: // Mercado Pago
        metodo = ui->rbMercadoPago->text();
        if (ui->txtComprobanteMP->text().length() != 4) {
            QMessageBox::warning(this, "Comprobante Inválido", "Ingresá los 4 dígitos finales del comprobante.");
            return;
        }
        break;
    case 3: // Saldo de cuenta escolar
        metodo = ui->rbSaldo->text();
        if (SistemaBuffet::usuarioSaldo < m_totalPagar) {
            QMessageBox::warning(this, "Saldo Insuficiente", "El saldo de tu cuenta escolar no alcanza.");
            return;
        }
        SistemaBuffet::usuarioSaldo -= m_totalPagar;
        break;
    }

    QString recreo = ui->comboRecreo->currentText();

    // Código de retiro de 5 caracteres al azar (Codigo_Retiro CHAR(5) UNIQUE en la BDD)
    QString caracteres = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    QString codigo;
    do {
        codigo = "";
        for (int i = 0; i < 5; i++)
            codigo += caracteres[QRandomGenerator::global()->bounded(caracteres.length())];
    } while (SistemaBuffet::pedidosCodigos.contains(codigo));
    // Con BDD: CALL sp_generar_retiro(VentaID, franja, codigo); si el UNIQUE lo rechaza, se sortea otro

    // Registrar pedido en las listas paralelas de SistemaBuffet
    SistemaBuffet::pedidosCodigos.append(codigo);
    SistemaBuffet::pedidosAlumnos.append(nombre);
    SistemaBuffet::pedidosCursos.append(curso);
    SistemaBuffet::pedidosDnis.append(dni);
    SistemaBuffet::pedidosRecreos.append(recreo);
    SistemaBuffet::pedidosMetodos.append(metodo);
    SistemaBuffet::pedidosMontos.append(m_totalPagar);
    SistemaBuffet::pedidosAbonados.append(montoAbonado);
    SistemaBuffet::pedidosVueltos.append(vuelto);
    SistemaBuffet::pedidosEstados.append("PENDIENTE");

    QString itemsTexto = "";
    for (int i = 0; i < SistemaBuffet::carritoNombres.size(); ++i) {
        itemsTexto += QString("%1 x %2 ($%3)\n")
            .arg(SistemaBuffet::carritoCantidades[i])
            .arg(SistemaBuffet::carritoNombres[i])
            .arg(QString::number(SistemaBuffet::carritoPrecios[i] * SistemaBuffet::carritoCantidades[i], 'f', 0));
    }
    SistemaBuffet::pedidosDetalles.append(itemsTexto);

    // Restar del stock lo vendido (con BDD: UPDATE Producto SET Stock_Producto = Stock_Producto - ?)
    for (int i = 0; i < SistemaBuffet::carritoIds.size(); ++i) {
        SistemaBuffet::productosStock[SistemaBuffet::carritoIds[i]] -= SistemaBuffet::carritoCantidades[i];
    }

    // Vaciar el carrito tras confirmar la compra
    SistemaBuffet::vaciarCarrito();

    m_codigoGenerado = codigo;
    accept();
}
