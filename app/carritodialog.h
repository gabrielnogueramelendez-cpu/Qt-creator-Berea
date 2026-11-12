#ifndef CARRITODIALOG_H
#define CARRITODIALOG_H

#include <QDialog>

namespace Ui {
class CarritoDialog;
}

class CarritoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CarritoDialog(QWidget *parent = nullptr);
    ~CarritoDialog();

    bool fueHaciaElPago() const { return m_haciaElPago; }

private slots:
    void on_btnModificarCantidad_clicked();
    void on_btnEliminarItem_clicked();
    void on_btnVaciarCarrito_clicked();
    void on_btnSeguirComprando_clicked();
    void on_btnProcederPago_clicked();

private:
    Ui::CarritoDialog *ui;
    bool m_haciaElPago;
    void recargarTablaCarrito();
};

#endif // CARRITODIALOG_H
