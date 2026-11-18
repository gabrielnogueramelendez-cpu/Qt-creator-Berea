#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>

namespace Ui {
class MainWindow;
}

// Catálogo del estudiante: productos, menú del día, carrito y perfil
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Un botón "VER" por producto (el número es el ProductoID)
    void on_btnVerMilanesa_clicked();      // 1
    void on_btnVerEmpanada_clicked();      // 2
    void on_btnVerPizza_clicked();         // 3
    void on_btnVerPebete_clicked();        // 4
    void on_btnVerMedialuna_clicked();     // 5
    void on_btnVerEnsalada_clicked();      // 6
    void on_btnVerManzana_clicked();       // 7
    void on_btnVerChocolatada_clicked();   // 8
    void on_btnVerYogur_clicked();         // 9
    void on_btnVerAgua_clicked();          // 10
    void on_btnVerJugo_clicked();          // 11
    void on_btnVerGaseosa_clicked();       // 12
    void on_btnVerCafe_clicked();          // 13
    void on_btnVerBarra_clicked();         // 14
    void on_btnVerGalletitas_clicked();    // 15
    void on_btnVerAlfajor_clicked();       // 16
    void on_btnVerTurron_clicked();        // 17
    void on_btnVerFrutosSecos_clicked();   // 18
    void on_btnVerPolloPure_clicked();     // 19
    void on_btnVerCombo_clicked();         // 20
    void on_btnVerMenu_clicked();          // menú del día
    void on_btnCheckout_clicked();
    void on_btnPerfil_clicked();
    void on_btnLogout_clicked();

private:
    Ui::MainWindow *ui;
    void actualizarBotonCarrito();
    void mostrarMenuDelDia();
    void mostrarPrecios();
    void cargarFoto(QLabel *etiquetaFoto, QString nombreArchivo);
    void abrirDetalleProducto(int productoID);
};

#endif // MAINWINDOW_H
