#ifndef SISTEMABUFFET_H
#define SISTEMABUFFET_H

#include <QString>
#include <QList>
#include <mysql.h>

// =====================================================================
// MODELO DE DATOS (structs)
// Cada struct agrupa los datos de un registro, como en la guía
// "Archivos y Structs": se crea una variable y se usa con punto.
//   Ejemplo:  Pedido nuevo;  nuevo.codigo = "3C6MB";
// Los campos son los mismos que las columnas de las tablas de la BDD.
// =====================================================================

// Tabla Estudiante (con el curso ya armado, ej. "6° Año 1ra")
struct Estudiante {
    QString dni;
    QString nombre;
    QString apellido;
    QString curso;
};

// Tabla Profesor
struct Profesor {
    QString dni;
    QString nombre;
    QString apellido;
};

// Tabla Usuario (el personal no tiene DNI)
struct Usuario {
    QString dni;
    QString apodo;
    QString contrasena;
    QString rol;
    double saldo;
};

// Tabla Empleado: el PIN es para la caja y el token para el desarrollador
struct Empleado {
    QString apodo;
    QString nombreCompleto;
    QString puesto;
    QString pin;
    QString token;
};

// Tabla Producto
struct Producto {
    int id;
    QString nombre;
    QString descripcion;
    QString categoria;
    double precio;
    int stock;
};

// Un renglón del carrito de compras
struct ItemCarrito {
    int id;            // ProductoID (el 0 es el menú del día)
    QString nombre;
    double precio;
    int cantidad;
};

// Un menú fijo de la semana (tablas Menu_Del_Dia y Detalle_Menu_Dia)
struct MenuSemanal {
    QString dia;
    QString plato;
    double precio;
    int stock;
    QString incluye;   // el producto que viene con el menú
};

// El menú que corresponde hoy (su stock está en productos[0].stock)
struct MenuDelDia {
    int id;            // Menu_Del_DiaID (0 = hoy no hay menú)
    QString plato;
    QString incluye;
    double precio;
};

// Un pedido: tablas Retiro + Venta + Carrito juntas
struct Pedido {
    int retiroID;
    QString codigo;    // código de retiro de 5 caracteres
    QString cliente;
    QString curso;
    QString dni;
    QString recreo;    // franja de retiro
    QString metodo;    // método de pago
    double monto;
    double abonado;    // con cuánto pagó (efectivo)
    double vuelto;
    QString estado;    // PENDIENTE, ENTREGADO o CANCELADO
    QString detalle;   // un renglón por producto
};

// El usuario que inició sesión
struct UsuarioActivo {
    int id;            // UserID
    QString rol;
    QString nombre;
    QString apellido;
    QString curso;
    QString dni;
    double saldo;
};

// =====================================================================
// SistemaBuffet: los datos que comparten todas las ventanas
// Todo es "static": hay una sola copia de cada variable para todo el
// programa, y se usa escribiendo SistemaBuffet::nombreVariable.
//
// El programa funciona de dos formas:
//   - CON BDD: conexion != NULL. Los datos se leen y se guardan en MySQL.
//   - SIN BDD: conexion == NULL. Se usan los datos de ejemplo de estas
//     listas (los mismos de 04_DatosSQL) y los cambios no se guardan.
// =====================================================================
class SistemaBuffet
{
public:
    // Conexión con MySQL (NULL si no se pudo conectar)
    static MYSQL *conexion;
    // true cuando ya se cargaron los pedidos de ejemplo
    static bool datosInicializados;

    // ----- Usuario que inició sesión -----
    static UsuarioActivo usuario;

    // ----- Tablas de la BDD (datos de ejemplo de 04_DatosSQL) -----
    static QList<Estudiante> estudiantes;
    static QList<Profesor> profesores;
    static QList<Usuario> usuarios;          // la posición + 1 es el UserID
    static QList<Empleado> empleados;
    static QList<Producto> productos;        // la posición es el ProductoID; la 0 es el menú del día
    static QList<MenuSemanal> menusSemana;
    static QList<Pedido> pedidos;

    // ----- Carrito y menú de hoy -----
    static QList<ItemCarrito> carrito;
    static MenuDelDia menuHoy;

    // ----- Búsquedas: devuelven la posición en la lista, o -1 si no está -----
    static int buscarUsuario(QString apodo);
    static int buscarEmpleado(QString apodo);
    static int buscarEstudiante(QString dni);
    static int buscarProfesor(QString dni);
    static int buscarDniRegistrado(QString dni);
    static int buscarPedido(QString codigo);
    static int buscarEnCarrito(int id);

    // ----- Carrito -----
    static void agregarAlCarrito(int id, QString nombre, double precio, int cantidad);
    static void vaciarCarrito();
    static double getTotalCarrito();
    static int getCantidadTotalCarrito();
    static int getCantidadEnCarrito(int id);

    // ----- Carga de datos -----
    static void inicializarDatos();                       // sin BDD: pedidos de ejemplo
    static void cargarUsuarioActivo(int posicionUsuario); // sin BDD: datos del usuario que entró
    static bool cargarUsuarioDesdeBDD(int userID);        // con BDD: datos del usuario que entró
    static void cargarMenuDelDia();                       // el menú que corresponde hoy
    static void cargarDesdeBDD();                         // con BDD: stock, precios, pedidos y menú
    static void cerrarSesion();                           // con BDD: deja el cierre en Log_Auditoria
};

#endif // SISTEMABUFFET_H
