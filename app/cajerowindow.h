#ifndef CAJEROWINDOW_H
#define CAJEROWINDOW_H

#include <QWidget>

namespace Ui {
class CajeroWindow;
}

class CajeroWindow : public QWidget
{
    Q_OBJECT

public:
    explicit CajeroWindow(QWidget *parent = nullptr);
    ~CajeroWindow();

private slots:
    void on_btnBuscarCodigo_clicked();
    void on_btnEntregarPedido_clicked();
    void on_btnCancelarPedido_clicked();
    void on_btnLogout_clicked();

private:
    Ui::CajeroWindow *ui;
    QString m_codigoActual;
    void cargarPedidosDelSistema();
};

#endif // CAJEROWINDOW_H