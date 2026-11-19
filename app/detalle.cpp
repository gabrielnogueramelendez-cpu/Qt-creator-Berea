#include "pantallas.h"
#include "ui_productdetaildialog.h"
#include "datos.h"
#include <QDialog>
#include <QMessageBox>

// Detalle de un producto o del menú del día (productdetaildialog.ui)
static QDialog *dialogo = NULL;
static Ui::ProductDetailDialog *ui = NULL;
static int producto_id = 0;
static int cantidad = 1;

// Límite por pedido escolar
static const int LIMITE_POR_PEDIDO = 10;

static int maximo_permitido()
{
    // No más de 10 ni más que el stock, contando lo que ya está en el carrito
    int maximo = productos_stock[producto_id];
    if (maximo > LIMITE_POR_PEDIDO) {
        maximo = LIMITE_POR_PEDIDO;
    }
    return maximo - cantidad_en_carrito(producto_id);
}

static void mostrar_cantidad()
{
    ui->lblCantidad->setText(QString::number(cantidad));
    ui->lblCalculatedTotal->setText("Total: " + pesos(productos_precios[producto_id] * cantidad));
}

static void sumar()
{
    if (cantidad >= maximo_permitido()) {
        int enCarrito = cantidad_en_carrito(producto_id);
        if (enCarrito > 0) {
            QMessageBox::warning(dialogo, "Límite Alcanzado",
                QString("Ya tenés %1 unidad(es) en el carrito. No podés sumar más de este producto.").arg(enCarrito));
        } else if (productos_stock[producto_id] < LIMITE_POR_PEDIDO) {
            QMessageBox::warning(dialogo, "Stock Límite",
                QString("Solo quedan %1 unidad(es) disponibles en stock para este producto.").arg(productos_stock[producto_id]));
        } else {
            QMessageBox::warning(dialogo, "Límite por Pedido",
                QString("Por políticas del buffet escolar, el límite máximo es de %1 unidades por producto por pedido.").arg(LIMITE_POR_PEDIDO));
        }
        return;
    }
    cantidad++;
    mostrar_cantidad();
}

static void restar()
{
    if (cantidad > 1) {
        cantidad--;
        mostrar_cantidad();
    }
}

static void agregar()
{
    if (cantidad > maximo_permitido()) {
        QMessageBox::warning(dialogo, "Sin Stock Suficiente",
            QString("Ya tenés %1 en el carrito y no quedan más unidades disponibles de este producto.")
                .arg(cantidad_en_carrito(producto_id)));
        return;
    }
    agregar_al_carrito(producto_id, productos_nombres[producto_id], productos_precios[producto_id], cantidad);
    dialogo->accept();
}

bool abrir_detalle(QWidget *padre, int productoID)
{
    if (productoID < 0 || productoID >= productos_stock.size()) {
        return false;
    }
    producto_id = productoID;
    cantidad = 1;

    dialogo = new QDialog(padre);
    ui = new Ui::ProductDetailDialog();
    ui->setupUi(dialogo);
    dialogo->setWindowTitle("Detalle de Producto - " + productos_nombres[producto_id]);

    ui->lblTitle->setText(productos_nombres[producto_id]);
    ui->lblDescription->setText(productos_descripciones[producto_id]);
    ui->lblPriceTag->setText(pesos(productos_precios[producto_id]));
    QPixmap foto(":/imagenes/imagenes/" + productos_nombres[producto_id] + ".jpg");
    if (!foto.isNull()) {
        ui->lblFotoProducto->setPixmap(foto.scaled(ui->lblFotoProducto->size(), Qt::KeepAspectRatio));
    }
    mostrar_cantidad();

    QObject::connect(ui->btnPlus, &QPushButton::clicked, sumar);
    QObject::connect(ui->btnMinus, &QPushButton::clicked, restar);
    QObject::connect(ui->btnAddToCart, &QPushButton::clicked, agregar);

    bool agregado = (dialogo->exec() == QDialog::Accepted);

    delete ui;
    delete dialogo;
    ui = NULL;
    dialogo = NULL;
    return agregado;
}
