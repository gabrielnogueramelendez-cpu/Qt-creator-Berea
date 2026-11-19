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
// En esta versión TODOS los datos se leen de MySQL: los structs solo
// guardan en memoria lo que devolvió la BDD, para mostrarlo en pantalla.
// =====================================================================

// Tabla Producto (+ Categoria). Se llena en cargarDesdeBDD()
struct Producto {
    int id;               // ProductoID
    QString nombre;       // Nombre_Producto
    QString descripcion;  // Descripcion_Producto
    QString categoria;    // Categoria.Categoria
    double precio;        // Precio_Producto
    int stock;            // Stock_Producto (0 si el producto está dado de baja)
};

// Un renglón del carrito de compras (todavía no está en la BDD:
// se guarda en la tabla Carrito recién al confirmar el pago)
struct ItemCarrito {
    int id;            // ProductoID (el 0 es el menú del día)
    QString nombre;
    double precio;
    int cantidad;
};

// El menú que corresponde hoy (lo devuelve sp_menu_del_dia).
// Su stock se guarda en productos[0].stock
struct MenuDelDia {
    int id;            // Menu_Del_DiaID (0 = hoy no hay menú: sábado o domingo)
    QString plato;     // Plato_menu
    QString incluye;   // producto que viene con el menú (Detalle_Menu_Dia)
    double precio;     // Precio_menu
    QString dia;       // Dia_Semana: se lo pasa a sp_agregar_menu_carrito al pagar
};

// Un pedido: tablas Retiro + Venta + Carrito juntas (consulta obtener_pedidos)
struct Pedido {
    int retiroID;      // RetiroID: lo usa sp_actualizar_estado_retiro
    QString codigo;    // Codigo_Retiro de 5 caracteres
    QString cliente;   // nombre y apellido del estudiante o profesor
    QString curso;
    QString dni;
    QString recreo;    // Franja_Retiro.Nombre_Franja
    QString metodo;    // Metodo_Pago.MetodoPago
    double monto;      // Venta.Monto_Total
    QString estado;    // PENDIENTE, ENTREGADO o CANCELADO (Estado_Retiro)
    QString detalle;   // un renglón por producto del Carrito
};

// El usuario que inició sesión (consulta obtener_datos_usuario)
struct UsuarioActivo {
    int id;            // UserID
    QString rol;       // Rol.Nombre_Rol
    QString nombre;
    QString apellido;
    QString curso;
    QString dni;
    double saldo;      // Usuario.Saldo (cuenta escolar)
};

// =====================================================================
// SistemaBuffet: los datos que comparten todas las ventanas
// Todo es "static": hay una sola copia de cada variable para todo el
// programa, y se usa escribiendo SistemaBuffet::nombreVariable.
//
// VERSIÓN SOLO BDD: el programa no arranca sin MySQL (ver main.cpp).
// Las listas empiezan vacías y se llenan con consultas a la BDD.
// =====================================================================
class SistemaBuffet
{
public:
    // Conexión con MySQL. main.cpp la abre al arrancar y la cierra al salir
    static MYSQL *conexion;

    // ----- Usuario que inició sesión -----
    static UsuarioActivo usuario;

    // ----- Datos leídos de la BDD -----
    static QList<Producto> productos;   // la posición es el ProductoID; la 0 es el menú del día
    static QList<Pedido> pedidos;       // todos los retiros, del más viejo al más nuevo

    // ----- Carrito y menú de hoy -----
    static QList<ItemCarrito> carrito;
    static MenuDelDia menuHoy;

    // ----- Búsquedas: devuelven la posición en la lista, o -1 si no está -----
    static int buscarPedido(QString codigo);
    static int buscarEnCarrito(int id);

    // ----- Carrito -----
    static void agregarAlCarrito(int id, QString nombre, double precio, int cantidad);
    static void vaciarCarrito();
    static double getTotalCarrito();
    static int getCantidadTotalCarrito();
    static int getCantidadEnCarrito(int id);

    // ----- Lectura de la BDD -----
    static bool cargarUsuarioDesdeBDD(int userID);  // datos del usuario que entró
    static void cargarProductos();                  // tabla Producto
    static void cargarPedidos();                    // tablas Retiro + Venta + Carrito
    static void cargarMenuDelDia();                 // sp_menu_del_dia con el día de hoy
    static void cargarDesdeBDD();                   // las tres de arriba juntas
    static void cerrarSesion();                     // sp_cerrar_sesion (Log_Auditoria)
};

#endif // SISTEMABUFFET_H
