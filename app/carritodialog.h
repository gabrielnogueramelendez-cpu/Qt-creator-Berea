#ifndef CARRITODIALOG_H
#define CARRITODIALOG_H

#include <QDialog>

namespace Ui {
class CarritoDialog;
}

// Ventana del carrito: ver, cambiar cantidades, quitar productos y pasar al pago
class CarritoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CarritoDialog(QWidget *parent = nullptr);
    ~CarritoDialog();

    // true si se cerró con "PROCEDER AL PAGO"
    bool fueHaciaElPago() { return eligioPagar; }

private slots:
    void on_btnModificarCantidad_clicked();
    void on_tableCarrito_cellClicked(int fila, int columna);
    void on_btnEliminarItem_clicked();
    void on_btnVaciarCarrito_clicked();
    void on_btnSeguirComprando_clicked();
    void on_btnProcederPago_clicked();

private:
    Ui::CarritoDialog *ui;
    bool eligioPagar;
    void recargarTablaCarrito();
};

#endif // CARRITODIALOG_H
