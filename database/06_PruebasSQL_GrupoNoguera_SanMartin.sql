-- ============================================================================
-- PROYECTO INFORMÁTICO I - SECCIÓN BASES DE DATOS
-- El Buffet de Alejandro - PUNTO 6: SCRIPT DE PRUEBAS DE LOS STORED PROCEDURES
-- Por cada SP hay 4 pruebas (consigna 6.b):
--   i.   Éxito (flujo normal)
--   ii.  Sin coincidencias (valores que no existen)
--   iii. Parámetros NULL / vacíos / límite
--   iv.  Consistencia lógica: se compara contra un SELECT directo
-- ============================================================================

SET NAMES utf8mb4;

-- ############################################################################
-- FUNCIONALIDADES - USUARIOS
-- ############################################################################
USE schema_usuarios;

INSERT INTO Estudiante (DNI, CUIT, Nombre_Estudiante, Apellido_Estudiante, Fecha_Nacimiento, ID_Colegio, ID_Curso)
VALUES ('50111222', '20-50111222-4', 'Tomás', 'Prueba', '2010-05-10', 1, 13);

-- ---------------- P01 sp_registrar_usuario ----------------
-- i. Éxito: devuelve el usuario nuevo (UserID 32, Nombre_Rol Estudiante)
CALL sp_registrar_usuario('50111222', 'tomaspru10', 'Tomas#2026');
-- i-bis. Éxito con el DNI de un profesor: UserID 33, Nombre_Rol Profesor
CALL sp_registrar_usuario('25418763', 'pablolin', 'Pablo#2026');
-- ii. DNI que no es de ningún estudiante ni profesor: ERROR 1452 (clave foránea)
CALL sp_registrar_usuario('99999999', 'nadie', 'Clave#1');
-- iii. Apodo NULL: ERROR 1048 (la columna no acepta NULL)
CALL sp_registrar_usuario('50111222', NULL, NULL);
-- iv. Consistencia: los dos usuarios quedaron guardados con su rol y su alta está en el log
SELECT u.UserID, u.Apodo, r.Nombre_Rol, l.Detalle
FROM Usuario u INNER JOIN Rol r ON u.RolID = r.RolID
INNER JOIN Log_Auditoria l ON u.UserID = l.UserID
WHERE (u.DNI = '50111222' OR u.DNI_Profesor = '25418763') AND l.Tipo_Evento_ID = 4;

-- ---------------- P02 sp_iniciar_sesion ----------------
-- i. Éxito: devuelve UserID 1, Tipo_Usuario Estudiante
CALL sp_iniciar_sesion('davidmel09', 'Davidmel#2026');
-- i-bis. Éxito con una cajera: Tipo_Usuario Personal de caja
CALL sp_iniciar_sesion('marcela.ibanez', 'MarcelaBuffet#2026');
-- ii. Usuario inexistente: 0 filas (y queda un "Fallo de Autenticación" en el log)
CALL sp_iniciar_sesion('noexiste', 'x');
-- iii. NULL: 0 filas, sin error
CALL sp_iniciar_sesion(NULL, NULL);
-- iv. Consistencia: hoy hay 2 inicios exitosos (evento 1) y 2 fallos (evento 3)
SELECT Tipo_Evento_ID, COUNT(*) AS Cantidad FROM Log_Auditoria
WHERE Fecha = CURDATE() AND Tipo_Evento_ID IN (1, 3) GROUP BY Tipo_Evento_ID;

-- ---------------- P03 sp_cerrar_sesion ----------------
-- i. Éxito: Sesiones_Cerradas = 1
CALL sp_cerrar_sesion(1);
-- ii. Usuario inexistente: Sesiones_Cerradas = 0
CALL sp_cerrar_sesion(9999);
-- iii. NULL: Sesiones_Cerradas = 0
CALL sp_cerrar_sesion(NULL);
-- iv. Consistencia: hay 1 cierre de sesión del usuario 1
SELECT COUNT(*) AS Cierres FROM Log_Auditoria WHERE Tipo_Evento_ID = 2 AND UserID = 1;


