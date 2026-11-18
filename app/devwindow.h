#ifndef DEVWINDOW_H
#define DEVWINDOW_H

#include <QWidget>
#include <QList>

namespace Ui {
class DevWindow;
}

// Un stored procedure leído de 05_Backend.sql
struct ProcedimientoSQL {
    QString nombre;
    QString schema;
    QString codigo;
};

// Una prueba leída de 06_PruebasSQL.sql
struct PruebaSQL {
    QString titulo;
    QString codigo;
};

// Panel del desarrollador: ver y ejecutar SPs, ver pruebas y abrir las otras vistas
class DevWindow : public QWidget
{
    Q_OBJECT

public:
    explicit DevWindow(QWidget *parent = nullptr);
    ~DevWindow();

private slots:
    void on_listSP_currentRowChanged(int filaElegida);
    void on_btnEjecutarSP_clicked();
    void on_listPruebas_currentRowChanged(int filaElegida);
    void on_btnAbrirVistaAlumno_clicked();
    void on_btnAbrirVistaCaja_clicked();
    void on_btnAbrirVistaAdmin_clicked();
    void on_btnLogoutDev_clicked();

private:
    Ui::DevWindow *ui;
    QList<ProcedimientoSQL> procedimientos;
    QList<PruebaSQL> pruebas;

    QString leerArchivo(QString ruta);
    void cargarSPs();
    void cargarPruebas();
};

#endif // DEVWINDOW_H
