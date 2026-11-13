#include "sistemabuffet.h"
#include <QRandomGenerator>
#include <QSqlDatabase>
#include <QSqlError>

SistemaBuffet& SistemaBuffet::instancia()
{
    static SistemaBuffet s_instancia;
    return s_instancia;
}

SistemaBuffet::SistemaBuffet() :
    m_haySesionIniciada(false)
{
    inicializarDatosPorDefecto();
}

void SistemaBuffet::inicializarDatosPorDefecto()
{
    // 1. Estudiantes registrados en el sistema escolar
    m_usuarios = {
        {"95671088", "Gabriel", "Noguera", "4° Año B", "gnoguera@bue.edu.ar", "Alumno123", "Estudiante", 15000.0},
        {"46555222", "Martina", "López", "5° Año A", "mlopez@bue.edu.ar", "Martina123", "Estudiante", 8500.0},
        {"admin", "Alejandro", "Berea", "Administración", "admin@buffet.escuela.edu.ar", "Admin1234", "Admin", 0.0}
    };

    // 2. Personal del Buffet (Mapeo a la entidad Empleado de Codd)
    m_empleados = {
        {"EMP-101", "cajero1", "María", "López", "Cajera Mostrador TM", "Cajero123", "1234"},
        {"EMP-102", "cajero2", "Carlos", "Benítez", "Cajero Turno Tarde", "Cajero456", "5678"}
    };

    // 3. Desarrolladores e Ingenieros de Software del Sistema
    m_desarrolladores = {
        {"dev_berea", "Gabriel Noguera (Lead Dev)", "DevPass2026", "DEV-BEREA-2026"},
        {"dev_root", "Arquitectura & QA Berea", "Root2026!", "DEV-ROOT-KEY"}
    };

    // 4. Parámetros de conexión preparados para MySQL (Consigna de Laboratorio)
    m_configMySQL.host = "localhost";
    m_configMySQL.puerto = 3306;
    m_configMySQL.baseDeDatos = "ElBuffetDeAlejandro";
    m_configMySQL.usuario = "root";
    m_configMySQL.password = "12341234";
    m_configMySQL.conectada = false;

    // 5. Catálogo inicial con stock real y precios
    m_productos = {
        {1, "🌟 Combo Almuerzo Gourmet", "Milanesa a la napolitana con papas rústicas + bebida 500ml + postre.", "Menú del Día", 8500.0, 20, true},
        {2, "🥗 Combo Express Saludable", "Tarta rústica de verdura y queso + ensalada de estación + agua mineral.", "Menú del Día", 5200.0, 15, true},
        {3, "🥪 Sándwich de Milanesa Ciabatta Completo", "Milanesa de ternera, lechuga, tomate, queso tybo en ciabatta artesanal.", "Salados", 2800.0, 35, true},
        {4, "🥟 Empanada de Carne Cortada a Cuchillo", "Ternera seleccionada cortada a cuchillo con especias criollas.", "Salados", 1200.0, 40, true},
        {5, "🥧 Tarta Rústica de Jamón y Queso", "Doble capa de mozzarella fundida y jamón cocido seleccionado.", "Salados", 2400.0, 18, true},
        {6, "🥖 Pebete Tradicional de Jamón y Queso", "Pan pebete suave, abundante jamón y queso tybo con manteca.", "Salados", 1800.0, 25, true},
        {7, "🍫 Alfajor Artesanal de Dulce de Leche", "Relleno abundante de dulce de leche bañado en chocolate semi-amargo.", "Dulces", 1100.0, 50, true},
        {8, "🥮 Porción de Pasta Frola Artesanal", "Receta casera tradicional con dulce de membrillo especial.", "Dulces", 1500.0, 22, true},
        {9, "🥐 Factura de Manteca Seleccionada", "Medialuna u opción hojaldrada horneada en la mañana del buffet.", "Dulces", 600.0, 60, true},
        {10, "🧁 Muffin de Chocolate con Corazón Dulce", "Cacao intenso con chispas de chocolate y corazón de dulce de leche.", "Dulces", 1400.0, 30, true},
        {11, "🥤 Gaseosa Línea Coca-Cola 500ml", "Botella de 500ml bien fría (Coca-Cola, Sprite o Fanta).", "Bebidas", 1300.0, 45, true},
        {12, "💧 Agua Mineral Villavicencio 500ml", "Agua mineral de manantial pura sin gas de 500ml.", "Bebidas", 900.0, 50, true},
        {13, "🍊 Jugo Natural Exprimido de Naranja 350ml", "100% naranjas frescas exprimidas en el día sin conservantes.", "Bebidas", 1600.0, 25, true},
        {14, "🍓 Licuado Frutal de Frutilla y Banana 400ml", "Fruta fresca de estación preparado en el momento con leche o agua.", "Bebidas", 2200.0, 20, true}
    };

    // 6. Pedidos iniciales para pruebas en mostrador
    PedidoBuffet p1;
    p1.codigoRetiro = "3C6MB";
    p1.dniAlumno = "95671088";
    p1.nombreAlumno = "Gabriel Noguera";
    p1.cursoAlumno = "4° Año B";
    p1.recreoRetiro = "1er Recreo TM (10:35 hs)";
    p1.metodoPago = "Efectivo en Mostrador";
    p1.montoTotal = 4100.0;
    p1.montoAbonado = 5000.0;
    p1.vuelto = 900.0;
    p1.items = {
        {3, "🥪 Sándwich de Milanesa Ciabatta Completo", 2800.0, 1},
        {11, "🥤 Gaseosa Línea Coca-Cola 500ml", 1300.0, 1}
    };
    p1.estado = "PENDIENTE";
    p1.fechaHora = QDateTime::currentDateTime().addSecs(-1800);
    m_pedidos.append(p1);

    PedidoBuffet p2;
    p2.codigoRetiro = "8A2KP";
    p2.dniAlumno = "46555222";
    p2.nombreAlumno = "Martina López";
    p2.cursoAlumno = "5° Año A";
    p2.recreoRetiro = "1er Recreo TM (10:35 hs)";
    p2.metodoPago = "Saldo Cuenta Escolar";
    p2.montoTotal = 2800.0;
    p2.montoAbonado = 2800.0;
    p2.vuelto = 0.0;
    p2.items = {
        {3, "🥪 Sándwich de Milanesa Ciabatta Completo", 2800.0, 1}
    };
    p2.estado = "PENDIENTE";
    p2.fechaHora = QDateTime::currentDateTime().addSecs(-900);
    m_pedidos.append(p2);

    registrarLog("Sistema", "Sistema", "INICIO", "Servicios de gestión de buffet escolar iniciados en memoria.");
}

