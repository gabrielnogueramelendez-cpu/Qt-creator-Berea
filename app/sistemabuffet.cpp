#include "sistemabuffet.h"
#include "basedatos.h"
#include <QDate>

// Definición e inicialización de variables estáticas
MYSQL *SistemaBuffet::conn = NULL;
int SistemaBuffet::usuarioID = 1;
QString SistemaBuffet::usuarioRol = "Estudiante";
QString SistemaBuffet::usuarioNombre = "David";
QString SistemaBuffet::usuarioApellido = "Melendez";
QString SistemaBuffet::usuarioCurso = "6° Año 1ra";
QString SistemaBuffet::usuarioDni = "12345678";
double SistemaBuffet::usuarioSaldo = 10000.0;

// Estudiante (04_DatosSQL); los 2 últimos no tienen usuario y sirven para probar el registro. Con BDD: SELECT de Estudiante y Curso
QStringList SistemaBuffet::estudiantesDnis = {"12345678", "45987654", "48499250", "51715407", "52929091", "49373046", "52303055", "52420093", "49512712", "52652318", "48428925", "48322516", "47596375", "53199127", "49263492", "49609844", "50491680", "51808284", "50355436", "52176234", "95671088", "50283716"};
QStringList SistemaBuffet::estudiantesNombres = {"David", "Lucía", "Mateo", "Valentina", "Santiago", "Camila", "Benjamín", "Martina", "Thiago", "Sofía", "Joaquín", "Julieta", "Bautista", "Agustina", "Lautaro", "Micaela", "Facundo", "Florencia", "Nicolás", "Rocío", "Gabriel", "Tomás"};
QStringList SistemaBuffet::estudiantesApellidos = {"Melendez", "Gómez", "Fernández", "Rodríguez", "López", "Martínez", "García", "Pérez", "Sánchez", "Romero", "Díaz", "Álvarez", "Torres", "Ruiz", "Acosta", "Benítez", "Medina", "Herrera", "Suárez", "Aguirre", "Noguera", "San Martín"};
QStringList SistemaBuffet::estudiantesCursos = {"6° Año 1ra", "6° Año 2da", "3° Año 3ra", "1° Año 4ta", "5° Año 1ra", "2° Año 2da", "1° Año 3ra", "1° Año 4ta", "6° Año 1ra", "5° Año 2da", "3° Año 3ra", "1° Año 4ta", "1° Año 1ra", "3° Año 2da", "2° Año 3ra", "2° Año 4ta", "2° Año 1ra", "5° Año 2da", "5° Año 3ra", "3° Año 4ta", "4° Año 2da", "4° Año 2da"};

// Profesor (04_DatosSQL); ninguno tiene usuario todavía. Con BDD: SELECT de Profesor
QStringList SistemaBuffet::profesoresDnis = {"25418763", "27904512", "29387104", "26150938", "23716485", "22058391", "33842017", "30217649", "20574836", "21863270", "28640195", "34129057", "31406782", "32791546", "35268014", "33015479", "36482910", "31957320", "29801643", "24360871", "27512398"};
QStringList SistemaBuffet::profesoresNombres = {"Pablo", "Cynthya", "Cristian", "Gabriela", "Sergio", "Julio", "Facundo", "Mariana", "Norma", "Néstor", "Miguel", "Victoria", "Cecilia", "Andrés", "Josefina", "Yamil", "Luz", "Bárbara", "Natalia", "Rossana", "Francisco"};
QStringList SistemaBuffet::profesoresApellidos = {"Linares", "Berea", "Dávalos", "Tamer", "Ipolo", "Toni", "Scally", "Milano", "Aloma", "Quiñones", "Riños", "Erramuspe", "Garabato", "Diz", "Maldonado", "Decastelli", "Retamazo", "Saldaño", "Guerrero", "Fortunatti", "Anselmi"};

// Rol (04_DatosSQL)
QStringList SistemaBuffet::roles = {"Estudiante", "Profesor", "Personal de caja", "Administración", "Desarrollador"};

