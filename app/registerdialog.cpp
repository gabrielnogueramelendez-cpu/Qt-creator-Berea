#include "registerdialog.h"
#include "ui_registerdialog.h"
#include "sistemabuffet.h"
#include <QMessageBox>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QAction>

RegisterDialog::RegisterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegisterDialog)
{
    ui->setupUi(this);
    setWindowTitle("Registro de Alumno - El Buffet de Alejandro");
    aplicarValidadores();

    // Límite de caracteres para la contraseña (máximo 12)
    ui->txtRegPass->setMaxLength(12);

    // Ojito 👁️ para revelar / ocultar contraseña en el registro
    QAction *eyeAction = ui->txtRegPass->addAction(QString("👁️"), QLineEdit::TrailingPosition);
    eyeAction->setToolTip("Presiona para revelar u ocultar la contraseña");
    connect(eyeAction, &QAction::triggered, this, [this, eyeAction]() {
        if (ui->txtRegPass->echoMode() == QLineEdit::Password) {
            ui->txtRegPass->setEchoMode(QLineEdit::Normal);
            eyeAction->setText("🙈");
        } else {
            ui->txtRegPass->setEchoMode(QLineEdit::Password);
            eyeAction->setText("👁️");
        }
    });
}

RegisterDialog::~RegisterDialog()
{
    delete ui;
}

void RegisterDialog::aplicarValidadores()
{
    // Validador DNI: 7 a 8 dígitos numéricos sin puntos
    QRegularExpression regexDNI("^[0-9]{7,8}$");
    ui->txtRegDni->setValidator(new QRegularExpressionValidator(regexDNI, this));
    ui->txtRegDni->setMaxLength(8);

    // Validador Nombre y Apellido: Letras, espacios y tildes
    QRegularExpression regexTexto("^[a-zA-ZáéíóúÁÉÍÓÚñÑ ]{2,30}$");
    ui->txtRegNombre->setValidator(new QRegularExpressionValidator(regexTexto, this));
    ui->txtRegApellido->setValidator(new QRegularExpressionValidator(regexTexto, this));

    // Validador Correo Institucional @bue.edu.ar
    QRegularExpression regexEmail("^[a-zA-Z0-9._%+-]+@bue\\.edu\\.ar$");
    ui->txtRegEmail->setValidator(new QRegularExpressionValidator(regexEmail, this));
}

void RegisterDialog::on_btnRegisterConfirm_clicked()
{
    QString nombre = ui->txtRegNombre->text().trimmed();
    QString apellido = ui->txtRegApellido->text().trimmed();
    QString dni = ui->txtRegDni->text().trimmed();
    QString curso = ui->txtRegCurso->text().trimmed();
    QString email = ui->txtRegEmail->text().trimmed();
    QString password = ui->txtRegPass->text();

    // [REG-01] Campos obligatorios completos
    if (nombre.isEmpty() || apellido.isEmpty() || dni.isEmpty() || curso.isEmpty() || email.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Campos Incompletos", "Por favor complete todos los campos obligatorios del formulario.");
        return;
    }

    // [REG-01] Validación de longitud y formato de DNI (7 u 8 dígitos exactos)
    if (dni.length() < 7 || dni.length() > 8) {
        QMessageBox::warning(this, "DNI Inválido", "El DNI debe tener entre 7 y 8 dígitos numéricos (sin puntos ni espacios).");
        return;
    }

    // [REG-02] Validación de longitud de nombre y apellido
    if (nombre.length() < 2 || apellido.length() < 2) {
        QMessageBox::warning(this, "Nombre o Apellido Corto", "El nombre y el apellido deben tener al menos 2 caracteres cada uno.");
        return;
    }

    // [REG-04] Validación de curso y división
    if (curso.length() < 2) {
        QMessageBox::warning(this, "Curso Inválido", "Por favor ingrese un curso y división válidos (ej: 4° B o 4to 1ra).");
        return;
    }

    // [REG-03] Validación estricta de correo institucional (@bue.edu.ar)
    QRegularExpression regexEmail("^[a-zA-Z0-9._%+-]+@bue\\.edu\\.ar$");
    if (!regexEmail.match(email).hasMatch()) {
        QMessageBox::critical(this, "Correo Institucional Inválido",
            "El correo ingresado no es válido.\nDebe pertenecer al dominio institucional oficial @bue.edu.ar (ej: alumno@bue.edu.ar).");
        return;
    }

    // [REG-06] Validación de longitud de contraseña (entre 6 y 12 caracteres estrictos)
    if (password.length() < 6 || password.length() > 12) {
        QMessageBox::warning(this, "Longitud de Contraseña Inválida",
            "La contraseña debe tener exactamente entre 6 y 12 caracteres o dígitos.");
        return;
    }

    bool tieneNumero = false;
    for (const QChar &ch : password) {
        if (ch.isDigit()) {
            tieneNumero = true;
            break;
        }
    }
    if (!tieneNumero) {
        QMessageBox::warning(this, "Contraseña Débil", "Por seguridad, la contraseña debe incluir al menos un número.");
        return;
    }

    // [REG-05] Registro en la base del Sistema Central
    QString errorMsg;
    bool registrado = SistemaBuffet::instancia().registrarEstudiante(dni, nombre, apellido, curso, email, password, errorMsg);

    if (!registrado) {
        QMessageBox::critical(this, "Error de Registro", errorMsg);
        return;
    }

    QMessageBox::information(this, "¡Registro Exitoso!",
        QString("¡Bienvenido/a %1!\nTu cuenta de estudiante ha sido registrada correctamente con DNI %2.\nYa puedes iniciar sesión en el buffet con tu DNI y tu contraseña.")
            .arg(nombre, dni));
    accept();
}
