#ifndef PRODUCTDETAILDIALOG_H
#define PRODUCTDETAILDIALOG_H

#include <QDialog>

namespace Ui {
class ProductDetailDialog;
}

class ProductDetailDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ProductDetailDialog(int productoId, const QString &nombre, const QString &desc, double precio, int stockDisponible = 15, QWidget *parent = nullptr);
    ~ProductDetailDialog();

    int getProductoId() const { return m_productoId; }
    QString getNombre() const { return m_nombre; }
    int getCantidad() const { return m_cantidad; }
    double getSubtotal() const { return m_precioUnitario * m_cantidad; }

private slots:
    void on_btnPlus_clicked();
    void on_btnMinus_clicked();
    void on_btnAddToCart_clicked();

private:
    Ui::ProductDetailDialog *ui;
    int m_productoId;
    QString m_nombre;
    double m_precioUnitario;
    int m_cantidad;
    int m_stockDisponible;
    void actualizarTotal();
};

#endif // PRODUCTDETAILDIALOG_H
