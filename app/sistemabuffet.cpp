#include "sistemabuffet.h"
#include "basedatos.h"
#include <QDate>
#include <QStringList>

// =====================================================================
// VALORES INICIALES DE LAS VARIABLES COMPARTIDAS
// Son los mismos datos que carga 04_DatosSQL en la BDD. Se usan cuando
// no hay conexión con MySQL (modo "datos de ejemplo").
// Cada {...} es un struct, con los campos en el mismo orden que en
// sistemabuffet.h
// =====================================================================

// Conexión con MySQL: empieza en NULL y main.cpp la abre al arrancar
MYSQL *SistemaBuffet::conexion = NULL;
bool SistemaBuffet::datosInicializados = false;

// Usuario activo: {UserID, rol, nombre, apellido, curso, DNI, saldo}. Empieza con un estudiante de prueba
UsuarioActivo SistemaBuffet::usuario = {1, "Estudiante", "David", "Melendez", "6° Año 1ra", "12345678", 10000.0};

// Menú de hoy: {id, plato, incluye, precio}. Lo completa cargarMenuDelDia()
MenuDelDia SistemaBuffet::menuHoy = {0, "", "", 0};

// Carrito y pedidos: empiezan vacíos
QList<ItemCarrito> SistemaBuffet::carrito;
QList<Pedido> SistemaBuffet::pedidos;

// Estudiante (04_DatosSQL): {dni, nombre, apellido, curso}
// Los 2 últimos no tienen usuario y sirven para probar el registro
QList<Estudiante> SistemaBuffet::estudiantes = {
    {"12345678", "David", "Melendez", "6° Año 1ra"},
    {"45987654", "Lucía", "Gómez", "6° Año 2da"},
    {"48499250", "Mateo", "Fernández", "3° Año 3ra"},
    {"51715407", "Valentina", "Rodríguez", "1° Año 4ta"},
    {"52929091", "Santiago", "López", "5° Año 1ra"},
    {"49373046", "Camila", "Martínez", "2° Año 2da"},
    {"52303055", "Benjamín", "García", "1° Año 3ra"},
    {"52420093", "Martina", "Pérez", "1° Año 4ta"},
    {"49512712", "Thiago", "Sánchez", "6° Año 1ra"},
    {"52652318", "Sofía", "Romero", "5° Año 2da"},
    {"48428925", "Joaquín", "Díaz", "3° Año 3ra"},
    {"48322516", "Julieta", "Álvarez", "1° Año 4ta"},
    {"47596375", "Bautista", "Torres", "1° Año 1ra"},
    {"53199127", "Agustina", "Ruiz", "3° Año 2da"},
    {"49263492", "Lautaro", "Acosta", "2° Año 3ra"},
    {"49609844", "Micaela", "Benítez", "2° Año 4ta"},
    {"50491680", "Facundo", "Medina", "2° Año 1ra"},
    {"51808284", "Florencia", "Herrera", "5° Año 2da"},
    {"50355436", "Nicolás", "Suárez", "5° Año 3ra"},
    {"52176234", "Rocío", "Aguirre", "3° Año 4ta"},
    {"95671088", "Gabriel", "Noguera", "4° Año 2da"},
    {"50283716", "Tomás", "San Martín", "4° Año 2da"}
};

// Profesor (04_DatosSQL): {dni, nombre, apellido}. Ninguno tiene usuario todavía
QList<Profesor> SistemaBuffet::profesores = {
    {"25418763", "Pablo", "Linares"},
    {"27904512", "Cynthya", "Berea"},
    {"29387104", "Cristian", "Dávalos"},
    {"26150938", "Gabriela", "Tamer"},
    {"23716485", "Sergio", "Ipolo"},
    {"22058391", "Julio", "Toni"},
    {"33842017", "Facundo", "Scally"},
    {"30217649", "Mariana", "Milano"},
    {"20574836", "Norma", "Aloma"},
    {"21863270", "Néstor", "Quiñones"},
    {"28640195", "Miguel", "Riños"},
    {"34129057", "Victoria", "Erramuspe"},
    {"31406782", "Cecilia", "Garabato"},
    {"32791546", "Andrés", "Diz"},
    {"35268014", "Josefina", "Maldonado"},
    {"33015479", "Yamil", "Decastelli"},
    {"36482910", "Luz", "Retamazo"},
    {"31957320", "Bárbara", "Saldaño"},
    {"29801643", "Natalia", "Guerrero"},
    {"24360871", "Rossana", "Fortunatti"},
    {"27512398", "Francisco", "Anselmi"}
};

