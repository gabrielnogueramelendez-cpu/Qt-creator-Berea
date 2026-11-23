// pantallas.h: Una función abrir_xxx() por pantalla.

#ifndef PANTALLAS_H
#define PANTALLAS_H

#include <QWidget>
#include <QString>

// ===== Ventanas: se abren y quedan abiertas hasta que se cierran solas =====
void abrir_login();
void abrir_catalogo();
void abrir_caja();
void abrir_admin();
void abrir_desarrollo();

// ===== Diálogos: frenan la pantalla de atrás hasta que el usuario los cierra =====
void abrir_registro(QWidget *padre);
bool abrir_detalle(QWidget *padre, int productoID);   // true si agregó al carrito
bool abrir_carrito(QWidget *padre);                   // true si eligió "ir al pago"
bool abrir_pago(QWidget *padre, QString &codigo, double &abonado, double &vuelto); // true si pagó; devuelve el código
void abrir_comprobante(QWidget *padre, const QString &codigo, double abonado, double vuelto);
void abrir_perfil(QWidget *padre);

#endif // PANTALLAS_H