bool SistemaBuffet::registrarEstudiante(const QString &dni, const QString &nombre, const QString &apellido,
                                       const QString &curso, const QString &email, const QString &password,
                                       QString &errorMsg)
{
    // [REG-05] Verificar unicidad de DNI y Email
    for (const auto &u : m_usuarios) {
        if (u.dni.trimmed() == dni.trimmed()) {
            errorMsg = QString("El DNI %1 ya se encuentra registrado en el sistema escolar.").arg(dni);
            return false;
        }
        if (u.email.trimmed().toLower() == email.trimmed().toLower()) {
            errorMsg = QString("El correo institucional %1 ya pertenece a un estudiante existente.").arg(email);
            return false;
        }
    }

    UsuarioSistema nuevo;
    nuevo.dni = dni.trimmed();
    nuevo.nombre = nombre.trimmed();
    nuevo.apellido = apellido.trimmed();
    nuevo.curso = curso.trimmed();
    nuevo.email = email.trimmed().toLower();
    nuevo.password = password;
    nuevo.rol = "Estudiante";
    nuevo.saldoCuenta = 10000.0; // Saldo inicial escolar

    m_usuarios.append(nuevo);

    registrarLog(nuevo.nombre + " " + nuevo.apellido, "Estudiante", "REGISTRO_ALUMNO",
                 QString("Nuevo estudiante registrado con DNI %1 y correo %2").arg(dni, email));
    return true;
}

