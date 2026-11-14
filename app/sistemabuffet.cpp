#include "sistemabuffet.h"

// Definición e inicialización de variables estáticas
QString SistemaBuffet::usuarioNombre = "Jorge";
QString SistemaBuffet::usuarioApellido = "Dahan";
QString SistemaBuffet::usuarioCurso = "6° Año 1ra";
QString SistemaBuffet::usuarioDni = "12345678";
double SistemaBuffet::usuarioSaldo = 10000.0;

// Estudiante (04_DatosSQL). Con BDD: SELECT de Estudiante y Curso
QStringList SistemaBuffet::estudiantesDnis = {"12345678", "45987654", "48499250", "51715407", "52929091", "49373046", "52303055", "52420093", "49512712", "52652318", "48428925", "48322516", "47596375", "53199127", "49263492", "49609844", "50491680", "51808284", "50355436", "52176234"};
QStringList SistemaBuffet::estudiantesNombres = {"Jorge", "Lucía", "Mateo", "Valentina", "Santiago", "Camila", "Benjamín", "Martina", "Thiago", "Sofía", "Joaquín", "Julieta", "Bautista", "Agustina", "Lautaro", "Micaela", "Facundo", "Florencia", "Nicolás", "Rocío"};
QStringList SistemaBuffet::estudiantesApellidos = {"Dahan", "Gómez", "Fernández", "Rodríguez", "López", "Martínez", "García", "Pérez", "Sánchez", "Romero", "Díaz", "Álvarez", "Torres", "Ruiz", "Acosta", "Benítez", "Medina", "Herrera", "Suárez", "Aguirre"};
QStringList SistemaBuffet::estudiantesCursos = {"6° Año 1ra", "6° Año 2da", "3° Año 3ra", "1° Año 4ta", "5° Año 1ra", "2° Año 2da", "1° Año 3ra", "1° Año 4ta", "6° Año 1ra", "5° Año 2da", "3° Año 3ra", "1° Año 4ta", "1° Año 1ra", "3° Año 2da", "2° Año 3ra", "2° Año 4ta", "2° Año 1ra", "5° Año 2da", "5° Año 3ra", "3° Año 4ta"};

// Usuario (04_DatosSQL): la posición + 1 es el UserID. El personal no tiene DNI
QStringList SistemaBuffet::usuariosDnis = {"12345678", "45987654", "48499250", "51715407", "52929091", "49373046", "52303055", "52420093", "49512712", "52652318", "48428925", "48322516", "47596375", "53199127", "49263492", "49609844", "50491680", "51808284", "50355436", "52176234", "", "", "", "", "", "", "", "", "", "", ""};
QStringList SistemaBuffet::usuariosApodos = {"jorgedah09", "luciagom08", "mateofer11", "valentinarod13", "santiagolop09", "camilamar12", "benjamingar13", "martinaper13", "thiagosan08", "sofiarom09", "joaquindia11", "julietaalv13", "bautistator13", "agustinarui11", "lautaroaco12", "micaelaben12", "facundomed12", "florenciaher09", "nicolassua09", "rocioagu11", "marcela.ibanez", "diego.paz", "karina.luna", "sergio.ledesma", "paola.vega", "hector.maldonado", "alejandro.funes", "silvia.arce", "ramiro.sosa", "veronica.campos", "soporte.dev"};
QStringList SistemaBuffet::usuariosContrasenas = {"Jorgedah#2026", "Luciagom#2026", "Mateofer#2026", "Valentinarod#2026", "Santiagolop#2026", "Camilamar#2026", "Benjamingar#2026", "Martinaper#2026", "Thiagosan#2026", "Sofiarom#2026", "Joaquindia#2026", "Julietaalv#2026", "Bautistator#2026", "Agustinarui#2026", "Lautaroaco#2026", "Micaelaben#2026", "Facundomed#2026", "Florenciaher#2026", "Nicolassua#2026", "Rocioagu#2026", "MarcelaBuffet#2026", "DiegoBuffet#2026", "KarinaBuffet#2026", "SergioBuffet#2026", "PaolaBuffet#2026", "HectorBuffet#2026", "AlejandroBuffet#2026", "SilviaBuffet#2026", "RamiroBuffet#2026", "VeronicaBuffet#2026", "SoporteDev#2026"};
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
    // Con BDD: los devuelve sp_iniciar_sesion junto con el Estudiante del DNI
    usuarioDni = usuariosDnis[indiceUsuario];
    // shortcut: el saldo se copia al entrar y no se guarda al salir; con BDD se lee siempre de Usuario.Saldo
    usuarioSaldo = usuariosSaldos[indiceUsuario];

    int indiceEstudiante = estudiantesDnis.indexOf(usuarioDni);
    usuarioNombre = estudiantesNombres[indiceEstudiante];
    usuarioApellido = estudiantesApellidos[indiceEstudiante];
    usuarioCurso = estudiantesCursos[indiceEstudiante];
}

void SistemaBuffet::inicializarDatos()
{
    static bool inicializado = false;
    if (inicializado) return;
    inicializado = true;

    // Pedido de ejemplo 1
    pedidosCodigos.append("3C6MB");
    pedidosAlumnos.append("Jorge Dahan");
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
