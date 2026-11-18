#ifndef PRODUCTDETAILDIALOG_H
#define PRODUCTDETAILDIALOG_H

#include <QDialog>
#include "sistemabuffet.h"

namespace Ui {
class ProductDetailDialog;
}

// Ventana de detalle: foto, descripción, precio y cuántas unidades llevar
class ProductDetailDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ProductDetailDialog(Producto productoElegido, QWidget *parent = nullptr);
    ~ProductDetailDialog();

private slots:
    void on_btnPlus_clicked();
    void on_btnMinus_clicked();
    void on_btnAddToCart_clicked();

private:
    Ui::ProductDetailDialog *ui;
    Producto producto;      // el producto que se está mirando
    int cantidadElegida;    // cuántas unidades quiere llevar
    void actualizarTotal();
    int maximoPermitido();
};

#endif // PRODUCTDETAILDIALOG_H
