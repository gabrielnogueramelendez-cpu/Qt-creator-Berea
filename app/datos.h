#ifndef DATOS_H
#define DATOS_H

#include <QString>
#include <QStringList>
#include <QList>
#include <mysql.h>

// Variables globales que comparten todas las pantallas y funciones que las llenan con la BDD.
// Programación estructurada: no hay clases propias, solo variables y funciones.

// Conexión abierta en main (conectar_bdd)
extern MYSQL *conexion;

// Usuario que inició sesión (obtener_datos_usuario)
extern int usuario_id;
extern QString usuario_rol;
extern QString usuario_nombre;
extern QString usuario_apellido;
extern QString usuario_curso;
extern QString usuario_dni;
extern double usuario_saldo;

// Tabla Producto en listas paralelas: la posición es el ProductoID.
// La posición 0 no es un producto: es el menú del día (en el carrito va con id 0)
extern QStringList productos_nombres;
extern QStringList productos_descripciones;
extern QList<double> productos_precios;
extern QList<int> productos_stock;

// Menú del día de hoy (sp_menu_del_dia). menu_hoy_id = 0 si hoy no hay menú
extern int menu_hoy_id;
extern QString menu_hoy_plato;
extern QString menu_hoy_incluye;
extern double menu_hoy_precio;

// Carrito de compras en listas paralelas (todavía no está en la BDD: se guarda al pagar)
extern QList<int> carrito_ids;
extern QStringList carrito_nombres;
extern QList<double> carrito_precios;
extern QList<int> carrito_cantidades;

// Pedidos: Retiro + Venta + Carrito (obtener_pedidos), un renglón por retiro
extern QList<int> pedidos_retiro_ids;
extern QStringList pedidos_codigos;
extern QStringList pedidos_clientes;
extern QStringList pedidos_cursos;
extern QStringList pedidos_dnis;
extern QStringList pedidos_recreos;
extern QStringList pedidos_metodos;
extern QList<double> pedidos_montos;
extern QStringList pedidos_estados;
extern QStringList pedidos_detalles;

// Carga desde la BDD
bool cargar_usuario(int userID);
void cargar_productos();
void cargar_menu_del_dia();
void cargar_pedidos();
int buscar_pedido(const QString &codigo);
void salir_de_la_cuenta();

// Carrito
void agregar_al_carrito(int id, const QString &nombre, double precio, int cantidad);
void quitar_del_carrito(int fila);
void vaciar_carrito();
double total_carrito();
int cantidad_total_carrito();
int cantidad_en_carrito(int id);

// Formato de precios: $2800
QString pesos(double monto);

#endif // DATOS_H
