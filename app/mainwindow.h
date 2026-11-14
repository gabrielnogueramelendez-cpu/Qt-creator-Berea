#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>

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
    void on_btnVerMilanesa_clicked();
    void on_btnVerEmpanada_clicked();
    void on_btnVerPizza_clicked();
    void on_btnVerPebete_clicked();
    void on_btnVerMedialuna_clicked();
    void on_btnVerEnsalada_clicked();
    void on_btnVerManzana_clicked();
    void on_btnVerChocolatada_clicked();
    void on_btnVerYogur_clicked();
    void on_btnVerAgua_clicked();
    void on_btnVerJugo_clicked();
    void on_btnVerGaseosa_clicked();
    void on_btnVerCafe_clicked();
    void on_btnVerBarra_clicked();
    void on_btnVerGalletitas_clicked();
    void on_btnVerAlfajor_clicked();
    void on_btnVerTurron_clicked();
    void on_btnVerFrutosSecos_clicked();
    void on_btnVerPolloPure_clicked();
    void on_btnVerCombo_clicked();
    void on_btnCheckout_clicked();
    void on_btnPerfil_clicked();
    void on_btnLogout_clicked();

private:
    Ui::MainWindow *ui;
    void actualizarBotonCarrito();
    void cargarFoto(QLabel *lblFoto, QString nombreProducto);
    void abrirDetalleProducto(int id, const QString &nombre, const QString &desc, double precio, int stock = 15);
};

#endif // MAINWINDOW_H
