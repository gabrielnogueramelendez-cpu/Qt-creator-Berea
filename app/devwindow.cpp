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

QString DevWindow::leerArchivo(QString ruta)
{
    // Los .sql de la carpeta BDD están en recursos.qrc (prefijo /bdd)
    QFile archivo(ruta);
    if (!archivo.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, "Error", "No se pudo abrir " + ruta);
        return "";
    }
    QString texto = QString::fromUtf8(archivo.readAll());
    archivo.close();
    return texto.remove('\r');
}

void DevWindow::cargarSPs()
{
    QStringList lineas = leerArchivo(":/bdd/05_Backend.sql").split('\n');
    QString schema = "";
    QString codigo = "";
    bool dentroDeUnSP = false;

    for (int i = 0; i < lineas.size(); ++i) {
        QString linea = lineas[i];

        if (linea.startsWith("USE ")) {
            schema = linea.mid(4).remove(';').trimmed();
        }
        if (linea.startsWith("CREATE PROCEDURE ")) {
            dentroDeUnSP = true;
            codigo = "";
            spNombres.append(linea.mid(17, linea.indexOf('(') - 17));
            spSchemas.append(schema);
        }
        if (dentroDeUnSP) {
            codigo += linea + "\n";
            if (linea.startsWith("END //")) {
                spCodigos.append(codigo);
                dentroDeUnSP = false;
            }
        }
    }

    for (int i = 0; i < spNombres.size(); ++i) {
        ui->listSP->addItem(spNombres[i]);
    }
}

void DevWindow::cargarPruebas()
{
    QStringList lineas = leerArchivo(":/bdd/06_Pruebas.sql").split('\n');
    QString codigo = "";
    bool dentroDeUnaPrueba = false;

    for (int i = 0; i < lineas.size(); ++i) {
        QString linea = lineas[i];

        // Cada prueba empieza con "-- ---------------- Pxx ..." y termina en la próxima o en un "-- ####"
        bool empiezaPrueba = linea.startsWith("-- ---------------- P");
        bool terminaSeccion = linea.startsWith("-- ####");

        if ((empiezaPrueba || terminaSeccion) && dentroDeUnaPrueba) {
            pruebasCodigos.append(codigo.trimmed());
            dentroDeUnaPrueba = false;
        }
        if (empiezaPrueba) {
            QString titulo = linea;
            pruebasTitulos.append(titulo.remove('-').trimmed());
            codigo = "";
            dentroDeUnaPrueba = true;
        } else if (dentroDeUnaPrueba) {
            codigo += linea + "\n";
        }
    }
    if (dentroDeUnaPrueba) {
        pruebasCodigos.append(codigo.trimmed());
    }

    for (int i = 0; i < pruebasTitulos.size(); ++i) {
        ui->listPruebas->addItem(pruebasTitulos[i]);
    }
}

void DevWindow::on_listSP_currentRowChanged(int fila)
{
    if (fila < 0) return;

    // La firma es lo que está antes de BEGIN: nombre y parámetros
    QString codigo = spCodigos[fila];
    QString firma = codigo.left(codigo.indexOf("BEGIN")).mid(17).simplified();

    ui->lblFirmaSP->setText(spSchemas[fila] + "." + firma);
    ui->txtCodigoSP->setPlainText(codigo);
    ui->txtParametros->clear();
    ui->lblSalidaSP->setText("");
}

void DevWindow::on_btnEjecutarSP_clicked()
{
    int fila = ui->listSP->currentRow();
    if (fila == -1) {
        QMessageBox::warning(this, "Error", "Debe seleccionar un SP de la lista.");
        return;
    }

    QString llamada = QString("CALL %1.%2(%3);")
        .arg(spSchemas[fila])
        .arg(spNombres[fila])
        .arg(ui->txtParametros->text().trimmed());
    if (SistemaBuffet::conn == NULL) {
        ui->lblSalidaSP->setText(llamada + "\n(sin conexión con MySQL: no se ejecutó)");
        return;
    }

    // Con BDD: se manda el CALL y se muestran las filas que devuelve, como en los ejemplos con cout
    MYSQL_RES *result = ejecutar_consulta(SistemaBuffet::conn, llamada.toStdString());
    if (result == NULL) {
        ui->lblSalidaSP->setText(llamada + "\nError: " + QString::fromUtf8(mysql_error(SistemaBuffet::conn)));
        return;
    }
    QString salida = llamada + "\n";
    int columnas = mysql_num_fields(result);
    for (int c = 0; c < columnas; ++c) {
        salida += QString::fromUtf8(mysql_fetch_field_direct(result, c)->name) + (c < columnas - 1 ? " | " : "\n");
    }
    MYSQL_ROW registro;
    int mostradas = 0;
    while ((registro = mysql_fetch_row(result)) != NULL && mostradas < 15) {
        for (int c = 0; c < columnas; ++c) {
            salida += valor_fila(registro, c) + (c < columnas - 1 ? " | " : "\n");
        }
        mostradas++;
    }
    salida += QString("(%1 filas)").arg(mysql_num_rows(result));
    mysql_free_result(result);
    ui->lblSalidaSP->setText(salida);
}

void DevWindow::on_listPruebas_currentRowChanged(int fila)
{
    if (fila < 0) return;
    ui->lblTituloPrueba->setText(pruebasTitulos[fila]);
    ui->txtPrueba->setPlainText(pruebasCodigos[fila]);
}

void DevWindow::on_btnAbrirVistaAlumno_clicked()
{
    MainWindow *mainWin = new MainWindow();
    mainWin->show();
}

void DevWindow::on_btnAbrirVistaCaja_clicked()
{
    CajeroWindow *cajeroWin = new CajeroWindow();
    cajeroWin->show();
}

void DevWindow::on_btnAbrirVistaAdmin_clicked()
{
    AdminWindow *adminWin = new AdminWindow();
    adminWin->show();
}

void DevWindow::on_btnLogoutDev_clicked()
{
    SistemaBuffet::cerrarSesion();
    LoginWindow *loginWin = new LoginWindow();
    loginWin->show();
    this->close();
}
