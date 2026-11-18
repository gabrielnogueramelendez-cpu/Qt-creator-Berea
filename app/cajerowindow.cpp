#include "cajerowindow.h"
#include "ui_cajerowindow.h"
#include "loginwindow.h"
#include "sistemabuffet.h"
#include "basedatos.h"
#include <QMessageBox>
#include <QTableWidgetItem>

// IDs de la tabla Estado_Retiro
const int ESTADO_ENTREGADO = 2;
const int ESTADO_CANCELADO = 3;

// =====================================================================
// Constructor: prepara el mostrador y carga la fila de pedidos
// =====================================================================
CajeroWindow::CajeroWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CajeroWindow)
{
    ui->setupUi(this);
    setWindowTitle("Mostrador - El Buffet de Alejandro");

    // Hasta que no se busca un pedido, no se puede entregar ni cancelar
    ui->btnEntregarPedido->setEnabled(false);
    ui->btnCancelarPedido->setEnabled(false);

    // Codigo_Retiro es CHAR(5) en la BDD
    ui->txtCodigoRetiro->setMaxLength(5);

    // En la tabla se elige una fila entera por vez
    ui->tableColasPedidos->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableColasPedidos->setSelectionMode(QAbstractItemView::SingleSelection);

    cargarPedidosDelSistema();
}

CajeroWindow::~CajeroWindow()
{
    delete ui;
}

// =====================================================================
// cargarPedidosDelSistema: llena la tabla con todos los pedidos
// =====================================================================
void CajeroWindow::cargarPedidosDelSistema()
{
    // Con BDD: los pedidos se vuelven a leer de Retiro + Venta + Carrito
    SistemaBuffet::cargarDesdeBDD();

    ui->tableColasPedidos->setRowCount(0);
    ui->tableColasPedidos->setColumnCount(5);
    QStringList titulos;
    titulos << "Código" << "Cliente" << "Retiro" << "Total" << "Estado";
    ui->tableColasPedidos->setHorizontalHeaderLabels(titulos);
    ui->tableColasPedidos->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);

    for (int i = 0; i < SistemaBuffet::pedidos.size(); i++) {
        Pedido pedido = SistemaBuffet::pedidos[i];

        ui->tableColasPedidos->insertRow(i);
        ui->tableColasPedidos->setItem(i, 0, new QTableWidgetItem(pedido.codigo));
        ui->tableColasPedidos->setItem(i, 1, new QTableWidgetItem(pedido.cliente + " (" + pedido.curso + ")"));
        ui->tableColasPedidos->setItem(i, 2, new QTableWidgetItem(pedido.recreo));
        ui->tableColasPedidos->setItem(i, 3, new QTableWidgetItem("$" + QString::number(pedido.monto, 'f', 0)));
        ui->tableColasPedidos->setItem(i, 4, new QTableWidgetItem(pedido.estado));
    }
}

// =====================================================================
// Clic en una fila de la tabla: busca ese pedido
// =====================================================================
void CajeroWindow::on_tableColasPedidos_cellClicked(int fila, int columna)
{
    QString codigoDeLaFila = ui->tableColasPedidos->item(fila, 0)->text();
    ui->txtCodigoRetiro->setText(codigoDeLaFila);
    on_btnBuscarCodigo_clicked();
}