-- ############################################################################
-- FUNCIONALIDADES - COMERCIAL
-- UserID 27 = Alejandro Funes (Encargado General) hace las tareas de administración
-- UserID 21 = Marcela Ibáñez (cajera, EmpleadoID 1)
-- ############################################################################
USE schema_comercial;

-- ---------------- P04 sp_alta_producto ----------------
-- i. Éxito: devuelve el ProductoID 21
CALL sp_alta_producto(27, 11, 'Jugo de Naranja Exprimido', 'Vaso de 300 ml', 30, 1500.00);
-- ii. Categoría inexistente: ERROR 1452 (clave foránea)
CALL sp_alta_producto(27, 999, 'Producto X', NULL, 10, 100.00);
-- iii. Precio negativo: ERROR 3819 (no cumple el CHECK)
CALL sp_alta_producto(27, 11, 'Producto Y', NULL, 10, -5);
-- iv. Consistencia
SELECT ProductoID, Nombre_Producto, Stock_Producto, Precio_Producto, Activo FROM Producto WHERE ProductoID = 21;

-- ---------------- P05 sp_modificar_stock ----------------
-- i. Éxito: el stock del producto 21 pasa de 30 a 50 (Filas_Actualizadas = 1)
CALL sp_modificar_stock(27, 21, 50);
-- ii. Producto inexistente: Filas_Actualizadas = 0
CALL sp_modificar_stock(27, 9999, 10);
-- iii. Stock negativo: ERROR 3819 (no cumple el CHECK)
CALL sp_modificar_stock(27, 21, -1);
-- iv. Consistencia
SELECT Stock_Producto FROM Producto WHERE ProductoID = 21;

-- ---------------- P06 sp_baja_producto ----------------
-- i. Éxito: Filas_Actualizadas = 1
CALL sp_baja_producto(27, 21);
-- ii. Producto inexistente: Filas_Actualizadas = 0
CALL sp_baja_producto(27, 9999);
-- iii. Límite: dar de baja dos veces el mismo producto: Filas_Actualizadas = 0
CALL sp_baja_producto(27, 21);
-- iv. Consistencia: Activo = 0 y la fila sigue existiendo
SELECT ProductoID, Activo FROM Producto WHERE ProductoID = 21;

-- ---------------- P07 sp_modificar_menu_dia ----------------
-- El menú es fijo (un menú por día hábil). El día lo pasa la app; en las pruebas se usa
-- el miércoles: ese menú pasa a ser 'Milanesa con ensalada', $4500, stock 30 (lo usa P10)
SET @dia_hoy = 'Miércoles';
-- i. Éxito: Filas_Actualizadas = 1
CALL sp_modificar_menu_dia(27, @dia_hoy, 'Milanesa con ensalada', 4500.00, 30);
-- ii. Sábado (no hay menú): no modifica nada y devuelve 0 filas
CALL sp_modificar_menu_dia(27, 'Sábado', 'Plato X', 4000.00, 10);
-- iii. Precio NULL: ERROR 1048 (la columna no acepta NULL)
CALL sp_modificar_menu_dia(27, @dia_hoy, 'Plato X', NULL, 10);
-- iv. Consistencia: siguen siendo 5 menús, uno por día, y el de hoy quedó modificado
SELECT COUNT(*) AS Menus, COUNT(DISTINCT Dia_Semana) AS Dias,
       (SELECT Plato_menu FROM Menu_Del_Dia WHERE Dia_Semana = @dia_hoy) AS Plato_De_Hoy
FROM Menu_Del_Dia;

-- ---------------- P08 sp_registrar_venta ----------------
-- i. Éxito: el alumno 3 compra en mostrador con la cajera 1. Devuelve VentaID 51
CALL sp_registrar_venta(3, 1, 1);
-- ii. Usuario inexistente: ERROR 1452 (clave foránea)
CALL sp_registrar_venta(9999, 1, 1);
-- iii. Método de pago NULL: ERROR 1048 (la columna no acepta NULL)
CALL sp_registrar_venta(3, NULL, NULL);
-- iv. Consistencia: la venta 51 existe con monto 0 y fecha de hoy
SELECT VentaID, UserID, EmpleadoID, Monto_Total, Fecha = CURDATE() AS Es_De_Hoy FROM Venta WHERE VentaID = 51;

