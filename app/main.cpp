// main.cpp: Arranque: colores de toda la app, conexión a MySQL y login.

#include "pantallas.h"
#include "datos.h"
#include "basedatos.h"
#include <QApplication>
#include <QMessageBox>

// Sirve para arrancar la app: estilo, conexión a MySQL y login.
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setStyleSheet(
        "QWidget {"
        "    font-family: 'Segoe UI';"
        "    font-size: 13px;"
        "    color: #2B1A1A;"
        "}"
        "QMainWindow, QDialog, #LoginWindow, #CajeroWindow, #AdminWindow, #DevWindow {"
        "    background-color: #FBF6EC;"
        "}"
        "QPushButton {"
        "    background-color: #6B1B2A;"
        "    color: #FFFFFF;"
        "    border: none;"
        "    border-radius: 2px;"
        "    padding: 8px 16px;"
        "    font-weight: bold;"
        "    font-size: 12px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #561421;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #42101A;"
        "}"
        "QPushButton:disabled {"
        "    background-color: #E3D8C8;"
        "    color: #9A8C80;"
        "}"
        "QLineEdit, QComboBox, QSpinBox {"
        "    background-color: #FFFDF8;"
        "    border: 1px solid #D9CBB5;"
        "    border-radius: 2px;"
        "    padding: 7px;"
        "}"
        "QLineEdit:focus, QComboBox:focus, QSpinBox:focus {"
        "    border: 1px solid #6B1B2A;"
        "}"
        "QLineEdit:disabled, QLabel:disabled {"
        "    background-color: #F3EADB;"
        "    color: #B5A898;"
        "}"
        "QGroupBox {"
        "    border: 1px solid #D9CBB5;"
        "    border-radius: 2px;"
        "    margin-top: 14px;"
        "    padding-top: 10px;"
        "}"
        "QGroupBox::title {"
        "    subcontrol-origin: margin;"
        "    left: 10px;"
        "    color: #6B1B2A;"
        "    font-weight: bold;"
        "}"
        "QTableWidget {"
        "    background-color: #FFFDF8;"
        "    border: 1px solid #D9CBB5;"
        "    gridline-color: #EDE3D3;"
        "    selection-background-color: #6B1B2A;"
        "    selection-color: #FFFFFF;"
        "}"
        "QHeaderView::section {"
        "    background-color: #EDE3D3;"
        "    color: #6B1B2A;"
        "    border: none;"
        "    padding: 6px;"
        "    font-weight: bold;"
        "}"
        "QCheckBox, QRadioButton {"
        "    color: #2B1A1A;"
        "}"
    );

    conexion = conectar_bdd();
    if (conexion == NULL) {
        QMessageBox::critical(NULL, "Sin conexión",
            "No se pudo conectar con MySQL.\n"
            "Revisá que el servidor esté corriendo y que se hayan ejecutado los scripts 03, 04 y 05 de la carpeta database.");
        return 1;
    }

    abrir_login();
    int resultado = a.exec();

    mysql_close(conexion);
    return resultado;
}
