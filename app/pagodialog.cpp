#include "pagodialog.h"
#include "ui_pagodialog.h"
#include "sistemabuffet.h"
#include "basedatos.h"
#include <QMessageBox>
#include <QIntValidator>
#include <QRandomGenerator>
#include <QTime>

PagoDialog::PagoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PagoDialog),
    m_totalPagar(0.0)
{
    ui->setupUi(this);
    setWindowTitle("Pago - El Buffet de Alejandro");

    // Agrupador lógico de los métodos de pago (guía Configurador de Pedidos)
    grupoMetodo = new QButtonGroup(this);
    grupoMetodo->addButton(ui->rbEfectivo, 1);
    grupoMetodo->addButton(ui->rbMercadoPago, 2);
    grupoMetodo->addButton(ui->rbSaldo, 3);

    // Solo números en monto y comprobante (guía Gestor de Inventario)
    ui->txtMontoAbonado->setValidator(new QIntValidator(0, 999999, this));
    ui->txtComprobanteMP->setValidator(new QIntValidator(0, 9999, this));
    ui->txtComprobanteMP->setMaxLength(4);

    m_totalPagar = SistemaBuffet::getTotalCarrito();
    ui->lblResumenTotal->setText("Total: $" + QString::number(m_totalPagar, 'f', 0));

    cargarDatosAlumno();
    habilitarCamposMetodo();
}

PagoDialog::~PagoDialog()
{
    delete ui;
}

void PagoDialog::cargarDatosAlumno()
{
    ui->txtPagoDni->setText(SistemaBuffet::usuarioDni);
    ui->txtPagoNombre->setText(SistemaBuffet::usuarioNombre + " " + SistemaBuffet::usuarioApellido);
    ui->txtPagoCurso->setText(SistemaBuffet::usuarioCurso);
    ui->lblSaldoDisponible->setText("Saldo disponible: $" + QString::number(SistemaBuffet::usuarioSaldo, 'f', 0));

    if (SistemaBuffet::usuarioSaldo >= m_totalPagar) {
        ui->lblEstadoSaldo->setText(QString("Te quedarán $%1 después de pagar.")
            .arg(QString::number(SistemaBuffet::usuarioSaldo - m_totalPagar, 'f', 0)));
    } else {
        ui->lblEstadoSaldo->setText("El saldo no alcanza para cubrir el total.");
    }
}

