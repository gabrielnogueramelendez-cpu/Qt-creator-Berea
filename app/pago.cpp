#include "pantallas.h"
#include "ui_pagodialog.h"
#include "datos.h"
#include "basedatos.h"
#include <QDialog>
#include <QMessageBox>
#include <QIntValidator>
#include <QRandomGenerator>
#include <QTime>

// Pago y confirmación del pedido (pagodialog.ui)
static QDialog *dialogo = NULL;
static Ui::PagoDialog *ui = NULL;
static double total_a_pagar = 0;
static QString codigo_generado = "";
static double monto_abonado = 0;
static double monto_vuelto = 0;

// Se pide de un recreo para el siguiente. Ventana de pedido de cada franja (ID_Franja 1 a 6 de Franja_Retiro):
// el 1er recreo TM se pide antes de entrar (06:00 a 07:45) y los demás durante el recreo anterior.
// En horario de clases no se toman pedidos (devuelve 0)
static int franja_disponible()
{
    const char *ventanas[6][2] = {{"06:00", "07:45"}, {"09:00", "09:15"}, {"10:35", "10:45"},
                                  {"12:05", "13:30"}, {"14:50", "15:00"}, {"16:20", "16:30"}};
    QTime ahora = QTime::currentTime();
    for (int i = 0; i < 6; i++) {
        if (ahora >= QTime::fromString(ventanas[i][0], "HH:mm") && ahora < QTime::fromString(ventanas[i][1], "HH:mm")) {
            return i + 1;
        }
    }
    return 0;
}

// 1 Efectivo · 2 Mercado Pago · 3 Saldo de cuenta escolar · -1 ninguno
static int metodo_elegido()
{
    if (ui->rbEfectivo->isChecked()) return 1;
    if (ui->rbMercadoPago->isChecked()) return 2;
    if (ui->rbSaldo->isChecked()) return 3;
    return -1;
}

static QString codigo_al_azar()
{
    // Código de retiro de 5 caracteres (Codigo_Retiro CHAR(5) UNIQUE)
    QString caracteres = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    QString codigo = "";
    for (int i = 0; i < 5; i++) {
        codigo += caracteres[QRandomGenerator::global()->bounded(caracteres.length())];
    }
    return codigo;
}

static void habilitar_campos_del_metodo()
{
    int metodo = metodo_elegido();
    ui->txtMontoAbonado->setEnabled(metodo == 1);
    ui->lblVueltoCalculado->setEnabled(metodo == 1);
    ui->txtComprobanteMP->setEnabled(metodo == 2);
    ui->lblSaldoDisponible->setEnabled(metodo == 3);
    ui->lblEstadoSaldo->setEnabled(metodo == 3);
}

static void calcular_vuelto(const QString &texto)
{
    double abonado = texto.toDouble();
    if (texto.isEmpty()) {
        ui->lblVueltoCalculado->setText("Vuelto: $0");
    } else if (abonado >= total_a_pagar) {
        ui->lblVueltoCalculado->setText("Vuelto: " + pesos(abonado - total_a_pagar));
    } else {
        ui->lblVueltoCalculado->setText("Faltan " + pesos(total_a_pagar - abonado));
    }
}

static void cancelar()
{
    dialogo->reject();
}

// Guarda el pedido con los SP en 3 pasos. Van juntos en una transacción: si uno falla, ROLLBACK
static bool guardar_pedido(int metodoPagoID, int franjaID)
{
    mysql_query(conexion, "START TRANSACTION;");

    // Paso 1: la venta (sp_registrar_venta devuelve el VentaID)
    int ventaID = numero_del_sp(registrar_venta(conexion, usuario_id, metodoPagoID));
    if (ventaID == 0) {
        mysql_query(conexion, "ROLLBACK;");
        QMessageBox::warning(dialogo, "Error", "No se pudo registrar la venta en la base de datos.");
        return false;
    }

    // Paso 2: cada renglón del carrito (el SP controla y descuenta el stock). El id 0 es el menú del día
    for (int i = 0; i < carrito_ids.size(); i++) {
        int agregados = 0;
        if (carrito_ids[i] == 0) {
            agregados = numero_del_sp(agregar_menu_carrito(conexion, ventaID, menu_hoy_id, carrito_cantidades[i]));
        } else {
            agregados = numero_del_sp(agregar_producto_carrito(conexion, ventaID, carrito_ids[i], carrito_cantidades[i]));
        }
        if (agregados == 0) {
            mysql_query(conexion, "ROLLBACK;");
            QMessageBox::warning(dialogo, "Sin Stock", "No hay stock suficiente de " + carrito_nombres[i] + ". Revisá el carrito.");
            return false;
        }
    }

    // Paso 3: el retiro con su código. Si el código ya existe (UNIQUE, error 1062) se sortea otro
    for (int intento = 0; intento < 5; intento++) {
        MYSQL_RES *result = generar_retiro(conexion, ventaID, franjaID, codigo_generado);
        int error = error_del_sp(result);
        int creados = numero_del_sp(result);
        if (creados == 1) {
            mysql_query(conexion, "COMMIT;");
            return true;
        }
        if (error != 1062) {
            break;
        }
        codigo_generado = codigo_al_azar();
    }

    mysql_query(conexion, "ROLLBACK;");
    QMessageBox::warning(dialogo, "Error", "No se pudo generar el retiro (revisá el saldo de tu cuenta escolar).");
    return false;
}