-- ---------------- P09 sp_agregar_producto_carrito ----------------
-- i. Éxito: 2 medialunas (producto 5, $800 c/u). Filas_Agregadas = 1, Monto_Total 1600
CALL sp_agregar_producto_carrito(51, 5, 2);
-- ii. Producto inexistente: Filas_Agregadas = 0
CALL sp_agregar_producto_carrito(51, 9999, 1);
-- iii. Límite: más cantidad que el stock: Filas_Agregadas = 0
CALL sp_agregar_producto_carrito(51, 5, 100000);
-- iii-bis. Cantidad 0: ERROR 3819 (no cumple el CHECK)
CALL sp_agregar_producto_carrito(51, 5, 0);
-- iv. Consistencia: monto de la venta = suma del carrito; stock de medialunas 90 -> 88
SELECT v.Monto_Total, SUM(c.Cantidad * c.Precio_Unitario) AS Suma_Carrito,
       (SELECT Stock_Producto FROM Producto WHERE ProductoID = 5) AS Stock_Medialunas
FROM Venta v INNER JOIN Carrito c ON v.VentaID = c.VentaID WHERE v.VentaID = 51 GROUP BY v.Monto_Total;

-- ---------------- P10 sp_agregar_menu_carrito ----------------
SET @menu_hoy = (SELECT Menu_Del_DiaID FROM Menu_Del_Dia WHERE Dia_Semana = @dia_hoy);
SET @menu_otro = (SELECT MIN(Menu_Del_DiaID) FROM Menu_Del_Dia WHERE Dia_Semana <> @dia_hoy);
-- i. Éxito: 1 menú de hoy ($4500, P07). Filas_Agregadas = 1, Monto_Total 6100
CALL sp_agregar_menu_carrito(51, @menu_hoy, 1, @dia_hoy);
-- ii. Menú de otro día de la semana: Filas_Agregadas = 0
CALL sp_agregar_menu_carrito(51, @menu_otro, 1, @dia_hoy);
-- iii. Cantidad NULL: Filas_Agregadas = 0
CALL sp_agregar_menu_carrito(51, @menu_hoy, NULL, @dia_hoy);
-- iv. Consistencia: monto = suma del carrito; stock del menú de hoy 30 -> 29
SELECT v.Monto_Total, SUM(c.Cantidad * c.Precio_Unitario) AS Suma_Carrito,
       (SELECT Stock_menu FROM Menu_Del_Dia WHERE Menu_Del_DiaID = @menu_hoy) AS Stock_Menu
FROM Venta v INNER JOIN Carrito c ON v.VentaID = c.VentaID WHERE v.VentaID = 51 GROUP BY v.Monto_Total;

-- ---------------- P11 sp_generar_retiro ----------------
-- i. Éxito: retiro en el receso del almuerzo (franja 3) con el código AB123
CALL sp_generar_retiro(51, 3, 'AB123');
-- ii. Venta inexistente: 0 filas (no se crea ningún retiro)
CALL sp_generar_retiro(9999, 3, 'ZZ999');
-- iii. Límite: generar el retiro dos veces para la misma venta: ERROR 1062 (duplicado)
CALL sp_generar_retiro(51, 3, 'CD456');
-- iv. Consistencia: hay un solo retiro PENDIENTE para la venta 51, y ya no se le pueden agregar productos
SELECT RetiroID, Estado_Retiro_ID, ID_Franja, Codigo_Retiro FROM Retiro WHERE VentaID = 51;
CALL sp_agregar_producto_carrito(51, 5, 1);

-- ---------------- P12 sp_actualizar_estado_retiro ----------------
-- i. Éxito: la cajera (UserID 21) entrega el retiro 51. Filas_Actualizadas = 1, Estado ENTREGADO
CALL sp_actualizar_estado_retiro(21, 51, 2);
-- ii. Retiro inexistente: 0 filas (no devuelve retiro)
CALL sp_actualizar_estado_retiro(21, 99999, 2);
-- iii. Límite: volver a cambiar un pedido ya entregado (doble entrega): Filas_Actualizadas = 0
CALL sp_actualizar_estado_retiro(21, 51, 3);
-- iv. Consistencia: cancelar el retiro 46 (pendiente) devuelve el stock.
--     La venta 46 tenía 3 manzanas (producto 7): el stock pasa de 70 a 73.
SELECT Stock_Producto AS Stock_Antes FROM Producto WHERE ProductoID = 7;
CALL sp_actualizar_estado_retiro(21, 46, 3);
SELECT Stock_Producto AS Stock_Despues FROM Producto WHERE ProductoID = 7;


