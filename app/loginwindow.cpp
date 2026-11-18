// =====================================================================
// ARCHIVO: loginwindow.cpp (pantalla de INICIO DE SESIÓN)
// QUÉ HACE: pide apodo y clave (más PIN para caja o token para desarrollador),
//           los revisa en la BDD y abre la ventana que corresponde al rol.
// SE LLEGA: al abrir el programa, o con el botón SALIR de cualquier ventana.
// BDD:      sp_iniciar_sesion, sp_acceso_personal (los dos dejan el intento
//           en Log_Auditoria) y la consulta obtener_datos_usuario.
// ABRE:     MainWindow (estudiante/profesor), CajeroWindow (caja),
//           AdminWindow (administración), DevWindow (desarrollador),
//           RegisterDialog (crear cuenta).
// =====================================================================

#include "loginwindow.h"
#include "ui_loginwindow.h"
#include "registerdialog.h"
#include "mainwindow.h"
#include "cajerowindow.h"
#include "adminwindow.h"
#include "devwindow.h"
#include "sistemabuffet.h"
#include "basedatos.h"
#include <QMessageBox>

// Opciones del combo "Ingresar como" (comboRole), en el mismo orden que el .ui
const int OPCION_ESTUDIANTE = 0;
const int OPCION_CAJA = 1;
const int OPCION_ADMINISTRACION = 2;
const int OPCION_DESARROLLADOR = 3;

// =====================================================================
// Constructor: prepara la pantalla de login
// =====================================================================
LoginWindow::LoginWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
    setWindowTitle("El Buffet de Alejandro - Iniciar Sesión");

    // Contador para bloquear después de 3 intentos fallidos
    intentosFallidos = 0;

    // Mismos largos que la BDD: Usuario.Contrasena, Empleado.PIN y Empleado.Token_Acceso
    ui->txtPass->setMaxLength(50);
    ui->txtPinCajero->setMaxLength(4);
    ui->txtTokenDev->setMaxLength(30);

    // Foto del panel derecho (login.jpg en recursos.qrc). Sin foto queda el fondo bordó
    QPixmap foto(":/imagenes/imagenes/login.jpg");
    if (!foto.isNull()) {
        foto = foto.scaled(ui->lblFotoLogin->size(), Qt::KeepAspectRatio);
        ui->lblFotoLogin->setPixmap(foto);
    }

    actualizarCamposSegunRol(OPCION_ESTUDIANTE);
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

// =====================================================================
// Enter en cualquier campo = botón INICIAR SESIÓN
// =====================================================================
void LoginWindow::on_txtUser_returnPressed()
{
    on_btnLogin_clicked();
}

void LoginWindow::on_txtPass_returnPressed()
{
    on_btnLogin_clicked();
}

void LoginWindow::on_txtPinCajero_returnPressed()
{
    on_btnLogin_clicked();
}

void LoginWindow::on_txtTokenDev_returnPressed()
{
    on_btnLogin_clicked();
}

// =====================================================================
// Cambio en el combo "Ingresar como"
// =====================================================================
void LoginWindow::on_comboRole_currentIndexChanged(int opcionElegida)
{
    actualizarCamposSegunRol(opcionElegida);
}

