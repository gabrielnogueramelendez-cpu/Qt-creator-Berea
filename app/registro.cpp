#include "pantallas.h"
#include "ui_registerdialog.h"
#include "datos.h"
#include "basedatos.h"
#include <QDialog>
#include <QMessageBox>
#include <QIntValidator>

// Diálogo de registro (registerdialog.ui)
static QDialog *dialogo = NULL;
static Ui::RegisterDialog *ui = NULL;

static void registrar()
{
    QString dni = ui->txtRegDni->text().trimmed();
    QString apodo = ui->txtRegApodo->text().trimmed();
    QString clave = ui->txtRegPass->text();

    if (dni.isEmpty() || apodo.isEmpty() || clave.isEmpty()) {
        QMessageBox::warning(dialogo, "Campos Incompletos", "Por favor complete todos los campos del formulario.");
        return;
    }
    if (dni.length() < 7 || dni.length() > 8) {
        QMessageBox::warning(dialogo, "DNI Inválido", "El DNI debe tener entre 7 y 8 dígitos numéricos (sin puntos).");
        return;
    }
    if (clave.length() < 4) {
        QMessageBox::warning(dialogo, "Contraseña Corta", "La contraseña debe tener al menos 4 caracteres.");
        return;
    }

    // El SP valida con la FK (DNI de un estudiante o un profesor) y los UNIQUE (DNI y apodo)
    MYSQL_RES *result = registrar_usuario(conexion, dni, apodo, clave);
    int error = error_del_sp(result);
    QString detalle = "";
    if (error > 0) {
        MYSQL_ROW fila = mysql_fetch_row(result);
        detalle = valor_fila(fila, 2);
    }
    if (result != NULL) {
        mysql_free_result(result);
    }

    if (error == 1452) {
        QMessageBox::warning(dialogo, "DNI Inválido", "Ese DNI no corresponde a un estudiante ni a un profesor de la escuela.");
    } else if (error == 1062 && detalle.contains("Apodo")) {
        QMessageBox::warning(dialogo, "Apodo Ocupado", "Ese apodo ya existe. Elegí otro.");
    } else if (error == 1062) {
        QMessageBox::warning(dialogo, "DNI Registrado", "Ese DNI ya tiene una cuenta. Iniciá sesión con tu apodo.");
    } else if (error != 0) {
        QMessageBox::warning(dialogo, "Error", "No se pudo crear la cuenta: " + detalle);
    } else {
        QMessageBox::information(dialogo, "Registro Exitoso",
            QString("Tu cuenta quedó creada con el apodo %1.\nYa podés iniciar sesión.").arg(apodo));
        dialogo->accept();
    }
}

static void cancelar()
{
    dialogo->reject();
}

void abrir_registro(QWidget *padre)
{
    dialogo = new QDialog(padre);
    ui = new Ui::RegisterDialog();
    ui->setupUi(dialogo);
    dialogo->setWindowTitle("Registro - El Buffet de Alejandro");

    ui->txtRegDni->setValidator(new QIntValidator(0, 99999999, dialogo));
    ui->txtRegDni->setMaxLength(8);
    ui->txtRegApodo->setMaxLength(50);
    ui->txtRegPass->setMaxLength(50);

    QObject::connect(ui->btnRegisterConfirm, &QPushButton::clicked, registrar);
    QObject::connect(ui->btnCancelarRegistro, &QPushButton::clicked, cancelar);

    dialogo->exec();

    delete ui;
    delete dialogo;
    ui = NULL;
    dialogo = NULL;
}
