#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnVerDetalleMenuDia1_clicked();
    void on_btnVerDetalleMenuDia2_clicked();
    void on_btnVerDetalleMilanesa_clicked();
    void on_btnVerDetalleEmpanada_clicked();
    void on_btnVerDetalleTarta_clicked();
    void on_btnVerDetallePebete_clicked();
    void on_btnVerDetalleAlfajor_clicked();
    void on_btnVerDetallePastafrola_clicked();
    void on_btnVerDetalleFactura_clicked();
    void on_btnVerDetalleMuffin_clicked();
    void on_btnVerDetalleGaseosa_clicked();
    void on_btnVerDetalleAgua_clicked();
    void on_btnVerDetalleJugo_clicked();
    void on_btnVerDetalleLicuado_clicked();
    void on_btnCheckout_clicked();
    void on_btnLogout_clicked();

private:
    Ui::MainWindow *ui;
    void actualizarBotonCarrito();
    void abrirDetalleProducto(int id, const QString &nombre, const QString &desc, double precio, int stock = 15);
};

#endif // MAINWINDOW_H
