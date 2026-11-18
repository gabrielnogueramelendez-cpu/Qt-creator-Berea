// =====================================================================
// ARCHIVO: devwindow.cpp (DESARROLLADOR)
// QUÉ HACE: lista los 27 SP (leídos de 05_Backend.sql) y las pruebas de
//           06_Pruebas.sql; permite ejecutar un SP con parámetros y ver
//           lo que devuelve; abre las vistas de alumno, caja y admin.
// SE LLEGA: login de "Desarrollador" (con token).
// BDD:      ejecutar_consulta con el CALL que se arma en pantalla.
// =====================================================================

#include "devwindow.h"
#include "ui_devwindow.h"
#include "mainwindow.h"
#include "cajerowindow.h"
#include "adminwindow.h"
#include "loginwindow.h"
#include "sistemabuffet.h"
#include "basedatos.h"
#include <QMessageBox>
#include <QFile>

// =====================================================================
// Constructor: lee los SPs y las pruebas de los archivos .sql
// =====================================================================
DevWindow::DevWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DevWindow)
{
    ui->setupUi(this);
    setWindowTitle("Desarrollador - El Buffet de Alejandro");

    cargarSPs();
    cargarPruebas();
}

DevWindow::~DevWindow()
{
    delete ui;
}

// =====================================================================
// leerArchivo: devuelve todo el texto de un archivo
// Los .sql están dentro del programa, en recursos.qrc (prefijo /bdd)
// =====================================================================
QString DevWindow::leerArchivo(QString ruta)
{
    QFile archivo(ruta);
    if (!archivo.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, "Error", "No se pudo abrir " + ruta);
        return "";
    }
    QString texto = QString::fromUtf8(archivo.readAll());
    archivo.close();
    return texto.remove('\r');
}

// =====================================================================
// cargarSPs: recorre 05_Backend.sql línea por línea y guarda cada SP
//   "USE ..."              -> el schema de los SPs que siguen
//   "CREATE PROCEDURE ..." -> empieza un SP (el nombre va hasta el paréntesis)
//   "END //"               -> termina el SP
// =====================================================================
void DevWindow::cargarSPs()
{
    QStringList lineas = leerArchivo(":/bdd/05_Backend.sql").split('\n');
    QString schemaActual = "";
    bool dentroDeUnSP = false;
    ProcedimientoSQL nuevo;

    for (int i = 0; i < lineas.size(); i++) {
        QString linea = lineas[i];

        if (linea.startsWith("USE ")) {
            schemaActual = linea.mid(4).remove(';').trimmed();
        }

        if (linea.startsWith("CREATE PROCEDURE ")) {
            dentroDeUnSP = true;
            int posicionParentesis = linea.indexOf('(');
            nuevo.nombre = linea.mid(17, posicionParentesis - 17);   // 17 = largo de "CREATE PROCEDURE "
            nuevo.schema = schemaActual;
            nuevo.codigo = "";
        }

        if (dentroDeUnSP) {
            nuevo.codigo = nuevo.codigo + linea + "\n";
            if (linea.startsWith("END //")) {
                procedimientos.append(nuevo);
                dentroDeUnSP = false;
            }
        }
    }

    for (int i = 0; i < procedimientos.size(); i++) {
        ui->listSP->addItem(procedimientos[i].nombre);
    }
}

// =====================================================================
// cargarPruebas: recorre 06_Pruebas.sql y guarda cada prueba
// Cada prueba empieza con "-- ---------------- Pxx ..." y termina en la
// próxima prueba o en una línea "-- ####"
// =====================================================================
void DevWindow::cargarPruebas()
{
    QStringList lineas = leerArchivo(":/bdd/06_Pruebas.sql").split('\n');
    bool dentroDeUnaPrueba = false;
    PruebaSQL nueva;

    for (int i = 0; i < lineas.size(); i++) {
        QString linea = lineas[i];
        bool empiezaPrueba = linea.startsWith("-- ---------------- P");
        bool terminaSeccion = linea.startsWith("-- ####");

        // Si venía leyendo una prueba y empieza otra (o termina la sección), se guarda
        if (dentroDeUnaPrueba && (empiezaPrueba || terminaSeccion)) {
            nueva.codigo = nueva.codigo.trimmed();
            pruebas.append(nueva);
            dentroDeUnaPrueba = false;
        }

        if (empiezaPrueba) {
            QString titulo = linea;
            nueva.titulo = titulo.remove('-').trimmed();
            nueva.codigo = "";
            dentroDeUnaPrueba = true;
        } else if (dentroDeUnaPrueba) {
            nueva.codigo = nueva.codigo + linea + "\n";
        }
    }

    // La última prueba del archivo
    if (dentroDeUnaPrueba) {
        nueva.codigo = nueva.codigo.trimmed();
        pruebas.append(nueva);
    }

    for (int i = 0; i < pruebas.size(); i++) {
        ui->listPruebas->addItem(pruebas[i].titulo);
    }
}