// Usuario (04_DatosSQL): {dni, apodo, contraseña, rol, saldo}. La posición + 1 es el UserID
QList<Usuario> SistemaBuffet::usuarios = {
    {"12345678", "davidmel09", "Davidmel#2026", "Estudiante", 10000},
    {"45987654", "luciagom08", "Luciagom#2026", "Estudiante", 0},
    {"48499250", "mateofer11", "Mateofer#2026", "Estudiante", 0},
    {"51715407", "valentinarod13", "Valentinarod#2026", "Estudiante", 0},
    {"52929091", "santiagolop09", "Santiagolop#2026", "Estudiante", 0},
    {"49373046", "camilamar12", "Camilamar#2026", "Estudiante", 0},
    {"52303055", "benjamingar13", "Benjamingar#2026", "Estudiante", 0},
    {"52420093", "martinaper13", "Martinaper#2026", "Estudiante", 0},
    {"49512712", "thiagosan08", "Thiagosan#2026", "Estudiante", 0},
    {"52652318", "sofiarom09", "Sofiarom#2026", "Estudiante", 0},
    {"48428925", "joaquindia11", "Joaquindia#2026", "Estudiante", 0},
    {"48322516", "julietaalv13", "Julietaalv#2026", "Estudiante", 0},
    {"47596375", "bautistator13", "Bautistator#2026", "Estudiante", 0},
    {"53199127", "agustinarui11", "Agustinarui#2026", "Estudiante", 0},
    {"49263492", "lautaroaco12", "Lautaroaco#2026", "Estudiante", 0},
    {"49609844", "micaelaben12", "Micaelaben#2026", "Estudiante", 0},
    {"50491680", "facundomed12", "Facundomed#2026", "Estudiante", 0},
    {"51808284", "florenciaher09", "Florenciaher#2026", "Estudiante", 0},
    {"50355436", "nicolassua09", "Nicolassua#2026", "Estudiante", 0},
    {"52176234", "rocioagu11", "Rocioagu#2026", "Estudiante", 0},
    {"", "marcela.ibanez", "MarcelaBuffet#2026", "Personal de caja", 0},
    {"", "diego.paz", "DiegoBuffet#2026", "Personal de caja", 0},
    {"", "karina.luna", "KarinaBuffet#2026", "Personal de caja", 0},
    {"", "sergio.ledesma", "SergioBuffet#2026", "Personal de caja", 0},
    {"", "paola.vega", "PaolaBuffet#2026", "Personal de caja", 0},
    {"", "hector.maldonado", "HectorBuffet#2026", "Personal de caja", 0},
    {"", "alejandro.funes", "AlejandroBuffet#2026", "Administración", 0},
    {"", "silvia.arce", "SilviaBuffet#2026", "Personal de caja", 0},
    {"", "ramiro.sosa", "RamiroBuffet#2026", "Personal de caja", 0},
    {"", "veronica.campos", "VeronicaBuffet#2026", "Administración", 0},
    {"", "soporte.dev", "SoporteDev#2026", "Desarrollador", 0}
};

// Empleado con usuario (04_DatosSQL): {apodo, nombre, puesto, PIN, token}
QList<Empleado> SistemaBuffet::empleados = {
    {"marcela.ibanez", "Marcela Ibáñez", "Cajera Turno Mañana", "1234", ""},
    {"diego.paz", "Diego Paz", "Cajero Turno Mañana", "2580", ""},
    {"karina.luna", "Karina Luna", "Cajera Turno Mañana", "3691", ""},
    {"sergio.ledesma", "Sergio Ledesma", "Cajero Turno Tarde", "4702", ""},
    {"paola.vega", "Paola Vega", "Cajera Turno Tarde", "5813", ""},
    {"hector.maldonado", "Héctor Maldonado", "Cajero Turno Tarde", "6924", ""},
    {"alejandro.funes", "Alejandro Funes", "Encargado General de Buffet", "", ""},
    {"silvia.arce", "Silvia Arce", "Atención al Mostrador Turno Mañana", "7035", ""},
    {"ramiro.sosa", "Ramiro Sosa", "Atención al Mostrador Turno Tarde", "8146", ""},
    {"veronica.campos", "Verónica Campos", "Administrativa Contable", "", ""},
    {"soporte.dev", "Soporte Técnico", "Desarrollador de Sistemas", "", "DEV-BEREA-2026"}
};

