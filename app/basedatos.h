// basedatos.h: Funciones que hablan con MySQL (basedatos.cpp).

#ifndef BASEDATOS_H
#define BASEDATOS_H

#include <string>
#include <mysql.h>
#include <QString>
#include <QStringList>

// ===== BLOQUE 1: Conexión y ejecución =====
// Abre la conexión con MySQL. Devuelve NULL si no se pudo conectar.
MYSQL* conectar_bdd();

// Manda una instrucción SQL (SELECT o CALL) y devuelve su resultado, o NULL si falló.
MYSQL_RES* ejecutar_consulta(MYSQL *conn, const std::string &sql);

// ===== BLOQUE 2: Funciones auxiliares para armar consultas y leer resultados =====
// Escapa las comillas de un texto escrito por el usuario para meterlo en un SQL sin riesgo.
std::string texto_sql(const QString &valor);

// Devuelve la columna de una fila como QString ("" si es NULL).
QString valor_fila(MYSQL_ROW fila, int columna);

// Devuelve el código de error MySQL si la fila es la del EXIT HANDLER ('ERROR', 1062...), o 0 si salió bien.
int error_del_sp(MYSQL_ROW fila);

// Devuelve el número de la primera columna (por ejemplo el VentaID o las filas afectadas) y libera el resultado.
int numero_del_sp(MYSQL_RES *result);

// ===== BLOQUE 3: SP y consultas de schema_usuarios =====
MYSQL_RES* registrar_usuario(MYSQL *conn, const QString &dni, const QString &apodo, const QString &clave);   // sp_registrar_usuario
MYSQL_RES* iniciar_sesion(MYSQL *conn, const QString &apodo, const QString &clave);                         // sp_iniciar_sesion
MYSQL_RES* acceso_personal(MYSQL *conn, const QString &apodo, const QString &clave, const QString &pinOToken); // sp_acceso_personal
MYSQL_RES* cerrar_sesion(MYSQL *conn, int userID);                                                          // sp_cerrar_sesion
MYSQL_RES* cargar_saldo(MYSQL *conn, int userID, int monto);                                                // sp_cargar_saldo
MYSQL_RES* obtener_datos_usuario(MYSQL *conn, int userID);                                                  // SELECT de Usuario

// ===== BLOQUE 4: SP y consultas de schema_comercial =====
MYSQL_RES* alta_producto(MYSQL *conn, int userID, int categoriaID, const QString &nombre, const QString &descripcion, int stock, int precio); // sp_alta_producto
MYSQL_RES* modificar_stock(MYSQL *conn, int userID, int productoID, int stock);       // sp_modificar_stock
MYSQL_RES* modificar_precio(MYSQL *conn, int userID, int productoID, int precio);     // sp_modificar_precio
MYSQL_RES* baja_producto(MYSQL *conn, int userID, int productoID);                    // sp_baja_producto
MYSQL_RES* registrar_venta(MYSQL *conn, int userID, int metodoPagoID);                // sp_registrar_venta
MYSQL_RES* agregar_producto_carrito(MYSQL *conn, int ventaID, int productoID, int cantidad); // sp_agregar_producto_carrito
MYSQL_RES* obtener_menu_del_dia(MYSQL *conn, const QString &dia);                     // sp_menu_del_dia
MYSQL_RES* agregar_menu_carrito(MYSQL *conn, int ventaID, int menuID, int cantidad, const QString &dia); // sp_agregar_menu_carrito
MYSQL_RES* generar_retiro(MYSQL *conn, int ventaID, int franjaID, const QString &codigo); // sp_generar_retiro
MYSQL_RES* actualizar_estado_retiro(MYSQL *conn, int userID, int retiroID, int estadoID); // sp_actualizar_estado_retiro
MYSQL_RES* obtener_productos(MYSQL *conn);                                            // SELECT de Producto
MYSQL_RES* obtener_pedidos(MYSQL *conn);                                              // SELECT de Retiro + Venta + Carrito
MYSQL_RES* obtener_logs(MYSQL *conn);                                                 // SELECT de Log_Auditoria

#endif // BASEDATOS_H
