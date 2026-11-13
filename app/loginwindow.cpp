#include "loginwindow.h"
#include "ui_loginwindow.h"
#include "registerdialog.h"
#include "mainwindow.h"
#include "cajerowindow.h"
#include "adminwindow.h"
#include "devwindow.h"
#include "sistemabuffet.h"
#include <QMessageBox>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QAction>

LoginWindow::LoginWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginWindow),
    m_intentosFallidos(0)
{
    ui->setupUi(this);
    setWindowTitle("El Buffet de Alejandro - Iniciar Sesión");

    // Validador de entrada para usuario (letras, números, guiones, puntos y @)
    QRegularExpression regexUser("^[a-zA-Z0-9._%+-@]{3,50}$");
    ui->txtUser->setValidator(new QRegularExpressionValidator(regexUser, this));

    // Límite de caracteres de contraseña
    ui->txtPass->setMaxLength(12);

    // Validador para PIN de Personal (4 dígitos exactos)
    QRegularExpression regexPin("^[0-9]{4}$");
    ui->txtPinCajero->setValidator(new QRegularExpressionValidator(regexPin, this));
    ui->txtPinCajero->setMaxLength(4);

    // Ojito 👁️ para revelar / ocultar contraseña
    QAction *eyeAction = ui->txtPass->addAction(QString("👁️"), QLineEdit::TrailingPosition);
    eyeAction->setToolTip("Presiona para revelar u ocultar la contraseña");
    connect(eyeAction, &QAction::triggered, this, [this, eyeAction]() {
        if (ui->txtPass->echoMode() == QLineEdit::Password) {
            ui->txtPass->setEchoMode(QLineEdit::Normal);
            eyeAction->setText("🙈");
        } else {
            ui->txtPass->setEchoMode(QLineEdit::Password);
            eyeAction->setText("👁️");
        }
    });

    // Iniciar en modo Alumno
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
    ui->txtUser->clear();
    ui->txtPass->clear();
    ui->txtPinCajero->clear();
    ui->txtTokenDev->clear();

    switch (index) {
    case 0: // Estudiante
        ui->lblUser->setText("DNI o Correo Institucional (@bue.edu.ar):");
        ui->txtUser->setPlaceholderText("Ej. 95671088 o gnoguera@bue.edu.ar");
        ui->lblPinCajero->hide();
        ui->txtPinCajero->hide();
        ui->lblTokenDev->hide();
        ui->txtTokenDev->hide();
        ui->lblNoAccount->show();
        ui->btnGoRegister->show();
        ui->lblNoticeRol->setText("ℹ️ Los alumnos deben iniciar sesión con su DNI o correo oficial @bue.edu.ar.");
        ui->lblNoticeRol->setStyleSheet("color: #5C4A3E; font-size: 11px;");
        break;

    case 1: // Personal de Buffet / Cajero
        ui->lblUser->setText("Legajo de Empleado (ID) o Usuario:");
        ui->txtUser->setPlaceholderText("Ej. EMP-101 o cajero1");
        ui->lblPinCajero->show();
        ui->txtPinCajero->show();
        ui->lblTokenDev->hide();
        ui->txtTokenDev->hide();
        ui->lblNoAccount->hide();
        ui->btnGoRegister->hide();
        ui->lblNoticeRol->setText("🔒 Personal de Buffet: Requiere Legajo/Usuario oficial, contraseña y PIN de turno de 4 dígitos.");
        ui->lblNoticeRol->setStyleSheet("color: #8C6D1F; font-size: 11px; font-weight: bold;");
        break;

    case 2: // Administrador
        ui->lblUser->setText("Usuario Administrador:");
        ui->txtUser->setPlaceholderText("Ej. admin");
        ui->lblPinCajero->hide();
        ui->txtPinCajero->hide();
        ui->lblTokenDev->hide();
        ui->txtTokenDev->hide();
        ui->lblNoAccount->hide();
        ui->btnGoRegister->hide();
        ui->lblNoticeRol->setText("🛡️ Administración General: Acceso a gestión de catálogo, inventario y auditoría.");
        ui->lblNoticeRol->setStyleSheet("color: #3D1A24; font-size: 11px; font-weight: bold;");
        break;

    case 3: // Desarrollador / Soporte Técnico
        ui->lblUser->setText("Usuario Desarrollador / Ing. de Software:");
        ui->txtUser->setPlaceholderText("Ej. dev_berea o dev_root");
        ui->lblPinCajero->hide();
        ui->txtPinCajero->hide();
        ui->lblTokenDev->show();
        ui->txtTokenDev->show();
        ui->lblNoAccount->hide();
        ui->btnGoRegister->hide();
        ui->lblNoticeRol->setText("⚡ Consola de Desarrollador: Requiere contraseña técnica y Token de Autorización (Dev Secret Key).");
        ui->lblNoticeRol->setStyleSheet("color: #1E8449; font-size: 11px; font-weight: bold;");
        break;
    }
}

