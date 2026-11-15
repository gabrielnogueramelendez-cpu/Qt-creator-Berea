#ifndef BASEDATOS_H
#define BASEDATOS_H

#include <string>
#include <mysql.h>
#include <QString>

// Conexión y consultas a MySQL con la librería de C (como en "Conectar un gestor de BBDD al lenguaje C")

MYSQL* conectar_bdd();
MYSQL_RES* ejecutar_consulta(MYSQL *conn, const std::string &sql);

// Ayudas para armar las consultas y leer los resultados
std::string texto_sql(MYSQL *conn, const QString &valor);
QString valor_fila(MYSQL_ROW fila, int columna);
int error_del_sp(MYSQL_RES *result);

// Schema usuarios
MYSQL_RES* registrar_usuario(MYSQL *conn, const QString &dni, const QString &apodo, const QString &clave);
MYSQL_RES* iniciar_sesion(MYSQL *conn, const QString &apodo, const QString &clave);
MYSQL_RES* acceso_personal(MYSQL *conn, const QString &apodo, const QString &clave, const QString &pinOToken);
MYSQL_RES* cerrar_sesion(MYSQL *conn, int userID);
MYSQL_RES* cargar_saldo(MYSQL *conn, int userID, int monto);
MYSQL_RES* obtener_datos_usuario(MYSQL *conn, int userID);

// Schema comercial
MYSQL_RES* alta_producto(MYSQL *conn, int userID, int categoriaID, const QString &nombre, const QString &descripcion, int stock, int precio);
MYSQL_RES* modificar_stock(MYSQL *conn, int userID, int productoID, int stock);
MYSQL_RES* modificar_precio(MYSQL *conn, int userID, int productoID, int precio);
MYSQL_RES* baja_producto(MYSQL *conn, int userID, int productoID);
MYSQL_RES* registrar_venta(MYSQL *conn, int userID, int metodoPagoID);
MYSQL_RES* agregar_producto_carrito(MYSQL *conn, int ventaID, int productoID, int cantidad);
MYSQL_RES* obtener_menu_del_dia(MYSQL *conn);
MYSQL_RES* agregar_menu_carrito(MYSQL *conn, int ventaID, int menuID, int cantidad);
MYSQL_RES* generar_retiro(MYSQL *conn, int ventaID, int franjaID, const QString &codigo);
MYSQL_RES* actualizar_estado_retiro(MYSQL *conn, int userID, int retiroID, int estadoID);
MYSQL_RES* obtener_productos(MYSQL *conn);
MYSQL_RES* obtener_pedidos(MYSQL *conn);
MYSQL_RES* obtener_logs(MYSQL *conn);

#endif // BASEDATOS_H