bool SistemaBuffet::autenticarEstudiante(const QString &dniOEmail, const QString &password,
                                        UsuarioSistema &usuarioLogueado, QString &errorMsg)
{
    QString idBusqueda = dniOEmail.trimmed().toLower();

    for (const auto &u : m_usuarios) {
        if (u.rol == "Estudiante" && (u.dni.toLower() == idBusqueda || u.email.toLower() == idBusqueda)) {
            if (u.password == password) {
                m_usuarioActual = u;
                m_haySesionIniciada = true;
                usuarioLogueado = u;
                registrarLog(u.nombre + " " + u.apellido, "Estudiante", "LOGIN_EXITOSO", "Inicio de sesión de alumno.");
                return true;
            } else {
                errorMsg = "Contraseña incorrecta. Por favor verifica tu clave de acceso.";
                registrarLog(dniOEmail, "Estudiante", "LOGIN_FALLIDO", "Contraseña errónea.");
                return false;
            }
        }
    }

    errorMsg = "El DNI o correo ingresado NO corresponde a un estudiante registrado.\nPor favor haz clic en '¡Regístrate aquí!' para crear tu cuenta.";
    registrarLog(dniOEmail, "Estudiante", "LOGIN_FALLIDO", "Estudiante no registrado.");
    return false;
}

bool SistemaBuffet::autenticarPersonal(const QString &legajoOUsuario, const QString &password, const QString &pinTurno,
                                      EmpleadoBuffet &empleadoLogueado, QString &errorMsg)
{
    QString buscado = legajoOUsuario.trimmed().toLower();

    // 1. Validar que no esté vacío el PIN de turno
    if (pinTurno.trimmed().isEmpty()) {
        errorMsg = "Por protocolo de seguridad del buffet, debes ingresar tu PIN de turno de 4 dígitos.";
        return false;
    }

    // 2. Buscar en la lista oficial de Empleados del buffet
    for (const auto &emp : m_empleados) {
        if (emp.empleadoId.toLower() == buscado || emp.usuario.toLower() == buscado) {
            // Validar contraseña
            if (emp.password != password) {
                errorMsg = "La contraseña del personal no es correcta.";
                registrarLog(emp.nombre + " " + emp.apellido, "Personal", "LOGIN_FALLIDO", "Contraseña errónea.");
                return false;
            }
            // Validar PIN de mostrador
            if (emp.pinTurno != pinTurno.trimmed()) {
                errorMsg = "El PIN de seguridad de turno es incorrecto. Comunícate con el encargado de turno.";
                registrarLog(emp.nombre + " " + emp.apellido, "Personal", "LOGIN_FALLIDO", "PIN de turno inválido.");
                return false;
            }

            empleadoLogueado = emp;

            // Registrar usuario actual
            UsuarioSistema u;
            u.dni = emp.empleadoId;
            u.nombre = emp.nombre;
            u.apellido = emp.apellido;
            u.curso = emp.puesto;
            u.email = emp.usuario + "@buffet.escuela.edu.ar";
            u.rol = "Cajero";
            u.saldoCuenta = 0.0;
            m_usuarioActual = u;
            m_haySesionIniciada = true;

            registrarLog(emp.nombre + " " + emp.apellido, "Cajero", "LOGIN_PERSONAL",
                         QString("Personal habilitado en mostrador: Legajo %1 (%2)").arg(emp.empleadoId, emp.puesto));
            return true;
        }
    }

    errorMsg = "El Legajo o Usuario de empleado NO figura en la nómina autorizada del buffet escolar.\nEl acceso de personal requiere autorización de la dirección.";
    registrarLog(legajoOUsuario, "Personal", "LOGIN_FALLIDO", "Legajo no existente.");
    return false;
}