// =====================================================================
// Botón BUSCAR: muestra el pedido que tiene ese código
// =====================================================================
void CajeroWindow::on_btnBuscarCodigo_clicked()
{
    QString codigo = ui->txtCodigoRetiro->text().trimmed().toUpper();

    if (codigo.isEmpty()) {
        QMessageBox::warning(this, "Código Requerido", "Ingresá el código de retiro.");
        return;
    }

    int posicionPedido = SistemaBuffet::buscarPedido(codigo);

    // ----- No existe: se limpia todo -----
    if (posicionPedido == -1) {
        codigoSeleccionado = "";
        ui->lblStatusPedido->setText("No hay ningún pedido con el código " + codigo + ".");
        ui->lblDetalleAlumno->setText("");
        ui->lblDetalleRecreo->setText("");
        ui->listDetalleItems->clear();
        ui->lblTotalMonto->setText("");
        ui->btnEntregarPedido->setEnabled(false);
        ui->btnCancelarPedido->setEnabled(false);
        return;
    }

    // ----- Existe: se muestran sus datos -----
    codigoSeleccionado = codigo;
    Pedido pedido = SistemaBuffet::pedidos[posicionPedido];

    // Solo se puede cambiar un pedido PENDIENTE (igual que sp_actualizar_estado_retiro)
    if (pedido.estado == "PENDIENTE") {
        ui->btnEntregarPedido->setEnabled(true);
        ui->btnCancelarPedido->setEnabled(true);
        ui->lblStatusPedido->setText("PEDIDO " + codigo + " · PENDIENTE DE ENTREGA");
    } else {
        ui->btnEntregarPedido->setEnabled(false);
        ui->btnCancelarPedido->setEnabled(false);
        ui->lblStatusPedido->setText("PEDIDO " + codigo + " · " + pedido.estado + " · ya no se puede modificar");
    }

    ui->lblDetalleAlumno->setText("Cliente: " + pedido.cliente + "\nDNI " + pedido.dni + " · " + pedido.curso);
    ui->lblDetalleRecreo->setText("Retiro: " + pedido.recreo + "\nPago: " + pedido.metodo);

    // Un renglón por producto
    ui->listDetalleItems->clear();
    QStringList renglones = pedido.detalle.split('\n', Qt::SkipEmptyParts);
    for (int i = 0; i < renglones.size(); i++) {
        ui->listDetalleItems->addItem(renglones[i].trimmed());
    }

    QString textoTotal = "Total: $" + QString::number(pedido.monto, 'f', 0);
    if (pedido.metodo.contains("Efectivo") && pedido.vuelto > 0) {
        textoTotal = textoTotal + "\nPaga con $" + QString::number(pedido.abonado, 'f', 0)
                   + " · Vuelto $" + QString::number(pedido.vuelto, 'f', 0);
    }
    ui->lblTotalMonto->setText(textoTotal);
}

// =====================================================================
// Botón ENTREGAR: el pedido pasa a ENTREGADO
// =====================================================================
void CajeroWindow::on_btnEntregarPedido_clicked()
{
    int posicionPedido = SistemaBuffet::buscarPedido(codigoSeleccionado);
    if (posicionPedido == -1) {
        return;
    }

    if (SistemaBuffet::conexion != NULL) {
        cambiarEstadoEnBDD(posicionPedido, ESTADO_ENTREGADO);
    } else {
        SistemaBuffet::pedidos[posicionPedido].estado = "ENTREGADO";
    }

    cargarPedidosDelSistema();
    on_btnBuscarCodigo_clicked();
}

// =====================================================================
// Botón CANCELAR: el pedido pasa a CANCELADO
// Con BDD el SP devuelve el stock y el saldo
// =====================================================================
void CajeroWindow::on_btnCancelarPedido_clicked()
{
    int posicionPedido = SistemaBuffet::buscarPedido(codigoSeleccionado);
    if (posicionPedido == -1) {
        return;
    }

    if (SistemaBuffet::conexion != NULL) {
        cambiarEstadoEnBDD(posicionPedido, ESTADO_CANCELADO);
    } else {
        Pedido pedido = SistemaBuffet::pedidos[posicionPedido];
        SistemaBuffet::pedidos[posicionPedido].estado = "CANCELADO";

        // shortcut: sin BDD no se devuelve el stock y el saldo solo existe para el estudiante logueado; con BDD lo hace el SP
        if (pedido.metodo == "Saldo de Cuenta Escolar" && pedido.dni == SistemaBuffet::usuario.dni) {
            SistemaBuffet::usuario.saldo = SistemaBuffet::usuario.saldo + pedido.monto;
        }
    }

    cargarPedidosDelSistema();
    on_btnBuscarCodigo_clicked();
}

// =====================================================================
// cambiarEstadoEnBDD (MODO CON BDD): ENTREGADO (2) o CANCELADO (3)
// =====================================================================
void CajeroWindow::cambiarEstadoEnBDD(int posicionPedido, int estadoID)
{
    int retiroID = SistemaBuffet::pedidos[posicionPedido].retiroID;
    QStringList respuesta = leer_primera_fila(actualizar_estado_retiro(SistemaBuffet::conexion, SistemaBuffet::usuario.id, retiroID, estadoID));

    if (respuesta.size() == 0 || respuesta[0] == "ERROR") {
        QMessageBox::warning(this, "Error", "No se pudo actualizar el pedido en la base de datos.");
    }
}

// =====================================================================
// Botón SALIR: vuelve al login
// =====================================================================
void CajeroWindow::on_btnLogout_clicked()
{
    SistemaBuffet::cerrarSesion();
    LoginWindow *ventanaLogin = new LoginWindow();
    ventanaLogin->show();
    this->close();
}
