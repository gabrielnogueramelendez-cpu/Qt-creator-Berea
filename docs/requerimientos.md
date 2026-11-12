# Requerimientos

## Requerimientos funcionales

| ID | Nombre | Descripción |
|---|---|---|
| RF01 | Catálogo y selección de productos | Los estudiantes ven el menú del buffet con el stock actualizado. |
| RF02 | Registro de pedidos (Click & Collect) | El estudiante reserva su pedido de manera anticipada desde la app. |
| RF03 | Módulo de cobro | El sistema procesa el pago antes o al confirmar la reserva. |
| RF04 | Código único de retiro | Se genera un código alfanumérico único de 5 caracteres por pedido confirmado. |
| RF05 | Control de stock (BDD) | Se descuenta el stock de los productos reservados y se bloquea la compra sin existencias. |
| RF06 | Validación de retiro (personal del buffet) | El personal ingresa el código de 5 caracteres para verificar y entregar el pedido. |

## Requerimientos no funcionales

### Rendimiento
- **RNF01. Tiempo de validación:** validar el código no debe superar los **5 segundos**, para entregar en menos de 30 segundos.
- **RNF02. Sincronización de stock:** los cambios de stock se reflejan en la app en menos de 2 segundos.

### Usabilidad
- **RNF03. Mobile first:** reserva completa en un máximo de 3 pasos.
- **RNF04. Legibilidad del código:** el código de retiro se muestra con alto contraste y tamaño grande.

### Seguridad
- **RNF05. Autenticación e integridad:** el código caduca una vez validado, para evitar duplicaciones o fraude.
- **RNF06. Protección de datos de pago:** el módulo de cobro cumple estándares de cifrado.

## Historias de usuario

**HU01. Realizar reserva de pedido.** Como estudiante, quiero seleccionar productos del menú y reservar mi pedido desde la app, para evitar la fila.
1. Permite seleccionar uno o varios productos disponibles.
2. Muestra el total y el resumen antes de confirmar.
3. No permite reservar productos sin stock.

**HU02. Procesar pago del pedido.** Como estudiante, quiero pagar mi pedido desde la app, para dejar la compra hecha de antemano.
1. Ofrece los medios de pago configurados.
2. Al confirmarse el pago, el pedido pasa a "Pagado / Listo para preparar".
3. Si el pago falla, se notifica y no se descuenta stock.

**HU03. Generar y presentar código único de retiro.** Como estudiante, quiero recibir un código único de 5 caracteres al confirmar, para mostrarlo en el buffet.
1. El código es único por transacción.
2. Queda guardado en "Mis pedidos" para consultarlo sin conexión.

**HU04. Validar código y entregar pedido.** Como personal del buffet, quiero ingresar el código del estudiante para entregar en menos de 30 segundos.
1. La pantalla muestra el detalle exacto del pedido y el estado del pago.
2. Al entregar, el código queda "Entregado" y no se puede reutilizar.
