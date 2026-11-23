// datos.cpp: Variables globales (usuario, productos, menú, carrito, pedidos) y las funciones que las cargan.

#include "datos.h"
#include "basedatos.h"
#include <QDate>

// ===== BLOQUE 1: VARIABLES GLOBALES (valores iniciales) =====

MYSQL *conexion = NULL;

int usuario_id = 0;
QString usuario_rol = "";
QString usuario_nombre = "";
QString usuario_apellido = "";
QString usuario_curso = "";
QString usuario_dni = "";
double usuario_saldo = 0;

QStringList productos_nombres;
QStringList productos_descripciones;
QList<double> productos_precios;
QList<int> productos_stock;

int menu_hoy_id = 0;
QString menu_hoy_plato = "";
QString menu_hoy_incluye = "";
double menu_hoy_precio = 0;
QString menu_hoy_dia = "";

QList<int> carrito_ids;
QStringList carrito_nombres;
QList<double> carrito_precios;
QList<int> carrito_cantidades;

QList<int> pedidos_retiro_ids;
QStringList pedidos_codigos;
QStringList pedidos_clientes;
QStringList pedidos_cursos;
QStringList pedidos_dnis;
QStringList pedidos_recreos;
QStringList pedidos_metodos;
QList<double> pedidos_montos;
QStringList pedidos_estados;
QStringList pedidos_detalles;

// ===== BLOQUE 2: CARGA DESDE LA BDD =====

// Sirve para traer de la BDD los datos del usuario que inició sesión.
bool cargar_usuario(int userID)
{
    MYSQL_RES *result = obtener_datos_usuario(conexion, userID);
    if (result == NULL) {
        return false;
    }
    MYSQL_ROW fila = mysql_fetch_row(result);
    bool encontrado = (fila != NULL);
    if (encontrado) {
        usuario_id = userID;
        usuario_rol = valor_fila(fila, 0);
        usuario_nombre = valor_fila(fila, 1);
        usuario_apellido = valor_fila(fila, 2);
        usuario_curso = valor_fila(fila, 3);
        usuario_dni = valor_fila(fila, 4);
        usuario_saldo = valor_fila(fila, 5).toDouble();
    }
    mysql_free_result(result);
    return encontrado;
}

// Sirve para cargar el catálogo.
void cargar_productos()
{
    productos_nombres.clear();
    productos_descripciones.clear();
    productos_precios.clear();
    productos_stock.clear();

    // Posición 0 = menú del día
    productos_nombres.append("");
    productos_descripciones.append("");
    productos_precios.append(0);
    productos_stock.append(0);

    MYSQL_RES *result = obtener_productos(conexion);
    if (result != NULL) {
        MYSQL_ROW fila;
        while ((fila = mysql_fetch_row(result)) != NULL) {
            int id = valor_fila(fila, 0).toInt();
            while (productos_stock.size() <= id) {
                productos_nombres.append("");
                productos_descripciones.append("");
                productos_precios.append(0);
                productos_stock.append(0);
            }
            productos_nombres[id] = valor_fila(fila, 1);
            productos_precios[id] = valor_fila(fila, 3).toDouble();
            productos_stock[id] = valor_fila(fila, 4).toInt();
            productos_descripciones[id] = valor_fila(fila, 6);
            if (valor_fila(fila, 5) == "Inactivo") {
                productos_stock[id] = 0;
            }
        }
        mysql_free_result(result);
    }

    cargar_menu_del_dia();
}

// Sirve para cargar el menú de hoy.
void cargar_menu_del_dia()
{
    menu_hoy_id = 0;
    menu_hoy_plato = "";
    menu_hoy_incluye = "";
    menu_hoy_precio = 0;
    productos_stock[0] = 0;

    // Qt calcula el día (1 = lunes ... 7 = domingo) y se lo pasa al SP
    QStringList dias = {"Lunes", "Martes", "Miércoles", "Jueves", "Viernes", "Sábado", "Domingo"};
    QString hoy = dias[QDate::currentDate().dayOfWeek() - 1];
    menu_hoy_dia = hoy;

    MYSQL_RES *result = obtener_menu_del_dia(conexion, hoy);
    if (result == NULL) {
        return;
    }
    MYSQL_ROW fila = mysql_fetch_row(result);
    if (error_del_sp(fila) != 0) {
        fila = NULL;
    }
    if (fila != NULL) {
        menu_hoy_id = valor_fila(fila, 0).toInt();
        menu_hoy_plato = valor_fila(fila, 1);
        menu_hoy_precio = valor_fila(fila, 2).toDouble();
        productos_stock[0] = valor_fila(fila, 3).toInt();
        menu_hoy_incluye = valor_fila(fila, 5);
        productos_nombres[0] = menu_hoy_plato;
        productos_precios[0] = menu_hoy_precio;
        productos_descripciones[0] = "Menú del día. Incluye: " + menu_hoy_incluye;
    }
    mysql_free_result(result);
}

