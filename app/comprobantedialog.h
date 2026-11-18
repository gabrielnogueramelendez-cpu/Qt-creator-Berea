#ifndef COMPROBANTEDIALOG_H
#define COMPROBANTEDIALOG_H

#include <QDialog>

namespace Ui {
class ComprobanteDialog;
}

// Comprobante final: código de retiro y resumen del pedido
class ComprobanteDialog : public QDialog
{
    Q_OBJECT

public:
    // montoAbonado y vuelto solo se usan si pagó en efectivo
    explicit ComprobanteDialog(const QString &codigoRetiro, double montoAbonado, double vuelto, QWidget *parent = nullptr);
    ~ComprobanteDialog();

private slots:
    void on_btnClose_clicked();

private:
    Ui::ComprobanteDialog *ui;
    QString codigo;
    double montoAbonado;
    double vuelto;
    void cargarDatosComprobante();
};

#endif // COMPROBANTEDIALOG_H
