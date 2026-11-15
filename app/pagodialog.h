#ifndef PAGODIALOG_H
#define PAGODIALOG_H

#include <QDialog>
#include <QButtonGroup>

namespace Ui {
class PagoDialog;
}

class PagoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PagoDialog(QWidget *parent = nullptr);
    ~PagoDialog();

    QString getCodigoGenerado() const { return m_codigoGenerado; }

private slots:
    void on_rbEfectivo_clicked();
    void on_rbMercadoPago_clicked();
    void on_rbSaldo_clicked();
    void on_txtMontoAbonado_textChanged(const QString &text);
    void on_btnCancelarPago_clicked();
    void on_btnConfirmarPedido_clicked();

private:
    bool guardarPedidoEnBDD(int metodoPagoID, int franjaID, QString &codigo);
    Ui::PagoDialog *ui;
    QButtonGroup *grupoMetodo;
    double m_totalPagar;
    QString m_codigoGenerado;
    void cargarDatosAlumno();
    void habilitarCamposMetodo();
};

#endif // PAGODIALOG_H
