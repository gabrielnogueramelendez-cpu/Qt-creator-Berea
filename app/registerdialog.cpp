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
// 2. Crea el usuario (en la BDD o en las listas)
// Solo se pueden registrar DNIs de estudiantes o profesores de la escuela
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

    // ----- 2. Crear el usuario -----
    if (SistemaBuffet::conexion != NULL) {
        // ----- MODO CON BDD -----
        // Si algo está mal, el SP devuelve "ERROR", el código de MySQL y la descripción:
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
    } else {
        // ----- MODO SIN BDD -----
        // Las mismas reglas que pone la BDD (clave foránea y UNIQUE)
        bool esEstudiante = SistemaBuffet::buscarEstudiante(dni) != -1;
        bool esProfesor = SistemaBuffet::buscarProfesor(dni) != -1;

        if (!esEstudiante && !esProfesor) {
            QMessageBox::warning(this, "DNI Inválido", "Ese DNI no corresponde a un estudiante ni a un profesor de la escuela.");
            return;
        }
        if (SistemaBuffet::buscarDniRegistrado(dni) != -1) {
            QMessageBox::warning(this, "DNI Registrado", "Ese DNI ya tiene una cuenta. Iniciá sesión con tu apodo.");
            return;
        }
        if (SistemaBuffet::buscarUsuario(apodo) != -1) {
            QMessageBox::warning(this, "Apodo Ocupado", "Ese apodo ya existe. Elegí otro.");
            return;
        }

        // Se arma el usuario nuevo y se agrega a la lista
        Usuario nuevo;
        nuevo.dni = dni;
        nuevo.apodo = apodo;
        nuevo.contrasena = clave;
        nuevo.saldo = 0.0;   // igual que en la BDD: Usuario.Saldo arranca en 0
        if (esProfesor) {
            nuevo.rol = "Profesor";
        } else {
            nuevo.rol = "Estudiante";
        }
        SistemaBuffet::usuarios.append(nuevo);
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
