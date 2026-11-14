#include "perfildialog.h"
#include "ui_perfildialog.h"
#include "sistemabuffet.h"
#include <QMessageBox>
#include <QIntValidator>

PerfilDialog::PerfilDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PerfilDialog)
{
    ui->setupUi(this);
    setWindowTitle("Mi perfil - El Buffet de Alejandro");

    // Círculo con las iniciales del estudiante
    ui->lblAvatar->setText(SistemaBuffet::usuarioNombre.left(1) + SistemaBuffet::usuarioApellido.left(1));
    ui->lblNombrePerfil->setText(SistemaBuffet::usuarioNombre + " " + SistemaBuffet::usuarioApellido);
    ui->lblDatosPerfil->setText(QString("DNI %1 · %2")
        .arg(SistemaBuffet::usuarioDni)
        .arg(SistemaBuffet::usuarioCurso));

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

void PerfilDialog::mostrarSaldo()
{
    ui->lblSaldoActual->setText("$" + QString::number(SistemaBuffet::usuarioSaldo, 'f', 0));
}

void PerfilDialog::cargarHistorial()
{
    // Con BDD: SELECT de Venta + Retiro filtrando por el UserID del estudiante
    ui->listPedidos->clear();
    for (int i = SistemaBuffet::pedidosCodigos.size() - 1; i >= 0; --i) {
        if (SistemaBuffet::pedidosDnis[i] != SistemaBuffet::usuarioDni) {
            continue;
        }
        ui->listPedidos->addItem(QString("%1  ·  %2  ·  $%3\nRetiro: %4\nPago: %5")
            .arg(SistemaBuffet::pedidosCodigos[i])
            .arg(SistemaBuffet::pedidosEstados[i])
            .arg(QString::number(SistemaBuffet::pedidosMontos[i], 'f', 0))
            .arg(SistemaBuffet::pedidosRecreos[i])
            .arg(SistemaBuffet::pedidosMetodos[i]));
    }

    if (ui->listPedidos->count() == 0) {
        ui->listPedidos->addItem("Todavía no hiciste pedidos.");
    }
}

void PerfilDialog::on_btnCargarSaldo_clicked()
{
    QString textoMonto = ui->txtMontoCarga->text();

    if (textoMonto.isEmpty() || textoMonto.toInt() <= 0) {
        QMessageBox::warning(this, "Monto Inválido", "Ingresá el monto que querés cargar.");
        return;
    }
    if (ui->txtComprobanteCarga->text().length() != 4) {
        QMessageBox::warning(this, "Comprobante Inválido", "Ingresá los 4 dígitos finales del comprobante de Mercado Pago.");
        return;
    }

    // Con BDD: CALL sp_cargar_saldo(UserID, monto)
    SistemaBuffet::usuarioSaldo += textoMonto.toInt();

    ui->txtMontoCarga->clear();
    ui->txtComprobanteCarga->clear();
    mostrarSaldo();
    ui->lblEstadoCarga->setText(QString("Se cargaron $%1 en tu cuenta escolar.").arg(textoMonto));
}

void PerfilDialog::on_btnCerrar_clicked()
{
    accept();
}
