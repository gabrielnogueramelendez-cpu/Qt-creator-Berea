Aviso para el chat "BDD mejoras y recomendaciones": cambios hechos en la BDD desde el chat de Qt (14/11/2026)

1. 03_ModeloSQL
   - Usuario: nueva columna `Saldo DECIMAL(10,2) NOT NULL DEFAULT 0 CHECK (Saldo >= 0)`.
   - Se borraron los dos índices (`idx_log_fecha` e `idx_venta_fecha`): no se vieron en clase. El encabezado ahora dice "schemas, tablas y restricciones".

2. 04_DatosSQL
   - Tipo_Evento 21 'Carga de Saldo' (el encabezado ahora dice 21).
   - Metodo_Pago 21 'Saldo de Cuenta Escolar' (el encabezado ahora dice 21).
   - `UPDATE Usuario SET Saldo = 10000 WHERE UserID = 1;` como saldo de prueba.

3. 05_Backend
   - SP nuevo `sp_cargar_saldo(p_UserID, p_Monto)` (SP 4b): solo suma montos > 0 y solo a usuarios con DNI (estudiantes). Deja el evento 21 en Log_Auditoria y devuelve Filas_Actualizadas y Saldo.
   - `sp_generar_retiro`: si la venta usa el método 21, crea el retiro solo si el saldo alcanza para el Monto_Total, y en ese caso descuenta el saldo (una sola vez, porque Retiro.VentaID es UNIQUE).
   - `sp_actualizar_estado_retiro`: al cancelar (estado 3) una venta pagada con el método 21, devuelve el saldo.
   - El encabezado suma "21 Carga de saldo" y "ID de Metodo_Pago usado: 21".

4. 06_PruebasSQL
   - P25 `sp_cargar_saldo` y P26 pago con saldo (casos i a iv) al final del archivo.
   - Falta pasarlas al Excel `06_PruebasSQL (1).xlsx`.

Verificación: 03 -> 04 -> 05 -> 06 se ejecutaron completos en un MySQL 8.0 de prueba. 03, 04 y 05 corrieron sin errores (esto fue antes de borrar los índices). P26 dio un saldo de 5000 -> 2200 al pagar, sin retiro cuando el saldo no alcanza, y 5000 de nuevo al cancelar.

## Segunda tanda: acceso del personal (PIN y token)

1. 03_ModeloSQL: en Empleado, `PIN CHAR(4) NULL CHECK (CHAR_LENGTH(PIN) = 4)` y `Token_Acceso VARCHAR(30) NULL UNIQUE`.
2. 04_DatosSQL
   - Puesto 21 'Desarrollador de Sistemas', Usuario 31 'soporte.dev', Empleado 21 'Soporte Técnico' con token 'DEV-BEREA-2026'. Los encabezados ahora dicen (21), (31) y (21).
   - PIN para los usuarios 21 a 26, 28 y 29 (cajeros y atención al mostrador). El encargado (27) y la administrativa (30) no tienen PIN y entran con sp_iniciar_sesion.
3. 05_Backend: SP nuevo `sp_acceso_personal(p_Apodo, p_Contrasena, p_Clave)` (SP 2b). p_Clave es el PIN o el token. Deja el evento 1 o el 3, igual que sp_iniciar_sesion.
4. 06_PruebasSQL: P27 `sp_acceso_personal` (casos i a iv). Falta pasarla al Excel.

Verificación: 03 -> 04 -> 05 corrieron sin errores en un MySQL 8.0 de prueba, ya sin los índices. En P27 la cajera con su PIN y el desarrollador con su token devuelven 1 fila. Un PIN incorrecto o NULL, un estudiante y el encargado devuelven 0 filas, y eso deja el evento 3 en el log. Un PIN de 2 dígitos lo rechaza el CHECK.

## Tercera tanda: precio de producto

1. 04_DatosSQL: Tipo_Evento 22 'Modificación de Precio de Producto'. El encabezado ahora dice (22).
2. 05_Backend: SP nuevo `sp_modificar_precio(p_UserID, p_ProductoID, p_NuevoPrecio)` (SP 6b), igual que sp_modificar_stock. Deja el evento 22 en el log. El encabezado suma "22 Modificación de precio".
3. 06_PruebasSQL: P28 `sp_modificar_precio` (casos i a iv). Falta pasarla al Excel.

Verificación: 03 -> 04 -> 05 corrieron sin errores. P28 dio 1 fila al cambiar 2800 -> 3000, 0 filas con un producto inexistente y el error 3819 con un precio negativo, y un solo evento 22 en el log.

## Cuarta tanda: se saca sp_cambiar_contrasena

Releímos la consigna (punto 5). Pide como mínimo 8 reportes, 1 de auditoría y 2 tuneados, y para las funcionalidades no da un número fijo. sp_cambiar_contrasena no lo usa ninguna pantalla de Qt y no hace falta para la consigna, así que se saca.

1. 05_Backend: se borró el SP 4 (sp_cambiar_contrasena). El de saldo pasó a ser el SP 4. Del encabezado se sacó "5 Cambio de contraseña".
2. 04_DatosSQL: el Tipo_Evento 5 queda en el catálogo, igual que el 17 al 20, que tampoco los usa ningún SP. El comentario ahora dice "Los IDs 1 a 4, 6 a 16, el 21 y el 22".
3. 06_PruebasSQL: se borró P04 y las pruebas se renumeraron. La vieja P05 ahora es P04, y así hasta la P28, que ahora es P27. Las referencias de los tuneados también cambiaron: "que P14-i" y "que P17-i".
   - En el Excel hay que borrar la fila de sp_cambiar_contrasena y renumerar las demás.

Verificación: 03 -> 04 -> 05 corrieron sin errores y quedan 26 SPs. 06 da los mismos errores que antes, que son todos casos de prueba de error.

## Quinta tanda: comentarios y Excel

1. 03, 04, 05 y 06: se borraron los comentarios que solo explicaban. Quedaron:
   - las barras separadoras y los títulos de bloque (schemas, tablas, SPs, reportes, P01..P27);
   - las enumeraciones: los IDs de 05, el "-- 21" de los inserts, los casos i a iv de 06 y los "Paso 1/2/3" de los tuneados.
2. 06: P01 ahora espera UserID 32 (el 31 es soporte.dev). El caso ii de P25 dice "No se crea el retiro (0 filas)".
3. 06_PruebasSQL (1).xlsx: se rehizo desde una corrida real de 03 -> 04 -> 05 -> 06 en MySQL 8.0. Tiene 27 pruebas y 108 casos, todos en PASS, con el mismo formato que antes. Las redacciones viejas de H e I se conservaron.

## Sexta tanda: estudiantes para probar el registro

1. 04_DatosSQL: se agregaron 3 estudiantes sin usuario, para que se pueda probar sp_registrar_usuario en la exposición. Los 20 anteriores ya tenían usuario.
   - 48715903 Pablo Linares (6° 1ra), 48902617 Cynthia Berea (6° 2da), 49031458 Cristian Dávalos (6° 3ra). DNI y fecha de nacimiento son inventados.
2. No cambia ningún UserID ni ningún resultado de 06: la planilla de pruebas sigue igual.