static void confirmar()
{
    if (ui->txtPagoDni->text().trimmed().isEmpty() || ui->txtPagoNombre->text().trimmed().isEmpty()
        || ui->txtPagoCurso->text().trimmed().isEmpty()) {
        QMessageBox::warning(dialogo, "Datos Incompletos", "Completá tus datos.");
        return;
    }

    // Se vuelve a calcular al confirmar, por si cambió la franja con el diálogo abierto
    int franja = franja_disponible();
    ui->comboRecreo->setCurrentIndex(franja);
    if (franja == 0) {
        QMessageBox::warning(dialogo, "Horario de clases",
            "Ahora no se toman pedidos. Podés pedir de 06:00 a 07:45 para el 1er recreo, "
            "o durante cada recreo para el siguiente.");
        return;
    }

    int metodo = metodo_elegido();
    if (metodo == -1) {
        QMessageBox::warning(dialogo, "Error", "Debe seleccionar un método de pago.");
        return;
    }
    if (!ui->chkConfirmarTerminos->isChecked()) {
        QMessageBox::warning(dialogo, "Confirmación Requerida", "Marcá la casilla para confirmar los datos y el horario de retiro.");
        return;
    }

    // Los métodos son los ID 1, 2 y 21 de Metodo_Pago
    int metodoPagoID = 1;
    monto_abonado = total_a_pagar;
    monto_vuelto = 0;

    switch (metodo) {
    case 1:
        monto_abonado = ui->txtMontoAbonado->text().toDouble();
        if (ui->txtMontoAbonado->text().isEmpty() || monto_abonado < total_a_pagar) {
            QMessageBox::warning(dialogo, "Monto Insuficiente", "El monto ingresado no alcanza para cubrir el total.");
            return;
        }
        monto_vuelto = monto_abonado - total_a_pagar;
        metodoPagoID = 1;
        break;
    case 2:
        if (ui->txtComprobanteMP->text().length() != 4) {
            QMessageBox::warning(dialogo, "Comprobante Inválido", "Ingresá los 4 dígitos finales del comprobante.");
            return;
        }
        metodoPagoID = 2;
        break;
    case 3:
        if (usuario_saldo < total_a_pagar) {
            QMessageBox::warning(dialogo, "Saldo Insuficiente", "El saldo de tu cuenta escolar no alcanza.");
            return;
        }
        metodoPagoID = 21;
        break;
    }

    codigo_generado = codigo_al_azar();
    if (!guardar_pedido(metodoPagoID, franja)) {
        return;
    }
    vaciar_carrito();
    cargar_usuario(usuario_id); // el SP ya descontó el saldo
    dialogo->accept();
}

bool abrir_pago(QWidget *padre, QString &codigo, double &abonado, double &vuelto)
{
    total_a_pagar = total_carrito();
    codigo_generado = "";

    dialogo = new QDialog(padre);
    ui = new Ui::PagoDialog();
    ui->setupUi(dialogo);
    dialogo->setWindowTitle("Pago - El Buffet de Alejandro");

    ui->txtMontoAbonado->setValidator(new QIntValidator(0, 999999, dialogo));
    ui->txtComprobanteMP->setValidator(new QIntValidator(0, 9999, dialogo));
    ui->txtComprobanteMP->setMaxLength(4);
    ui->lblResumenTotal->setText("Total: " + pesos(total_a_pagar));

    // Datos del usuario que inició sesión
    ui->txtPagoDni->setText(usuario_dni);
    ui->txtPagoNombre->setText(usuario_nombre + " " + usuario_apellido);
    ui->txtPagoCurso->setText(usuario_curso);
    ui->lblSaldoDisponible->setText("Saldo disponible: " + pesos(usuario_saldo));
    if (usuario_saldo >= total_a_pagar) {
        ui->lblEstadoSaldo->setText("Te quedarán " + pesos(usuario_saldo - total_a_pagar) + " después de pagar.");
    } else {
        ui->lblEstadoSaldo->setText("El saldo no alcanza para cubrir el total.");
    }

    // El horario de retiro no se elige: es siempre el recreo que sigue
    ui->comboRecreo->setEnabled(false);
    ui->comboRecreo->setCurrentIndex(franja_disponible());

    QObject::connect(ui->rbEfectivo, &QRadioButton::clicked, habilitar_campos_del_metodo);
    QObject::connect(ui->rbMercadoPago, &QRadioButton::clicked, habilitar_campos_del_metodo);
    QObject::connect(ui->rbSaldo, &QRadioButton::clicked, habilitar_campos_del_metodo);
    QObject::connect(ui->txtMontoAbonado, &QLineEdit::textChanged, calcular_vuelto);
    QObject::connect(ui->btnCancelarPago, &QPushButton::clicked, cancelar);
    QObject::connect(ui->btnConfirmarPedido, &QPushButton::clicked, confirmar);
    habilitar_campos_del_metodo();

    bool pagado = (dialogo->exec() == QDialog::Accepted);

    delete ui;
    delete dialogo;
    ui = NULL;
    dialogo = NULL;

    codigo = codigo_generado;
    abonado = monto_abonado;
    vuelto = monto_vuelto;
    return pagado;
}
