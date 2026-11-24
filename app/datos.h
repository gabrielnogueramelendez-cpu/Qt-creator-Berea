// datos.h: Variables globales en listas paralelas: la posición i de cada lista es el mismo registro.

#ifndef DATOS_H
#define DATOS_H

#include <QString>
#include <QStringList>
#include <QList>
#include <mysql.h>

// ===== BLOQUE 1: Conexión =====
// Conexión con MySQL. La abre main() con conectar_bdd() y la usan todas las consultas.
extern MYSQL *conexion;

// ===== BLOQUE 2: Usuario que inició sesión =====
// Las llena cargar_usuario() con la consulta obtener_datos_usuario().
extern int usuario_id;
extern QString usuario_rol;        // Nombre_Rol: "Estudiante", "Profesor", "Personal de caja"...
extern QString usuario_nombre;
extern QString usuario_apellido;
extern QString usuario_curso;      // "4° Año A" o "Docente"
extern QString usuario_dni;
extern double usuario_saldo;       // saldo de la cuenta escolar

// ===== BLOQUE 3: Productos (tabla Producto en listas paralelas) =====
// La posición en la lista es el ProductoID (productos_nombres[5] es el producto 5).
extern QStringList productos_nombres;
extern QStringList productos_descripciones;
extern QList<double> productos_precios;
extern QList<int> productos_stock;
extern QList<bool> productos_activos; // false = dado de baja (no se muestra en el catálogo)

// ===== BLOQUE 4: Menú del día de hoy (sp_menu_del_dia) =====
// menu_hoy_id = 0 si hoy no hay menú (sábado y domingo).
extern int menu_hoy_id;
extern QString menu_hoy_plato;
extern QString menu_hoy_incluye;
extern double menu_hoy_precio;
extern QString menu_hoy_dia;       // "Lunes" ... "Domingo": se le pasa a sp_agregar_menu_carrito al pagar

// ===== BLOQUE 5: Carrito de compras (listas paralelas) =====
// Todavía no está en la BDD: se guarda recién al pagar (guardar_pedido en pago.cpp).
extern QList<int> carrito_ids;            // ProductoID (0 = menú del día)
extern QStringList carrito_nombres;
extern QList<double> carrito_precios;
extern QList<int> carrito_cantidades;

// ===== BLOQUE 6: Pedidos (Retiro + Venta + Carrito), un renglón por retiro =====
// Las llena cargar_pedidos() con la consulta obtener_pedidos().
extern QList<int> pedidos_retiro_ids;
extern QStringList pedidos_codigos;       // código de retiro de 5 caracteres
extern QStringList pedidos_clientes;
extern QStringList pedidos_cursos;
extern QStringList pedidos_dnis;
extern QStringList pedidos_recreos;       // franja de retiro
extern QStringList pedidos_metodos;       // método de pago
extern QList<double> pedidos_montos;
extern QStringList pedidos_estados;       // PENDIENTE, ENTREGADO o CANCELADO
extern QStringList pedidos_detalles;      // un renglón por producto, separados con '\n'

// ===== BLOQUE 7: Funciones que cargan datos desde la BDD (datos.cpp) =====
bool cargar_usuario(int userID);          // llena el bloque 2; false si el usuario no existe
void cargar_productos();                  // llena el bloque 3 (y llama a cargar_menu_del_dia)
void cargar_menu_del_dia();               // llena el bloque 4 con el menú de hoy
void cargar_pedidos();                    // llena el bloque 6 con todos los pedidos
int buscar_pedido(const QString &codigo); // posición del pedido con ese código, o -1
void salir_de_la_cuenta();                // cierre de sesión en la BDD y vacía el carrito

// ===== BLOQUE 8: Funciones del carrito (datos.cpp) =====
void agregar_al_carrito(int id, const QString &nombre, double precio, int cantidad);
void quitar_del_carrito(int fila);
void vaciar_carrito();
double total_carrito();                   // suma de precio x cantidad
int cantidad_total_carrito();             // suma de las cantidades
int cantidad_en_carrito(int id);          // cuántas unidades de ese producto ya hay

// ===== BLOQUE 9: Formato =====
QString pesos(double monto);              // 2800 -> "$2800"

#endif // DATOS_H
