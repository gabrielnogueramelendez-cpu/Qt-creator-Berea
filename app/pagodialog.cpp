// =====================================================================
// ARCHIVO: pagodialog.cpp (PAGO)
// QUÉ HACE: datos del alumno, método de pago (efectivo, Mercado Pago o
//           saldo), recreo de retiro y confirmación del pedido.
// REGLAS:   solo se pide en los horarios de franjaDisponible(); el efectivo
//           tiene que alcanzar; Mercado Pago pide 4 dígitos; el saldo tiene
//           que alcanzar.
// BDD:      una TRANSACCIÓN con sp_registrar_venta + sp_agregar_producto_carrito
//           / sp_agregar_menu_carrito + sp_generar_retiro. Si un paso falla,
//           ROLLBACK deshace todo.
// =====================================================================

#include "pagodialog.h"
#include "ui_pagodialog.h"
#include "sistemabuffet.h"
#include "basedatos.h"
#include <QMessageBox>
#include <QIntValidator>
#include <QTime>
#include <cstdlib>

// =====================================================================
// franjaDisponible: en qué recreo se retira un pedido hecho ahora
// Se pide de un recreo para el siguiente. Devuelve el ID_Franja (1 a 6)
// de la tabla Franja_Retiro, o 0 si ahora no se toman pedidos.
//   06:00 a 07:45 (antes de entrar)  -> 1 = 1er Recreo TM
//   09:00 a 09:15 (1er recreo TM)    -> 2 = 2do Recreo TM
//   10:35 a 10:45 (2do recreo TM)    -> 3 = Almuerzo
//   12:05 a 13:30 (almuerzo)         -> 4 = 1er Recreo TT
//   14:50 a 15:00 (1er recreo TT)    -> 5 = 2do Recreo TT
//   16:20 a 16:30 (2do recreo TT)    -> 6 = Atención post-clases
//   cualquier otra hora (clases)     -> 0 = cerrado
// =====================================================================
static int franjaDisponible()
{
    QTime ahora = QTime::currentTime();

    if (ahora >= QTime(6, 0) && ahora < QTime(7, 45)) {
        return 1;
    } else if (ahora >= QTime(9, 0) && ahora < QTime(9, 15)) {
        return 2;
    } else if (ahora >= QTime(10, 35) && ahora < QTime(10, 45)) {
        return 3;
    } else if (ahora >= QTime(12, 5) && ahora < QTime(13, 30)) {
        return 4;
    } else if (ahora >= QTime(14, 50) && ahora < QTime(15, 0)) {
        return 5;
    } else if (ahora >= QTime(16, 20) && ahora < QTime(16, 30)) {
        return 6;
    } else {
        return 0;
    }
}

// =====================================================================
// generarCodigoRetiro: 5 letras o números al azar (ej. "3C6MB")
// Codigo_Retiro es CHAR(5) UNIQUE en la BDD
// =====================================================================
static QString generarCodigoRetiro()
{
    QString caracteres = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    QString codigo = "";

    for (int i = 0; i < 5; i++) {
        int posicionAlAzar = rand() % caracteres.length();
        codigo = codigo + caracteres[posicionAlAzar];
    }
    return codigo;
}

// =====================================================================
// Constructor: prepara la ventana de pago
// =====================================================================
PagoDialog::PagoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PagoDialog)
{
    ui->setupUi(this);
    setWindowTitle("Pago - El Buffet de Alejandro");

    // Solo números en el monto y en el comprobante (guía Gestor de Inventario)
    ui->txtMontoAbonado->setValidator(new QIntValidator(0, 999999, this));
    ui->txtComprobanteMP->setValidator(new QIntValidator(0, 9999, this));
    ui->txtComprobanteMP->setMaxLength(4);

    totalAPagar = SistemaBuffet::getTotalCarrito();
    montoAbonado = 0;
    vuelto = 0;
    ui->lblResumenTotal->setText("Total: $" + QString::number(totalAPagar, 'f', 0));

    cargarDatosAlumno();
    habilitarCamposMetodo();

    // El horario de retiro no se elige: es siempre el recreo que sigue
    // INTERRUPTOR HORARIO (1 de 2): comentá esta línea para poder elegir el recreo a mano
    ui->comboRecreo->setEnabled(false);
    ui->comboRecreo->setCurrentIndex(franjaDisponible());
}

PagoDialog::~PagoDialog()
{
    delete ui;
}

// =====================================================================
// cargarDatosAlumno: DNI, nombre, curso y saldo del usuario
// =====================================================================
void PagoDialog::cargarDatosAlumno()
{
    UsuarioActivo usuario = SistemaBuffet::usuario;

    ui->txtPagoDni->setText(usuario.dni);
    ui->txtPagoNombre->setText(usuario.nombre + " " + usuario.apellido);
    ui->txtPagoCurso->setText(usuario.curso);
    ui->lblSaldoDisponible->setText("Saldo disponible: $" + QString::number(usuario.saldo, 'f', 0));

    if (usuario.saldo >= totalAPagar) {
        double saldoQueQueda = usuario.saldo - totalAPagar;
        ui->lblEstadoSaldo->setText("Te quedarán $" + QString::number(saldoQueQueda, 'f', 0) + " después de pagar.");
    } else {
        ui->lblEstadoSaldo->setText("El saldo no alcanza para cubrir el total.");
    }
}

