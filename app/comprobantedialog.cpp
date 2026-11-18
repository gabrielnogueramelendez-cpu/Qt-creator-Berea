#include "comprobantedialog.h"
#include "ui_comprobantedialog.h"
#include "sistemabuffet.h"

// =====================================================================
// Constructor: recibe el código de retiro y muestra el comprobante
// =====================================================================
ComprobanteDialog::ComprobanteDialog(const QString &codigoRetiro, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ComprobanteDialog)
{
    ui->setupUi(this);
    setWindowTitle("Comprobante de Pedido - El Buffet de Alejandro");

    codigo = codigoRetiro;
    cargarDatosComprobante();
}

ComprobanteDialog::~ComprobanteDialog()
{
    delete ui;
}

// =====================================================================
// cargarDatosComprobante: busca el pedido por su código y muestra los datos
// =====================================================================
void ComprobanteDialog::cargarDatosComprobante()
{
    ui->lblCode->setText(codigo);

    int posicionPedido = SistemaBuffet::buscarPedido(codigo);
    if (posicionPedido == -1) {
        return;
    }

    Pedido pedido = SistemaBuffet::pedidos[posicionPedido];

    QString resumen = "Cliente: " + pedido.cliente + " (DNI " + pedido.dni + " · " + pedido.curso + ")\n"
                    + "Retiro: " + pedido.recreo + "\n"
                    + "Pago: " + pedido.metodo + "\n"
                    + "Estado: " + pedido.estado + "\n\n"
                    + pedido.detalle + "\n"
                    + "Total: $" + QString::number(pedido.monto, 'f', 0);

    // Si pagó en efectivo, se agrega con cuánto paga y el vuelto
    if (pedido.metodo.contains("Efectivo", Qt::CaseInsensitive)) {
        resumen = resumen + "\nPagás con $" + QString::number(pedido.abonado, 'f', 0)
                + " · Vuelto $" + QString::number(pedido.vuelto, 'f', 0);
    }

    ui->lblDetails->setText(resumen);
    ui->lblRuleNotice->setText("Dictá este código o mostrá esta pantalla en el mostrador para retirar tu pedido.");
}

// =====================================================================
// Botón CERRAR
// =====================================================================
void ComprobanteDialog::on_btnClose_clicked()
{
    accept();
}
