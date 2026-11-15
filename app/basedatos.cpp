#include "basedatos.h"
#include <iostream>

// Datos del servidor MySQL: cambiar la contraseña por la del root de cada computadora
#ifndef BDD_SERVIDOR
#define BDD_SERVIDOR "127.0.0.1"
#endif
#ifndef BDD_USUARIO
#define BDD_USUARIO "root"
#endif
#ifndef BDD_CLAVE
#define BDD_CLAVE "12341234"
#endif
#ifndef BDD_PUERTO
#define BDD_PUERTO 3306
#endif

MYSQL* conectar_bdd()
{
    MYSQL *conn = mysql_init(NULL);

    if (mysql_real_connect(conn, BDD_SERVIDOR, BDD_USUARIO, BDD_CLAVE, "schema_comercial", BDD_PUERTO, NULL, 0)) {
        // Sin esto los acentos y la ñ llegan mal
        mysql_set_character_set(conn, "utf8mb4");
        return conn;
    }

    std::cout << "Error de conexión: " << mysql_error(conn) << "\n";
    mysql_close(conn);
    return NULL;
}

MYSQL_RES* ejecutar_consulta(MYSQL *conn, const std::string &sql)
{
    if (mysql_query(conn, sql.c_str()) != 0) {
        std::cout << "Error al ejecutar: " << mysql_error(conn) << "\n";
        return NULL;
    }
    MYSQL_RES *result = mysql_store_result(conn);

    // Un CALL devuelve un resultado extra al final; hay que leerlo para poder mandar la próxima consulta
    while (mysql_next_result(conn) == 0) {
        MYSQL_RES *extra = mysql_store_result(conn);
        if (extra != NULL) {
            mysql_free_result(extra);
        }
    }
    return result;
}

std::string texto_sql(MYSQL *conn, const QString &valor)
{
    // Hay que tener en cuenta las comillas: una ' escrita por el usuario rompería la consulta
    std::string original = valor.toStdString();
    std::string escapado(original.size() * 2 + 1, '\0');
    unsigned long largo = mysql_real_escape_string(conn, &escapado[0], original.c_str(), original.size());
    escapado.resize(largo);
    return escapado;
}

QString valor_fila(MYSQL_ROW fila, int columna)
{
    // Un NULL de la BDD llega como puntero nulo
    if (fila[columna] == NULL) {
        return "";
    }
    return QString::fromUtf8(fila[columna]);
}

int error_del_sp(MYSQL_RES *result)
{
    // Los SP devuelven estado = 'ERROR', codigo_sql y descripcion_error cuando salta el HANDLER
    // Se llama antes de leer filas: deja el resultado otra vez en la primera fila
    if (result == NULL) {
        return -1;
    }
    if (std::string(mysql_fetch_field_direct(result, 0)->name) != "estado") {
        return 0;
    }
    MYSQL_ROW fila = mysql_fetch_row(result);
    int codigo = atoi(fila[1]);
    mysql_data_seek(result, 0);
    return codigo;
}

// ---------------------------------------------------------------- Schema usuarios

MYSQL_RES* registrar_usuario(MYSQL *conn, const QString &dni, const QString &apodo, const QString &clave)
{
    std::string sql = "CALL schema_usuarios.sp_registrar_usuario('"
                      + texto_sql(conn, dni) + "', '"
                      + texto_sql(conn, apodo) + "', '"
                      + texto_sql(conn, clave) + "');";
    return ejecutar_consulta(conn, sql);
}

MYSQL_RES* iniciar_sesion(MYSQL *conn, const QString &apodo, const QString &clave)
{
    std::string sql = "CALL schema_usuarios.sp_iniciar_sesion('"
                      + texto_sql(conn, apodo) + "', '"
                      + texto_sql(conn, clave) + "');";
    return ejecutar_consulta(conn, sql);
}

MYSQL_RES* acceso_personal(MYSQL *conn, const QString &apodo, const QString &clave, const QString &pinOToken)
{
    std::string sql = "CALL schema_usuarios.sp_acceso_personal('"
                      + texto_sql(conn, apodo) + "', '"
                      + texto_sql(conn, clave) + "', '"
                      + texto_sql(conn, pinOToken) + "');";
    return ejecutar_consulta(conn, sql);
}

MYSQL_RES* cerrar_sesion(MYSQL *conn, int userID)
{
    std::string sql = "CALL schema_usuarios.sp_cerrar_sesion(" + std::to_string(userID) + ");";
    return ejecutar_consulta(conn, sql);
}

MYSQL_RES* cargar_saldo(MYSQL *conn, int userID, int monto)
{
    std::string sql = "CALL schema_usuarios.sp_cargar_saldo(" + std::to_string(userID) + ", "
                      + std::to_string(monto) + ");";
    return ejecutar_consulta(conn, sql);
}