-- ############################################################################
-- REPORTES
-- ############################################################################
USE schema_comercial;

-- ---------------- P13 sp_reporte_ventas_por_categoria ----------------
CALL sp_reporte_ventas_por_categoria('2026-10-14', '2026-11-10');   -- i
CALL sp_reporte_ventas_por_categoria('2020-01-01', '2020-12-31');   -- ii: 0 filas
CALL sp_reporte_ventas_por_categoria(NULL, '2026-11-10');           -- iii: 0 filas
-- iv. SELECT directo: la recaudación total es igual a la suma de la columna Recaudacion_Total del reporte
SELECT SUM(c.Cantidad * c.Precio_Unitario) AS Total_Directo
FROM Carrito c INNER JOIN Venta v ON c.VentaID = v.VentaID INNER JOIN Retiro r ON v.VentaID = r.VentaID
WHERE v.Fecha BETWEEN '2026-10-14' AND '2026-11-10' AND r.Estado_Retiro_ID <> 3;

-- ---------------- P14 sp_reporte_top_productos ----------------
CALL sp_reporte_top_productos('2026-10-14', '2026-11-10');          -- i
CALL sp_reporte_top_productos('2020-01-01', '2020-12-31');          -- ii: 0 filas
CALL sp_reporte_top_productos('2026-11-10', '2026-10-14');          -- iii: 0 filas (rango invertido)
-- iv
SELECT c.ProductoID, SUM(c.Cantidad) AS Unidades
FROM Carrito c INNER JOIN Venta v ON c.VentaID = v.VentaID INNER JOIN Retiro r ON v.VentaID = r.VentaID
WHERE c.ProductoID IS NOT NULL AND v.Fecha BETWEEN '2026-10-14' AND '2026-11-10' AND r.Estado_Retiro_ID <> 3
GROUP BY c.ProductoID ORDER BY Unidades DESC, c.ProductoID;

-- ---------------- P15 sp_reporte_metodos_pago ----------------
CALL sp_reporte_metodos_pago('2026-10-14', '2026-11-10');           -- i
CALL sp_reporte_metodos_pago('2020-01-01', '2020-12-31');           -- ii: 0 filas
CALL sp_reporte_metodos_pago(NULL, NULL);                           -- iii: 0 filas
-- iv
SELECT v.Metodo_PagoID, COUNT(*) AS Transacciones, SUM(v.Monto_Total) AS Total
FROM Venta v INNER JOIN Retiro r ON v.VentaID = r.VentaID
WHERE v.Fecha BETWEEN '2026-10-14' AND '2026-11-10' AND r.Estado_Retiro_ID <> 3
GROUP BY v.Metodo_PagoID ORDER BY Total DESC;

-- ---------------- P16 sp_reporte_ventas_diarias ----------------
CALL sp_reporte_ventas_diarias('2026-10-14', '2026-11-10');         -- i
CALL sp_reporte_ventas_diarias('2020-01-01', '2020-12-31');         -- ii: 0 filas
CALL sp_reporte_ventas_diarias('2026-10-14', NULL);                 -- iii: 0 filas
-- iv
SELECT COUNT(DISTINCT v.Fecha) AS Dias, COUNT(*) AS Ventas, SUM(v.Monto_Total) AS Total
FROM Venta v INNER JOIN Retiro r ON v.VentaID = r.VentaID
WHERE v.Fecha BETWEEN '2026-10-14' AND '2026-11-10' AND r.Estado_Retiro_ID <> 3;

