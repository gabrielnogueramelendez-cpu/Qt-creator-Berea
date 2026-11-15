#include "loginwindow.h"
#include "ui_loginwindow.h"
#include "registerdialog.h"
#include "mainwindow.h"
#include "cajerowindow.h"
#include "adminwindow.h"
#include "devwindow.h"
#include "sistemabuffet.h"
#include <QMessageBox>

LoginWindow::LoginWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginWindow),
    m_intentosFallidos(0)
{
    ui->setupUi(this);
    setWindowTitle("El Buffet de Alejandro - Iniciar Sesión");

    // Inicializar datos del sistema
    SistemaBuffet::inicializarDatos();

    // Límites de longitud básicos (sin validadores bloqueantes)
    // Mismos largos que la BDD: Usuario.Contrasena, Empleado.PIN y Empleado.Token_Acceso
    ui->txtPass->setMaxLength(50);
    ui->txtPinCajero->setMaxLength(4);
    ui->txtTokenDev->setMaxLength(30);

    // Conectar tecla Enter (returnPressed) de los campos al botón Iniciar Sesión
    connect(ui->txtUser, &QLineEdit::returnPressed, this, &LoginWindow::on_btnLogin_clicked);
    connect(ui->txtPass, &QLineEdit::returnPressed, this, &LoginWindow::on_btnLogin_clicked);
    connect(ui->txtPinCajero, &QLineEdit::returnPressed, this, &LoginWindow::on_btnLogin_clicked);
    connect(ui->txtTokenDev, &QLineEdit::returnPressed, this, &LoginWindow::on_btnLogin_clicked);

    // Foto del panel derecho (login.jpg en el .qrc); sin foto queda el fondo bordó con la frase
    QPixmap foto(":/imagenes/imagenes/login.jpg");
    if (!foto.isNull()) {
        foto = foto.scaled(ui->lblFotoLogin->size(), Qt::KeepAspectRatio);
        ui->lblFotoLogin->setPixmap(foto);
    }

    actualizarCamposSegunRol(0);
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::on_comboRole_currentIndexChanged(int index)
{
    actualizarCamposSegunRol(index);
}

void LoginWindow::actualizarCamposSegunRol(int index)
{
    switch (index) {
    case 0: // Estudiante
        ui->lblUser->setText("APODO");
        ui->txtUser->setPlaceholderText("jorgedah09");
        ui->lblPinCajero->hide();
        ui->txtPinCajero->hide();
        ui->lblTokenDev->hide();
        ui->txtTokenDev->hide();
        ui->lblNoAccount->show();
        ui->btnGoRegister->show();
        ui->lblNoticeRol->setText("ACCESO DE PRUEBA\nApodo: jorgedah09 · Clave: Jorgedah#2026");
        break;

    case 1: // Personal de Buffet / Cajero
        ui->lblUser->setText("APODO DE EMPLEADO");
        ui->txtUser->setPlaceholderText("marcela.ibanez");
        ui->lblPinCajero->show();
        ui->txtPinCajero->show();
        ui->lblTokenDev->hide();
        ui->txtTokenDev->hide();
        ui->lblNoAccount->hide();
        ui->btnGoRegister->hide();
        ui->lblNoticeRol->setText("ACCESO DE PRUEBA\nApodo: marcela.ibanez · Clave: MarcelaBuffet#2026 · PIN: 1234");
        break;

    case 2: // Administrador
        ui->lblUser->setText("APODO DEL ENCARGADO");
        ui->txtUser->setPlaceholderText("alejandro.funes");
        ui->lblPinCajero->hide();
        ui->txtPinCajero->hide();
        ui->lblTokenDev->hide();
        ui->txtTokenDev->hide();
        ui->lblNoAccount->hide();
        ui->btnGoRegister->hide();
        ui->lblNoticeRol->setText("ACCESO DE PRUEBA\nApodo: alejandro.funes · Clave: AlejandroBuffet#2026");
        break;

    case 3: // Desarrollador / Soporte Técnico
        ui->lblUser->setText("APODO DE DESARROLLADOR");
        ui->txtUser->setPlaceholderText("soporte.dev");
        ui->lblPinCajero->hide();
        ui->txtPinCajero->hide();
        ui->lblTokenDev->show();
        ui->txtTokenDev->show();
        ui->lblNoAccount->hide();
        ui->btnGoRegister->hide();
        ui->lblNoticeRol->setText("ACCESO DE PRUEBA\nApodo: soporte.dev · Clave: SoporteDev#2026 · Token: DEV-BEREA-2026");
        break;
    }
}

void LoginWindow::on_btnLogin_clicked()
{
    QString usuario = ui->txtUser->text().trimmed();
    QString password = ui->txtPass->text();
    int indexRol = ui->comboRole->currentIndex();

    if (m_intentosFallidos >= 3) {
        QMessageBox::critical(this, "Bloqueado", "Superó el límite de 3 intentos fallidos.");
        return;
    }

    if (usuario.isEmpty() || password.isEmpty()) {
        m_intentosFallidos++;
        QMessageBox::warning(this, "Datos Incompletos", "Por favor completa usuario y contraseña.");
        return;
    }

    if (password.length() < 4) {
        m_intentosFallidos++;
        QMessageBox::warning(this, "Contraseña Corta", "La contraseña debe tener al menos 4 caracteres.");
        return;
    }

    // Igual que sp_iniciar_sesion: el apodo y la contraseña tienen que coincidir
    int indiceUsuario = SistemaBuffet::usuariosApodos.indexOf(usuario);
    bool claveCorrecta = indiceUsuario != -1 && SistemaBuffet::usuariosContrasenas[indiceUsuario] == password;
    int indicePersonal = SistemaBuffet::personalApodos.indexOf(usuario);
    // Igual que Usuario.RolID: el rol dice a qué ventana puede entrar
    QString rol = "";
    if (indiceUsuario != -1) {
        rol = SistemaBuffet::usuariosRoles[indiceUsuario];
    }

    if (indexRol == 0) { // Alumno o profesor (Con BDD: CALL sp_iniciar_sesion)
        if (claveCorrecta && (rol == "Estudiante" || rol == "Profesor")) {
            m_intentosFallidos = 0;
            SistemaBuffet::cargarUsuarioActivo(indiceUsuario);
            MainWindow *mainWin = new MainWindow();
            mainWin->show();
            this->close();
        } else {
            m_intentosFallidos++;
            int res = QMessageBox::warning(this, "Acceso Denegado",
                "Apodo o contraseña incorrectos.\n¿Deseas registrarte?",
                QMessageBox::Yes | QMessageBox::No);
            if (res == QMessageBox::Yes) {
                RegisterDialog regDlg(this);
                regDlg.exec();
            }
        }
    }
    else if (indexRol == 1) { // Personal de Buffet (Con BDD: CALL sp_acceso_personal con el PIN)
        QString pin = ui->txtPinCajero->text().trimmed();
        if (pin.length() != 4) {
            QMessageBox::warning(this, "PIN Inválido", "El PIN debe tener exactamente 4 dígitos.");
            return;
        }

        if (claveCorrecta && rol == "Personal de caja" && SistemaBuffet::personalPines[indicePersonal] == pin) {
            m_intentosFallidos = 0;
            CajeroWindow *cajeroWin = new CajeroWindow();
            cajeroWin->show();
            this->close();
        } else {
            m_intentosFallidos++;
            QMessageBox::warning(this, "Acceso Denegado", "Apodo, contraseña o PIN incorrectos.");
        }
    }
    else if (indexRol == 2) { // Administración (Con BDD: sp_iniciar_sesion devuelve el rol)
        if (claveCorrecta && rol == "Administración") {
            m_intentosFallidos = 0;
            AdminWindow *adminWin = new AdminWindow();
            adminWin->show();
            this->close();
        } else {
            m_intentosFallidos++;
            QMessageBox::warning(this, "Acceso Denegado", "Apodo o contraseña incorrectos, o el usuario no es de administración.");
        }
    }
    else if (indexRol == 3) { // Desarrollador (Con BDD: CALL sp_acceso_personal con el token)
        QString token = ui->txtTokenDev->text().trimmed();
        if (token.isEmpty()) {
            QMessageBox::warning(this, "Token Inválido", "Ingresá el token de desarrollador.");
            return;
        }

        if (claveCorrecta && rol == "Desarrollador" && SistemaBuffet::personalTokens[indicePersonal] == token) {
            m_intentosFallidos = 0;
            DevWindow *devWin = new DevWindow();
            devWin->show();
            this->close();
        } else {
            m_intentosFallidos++;
            QMessageBox::warning(this, "Acceso Denegado", "Apodo, contraseña o token incorrectos.");
        }
    }
}

void LoginWindow::on_btnMostrar_clicked()
{
    // Muestra u oculta la contraseña
    if (ui->txtPass->echoMode() == QLineEdit::Password) {
        ui->txtPass->setEchoMode(QLineEdit::Normal);
        ui->btnMostrar->setText("OCULTAR");
    } else {
        ui->txtPass->setEchoMode(QLineEdit::Password);
        ui->btnMostrar->setText("MOSTRAR");
    }
}

void LoginWindow::on_btnGoRegister_clicked()
{
    RegisterDialog regDlg(this);
    regDlg.exec();
}
