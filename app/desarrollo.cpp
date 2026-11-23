// desarrollo.cpp: Consola del desarrollador: ejecutar SP y ver las pruebas.  Diseño: devwindow.ui

#include "pantallas.h"
#include "ui_devwindow.h"
#include "datos.h"
#include "basedatos.h"
#include <QMessageBox>
#include <QFile>

// ===== BLOQUE 1: VARIABLES DE LA PANTALLA =====

static QWidget *ventana = NULL;
static Ui::DevWindow *ui = NULL;

// Listas paralelas leídas de los .sql que están en recursos.qrc (prefijo /bdd)
static QStringList sp_nombres;
static QStringList sp_schemas;
static QStringList sp_codigos;
static QStringList pruebas_titulos;
static QStringList pruebas_codigos;

// ===== BLOQUE 2: LECTURA DE LOS SCRIPTS 05 Y 06 =====

// Sirve para liberar el .ui cuando la ventana se cierra.
static void liberar()
{
    delete ui;
    ui = NULL;
    ventana = NULL;
}

// Sirve para leer un archivo de texto de los recursos (los .sql del qrc).
static QString leer_archivo(const QString &ruta)
{
    QFile archivo(ruta);
    if (!archivo.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(ventana, "Error", "No se pudo abrir " + ruta);
        return "";
    }
    QString texto = QString::fromUtf8(archivo.readAll());
    archivo.close();
    return texto.remove('\r');
}

// Sirve para recorrer el script 05 y guardar nombre, schema y código de cada SP.
static void cargar_sps()
{
    sp_nombres.clear();
    sp_schemas.clear();
    sp_codigos.clear();

    QStringList lineas = leer_archivo(":/bdd/05_Backend.sql").split('\n');
    QString schema = "";
    QString codigo = "";
    bool dentro_de_un_sp = false;

    for (int i = 0; i < lineas.size(); i++) {
        QString linea = lineas[i];
        if (linea.startsWith("USE ")) {
            schema = linea.mid(4).remove(';').trimmed();
        }
        if (linea.startsWith("CREATE PROCEDURE ")) {
            dentro_de_un_sp = true;
            codigo = "";
            sp_nombres.append(linea.mid(17, linea.indexOf('(') - 17));
            sp_schemas.append(schema);
        }
        if (dentro_de_un_sp) {
            codigo += linea + "\n";
            if (linea.startsWith("END //")) {
                sp_codigos.append(codigo);
                dentro_de_un_sp = false;
            }
        }
    }
    for (int i = 0; i < sp_nombres.size(); i++) {
        ui->listSP->addItem(sp_nombres[i]);
    }
}

// Sirve para recorrer el script 06 y guardar título y código de cada prueba.
static void cargar_pruebas()
{
    pruebas_titulos.clear();
    pruebas_codigos.clear();

    QStringList lineas = leer_archivo(":/bdd/06_Pruebas.sql").split('\n');
    QString codigo = "";
    bool dentro_de_una_prueba = false;

    for (int i = 0; i < lineas.size(); i++) {
        QString linea = lineas[i];
        bool empieza = linea.startsWith("-- ---------------- P");
        bool termina = linea.startsWith("-- ####");

        if ((empieza || termina) && dentro_de_una_prueba) {
            pruebas_codigos.append(codigo.trimmed());
            dentro_de_una_prueba = false;
        }
        if (empieza) {
            QString titulo = linea;
            pruebas_titulos.append(titulo.remove('-').trimmed());
            codigo = "";
            dentro_de_una_prueba = true;
        } else if (dentro_de_una_prueba) {
            codigo += linea + "\n";
        }
    }
    if (dentro_de_una_prueba) {
        pruebas_codigos.append(codigo.trimmed());
    }
    for (int i = 0; i < pruebas_titulos.size(); i++) {
        ui->listPruebas->addItem(pruebas_titulos[i]);
    }
}

// ===== BLOQUE 3: FUNCIONES DE LOS BOTONES Y LISTAS =====

// Sirve para mostrar la firma y el código del SP elegido en la lista.
static void elegir_sp(int fila)
{
    if (fila < 0) {
        return;
    }
    QString codigo = sp_codigos[fila];
    QString firma = codigo.left(codigo.indexOf("BEGIN")).mid(17).simplified();
    ui->lblFirmaSP->setText(sp_schemas[fila] + "." + firma);
    ui->txtCodigoSP->setPlainText(codigo);
    ui->txtParametros->clear();
    ui->lblSalidaSP->setText("");
}

// Sirve para el botón EJECUTAR.
static void ejecutar_sp()
{
    int fila = ui->listSP->currentRow();
    if (fila == -1) {
        QMessageBox::warning(ventana, "Error", "Debe seleccionar un SP de la lista.");
        return;
    }

    QString llamada = QString("CALL %1.%2(%3);").arg(sp_schemas[fila], sp_nombres[fila], ui->txtParametros->text().trimmed());
    MYSQL_RES *result = ejecutar_consulta(conexion, llamada.toStdString());
    if (result == NULL) {
        ui->lblSalidaSP->setText(llamada + "\nError: " + QString::fromUtf8(mysql_error(conexion)));
        return;
    }

    QString salida = llamada + "\n";
    MYSQL_ROW registro = mysql_fetch_row(result);
    int error = error_del_sp(registro);
    if (error != 0) {
        salida += QString("El SP dio el error %1: %2").arg(error).arg(valor_fila(registro, 2));
    } else {
        int filas = 0;
        while (registro != NULL) {
            filas++;
            registro = mysql_fetch_row(result);
        }
        salida += QString("El SP se ejecutó bien y devolvió %1 filas.").arg(filas);
    }
    mysql_free_result(result);
    ui->lblSalidaSP->setText(salida);
}

// Sirve para mostrar el código de la prueba elegida.
static void elegir_prueba(int fila)
{
    if (fila < 0) {
        return;
    }
    ui->lblTituloPrueba->setText(pruebas_titulos[fila]);
    ui->txtPrueba->setPlainText(pruebas_codigos[fila]);
}

// Sirve para el botón SALIR: cierra la sesión y vuelve al login.
static void cerrar_sesion_desarrollo()
{
    salir_de_la_cuenta();
    abrir_login();
    ventana->close();
}

// ===== BLOQUE 4: APERTURA DE LA PANTALLA =====

// Sirve para abrir la consola.
void abrir_desarrollo()
{
    if (ventana != NULL) {
        ventana->raise();
        return;
    }
    ventana = new QWidget();
    ventana->setAttribute(Qt::WA_DeleteOnClose);
    ui = new Ui::DevWindow();
    ui->setupUi(ventana);
    ventana->setWindowTitle("Desarrollador - El Buffet de Alejandro");
    QObject::connect(ventana, &QObject::destroyed, liberar);

    QObject::connect(ui->listSP, &QListWidget::currentRowChanged, elegir_sp);
    QObject::connect(ui->btnEjecutarSP, &QPushButton::clicked, ejecutar_sp);
    QObject::connect(ui->listPruebas, &QListWidget::currentRowChanged, elegir_prueba);
    QObject::connect(ui->btnAbrirVistaAlumno, &QPushButton::clicked, abrir_catalogo);
    QObject::connect(ui->btnAbrirVistaCaja, &QPushButton::clicked, abrir_caja);
    QObject::connect(ui->btnAbrirVistaAdmin, &QPushButton::clicked, abrir_admin);
    QObject::connect(ui->btnLogoutDev, &QPushButton::clicked, cerrar_sesion_desarrollo);

    cargar_sps();
    cargar_pruebas();
    ventana->show();
}
