#include "basedatos.h"
#include <iostream>

// =====================================================================
// basedatos.cpp: TODO lo que habla con MySQL está en este archivo
// Como en el material 006: una función por consulta. Cada función arma
// el texto SQL (casi siempre un CALL a un stored procedure), lo manda
// con ejecutar_consulta() y devuelve la tabla de resultados (MYSQL_RES).
// Quien la llama la lee con leer_primera_fila() o con mysql_fetch_row().
//
// Orden del archivo:
//   1. Datos del servidor y conexión
//   2. Funciones de ayuda (ejecutar, escapar textos, leer filas)
//   3. Consultas del schema_usuarios
//   4. Consultas del schema_comercial
// =====================================================================

// =====================================================================
// 1. DATOS DEL SERVIDOR MYSQL
// Cambiar la contraseña por la del usuario root de cada computadora
// =====================================================================
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

// =====================================================================
// conectar_bdd: abre la conexión con MySQL
// Devuelve la conexión, o NULL si no se pudo conectar
// =====================================================================
MYSQL* conectar_bdd()
{
    MYSQL *conexion = mysql_init(NULL);

    bool conecto = mysql_real_connect(conexion, BDD_SERVIDOR, BDD_USUARIO, BDD_CLAVE,
                                      "schema_comercial", BDD_PUERTO, NULL, 0);
    if (conecto) {
        // Sin esto los acentos y la ñ llegan mal
        mysql_set_character_set(conexion, "utf8mb4");
        return conexion;
    } else {
        std::cout << "Error de conexión: " << mysql_error(conexion) << "\n";
        mysql_close(conexion);
        return NULL;
    }
}

// =====================================================================
// 2. FUNCIONES DE AYUDA
// =====================================================================

// =====================================================================
// ejecutar_consulta: manda una consulta (SELECT o CALL) a MySQL
// Devuelve la tabla de resultados, o NULL si la consulta tuvo un error
// =====================================================================
MYSQL_RES* ejecutar_consulta(MYSQL *conexion, const std::string &sql)
{
    // 1. Se manda la consulta (mysql_query devuelve 0 si salió bien)
    if (mysql_query(conexion, sql.c_str()) != 0) {
        std::cout << "Error al ejecutar: " << mysql_error(conexion) << "\n";
        return NULL;
    }

    // 2. Se guarda la tabla de resultados
    MYSQL_RES *resultado = mysql_store_result(conexion);

    // 3. Un CALL a un SP devuelve un resultado extra (vacío) al final.
    //    Hay que leerlo y descartarlo; si no, la próxima consulta da error.
    while (mysql_next_result(conexion) == 0) {
        MYSQL_RES *resultadoExtra = mysql_store_result(conexion);
        if (resultadoExtra != NULL) {
            mysql_free_result(resultadoExtra);
        }
    }

    return resultado;
}

// =====================================================================
// texto_sql: prepara un texto escrito por el usuario para meterlo en un SQL
// Si el usuario escribe una comilla ', se le agrega una \ adelante para
// que no rompa la consulta (mysql_real_escape_string)
// =====================================================================
std::string texto_sql(MYSQL *conexion, const QString &valor)
{
    std::string textoOriginal = valor.toStdString();

    // Lugar de sobra: en el peor caso cada letra ocupa el doble
    std::string textoSeguro(textoOriginal.size() * 2 + 1, ' ');
    unsigned long largoFinal = mysql_real_escape_string(conexion, &textoSeguro[0],
                                                        textoOriginal.c_str(), textoOriginal.size());
    textoSeguro.resize(largoFinal);
    return textoSeguro;
}

// =====================================================================
// valor_fila: lee una columna de una fila como texto de Qt
// Si en la BDD ese dato es NULL, devuelve un texto vacío
// =====================================================================
QString valor_fila(MYSQL_ROW fila, int columna)
{
    if (fila[columna] == NULL) {
        return "";
    } else {
        return QString::fromUtf8(fila[columna]);
    }
}

// =====================================================================
// leer_primera_fila: lee la primera fila de un resultado y lo libera
// La usan los SP que devuelven una sola fila.
// Devuelve una lista de textos, una posición por columna:
//   - lista vacía      -> la consulta falló o no devolvió filas
//   - lista[0] "ERROR" -> el SP entró en su HANDLER de error:
//                         lista[1] es el código y lista[2] la descripción
//   - otra cosa        -> los datos que devolvió el SP
// =====================================================================
QStringList leer_primera_fila(MYSQL_RES *resultado)
{
    QStringList datos;

    if (resultado == NULL) {
        return datos;
    }

    MYSQL_ROW fila = mysql_fetch_row(resultado);
    if (fila != NULL) {
        int cantidadColumnas = mysql_num_fields(resultado);
        for (int columna = 0; columna < cantidadColumnas; columna++) {
            datos.append(valor_fila(fila, columna));
        }
    }

    mysql_free_result(resultado);
    return datos;
}

