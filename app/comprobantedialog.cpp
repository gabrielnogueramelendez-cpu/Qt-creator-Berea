#include "comprobantedialog.h"
#include "ui_comprobantedialog.h"
#include "sistemabuffet.h"

ComprobanteDialog::ComprobanteDialog(const QString &codigoRetiro, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ComprobanteDialog),
    m_codigo(codigoRetiro)
{
    ui->setupUi(this);
    setWindowTitle("Comprobante de Pedido - El Buffet de Alejandro");
    cargarDatosComprobante();
}

ComprobanteDialog::~ComprobanteDialog()
{
    delete ui;
}

void ComprobanteDialog::cargarDatosComprobante()
{
    ui->lblCode->setText(m_codigo);

    int idx = SistemaBuffet::pedidosCodigos.indexOf(m_codigo);
    if (idx != -1) {
        QString itemsTexto = SistemaBuffet::pedidosDetalles[idx];

        QString resumen = QString(
            "Estudiante: %1 (DNI %2 · %3)\n"
            "Retiro: %4\n"
            "Pago: %5\n"
            "Estado: %6\n\n"
            "%7\n"
            "Total: $%8"
        ).arg(SistemaBuffet::pedidosAlumnos[idx])
         .arg(SistemaBuffet::pedidosDnis[idx])
         .arg(SistemaBuffet::pedidosCursos[idx])
         .arg(SistemaBuffet::pedidosRecreos[idx])
         .arg(SistemaBuffet::pedidosMetodos[idx])
         .arg(SistemaBuffet::pedidosEstados[idx])
         .arg(itemsTexto)
         .arg(QString::number(SistemaBuffet::pedidosMontos[idx], 'f', 0));

        if (SistemaBuffet::pedidosMetodos[idx].contains("Efectivo", Qt::CaseInsensitive)) {
            resumen += QString("\nPagás con $%1 · Vuelto $%2")
                .arg(QString::number(SistemaBuffet::pedidosAbonados[idx], 'f', 0))
                .arg(QString::number(SistemaBuffet::pedidosVueltos[idx], 'f', 0));
        }

        ui->lblDetails->setText(resumen);
        ui->lblRuleNotice->setText("Dictá este código o mostrá esta pantalla en el mostrador para retirar tu pedido.");
    }
}

void ComprobanteDialog::on_btnClose_clicked()
{
    accept();
}