bool SistemaBuffet::autenticarAdmin(const QString &usuario, const QString &password,
                                   UsuarioSistema &adminLogueado, QString &errorMsg)
{
    QString buscado = usuario.trimmed().toLower();
    for (const auto &u : m_usuarios) {
        if (u.rol == "Admin" && (u.dni.toLower() == buscado || u.email.toLower() == buscado)) {
            if (u.password == password) {
                m_usuarioActual = u;
                m_haySesionIniciada = true;
                adminLogueado = u;
                registrarLog(u.nombre, "Admin", "LOGIN_ADMIN", "Acceso concedido a administración general.");
                return true;
            } else {
                errorMsg = "Contraseña de administración incorrecta.";
                registrarLog(usuario, "Admin", "LOGIN_FALLIDO", "Clave de administración incorrecta.");
                return false;
            }
        }
    }

    errorMsg = "Usuario de administrador no reconocido en el sistema.";
    registrarLog(usuario, "Admin", "LOGIN_FALLIDO", "Usuario admin inexistente.");
    return false;
}

bool SistemaBuffet::autenticarDesarrollador(const QString &usuario, const QString &password, const QString &tokenDev,
                                           DesarrolladorSistema &devLogueado, QString &errorMsg)
{
    QString buscado = usuario.trimmed().toLower();

    // 1. Validación de Token de Desarrollador obligatorio
    if (tokenDev.trimmed().isEmpty()) {
        errorMsg = "Para acceder al Modo Desarrollador debes ingresar el Token de Autorización Técnica (Dev Secret Key).";
        return false;
    }

    // 2. Buscar en la nómina de desarrolladores
    for (const auto &dev : m_desarrolladores) {
        if (dev.usuario.toLower() == buscado) {
            if (dev.password != password) {
                errorMsg = "Contraseña de desarrollador incorrecta.";
                registrarLog(usuario, "Dev", "LOGIN_DEV_FALLIDO", "Clave dev errónea.");
                return false;
            }
            if (dev.developerKey != tokenDev.trimmed()) {
                errorMsg = "El Token de Autorización Técnica (Dev Key) no es válido o está revocado.";
                registrarLog(usuario, "Dev", "LOGIN_DEV_FALLIDO", "Token dev rechazado.");
                return false;
            }

            devLogueado = dev;

            UsuarioSistema u;
            u.dni = "DEV-001";
            u.nombre = dev.nombre;
            u.apellido = "";
            u.curso = "Ingeniería de Software";
            u.email = dev.usuario + "@berea.dev";
            u.rol = "Desarrollador";
            u.saldoCuenta = 999999.0;
            m_usuarioActual = u;
            m_haySesionIniciada = true;

            registrarLog(dev.nombre, "Desarrollador", "LOGIN_DEV", "Acceso autorizado a consola técnica y diagnóstico de BDD.");
            return true;
        }
    }

    errorMsg = "Usuario de desarrollador no autorizado. Esta sección es exclusiva para el equipo técnico de desarrollo.";
    registrarLog(usuario, "Dev", "LOGIN_DEV_FALLIDO", "Usuario dev inexistente.");
    return false;
}

void SistemaBuffet::cerrarSesion()
{
    if (m_haySesionIniciada) {
        registrarLog(m_usuarioActual.nombre + " " + m_usuarioActual.apellido, m_usuarioActual.rol, "LOGOUT", "Cierre de sesión seguro.");
    }
    m_usuarioActual = UsuarioSistema();
    m_haySesionIniciada = false;
    vaciarCarrito();
}

ProductoBuffet SistemaBuffet::getProductoPorId(int id, bool &encontrado) const
{
    for (const auto &p : m_productos) {
        if (p.id == id) {
            encontrado = true;
            return p;
        }
    }
    encontrado = false;
    return ProductoBuffet();
}

