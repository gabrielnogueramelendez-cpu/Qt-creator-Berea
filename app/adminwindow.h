#ifndef ADMINWINDOW_H
#define ADMINWINDOW_H

#include <QWidget>
#include <QStringList>

namespace Ui {
class AdminWindow;
}

// Administración: inventario (alta, modificación, baja) y registro de movimientos
class AdminWindow : public QWidget
{
    Q_OBJECT

public:
    explicit AdminWindow(QWidget *parent = nullptr);
    ~AdminWindow();

private slots:
    void on_btnAgregarProd_clicked();
    void on_btnGuardarEdit_clicked();
    void on_btnEliminarProd_clicked();
    void on_btnLogout_clicked();

private:
    Ui::AdminWindow *ui;
    void cargarInventario();
    void prepararTablaLogs();
    void cargarLogsDesdeBDD();
    void registrarLog(QString evento, QString detalle);
    void limpiarFormulario();
    bool respuestaCorrecta(QStringList respuesta);
};

#endif // ADMINWINDOW_H
