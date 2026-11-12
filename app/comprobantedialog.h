#ifndef COMPROBANTEDIALOG_H
#define COMPROBANTEDIALOG_H

#include <QDialog>

namespace Ui {
class ComprobanteDialog;
}

class ComprobanteDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ComprobanteDialog(const QString &codigoRetiro = "3C6MB", QWidget *parent = nullptr);
    ~ComprobanteDialog();

private slots:
    void on_btnClose_clicked();

private:
    Ui::ComprobanteDialog *ui;
    QString m_codigo;
    void cargarDatosComprobante();
};

#endif // COMPROBANTEDIALOG_H
