#ifndef PAGODIALOG_H
#define PAGODIALOG_H

#include <QDialog>

namespace Ui {
class PagoDialog;
}

// Ventana de pago: datos del alumno, método de pago y confirmación del pedido
class PagoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PagoDialog(QWidget *parent = nullptr);
    ~PagoDialog();

    // Datos del pedido confirmado que usa el comprobante
    QString getCodigoGenerado() { return codigoGenerado; }
    double getMontoAbonado() { return montoAbonado; }   // con cuánto pagó en efectivo
    double getVuelto() { return vuelto; }

private slots:
    void on_rbEfectivo_clicked();
    void on_rbMercadoPago_clicked();
    void on_rbSaldo_clicked();
    void on_txtMontoAbonado_textChanged(const QString &textoMonto);
    void on_btnCancelarPago_clicked();
    void on_btnConfirmarPedido_clicked();

private:
    Ui::PagoDialog *ui;
    double totalAPagar;
    QString codigoGenerado;
    double montoAbonado;   // la BDD no lo guarda: solo se muestra en el comprobante
    double vuelto;
    void cargarDatosAlumno();
    void habilitarCamposMetodo();
    bool guardarPedidoEnBDD(int metodoPagoID, int franjaID);
};

#endif // PAGODIALOG_H
