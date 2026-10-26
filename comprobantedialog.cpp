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

    PedidoBuffet *p = SistemaBuffet::instancia().buscarPedidoPorCodigo(m_codigo);
    if (p) {
        QString itemsTexto = "";
        for (const auto &it : p->items) {
            itemsTexto += QString("  • %1 x %2 ($%3)\n")
                .arg(it.cantidad)
                .arg(it.nombre)
                .arg(QString::number(it.subtotal(), 'f', 0));
        }

        QString resumen = QString(
            "📋 RESUMEN OFICIAL DEL PEDIDO:\n"
            "- Estudiante: %1 (DNI %2 - %3)\n"
            "- Horario de Retiro: %4\n"
            "- Método de Pago: %5\n"
            "- Estado: %6\n"
            "- Ítems:\n%7"
            "- Total: $%8\n"
        ).arg(p->nombreAlumno)
         .arg(p->dniAlumno)
         .arg(p->cursoAlumno)
         .arg(p->recreoRetiro)
         .arg(p->metodoPago)
         .arg(p->estado)
         .arg(itemsTexto)
         .arg(QString::number(p->montoTotal, 'f', 0));

        if (p->metodoPago.contains("Efectivo", Qt::CaseInsensitive)) {
            resumen += QString("- Abonó con: $%1 | Vuelto a recibir: $%2\n")
                .arg(QString::number(p->montoAbonado, 'f', 0))
                .arg(QString::number(p->vuelto, 'f', 0));
        }

        ui->lblDetails->setText(resumen);
        ui->lblRuleNotice->setText(QString("📱 Solo necesitas dictar el código '%1' o mostrar esta pantalla en el mostrador para retirar tu pedido.").arg(m_codigo));
    }
}

void ComprobanteDialog::on_btnClose_clicked()
{
    accept();
}
