#ifndef DEVWINDOW_H
#define DEVWINDOW_H

#include <QWidget>
#include <QStringList>

namespace Ui {
class DevWindow;
}

class DevWindow : public QWidget
{
    Q_OBJECT

public:
    explicit DevWindow(QWidget *parent = nullptr);
    ~DevWindow();

private slots:
    void on_listSP_currentRowChanged(int fila);
    void on_btnEjecutarSP_clicked();
    void on_listPruebas_currentRowChanged(int fila);
    void on_btnAbrirVistaAlumno_clicked();
    void on_btnAbrirVistaCaja_clicked();
    void on_btnAbrirVistaAdmin_clicked();
    void on_btnLogoutDev_clicked();

private:
    Ui::DevWindow *ui;

    // Listas paralelas leídas de 05_Backend (SPs) y 06_PruebasSQL (pruebas)
    QStringList spNombres;
    QStringList spSchemas;
    QStringList spCodigos;
    QStringList pruebasTitulos;
    QStringList pruebasCodigos;

    QString leerArchivo(QString ruta);
    void cargarSPs();
    void cargarPruebas();
};

#endif // DEVWINDOW_H