// Producto (04_DatosSQL): {id, nombre, descripción, categoría, precio, stock}
// La posición es el ProductoID. La posición 0 no es un producto: es el menú del día
QList<Producto> SistemaBuffet::productos = {
    {0, "Menú del día", "", "", 0, 0},
    {1, "Sándwich de Milanesa", "Milanesa de carne en pan francés con lechuga y tomate", "Sándwiches y Tostados", 2800, 40},
    {2, "Empanada de Carne", "Empanada al horno rellena de carne cortada a cuchillo", "Empanadas y Tartas", 1100, 80},
    {3, "Porción de Pizza Muzzarella", "Porción de pizza de muzzarella", "Pizzas", 1800, 50},
    {4, "Pebete de Queso", "Pebete con queso cremoso", "Panificados", 1600, 45},
    {5, "Medialuna de Manteca", "Medialuna dulce de manteca", "Medialunas y Facturas", 800, 90},
    {6, "Ensalada Mixta", "Lechuga, tomate y zanahoria", "Ensaladas", 2200, 25},
    {7, "Manzana", "Manzana roja por unidad", "Frutas", 700, 70},
    {8, "Chocolatada 250 ml", "Leche chocolatada en envase individual", "Lácteos", 1100, 70},
    {9, "Yogur Bebible de Frutilla", "Yogur bebible 200 ml", "Yogures y Postres", 1200, 60},
    {10, "Agua Mineral 500 ml", "Agua mineral sin gas", "Aguas", 1000, 150},
    {11, "Jugo en Caja 200 ml", "Jugo de frutas en envase individual", "Jugos", 900, 100},
    {12, "Gaseosa 500ml", "Gaseosa línea cola 500 ml", "Gaseosas", 1300, 100},
    {13, "Café con Leche", "Vaso de café con leche", "Infusiones Calientes", 1400, 60},
    {14, "Barra de Cereal", "Barra de cereal con frutas", "Cereales y Barras", 900, 80},
    {15, "Galletitas de Agua", "Paquete individual de galletitas de agua", "Galletitas", 700, 80},
    {16, "Alfajor de Maicena", "Alfajor de maicena con dulce de leche y coco", "Alfajores", 800, 70},
    {17, "Turrón de Maní", "Turrón de maní individual", "Golosinas", 600, 60},
    {18, "Mix de Frutos Secos", "Mix de almendras, pasas y maní sin sal", "Snacks Saludables", 1500, 40},
    {19, "Milanesa de Pollo con Puré", "Milanesa al horno con puré de papas", "Platos Calientes", 4200, 25},
    {20, "Combo Sándwich + Gaseosa", "Sándwich de milanesa + gaseosa 500 ml", "Combos Escolares", 4100, 40}
};

// Menú semanal fijo: los 5 primeros de Menu_Del_Dia (04_DatosSQL): {día, plato, precio, stock, incluye}
// Las semanas siguientes de la BDD repiten estos mismos platos
QList<MenuSemanal> SistemaBuffet::menusSemana = {
    {"Miércoles", "Milanesa de pollo con puré", 4200, 40, "Agua Mineral 500 ml"},
    {"Jueves", "Fideos con tuco y queso", 3800, 35, "Jugo en Caja 200 ml"},
    {"Viernes", "Ravioles con salsa fileto", 4300, 35, "Jugo en Caja 200 ml"},
    {"Lunes", "Tarta de choclo con ensalada", 3900, 30, "Manzana"},
    {"Martes", "Tortilla de papas con ensalada", 3900, 40, "Agua Mineral 500 ml"}
};


// =====================================================================
// BÚSQUEDAS
// Recorren la lista hasta encontrar el dato. Devuelven la posición,
// o -1 si no lo encontraron.
// =====================================================================

int SistemaBuffet::buscarUsuario(QString apodo)
{
    for (int i = 0; i < usuarios.size(); i++) {
        if (usuarios[i].apodo == apodo) {
            return i;
        }
    }
    return -1;
}

int SistemaBuffet::buscarEmpleado(QString apodo)
{
    for (int i = 0; i < empleados.size(); i++) {
        if (empleados[i].apodo == apodo) {
            return i;
        }
    }
    return -1;
}

int SistemaBuffet::buscarEstudiante(QString dni)
{
    for (int i = 0; i < estudiantes.size(); i++) {
        if (estudiantes[i].dni == dni) {
            return i;
        }
    }
    return -1;
}

int SistemaBuffet::buscarProfesor(QString dni)
{
    for (int i = 0; i < profesores.size(); i++) {
        if (profesores[i].dni == dni) {
            return i;
        }
    }
    return -1;
}

// Busca si un DNI ya tiene cuenta de usuario
int SistemaBuffet::buscarDniRegistrado(QString dni)
{
    for (int i = 0; i < usuarios.size(); i++) {
        if (usuarios[i].dni == dni) {
            return i;
        }
    }
    return -1;
}

