#include "sistemabuffet.h"
#include "basedatos.h"
#include <QDate>
#include <QStringList>

// =====================================================================
// sistemabuffet.cpp
// Qué hay en este archivo:
//   1. Valores iniciales de las variables compartidas (todas vacías)
//   2. Búsquedas en las listas
//   3. Funciones del carrito
//   4. Lectura de la BDD: usuario, productos, pedidos y menú del día
// Ningún dato está escrito acá: todo sale de MySQL.
// =====================================================================

// Cantidad de productos que muestra el catálogo (mainwindow.ui tiene 20 tarjetas fijas)
const int PRODUCTOS_DEL_CATALOGO = 20;


// =====================================================================
// 1. VALORES INICIALES
// Todo empieza vacío. Se llena cuando el usuario inicia sesión y cuando
// se abre cada ventana (cargarDesdeBDD).
// =====================================================================

// Conexión con MySQL: empieza en NULL y main.cpp la abre al arrancar
MYSQL *SistemaBuffet::conexion = NULL;

// Usuario activo: {UserID, rol, nombre, apellido, curso, DNI, saldo}. Lo completa cargarUsuarioDesdeBDD()
UsuarioActivo SistemaBuffet::usuario = {0, "", "", "", "", "", 0.0};

// Menú de hoy: {id, plato, incluye, precio}. Lo completa cargarMenuDelDia()
MenuDelDia SistemaBuffet::menuHoy = {0, "", "", 0.0};

// Listas: empiezan vacías
QList<Producto> SistemaBuffet::productos;
QList<Pedido> SistemaBuffet::pedidos;
QList<ItemCarrito> SistemaBuffet::carrito;


// =====================================================================
// 2. BÚSQUEDAS
// Recorren la lista hasta encontrar el dato. Devuelven la posición,
// o -1 si no lo encontraron.
// =====================================================================

// Busca un pedido por su código de retiro (lo usa la caja y el pago)
int SistemaBuffet::buscarPedido(QString codigo)
{
    for (int i = 0; i < pedidos.size(); i++) {
        if (pedidos[i].codigo == codigo) {
            return i;
        }
    }
    return -1;
}

// Busca un producto dentro del carrito por su ProductoID
int SistemaBuffet::buscarEnCarrito(int id)
{
    for (int i = 0; i < carrito.size(); i++) {
        if (carrito[i].id == id) {
            return i;
        }
    }
    return -1;
}


// =====================================================================
// 3. FUNCIONES DEL CARRITO
// El carrito vive en memoria mientras el alumno elige. Recién al
// confirmar el pago se guarda en la tabla Carrito (pagodialog.cpp).
// =====================================================================

// Agrega un producto. Si ya estaba en el carrito, le suma la cantidad
void SistemaBuffet::agregarAlCarrito(int id, QString nombre, double precio, int cantidad)
{
    int posicion = buscarEnCarrito(id);

    if (posicion != -1) {
        // Ya estaba: solo se suma la cantidad
        carrito[posicion].cantidad = carrito[posicion].cantidad + cantidad;
    } else {
        // Es nuevo: se arma el struct y se agrega al final de la lista
        ItemCarrito nuevo;
        nuevo.id = id;
        nuevo.nombre = nombre;
        nuevo.precio = precio;
        nuevo.cantidad = cantidad;
        carrito.append(nuevo);
    }
}

// Deja el carrito vacío (después de pagar)
void SistemaBuffet::vaciarCarrito()
{
    carrito.clear();
}

// Suma precio x cantidad de cada producto
double SistemaBuffet::getTotalCarrito()
{
    double total = 0.0;
    for (int i = 0; i < carrito.size(); i++) {
        total = total + carrito[i].precio * carrito[i].cantidad;
    }
    return total;
}

