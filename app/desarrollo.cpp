#include "pantallas.h"
#include "ui_devwindow.h"
#include "datos.h"
#include "basedatos.h"
#include <QMessageBox>
#include <QFile>

// Consola de desarrollo: SPs de 05_Backend, pruebas de 06_PruebasSQL y acceso a las otras vistas (devwindow.ui)
static QWidget *ventana = NULL;
static Ui::DevWindow *ui = NULL;

// Listas paralelas leídas de los .sql que están en recursos.qrc (prefijo /bdd)
static QStringList sp_nombres;
static QStringList sp_schemas;
static QStringList sp_codigos;
static QStringList pruebas_titulos;
static QStringList pruebas_codigos;

static void liberar()
{
    delete ui;
    ui = NULL;
    ventana = NULL;
}

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

static void cargar_pruebas()
{
    pruebas_titulos.clear();
    pruebas_codigos.clear();

    QStringList lineas = leer_archivo(":/bdd/06_Pruebas.sql").split('\n');
    QString codigo = "";
    bool dentro_de_una_prueba = false;

    for (int i = 0; i < lineas.size(); i++) {
        QString linea = lineas[i];
        // Cada prueba empieza con "-- ---------------- Pxx ..." y termina en la próxima o en un "-- ####"
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

static void elegir_sp(int fila)
{
    if (fila < 0) {
        return;
    }
    // La firma es lo que está antes de BEGIN: nombre y parámetros
    QString codigo = sp_codigos[fila];
    QString firma = codigo.left(codigo.indexOf("BEGIN")).mid(17).simplified();
    ui->lblFirmaSP->setText(sp_schemas[fila] + "." + firma);
    ui->txtCodigoSP->setPlainText(codigo);
    ui->txtParametros->clear();
    ui->lblSalidaSP->setText("");
}

static void ejecutar_sp()
{
    int fila = ui->listSP->currentRow();
    if (fila == -1) {
        QMessageBox::warning(ventana, "Error", "Debe seleccionar un SP de la lista.");
        return;
    }

    // Se arma el CALL con los parámetros escritos, como en ejemplobd04
    QString llamada = QString("CALL %1.%2(%3);").arg(sp_schemas[fila], sp_nombres[fila], ui->txtParametros->text().trimmed());
    MYSQL_RES *result = ejecutar_consulta(conexion, llamada.toStdString());
    if (result == NULL) {
        ui->lblSalidaSP->setText(llamada + "\nError: " + QString::fromUtf8(mysql_error(conexion)));
        return;
    }

    // Se recorren las filas con mysql_fetch_row, igual que los ejemplos con cout
    QString salida = llamada + "\n";
    int columnas = mysql_num_fields(result);
    for (int c = 0; c < columnas; c++) {
        salida += QString::fromUtf8(mysql_fetch_field_direct(result, c)->name) + (c < columnas - 1 ? " | " : "\n");
    }
    MYSQL_ROW registro;
    int mostradas = 0;
    while ((registro = mysql_fetch_row(result)) != NULL && mostradas < 15) {
        for (int c = 0; c < columnas; c++) {
            salida += valor_fila(registro, c) + (c < columnas - 1 ? " | " : "\n");
        }
        mostradas++;
    }
    salida += QString("(%1 filas)").arg(mysql_num_rows(result));
    mysql_free_result(result);
    ui->lblSalidaSP->setText(salida);
}

static void elegir_prueba(int fila)
{
    if (fila < 0) {
        return;
    }
    ui->lblTituloPrueba->setText(pruebas_titulos[fila]);
    ui->txtPrueba->setPlainText(pruebas_codigos[fila]);
}

static void cerrar_sesion_desarrollo()
{
    salir_de_la_cuenta();
    abrir_login();
    ventana->close();
}

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
    // Las otras vistas se abren con el usuario del desarrollador (sus datos quedan en las variables globales)
    QObject::connect(ui->btnAbrirVistaAlumno, &QPushButton::clicked, abrir_catalogo);
    QObject::connect(ui->btnAbrirVistaCaja, &QPushButton::clicked, abrir_caja);
    QObject::connect(ui->btnAbrirVistaAdmin, &QPushButton::clicked, abrir_admin);
    QObject::connect(ui->btnLogoutDev, &QPushButton::clicked, cerrar_sesion_desarrollo);

    cargar_sps();
    cargar_pruebas();
    ventana->show();
}