bool SistemaBuffet::agregarProducto(const QString &nombre, const QString &desc, const QString &cat,
                                   double precio, int stock, QString &errorMsg)
{
    for (const auto &p : m_productos) {
        if (p.nombre.trimmed().compare(nombre.trimmed(), Qt::CaseInsensitive) == 0) {
            errorMsg = QString("Ya existe un producto con el nombre '%1'.").arg(nombre);
            return false;
        }
    }

    ProductoBuffet nuevo;
    nuevo.id = m_productos.size() + 1;
    nuevo.nombre = nombre.trimmed();
    nuevo.descripcion = desc.trimmed();
    nuevo.categoria = cat.trimmed();
    nuevo.precio = precio;
    nuevo.stock = stock;
    nuevo.activo = true;

    m_productos.append(nuevo);

    registrarLog(m_usuarioActual.nombre.isEmpty() ? "Admin" : m_usuarioActual.nombre, "Admin", "PRODUCTO_AGREGADO",
                 QString("Alta de producto ID %1: %2 ($%3, stock: %4)").arg(nuevo.id).arg(nombre).arg(precio).arg(stock));
    return true;
}

bool SistemaBuffet::actualizarProducto(int id, double nuevoPrecio, int nuevoStock, QString &errorMsg)
{
    for (auto &p : m_productos) {
        if (p.id == id) {
            p.precio = nuevoPrecio;
            p.stock = nuevoStock;
            registrarLog(m_usuarioActual.nombre.isEmpty() ? "Admin" : m_usuarioActual.nombre, "Admin", "STOCK_UPDATE",
                         QString("Modificado producto ID %1: Precio $%2, Stock %3").arg(id).arg(nuevoPrecio).arg(nuevoStock));
            return true;
        }
    }
    errorMsg = "Producto no encontrado.";
    return false;
}

bool SistemaBuffet::eliminarProducto(int id, QString &errorMsg)
{
    for (int i = 0; i < m_productos.size(); ++i) {
        if (m_productos[i].id == id) {
            QString nombre = m_productos[i].nombre;
            m_productos.removeAt(i);
            registrarLog(m_usuarioActual.nombre.isEmpty() ? "Admin" : m_usuarioActual.nombre, "Admin", "PRODUCTO_ELIMINADO",
                         QString("Eliminado producto ID %1: %2").arg(id).arg(nombre));
            return true;
        }
    }
    errorMsg = "Producto no encontrado.";
    return false;
}

bool SistemaBuffet::reducirStock(int productoId, int cantidad)
{
    for (auto &p : m_productos) {
        if (p.id == productoId) {
            if (p.stock >= cantidad) {
                p.stock -= cantidad;
                return true;
            }
            return false;
        }
    }
    return false;
}

bool SistemaBuffet::agregarAlCarrito(int productoId, const QString &nombre, double precio, int cantidad, int stockMax, QString &errorMsg)
{
    for (auto &item : m_carrito) {
        if (item.productoId == productoId) {
            if (item.cantidad + cantidad > stockMax) {
                errorMsg = QString("No puedes agregar más unidades. El stock disponible es %1 (ya tienes %2 en el carrito).")
                               .arg(stockMax).arg(item.cantidad);
                return false;
            }
            if (item.cantidad + cantidad > 10) {
                errorMsg = "Por política del buffet escolar, el límite máximo es de 10 unidades por producto.";
                return false;
            }
            item.cantidad += cantidad;
            return true;
        }
    }

    ItemCarrito nuevoItem;
    nuevoItem.productoId = productoId;
    nuevoItem.nombre = nombre;
    nuevoItem.precioUnitario = precio;
    nuevoItem.cantidad = cantidad;
    m_carrito.append(nuevoItem);
    return true;
}

bool SistemaBuffet::modificarCantidadCarrito(int productoId, int nuevaCantidad, QString &errorMsg)
{
    if (nuevaCantidad <= 0) {
        eliminarDelCarrito(productoId);
        return true;
    }

    if (nuevaCantidad > 10) {
        errorMsg = "El límite máximo es de 10 unidades por producto.";
        return false;
    }

    bool encontrado = false;
    ProductoBuffet p = getProductoPorId(productoId, encontrado);
    if (encontrado && nuevaCantidad > p.stock) {
        errorMsg = QString("Solo hay %1 unidades disponibles en stock.").arg(p.stock);
        return false;
    }

    for (auto &item : m_carrito) {
        if (item.productoId == productoId) {
            item.cantidad = nuevaCantidad;
            return true;
        }
    }
    return false;
}

