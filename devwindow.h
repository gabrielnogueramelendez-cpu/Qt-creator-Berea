#ifndef DEVWINDOW_H
#define DEVWINDOW_H

#include <QWidget>

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
    void on_btnTestConexion_clicked();
    void on_btnEjecutarSP_clicked();
    void on_btnAbrirVistaAlumno_clicked();
    void on_btnAbrirVistaCaja_clicked();
    void on_btnAbrirVistaAdmin_clicked();
    void on_btnLogoutDev_clicked();

private:
    Ui::DevWindow *ui;
    void cargarEsquemaCodd();
};

#endif // DEVWINDOW_H