void PagoDialog::habilitarCamposMetodo()
{
    // Cada método enciende solo los campos que necesita
    int metodo = grupoMetodo->checkedId();
    ui->txtMontoAbonado->setEnabled(metodo == 1);
    ui->lblVueltoCalculado->setEnabled(metodo == 1);
    ui->txtComprobanteMP->setEnabled(metodo == 2);
    ui->lblSaldoDisponible->setEnabled(metodo == 3);
    ui->lblEstadoSaldo->setEnabled(metodo == 3);
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

void PagoDialog::on_txtMontoAbonado_textChanged(const QString &text)
{
    double abonado = text.toDouble();
    if (text.isEmpty()) {
        ui->lblVueltoCalculado->setText("Vuelto: $0");
    } else if (abonado >= m_totalPagar) {
        ui->lblVueltoCalculado->setText("Vuelto: $" + QString::number(abonado - m_totalPagar, 'f', 0));
    } else {
        ui->lblVueltoCalculado->setText("Faltan $" + QString::number(m_totalPagar - abonado, 'f', 0));
    }
}

void PagoDialog::on_btnCancelarPago_clicked()
{
    reject();
}

void PagoDialog::on_btnConfirmarPedido_clicked()
{
    QString dni = ui->txtPagoDni->text().trimmed();
    QString nombre = ui->txtPagoNombre->text().trimmed();
    QString curso = ui->txtPagoCurso->text().trimmed();

    if (dni.isEmpty() || nombre.isEmpty() || curso.isEmpty()) {
        QMessageBox::warning(this, "Datos Incompletos", "Completá tus datos.");
        return;
    }

    // El primer ítem del combo es el texto "Seleccioná un horario..."
    if (ui->comboRecreo->currentIndex() == 0) {
        QMessageBox::warning(this, "Error", "Debe seleccionar un horario de retiro.");
        return;
    }

    // Se retira de un recreo para el siguiente: el horario elegido tiene que empezar después de la compra.
    // Mismas horas que Franja_Retiro.Hora_Retiro (ID_Franja 1 a 7, en el orden del combo)
    QStringList horasFranja = {"07:45", "09:00", "10:35", "12:05", "14:50", "16:20", "17:50"};
    QTime inicioFranja = QTime::fromString(horasFranja[ui->comboRecreo->currentIndex() - 1], "HH:mm");
    if (inicioFranja <= QTime::currentTime()) {
        QMessageBox::warning(this, "Horario no disponible",
            "Ese horario ya empezó. Lo que comprás ahora se retira en un recreo posterior.");
        return;
    }

    int selectMetodo = grupoMetodo->checkedId();
    if (selectMetodo == -1) {
        QMessageBox::warning(this, "Error", "Debe seleccionar un método de pago.");
        return;
    }

    if (!ui->chkConfirmarTerminos->isChecked()) {
        QMessageBox::warning(this, "Confirmación Requerida",
            "Marcá la casilla para confirmar los datos y el horario de retiro.");
        return;
    }

    QString metodo;
    double montoAbonado = m_totalPagar;
    double vuelto = 0.0;

    switch (selectMetodo) {
    case 1: // Efectivo
        metodo = ui->rbEfectivo->text();
        montoAbonado = ui->txtMontoAbonado->text().toDouble();
        if (ui->txtMontoAbonado->text().isEmpty() || montoAbonado < m_totalPagar) {
            QMessageBox::warning(this, "Monto Insuficiente", "El monto ingresado no alcanza para cubrir el total.");
            return;
        }
        vuelto = montoAbonado - m_totalPagar;
        break;
    case 2: // Mercado Pago
        metodo = ui->rbMercadoPago->text();
        if (ui->txtComprobanteMP->text().length() != 4) {
            QMessageBox::warning(this, "Comprobante Inválido", "Ingresá los 4 dígitos finales del comprobante.");
            return;
        }
        break;
    case 3: // Saldo de cuenta escolar
        metodo = ui->rbSaldo->text();
        if (SistemaBuffet::usuarioSaldo < m_totalPagar) {
            QMessageBox::warning(this, "Saldo Insuficiente", "El saldo de tu cuenta escolar no alcanza.");
            return;
        }
        break;
    }

    QString recreo = ui->comboRecreo->currentText();
    // El orden del combo es el de Franja_Retiro (ID_Franja 1 a 7) y los métodos son los ID 1, 2 y 21 de Metodo_Pago
    int franjaID = ui->comboRecreo->currentIndex();
    int metodoPagoID = 1;
    if (selectMetodo == 2) {
        metodoPagoID = 2;
    } else if (selectMetodo == 3) {
        metodoPagoID = 21;
    }

    // Código de retiro de 5 caracteres al azar (Codigo_Retiro CHAR(5) UNIQUE en la BDD)
    QString caracteres = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    QString codigo;
    do {
        codigo = "";
        for (int i = 0; i < 5; i++)
            codigo += caracteres[QRandomGenerator::global()->bounded(caracteres.length())];
    } while (SistemaBuffet::pedidosCodigos.contains(codigo));

    if (SistemaBuffet::conn != NULL) {
        if (!guardarPedidoEnBDD(metodoPagoID, franjaID, codigo)) {
            return;
        }
        SistemaBuffet::vaciarCarrito();
        SistemaBuffet::cargarUsuarioDesdeBDD(SistemaBuffet::usuarioID); // el SP ya descontó el saldo
        SistemaBuffet::cargarDesdeBDD();
        m_codigoGenerado = codigo;
        accept();
        return;
    }

    // Sin BDD: se descuenta el saldo y se guarda el pedido en las listas
    if (selectMetodo == 3) {
        SistemaBuffet::usuarioSaldo -= m_totalPagar;
    }

    // Registrar pedido en las listas paralelas de SistemaBuffet
    SistemaBuffet::pedidosCodigos.append(codigo);
    SistemaBuffet::pedidosAlumnos.append(nombre);
    SistemaBuffet::pedidosCursos.append(curso);
    SistemaBuffet::pedidosDnis.append(dni);
    SistemaBuffet::pedidosRecreos.append(recreo);
    SistemaBuffet::pedidosMetodos.append(metodo);
    SistemaBuffet::pedidosMontos.append(m_totalPagar);
    SistemaBuffet::pedidosAbonados.append(montoAbonado);
    SistemaBuffet::pedidosVueltos.append(vuelto);
    SistemaBuffet::pedidosEstados.append("PENDIENTE");

    QString itemsTexto = "";
    for (int i = 0; i < SistemaBuffet::carritoNombres.size(); ++i) {
        itemsTexto += QString("%1 x %2 ($%3)\n")
            .arg(SistemaBuffet::carritoCantidades[i])
            .arg(SistemaBuffet::carritoNombres[i])
            .arg(QString::number(SistemaBuffet::carritoPrecios[i] * SistemaBuffet::carritoCantidades[i], 'f', 0));
    }
    SistemaBuffet::pedidosDetalles.append(itemsTexto);

    // Restar del stock lo vendido (con BDD: UPDATE Producto SET Stock_Producto = Stock_Producto - ?)
    for (int i = 0; i < SistemaBuffet::carritoIds.size(); ++i) {
        SistemaBuffet::productosStock[SistemaBuffet::carritoIds[i]] -= SistemaBuffet::carritoCantidades[i];
    }

    // Vaciar el carrito tras confirmar la compra
    SistemaBuffet::vaciarCarrito();

    m_codigoGenerado = codigo;
    accept();
}

bool PagoDialog::guardarPedidoEnBDD(int metodoPagoID, int franjaID, QString &codigo)
{
    // Los 3 pasos van juntos: si uno falla, ROLLBACK deshace la venta y el stock descontado
    mysql_query(SistemaBuffet::conn, "START TRANSACTION;");

    // Paso 1: la venta (sp_registrar_venta devuelve el VentaID)
    MYSQL_RES *result = registrar_venta(SistemaBuffet::conn, SistemaBuffet::usuarioID, metodoPagoID);
    int ventaID = 0;
    if (result != NULL && error_del_sp(result) == 0) {
        MYSQL_ROW fila = mysql_fetch_row(result);
        if (fila != NULL) {
            ventaID = valor_fila(fila, 0).toInt();
        }
    }
    if (result != NULL) {
        mysql_free_result(result);
    }
    if (ventaID == 0) {
        mysql_query(SistemaBuffet::conn, "ROLLBACK;");
        QMessageBox::warning(this, "Error", "No se pudo registrar la venta en la base de datos.");
        return false;
    }

    // Paso 2: cada producto del carrito (el SP controla el stock y lo descuenta). El id 0 es el menú del día
    for (int i = 0; i < SistemaBuffet::carritoIds.size(); ++i) {
        if (SistemaBuffet::carritoIds[i] == 0) {
            result = agregar_menu_carrito(SistemaBuffet::conn, ventaID, SistemaBuffet::menuHoyID, SistemaBuffet::carritoCantidades[i]);
        } else {
            result = agregar_producto_carrito(SistemaBuffet::conn, ventaID, SistemaBuffet::carritoIds[i], SistemaBuffet::carritoCantidades[i]);
        }
        int agregados = 0;
        if (result != NULL && error_del_sp(result) == 0) {
            MYSQL_ROW fila = mysql_fetch_row(result);
            if (fila != NULL) {
                agregados = valor_fila(fila, 0).toInt();
            }
        }
        if (result != NULL) {
            mysql_free_result(result);
        }
        if (agregados == 0) {
            mysql_query(SistemaBuffet::conn, "ROLLBACK;");
            QMessageBox::warning(this, "Sin Stock",
                "No hay stock suficiente de " + SistemaBuffet::carritoNombres[i] + ". Revisá el carrito.");
            return false;
        }
    }

    // Paso 3: el retiro con el código; si el código ya existe (UNIQUE, error 1062) se sortea otro
    QString caracteres = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    for (int intento = 0; intento < 5; ++intento) {
        result = generar_retiro(SistemaBuffet::conn, ventaID, franjaID, codigo);
        int error = error_del_sp(result);
        int creados = 0;
        if (result != NULL && error == 0) {
            MYSQL_ROW fila = mysql_fetch_row(result);
            if (fila != NULL) {
                creados = valor_fila(fila, 0).toInt();
            }
        }
        if (result != NULL) {
            mysql_free_result(result);
        }
        if (creados == 1) {
            mysql_query(SistemaBuffet::conn, "COMMIT;");
            return true;
        }
        if (error != 1062) {
            break;
        }
        codigo = "";
        for (int i = 0; i < 5; i++)
            codigo += caracteres[QRandomGenerator::global()->bounded(caracteres.length())];
    }

    mysql_query(SistemaBuffet::conn, "ROLLBACK;");
    QMessageBox::warning(this, "Error", "No se pudo generar el retiro (revisá el saldo de tu cuenta escolar).");
    return false;
}