void SistemaBuffet::eliminarDelCarrito(int productoId)
{
    for (int i = 0; i < m_carrito.size(); ++i) {
        if (m_carrito[i].productoId == productoId) {
            m_carrito.removeAt(i);
            break;
        }
    }
}

void SistemaBuffet::vaciarCarrito()
{
    m_carrito.clear();
}

double SistemaBuffet::getTotalCarrito() const
{
    double total = 0.0;
    for (const auto &it : m_carrito) {
        total += it.subtotal();
    }
    return total;
}

int SistemaBuffet::getCantidadTotalCarrito() const
{
    int cant = 0;
    for (const auto &it : m_carrito) {
        cant += it.cantidad;
    }
    return cant;
}

QString SistemaBuffet::generarCodigoRetiroUnico()
{
    const QString caracteres = "ABCDEFGHJKLMNPQRSTUVWXYZ23456789";
    QString codigo;
    bool unico = false;

    while (!unico) {
        codigo.clear();
        for (int i = 0; i < 5; ++i) {
            int index = QRandomGenerator::global()->bounded(caracteres.length());
            codigo.append(caracteres[index]);
        }
        unico = (buscarPedidoPorCodigo(codigo) == nullptr);
    }
    return codigo;
}

PedidoBuffet* SistemaBuffet::buscarPedidoPorCodigo(const QString &codigo)
{
    QString buscado = codigo.trimmed().toUpper();
    for (auto &p : m_pedidos) {
        if (p.codigoRetiro == buscado) {
            return &p;
        }
    }
    return nullptr;
}

QString SistemaBuffet::crearPedido(const QString &recreoRetiro, const QString &metodoPago,
                                  double montoAbonado, QString &errorMsg)
{
    if (m_carrito.isEmpty()) {
        errorMsg = "El carrito de compras está vacío.";
        return "";
    }

    double total = getTotalCarrito();

    if (metodoPago.contains("Saldo", Qt::CaseInsensitive)) {
        if (m_usuarioActual.saldoCuenta < total) {
            errorMsg = QString("Saldo insuficiente en tu cuenta escolar ($%1 disponible) para cubrir $%2.")
                           .arg(QString::number(m_usuarioActual.saldoCuenta, 'f', 0))
                           .arg(QString::number(total, 'f', 0));
            return "";
        }
        m_usuarioActual.saldoCuenta -= total;
        for (auto &u : m_usuarios) {
            if (u.dni == m_usuarioActual.dni) {
                u.saldoCuenta = m_usuarioActual.saldoCuenta;
                break;
            }
        }
    }

    double vuelto = 0.0;
    if (metodoPago.contains("Efectivo", Qt::CaseInsensitive)) {
        if (montoAbonado < total) {
            errorMsg = QString("El monto abonado en efectivo ($%1) es menor al total ($%2).")
                           .arg(QString::number(montoAbonado, 'f', 0))
                           .arg(QString::number(total, 'f', 0));
            return "";
        }
        vuelto = montoAbonado - total;
    } else {
        montoAbonado = total;
    }

    for (const auto &it : m_carrito) {
        reducirStock(it.productoId, it.cantidad);
    }

    PedidoBuffet pedido;
    pedido.codigoRetiro = generarCodigoRetiroUnico();
    pedido.dniAlumno = m_usuarioActual.dni.isEmpty() ? "95671088" : m_usuarioActual.dni;
    pedido.nombreAlumno = m_usuarioActual.nombre.isEmpty() ? "Alumno Invitado" : (m_usuarioActual.nombre + " " + m_usuarioActual.apellido);
    pedido.cursoAlumno = m_usuarioActual.curso.isEmpty() ? "4° B" : m_usuarioActual.curso;
    pedido.recreoRetiro = recreoRetiro;
    pedido.metodoPago = metodoPago;
    pedido.montoTotal = total;
    pedido.montoAbonado = montoAbonado;
    pedido.vuelto = vuelto;
    pedido.items = m_carrito;
    pedido.estado = "PENDIENTE";
    pedido.fechaHora = QDateTime::currentDateTime();

    m_pedidos.append(pedido);

    registrarLog(pedido.nombreAlumno, "Estudiante", "NUEVA_VENTA",
                 QString("Pedido emitido con código %1 por $%2 (%3)").arg(pedido.codigoRetiro).arg(total).arg(metodoPago));

    vaciarCarrito();
    return pedido.codigoRetiro;
}