// Usuario (04_DatosSQL): la posición + 1 es el UserID. El personal no tiene DNI; el de un profesor es su DNI_Profesor
QStringList SistemaBuffet::usuariosDnis = {"12345678", "45987654", "48499250", "51715407", "52929091", "49373046", "52303055", "52420093", "49512712", "52652318", "48428925", "48322516", "47596375", "53199127", "49263492", "49609844", "50491680", "51808284", "50355436", "52176234", "", "", "", "", "", "", "", "", "", "", ""};
QStringList SistemaBuffet::usuariosApodos = {"davidmel09", "luciagom08", "mateofer11", "valentinarod13", "santiagolop09", "camilamar12", "benjamingar13", "martinaper13", "thiagosan08", "sofiarom09", "joaquindia11", "julietaalv13", "bautistator13", "agustinarui11", "lautaroaco12", "micaelaben12", "facundomed12", "florenciaher09", "nicolassua09", "rocioagu11", "marcela.ibanez", "diego.paz", "karina.luna", "sergio.ledesma", "paola.vega", "hector.maldonado", "alejandro.funes", "silvia.arce", "ramiro.sosa", "veronica.campos", "soporte.dev"};
QStringList SistemaBuffet::usuariosContrasenas = {"Davidmel#2026", "Luciagom#2026", "Mateofer#2026", "Valentinarod#2026", "Santiagolop#2026", "Camilamar#2026", "Benjamingar#2026", "Martinaper#2026", "Thiagosan#2026", "Sofiarom#2026", "Joaquindia#2026", "Julietaalv#2026", "Bautistator#2026", "Agustinarui#2026", "Lautaroaco#2026", "Micaelaben#2026", "Facundomed#2026", "Florenciaher#2026", "Nicolassua#2026", "Rocioagu#2026", "MarcelaBuffet#2026", "DiegoBuffet#2026", "KarinaBuffet#2026", "SergioBuffet#2026", "PaolaBuffet#2026", "HectorBuffet#2026", "AlejandroBuffet#2026", "SilviaBuffet#2026", "RamiroBuffet#2026", "VeronicaBuffet#2026", "SoporteDev#2026"};
QStringList SistemaBuffet::usuariosRoles = {"Estudiante", "Estudiante", "Estudiante", "Estudiante", "Estudiante", "Estudiante", "Estudiante", "Estudiante", "Estudiante", "Estudiante", "Estudiante", "Estudiante", "Estudiante", "Estudiante", "Estudiante", "Estudiante", "Estudiante", "Estudiante", "Estudiante", "Estudiante", "Personal de caja", "Personal de caja", "Personal de caja", "Personal de caja", "Personal de caja", "Personal de caja", "Administración", "Personal de caja", "Personal de caja", "Administración", "Desarrollador"};
QList<double> SistemaBuffet::usuariosSaldos = {10000, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

// Empleado con usuario (04_DatosSQL): PIN para el mostrador, token para el desarrollador
QStringList SistemaBuffet::personalApodos = {"marcela.ibanez", "diego.paz", "karina.luna", "sergio.ledesma", "paola.vega", "hector.maldonado", "alejandro.funes", "silvia.arce", "ramiro.sosa", "veronica.campos", "soporte.dev"};
QStringList SistemaBuffet::personalNombres = {"Marcela Ibáñez", "Diego Paz", "Karina Luna", "Sergio Ledesma", "Paola Vega", "Héctor Maldonado", "Alejandro Funes", "Silvia Arce", "Ramiro Sosa", "Verónica Campos", "Soporte Técnico"};
QStringList SistemaBuffet::personalPuestos = {"Cajera Turno Mañana", "Cajero Turno Mañana", "Cajera Turno Mañana", "Cajero Turno Tarde", "Cajera Turno Tarde", "Cajero Turno Tarde", "Encargado General de Buffet", "Atención al Mostrador Turno Mañana", "Atención al Mostrador Turno Tarde", "Administrativa Contable", "Desarrollador de Sistemas"};
QStringList SistemaBuffet::personalPines = {"1234", "2580", "3691", "4702", "5813", "6924", "", "7035", "8146", "", ""};
QStringList SistemaBuffet::personalTokens = {"", "", "", "", "", "", "", "", "", "", "DEV-BEREA-2026"};

QList<int> SistemaBuffet::carritoIds;
QStringList SistemaBuffet::carritoNombres;
QList<double> SistemaBuffet::carritoPrecios;
QList<int> SistemaBuffet::carritoCantidades;

// Mismo stock que la tabla Producto (04_DatosSQL). Con BDD: SELECT Stock_Producto FROM Producto
QList<int> SistemaBuffet::productosStock = {0, 40, 80, 50, 45, 90, 25, 70, 70, 60, 150,
                                            100, 100, 60, 80, 80, 70, 60, 40, 25, 40};
QList<double> SistemaBuffet::productosPrecios = {0, 2800, 1100, 1800, 1600, 800, 2200, 700, 1100, 1200, 1000,
                                                 900, 1300, 1400, 900, 700, 800, 600, 1500, 4200, 4100};

int SistemaBuffet::menuHoyID = 0;
QString SistemaBuffet::menuHoyPlato = "";
QString SistemaBuffet::menuHoyIncluye = "";
double SistemaBuffet::menuHoyPrecio = 0;

// Menú fijo (Menu_Del_Dia y Detalle_Menu_Dia de 04_DatosSQL), en orden de Menu_Del_DiaID: 1 = Lunes ... 5 = Viernes.
// Los fines de semana no hay menú
QStringList SistemaBuffet::menusDias = {"Lunes", "Martes", "Miércoles", "Jueves", "Viernes"};
QStringList SistemaBuffet::menusPlatos = {"Tarta de choclo con ensalada", "Tortilla de papas con ensalada", "Milanesa de pollo con puré", "Fideos con tuco y queso", "Ravioles con salsa fileto"};
QList<double> SistemaBuffet::menusPrecios = {3900, 3900, 4200, 3800, 4300};
QList<int> SistemaBuffet::menusStocks = {30, 40, 40, 35, 35};
QStringList SistemaBuffet::menusIncluye = {"Manzana", "Agua Mineral 500 ml", "Agua Mineral 500 ml", "Jugo en Caja 200 ml", "Jugo en Caja 200 ml"};

QList<int> SistemaBuffet::pedidosRetiroIds;
QStringList SistemaBuffet::pedidosCodigos;
QStringList SistemaBuffet::pedidosAlumnos;
QStringList SistemaBuffet::pedidosCursos;
QStringList SistemaBuffet::pedidosDnis;
QStringList SistemaBuffet::pedidosRecreos;
QStringList SistemaBuffet::pedidosMetodos;
QList<double> SistemaBuffet::pedidosMontos;
QList<double> SistemaBuffet::pedidosAbonados;
QList<double> SistemaBuffet::pedidosVueltos;
QStringList SistemaBuffet::pedidosEstados;
QStringList SistemaBuffet::pedidosDetalles;

void SistemaBuffet::cargarUsuarioActivo(int indiceUsuario)
{
    // Con BDD: los devuelve sp_iniciar_sesion junto con el Estudiante o el Profesor del DNI
    usuarioDni = usuariosDnis[indiceUsuario];
    // shortcut: el saldo se copia al entrar y no se guarda al salir; con BDD se lee siempre de Usuario.Saldo
    usuarioSaldo = usuariosSaldos[indiceUsuario];

    int indiceEstudiante = estudiantesDnis.indexOf(usuarioDni);
    if (indiceEstudiante != -1) {
        usuarioNombre = estudiantesNombres[indiceEstudiante];
        usuarioApellido = estudiantesApellidos[indiceEstudiante];
        usuarioCurso = estudiantesCursos[indiceEstudiante];
    } else {
        // Profesor: no tiene curso
        int indiceProfesor = profesoresDnis.indexOf(usuarioDni);
        usuarioNombre = profesoresNombres[indiceProfesor];
        usuarioApellido = profesoresApellidos[indiceProfesor];
        usuarioCurso = "Docente";
    }
}

void SistemaBuffet::inicializarDatos()
{
    static bool inicializado = false;
    if (inicializado) return;
    inicializado = true;

    // Pedido de ejemplo 1
    pedidosCodigos.append("3C6MB");
    pedidosAlumnos.append("David Melendez");
    pedidosCursos.append("6° Año 1ra");
    pedidosDnis.append("12345678");
    pedidosRecreos.append("2do Recreo TM (10:35 a 10:45 hs)");
    pedidosMetodos.append("Efectivo en Mostrador");
    pedidosMontos.append(4100.0);
    pedidosAbonados.append(5000.0);
    pedidosVueltos.append(900.0);
    pedidosEstados.append("PENDIENTE");
    pedidosDetalles.append("1x Sándwich de Milanesa ($2800)\n1x Gaseosa 500ml ($1300)");

    // Pedido de ejemplo 2
    pedidosCodigos.append("8A2KP");
    pedidosAlumnos.append("Lucía Gómez");
    pedidosCursos.append("6° Año 2da");
    pedidosDnis.append("45987654");
    pedidosRecreos.append("Receso Inter-Turno - Almuerzo (12:05 a 13:30 hs)");
    pedidosMetodos.append("Saldo de Cuenta Escolar");
    pedidosMontos.append(2800.0);
    pedidosAbonados.append(2800.0);
    pedidosVueltos.append(0.0);
    pedidosEstados.append("PENDIENTE");
    pedidosDetalles.append("1x Sándwich de Milanesa ($2800)");

    cargarMenuDelDia();
}

void SistemaBuffet::cargarMenuDelDia()
{
    menuHoyID = 0;
    menuHoyPlato = "";
    menuHoyIncluye = "";
    menuHoyPrecio = 0;
    productosStock[0] = 0;

    // El día de la semana lo calcula Qt (dayOfWeek: 1 = lunes ... 7 = domingo) y se lo pasa al SP
    QStringList dias = {"Lunes", "Martes", "Miércoles", "Jueves", "Viernes", "Sábado", "Domingo"};
    QString hoy = dias[QDate::currentDate().dayOfWeek() - 1];

    if (conn != NULL) {
        // Con BDD: sp_menu_del_dia devuelve el menú fijo de ese día de la semana
        MYSQL_RES *result = obtener_menu_del_dia(conn, hoy);
        if (result == NULL) return;
        MYSQL_ROW fila = NULL;
        if (error_del_sp(result) == 0) {
            fila = mysql_fetch_row(result);
        }
        if (fila != NULL) {
            menuHoyID = valor_fila(fila, 0).toInt();
            menuHoyPlato = valor_fila(fila, 1);
            menuHoyPrecio = valor_fila(fila, 2).toDouble();
            productosStock[0] = valor_fila(fila, 3).toInt();
            menuHoyIncluye = valor_fila(fila, 5);
        }
        mysql_free_result(result);
        return;
    }

    // Sin BDD: el menú fijo de ese día de la semana (sábado y domingo no están en la lista)
    int i = menusDias.indexOf(hoy);
    if (i != -1) {
        menuHoyID = i + 1;
        menuHoyPlato = menusPlatos[i];
        menuHoyIncluye = menusIncluye[i];
        menuHoyPrecio = menusPrecios[i];
        productosStock[0] = menusStocks[i];
    }
}

void SistemaBuffet::agregarAlCarrito(int id, QString nombre, double precio, int cantidad)
{
    // Si ya existe el producto en el carrito, sumar la cantidad
    for (int i = 0; i < carritoIds.size(); ++i) {
        if (carritoIds[i] == id) {
            carritoCantidades[i] += cantidad;
            return;
        }
    }

    // Si es nuevo, añadirlo a las listas
    carritoIds.append(id);
    carritoNombres.append(nombre);
    carritoPrecios.append(precio);
    carritoCantidades.append(cantidad);
}

void SistemaBuffet::vaciarCarrito()
{
    carritoIds.clear();
    carritoNombres.clear();
    carritoPrecios.clear();
    carritoCantidades.clear();
}

double SistemaBuffet::getTotalCarrito()
{
    double total = 0.0;
    for (int i = 0; i < carritoPrecios.size(); ++i) {
        total += carritoPrecios[i] * carritoCantidades[i];
    }
    return total;
}

int SistemaBuffet::getCantidadTotalCarrito()
{
    int cant = 0;
    for (int i = 0; i < carritoCantidades.size(); ++i) {
        cant += carritoCantidades[i];
    }
    return cant;
}

int SistemaBuffet::getCantidadEnCarrito(int id)
{
    for (int i = 0; i < carritoIds.size(); ++i) {
        if (carritoIds[i] == id) {
            return carritoCantidades[i];
        }
    }
    return 0;
}

bool SistemaBuffet::cargarUsuarioDesdeBDD(int userID)
{
    MYSQL_RES *result = obtener_datos_usuario(conn, userID);
    if (result == NULL) {
        return false;
    }
    MYSQL_ROW fila = mysql_fetch_row(result);
    if (fila != NULL) {
        usuarioID = userID;
        usuarioRol = valor_fila(fila, 0);
        usuarioNombre = valor_fila(fila, 1);
        usuarioApellido = valor_fila(fila, 2);
        usuarioCurso = valor_fila(fila, 3);
        usuarioDni = valor_fila(fila, 4);
        usuarioSaldo = valor_fila(fila, 5).toDouble();
    }
    mysql_free_result(result);
    return fila != NULL;
}

void SistemaBuffet::cargarDesdeBDD()
{
    if (conn == NULL) return;

    MYSQL_RES *result = obtener_productos(conn);
    if (result != NULL) {
        MYSQL_ROW fila;
        while ((fila = mysql_fetch_row(result)) != NULL) {
            int id = valor_fila(fila, 0).toInt();
            while (productosStock.size() <= id) {
                productosStock.append(0);
                productosPrecios.append(0);
            }
            productosPrecios[id] = valor_fila(fila, 3).toDouble();
            productosStock[id] = valor_fila(fila, 4).toInt();
            // Un producto dado de baja no se puede vender (sp_agregar_producto_carrito pide Activo = TRUE)
            if (valor_fila(fila, 5) == "Inactivo") {
                productosStock[id] = 0;
            }
        }
        mysql_free_result(result);
    }

    result = obtener_pedidos(conn);
    if (result != NULL) {
        pedidosRetiroIds.clear();
        pedidosCodigos.clear();
        pedidosAlumnos.clear();
        pedidosCursos.clear();
        pedidosDnis.clear();
        pedidosRecreos.clear();
        pedidosMetodos.clear();
        pedidosMontos.clear();
        pedidosAbonados.clear();
        pedidosVueltos.clear();
        pedidosEstados.clear();
        pedidosDetalles.clear();

        MYSQL_ROW fila;
        while ((fila = mysql_fetch_row(result)) != NULL) {
            pedidosRetiroIds.append(valor_fila(fila, 0).toInt());
            pedidosCodigos.append(valor_fila(fila, 1));
            pedidosAlumnos.append(valor_fila(fila, 2));
            pedidosCursos.append(valor_fila(fila, 3));
            pedidosDnis.append(valor_fila(fila, 4));
            pedidosRecreos.append(valor_fila(fila, 5));
            pedidosMetodos.append(valor_fila(fila, 6));
            pedidosMontos.append(valor_fila(fila, 7).toDouble());
            // La BDD no guarda con cuánto pagó en efectivo
            pedidosAbonados.append(valor_fila(fila, 7).toDouble());
            pedidosVueltos.append(0.0);
            pedidosEstados.append(valor_fila(fila, 8));
            pedidosDetalles.append(valor_fila(fila, 9));
        }
        mysql_free_result(result);
    }

    cargarMenuDelDia();
}

void SistemaBuffet::cerrarSesion()
{
    if (conn == NULL) return;
    MYSQL_RES *result = cerrar_sesion(conn, usuarioID);
    if (result != NULL) {
        mysql_free_result(result);
    }
}
