#ifndef CAJEROWINDOW_H
#define CAJEROWINDOW_H

#include <QWidget>

namespace Ui {
class CajeroWindow;
}

// Mostrador: fila de pedidos, búsqueda por código, entregar o cancelar
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
    void on_tableColasPedidos_cellClicked(int fila, int columna);
    void on_btnLogout_clicked();

private:
    Ui::CajeroWindow *ui;
    QString codigoSeleccionado;   // código del pedido que se está mirando
    void cargarPedidosDelSistema();
    void cambiarEstadoEnBDD(int posicionPedido, int estadoID);
};

#endif // CAJEROWINDOW_H
