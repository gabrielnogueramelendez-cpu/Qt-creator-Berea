#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>

namespace Ui {
class LoginWindow;
}

// Pantalla de inicio de sesión: estudiante/profesor, caja, administración o desarrollador
class LoginWindow : public QWidget
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();

private slots:
    // Qt conecta estas funciones solas por el nombre: on_<objeto>_<señal>
    void on_btnLogin_clicked();
    void on_btnGoRegister_clicked();
    void on_btnMostrar_clicked();
    void on_comboRole_currentIndexChanged(int opcionElegida);

    // Apretar Enter en cualquier campo es igual que apretar INICIAR SESIÓN
    void on_txtUser_returnPressed();
    void on_txtPass_returnPressed();
    void on_txtPinCajero_returnPressed();
    void on_txtTokenDev_returnPressed();

private:
    Ui::LoginWindow *ui;
    int intentosFallidos;
    void actualizarCamposSegunRol(int opcionElegida);
};

#endif // LOGINWINDOW_H
