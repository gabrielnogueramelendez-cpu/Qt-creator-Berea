#include "pantallas.h"
#include "ui_loginwindow.h"
#include "datos.h"
#include "basedatos.h"
#include <QMessageBox>

// Ventana de inicio de sesión (loginwindow.ui)
static QWidget *ventana = NULL;
static Ui::LoginWindow *ui = NULL;
static int intentos_fallidos = 0;

static void liberar()
{
    delete ui;
    ui = NULL;
    ventana = NULL;
}

static void mostrar_campos_del_rol(int rol)
{
    // 0 Estudiante o profesor · 1 Personal de caja · 2 Administración · 3 Desarrollador
    ui->lblPinCajero->setVisible(rol == 1);
    ui->txtPinCajero->setVisible(rol == 1);
    ui->lblTokenDev->setVisible(rol == 3);
    ui->txtTokenDev->setVisible(rol == 3);
    ui->lblNoAccount->setVisible(rol == 0);
    ui->btnGoRegister->setVisible(rol == 0);

    switch (rol) {
    case 0:
        ui->lblUser->setText("APODO");
        ui->txtUser->setPlaceholderText("davidmel09");
        ui->lblNoticeRol->setText("ACCESO DE PRUEBA\nApodo: davidmel09 · Clave: Davidmel#2026");
        break;
    case 1:
        ui->lblUser->setText("APODO DE EMPLEADO");
        ui->txtUser->setPlaceholderText("marcela.ibanez");
        ui->lblNoticeRol->setText("ACCESO DE PRUEBA\nApodo: marcela.ibanez · Clave: MarcelaBuffet#2026 · PIN: 1234");
        break;
    case 2:
        ui->lblUser->setText("APODO DEL ENCARGADO");
        ui->txtUser->setPlaceholderText("alejandro.funes");
        ui->lblNoticeRol->setText("ACCESO DE PRUEBA\nApodo: alejandro.funes · Clave: AlejandroBuffet#2026");
        break;
    case 3:
        ui->lblUser->setText("APODO DE DESARROLLADOR");
        ui->txtUser->setPlaceholderText("soporte.dev");
        ui->lblNoticeRol->setText("ACCESO DE PRUEBA\nApodo: soporte.dev · Clave: SoporteDev#2026 · Token: DEV-BEREA-2026");
        break;
    }
}

static void mostrar_u_ocultar_clave()
{
    if (ui->txtPass->echoMode() == QLineEdit::Password) {
        ui->txtPass->setEchoMode(QLineEdit::Normal);
        ui->btnMostrar->setText("OCULTAR");
    } else {
        ui->txtPass->setEchoMode(QLineEdit::Password);
        ui->btnMostrar->setText("MOSTRAR");
    }
}

static void ir_al_registro()
{
    abrir_registro(ventana);
}

// Llama al SP que corresponde al rol y devuelve el Nombre_Rol del usuario, o "" si los datos no son correctos.
// El SP deja el inicio de sesión (o el intento fallido) en Log_Auditoria
static QString validar_acceso(int rol, const QString &apodo, const QString &clave, const QString &pin, const QString &token)
{
    MYSQL_RES *result = NULL;
    if (rol == 0 || rol == 2) {
        result = iniciar_sesion(conexion, apodo, clave);
    } else if (rol == 1) {
        result = acceso_personal(conexion, apodo, clave, pin);
    } else {
        result = acceso_personal(conexion, apodo, clave, token);
    }
    if (result == NULL) {
        return "";
    }

    int userID = 0;
    if (error_del_sp(result) == 0) {
        MYSQL_ROW fila = mysql_fetch_row(result);
        if (fila != NULL) {
            userID = valor_fila(fila, 0).toInt();
        }
    }
    mysql_free_result(result);

    if (userID == 0 || !cargar_usuario(userID)) {
        return "";
    }
    return usuario_rol;
}