int SistemaBuffet::buscarPedido(QString codigo)
{
    for (int i = 0; i < pedidos.size(); i++) {
        if (pedidos[i].codigo == codigo) {
            return i;
        }
    }
    return -1;
}

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
// FUNCIONES DEL CARRITO
// =====================================================================

// Agrega un producto. Si ya estaba en el carrito, le suma la cantidad
void SistemaBuffet::agregarAlCarrito(int id, QString nombre, double precio, int cantidad)
{
    int posicion = buscarEnCarrito(id);

    if (posicion != -1) {
        carrito[posicion].cantidad = carrito[posicion].cantidad + cantidad;
    } else {
        ItemCarrito nuevo;
        nuevo.id = id;
        nuevo.nombre = nombre;
        nuevo.precio = precio;
        nuevo.cantidad = cantidad;
        carrito.append(nuevo);
    }
}

// Deja el carrito vacío
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
// inicializarDatos (MODO SIN BDD)
// Carga dos pedidos de ejemplo y el menú de hoy. Se hace una sola vez,
// aunque se vuelva a la pantalla de login.
// =====================================================================
void SistemaBuffet::inicializarDatos()
{
    if (datosInicializados) {
        return;
    }
    datosInicializados = true;

    Pedido ejemplo1;
    ejemplo1.retiroID = 0;
    ejemplo1.codigo = "3C6MB";
    ejemplo1.cliente = "David Melendez";
    ejemplo1.curso = "6° Año 1ra";
    ejemplo1.dni = "12345678";
    ejemplo1.recreo = "2do Recreo TM (10:35 a 10:45 hs)";
    ejemplo1.metodo = "Efectivo en Mostrador";
    ejemplo1.monto = 4100.0;
    ejemplo1.abonado = 5000.0;
    ejemplo1.vuelto = 900.0;
    ejemplo1.estado = "PENDIENTE";
    ejemplo1.detalle = "1x Sándwich de Milanesa ($2800)\n1x Gaseosa 500ml ($1300)";
    pedidos.append(ejemplo1);

    Pedido ejemplo2;
    ejemplo2.retiroID = 0;
    ejemplo2.codigo = "8A2KP";
    ejemplo2.cliente = "Lucía Gómez";
    ejemplo2.curso = "6° Año 2da";
    ejemplo2.dni = "45987654";
    ejemplo2.recreo = "Receso Inter-Turno - Almuerzo (12:05 a 13:30 hs)";
    ejemplo2.metodo = "Saldo de Cuenta Escolar";
    ejemplo2.monto = 2800.0;
    ejemplo2.abonado = 2800.0;
    ejemplo2.vuelto = 0.0;
    ejemplo2.estado = "PENDIENTE";
    ejemplo2.detalle = "1x Sándwich de Milanesa ($2800)";
    pedidos.append(ejemplo2);

    cargarMenuDelDia();
}

// =====================================================================
// cargarUsuarioActivo (MODO SIN BDD)
// Copia los datos del usuario que inició sesión al struct "usuario".
// posicionUsuario es su posición en la lista "usuarios".
// =====================================================================
void SistemaBuffet::cargarUsuarioActivo(int posicionUsuario)
{
    usuario.id = posicionUsuario + 1;
    usuario.rol = usuarios[posicionUsuario].rol;
    usuario.dni = usuarios[posicionUsuario].dni;
    // shortcut: el saldo se copia al entrar y no se guarda al salir; con BDD se lee siempre de Usuario.Saldo
    usuario.saldo = usuarios[posicionUsuario].saldo;

    // El nombre y el curso se buscan por DNI: primero entre los estudiantes
    int posicionEstudiante = buscarEstudiante(usuario.dni);
    if (posicionEstudiante != -1) {
        usuario.nombre = estudiantes[posicionEstudiante].nombre;
        usuario.apellido = estudiantes[posicionEstudiante].apellido;
        usuario.curso = estudiantes[posicionEstudiante].curso;
    } else {
        // Si no es estudiante, es profesor (no tiene curso)
        int posicionProfesor = buscarProfesor(usuario.dni);
        usuario.nombre = profesores[posicionProfesor].nombre;
        usuario.apellido = profesores[posicionProfesor].apellido;
        usuario.curso = "Docente";
    }
}