// =====================================================================
// habilitarCamposMetodo: cada método enciende solo los campos que usa
//   Efectivo     -> monto con el que paga y vuelto
//   Mercado Pago -> 4 dígitos del comprobante
//   Saldo        -> saldo disponible
// =====================================================================
void PagoDialog::habilitarCamposMetodo()
{
    bool efectivo = ui->rbEfectivo->isChecked();
    bool mercadoPago = ui->rbMercadoPago->isChecked();
    bool saldo = ui->rbSaldo->isChecked();

    ui->txtMontoAbonado->setEnabled(efectivo);
    ui->lblVueltoCalculado->setEnabled(efectivo);
    ui->txtComprobanteMP->setEnabled(mercadoPago);
    ui->lblSaldoDisponible->setEnabled(saldo);
    ui->lblEstadoSaldo->setEnabled(saldo);
}

void PagoDialog::on_rbEfectivo_clicked()
{
    habilitarCamposMetodo();
}

void PagoDialog::on_rbMercadoPago_clicked()
{
    habilitarCamposMetodo();
}

void PagoDialog::on_rbSaldo_clicked()
{
    habilitarCamposMetodo();
}

// =====================================================================
// Mientras escribe el monto en efectivo, calcula el vuelto
// =====================================================================
void PagoDialog::on_txtMontoAbonado_textChanged(const QString &textoMonto)
{
    double montoAbonado = textoMonto.toDouble();

    if (textoMonto.isEmpty()) {
        ui->lblVueltoCalculado->setText("Vuelto: $0");
    } else if (montoAbonado >= totalAPagar) {
        ui->lblVueltoCalculado->setText("Vuelto: $" + QString::number(montoAbonado - totalAPagar, 'f', 0));
    } else {
        ui->lblVueltoCalculado->setText("Faltan $" + QString::number(totalAPagar - montoAbonado, 'f', 0));
    }
}

// =====================================================================
// Botón CANCELAR
// =====================================================================
void PagoDialog::on_btnCancelarPago_clicked()
{
    reject();
}

// =====================================================================
// Botón CONFIRMAR PEDIDO
// 1. Controla los datos, el horario y el método de pago
// 2. Genera el código de retiro
// 3. Guarda el pedido en la BDD (venta + carrito + retiro)
// 4. Vuelve a leer el saldo, el stock y los pedidos
// =====================================================================
void PagoDialog::on_btnConfirmarPedido_clicked()
{
    QString dni = ui->txtPagoDni->text().trimmed();
    QString nombre = ui->txtPagoNombre->text().trimmed();
    QString curso = ui->txtPagoCurso->text().trimmed();

    // ----- 1. Controles -----
    if (dni.isEmpty() || nombre.isEmpty() || curso.isEmpty()) {
        QMessageBox::warning(this, "Datos Incompletos", "Completá tus datos.");
        return;
    }

    // Se vuelve a calcular al confirmar, por si la ventana de pedidos cerró con esta pantalla abierta
    int franjaID = franjaDisponible();
    // INTERRUPTOR HORARIO (2 de 2): comentá la de arriba y descomentá la de abajo
    // int franjaID = ui->comboRecreo->currentIndex();
    if (franjaID == 0) {
        QMessageBox::warning(this, "Horario de retiro",
            "No se puede pedir ahora (horario de clases) o falta elegir un recreo. "
            "Podés pedir de 06:00 a 07:45 para el 1er recreo, o durante cada recreo para el siguiente.");
        return;
    }

    if (!ui->rbEfectivo->isChecked() && !ui->rbMercadoPago->isChecked() && !ui->rbSaldo->isChecked()) {
        QMessageBox::warning(this, "Error", "Debe seleccionar un método de pago.");
        return;
    }

    if (!ui->chkConfirmarTerminos->isChecked()) {
        QMessageBox::warning(this, "Confirmación Requerida",
            "Marcá la casilla para confirmar los datos y el horario de retiro.");
        return;
    }

    // Controles de cada método de pago.
    // metodoPagoID es el ID de la tabla Metodo_Pago: 1 = Efectivo, 2 = Mercado Pago, 21 = Saldo
    int metodoPagoID = 0;

    if (ui->rbEfectivo->isChecked()) {
        // Efectivo: el monto con el que paga tiene que alcanzar
        metodoPagoID = 1;
        montoAbonado = ui->txtMontoAbonado->text().toDouble();
        // INTERRUPTOR EFECTIVO: comentá este if completo (las 4 líneas) para desactivar
        if (ui->txtMontoAbonado->text().isEmpty() || montoAbonado < totalAPagar) {
            QMessageBox::warning(this, "Monto Insuficiente", "El monto ingresado no alcanza para cubrir el total.");
            return;
        }
        vuelto = montoAbonado - totalAPagar;
    } else if (ui->rbMercadoPago->isChecked()) {
        // Mercado Pago: los 4 dígitos finales del comprobante
        metodoPagoID = 2;
        // INTERRUPTOR MERCADO PAGO: comentá este if completo (las 4 líneas) para desactivar
        if (ui->txtComprobanteMP->text().length() != 4) {
            QMessageBox::warning(this, "Comprobante Inválido", "Ingresá los 4 dígitos finales del comprobante.");
            return;
        }
    } else {
        // Saldo de la cuenta escolar: tiene que alcanzar (el SP lo vuelve a controlar)
        metodoPagoID = 21;
        // INTERRUPTOR SALDO: comentá este if completo (las 4 líneas) para desactivar (el SP igual lo controla en la BDD)
        if (SistemaBuffet::usuario.saldo < totalAPagar) {
            QMessageBox::warning(this, "Saldo Insuficiente", "El saldo de tu cuenta escolar no alcanza.");
            return;
        }
    }

    // ----- 2. Código de retiro que no esté repetido -----
    codigoGenerado = generarCodigoRetiro();
    while (SistemaBuffet::buscarPedido(codigoGenerado) != -1) {
        codigoGenerado = generarCodigoRetiro();
    }

    // ----- 3. Guardar el pedido en la BDD -----
    if (!guardarPedidoEnBDD(metodoPagoID, franjaID)) {
        return;
    }

    // ----- 4. Volver a leer lo que cambió -----
    // Los SP ya descontaron el stock y (si pagó con saldo) el saldo
    SistemaBuffet::vaciarCarrito();
    SistemaBuffet::cargarUsuarioDesdeBDD(SistemaBuffet::usuario.id);
    SistemaBuffet::cargarDesdeBDD();

    accept();
}

