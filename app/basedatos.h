#ifndef BASEDATOS_H
#define BASEDATOS_H

#include <string>
#include <mysql.h>
#include <QString>
#include <QStringList>

// =====================================================================
// CONEXIÓN Y CONSULTAS A MYSQL
// Se usa la librería de C de MySQL, como en la guía
// "Conectar un gestor de BBDD al lenguaje C".
//
// Los tres tipos de la librería que aparecen en todo el programa:
//   MYSQL      -> la conexión con el servidor. Se usa como MYSQL* (puntero)
//                 porque así la devuelve mysql_init().
//   MYSQL_RES  -> la tabla de resultados que devuelve una consulta.
//                 También es un puntero: si vale NULL, la consulta falló.
//                 Cuando se termina de usar se libera con mysql_free_result().
//   MYSQL_ROW  -> una fila de esa tabla. fila[0] es la primera columna,
//                 fila[1] la segunda, y así.
// =====================================================================

// Abre la conexión. Devuelve NULL si MySQL no está disponible
MYSQL* conectar_bdd();

// Manda una consulta y devuelve la tabla de resultados (o NULL si hubo error)
MYSQL_RES* ejecutar_consulta(MYSQL *conexion, const std::string &sql);

// Ayudas para armar las consultas y leer los resultados
std::string texto_sql(MYSQL *conexion, const QString &valor);
QString valor_fila(MYSQL_ROW fila, int columna);
QStringList leer_primera_fila(MYSQL_RES *resultado);

// ---------------------------------------------------------------- Schema usuarios
MYSQL_RES* registrar_usuario(MYSQL *conexion, const QString &dni, const QString &apodo, const QString &clave);
MYSQL_RES* iniciar_sesion(MYSQL *conexion, const QString &apodo, const QString &clave);
MYSQL_RES* acceso_personal(MYSQL *conexion, const QString &apodo, const QString &clave, const QString &pinOToken);
MYSQL_RES* cerrar_sesion(MYSQL *conexion, int userID);
MYSQL_RES* cargar_saldo(MYSQL *conexion, int userID, int monto);
MYSQL_RES* obtener_datos_usuario(MYSQL *conexion, int userID);

// ---------------------------------------------------------------- Schema comercial
MYSQL_RES* alta_producto(MYSQL *conexion, int userID, int categoriaID, const QString &nombre, const QString &descripcion, int stock, int precio);
MYSQL_RES* modificar_stock(MYSQL *conexion, int userID, int productoID, int stock);
MYSQL_RES* modificar_precio(MYSQL *conexion, int userID, int productoID, int precio);
MYSQL_RES* baja_producto(MYSQL *conexion, int userID, int productoID);
MYSQL_RES* registrar_venta(MYSQL *conexion, int userID, int metodoPagoID);
MYSQL_RES* agregar_producto_carrito(MYSQL *conexion, int ventaID, int productoID, int cantidad);
MYSQL_RES* obtener_menu_del_dia(MYSQL *conexion, const QString &dia);
MYSQL_RES* agregar_menu_carrito(MYSQL *conexion, int ventaID, int menuID, int cantidad);
MYSQL_RES* generar_retiro(MYSQL *conexion, int ventaID, int franjaID, const QString &codigo);
MYSQL_RES* actualizar_estado_retiro(MYSQL *conexion, int userID, int retiroID, int estadoID);
MYSQL_RES* obtener_productos(MYSQL *conexion);
MYSQL_RES* obtener_pedidos(MYSQL *conexion);
MYSQL_RES* obtener_logs(MYSQL *conexion);

#endif // BASEDATOS_H