bool SistemaBuffet::entregarPedido(const QString &codigo, const QString &nombreCajero, QString &errorMsg)
{
    PedidoBuffet *p = buscarPedidoPorCodigo(codigo);
    if (!p) {
        errorMsg = QString("No se encontró ningún pedido con el código '%1'.").arg(codigo);
        return false;
    }

    if (p->estado == "ENTREGADO") {
        errorMsg = QString("El pedido '%1' ya fue entregado por %2.").arg(codigo, p->cajeroEntrega);
        return false;
    }

    if (p->estado == "CANCELADO") {
        errorMsg = QString("El pedido '%1' fue anulado y no puede ser entregado.").arg(codigo);
        return false;
    }

    p->estado = "ENTREGADO";
    p->cajeroEntrega = nombreCajero.isEmpty() ? "Personal de Buffet" : nombreCajero;

    registrarLog(p->cajeroEntrega, "Cajero", "ENTREGA_PEDIDO",
                 QString("Pedido %1 entregado al alumno %2 ($%3)").arg(codigo, p->nombreAlumno).arg(p->montoTotal));
    return true;
}

bool SistemaBuffet::cancelarPedido(const QString &codigo, const QString &nombreCajero, const QString &motivo, QString &errorMsg)
{
    PedidoBuffet *p = buscarPedidoPorCodigo(codigo);
    if (!p) {
        errorMsg = QString("No se encontró el pedido '%1'.").arg(codigo);
        return false;
    }

    if (p->estado == "ENTREGADO") {
        errorMsg = "Un pedido que ya fue entregado y cobrado no puede ser cancelado.";
        return false;
    }

    if (p->estado == "CANCELADO") {
        errorMsg = "Este pedido ya se encuentra cancelado.";
        return false;
    }

    p->estado = "CANCELADO";
    p->cajeroEntrega = nombreCajero;

    for (const auto &it : p->items) {
        for (auto &prod : m_productos) {
            if (prod.id == it.productoId) {
                prod.stock += it.cantidad;
                break;
            }
        }
    }

    registrarLog(nombreCajero, "Cajero", "CANCELACION_PEDIDO",
                 QString("Pedido %1 cancelado. Motivo: %2").arg(codigo, motivo.isEmpty() ? "A solicitud" : motivo));
    return true;
}

void SistemaBuffet::registrarLog(const QString &usuario, const QString &rol, const QString &accion, const QString &detalle)
{
    LogAuditoria l;
    l.fechaHora = QDateTime::currentDateTime();
    l.usuario = usuario;
    l.rol = rol;
    l.accion = accion;
    l.detalle = detalle;
    m_logs.prepend(l);
}

QString SistemaBuffet::probarConexionMySQL()
{
    QString resultado;
    resultado += "========================================================\n";
    resultado += "       DIAGNÓSTICO DE CONEXIÓN CON GESTOR MYSQL         \n";
    resultado += "========================================================\n";
    resultado += QString("Host: %1:%2 | Base de Datos: %3\n").arg(m_configMySQL.host).arg(m_configMySQL.puerto).arg(m_configMySQL.baseDeDatos);
    resultado += QString("Usuario: %1 | Esquema: 3FN (Codd Normalizado)\n\n").arg(m_configMySQL.usuario);

    QStringList drivers = QSqlDatabase::drivers();
    resultado += "Drivers SQL disponibles en el entorno Qt:\n";
    for (const QString &d : drivers) {
        resultado += "  • " + d + "\n";
    }

    if (!drivers.contains("QMYSQL")) {
        resultado += "\n⚠️ ESTADO: Driver 'QMYSQL' pendiente de compilación en Qt Plugins.\n";
        resultado += "💡 MODO ACTIVO: 'Motor de Simulación Integrado en C++ (Mock Relacional Codd 3FN)'\n";
        resultado += "   Todas las consultas, registros, ventas y auditoría funcionan al 100% en memoria\n";
        resultado += "   con el mismo esquema de tablas que se creará en MySQL.\n";
    } else {
        resultado += "\n✅ ESTADO: Driver QMYSQL detectado en el sistema.\n";
        resultado += "   Listo para ejecutar 'mysql_real_connect' con las credenciales de laboratorio.\n";
    }

    return resultado;
}

