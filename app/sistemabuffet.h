#ifndef SISTEMABUFFET_H
#define SISTEMABUFFET_H

#include <QString>
#include <QStringList>
#include <QList>
#include <mysql.h>

// Clase auxiliar de datos globales compartidos entre ventanas
// Implementada exclusivamente con tipos primitivos y clases estándar de Qt (SIN STRUCTS)
class SistemaBuffet
{
public:
    // Conexión a MySQL (NULL si no se pudo conectar: la app usa los datos de ejemplo de abajo)
    static MYSQL *conn;

    // Datos del Usuario Activo
    static int usuarioID;
    static QString usuarioRol;
    static QString usuarioNombre;
    static QString usuarioApellido;
    static QString usuarioCurso;
    static QString usuarioDni;
    static double usuarioSaldo;

    // Tablas de la BDD (04_DatosSQL) en listas paralelas
    static QStringList estudiantesDnis;
    static QStringList estudiantesNombres;
    static QStringList estudiantesApellidos;
    static QStringList estudiantesCursos;
    static QStringList profesoresDnis;
    static QStringList profesoresNombres;
    static QStringList profesoresApellidos;
    static QStringList roles;
    static QStringList usuariosDnis;
    static QStringList usuariosRoles;
    static QStringList usuariosApodos;
    static QStringList usuariosContrasenas;
    static QList<double> usuariosSaldos;
    static QStringList personalApodos;
    static QStringList personalNombres;
    static QStringList personalPuestos;
    static QStringList personalPines;
    static QStringList personalTokens;

    // Pasa los datos del usuario que inició sesión a las variables del usuario activo
    static void cargarUsuarioActivo(int indiceUsuario);
    // Lo mismo pero leyendo de la BDD el usuario que devolvió el SP de login
    static bool cargarUsuarioDesdeBDD(int userID);

    // Carrito de compras (listas paralelas simples)
    static QList<int> carritoIds;
    static QStringList carritoNombres;
    static QList<double> carritoPrecios;
    static QList<int> carritoCantidades;

    // Operaciones básicas del Carrito
    static void agregarAlCarrito(int id, QString nombre, double precio, int cantidad);
    static void vaciarCarrito();
    static double getTotalCarrito();
    static int getCantidadTotalCarrito();
    static int getCantidadEnCarrito(int id);

    // Stock de cada producto: la posición es el ProductoID de la BDD.
    // La posición 0 (ningún producto la usa) es el menú del día: en el carrito va con id 0
    static QList<int> productosStock;
    static QList<double> productosPrecios;

    // Menú del día de hoy (menuHoyID = Menu_Del_DiaID, 0 si hoy no hay menú)
    static int menuHoyID;
    static QString menuHoyPlato;
    static QString menuHoyIncluye;
    static double menuHoyPrecio;
    // Menú semanal fijo de Menu_Del_Dia (04_DatosSQL) y el producto que incluye cada menú (Detalle_Menu_Dia)
    static QStringList menusDias;
    static QStringList menusPlatos;
    static QList<double> menusPrecios;
    static QList<int> menusStocks;
    static QStringList menusIncluye;
    // Igual que sp_menu_del_dia: de lunes a viernes, cada día con su plato fijo
    static void cargarMenuDelDia();

    // Pedidos generados para Mostrador/Caja y Comprobantes
    static QList<int> pedidosRetiroIds;
    static QStringList pedidosCodigos;
    static QStringList pedidosAlumnos;
    static QStringList pedidosCursos;
    static QStringList pedidosDnis;
    static QStringList pedidosRecreos;
    static QStringList pedidosMetodos;
    static QList<double> pedidosMontos;
    static QList<double> pedidosAbonados;
    static QList<double> pedidosVueltos;
    static QStringList pedidosEstados;
    static QStringList pedidosDetalles;

    // Inicializador de datos predeterminados
    static void inicializarDatos();
    // Con conexión: reemplaza el stock, los precios y los pedidos por los de la BDD
    static void cargarDesdeBDD();
    // Deja el cierre de sesión en Log_Auditoria (sp_cerrar_sesion)
    static void cerrarSesion();
};

#endif // SISTEMABUFFET_H