-- ---------------- P17 sp_reporte_top_estudiantes ----------------
CALL sp_reporte_top_estudiantes('2026-10-14', '2026-11-10');        -- i
CALL sp_reporte_top_estudiantes('2020-01-01', '2020-12-31');        -- ii: 0 filas
CALL sp_reporte_top_estudiantes(NULL, NULL);                        -- iii: 0 filas
-- iv
SELECT v.UserID, COUNT(*) AS Compras, SUM(v.Monto_Total) AS Total
FROM Venta v INNER JOIN Retiro r ON v.VentaID = r.VentaID
WHERE v.Fecha BETWEEN '2026-10-14' AND '2026-11-10' AND r.Estado_Retiro_ID <> 3
GROUP BY v.UserID HAVING COUNT(*) >= 2 ORDER BY Total DESC, v.UserID;

-- ---------------- P18 sp_reporte_rendimiento_empleados ----------------
CALL sp_reporte_rendimiento_empleados('2026-10-14', '2026-11-10');  -- i
CALL sp_reporte_rendimiento_empleados('2020-01-01', '2020-12-31');  -- ii: 0 filas
CALL sp_reporte_rendimiento_empleados('2026-11-10', '2026-10-14');  -- iii: 0 filas (rango invertido)
-- iv
SELECT v.EmpleadoID, COUNT(*) AS Ventas, SUM(v.Monto_Total) AS Total
FROM Venta v INNER JOIN Retiro r ON v.VentaID = r.VentaID
WHERE v.EmpleadoID IS NOT NULL AND v.Fecha BETWEEN '2026-10-14' AND '2026-11-10' AND r.Estado_Retiro_ID <> 3
GROUP BY v.EmpleadoID ORDER BY Total DESC;

-- ---------------- P19 sp_reporte_retiros_por_franja ----------------
CALL sp_reporte_retiros_por_franja('2026-10-14', '2026-11-10');     -- i
CALL sp_reporte_retiros_por_franja('2020-01-01', '2020-12-31');     -- ii: 0 filas
CALL sp_reporte_retiros_por_franja(NULL, '2026-11-10');             -- iii: 0 filas
-- iv: el total de pedidos del reporte tiene que dar 50
SELECT COUNT(*) AS Total_Retiros FROM Retiro r INNER JOIN Venta v ON r.VentaID = v.VentaID
WHERE v.Fecha BETWEEN '2026-10-14' AND '2026-11-10';

-- ---------------- P20 sp_reporte_demanda_menu_dia ----------------
CALL sp_reporte_demanda_menu_dia('2026-10-14', '2026-11-10');       -- i
CALL sp_reporte_demanda_menu_dia('2020-01-01', '2020-12-31');       -- ii: 0 filas
CALL sp_reporte_demanda_menu_dia(NULL, NULL);                       -- iii: 0 filas
-- iv
SELECT c.Menu_Del_DiaID, SUM(c.Cantidad) AS Vendidos
FROM Carrito c INNER JOIN Venta v ON c.VentaID = v.VentaID
INNER JOIN Retiro r ON c.VentaID = r.VentaID
WHERE c.Menu_Del_DiaID IS NOT NULL AND v.Fecha BETWEEN '2026-10-14' AND '2026-11-10' AND r.Estado_Retiro_ID <> 3
GROUP BY c.Menu_Del_DiaID ORDER BY Vendidos DESC, c.Menu_Del_DiaID;


-- ############################################################################
-- AUDITORÍA (los eventos de hoy los generaron las pruebas de arriba)
-- ############################################################################
USE schema_usuarios;

-- ---------------- P21 sp_reporte_auditoria ----------------
CALL sp_reporte_auditoria(CURDATE(), CURDATE(), NULL, NULL);        -- i: todos los eventos de hoy
CALL sp_reporte_auditoria(CURDATE(), CURDATE(), 21, NULL);          -- i-bis: solo los eventos de la cajera (UserID 21)
CALL sp_reporte_auditoria('2020-01-01', '2020-12-31', NULL, NULL);  -- ii: 0 filas
CALL sp_reporte_auditoria(NULL, NULL, 1, NULL);                     -- iii: 0 filas
-- iv: la cantidad de eventos de hoy es igual a la cantidad de filas del caso i
SELECT COUNT(*) AS Eventos_Hoy FROM Log_Auditoria WHERE Fecha = CURDATE();


-- ############################################################################
-- TUNING: el resultado del SP tuneado tiene que ser idéntico al original
-- ############################################################################
USE schema_comercial;