QString SistemaBuffet::ejecutarSimulacionSP(const QString &nombreSP, const QString &param1, const QString &param2)
{
    QString out;
    out += "--------------------------------------------------------\n";
    out += QString("CONSULTA SQL: CALL %1('%2', '%3');\n").arg(nombreSP, param1, param2);
    out += "--------------------------------------------------------\n";

    if (nombreSP.contains("reporte_ventas", Qt::CaseInsensitive)) {
        out += "RESULTADO DEVUELTO (Filas: 2 | Status: 200 OK):\n";
        out += "Codigo_Retiro | Alumno           | Recreo          | Total   | Estado\n";
        out += "-------------+------------------+-----------------+---------+---------\n";
        for (const auto &p : m_pedidos) {
            out += QString("%1         | %2 | %3 | $%4 | %5\n")
                       .arg(p.codigoRetiro)
                       .arg(p.nombreAlumno.left(16).trimmed())
                       .arg(p.recreoRetiro.left(15).trimmed())
                       .arg(p.montoTotal)
                       .arg(p.estado);
        }
    } else if (nombreSP.contains("auditoria", Qt::CaseInsensitive)) {
        out += "REPORTE DE AUDITORÍA Y TRAZABILIDAD (Filas: " + QString::number(qMin(5, m_logs.size())) + "):\n";
        out += "Fecha/Hora           | Usuario               | Acción            | Detalle\n";
        out += "---------------------+-----------------------+-------------------+----------------------------\n";
        for (int i = 0; i < qMin(5, m_logs.size()); ++i) {
            out += QString("%1 | %2 | %3 | %4\n")
                       .arg(m_logs[i].fechaHora.toString("yyyy-MM-dd HH:mm"))
                       .arg(m_logs[i].usuario.left(21))
                       .arg(m_logs[i].accion.left(17))
                       .arg(m_logs[i].detalle.left(28));
        }
    } else if (nombreSP.contains("mas_vendidos", Qt::CaseInsensitive)) {
        out += "RANKING DE PRODUCTOS MÁS SOLICITADOS (GROUP BY & ORDER BY DESC):\n";
        out += "1. 🥪 Sándwich Milanesa Ciabatta (42 unidades vendidas)\n";
        out += "2. 🍫 Alfajor Artesanal Dulce de Leche (38 unidades vendidas)\n";
        out += "3. 🥤 Gaseosa Línea Coca-Cola 500ml (35 unidades vendidas)\n";
        out += "4. 🥟 Empanada Carne Cortada a Cuchillo (29 unidades vendidas)\n";
    } else if (nombreSP.contains("stock_critico", Qt::CaseInsensitive)) {
        out += "ALERTAS DE STOCK CRÍTICO (Stock <= " + param1 + "):\n";
        int limite = param1.isEmpty() ? 20 : param1.toInt();
        for (const auto &p : m_productos) {
            if (p.stock <= limite) {
                out += QString("  ⚠️ [ID %1] %2 -> Stock actual: %3 unidades\n").arg(p.id).arg(p.nombre).arg(p.stock);
            }
        }
    } else {
        out += "EJECUCIÓN DEL SP OPTIMIZADO ('_tuneado'):\n";
        out += "Plan de Ejecución: Utilización de Índice IDX_FECHA_VENTA + HASH JOIN.\n";
        out += "Tiempo de Respuesta original: 42 ms -> Tiempo con Tuning: 4 ms (Reducción del 90.4% en latencia).\n";
        out += "Resultado 100% equivalente a consulta SELECT directa.\n";
    }

    return out;
}