// Cuenta cuántas unidades hay en total (2 alfajores + 1 agua = 3)
int SistemaBuffet::getCantidadTotalCarrito()
{
    int cantidadTotal = 0;
    for (int i = 0; i < carrito.size(); i++) {
        cantidadTotal = cantidadTotal + carrito[i].cantidad;
    }
    return cantidadTotal;
}

// Cuántas unidades de un producto ya están en el carrito (0 si no está)
int SistemaBuffet::getCantidadEnCarrito(int id)
{
    int posicion = buscarEnCarrito(id);

    if (posicion != -1) {
        return carrito[posicion].cantidad;
    } else {
        return 0;
    }
}


// =====================================================================
// 4. LECTURA DE LA BDD
// =====================================================================

// ---------------------------------------------------------------------
// cargarUsuarioDesdeBDD
// Lee los datos del usuario que inició sesión (consulta
// obtener_datos_usuario en basedatos.cpp) y los guarda en "usuario".
// Devuelve true si lo encontró.
// ---------------------------------------------------------------------
bool SistemaBuffet::cargarUsuarioDesdeBDD(int userID)
{
    // Columnas: 0 rol, 1 nombre, 2 apellido, 3 curso, 4 DNI, 5 saldo
    QStringList datos = leer_primera_fila(obtener_datos_usuario(conexion, userID));

    if (datos.size() == 0) {
        return false;
    }

    usuario.id = userID;
    usuario.rol = datos[0];
    usuario.nombre = datos[1];
    usuario.apellido = datos[2];
    usuario.curso = datos[3];
    usuario.dni = datos[4];
    usuario.saldo = datos[5].toDouble();
    return true;
}

// ---------------------------------------------------------------------
// cargarProductos
// Vuelve a armar la lista "productos" con la tabla Producto.
// La posición en la lista es el ProductoID, así productos[16] es
// siempre el Alfajor de Maicena. La posición 0 queda para el menú del día.
// ---------------------------------------------------------------------
void SistemaBuffet::cargarProductos()
{
    // ----- Paso 1: lista vacía con el lugar 0 para el menú del día -----
    productos.clear();
    Producto menu;
    menu.id = 0;
    menu.nombre = "Menú del día";
    menu.precio = 0;
    menu.stock = 0;
    productos.append(menu);

    // ----- Paso 2: un struct por cada fila de la tabla Producto -----
    // Columnas: 0 ID, 1 nombre, 2 categoría, 3 precio, 4 stock, 5 Activo/Inactivo, 6 descripción
    MYSQL_RES *resultado = obtener_productos(conexion);
    if (resultado != NULL) {
        MYSQL_ROW fila = mysql_fetch_row(resultado);
        while (fila != NULL) {
            int productoID = valor_fila(fila, 0).toInt();

            // Se agranda la lista hasta tener lugar para este ProductoID
            while (productos.size() <= productoID) {
                Producto vacio;
                vacio.id = productos.size();
                vacio.precio = 0;
                vacio.stock = 0;
                productos.append(vacio);
            }

            productos[productoID].id = productoID;
            productos[productoID].nombre = valor_fila(fila, 1);
            productos[productoID].categoria = valor_fila(fila, 2);
            productos[productoID].precio = valor_fila(fila, 3).toDouble();
            productos[productoID].stock = valor_fila(fila, 4).toInt();
            productos[productoID].descripcion = valor_fila(fila, 6);

            // Un producto dado de baja no se puede vender: se muestra sin stock
            if (valor_fila(fila, 5) == "Inactivo") {
                productos[productoID].stock = 0;
            }

            fila = mysql_fetch_row(resultado);
        }
        mysql_free_result(resultado);
    }

    // ----- Paso 3: el catálogo tiene 20 tarjetas fijas -----
    // Si en la BDD hubiera menos productos, se completan vacíos (sin stock)
    // para que el catálogo no lea una posición que no existe
    while (productos.size() <= PRODUCTOS_DEL_CATALOGO) {
        Producto vacio;
        vacio.id = productos.size();
        vacio.precio = 0;
        vacio.stock = 0;
        productos.append(vacio);
    }
}