-- ---------------- P22 sp_reporte_top_productos_tuneado ----------------
CALL sp_reporte_top_productos_tuneado('2026-10-14', '2026-11-10');  -- i: mismas filas que P14-i
CALL sp_reporte_top_productos_tuneado('2020-01-01', '2020-12-31');  -- ii: 0 filas
CALL sp_reporte_top_productos_tuneado(NULL, NULL);                  -- iii: 0 filas
-- iv. Equivalencia: SELECT directo con las mismas filas y valores que el SP
SELECT c.ProductoID, SUM(c.Cantidad) AS Total_Unidades, SUM(c.Cantidad * c.Precio_Unitario) AS Total_Recaudado
FROM Carrito c INNER JOIN Venta v ON c.VentaID = v.VentaID INNER JOIN Retiro r ON v.VentaID = r.VentaID
WHERE c.ProductoID IS NOT NULL AND v.Fecha BETWEEN '2026-10-14' AND '2026-11-10' AND r.Estado_Retiro_ID <> 3
GROUP BY c.ProductoID ORDER BY Total_Unidades DESC, c.ProductoID;

-- ---------------- P23 sp_reporte_top_estudiantes_tuneado ----------------
CALL sp_reporte_top_estudiantes_tuneado('2026-10-14', '2026-11-10'); -- i: mismas filas que P17-i
CALL sp_reporte_top_estudiantes_tuneado('2020-01-01', '2020-12-31'); -- ii: 0 filas
CALL sp_reporte_top_estudiantes_tuneado('2026-10-14', NULL);         -- iii: 0 filas
-- iv. Equivalencia: SELECT directo con las mismas filas y valores que el SP
SELECT v.UserID, COUNT(*) AS Compras_Realizadas, SUM(v.Monto_Total) AS Total_Gastado, MAX(v.Fecha) AS Ultima_Compra
FROM Venta v INNER JOIN Retiro r ON v.VentaID = r.VentaID
WHERE v.Fecha BETWEEN '2026-10-14' AND '2026-11-10' AND r.Estado_Retiro_ID <> 3
GROUP BY v.UserID HAVING COUNT(*) >= 2 ORDER BY Total_Gastado DESC, v.UserID;

-- ############################################################################
-- SALDO DE CUENTA ESCOLAR
-- ############################################################################
USE schema_usuarios;

-- ---------------- P24 sp_cargar_saldo ----------------
-- i. Éxito: el usuario 3 carga $5000. Filas_Actualizadas = 1, Saldo = 5000
CALL sp_cargar_saldo(3, 5000);
-- ii. Usuario del personal (sin DNI): Filas_Actualizadas = 0
CALL sp_cargar_saldo(21, 5000);
-- iii. Monto 0 o NULL: Filas_Actualizadas = 0
CALL sp_cargar_saldo(3, 0);
CALL sp_cargar_saldo(3, NULL);
-- iv. Consistencia: saldo 5000 y una sola carga en el log
SELECT Saldo, (SELECT COUNT(*) FROM Log_Auditoria WHERE UserID = 3 AND Tipo_Evento_ID = 21) AS Cargas
FROM Usuario WHERE UserID = 3;

USE schema_comercial;

-- ---------------- P25 pago con saldo (sp_generar_retiro / sp_actualizar_estado_retiro) ----------------
-- i. Éxito: venta del usuario 3 con saldo (método 21). Al generar el retiro se descuenta el total
CALL sp_registrar_venta(3, NULL, 21);
SET @venta_saldo = (SELECT MAX(VentaID) FROM Venta WHERE UserID = 3);
CALL sp_agregar_producto_carrito(@venta_saldo, 1, 1);
CALL sp_generar_retiro(@venta_saldo, 3, 'SA001');
SELECT u.Saldo, 5000 - v.Monto_Total AS Saldo_Esperado
FROM schema_usuarios.Usuario u INNER JOIN Venta v ON v.UserID = u.UserID WHERE v.VentaID = @venta_saldo;
-- ii. Límite: el saldo no alcanza. No se crea el retiro (0 filas) y el saldo no cambia
CALL sp_registrar_venta(3, NULL, 21);
SET @venta_sin_saldo = (SELECT MAX(VentaID) FROM Venta WHERE UserID = 3);
CALL sp_agregar_producto_carrito(@venta_sin_saldo, 1, 2);
CALL sp_generar_retiro(@venta_sin_saldo, 3, 'SA002');
-- iii. Cancelar el pedido pagado con saldo devuelve el saldo: vuelve a 5000
SET @retiro_saldo = (SELECT RetiroID FROM Retiro WHERE VentaID = @venta_saldo);
CALL sp_actualizar_estado_retiro(21, @retiro_saldo, 3);
-- iv. Consistencia: saldo 5000 y la venta sin saldo no tiene retiro
SELECT (SELECT Saldo FROM schema_usuarios.Usuario WHERE UserID = 3) AS Saldo,
       (SELECT COUNT(*) FROM Retiro WHERE VentaID = @venta_sin_saldo) AS Retiros_Sin_Saldo;

