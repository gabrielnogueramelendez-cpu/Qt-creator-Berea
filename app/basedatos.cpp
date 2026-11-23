// basedatos.cpp: Todo lo que habla con MySQL: conexión y una función por SP o consulta.

#include "basedatos.h"
#include <iostream>
#include <cstdlib>

// ===== BLOQUE 1: CONFIGURACIÓN DE CONEXIÓN Y SERVIDOR =====
// Datos de conexión. Si BDD_CLAVE da error 1045, se prueba BDD_CLAVE_WINDOWS.

#ifndef BDD_SERVIDOR
#define BDD_SERVIDOR "127.0.0.1"
#endif

#ifndef BDD_USUARIO
#define BDD_USUARIO "root"
#endif

#ifndef BDD_CLAVE
#define BDD_CLAVE "12341234"               // Clave estándar de las computadoras con Ubuntu
#endif

#ifndef BDD_CLAVE_WINDOWS
#define BDD_CLAVE_WINDOWS ""  // clave alternativa (PC de casa): poner la propia al compilar
#endif

#ifndef BDD_PUERTO
#define BDD_PUERTO 3306
#endif

#ifndef BDD_DEFAULT_SCHEMA
#define BDD_DEFAULT_SCHEMA "schema_comercial"
#endif

// ===== BLOQUE 2: INICIALIZACIÓN Y CONEXIÓN (conectar_bdd) =====
// mysql_init + mysql_real_connect.
// Sirve para abrir la conexión con MySQL.
MYSQL* conectar_bdd()
{
    MYSQL *conn = mysql_init(NULL);
    if (conn == NULL) {
        std::cout << "[ERROR BDD] No se pudo inicializar la estructura MySQL (memoria insuficiente).\n";
        return NULL;
    }

    if (mysql_real_connect(conn, BDD_SERVIDOR, BDD_USUARIO, BDD_CLAVE,
                           BDD_DEFAULT_SCHEMA, BDD_PUERTO, NULL, 0)) {
        mysql_query(conn, "SET NAMES utf8mb4;");   // para que lleguen bien las tildes y la ñ
        std::cout << "[BDD] Conectado exitosamente a MySQL (credencial principal: " << BDD_USUARIO << "@" << BDD_SERVIDOR << ").\n";
        return conn;
    }

    // Si no entró con la clave principal, prueba con la de Windows
    mysql_close(conn);
    conn = mysql_init(NULL);
    if (conn == NULL) {
        return NULL;
    }
    if (mysql_real_connect(conn, BDD_SERVIDOR, BDD_USUARIO, BDD_CLAVE_WINDOWS,
                           BDD_DEFAULT_SCHEMA, BDD_PUERTO, NULL, 0)) {
        mysql_query(conn, "SET NAMES utf8mb4;");
        std::cout << "[BDD] Conectado exitosamente a MySQL (credencial Windows: " << BDD_USUARIO << "@" << BDD_SERVIDOR << ").\n";
        return conn;
    }

    std::cout << "[ERROR BDD] Error al conectar con MySQL: " << mysql_error(conn) << "\n";
    mysql_close(conn);
    return NULL;
}

// ===== BLOQUE 3: MOTOR DE EJECUCIÓN SQL Y MULTI-RESULTADOS (ejecutar_consulta) =====
// mysql_query + mysql_store_result.
// Sirve para mandar cualquier SQL (SELECT o CALL) y recibir su resultado.
MYSQL_RES* ejecutar_consulta(MYSQL *conn, const std::string &sql)
{
    if (conn == NULL) {
        std::cout << "[ERROR BDD] Intento de consulta sobre conexion nula.\n";
        return NULL;
    }

    if (mysql_query(conn, sql.c_str()) != 0) {
        std::cout << "[ERROR SQL] Fallo al ejecutar consulta: " << mysql_error(conn) << "\n";
        std::cout << "[CONSULTA FALLIDA]: " << sql << "\n";
        return NULL;
    }

    MYSQL_RES *result = mysql_store_result(conn);

    while (mysql_next_result(conn) == 0) {
        MYSQL_RES *extra = mysql_store_result(conn);
        if (extra != NULL) {
            mysql_free_result(extra);
        }
    }

    return result;
}

// ===== BLOQUE 4: UTILIDADES DE SANITIZACIÓN Y LECTURA DE DATOS =====

