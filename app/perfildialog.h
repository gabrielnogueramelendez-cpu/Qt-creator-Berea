#ifndef PERFILDIALOG_H
#define PERFILDIALOG_H

#include <QDialog>

namespace Ui {
class PerfilDialog;
}

// Perfil del estudiante: datos, saldo, carga de saldo e historial de pedidos
class PerfilDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PerfilDialog(QWidget *parent = nullptr);
    ~PerfilDialog();

private slots:
    void on_btnCargarSaldo_clicked();
    void on_btnCerrar_clicked();

private:
    Ui::PerfilDialog *ui;
    void cargarHistorial();
    void mostrarSaldo();
};

#endif // PERFILDIALOG_H
