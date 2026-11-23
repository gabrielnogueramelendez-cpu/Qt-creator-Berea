// comprobante.cpp: Comprobante con el código de retiro.  Diseño: comprobantedialog.ui

#include "pantallas.h"
#include "ui_comprobantedialog.h"
#include "datos.h"
#include <QDialog>

static QDialog *dialogo = NULL;

// Sirve para el botón CERRAR del comprobante.
static void cerrar()
{
    dialogo->accept();
}

// Sirve para abrir el comprobante.
void abrir_comprobante(QWidget *padre, const QString &codigo, double abonado, double vuelto)
{
    dialogo = new QDialog(padre);
    Ui::ComprobanteDialog *ui = new Ui::ComprobanteDialog();
    ui->setupUi(dialogo);
    dialogo->setWindowTitle("Comprobante de Pedido - El Buffet de Alejandro");
    ui->lblCode->setText(codigo);

    cargar_pedidos();
    int i = buscar_pedido(codigo);
    if (i != -1) {
        QString resumen = QString("Cliente: %1 (DNI %2 · %3)\nRetiro: %4\nPago: %5\nEstado: %6\n\n%7\nTotal: %8")
            .arg(pedidos_clientes[i], pedidos_dnis[i], pedidos_cursos[i], pedidos_recreos[i],
                 pedidos_metodos[i], pedidos_estados[i], pedidos_detalles[i], pesos(pedidos_montos[i]));
        // Con cuánto pagó en efectivo lo pasa la pantalla de pago
        if (pedidos_metodos[i].contains("Efectivo", Qt::CaseInsensitive)) {
            resumen += QString("\nPagás con %1 · Vuelto %2").arg(pesos(abonado), pesos(vuelto));
        }
        ui->lblDetails->setText(resumen);
        ui->lblRuleNotice->setText("Dictá este código o mostrá esta pantalla en el mostrador para retirar tu pedido.");
    }

    QObject::connect(ui->btnClose, &QPushButton::clicked, cerrar);
    dialogo->exec();

    delete ui;
    delete dialogo;
    dialogo = NULL;
}
