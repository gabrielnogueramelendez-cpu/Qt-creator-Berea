# Guía del código para la expo (versión solo BDD)

Esta versión funciona **únicamente con MySQL**. No hay datos de ejemplo en el código: productos, usuarios, pedidos y menú se leen de la BDD. Si no hay conexión, el programa muestra un error y se cierra.

Antes de abrirla en una compu nueva:

1. Correr los scripts `database/03` a `06` en MySQL.
2. Poner la contraseña de root de esa compu en `BDD_CLAVE` (`app/basedatos.cpp`, línea 29).

## Cómo arranca

`main.cpp` → aplica los colores → `conectar_bdd()` → si conecta, abre `LoginWindow`.

## Qué hace cada archivo

| Archivo | Pantalla | Qué usa de la BDD |
|---|---|---|
| `main.cpp` | — | `conectar_bdd()` |
| `basedatos.cpp` | — | Todas las consultas. Una función por consulta, como en el material 006 |
| `sistemabuffet.cpp` | — | Guarda en memoria lo que leyó: `usuario`, `productos`, `pedidos`, `carrito`, `menuHoy` |
| `loginwindow.cpp` | Iniciar sesión | `sp_iniciar_sesion`, `sp_acceso_personal` |
| `registerdialog.cpp` | Crear cuenta | `sp_registrar_usuario` |
| `mainwindow.cpp` | Catálogo | `cargarDesdeBDD()` |
| `productdetaildialog.cpp` | Detalle de producto | Nada: usa el producto ya leído |
| `carritodialog.cpp` | Mi carrito | Nada: el carrito está en memoria hasta pagar |
| `pagodialog.cpp` | Pago | `sp_registrar_venta`, `sp_agregar_producto_carrito`, `sp_agregar_menu_carrito`, `sp_generar_retiro` |
| `comprobantedialog.cpp` | Comprobante | Nada: busca el pedido recién leído |
| `perfildialog.cpp` | Mi perfil | `sp_cargar_saldo`, `cargarDesdeBDD()` |
| `cajerowindow.cpp` | Mostrador | `cargarDesdeBDD()`, `sp_actualizar_estado_retiro` |
| `adminwindow.cpp` | Administración | `sp_alta_producto`, `sp_modificar_stock`, `sp_modificar_precio`, `sp_baja_producto`, `obtener_logs` |
| `devwindow.cpp` | Desarrollador | Ejecuta el `CALL` que se escribe en pantalla |

Cada `.cpp` empieza con un bloque que dice lo mismo, para tenerlo a mano en Qt Creator.

## Recorrido de un pedido

1. **Login** (`on_btnLogin_clicked`): `sp_iniciar_sesion` devuelve el `UserID` y `cargarUsuarioDesdeBDD()` llena `SistemaBuffet::usuario`.
2. **Catálogo** (`MainWindow`): `cargarDesdeBDD()` lee productos, pedidos y el menú de hoy.
3. **Ver producto** (`ProductDetailDialog`): `agregarAlCarrito()` suma el producto a `SistemaBuffet::carrito`.
4. **Pagar** (`on_btnConfirmarPedido_clicked`): controla los datos y llama a `guardarPedidoEnBDD()`:
   - `START TRANSACTION`
   - `sp_registrar_venta` → `VentaID`
   - un `sp_agregar_producto_carrito` por producto (descuenta stock)
   - `sp_generar_retiro` con el código de 5 caracteres
   - `COMMIT`, o `ROLLBACK` si algo falló
5. **Comprobante**: muestra el código de retiro.
6. **Caja** (`on_btnEntregarPedido_clicked`): `sp_actualizar_estado_retiro` lo pasa a ENTREGADO.

## Cómo se lee lo que devuelve un SP

`leer_primera_fila()` devuelve una lista de textos, una posición por columna:

- **Lista vacía:** la consulta falló.
- **`respuesta[0] == "ERROR"`:** el SP entró en su `EXIT HANDLER`. `respuesta[1]` es el código de MySQL y `respuesta[2]` la descripción.
- **Cualquier otra cosa:** los datos. Por ejemplo, `respuesta[0]` es el `UserID` en el login.

Para varias filas se usa `mysql_fetch_row()` dentro de un `while`, como en `cargarProductos()`.

## Interruptores

Cada regla de negocio tiene un comentario `// INTERRUPTOR ...` en el código. Se busca con Ctrl+Shift+F (buscar `INTERRUPTOR`).

| Regla | Archivo y función | Qué comentar |
|---|---|---|
| Horario de retiro (recreo automático) | `pagodialog.cpp`: constructor y `on_btnConfirmarPedido_clicked` | 1 de 2: `ui->comboRecreo->setEnabled(false);` (constructor). 2 de 2: comentar `int franjaID = franjaDisponible();` y descomentar la línea de abajo (`currentIndex()`). El `if (franjaID == 0)` queda activo: obliga a elegir un recreo |
| Monto en efectivo insuficiente | `pagodialog.cpp`: `on_btnConfirmarPedido_clicked` | El `if` de "Monto Insuficiente" (4 líneas). Ojo: el vuelto puede dar negativo |
| 4 dígitos de Mercado Pago | `pagodialog.cpp`: `on_btnConfirmarPedido_clicked` | El `if` de "Comprobante Inválido" (4 líneas) |
| Saldo insuficiente | `pagodialog.cpp`: `on_btnConfirmarPedido_clicked` | El `if` de "Saldo Insuficiente" (4 líneas). El SP `sp_registrar_venta` igual lo controla en la BDD |
| Control de stock | `productdetaildialog.cpp`: `maximoPermitido` y `carritodialog.cpp`: `on_btnModificarCantidad_clicked` | El `if` de 3 líneas en cada una. Los SP de la BDD igual rechazan si no hay stock |
| Límite de 10 por producto | `productdetaildialog.cpp` (`LIMITE_POR_PEDIDO`) y `carritodialog.cpp` (constructor y `on_btnModificarCantidad_clicked`) | Comentar `setRange(1, 10)`; cambiar el 10 por 99 en `LIMITE_POR_PEDIDO` y en `int maximo = 10;` (no se puede solo comentando: es un valor) |
| Sin menú el fin de semana | `sistemabuffet.cpp`: `cargarMenuDelDia` | No es un interruptor: lo decide `sp_menu_del_dia` en la BDD (no devuelve nada sábado y domingo) |

## Si la profe pide cambiar algo

| Pedido | Dónde |
|---|---|
| Límite de 10 unidades por producto | `productdetaildialog.cpp`: `LIMITE_POR_PEDIDO`, y `carritodialog.cpp`: `setRange(1, 10)` |
| Horarios en que se puede pedir | `pagodialog.cpp`: `franjaDisponible()` |
| Contraseña de MySQL | `basedatos.cpp`: `BDD_CLAVE` |
| IDs de métodos de pago (1, 2, 21) | `pagodialog.cpp`: `on_btnConfirmarPedido_clicked` |
| Un mensaje de error | Buscar el texto del mensaje en el `.cpp` de esa pantalla (`QMessageBox::warning`) |
| Una consulta nueva | Agregar una función en `basedatos.cpp` y su línea en `basedatos.h`, copiando una que ya exista |

## Diferencias con la versión simplificada

- No están las listas de ejemplo (estudiantes, profesores, usuarios, empleados, productos, menús) ni los bloques `MODO SIN BDD`.
- La descripción de cada producto ahora sale de la columna `Descripcion_Producto`.
- El comprobante muestra con cuánto pagó en efectivo y el vuelto real. En la simplificada, con BDD, mostraba el monto justo y vuelto $0.
- Las pantallas y lo que hace cada botón no cambiaron.
