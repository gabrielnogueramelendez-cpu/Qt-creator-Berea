#ifndef ADMINWINDOW_H
#define ADMINWINDOW_H

#include <QWidget>
#include <mysql.h>

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
    void cargarLogsDesdeBDD();
    bool resultadoCorrecto(MYSQL_RES *result);
    Ui::AdminWindow *ui;
    void cargarInventario();
    void cargarLogs();
    void registrarLog(QString evento, QString detalle);
};

#endif // ADMINWINDOW_H