-- ############################################################################
-- ACCESO DEL PERSONAL (PIN Y TOKEN)
-- ############################################################################
USE schema_usuarios;

-- ---------------- P26 sp_acceso_personal ----------------
-- i. Éxito: cajera con su PIN y desarrollador con su token. 1 fila cada uno
CALL sp_acceso_personal('marcela.ibanez', 'MarcelaBuffet#2026', '1234');
CALL sp_acceso_personal('soporte.dev', 'SoporteDev#2026', 'DEV-BEREA-2026');
-- ii. PIN incorrecto o vacío: 0 filas
CALL sp_acceso_personal('marcela.ibanez', 'MarcelaBuffet#2026', '9999');
CALL sp_acceso_personal('marcela.ibanez', 'MarcelaBuffet#2026', NULL);
-- iii. Un estudiante o el encargado (sin PIN ni token): 0 filas
CALL sp_acceso_personal('davidmel09', 'Davidmel#2026', '1234');
CALL sp_acceso_personal('alejandro.funes', 'AlejandroBuffet#2026', NULL);
-- iv. Restricción: un PIN que no tiene 4 dígitos lo rechaza el CHECK (error 3819)
UPDATE Empleado SET PIN = '12' WHERE UserID = 22;
SELECT PIN FROM Empleado WHERE UserID = 22;

-- ############################################################################
-- PRECIO DE PRODUCTO
-- ############################################################################
USE schema_comercial;

-- ---------------- P27 sp_modificar_precio ----------------
-- i. Éxito: el precio del producto 1 pasa de 2800 a 3000 (Filas_Actualizadas = 1)
CALL sp_modificar_precio(27, 1, 3000);
-- ii. Producto inexistente: Filas_Actualizadas = 0
CALL sp_modificar_precio(27, 9999, 3000);
-- iii. Precio negativo: ERROR 3819 (no cumple el CHECK)
CALL sp_modificar_precio(27, 1, -1);
-- iv. Consistencia: precio 3000 y un solo cambio de precio en el log
SELECT Precio_Producto,
       (SELECT COUNT(*) FROM schema_usuarios.Log_Auditoria WHERE Tipo_Evento_ID = 22) AS Cambios_De_Precio
FROM Producto WHERE ProductoID = 1;

-- ############################################################################
-- MENÚ SEMANAL FIJO (CADA DÍA HÁBIL REPITE SU PLATO)
-- ############################################################################
USE schema_comercial;

-- ---------------- P28 sp_menu_del_dia ----------------
-- i. Éxito: el menú fijo del miércoles (Menu_Del_DiaID 3) con el producto que incluye
CALL sp_menu_del_dia('Miércoles');
-- ii. Sábado: 0 filas (los fines de semana no hay menú)
CALL sp_menu_del_dia('Sábado');
-- iii. Día NULL: 0 filas
CALL sp_menu_del_dia(NULL);
-- iv. Consistencia: hay un solo menú por día hábil (5 menús, 5 días) y cada uno incluye un producto
SELECT COUNT(*) AS Menus, COUNT(DISTINCT Dia_Semana) AS Dias,
       (SELECT COUNT(*) FROM Detalle_Menu_Dia) AS Productos_Incluidos
FROM Menu_Del_Dia;
