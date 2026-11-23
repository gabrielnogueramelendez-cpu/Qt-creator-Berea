// perfil.cpp: Perfil: saldo, carga de saldo e historial de pedidos.  Diseño: perfildialog.ui

#include "pantallas.h"
#include "ui_perfildialog.h"
#include "datos.h"
#include "basedatos.h"
#include <QDialog>
#include <QMessageBox>
#include <QIntValidator>

// ===== BLOQUE 1: VARIABLES DE LA PANTALLA =====

static QDialog *dialogo = NULL;
static Ui::PerfilDialog *ui = NULL;

// ===== BLOQUE 2: FUNCIONES DE LOS BOTONES =====

// Sirve para escribir el saldo actual del usuario.
static void mostrar_saldo()
{
    ui->lblSaldoActual->setText(pesos(usuario_saldo));
}

// Sirve para listar los pedidos del usuario (por su DNI), del más nuevo al más viejo.
static void mostrar_historial()
{
    cargar_pedidos();
    ui->listPedidos->clear();
    for (int i = pedidos_codigos.size() - 1; i >= 0; i--) {
        if (pedidos_dnis[i] != usuario_dni) {
            continue;
        }
        ui->listPedidos->addItem(QString("%1  ·  %2  ·  %3\nRetiro: %4\nPago: %5")
            .arg(pedidos_codigos[i], pedidos_estados[i], pesos(pedidos_montos[i]), pedidos_recreos[i], pedidos_metodos[i]));
    }
    if (ui->listPedidos->count() == 0) {
        ui->listPedidos->addItem("Todavía no hiciste pedidos.");
    }
}

// Sirve para cargar saldo.
static void cargar_saldo_en_cuenta()
{
    QString monto = ui->txtMontoCarga->text();
    if (monto.isEmpty() || monto.toInt() <= 0) {
        QMessageBox::warning(dialogo, "Monto Inválido", "Ingresá el monto que querés cargar.");
        return;
    }
    if (ui->txtComprobanteCarga->text().length() != 4) {
        QMessageBox::warning(dialogo, "Comprobante Inválido", "Ingresá los 4 dígitos finales del comprobante de Mercado Pago.");
        return;
    }

    if (numero_del_sp(cargar_saldo(conexion, usuario_id, monto.toInt())) != 1) {
        QMessageBox::warning(dialogo, "Error", "No se pudo cargar el saldo en la base de datos.");
        return;
    }
    cargar_usuario(usuario_id);

    ui->txtMontoCarga->clear();
    ui->txtComprobanteCarga->clear();
    mostrar_saldo();
    ui->lblEstadoCarga->setText(QString("Se cargaron $%1 en tu cuenta escolar.").arg(monto));
}

// Sirve para el botón CERRAR del perfil.
static void cerrar()
{
    dialogo->accept();
}

// ===== BLOQUE 3: APERTURA DE LA PANTALLA =====

// Sirve para abrir el perfil.
void abrir_perfil(QWidget *padre)
{
    dialogo = new QDialog(padre);
    ui = new Ui::PerfilDialog();
    ui->setupUi(dialogo);
    dialogo->setWindowTitle("Mi perfil - El Buffet de Alejandro");

    ui->lblAvatar->setText(usuario_nombre.left(1) + usuario_apellido.left(1));
    ui->lblNombrePerfil->setText(usuario_nombre + " " + usuario_apellido);
    ui->lblDatosPerfil->setText(QString("DNI %1 · %2").arg(usuario_dni, usuario_curso));
    ui->txtMontoCarga->setValidator(new QIntValidator(1, 50000, dialogo));
    ui->txtComprobanteCarga->setValidator(new QIntValidator(0, 9999, dialogo));
    ui->txtComprobanteCarga->setMaxLength(4);

    QObject::connect(ui->btnCargarSaldo, &QPushButton::clicked, cargar_saldo_en_cuenta);
    QObject::connect(ui->btnCerrar, &QPushButton::clicked, cerrar);

    mostrar_saldo();
    mostrar_historial();
    dialogo->exec();

    delete ui;
    delete dialogo;
    ui = NULL;
    dialogo = NULL;
}
