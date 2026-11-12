#include "devwindow.h"
#include "ui_devwindow.h"
#include "mainwindow.h"
#include "cajerowindow.h"
#include "adminwindow.h"
#include "loginwindow.h"
#include "sistemabuffet.h"
#include <QMessageBox>

DevWindow::DevWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DevWindow)
{
    ui->setupUi(this);
    setWindowTitle("💻 Modo Desarrollador & BDD - El Buffet de Alejandro");

    cargarEsquemaCodd();
    on_btnTestConexion_clicked();
}

DevWindow::~DevWindow()
{
    delete ui;
}

void DevWindow::cargarEsquemaCodd()
{
    QString esquema =
        "===============================================================================\n"
        " ESQUEMA RELACIONAL EN TERCERA FORMA NORMAL (3FN) — 20 ENTIDADES (EDGAR CODD)  \n"
        " Proyecto Berea - El Buffet de Alejandro (Arquitectura Backend MySQL)          \n"
        "===============================================================================\n\n"
        "[1]  Pais (PK: ID_Pais, Nombre_Pais)\n"
        "[2]  Provincia (PK: ID_Provincia, FK: ID_Pais, Nombre_Provincia)\n"
        "[3]  Ciudad (PK: ID_Ciudad, FK: ID_Provincia, Nombre_Ciudad)\n"
        "[4]  Region (PK: ID_Region, FK: ID_Ciudad, Nombre_Region)\n"
        "[5]  Distrito_Escolar (PK: ID_DE, FK: ID_Region, Jurisdiccion)\n"
        "[6]  Colegio (PK: ID_Colegio, FK: ID_DE, Nombre_Colegio, Direccion)\n"
        "[7]  Estudiante (PK: DNI, FK: ID_Colegio, CUIT, Nombre_Estudiante, Apellido_Estudiante, Fecha_Nacimiento)\n"
        "[8]  Usuario (PK: UserID, FK: DNI, Apodo)\n"
        "[9]  Email (PK: Email_ID, FK: UserID, Email)\n"
        "[10] Telefono (PK: Telefono_ID, FK: UserID, Telefono)\n"
        "[11] Empleado (PK: EmpleadoID, Nombre_Empleado, Apellido_Empleado, Puesto_Empleado)\n"
        "[12] Metodo_Pago (PK: Metodo_PagoID, MetodoPago)\n"
        "[13] Venta (PK: VentaID, FK: EmpleadoID, FK: UserID, FK: Metodo_PagoID, Fecha_Hora, Monto_Total)\n"
        "[14] Estado_Retiro (PK: Estado_Retiro_ID, Estado)\n"
        "[15] Retiro (PK: RetiroID, FK: VentaID, FK: EstadoID, Tiempo_Retiro, Codigo_Retiro)\n"
        "[16] Categoria (PK: ID_Categoria, Categoria)\n"
        "[17] Producto (PK: ProductID, FK: ID_Categoria, Nombre_Producto, Descripcion_Producto, Stock_Producto, Precio_Producto)\n"
        "[18] Menu_Del_Dia (PK: Menu_Del_DiaID, FK: ID_Categoria, Plato_menu, Precio_menu, Stock_menu, Fecha_menu)\n"
        "[19] Detalle_Menu_Dia (PK: Detalle_Menu_Dia, FK: Menu_Del_DiaID, FK: ProductID)\n"
        "[20] Carrito (PK: CarritoID, FK: VentaID, FK: ProductID, FK: Menu_Del_DiaID, Cantidad, precio_Unitario)\n\n"
        "[ANEXO AUDITORÍA]\n"
        "• Tipo_Evento (PK: Tipo_Evento_ID, Nombre_Evento)\n"
        "• Log_Auditoria (PK: Log_ID, FK: UserID, FK: Tipo_Evento_ID, Fecha, Hora)\n";

    ui->txtModeloCodd->setText(esquema);
}

void DevWindow::on_btnTestConexion_clicked()
{
    QString diag = SistemaBuffet::instancia().probarConexionMySQL();
    ui->txtSalidaMySQL->setText(diag);
}

void DevWindow::on_btnEjecutarSP_clicked()
{
    QString sp = ui->comboSP->currentText();
    QString p1 = ui->txtParam1->text().trimmed();
    QString p2 = ui->txtParam2->text().trimmed();

    QString res = SistemaBuffet::instancia().ejecutarSimulacionSP(sp, p1, p2);
    ui->txtSalidaSP->setText(res);
}

void DevWindow::on_btnAbrirVistaAlumno_clicked()
{
    MainWindow *mainWin = new MainWindow();
    mainWin->show();
}

void DevWindow::on_btnAbrirVistaCaja_clicked()
{
    CajeroWindow *cajeroWin = new CajeroWindow();
    cajeroWin->show();
}

void DevWindow::on_btnAbrirVistaAdmin_clicked()
{
    AdminWindow *adminWin = new AdminWindow();
    adminWin->show();
}

void DevWindow::on_btnLogoutDev_clicked()
{
    SistemaBuffet::instancia().cerrarSesion();
    LoginWindow *loginWin = new LoginWindow();
    loginWin->show();
    this->close();
}