// =====================================================================
// actualizarCamposSegunRol: muestra solo los campos que pide cada rol
//   - Caja: además del apodo y la clave, el PIN
//   - Desarrollador: además del apodo y la clave, el token
//   - Solo el estudiante puede ir a "Crear cuenta"
// =====================================================================
void LoginWindow::actualizarCamposSegunRol(int opcionElegida)
{
    // Primero se esconde todo lo que es opcional
    ui->lblPinCajero->hide();
    ui->txtPinCajero->hide();
    ui->lblTokenDev->hide();
    ui->txtTokenDev->hide();
    ui->lblNoAccount->hide();
    ui->btnGoRegister->hide();

    // Después se muestra lo que corresponde a la opción elegida
    if (opcionElegida == OPCION_ESTUDIANTE) {
        ui->lblUser->setText("APODO");
        ui->txtUser->setPlaceholderText("davidmel09");
        ui->lblNoAccount->show();
        ui->btnGoRegister->show();
        ui->lblNoticeRol->setText("ACCESO DE PRUEBA\nApodo: davidmel09 · Clave: Davidmel#2026");
    } else if (opcionElegida == OPCION_CAJA) {
        ui->lblUser->setText("APODO DE EMPLEADO");
        ui->txtUser->setPlaceholderText("marcela.ibanez");
        ui->lblPinCajero->show();
        ui->txtPinCajero->show();
        ui->lblNoticeRol->setText("ACCESO DE PRUEBA\nApodo: marcela.ibanez · Clave: MarcelaBuffet#2026 · PIN: 1234");
    } else if (opcionElegida == OPCION_ADMINISTRACION) {
        ui->lblUser->setText("APODO DEL ENCARGADO");
        ui->txtUser->setPlaceholderText("alejandro.funes");
        ui->lblNoticeRol->setText("ACCESO DE PRUEBA\nApodo: alejandro.funes · Clave: AlejandroBuffet#2026");
    } else if (opcionElegida == OPCION_DESARROLLADOR) {
        ui->lblUser->setText("APODO DE DESARROLLADOR");
        ui->txtUser->setPlaceholderText("soporte.dev");
        ui->lblTokenDev->show();
        ui->txtTokenDev->show();
        ui->lblNoticeRol->setText("ACCESO DE PRUEBA\nApodo: soporte.dev · Clave: SoporteDev#2026 · Token: DEV-BEREA-2026");
    }
}