static void iniciar()
{
    QString apodo = ui->txtUser->text().trimmed();
    QString clave = ui->txtPass->text();
    QString pin = ui->txtPinCajero->text().trimmed();
    QString token = ui->txtTokenDev->text().trimmed();
    int rol = ui->comboRole->currentIndex();

    if (intentos_fallidos >= 3) {
        QMessageBox::critical(ventana, "Bloqueado", "Superó el límite de 3 intentos fallidos.");
        return;
    }
    if (apodo.isEmpty() || clave.isEmpty()) {
        intentos_fallidos++;
        QMessageBox::warning(ventana, "Datos Incompletos", "Por favor completa usuario y contraseña.");
        return;
    }
    if (clave.length() < 4) {
        intentos_fallidos++;
        QMessageBox::warning(ventana, "Contraseña Corta", "La contraseña debe tener al menos 4 caracteres.");
        return;
    }
    if (rol == 1 && pin.length() != 4) {
        QMessageBox::warning(ventana, "PIN Inválido", "El PIN debe tener exactamente 4 dígitos.");
        return;
    }
    if (rol == 3 && token.isEmpty()) {
        QMessageBox::warning(ventana, "Token Inválido", "Ingresá el token de desarrollador.");
        return;
    }

    QString rolBDD = validar_acceso(rol, apodo, clave, pin, token);

    // Cada opción del combo solo deja entrar a los roles que le corresponden
    bool correcto = false;
    if (rol == 0) correcto = (rolBDD == "Estudiante" || rolBDD == "Profesor");
    if (rol == 1) correcto = (rolBDD == "Personal de caja");
    if (rol == 2) correcto = (rolBDD == "Administración");
    if (rol == 3) correcto = (rolBDD == "Desarrollador");

    if (!correcto) {
        intentos_fallidos++;
        if (rol == 0) {
            int respuesta = QMessageBox::warning(ventana, "Acceso Denegado",
                "Apodo o contraseña incorrectos.\n¿Deseas registrarte?", QMessageBox::Yes | QMessageBox::No);
            if (respuesta == QMessageBox::Yes) {
                abrir_registro(ventana);
            }
        } else if (rol == 1) {
            QMessageBox::warning(ventana, "Acceso Denegado", "Apodo, contraseña o PIN incorrectos.");
        } else if (rol == 2) {
            QMessageBox::warning(ventana, "Acceso Denegado", "Apodo o contraseña incorrectos, o el usuario no es de administración.");
        } else {
            QMessageBox::warning(ventana, "Acceso Denegado", "Apodo, contraseña o token incorrectos.");
        }
        return;
    }

    intentos_fallidos = 0;
    if (rol == 0) abrir_catalogo();
    if (rol == 1) abrir_caja();
    if (rol == 2) abrir_admin();
    if (rol == 3) abrir_desarrollo();
    ventana->close();
}

void abrir_login()
{
    if (ventana != NULL) {
        ventana->raise();
        return;
    }
    ventana = new QWidget();
    ventana->setAttribute(Qt::WA_DeleteOnClose);
    ui = new Ui::LoginWindow();
    ui->setupUi(ventana);
    ventana->setWindowTitle("El Buffet de Alejandro - Iniciar Sesión");
    QObject::connect(ventana, &QObject::destroyed, liberar);

    // Mismos largos que la BDD: Usuario.Contrasena, Empleado.PIN y Empleado.Token_Acceso
    ui->txtPass->setMaxLength(50);
    ui->txtPinCajero->setMaxLength(4);
    ui->txtTokenDev->setMaxLength(30);

    QObject::connect(ui->btnLogin, &QPushButton::clicked, iniciar);
    QObject::connect(ui->txtUser, &QLineEdit::returnPressed, iniciar);
    QObject::connect(ui->txtPass, &QLineEdit::returnPressed, iniciar);
    QObject::connect(ui->txtPinCajero, &QLineEdit::returnPressed, iniciar);
    QObject::connect(ui->txtTokenDev, &QLineEdit::returnPressed, iniciar);
    QObject::connect(ui->btnMostrar, &QPushButton::clicked, mostrar_u_ocultar_clave);
    QObject::connect(ui->btnGoRegister, &QPushButton::clicked, ir_al_registro);
    QObject::connect(ui->comboRole, &QComboBox::currentIndexChanged, mostrar_campos_del_rol);

    // Foto del panel derecho (si está en el .qrc)
    QPixmap foto(":/imagenes/imagenes/login.jpg");
    if (!foto.isNull()) {
        ui->lblFotoLogin->setPixmap(foto.scaled(ui->lblFotoLogin->size(), Qt::KeepAspectRatio));
    }

    mostrar_campos_del_rol(0);
    ventana->show();
}