void LoginWindow::on_btnLogin_clicked()
{
    QString usuario = ui->txtUser->text().trimmed();
    QString password = ui->txtPass->text();
    int indexRol = ui->comboRole->currentIndex();

    // 1. Control de intentos fallidos
    if (m_intentosFallidos >= 3) {
        QMessageBox::critical(this, "Acceso Bloqueado Temporalmente",
            "Has superado el límite de 3 intentos fallidos por seguridad.\nPor favor espera unos instantes.");
        return;
    }

    // 2. Validación de campos básicos
    if (usuario.isEmpty() || password.isEmpty()) {
        m_intentosFallidos++;
        QMessageBox::warning(this, "Datos Incompletos",
            QString("Por favor completa los campos de usuario y contraseña obligatorios.\n(Intentos fallidos: %1/3)").arg(m_intentosFallidos));
        return;
    }

    // 3. Longitud de contraseña
    if (password.length() < 6 || password.length() > 12) {
        m_intentosFallidos++;
        QMessageBox::warning(this, "Contraseña Inválida",
            QString("La contraseña debe tener entre 6 y 12 caracteres.\n(Intentos fallidos: %1/3)").arg(m_intentosFallidos));
        return;
    }

    // 4. Autenticación especializada por Rol
    QString errorMsg;

    if (indexRol == 0) { // Estudiante
        UsuarioSistema alumno;
        if (!SistemaBuffet::instancia().autenticarEstudiante(usuario, password, alumno, errorMsg)) {
            m_intentosFallidos++;
            QMessageBox::StandardButton btn = QMessageBox::warning(this, "Acceso Denegado",
                QString("%1\n\n¿Deseas registrarte como nuevo alumno ahora?").arg(errorMsg),
                QMessageBox::Yes | QMessageBox::No);

            if (btn == QMessageBox::Yes) {
                RegisterDialog regDlg(this);
                regDlg.exec();
            }
            return;
        }

        m_intentosFallidos = 0;
        MainWindow *mainWin = new MainWindow();
        mainWin->show();
        this->close();
    }
    else if (indexRol == 1) { // Personal de Buffet
        QString pin = ui->txtPinCajero->text().trimmed();
        if (pin.length() != 4) {
            m_intentosFallidos++;
            QMessageBox::warning(this, "PIN Inválido", "El PIN de seguridad de turno del personal debe contener exactamente 4 dígitos.");
            return;
        }

        EmpleadoBuffet empleado;
        if (!SistemaBuffet::instancia().autenticarPersonal(usuario, password, pin, empleado, errorMsg)) {
            m_intentosFallidos++;
            QMessageBox::critical(this, "Acceso Denegado - Personal", errorMsg);
            return;
        }

        m_intentosFallidos = 0;
        CajeroWindow *cajeroWin = new CajeroWindow();
        cajeroWin->show();
        this->close();
    }
    else if (indexRol == 2) { // Administrador
        UsuarioSistema admin;
        if (!SistemaBuffet::instancia().autenticarAdmin(usuario, password, admin, errorMsg)) {
            m_intentosFallidos++;
            QMessageBox::critical(this, "Acceso Denegado - Administración", errorMsg);
            return;
        }

        m_intentosFallidos = 0;
        AdminWindow *adminWin = new AdminWindow();
        adminWin->show();
        this->close();
    }
    else if (indexRol == 3) { // Desarrollador
        QString token = ui->txtTokenDev->text().trimmed();
        if (token.isEmpty()) {
            m_intentosFallidos++;
            QMessageBox::warning(this, "Token Requerido", "Debes ingresar el Token de Autorización Técnica (Dev Secret Key).");
            return;
        }

        DesarrolladorSistema dev;
        if (!SistemaBuffet::instancia().autenticarDesarrollador(usuario, password, token, dev, errorMsg)) {
            m_intentosFallidos++;
            QMessageBox::critical(this, "Acceso Denegado - Modo Desarrollador", errorMsg);
            return;
        }

        m_intentosFallidos = 0;
        DevWindow *devWin = new DevWindow();
        devWin->show();
        this->close();
    }
}

void LoginWindow::on_btnGoRegister_clicked()
{
    RegisterDialog regDlg(this);
    regDlg.exec();
}