// =====================================================================
// 3. SCHEMA_USUARIOS: registro, login, sesión y saldo
// =====================================================================

// Crea la cuenta de un estudiante o profesor (sp_registrar_usuario)
MYSQL_RES* registrar_usuario(MYSQL *conexion, const QString &dni, const QString &apodo, const QString &clave)
{
    std::string sql = "CALL schema_usuarios.sp_registrar_usuario('"
                      + texto_sql(conexion, dni) + "', '"
                      + texto_sql(conexion, apodo) + "', '"
                      + texto_sql(conexion, clave) + "');";
    return ejecutar_consulta(conexion, sql);
}

// Login de estudiantes, profesores y administración (sp_iniciar_sesion)
MYSQL_RES* iniciar_sesion(MYSQL *conexion, const QString &apodo, const QString &clave)
{
    std::string sql = "CALL schema_usuarios.sp_iniciar_sesion('"
                      + texto_sql(conexion, apodo) + "', '"
                      + texto_sql(conexion, clave) + "');";
    return ejecutar_consulta(conexion, sql);
}

// Login del personal de caja (con PIN) y del desarrollador (con token)
MYSQL_RES* acceso_personal(MYSQL *conexion, const QString &apodo, const QString &clave, const QString &pinOToken)
{
    std::string sql = "CALL schema_usuarios.sp_acceso_personal('"
                      + texto_sql(conexion, apodo) + "', '"
                      + texto_sql(conexion, clave) + "', '"
                      + texto_sql(conexion, pinOToken) + "');";
    return ejecutar_consulta(conexion, sql);
}

// Deja el cierre de sesión en Log_Auditoria
MYSQL_RES* cerrar_sesion(MYSQL *conexion, int userID)
{
    std::string sql = "CALL schema_usuarios.sp_cerrar_sesion(" + std::to_string(userID) + ");";
    return ejecutar_consulta(conexion, sql);
}

// Suma plata al saldo de la cuenta escolar
MYSQL_RES* cargar_saldo(MYSQL *conexion, int userID, int monto)
{
    std::string sql = "CALL schema_usuarios.sp_cargar_saldo(" + std::to_string(userID) + ", "
                      + std::to_string(monto) + ");";
    return ejecutar_consulta(conexion, sql);
}

// Nombre, apellido, curso, DNI, rol y saldo del usuario que entró
MYSQL_RES* obtener_datos_usuario(MYSQL *conexion, int userID)
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
    return ejecutar_consulta(conexion, sql);
}

// =====================================================================
// 4. SCHEMA_COMERCIAL: productos, ventas, menú, retiros y logs
// =====================================================================

// Agrega un producto nuevo al catálogo
MYSQL_RES* alta_producto(MYSQL *conexion, int userID, int categoriaID, const QString &nombre, const QString &descripcion, int stock, int precio)
{
    std::string sql = "CALL schema_comercial.sp_alta_producto(" + std::to_string(userID) + ", "
                      + std::to_string(categoriaID) + ", '"
                      + texto_sql(conexion, nombre) + "', '"
                      + texto_sql(conexion, descripcion) + "', "
                      + std::to_string(stock) + ", "
                      + std::to_string(precio) + ");";
    return ejecutar_consulta(conexion, sql);
}

// Cambia el stock de un producto
MYSQL_RES* modificar_stock(MYSQL *conexion, int userID, int productoID, int stock)
{
    std::string sql = "CALL schema_comercial.sp_modificar_stock(" + std::to_string(userID) + ", "
                      + std::to_string(productoID) + ", " + std::to_string(stock) + ");";
    return ejecutar_consulta(conexion, sql);
}

// Cambia el precio de un producto
MYSQL_RES* modificar_precio(MYSQL *conexion, int userID, int productoID, int precio)
{
    std::string sql = "CALL schema_comercial.sp_modificar_precio(" + std::to_string(userID) + ", "
                      + std::to_string(productoID) + ", " + std::to_string(precio) + ");";
    return ejecutar_consulta(conexion, sql);
}

// Da de baja un producto (queda inactivo, no se borra)
MYSQL_RES* baja_producto(MYSQL *conexion, int userID, int productoID)
{
    std::string sql = "CALL schema_comercial.sp_baja_producto(" + std::to_string(userID) + ", "
                      + std::to_string(productoID) + ");";
    return ejecutar_consulta(conexion, sql);
}

