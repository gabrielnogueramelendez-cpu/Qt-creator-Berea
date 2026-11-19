#ifndef PANTALLAS_H
#define PANTALLAS_H

#include <QWidget>
#include <QString>

// Cada pantalla es un par de funciones: abrir_xxx() arma la ventana con su .ui (Qt Designer)
// y conecta cada botón con una función. Sin clases propias: la ventana es un QWidget común.

// Ventanas (se abren y se cierran solas)
void abrir_login();
void abrir_catalogo();
void abrir_caja();
void abrir_admin();
void abrir_desarrollo();

// Diálogos (esperan a que el usuario los cierre)
void abrir_registro(QWidget *padre);
bool abrir_detalle(QWidget *padre, int productoID);
bool abrir_carrito(QWidget *padre);
bool abrir_pago(QWidget *padre, QString &codigo, double &abonado, double &vuelto);
void abrir_comprobante(QWidget *padre, const QString &codigo, double abonado, double vuelto);
void abrir_perfil(QWidget *padre);

#endif // PANTALLAS_H
