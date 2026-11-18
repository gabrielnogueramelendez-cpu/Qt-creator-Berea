// =====================================================================
// ARCHIVO: perfildialog.cpp (MI PERFIL)
// QUÉ HACE: datos del usuario, saldo de la cuenta escolar, carga de saldo
//           y el historial de sus pedidos.
// BDD:      sp_cargar_saldo y SistemaBuffet::cargarDesdeBDD() (pedidos).
// =====================================================================

#include "perfildialog.h"
#include "ui_perfildialog.h"
#include "sistemabuffet.h"
#include "basedatos.h"
#include <QMessageBox>
#include <QIntValidator>

// =====================================================================
// Constructor: datos del usuario, saldo e historial de pedidos
// =====================================================================
PerfilDialog::PerfilDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PerfilDialog)
{
    ui->setupUi(this);
    setWindowTitle("Mi perfil - El Buffet de Alejandro");

    UsuarioActivo usuario = SistemaBuffet::usuario;

    // Círculo con las iniciales, nombre completo, DNI y curso
    ui->lblAvatar->setText(usuario.nombre.left(1) + usuario.apellido.left(1));
    ui->lblNombrePerfil->setText(usuario.nombre + " " + usuario.apellido);
    ui->lblDatosPerfil->setText("DNI " + usuario.dni + " · " + usuario.curso);

    // Solo números (guía Gestor de Inventario)
    ui->txtMontoCarga->setValidator(new QIntValidator(1, 50000, this));
    ui->txtComprobanteCarga->setValidator(new QIntValidator(0, 9999, this));
    ui->txtComprobanteCarga->setMaxLength(4);

    mostrarSaldo();
    cargarHistorial();
}

PerfilDialog::~PerfilDialog()
{
    delete ui;
}

// =====================================================================
// mostrarSaldo: saldo actual de la cuenta escolar
// =====================================================================
void PerfilDialog::mostrarSaldo()
{
    ui->lblSaldoActual->setText("$" + QString::number(SistemaBuffet::usuario.saldo, 'f', 0));
}

// =====================================================================
// cargarHistorial: los pedidos de este usuario, del más nuevo al más viejo
// =====================================================================
void PerfilDialog::cargarHistorial()
{
    // Se vuelven a leer los pedidos de la BDD (puede haber uno recién entregado)
    SistemaBuffet::cargarDesdeBDD();
    ui->listPedidos->clear();

    // Se recorre la lista de atrás para adelante para que el último pedido quede arriba
    for (int i = SistemaBuffet::pedidos.size() - 1; i >= 0; i--) {
        Pedido pedido = SistemaBuffet::pedidos[i];

        // Solo los pedidos con el DNI del usuario
        if (pedido.dni == SistemaBuffet::usuario.dni) {
            ui->listPedidos->addItem(pedido.codigo + "  ·  " + pedido.estado + "  ·  $" + QString::number(pedido.monto, 'f', 0)
                                     + "\nRetiro: " + pedido.recreo
                                     + "\nPago: " + pedido.metodo);
        }
    }

    if (ui->listPedidos->count() == 0) {
        ui->listPedidos->addItem("Todavía no hiciste pedidos.");
    }
}

// =====================================================================
// Botón CARGAR SALDO (con un comprobante de Mercado Pago): sp_cargar_saldo
// El SP suma el monto a Usuario.Saldo y devuelve el saldo nuevo
// =====================================================================
void PerfilDialog::on_btnCargarSaldo_clicked()
{
    QString textoMonto = ui->txtMontoCarga->text();
    int monto = textoMonto.toInt();

    if (textoMonto.isEmpty() || monto <= 0) {
        QMessageBox::warning(this, "Monto Inválido", "Ingresá el monto que querés cargar.");
        return;
    }
    if (ui->txtComprobanteCarga->text().length() != 4) {
        QMessageBox::warning(this, "Comprobante Inválido", "Ingresá los 4 dígitos finales del comprobante de Mercado Pago.");
        return;
    }

    // Columnas de la respuesta: 0 filas actualizadas (1 si cargó), 1 saldo nuevo
    QStringList respuesta = leer_primera_fila(cargar_saldo(SistemaBuffet::conexion, SistemaBuffet::usuario.id, monto));

    if (respuesta.size() > 1 && respuesta[0] == "1") {
        SistemaBuffet::usuario.saldo = respuesta[1].toDouble();
    } else {
        QMessageBox::warning(this, "Error", "No se pudo cargar el saldo en la base de datos.");
        return;
    }

    ui->txtMontoCarga->clear();
    ui->txtComprobanteCarga->clear();
    mostrarSaldo();
    ui->lblEstadoCarga->setText("Se cargaron $" + textoMonto + " en tu cuenta escolar.");
}

// =====================================================================
// Botón CERRAR
// =====================================================================
void PerfilDialog::on_btnCerrar_clicked()
{
    accept();
}