// Sirve para traer todos los pedidos de la BDD.
void cargar_pedidos()
{
    pedidos_retiro_ids.clear();
    pedidos_codigos.clear();
    pedidos_clientes.clear();
    pedidos_cursos.clear();
    pedidos_dnis.clear();
    pedidos_recreos.clear();
    pedidos_metodos.clear();
    pedidos_montos.clear();
    pedidos_estados.clear();
    pedidos_detalles.clear();

    MYSQL_RES *result = obtener_pedidos(conexion);
    if (result == NULL) {
        return;
    }
    MYSQL_ROW fila;
    while ((fila = mysql_fetch_row(result)) != NULL) {
        pedidos_retiro_ids.append(valor_fila(fila, 0).toInt());
        pedidos_codigos.append(valor_fila(fila, 1));
        pedidos_clientes.append(valor_fila(fila, 2));
        pedidos_cursos.append(valor_fila(fila, 3));
        pedidos_dnis.append(valor_fila(fila, 4));
        pedidos_recreos.append(valor_fila(fila, 5));
        pedidos_metodos.append(valor_fila(fila, 6));
        pedidos_montos.append(valor_fila(fila, 7).toDouble());
        pedidos_estados.append(valor_fila(fila, 8));
        pedidos_detalles.append(valor_fila(fila, 9));
    }
    mysql_free_result(result);
}

// Sirve para encontrar un pedido por su código de retiro.
int buscar_pedido(const QString &codigo)
{
    for (int i = 0; i < pedidos_codigos.size(); i++) {
        if (pedidos_codigos[i] == codigo) {
            return i;
        }
    }
    return -1;
}

// Sirve para cerrar la sesión.
void salir_de_la_cuenta()
{
    MYSQL_RES *result = cerrar_sesion(conexion, usuario_id);
    if (result != NULL) {
        mysql_free_result(result);
    }
    vaciar_carrito();
}

// ===== BLOQUE 3: CARRITO (solo en memoria hasta que se paga) =====

// Sirve para agregar un producto al carrito.
void agregar_al_carrito(int id, const QString &nombre, double precio, int cantidad)
{
    for (int i = 0; i < carrito_ids.size(); i++) {
        if (carrito_ids[i] == id) {
            carrito_cantidades[i] += cantidad;
            return;
        }
    }
    carrito_ids.append(id);
    carrito_nombres.append(nombre);
    carrito_precios.append(precio);
    carrito_cantidades.append(cantidad);
}

// Sirve para borrar un renglón del carrito (la fila elegida en la tabla).
void quitar_del_carrito(int fila)
{
    carrito_ids.removeAt(fila);
    carrito_nombres.removeAt(fila);
    carrito_precios.removeAt(fila);
    carrito_cantidades.removeAt(fila);
}

// Sirve para dejar el carrito vacío (al pagar, al vaciarlo o al cerrar sesión).
void vaciar_carrito()
{
    carrito_ids.clear();
    carrito_nombres.clear();
    carrito_precios.clear();
    carrito_cantidades.clear();
}

// Sirve para calcular el total a pagar: suma precio x cantidad de cada renglón.
double total_carrito()
{
    double total = 0;
    for (int i = 0; i < carrito_precios.size(); i++) {
        total += carrito_precios[i] * carrito_cantidades[i];
    }
    return total;
}

// Sirve para contar cuántas unidades hay en total en el carrito.
int cantidad_total_carrito()
{
    int cantidad = 0;
    for (int i = 0; i < carrito_cantidades.size(); i++) {
        cantidad += carrito_cantidades[i];
    }
    return cantidad;
}

// Sirve para saber cuántas unidades de un producto ya están en el carrito.
int cantidad_en_carrito(int id)
{
    for (int i = 0; i < carrito_ids.size(); i++) {
        if (carrito_ids[i] == id) {
            return carrito_cantidades[i];
        }
    }
    return 0;
}

// ===== BLOQUE 4: FORMATO =====

// Sirve para mostrar un precio con el signo $ y sin decimales: 2800 -> "$2800".
QString pesos(double monto)
{
    return "$" + QString::number(monto, 'f', 0);
}
