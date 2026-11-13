# Informe de mercado, competencia y ventaja competitiva

## 1. Panorama competitivo

**Competidores directos: plataformas SaaS para buffets escolares** (por ejemplo OrderEAT)
- Fortalezas: interfaz móvil moderna y catálogo digital.
- Limitaciones: cobran comisión por transacción y abono mensual, y dependen 100 % de la nube y de los datos móviles de los estudiantes. Sin señal en el colegio, no funcionan.

**Competidores indirectos: sistemas POS comerciales**
- Fortalezas: tickets impresos y control de inventario general.
- Limitaciones: licencias y hardware caros. No están pensados para los picos del recreo ni tienen validación por código de 5 caracteres.

**Alternativa tradicional: atención manual y cobro en efectivo**
- Fortaleza: sin costo de software.
- Limitaciones: filas de 7 minutos, pérdidas de hasta 30 % en ventas, errores con el efectivo y ningún registro de las transacciones.

## 2. Matriz comparativa

| Criterio | El Buffet de Alejandro | SaaS comercial (OrderEAT) | POS comercial | Manual / efectivo |
|---|---|---|---|---|
| Arquitectura | App de escritorio C++/Qt + MySQL (3 schemas, 25 tablas en 3FN) | App móvil o web en la nube | Software propietario en terminales | Papel o planillas |
| Retiro express | Código alfanumérico único de 5 caracteres | QR o notificación push | Número de ticket impreso | Fila por orden de llegada |
| Validación en caja | Menos de 5 s (entrega en menos de 30 s) | Según la red y los datos móviles | 15 a 30 s por impresión y cobro | 30 a 60 s por contar efectivo y dar vuelto |
| Control de stock | Descuento automático en MySQL y bloqueo de agotados | Sincronización sujeta a la nube | Inventario general de tienda | Control visual |
| Auditoría | Registro en `Log_Auditoria` | Logs en servidores externos | Cierre de caja (Z) | Ninguna |
| Costo de licencias | $0 (desarrollo propio) | Comisión + abono mensual | Licencia por terminal + hardware | $0, con pérdida de 20–30 % en ventas |

## 3. Propuesta única de valor

Un sistema de gestión de escritorio en C++/Qt y MySQL que elimina las filas del buffet escolar. Baja la atención en mostrador a menos de 30 segundos con códigos únicos de 5 caracteres, sin costo de licencias y con auditoría automática.

## Ventaja competitiva

1. **Despacho express con código de 5 caracteres.** Cada pedido confirmado genera un código único de letras y números. En caja se valida en menos de 5 segundos y la entrega total lleva menos de 30.
2. **$0 en licencias y comisiones.** Desarrollo propio sobre herramientas open source (Qt Creator y MySQL Community Server): sin comisión por venta ni cuota mensual.
3. **Auditoría automática.** La tabla `Log_Auditoria` registra inicios y cierres de sesión y cada operación comercial con fecha y hora.
4. **Inventario en tiempo real.** El stock se descuenta en menos de 2 segundos después de cada venta, y no se pueden vender productos agotados.

> Nota: los Stored Procedures están en `database/05_Backend_Grupo1.sql`. La consola de desarrollador de la app todavía los simula en memoria.