// =====================================================================
// Al elegir un SP de la lista: muestra su firma y su código
// =====================================================================
void DevWindow::on_listSP_currentRowChanged(int filaElegida)
{
    if (filaElegida < 0) {
        return;
    }

    ProcedimientoSQL elegido = procedimientos[filaElegida];

    // La firma es lo que está antes de BEGIN: nombre y parámetros
    QString firma = elegido.codigo.left(elegido.codigo.indexOf("BEGIN")).mid(17).simplified();

    ui->lblFirmaSP->setText(elegido.schema + "." + firma);
    ui->txtCodigoSP->setPlainText(elegido.codigo);
    ui->txtParametros->clear();
    ui->lblSalidaSP->setText("");
}

// =====================================================================
// Botón EJECUTAR: arma el CALL con los parámetros escritos y muestra
// lo que devuelve (hasta 15 filas), como en los ejemplos con cout
// =====================================================================
void DevWindow::on_btnEjecutarSP_clicked()
{
    int filaElegida = ui->listSP->currentRow();
    if (filaElegida == -1) {
        QMessageBox::warning(this, "Error", "Debe seleccionar un SP de la lista.");
        return;
    }

    ProcedimientoSQL elegido = procedimientos[filaElegida];
    QString llamada = "CALL " + elegido.schema + "." + elegido.nombre + "(" + ui->txtParametros->text().trimmed() + ");";

    MYSQL_RES *resultado = ejecutar_consulta(SistemaBuffet::conexion, llamada.toStdString());
    if (resultado == NULL) {
        ui->lblSalidaSP->setText(llamada + "\nError: " + QString::fromUtf8(mysql_error(SistemaBuffet::conexion)));
        return;
    }

    QString salida = llamada + "\n";
    int cantidadColumnas = mysql_num_fields(resultado);

    // Renglón con los nombres de las columnas, separados por " | "
    for (int columna = 0; columna < cantidadColumnas; columna++) {
        MYSQL_FIELD *datosColumna = mysql_fetch_field_direct(resultado, columna);
        salida = salida + QString::fromUtf8(datosColumna->name);
        if (columna < cantidadColumnas - 1) {
            salida = salida + " | ";
        } else {
            salida = salida + "\n";
        }
    }

    // Un renglón por fila (hasta 15)
    int filasMostradas = 0;
    MYSQL_ROW fila = mysql_fetch_row(resultado);
    while (fila != NULL && filasMostradas < 15) {
        for (int columna = 0; columna < cantidadColumnas; columna++) {
            salida = salida + valor_fila(fila, columna);
            if (columna < cantidadColumnas - 1) {
                salida = salida + " | ";
            } else {
                salida = salida + "\n";
            }
        }
        filasMostradas++;
        fila = mysql_fetch_row(resultado);
    }

    salida = salida + "(" + QString::number(mysql_num_rows(resultado)) + " filas)";
    mysql_free_result(resultado);
    ui->lblSalidaSP->setText(salida);
}

// =====================================================================
// Al elegir una prueba de la lista: muestra su título y su código
// =====================================================================
void DevWindow::on_listPruebas_currentRowChanged(int filaElegida)
{
    if (filaElegida < 0) {
        return;
    }
    ui->lblTituloPrueba->setText(pruebas[filaElegida].titulo);
    ui->txtPrueba->setPlainText(pruebas[filaElegida].codigo);
}

// =====================================================================
// Botones para abrir las otras vistas (sin cerrar esta)
// =====================================================================
void DevWindow::on_btnAbrirVistaAlumno_clicked()
{
    MainWindow *ventanaCatalogo = new MainWindow();
    ventanaCatalogo->show();
}

void DevWindow::on_btnAbrirVistaCaja_clicked()
{
    CajeroWindow *ventanaMostrador = new CajeroWindow();
    ventanaMostrador->show();
}

void DevWindow::on_btnAbrirVistaAdmin_clicked()
{
    AdminWindow *ventanaAdmin = new AdminWindow();
    ventanaAdmin->show();
}

// =====================================================================
// Botón SALIR: vuelve al login
// =====================================================================
void DevWindow::on_btnLogoutDev_clicked()
{
    SistemaBuffet::cerrarSesion();
    LoginWindow *ventanaLogin = new LoginWindow();
    ventanaLogin->show();
    this->close();
}