// Sirve para meter en un SQL un texto escrito por el usuario.
std::string texto_sql(const QString &valor)
{
    QString escapado = valor;
    escapado.replace("\\", "\\\\");   // primero la barra, después la comilla
    escapado.replace("'", "''");      // en SQL, '' dentro de un texto es una comilla
    return escapado.toStdString();
}

// Sirve para leer la columna de una fila como QString.
QString valor_fila(MYSQL_ROW fila, int columna)
{
    if (fila == NULL || fila[columna] == NULL) {
        return "";
    }
    return QString::fromUtf8(fila[columna]);
}

// Sirve para saber si un SP falló: su EXIT HANDLER devuelve la fila 'ERROR', código, mensaje.
int error_del_sp(MYSQL_ROW fila)
{
    if (fila != NULL && valor_fila(fila, 0) == "ERROR") {
        return valor_fila(fila, 1).toInt();
    }
    return 0;
}

// Sirve para leer el número que devuelven muchos SP (el VentaID nuevo o cuántas filas cambió).
int numero_del_sp(MYSQL_RES *result)
{
    int numero = 0;
    if (result != NULL) {
        MYSQL_ROW fila = mysql_fetch_row(result);
        if (fila != NULL && error_del_sp(fila) == 0) {
            numero = valor_fila(fila, 0).toInt();
        }
        mysql_free_result(result);
    }
    return numero;
}

// ===== BLOQUE 5: PROCEDIMIENTOS ALMACENADOS DE schema_usuarios =====

// SP 1: Registra nuevo usuario verificando existencia de DNI en Estudiante o Profesor
MYSQL_RES* registrar_usuario(MYSQL *conn, const QString &dni, const QString &apodo, const QString &clave)
{
    std::string sql = "CALL schema_usuarios.sp_registrar_usuario('"
                      + texto_sql(dni) + "', '"
                      + texto_sql(apodo) + "', '"
                      + texto_sql(clave) + "');";
    return ejecutar_consulta(conn, sql);
}

// SP 2: Inicio de sesión estándar (Alumnos, Profesores y Administración)
MYSQL_RES* iniciar_sesion(MYSQL *conn, const QString &apodo, const QString &clave)
{
    std::string sql = "CALL schema_usuarios.sp_iniciar_sesion('"
                      + texto_sql(apodo) + "', '"
                      + texto_sql(clave) + "');";
    return ejecutar_consulta(conn, sql);
}

// SP 2b: Acceso con credencial de doble factor (PIN de 4 dígitos en caja o Token en desarrollo)
MYSQL_RES* acceso_personal(MYSQL *conn, const QString &apodo, const QString &clave, const QString &pinOToken)
{
    std::string sql = "CALL schema_usuarios.sp_acceso_personal('"
                      + texto_sql(apodo) + "', '"
                      + texto_sql(clave) + "', '"
                      + texto_sql(pinOToken) + "');";
    return ejecutar_consulta(conn, sql);
}

// SP 3: Cierre de sesión y registro de evento en Log_Auditoria
MYSQL_RES* cerrar_sesion(MYSQL *conn, int userID)
{
    std::string sql = "CALL schema_usuarios.sp_cerrar_sesion(" + std::to_string(userID) + ");";
    return ejecutar_consulta(conn, sql);
}

// SP 4: Carga de saldo en la cuenta escolar del estudiante
MYSQL_RES* cargar_saldo(MYSQL *conn, int userID, int monto)
{
    std::string sql = "CALL schema_usuarios.sp_cargar_saldo(" + std::to_string(userID) + ", "
                      + std::to_string(monto) + ");";
    return ejecutar_consulta(conn, sql);
}

// Consulta estructurada para obtener datos personales, rol y saldo del usuario activo
MYSQL_RES* obtener_datos_usuario(MYSQL *conn, int userID)
{
    std::string sql = "SELECT r.Nombre_Rol, "
                      "COALESCE(e.Nombre_Estudiante, p.Nombre_Profesor, em.Nombre_Empleado, u.Apodo), "
                      "COALESCE(e.Apellido_Estudiante, p.Apellido_Profesor, em.Apellido_Empleado, ''), "
                      "COALESCE(CONCAT(c.Anio, '° Año ', c.Division), IF(u.DNI_Profesor IS NULL, '', 'Docente')), "
                      "COALESCE(u.DNI, u.DNI_Profesor, ''), u.Saldo "
                      "FROM schema_usuarios.Usuario u "
                      "INNER JOIN schema_usuarios.Rol r ON u.RolID = r.RolID "
                      "LEFT JOIN schema_usuarios.Estudiante e ON u.DNI = e.DNI "
                      "LEFT JOIN schema_usuarios.Curso c ON e.ID_Curso = c.ID_Curso "
                      "LEFT JOIN schema_usuarios.Profesor p ON u.DNI_Profesor = p.DNI "
                      "LEFT JOIN schema_usuarios.Empleado em ON u.UserID = em.UserID "
                      "WHERE u.UserID = " + std::to_string(userID) + ";";
    return ejecutar_consulta(conn, sql);
}

