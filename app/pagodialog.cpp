#include "pagodialog.h"
#include "ui_pagodialog.h"
#include "sistemabuffet.h"
#include <QMessageBox>
#include <QRegularExpression>
#include <QRegularExpressionValidator>

PagoDialog::PagoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PagoDialog),
    m_totalPagar(0.0)
{
    ui->setupUi(this);
    setWindowTitle("💳 Verificación y Pago - El Buffet de Alejandro");

    m_totalPagar = SistemaBuffet::instancia().getTotalCarrito();
    ui->lblResumenTotal->setText(QString("Total a Pagar del Pedido: $%1")
        .arg(QString::number(m_totalPagar, 'f', 0)));

    // Validador de efectivo (números enteros o decimales)
    QRegularExpression regexMonto("^[0-9]{1,6}(\\.[0-9]{1,2})?$");
    ui->txtMontoAbonado->setValidator(new QRegularExpressionValidator(regexMonto, this));

    // Validador comprobante MP (4 dígitos)
    QRegularExpression regexMP("^[0-9]{4}$");
    ui->txtComprobanteMP->setValidator(new QRegularExpressionValidator(regexMP, this));
    ui->txtComprobanteMP->setMaxLength(4);

    cargarDatosAlumno();
    on_comboMetodoPago_currentIndexChanged(0);
}

PagoDialog::~PagoDialog()
{
    delete ui;
}

void PagoDialog::cargarDatosAlumno()
{
    UsuarioSistema actual = SistemaBuffet::instancia().getUsuarioActual();
    if (!actual.dni.isEmpty()) {
        ui->txtPagoDni->setText(actual.dni);
        ui->txtPagoNombre->setText(actual.nombre + " " + actual.apellido);
        ui->txtPagoCurso->setText(actual.curso);
        ui->lblSaldoDisponible->setText(QString("Saldo disponible en tu cuenta escolar: $%1")
            .arg(QString::number(actual.saldoCuenta, 'f', 0)));

        if (actual.saldoCuenta >= m_totalPagar) {
            ui->lblEstadoSaldo->setText(QString("✅ Saldo suficiente. Te quedarán $%1.")
                .arg(QString::number(actual.saldoCuenta - m_totalPagar, 'f', 0)));
            ui->lblEstadoSaldo->setStyleSheet("color: #2e7d32; font-weight: bold;");
        } else {
            ui->lblEstadoSaldo->setText("❌ Saldo insuficiente en cuenta escolar para cubrir el total.");
            ui->lblEstadoSaldo->setStyleSheet("color: #c62828; font-weight: bold;");
        }
    } else {
        ui->txtPagoDni->setText("44123456");
        ui->txtPagoNombre->setText("Jorge Dahán");
        ui->txtPagoCurso->setText("4° Año B");
    }
}

void PagoDialog::on_comboMetodoPago_currentIndexChanged(int index)
{
    ui->stackedDetallePago->setCurrentIndex(index);
    if (index == 0) {
        // Efectivo: sugerir el total exacto o recalcular vuelto
        on_txtMontoAbonado_textChanged(ui->txtMontoAbonado->text());
    }
}

void PagoDialog::on_txtMontoAbonado_textChanged(const QString &text)
{
    bool ok = false;
    double abonado = text.toDouble(&ok);
    if (ok && abonado >= m_totalPagar) {
        double vuelto = abonado - m_totalPagar;
        ui->lblVueltoCalculado->setText(QString("✅ Vuelto a recibir en caja: $%1")
            .arg(QString::number(vuelto, 'f', 0)));
        ui->lblVueltoCalculado->setStyleSheet("color: #2e7d32; font-weight: bold;");
    } else if (ok && abonado < m_totalPagar) {
        double falta = m_totalPagar - abonado;
        ui->lblVueltoCalculado->setText(QString("⚠️ Monto insuficiente (faltan $%1)").arg(QString::number(falta, 'f', 0)));
        ui->lblVueltoCalculado->setStyleSheet("color: #c62828; font-weight: bold;");
    } else {
        ui->lblVueltoCalculado->setText("Vuelto a recibir en caja: $0");
        ui->lblVueltoCalculado->setStyleSheet("color: #555;");
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

    // 1. Validaciones del Estudiante
    if (dni.isEmpty() || nombre.isEmpty() || curso.isEmpty()) {
        QMessageBox::warning(this, "Datos Incompletos", "Por favor completa la verificación de los datos del estudiante.");
        return;
    }

    if (dni.length() < 7 || dni.length() > 8) {
        QMessageBox::warning(this, "DNI Inválido", "El DNI del estudiante debe tener entre 7 y 8 dígitos numéricos.");
        return;
    }

    // 2. Validación de Términos
    if (!ui->chkConfirmarTerminos->isChecked()) {
        QMessageBox::warning(this, "Confirmación Requerida",
            "Debes marcar la casilla declarando que los datos son correctos y comprometiéndote a retirar en el recreo indicado.");
        return;
    }

    // 3. Validación según Método de Pago
    QString metodo = ui->comboMetodoPago->currentText();
    double montoAbonado = m_totalPagar;

    if (metodo.contains("Efectivo", Qt::CaseInsensitive)) {
        bool ok = false;
        montoAbonado = ui->txtMontoAbonado->text().trimmed().toDouble(&ok);
        if (!ok || montoAbonado <= 0) {
            QMessageBox::warning(this, "Monto Abonado Requerido",
                "Por favor ingresa con cuánto dinero vas a abonar en el mostrador para preparar tu vuelto.");
            return;
        }
        if (montoAbonado < m_totalPagar) {
            QMessageBox::critical(this, "Monto Insuficiente",
                QString("El monto ingresado ($%1) es inferior al total del pedido ($%2).")
                    .arg(QString::number(montoAbonado, 'f', 0))
                    .arg(QString::number(m_totalPagar, 'f', 0)));
            return;
        }
    } else if (metodo.contains("Saldo", Qt::CaseInsensitive)) {
        UsuarioSistema actual = SistemaBuffet::instancia().getUsuarioActual();
        if (actual.saldoCuenta < m_totalPagar) {
            QMessageBox::critical(this, "Saldo Insuficiente",
                QString("No cuentas con saldo suficiente en tu cuenta escolar ($%1 disponible) para cubrir esta compra de $%2.")
                    .arg(QString::number(actual.saldoCuenta, 'f', 0))
                    .arg(QString::number(m_totalPagar, 'f', 0)));
            return;
        }
    } else if (metodo.contains("Mercado", Qt::CaseInsensitive)) {
        QString comp = ui->txtComprobanteMP->text().trimmed();
        if (comp.length() != 4) {
            QMessageBox::warning(this, "Comprobante Inválido",
                "Por favor ingresa los 4 dígitos finales del comprobante de transferencia o Mercado Pago.");
            return;
        }
    }

    // 4. Crear pedido a través del Sistema Central
    QString recreo = ui->comboRecreo->currentText();
    QString errorMsg;
    QString codigo = SistemaBuffet::instancia().crearPedido(recreo, metodo, montoAbonado, errorMsg);

    if (codigo.isEmpty()) {
        QMessageBox::critical(this, "Error al generar pedido", errorMsg);
        return;
    }

    m_codigoGenerado = codigo;
    accept();
}
