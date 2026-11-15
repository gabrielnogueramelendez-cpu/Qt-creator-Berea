#ifndef SISTEMABUFFET_H
#define SISTEMABUFFET_H

#include <QString>
#include <QStringList>
#include <QList>

// Clase auxiliar de datos globales compartidos entre ventanas
// Implementada exclusivamente con tipos primitivos y clases estándar de Qt (SIN STRUCTS)
class SistemaBuffet
{
public:
    // Datos del Usuario Activo
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

    // Stock de cada producto: la posición es el ProductoID de la BDD (la posición 0 no se usa)
    static QList<int> productosStock;

    // Pedidos generados para Mostrador/Caja y Comprobantes
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
};

#endif // SISTEMABUFFET_H