// ===== BLOQUE 6: PROCEDIMIENTOS ALMACENADOS Y CONSULTAS DE schema_comercial =====

// SP 5: Alta de producto nuevo por parte de la administración
MYSQL_RES* alta_producto(MYSQL *conn, int userID, int categoriaID, const QString &nombre, const QString &descripcion, int stock, int precio)
{
    std::string sql = "CALL schema_comercial.sp_alta_producto(" + std::to_string(userID) + ", "
                      + std::to_string(categoriaID) + ", '"
                      + texto_sql(nombre) + "', '"
                      + texto_sql(descripcion) + "', "
                      + std::to_string(stock) + ", "
                      + std::to_string(precio) + ");";
    return ejecutar_consulta(conn, sql);
}

// SP 6: Modificación de stock con auditoría automática
MYSQL_RES* modificar_stock(MYSQL *conn, int userID, int productoID, int stock)
{
    std::string sql = "CALL schema_comercial.sp_modificar_stock(" + std::to_string(userID) + ", "
                      + std::to_string(productoID) + ", " + std::to_string(stock) + ");";
    return ejecutar_consulta(conn, sql);
}

// SP 6b: Modificación de precio con auditoría automática
MYSQL_RES* modificar_precio(MYSQL *conn, int userID, int productoID, int precio)
{
    std::string sql = "CALL schema_comercial.sp_modificar_precio(" + std::to_string(userID) + ", "
                      + std::to_string(productoID) + ", " + std::to_string(precio) + ");";
    return ejecutar_consulta(conn, sql);
}

// SP 7: Baja lógica de producto (Activo = FALSE)
MYSQL_RES* baja_producto(MYSQL *conn, int userID, int productoID)
{
    std::string sql = "CALL schema_comercial.sp_baja_producto(" + std::to_string(userID) + ", "
                      + std::to_string(productoID) + ");";
    return ejecutar_consulta(conn, sql);
}

// SP 8: Registro inicial de venta (cabecera)
MYSQL_RES* registrar_venta(MYSQL *conn, int userID, int metodoPagoID)
{
    std::string sql = "CALL schema_comercial.sp_registrar_venta(" + std::to_string(userID) + ", NULL, "
                      + std::to_string(metodoPagoID) + ");";
    return ejecutar_consulta(conn, sql);
}

// SP 9: Agregar ítem de producto al carrito y descontar stock
MYSQL_RES* agregar_producto_carrito(MYSQL *conn, int ventaID, int productoID, int cantidad)
{
    std::string sql = "CALL schema_comercial.sp_agregar_producto_carrito(" + std::to_string(ventaID) + ", "
                      + std::to_string(productoID) + ", " + std::to_string(cantidad) + ");";
    return ejecutar_consulta(conn, sql);
}

// SP 10: Consulta del menú fijo para el día hábil indicado
MYSQL_RES* obtener_menu_del_dia(MYSQL *conn, const QString &dia)
{
    std::string sql = "CALL schema_comercial.sp_menu_del_dia('" + texto_sql(dia) + "');";
    return ejecutar_consulta(conn, sql);
}

// SP 10b: Agregar menú del día al carrito.
MYSQL_RES* agregar_menu_carrito(MYSQL *conn, int ventaID, int menuID, int cantidad, const QString &dia)
{
    std::string sql = "CALL schema_comercial.sp_agregar_menu_carrito(" + std::to_string(ventaID) + ", "
                      + std::to_string(menuID) + ", " + std::to_string(cantidad) + ", '"
                      + texto_sql(dia) + "');";
    return ejecutar_consulta(conn, sql);
}

// SP 11: Generar retiro con código único alfanumérico de 5 caracteres
MYSQL_RES* generar_retiro(MYSQL *conn, int ventaID, int franjaID, const QString &codigo)
{
    std::string sql = "CALL schema_comercial.sp_generar_retiro(" + std::to_string(ventaID) + ", "
                      + std::to_string(franjaID) + ", '" + texto_sql(codigo) + "');";
    return ejecutar_consulta(conn, sql);
}