// =====================================================================
// Botón INICIAR SESIÓN
// 1. Controla que los campos estén bien escritos
// 2. Le pide a la BDD que revise apodo, clave y PIN o token
//    (sp_iniciar_sesion o sp_acceso_personal)
// 3. Si el rol coincide con la opción elegida, abre la ventana que va
// =====================================================================
void LoginWindow::on_btnLogin_clicked()
{
    QString apodo = ui->txtUser->text().trimmed();
    QString clave = ui->txtPass->text();
    QString pin = ui->txtPinCajero->text().trimmed();
    QString token = ui->txtTokenDev->text().trimmed();
    int opcionElegida = ui->comboRole->currentIndex();

    // ----- 1. Controles de los campos -----
    if (intentosFallidos >= 3) {
        QMessageBox::critical(this, "Bloqueado", "Superó el límite de 3 intentos fallidos.");
        return;
    }

    if (apodo.isEmpty() || clave.isEmpty()) {
        intentosFallidos++;
        QMessageBox::warning(this, "Datos Incompletos", "Por favor completa usuario y contraseña.");
        return;
    }

    if (clave.length() < 4) {
        intentosFallidos++;
        QMessageBox::warning(this, "Contraseña Corta", "La contraseña debe tener al menos 4 caracteres.");
        return;
    }

    if (opcionElegida == OPCION_CAJA && pin.length() != 4) {
        QMessageBox::warning(this, "PIN Inválido", "El PIN debe tener exactamente 4 dígitos.");
        return;
    }

    if (opcionElegida == OPCION_DESARROLLADOR && token.isEmpty()) {
        QMessageBox::warning(this, "Token Inválido", "Ingresá el token de desarrollador.");
        return;
    }

    // ----- 2. Revisión del acceso en la BDD -----
    // accesoCorrecto: true si apodo, clave y (si hace falta) PIN o token son correctos
    // rolDelUsuario: el rol que tiene ese usuario en la BDD ("Estudiante", "Personal de caja", ...)
    bool accesoCorrecto = false;
    QString rolDelUsuario = "";

    // El SP revisa los datos contra las tablas Usuario y Empleado, y deja el
    // intento (bien o mal) en Log_Auditoria. Si los datos son correctos,
    // devuelve el UserID en la primera columna
    //   - Caja:          sp_acceso_personal(apodo, clave, PIN)
    //   - Desarrollador: sp_acceso_personal(apodo, clave, token)
    //   - Los demás:     sp_iniciar_sesion(apodo, clave)
    QStringList respuesta;
    if (opcionElegida == OPCION_CAJA) {
        respuesta = leer_primera_fila(acceso_personal(SistemaBuffet::conexion, apodo, clave, pin));
    } else if (opcionElegida == OPCION_DESARROLLADOR) {
        respuesta = leer_primera_fila(acceso_personal(SistemaBuffet::conexion, apodo, clave, token));
    } else {
        respuesta = leer_primera_fila(iniciar_sesion(SistemaBuffet::conexion, apodo, clave));
    }

    // Con el UserID se leen nombre, curso, saldo y rol (SistemaBuffet::usuario)
    if (respuesta.size() > 0 && respuesta[0] != "ERROR") {
        int userID = respuesta[0].toInt();
        accesoCorrecto = SistemaBuffet::cargarUsuarioDesdeBDD(userID);
        rolDelUsuario = SistemaBuffet::usuario.rol;
    }

    // ----- 3. Abrir la ventana que corresponde -----
    // Las ventanas se crean con "new" para que sigan abiertas después de
    // cerrar el login (si se crearan como variable común, se borrarían al
    // terminar esta función)
    if (opcionElegida == OPCION_ESTUDIANTE) {
        if (accesoCorrecto && (rolDelUsuario == "Estudiante" || rolDelUsuario == "Profesor")) {
            intentosFallidos = 0;
            MainWindow *ventanaCatalogo = new MainWindow();
            ventanaCatalogo->show();
            this->close();
        } else {
            intentosFallidos++;
            int respuesta = QMessageBox::warning(this, "Acceso Denegado",
                "Apodo o contraseña incorrectos.\n¿Deseas registrarte?",
                QMessageBox::Yes | QMessageBox::No);
            if (respuesta == QMessageBox::Yes) {
                RegisterDialog ventanaRegistro(this);
                ventanaRegistro.exec();
            }
        }
    } else if (opcionElegida == OPCION_CAJA) {
        if (accesoCorrecto && rolDelUsuario == "Personal de caja") {
            intentosFallidos = 0;
            CajeroWindow *ventanaMostrador = new CajeroWindow();
            ventanaMostrador->show();
            this->close();
        } else {
            intentosFallidos++;
            QMessageBox::warning(this, "Acceso Denegado", "Apodo, contraseña o PIN incorrectos.");
        }
    } else if (opcionElegida == OPCION_ADMINISTRACION) {
        if (accesoCorrecto && rolDelUsuario == "Administración") {
            intentosFallidos = 0;
            AdminWindow *ventanaAdmin = new AdminWindow();
            ventanaAdmin->show();
            this->close();
        } else {
            intentosFallidos++;
            QMessageBox::warning(this, "Acceso Denegado", "Apodo o contraseña incorrectos, o el usuario no es de administración.");
        }
    } else if (opcionElegida == OPCION_DESARROLLADOR) {
        if (accesoCorrecto && rolDelUsuario == "Desarrollador") {
            intentosFallidos = 0;
            DevWindow *ventanaDesarrollador = new DevWindow();
            ventanaDesarrollador->show();
            this->close();
        } else {
            intentosFallidos++;
            QMessageBox::warning(this, "Acceso Denegado", "Apodo, contraseña o token incorrectos.");
        }
    }
}

// =====================================================================
// Botón MOSTRAR / OCULTAR: muestra u oculta la contraseña
// =====================================================================
void LoginWindow::on_btnMostrar_clicked()
{
    if (ui->txtPass->echoMode() == QLineEdit::Password) {
        ui->txtPass->setEchoMode(QLineEdit::Normal);
        ui->btnMostrar->setText("OCULTAR");
    } else {
        ui->txtPass->setEchoMode(QLineEdit::Password);
        ui->btnMostrar->setText("MOSTRAR");
    }
}

// =====================================================================
// Botón "Crear cuenta": abre el registro
// =====================================================================
void LoginWindow::on_btnGoRegister_clicked()
{
    RegisterDialog ventanaRegistro(this);
    ventanaRegistro.exec();
}
