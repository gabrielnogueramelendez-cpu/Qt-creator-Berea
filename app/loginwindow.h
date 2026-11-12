#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>

namespace Ui {
class LoginWindow;
}

class LoginWindow : public QWidget
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();

private slots:
    void on_btnLogin_clicked();
    void on_btnGoRegister_clicked();
    void on_comboRole_currentIndexChanged(int index);

private:
    Ui::LoginWindow *ui;
    int m_intentosFallidos;
    void actualizarCamposSegunRol(int index);
};

#endif // LOGINWINDOW_H
