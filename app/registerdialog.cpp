// =====================================================================
// ARCHIVO: registerdialog.cpp (ventana CREAR CUENTA)
// QUÉ HACE: crea el usuario de un estudiante o profesor de la escuela.
// SE LLEGA: botón "Crear cuenta" del login.
// BDD:      sp_registrar_usuario. La BDD rechaza un DNI que no es de la
//           escuela (error 1452) o un DNI/apodo repetido (error 1062).
// =====================================================================

#include "registerdialog.h"
#include "ui_registerdialog.h"
#include "sistemabuffet.h"
#include "basedatos.h"
#include <QMessageBox>
#include <QIntValidator>

// =====================================================================
// Constructor: prepara la ventana de registro
// =====================================================================
RegisterDialog::RegisterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegisterDialog)
{
    ui->setupUi(this);
    setWindowTitle("Registro - El Buffet de Alejandro");
    aplicarValidadores();
}

RegisterDialog::~RegisterDialog()
{
    delete ui;
}

// =====================================================================
// aplicarValidadores: límites de lo que se puede escribir en cada campo
// =====================================================================
void RegisterDialog::aplicarValidadores()
{
    // El DNI solo acepta números, hasta 8 cifras (guía Gestor de Inventario)
    ui->txtRegDni->setValidator(new QIntValidator(0, 99999999, this));
    ui->txtRegDni->setMaxLength(8);
    // Mismos largos que la tabla Usuario
    ui->txtRegApodo->setMaxLength(50);
    ui->txtRegPass->setMaxLength(50);
}

// =====================================================================
// Botón CREAR CUENTA
// 1. Controla los campos
// 2. Crea el usuario con sp_registrar_usuario
// Solo se pueden registrar DNIs de estudiantes o profesores de la escuela:
// lo controla la BDD con las claves foráneas de la tabla Usuario
// =====================================================================
void RegisterDialog::on_btnRegisterConfirm_clicked()
{
    QString dni = ui->txtRegDni->text().trimmed();
    QString apodo = ui->txtRegApodo->text().trimmed();
    QString clave = ui->txtRegPass->text();

    // ----- 1. Controles de los campos -----
    if (dni.isEmpty() || apodo.isEmpty() || clave.isEmpty()) {
        QMessageBox::warning(this, "Campos Incompletos", "Por favor complete todos los campos del formulario.");
        return;
    }

    if (dni.length() < 7 || dni.length() > 8) {
        QMessageBox::warning(this, "DNI Inválido", "El DNI debe tener entre 7 y 8 dígitos numéricos (sin puntos).");
        return;
    }

    if (clave.length() < 4) {
        QMessageBox::warning(this, "Contraseña Corta", "La contraseña debe tener al menos 4 caracteres.");
        return;
    }

    // ----- 2. Crear el usuario en la BDD -----
    // El SP decide si es Estudiante o Profesor según en qué tabla esté el DNI.
    // Si algo está mal, devuelve "ERROR", el código de MySQL y la descripción:
    //   1452 = el DNI no es de ningún estudiante ni profesor (clave foránea)
    //   1062 = el DNI o el apodo ya existen (UNIQUE)
    QStringList respuesta = leer_primera_fila(registrar_usuario(SistemaBuffet::conexion, dni, apodo, clave));

    if (respuesta.size() == 0) {
        QMessageBox::warning(this, "Error", "No se pudo crear la cuenta.");
        return;
    }

    if (respuesta[0] == "ERROR") {
        QString codigoError = respuesta[1];
        QString descripcion = respuesta[2];

        if (codigoError == "1452") {
            QMessageBox::warning(this, "DNI Inválido", "Ese DNI no corresponde a un estudiante ni a un profesor de la escuela.");
        } else if (codigoError == "1062" && descripcion.contains("Apodo")) {
            QMessageBox::warning(this, "Apodo Ocupado", "Ese apodo ya existe. Elegí otro.");
        } else if (codigoError == "1062") {
            QMessageBox::warning(this, "DNI Registrado", "Ese DNI ya tiene una cuenta. Iniciá sesión con tu apodo.");
        } else {
            QMessageBox::warning(this, "Error", "No se pudo crear la cuenta: " + descripcion);
        }
        return;
    }

    QMessageBox::information(this, "Registro Exitoso",
        "Tu cuenta quedó creada con el apodo " + apodo + ".\nYa podés iniciar sesión.");
    accept();
}

// =====================================================================
// Botón CANCELAR: cierra sin crear la cuenta
// =====================================================================
void RegisterDialog::on_btnCancelarRegistro_clicked()
{
    reject();
}
