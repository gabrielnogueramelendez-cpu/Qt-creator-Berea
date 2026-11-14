#include "registerdialog.h"
#include "ui_registerdialog.h"
#include "sistemabuffet.h"
#include <QMessageBox>
#include <QIntValidator>

RegisterDialog::RegisterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegisterDialog)
{
    ui->setupUi(this);
    setWindowTitle("Registro de Alumno - El Buffet de Alejandro");
    aplicarValidadores();
}

RegisterDialog::~RegisterDialog()
{
    delete ui;
}

void RegisterDialog::aplicarValidadores()
{
    // Limitar longitud máxima de campos sin bloquear la escritura
    ui->txtRegDni->setValidator(new QIntValidator(0, 99999999, this));
    ui->txtRegDni->setMaxLength(8);
    // Mismos largos que la tabla Usuario
    ui->txtRegApodo->setMaxLength(50);
    ui->txtRegPass->setMaxLength(50);
}

void RegisterDialog::on_btnRegisterConfirm_clicked()
{
    QString dni = ui->txtRegDni->text().trimmed();
    QString apodo = ui->txtRegApodo->text().trimmed();
    QString password = ui->txtRegPass->text();

    // Validación de campos obligatorios
    if (dni.isEmpty() || apodo.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Campos Incompletos", "Por favor complete todos los campos del formulario.");
        return;
    }

    // Validación de longitud de DNI
    if (dni.length() < 7 || dni.length() > 8) {
        QMessageBox::warning(this, "DNI Inválido", "El DNI debe tener entre 7 y 8 dígitos numéricos (sin puntos).");
        return;
    }

    // Validación de contraseña
    if (password.length() < 4) {
        QMessageBox::warning(this, "Contraseña Corta", "La contraseña debe tener al menos 4 caracteres.");
        return;
    }

    // Las mismas reglas que pone la BDD en sp_registrar_usuario (FK y UNIQUE)
    if (!SistemaBuffet::estudiantesDnis.contains(dni)) {
        QMessageBox::warning(this, "DNI Inválido", "Ese DNI no corresponde a un estudiante de la escuela.");
        return;
    }
    if (SistemaBuffet::usuariosDnis.contains(dni)) {
        QMessageBox::warning(this, "DNI Registrado", "Ese DNI ya tiene una cuenta. Iniciá sesión con tu apodo.");
        return;
    }
    if (SistemaBuffet::usuariosApodos.contains(apodo)) {
        QMessageBox::warning(this, "Apodo Ocupado", "Ese apodo ya existe. Elegí otro.");
        return;
    }

    // Con BDD: CALL sp_registrar_usuario(dni, apodo, password)
    SistemaBuffet::usuariosDnis.append(dni);
    SistemaBuffet::usuariosApodos.append(apodo);
    SistemaBuffet::usuariosContrasenas.append(password);
    SistemaBuffet::usuariosSaldos.append(0.0); // igual que en la BDD: Usuario.Saldo arranca en 0

    QMessageBox::information(this, "Registro Exitoso",
        QString("Tu cuenta quedó creada con el apodo %1.\nYa podés iniciar sesión.").arg(apodo));
    accept();
}

void RegisterDialog::on_btnCancelarRegistro_clicked()
{
    reject();
}
