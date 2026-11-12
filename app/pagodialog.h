#ifndef PAGODIALOG_H
#define PAGODIALOG_H

#include <QDialog>

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
    void on_comboMetodoPago_currentIndexChanged(int index);
    void on_txtMontoAbonado_textChanged(const QString &text);
    void on_btnCancelarPago_clicked();
    void on_btnConfirmarPedido_clicked();

private:
    Ui::PagoDialog *ui;
    double m_totalPagar;
    QString m_codigoGenerado;
    void cargarDatosAlumno();
};

#endif // PAGODIALOG_H