// =====================================================================
// cargarUsuarioDesdeBDD (MODO CON BDD)
// Lee de la BDD los datos del usuario que inició sesión.
// Devuelve true si lo encontró.
// =====================================================================
bool SistemaBuffet::cargarUsuarioDesdeBDD(int userID)
{
    // Columnas: rol, nombre, apellido, curso, DNI, saldo
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

// =====================================================================
// cargarMenuDelDia
// Busca qué menú corresponde hoy y lo guarda en "menuHoy".
// El stock del menú se guarda en productos[0].stock
// =====================================================================
void SistemaBuffet::cargarMenuDelDia()
{
    // Se arranca sin menú (así queda el sábado y el domingo)
    menuHoy.id = 0;
    menuHoy.plato = "";
    menuHoy.incluye = "";
    menuHoy.precio = 0;
    productos[0].stock = 0;

    // Nombre del día de hoy. dayOfWeek() da 1 para el lunes y 7 para el domingo
    QStringList nombresDias = {"Lunes", "Martes", "Miércoles", "Jueves", "Viernes", "Sábado", "Domingo"};
    QString hoy = nombresDias[QDate::currentDate().dayOfWeek() - 1];

    if (conexion != NULL) {
        // ----- MODO CON BDD -----
        // sp_menu_del_dia devuelve: ID, plato, precio, stock, día, producto incluido
        QStringList datos = leer_primera_fila(obtener_menu_del_dia(conexion, hoy));
        if (datos.size() > 0 && datos[0] != "ERROR") {
            menuHoy.id = datos[0].toInt();
            menuHoy.plato = datos[1];
            menuHoy.precio = datos[2].toDouble();
            productos[0].stock = datos[3].toInt();
            menuHoy.incluye = datos[5];
        }
    } else {
        // ----- MODO SIN BDD -----
        // Se busca el menú fijo de ese día (sábado y domingo no están)
        for (int i = 0; i < menusSemana.size(); i++) {
            if (menusSemana[i].dia == hoy) {
                menuHoy.id = i + 1;
                menuHoy.plato = menusSemana[i].plato;
                menuHoy.incluye = menusSemana[i].incluye;
                menuHoy.precio = menusSemana[i].precio;
                productos[0].stock = menusSemana[i].stock;
            }
        }
    }
}

// =====================================================================
// cargarDesdeBDD (MODO CON BDD)
// Trae de nuevo de la BDD los productos, los pedidos y el menú.
// Sin conexión no hace nada: quedan los datos de ejemplo.
// =====================================================================
void SistemaBuffet::cargarDesdeBDD()
{
    if (conexion == NULL) {
        return;
    }

    // ----- 1. Productos -----
    // Columnas: ID, nombre, categoría, precio, stock, Activo/Inactivo
    MYSQL_RES *resultado = obtener_productos(conexion);
    if (resultado != NULL) {
        MYSQL_ROW fila = mysql_fetch_row(resultado);
        while (fila != NULL) {
            int productoID = valor_fila(fila, 0).toInt();

            // Si el admin agregó productos nuevos, la lista se agranda
            while (productos.size() <= productoID) {
                Producto nuevo;
                nuevo.id = productos.size();
                nuevo.precio = 0;
                nuevo.stock = 0;
                productos.append(nuevo);
            }

            // Nombre y categoría solo para los productos nuevos (los demás ya los tienen)
            if (productos[productoID].nombre == "") {
                productos[productoID].nombre = valor_fila(fila, 1);
                productos[productoID].categoria = valor_fila(fila, 2);
            }
            productos[productoID].precio = valor_fila(fila, 3).toDouble();
            productos[productoID].stock = valor_fila(fila, 4).toInt();

            // Un producto dado de baja no se puede vender: se muestra sin stock
            if (valor_fila(fila, 5) == "Inactivo") {
                productos[productoID].stock = 0;
            }

            fila = mysql_fetch_row(resultado);
        }
        mysql_free_result(resultado);
    }

    // ----- 2. Pedidos: se borra la lista y se vuelve a llenar -----
    // Columnas: RetiroID, código, cliente, curso, DNI, recreo, método, monto, estado, detalle
    resultado = obtener_pedidos(conexion);
    if (resultado != NULL) {
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
            // La BDD no guarda con cuánto pagó en efectivo: se toma el monto justo
            pedido.abonado = pedido.monto;
            pedido.vuelto = 0.0;
            pedido.estado = valor_fila(fila, 8);
            pedido.detalle = valor_fila(fila, 9);
            pedidos.append(pedido);

            fila = mysql_fetch_row(resultado);
        }
        mysql_free_result(resultado);
    }

    // ----- 3. Menú del día -----
    cargarMenuDelDia();
}

// =====================================================================
// cerrarSesion: deja el cierre de sesión en Log_Auditoria (solo con BDD)
// =====================================================================
void SistemaBuffet::cerrarSesion()
{
    if (conexion != NULL) {
        leer_primera_fila(cerrar_sesion(conexion, usuario.id));
    }
}