MYSQL_RES* obtener_datos_usuario(MYSQL *conn, int userID)
{
    // Nombre, curso y DNI salen de Estudiante o de Profesor; el personal no tiene ninguno de los dos
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

// ---------------------------------------------------------------- Schema comercial

MYSQL_RES* alta_producto(MYSQL *conn, int userID, int categoriaID, const QString &nombre, const QString &descripcion, int stock, int precio)
{
    std::string sql = "CALL schema_comercial.sp_alta_producto(" + std::to_string(userID) + ", "
                      + std::to_string(categoriaID) + ", '"
                      + texto_sql(conn, nombre) + "', '"
                      + texto_sql(conn, descripcion) + "', "
                      + std::to_string(stock) + ", "
                      + std::to_string(precio) + ");";
    return ejecutar_consulta(conn, sql);
}

MYSQL_RES* modificar_stock(MYSQL *conn, int userID, int productoID, int stock)
{
    std::string sql = "CALL schema_comercial.sp_modificar_stock(" + std::to_string(userID) + ", "
                      + std::to_string(productoID) + ", " + std::to_string(stock) + ");";
    return ejecutar_consulta(conn, sql);
}

MYSQL_RES* modificar_precio(MYSQL *conn, int userID, int productoID, int precio)
{
    std::string sql = "CALL schema_comercial.sp_modificar_precio(" + std::to_string(userID) + ", "
                      + std::to_string(productoID) + ", " + std::to_string(precio) + ");";
    return ejecutar_consulta(conn, sql);
}

MYSQL_RES* baja_producto(MYSQL *conn, int userID, int productoID)
{
    std::string sql = "CALL schema_comercial.sp_baja_producto(" + std::to_string(userID) + ", "
                      + std::to_string(productoID) + ");";
    return ejecutar_consulta(conn, sql);
}

MYSQL_RES* registrar_venta(MYSQL *conn, int userID, int metodoPagoID)
{
    // EmpleadoID va en NULL: el pedido lo hace el alumno desde la app, no un cajero
    std::string sql = "CALL schema_comercial.sp_registrar_venta(" + std::to_string(userID) + ", NULL, "
                      + std::to_string(metodoPagoID) + ");";
    return ejecutar_consulta(conn, sql);
}

MYSQL_RES* agregar_producto_carrito(MYSQL *conn, int ventaID, int productoID, int cantidad)
{
    std::string sql = "CALL schema_comercial.sp_agregar_producto_carrito(" + std::to_string(ventaID) + ", "
                      + std::to_string(productoID) + ", " + std::to_string(cantidad) + ");";
    return ejecutar_consulta(conn, sql);
}

MYSQL_RES* obtener_menu_del_dia(MYSQL *conn, const QString &dia)
{
    std::string sql = "CALL schema_comercial.sp_menu_del_dia(CURDATE(), '" + texto_sql(conn, dia) + "');";
    return ejecutar_consulta(conn, sql);
}

MYSQL_RES* agregar_menu_carrito(MYSQL *conn, int ventaID, int menuID, int cantidad)
{
    std::string sql = "CALL schema_comercial.sp_agregar_menu_carrito(" + std::to_string(ventaID) + ", "
                      + std::to_string(menuID) + ", " + std::to_string(cantidad) + ");";
    return ejecutar_consulta(conn, sql);
}

MYSQL_RES* generar_retiro(MYSQL *conn, int ventaID, int franjaID, const QString &codigo)
{
    std::string sql = "CALL schema_comercial.sp_generar_retiro(" + std::to_string(ventaID) + ", "
                      + std::to_string(franjaID) + ", '" + texto_sql(conn, codigo) + "');";
    return ejecutar_consulta(conn, sql);
}

MYSQL_RES* actualizar_estado_retiro(MYSQL *conn, int userID, int retiroID, int estadoID)
{
    std::string sql = "CALL schema_comercial.sp_actualizar_estado_retiro(" + std::to_string(userID) + ", "
                      + std::to_string(retiroID) + ", " + std::to_string(estadoID) + ");";
    return ejecutar_consulta(conn, sql);
}

MYSQL_RES* obtener_productos(MYSQL *conn)
{
    std::string sql = "SELECT p.ProductoID, p.Nombre_Producto, c.Categoria, p.Precio_Producto, p.Stock_Producto, "
                      "IF(p.Activo, 'Activo', 'Inactivo') "
                      "FROM schema_comercial.Producto p "
                      "INNER JOIN schema_comercial.Categoria c ON p.ID_Categoria = c.ID_Categoria "
                      "ORDER BY p.ProductoID;";
    return ejecutar_consulta(conn, sql);
}

MYSQL_RES* obtener_pedidos(MYSQL *conn)
{
    // Un renglón por retiro, con el detalle de la tabla Carrito en un solo texto
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

MYSQL_RES* obtener_logs(MYSQL *conn)
{
    std::string sql = "SELECT u.Apodo, t.Nombre_Evento, l.Detalle "
                      "FROM schema_usuarios.Log_Auditoria l "
                      "INNER JOIN schema_usuarios.Tipo_Evento t ON l.Tipo_Evento_ID = t.Tipo_Evento_ID "
                      "LEFT JOIN schema_usuarios.Usuario u ON l.UserID = u.UserID "
                      "ORDER BY l.Log_ID DESC LIMIT 50;";
    return ejecutar_consulta(conn, sql);
}