// SP 12: Actualizar estado de retiro (ENTREGADO = 2, CANCELADO = 3)
MYSQL_RES* actualizar_estado_retiro(MYSQL *conn, int userID, int retiroID, int estadoID)
{
    std::string sql = "CALL schema_comercial.sp_actualizar_estado_retiro(" + std::to_string(userID) + ", "
                      + std::to_string(retiroID) + ", " + std::to_string(estadoID) + ");";
    return ejecutar_consulta(conn, sql);
}

// Consulta de catálogo completo con nombres de categoría
MYSQL_RES* obtener_productos(MYSQL *conn)
{
    std::string sql = "SELECT p.ProductoID, p.Nombre_Producto, c.Categoria, p.Precio_Producto, p.Stock_Producto, "
                      "IF(p.Activo, 'Activo', 'Inactivo'), p.Descripcion_Producto "
                      "FROM schema_comercial.Producto p "
                      "INNER JOIN schema_comercial.Categoria c ON p.ID_Categoria = c.ID_Categoria "
                      "ORDER BY p.ProductoID;";
    return ejecutar_consulta(conn, sql);
}

// Consulta de pedidos para mostrador y auditoría (unificación de Retiro + Venta + Carrito)
MYSQL_RES* obtener_pedidos(MYSQL *conn)
{
    std::string sql = "SELECT r.RetiroID, r.Codigo_Retiro, "
                      "COALESCE(CONCAT(e.Nombre_Estudiante, ' ', e.Apellido_Estudiante), "
                      "CONCAT(pf.Nombre_Profesor, ' ', pf.Apellido_Profesor), u.Apodo), "
                      "COALESCE(CONCAT(cu.Anio, '° Año ', cu.Division), IF(u.DNI_Profesor IS NULL, '', 'Docente')), "
                      "COALESCE(u.DNI, u.DNI_Profesor, ''), f.Nombre_Franja, mp.MetodoPago, v.Monto_Total, er.Estado, "
                      "(SELECT GROUP_CONCAT(CONCAT(ca.Cantidad, ' x ', COALESCE(pr.Nombre_Producto, m.Plato_Menu), "
                      "' ($', ROUND(ca.Cantidad * ca.Precio_Unitario), ')') SEPARATOR '\\n') "
                      " FROM schema_comercial.Carrito ca "
                      " LEFT JOIN schema_comercial.Producto pr ON ca.ProductoID = pr.ProductoID "
                      " LEFT JOIN schema_comercial.Menu_Del_Dia m ON ca.Menu_Del_DiaID = m.Menu_Del_DiaID "
                      " WHERE ca.VentaID = v.VentaID) "
                      "FROM schema_comercial.Retiro r "
                      "INNER JOIN schema_comercial.Venta v ON r.VentaID = v.VentaID "
                      "INNER JOIN schema_usuarios.Usuario u ON v.UserID = u.UserID "
                      "LEFT JOIN schema_usuarios.Estudiante e ON u.DNI = e.DNI "
                      "LEFT JOIN schema_usuarios.Curso cu ON e.ID_Curso = cu.ID_Curso "
                      "LEFT JOIN schema_usuarios.Profesor pf ON u.DNI_Profesor = pf.DNI "
                      "INNER JOIN schema_comercial.Franja_Retiro f ON r.ID_Franja = f.ID_Franja "
                      "INNER JOIN schema_comercial.Metodo_Pago mp ON v.Metodo_PagoID = mp.Metodo_PagoID "
                      "INNER JOIN schema_comercial.Estado_Retiro er ON r.Estado_Retiro_ID = er.Estado_Retiro_ID "
                      "ORDER BY r.RetiroID;";
    return ejecutar_consulta(conn, sql);
}

// Consulta de los últimos 50 registros de auditoría
MYSQL_RES* obtener_logs(MYSQL *conn)
{
    std::string sql = "SELECT u.Apodo, t.Nombre_Evento, l.Detalle "
                      "FROM schema_usuarios.Log_Auditoria l "
                      "INNER JOIN schema_usuarios.Tipo_Evento t ON l.Tipo_Evento_ID = t.Tipo_Evento_ID "
                      "LEFT JOIN schema_usuarios.Usuario u ON l.UserID = u.UserID "
                      "ORDER BY l.Log_ID DESC LIMIT 50;";
    return ejecutar_consulta(conn, sql);
}