// ---------------------------------------------------------------------
// cargarPedidos
// Vuelve a armar la lista "pedidos": un struct por cada Retiro, con su
// Venta y el detalle del Carrito (consulta obtener_pedidos).
// ---------------------------------------------------------------------
void SistemaBuffet::cargarPedidos()
{
    // Columnas: 0 RetiroID, 1 código, 2 cliente, 3 curso, 4 DNI, 5 recreo,
    //           6 método de pago, 7 monto, 8 estado, 9 detalle
    MYSQL_RES *resultado = obtener_pedidos(conexion);
    if (resultado == NULL) {
        return;
    }

    pedidos.clear();
    MYSQL_ROW fila = mysql_fetch_row(resultado);
    while (fila != NULL) {
        Pedido pedido;
        pedido.retiroID = valor_fila(fila, 0).toInt();
        pedido.codigo = valor_fila(fila, 1);
        pedido.cliente = valor_fila(fila, 2);
        pedido.curso = valor_fila(fila, 3);
        pedido.dni = valor_fila(fila, 4);
        pedido.recreo = valor_fila(fila, 5);
        pedido.metodo = valor_fila(fila, 6);
        pedido.monto = valor_fila(fila, 7).toDouble();
        pedido.estado = valor_fila(fila, 8);
        pedido.detalle = valor_fila(fila, 9);
        pedidos.append(pedido);

        fila = mysql_fetch_row(resultado);
    }
    mysql_free_result(resultado);
}

// ---------------------------------------------------------------------
// cargarMenuDelDia
// La app calcula qué día es y se lo pasa a sp_menu_del_dia.
// El SP devuelve el menú fijo de ese día, o nada si es sábado o domingo.
// ---------------------------------------------------------------------
void SistemaBuffet::cargarMenuDelDia()
{
    // ----- Paso 1: se arranca sin menú (así queda el fin de semana) -----
    menuHoy.id = 0;
    menuHoy.plato = "";
    menuHoy.incluye = "";
    menuHoy.precio = 0;
    productos[0].stock = 0;

    // ----- Paso 2: nombre del día de hoy -----
    // dayOfWeek() da 1 para el lunes y 7 para el domingo
    QStringList nombresDias = {"Lunes", "Martes", "Miércoles", "Jueves", "Viernes", "Sábado", "Domingo"};
    QString hoy = nombresDias[QDate::currentDate().dayOfWeek() - 1];

    // ----- Paso 3: se le pide el menú a la BDD -----
    // Columnas: 0 ID, 1 plato, 2 precio, 3 stock, 4 día, 5 producto incluido
    QStringList datos = leer_primera_fila(obtener_menu_del_dia(conexion, hoy));
    if (datos.size() > 0 && datos[0] != "ERROR") {
        menuHoy.id = datos[0].toInt();
        menuHoy.plato = datos[1];
        menuHoy.precio = datos[2].toDouble();
        productos[0].stock = datos[3].toInt();
        menuHoy.incluye = datos[5];
    }
}

// ---------------------------------------------------------------------
// cargarDesdeBDD
// Trae todo de nuevo: productos, pedidos y menú. Se llama al abrir el
// catálogo, la caja o el perfil, y después de cada compra, para que
// la pantalla muestre siempre lo que está guardado en MySQL.
// El orden importa: el menú usa productos[0], que crea cargarProductos.
// ---------------------------------------------------------------------
void SistemaBuffet::cargarDesdeBDD()
{
    cargarProductos();
    cargarPedidos();
    cargarMenuDelDia();
}

// ---------------------------------------------------------------------
// cerrarSesion: sp_cerrar_sesion deja el cierre en Log_Auditoria
// ---------------------------------------------------------------------
void SistemaBuffet::cerrarSesion()
{
    leer_primera_fila(cerrar_sesion(conexion, usuario.id));
}
