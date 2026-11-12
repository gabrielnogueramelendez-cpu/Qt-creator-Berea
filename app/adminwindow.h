#ifndef ADMINWINDOW_H
#define ADMINWINDOW_H

#include <QWidget>

namespace Ui {
class AdminWindow;
}

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
    void cargarInventarioSimulado();
    void cargarLogsSimulados();
};

#endif // ADMINWINDOW_H