// =====================================================================
// guardarPedidoEnBDD
// Son 3 pasos que van juntos (una TRANSACCIÓN): si uno falla, ROLLBACK
// deshace todo (la venta y el stock descontado). Si todo sale bien, COMMIT.
// Devuelve true si el pedido quedó guardado.
// =====================================================================
bool PagoDialog::guardarPedidoEnBDD(int metodoPagoID, int franjaID)
{
    MYSQL *conexion = SistemaBuffet::conexion;
    mysql_query(conexion, "START TRANSACTION;");

    // ----- Paso 1: la venta. El SP devuelve el VentaID -----
    QStringList respuesta = leer_primera_fila(registrar_venta(conexion, SistemaBuffet::usuario.id, metodoPagoID));
    int ventaID = 0;
    if (respuesta.size() > 0 && respuesta[0] != "ERROR") {
        ventaID = respuesta[0].toInt();
    }

    if (ventaID == 0) {
        mysql_query(conexion, "ROLLBACK;");
        QMessageBox::warning(this, "Error", "No se pudo registrar la venta en la base de datos.");
        return false;
    }

    // ----- Paso 2: cada producto del carrito. El SP controla el stock y lo descuenta -----
    // Devuelve 1 en la primera columna si lo agregó
    for (int i = 0; i < SistemaBuffet::carrito.size(); i++) {
        ItemCarrito item = SistemaBuffet::carrito[i];

        if (item.id == 0) {
            // El id 0 es el menú del día
            respuesta = leer_primera_fila(agregar_menu_carrito(conexion, ventaID, SistemaBuffet::menuHoy.id, item.cantidad));
        } else {
            respuesta = leer_primera_fila(agregar_producto_carrito(conexion, ventaID, item.id, item.cantidad));
        }

        bool agregado = false;
        if (respuesta.size() > 0 && respuesta[0] != "ERROR" && respuesta[0].toInt() > 0) {
            agregado = true;
        }

        if (!agregado) {
            mysql_query(conexion, "ROLLBACK;");
            QMessageBox::warning(this, "Sin Stock",
                "No hay stock suficiente de " + item.nombre + ". Revisá el carrito.");
            return false;
        }
    }

    // ----- Paso 3: el retiro con el código -----
    // Si el código ya existe en la BDD (error 1062), se prueba con otro (hasta 5 veces)
    for (int intento = 0; intento < 5; intento++) {
        respuesta = leer_primera_fila(generar_retiro(conexion, ventaID, franjaID, codigoGenerado));

        if (respuesta.size() > 0 && respuesta[0] == "1") {
            mysql_query(conexion, "COMMIT;");
            return true;
        }

        bool codigoRepetido = respuesta.size() > 1 && respuesta[0] == "ERROR" && respuesta[1] == "1062";
        if (!codigoRepetido) {
            break;
        }
        codigoGenerado = generarCodigoRetiro();
    }

    mysql_query(conexion, "ROLLBACK;");
    QMessageBox::warning(this, "Error", "No se pudo generar el retiro (revisá el saldo de tu cuenta escolar).");
    return false;
}