// Crea la venta y devuelve su VentaID
MYSQL_RES* registrar_venta(MYSQL *conexion, int userID, int metodoPagoID)
{
    // EmpleadoID va en NULL: el pedido lo hace el alumno desde la app, no un cajero
    std::string sql = "CALL schema_comercial.sp_registrar_venta(" + std::to_string(userID) + ", NULL, "
                      + std::to_string(metodoPagoID) + ");";
    return ejecutar_consulta(conexion, sql);
}

// Agrega un producto a la venta y descuenta el stock
MYSQL_RES* agregar_producto_carrito(MYSQL *conexion, int ventaID, int productoID, int cantidad)
{
    std::string sql = "CALL schema_comercial.sp_agregar_producto_carrito(" + std::to_string(ventaID) + ", "
                      + std::to_string(productoID) + ", " + std::to_string(cantidad) + ");";
    return ejecutar_consulta(conexion, sql);
}

// Devuelve el menú fijo del día de la semana (Lunes a Viernes)
MYSQL_RES* obtener_menu_del_dia(MYSQL *conexion, const QString &dia)
{
    std::string sql = "CALL schema_comercial.sp_menu_del_dia('" + texto_sql(conexion, dia) + "');";
    return ejecutar_consulta(conexion, sql);
}

// Agrega el menú del día a la venta y descuenta su stock
MYSQL_RES* agregar_menu_carrito(MYSQL *conexion, int ventaID, int menuID, int cantidad, const QString &dia)
{
    std::string sql = "CALL schema_comercial.sp_agregar_menu_carrito(" + std::to_string(ventaID) + ", "
                      + std::to_string(menuID) + ", " + std::to_string(cantidad) + ", '"
                      + texto_sql(conexion, dia) + "');";
    return ejecutar_consulta(conexion, sql);
}

// Crea el retiro con el código y el recreo en que se retira
MYSQL_RES* generar_retiro(MYSQL *conexion, int ventaID, int franjaID, const QString &codigo)
{
    std::string sql = "CALL schema_comercial.sp_generar_retiro(" + std::to_string(ventaID) + ", "
                      + std::to_string(franjaID) + ", '" + texto_sql(conexion, codigo) + "');";
    return ejecutar_consulta(conexion, sql);
}

// Marca un pedido como ENTREGADO (2) o CANCELADO (3)
MYSQL_RES* actualizar_estado_retiro(MYSQL *conexion, int userID, int retiroID, int estadoID)
{
    std::string sql = "CALL schema_comercial.sp_actualizar_estado_retiro(" + std::to_string(userID) + ", "
                      + std::to_string(retiroID) + ", " + std::to_string(estadoID) + ");";
    return ejecutar_consulta(conexion, sql);
}

// Lista de todos los productos con su categoría
// Columnas: 0 ID, 1 nombre, 2 categoría, 3 precio, 4 stock, 5 Activo/Inactivo, 6 descripción
// (la tabla de administración muestra de la 0 a la 5; el catálogo usa también la 6)
MYSQL_RES* obtener_productos(MYSQL *conexion)
{
    std::string sql = "SELECT p.ProductoID, p.Nombre_Producto, c.Categoria, p.Precio_Producto, p.Stock_Producto, "
                      "IF(p.Activo, 'Activo', 'Inactivo'), p.Descripcion_Producto "
                      "FROM schema_comercial.Producto p "
                      "INNER JOIN schema_comercial.Categoria c ON p.ID_Categoria = c.ID_Categoria "
                      "ORDER BY p.ProductoID;";
    return ejecutar_consulta(conexion, sql);
}

// Lista de todos los pedidos (uno por retiro)
MYSQL_RES* obtener_pedidos(MYSQL *conexion)
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
    return ejecutar_consulta(conexion, sql);
}

// Los últimos 50 movimientos de Log_Auditoria
MYSQL_RES* obtener_logs(MYSQL *conexion)
{
    std::string sql = "SELECT u.Apodo, t.Nombre_Evento, l.Detalle "
                      "FROM schema_usuarios.Log_Auditoria l "
                      "INNER JOIN schema_usuarios.Tipo_Evento t ON l.Tipo_Evento_ID = t.Tipo_Evento_ID "
                      "LEFT JOIN schema_usuarios.Usuario u ON l.UserID = u.UserID "
                      "ORDER BY l.Log_ID DESC LIMIT 50;";
    return ejecutar_consulta(conexion, sql);
}
