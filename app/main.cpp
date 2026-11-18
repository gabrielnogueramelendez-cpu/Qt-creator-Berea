#include "loginwindow.h"
#include "sistemabuffet.h"
#include "basedatos.h"
#include <QApplication>
#include <QMessageBox>
#include <cstdlib>
#include <ctime>

// =====================================================================
// main: punto de entrada del programa
// 1. Crea la aplicación y le pone los colores (crema y bordó)
// 2. Se conecta a MySQL
// 3. Abre la ventana de login
// =====================================================================
int main(int argc, char *argv[])
{
    QApplication aplicacion(argc, argv);

    // Semilla para rand(): así los códigos de retiro salen distintos cada vez
    srand(time(NULL));

    // ----- 1. Estilo de todas las ventanas (Qt Style Sheets, se escribe como CSS) -----
    aplicacion.setStyleSheet(
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

    // ----- 2. Conexión a MySQL -----
    // Si falla, el programa sigue igual con los datos de ejemplo (modo sin BDD)
    SistemaBuffet::conexion = conectar_bdd();
    if (SistemaBuffet::conexion == NULL) {
        QMessageBox::warning(NULL, "Sin conexión",
            "No se pudo conectar con MySQL.\nLa aplicación va a usar los datos de ejemplo, sin guardar cambios en la BDD.");
    }

    // ----- 3. Ventana de login -----
    LoginWindow ventanaLogin;
    ventanaLogin.show();

    // exec() deja el programa funcionando hasta que se cierra la última ventana
    int codigoSalida = aplicacion.exec();

    // Al salir se cierra la conexión con MySQL
    if (SistemaBuffet::conexion != NULL) {
        mysql_close(SistemaBuffet::conexion);
    }
    return codigoSalida;
}
