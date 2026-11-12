#ifndef SISTEMABUFFET_H
#define SISTEMABUFFET_H

#include <QString>
#include <QList>
#include <QDateTime>

struct UsuarioSistema {
    QString dni;
    QString nombre;
    QString apellido;
    QString curso;
    QString email;
    QString password;
    QString rol; // "Estudiante", "Cajero", "Admin", "Desarrollador"
    double saldoCuenta;
};

// Mapea la entidad Empleado del Modelo Relacional de Codd
struct EmpleadoBuffet {
    QString empleadoId; // "EMP-101", "EMP-102"
    QString usuario;    // "cajero1", "cajero2"
    QString nombre;
    QString apellido;
    QString puesto;     // "Cajera Mostrador TM", "Cajero Turno Tarde"
    QString password;
    QString pinTurno;   // 4 dígitos de seguridad en mostrador
};

// Mapea las credenciales de ingeniería de software / DevOps
struct DesarrolladorSistema {
    QString usuario;      // "dev_berea", "dev_root"
    QString nombre;       // "Gabriel Noguera (Dev)"
    QString password;     // "DevPass2026"
    QString developerKey; // "DEV-BEREA-2026"
};

struct ProductoBuffet {
    int id;
    QString nombre;
    QString descripcion;
    QString categoria; // "Menú del Día", "Salados", "Dulces", "Bebidas"
    double precio;
    int stock;
    bool activo;
};

struct ItemCarrito {
    int productoId;
    QString nombre;
    double precioUnitario;
    int cantidad;
    double subtotal() const { return precioUnitario * cantidad; }
};

struct PedidoBuffet {
    QString codigoRetiro; // 5 caracteres alfanuméricos
    QString dniAlumno;
    QString nombreAlumno;
    QString cursoAlumno;
    QString recreoRetiro;
    QString metodoPago;
    double montoTotal;
    double montoAbonado; // para efectivo
    double vuelto;       // para efectivo
    QList<ItemCarrito> items;
    QString estado; // "PENDIENTE", "ENTREGADO", "CANCELADO"
    QDateTime fechaHora;
    QString cajeroEntrega;
};

struct LogAuditoria {
    QDateTime fechaHora;
    QString usuario;
    QString rol;
    QString accion;
    QString detalle;
};

struct ConfiguracionMySQL {
    QString host;
    int puerto;
    QString baseDeDatos;
    QString usuario;
    QString password;
    bool conectada;
};

class SistemaBuffet
{
public:
    static SistemaBuffet& instancia();

    // Gestión de Sesión y Usuarios
    bool registrarEstudiante(const QString &dni, const QString &nombre, const QString &apellido,
                             const QString &curso, const QString &email, const QString &password,
                             QString &errorMsg);

    // Autenticación según rol especializado
    bool autenticarEstudiante(const QString &dniOEmail, const QString &password,
                              UsuarioSistema &usuarioLogueado, QString &errorMsg);
    bool autenticarPersonal(const QString &legajoOUsuario, const QString &password, const QString &pinTurno,
                            EmpleadoBuffet &empleadoLogueado, QString &errorMsg);
    bool autenticarAdmin(const QString &usuario, const QString &password,
                         UsuarioSistema &adminLogueado, QString &errorMsg);
    bool autenticarDesarrollador(const QString &usuario, const QString &password, const QString &tokenDev,
                                 DesarrolladorSistema &devLogueado, QString &errorMsg);

    UsuarioSistema getUsuarioActual() const { return m_usuarioActual; }
    void cerrarSesion();

    // Personal y Empleados (Modelo Codd)
    QList<EmpleadoBuffet> getEmpleados() const { return m_empleados; }
    QList<DesarrolladorSistema> getDesarrolladores() const { return m_desarrolladores; }

    // Catálogo e Inventario
    QList<ProductoBuffet> getProductos() const { return m_productos; }
    ProductoBuffet getProductoPorId(int id, bool &encontrado) const;
    bool agregarProducto(const QString &nombre, const QString &desc, const QString &cat,
                         double precio, int stock, QString &errorMsg);
    bool actualizarProducto(int id, double nuevoPrecio, int nuevoStock, QString &errorMsg);
    bool eliminarProducto(int id, QString &errorMsg);
    bool reducirStock(int productoId, int cantidad);

    // Carrito de Compras
    QList<ItemCarrito> getCarrito() const { return m_carrito; }
    bool agregarAlCarrito(int productoId, const QString &nombre, double precio, int cantidad, int stockMax, QString &errorMsg);
    bool modificarCantidadCarrito(int productoId, int nuevaCantidad, QString &errorMsg);
    void eliminarDelCarrito(int productoId);
    void vaciarCarrito();
    double getTotalCarrito() const;
    int getCantidadTotalCarrito() const;

    // Pedidos y Ventas
    QList<PedidoBuffet> getPedidos() const { return m_pedidos; }
    PedidoBuffet* buscarPedidoPorCodigo(const QString &codigo);
    QString crearPedido(const QString &recreoRetiro, const QString &metodoPago,
                        double montoAbonado, QString &errorMsg);
    bool entregarPedido(const QString &codigo, const QString &nombreCajero, QString &errorMsg);
    bool cancelarPedido(const QString &codigo, const QString &nombreCajero, const QString &motivo, QString &errorMsg);

    // Auditoría
    QList<LogAuditoria> getLogs() const { return m_logs; }
    void registrarLog(const QString &usuario, const QString &rol, const QString &accion, const QString &detalle);

    // Módulo Técnico y Configuración de BDD MySQL
    ConfiguracionMySQL getConfiguracionMySQL() const { return m_configMySQL; }
    void setConfiguracionMySQL(const ConfiguracionMySQL &cfg) { m_configMySQL = cfg; }
    QString probarConexionMySQL();
    QString ejecutarSimulacionSP(const QString &nombreSP, const QString &param1, const QString &param2);

private:
    SistemaBuffet();
    ~SistemaBuffet() = default;
    SistemaBuffet(const SistemaBuffet&) = delete;
    SistemaBuffet& operator=(const SistemaBuffet&) = delete;

    void inicializarDatosPorDefecto();
    QString generarCodigoRetiroUnico();

    QList<UsuarioSistema> m_usuarios;
    QList<EmpleadoBuffet> m_empleados;
    QList<DesarrolladorSistema> m_desarrolladores;
    UsuarioSistema m_usuarioActual;
    bool m_haySesionIniciada;

    QList<ProductoBuffet> m_productos;
    QList<ItemCarrito> m_carrito;
    QList<PedidoBuffet> m_pedidos;
    QList<LogAuditoria> m_logs;

    ConfiguracionMySQL m_configMySQL;
};

#endif // SISTEMABUFFET_H
