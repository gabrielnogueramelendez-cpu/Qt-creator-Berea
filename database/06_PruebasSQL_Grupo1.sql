-- ============================================================================
-- PROYECTO INFORMÁTICO I - SECCIÓN BASES DE DATOS
-- El Buffet de Alejandro - PUNTO 6: SCRIPT DE PRUEBAS DE LOS STORED PROCEDURES
--
-- Ejecutar con la base recién cargada: 03_ModeloSQL -> 04_DatosSQL -> 05_Backend -> este.
-- Por cada SP hay 4 pruebas (consigna 6.b):
--   i.   Éxito (flujo normal)
--   ii.  Sin coincidencias (valores que no existen)
--   iii. Parámetros NULL / vacíos / límite
--   iv.  Consistencia lógica: se compara contra un SELECT directo
-- Los resultados obtenidos se pasan a la planilla 06_PruebasSQL.xlsx.
-- ============================================================================

SET NAMES utf8mb4;  -- para que las tildes y la ñ se guarden bien desde cualquier cliente

-- ############################################################################
-- FUNCIONALIDADES - USUARIOS
-- ############################################################################
USE schema_usuarios;

-- Para la prueba se crea un estudiante sin usuario (los 20 del 04 ya tienen uno)
INSERT INTO Estudiante (DNI, CUIT, Nombre_Estudiante, Apellido_Estudiante, Fecha_Nacimiento, ID_Colegio, ID_Curso)
VALUES ('50111222', '20-50111222-4', 'Tomás', 'Prueba', '2010-05-10', 1, 13);

-- ---------------- P01 sp_registrar_usuario ----------------
-- i. Éxito: devuelve estado OK y UserID 31
CALL sp_registrar_usuario('50111222', 'tomaspru10', 'Tomas#2026', 'tomas.prueba@bue.edu.ar');
-- ii. DNI que no existe: ERROR 'El DNI no corresponde a un estudiante'
CALL sp_registrar_usuario('99999999', 'nadie', 'Clave#1', 'nadie@bue.edu.ar');
-- iii. Parámetros NULL / vacíos: ERROR 'Faltan datos obligatorios'
CALL sp_registrar_usuario(NULL, '', NULL, '');
-- iv. Consistencia: el usuario, su contraseña y su email quedaron guardados
SELECT u.UserID, u.Apodo, u.Contrasena, e.Email
FROM Usuario u INNER JOIN Email e ON u.UserID = e.UserID WHERE u.DNI = '50111222';

-- ---------------- P02 sp_iniciar_sesion ----------------
-- i. Éxito: OK, UserID 1, Tipo_Usuario 'Estudiante'
CALL sp_iniciar_sesion('jorgedah09', 'Jorgedah#2026');
-- i-bis. Éxito con un cajero: Tipo_Usuario 'Cajera Turno Mañana'
CALL sp_iniciar_sesion('marcela.ibanez', 'MarcelaBuffet#2026');
-- ii. Usuario inexistente: ERROR y log "Fallo de Autenticación" con UserID NULL
CALL sp_iniciar_sesion('noexiste', 'x');
-- iii. NULL: ERROR, no rompe
CALL sp_iniciar_sesion(NULL, NULL);
-- iv. Consistencia: el log tiene 2 inicios exitosos y 2 fallos de hoy
SELECT Tipo_Evento_ID, COUNT(*) AS Cantidad FROM Log_Auditoria
WHERE Fecha = CURDATE() AND Tipo_Evento_ID IN (1, 3) GROUP BY Tipo_Evento_ID;

-- ---------------- P03 sp_cerrar_sesion ----------------
-- i. Éxito
CALL sp_cerrar_sesion(1);
-- ii. Usuario inexistente: ERROR 'El usuario no existe'
CALL sp_cerrar_sesion(9999);
-- iii. NULL: ERROR 'El usuario no existe'
CALL sp_cerrar_sesion(NULL);
-- iv. Consistencia: hay 1 cierre de sesión del usuario 1
SELECT COUNT(*) AS Cierres FROM Log_Auditoria WHERE Tipo_Evento_ID = 2 AND UserID = 1;

-- ---------------- P04 sp_cambiar_contrasena ----------------
-- i. Éxito
CALL sp_cambiar_contrasena(2, 'Luciagom#2026', 'NuevaLucia#2026');
-- ii. Contraseña actual incorrecta: ERROR
CALL sp_cambiar_contrasena(2, 'otra', 'algo');
-- iii. Nueva vacía: ERROR 'La contraseña nueva no puede estar vacía'
CALL sp_cambiar_contrasena(2, 'NuevaLucia#2026', '');
-- iv. Consistencia: con la nueva clave puede entrar
SELECT COUNT(*) AS Coincide FROM Usuario WHERE UserID = 2 AND Contrasena = 'NuevaLucia#2026';


-- ############################################################################
-- FUNCIONALIDADES - COMERCIAL
-- UserID 27 = Alejandro Funes (Encargado General) hace las tareas de administración
-- UserID 21 = Marcela Ibáñez (cajera, EmpleadoID 1)
-- ############################################################################
USE schema_comercial;

-- ---------------- P05 sp_alta_producto ----------------
-- i. Éxito: ProductoID 21
CALL sp_alta_producto(27, 11, 'Jugo de Naranja Exprimido', 'Vaso de 300 ml', 30, 1500.00);
-- ii. Categoría inexistente: ERROR 'La categoría no existe'
CALL sp_alta_producto(27, 999, 'Producto X', NULL, 10, 100.00);
-- iii. Precio negativo / nombre vacío: ERROR 'Datos del producto inválidos'
CALL sp_alta_producto(27, 11, '', NULL, 10, -5);
-- iv. Consistencia
SELECT ProductoID, Nombre_Producto, Stock_Producto, Precio_Producto, Activo FROM Producto WHERE ProductoID = 21;

-- ---------------- P06 sp_modificar_stock ----------------
-- i. Éxito: stock del producto 21 pasa de 30 a 50
CALL sp_modificar_stock(27, 21, 50);
-- ii. Producto inexistente: ERROR
CALL sp_modificar_stock(27, 9999, 10);
-- iii. Stock negativo: ERROR
CALL sp_modificar_stock(27, 21, -1);
-- iv. Consistencia
SELECT Stock_Producto FROM Producto WHERE ProductoID = 21;

-- ---------------- P07 sp_baja_producto ----------------
-- i. Éxito
CALL sp_baja_producto(27, 21);
-- ii. Producto inexistente: ERROR
CALL sp_baja_producto(27, 9999);
-- iii. Límite: dar de baja dos veces el mismo producto: ERROR 'ya estaba dado de baja'
CALL sp_baja_producto(27, 21);
-- iv. Consistencia: Activo = 0 y la fila sigue existiendo
SELECT ProductoID, Activo FROM Producto WHERE ProductoID = 21;

-- ---------------- P08 sp_alta_menu_dia ----------------
-- i. Éxito: menú de HOY que incluye un agua (producto 10). Menu_Del_DiaID 21
CALL sp_alta_menu_dia(27, 19, 'Milanesa con ensalada', 4500.00, 30, CURDATE(), 10);
-- ii. Producto incluido inexistente: ERROR
CALL sp_alta_menu_dia(27, 19, 'Plato X', 4000.00, 10, CURDATE(), 9999);
-- iii. Fecha NULL: ERROR 'Datos del menú inválidos'
CALL sp_alta_menu_dia(27, 19, 'Plato X', 4000.00, 10, NULL, NULL);
-- iv. Consistencia: el menú y su detalle existen
SELECT m.Menu_Del_DiaID, m.Plato_menu, m.Fecha_menu, d.ProductoID
FROM Menu_Del_Dia m INNER JOIN Detalle_Menu_Dia d ON m.Menu_Del_DiaID = d.Menu_Del_DiaID
WHERE m.Menu_Del_DiaID = 21;

-- ---------------- P09 sp_registrar_venta ----------------
-- i. Éxito: el alumno 3 compra en mostrador con la cajera 1. Devuelve VentaID 51
CALL sp_registrar_venta(3, 1, 1);
-- ii. Usuario inexistente: ERROR 'El usuario no existe'
CALL sp_registrar_venta(9999, 1, 1);
-- iii. Método de pago NULL: ERROR 'El método de pago no existe'
CALL sp_registrar_venta(3, NULL, NULL);
-- iv. Consistencia: la venta 51 existe con monto 0
SELECT VentaID, UserID, EmpleadoID, Monto_Total FROM Venta WHERE VentaID = 51;

-- ---------------- P10 sp_agregar_producto_carrito ----------------
-- i. Éxito: 2 medialunas (producto 5, $800 c/u). Monto_Total 1600
CALL sp_agregar_producto_carrito(51, 5, 2);
-- ii. Producto inexistente: ERROR
CALL sp_agregar_producto_carrito(51, 9999, 1);
-- iii. Límite: cantidad 0 y cantidad mayor al stock: ERROR en los dos casos
CALL sp_agregar_producto_carrito(51, 5, 0);
CALL sp_agregar_producto_carrito(51, 5, 100000);
-- iv. Consistencia: monto de la venta = suma del carrito; stock de medialunas 90 -> 88
SELECT v.Monto_Total, SUM(c.Cantidad * c.Precio_Unitario) AS Suma_Carrito,
       (SELECT Stock_Producto FROM Producto WHERE ProductoID = 5) AS Stock_Medialunas
FROM Venta v INNER JOIN Carrito c ON v.VentaID = c.VentaID WHERE v.VentaID = 51 GROUP BY v.Monto_Total;

-- ---------------- P11 sp_agregar_menu_carrito ----------------
-- i. Éxito: 1 menú de hoy (21, $4500). Monto_Total 6100
CALL sp_agregar_menu_carrito(51, 21, 1);
-- ii. Menú de otro día (menú 1 es del 14/10): ERROR 'no corresponde al día de la venta'
CALL sp_agregar_menu_carrito(51, 1, 1);
-- iii. Cantidad NULL: ERROR
CALL sp_agregar_menu_carrito(51, 21, NULL);
-- iv. Consistencia: monto = suma del carrito; stock del menú 30 -> 29
SELECT v.Monto_Total, SUM(c.Cantidad * c.Precio_Unitario) AS Suma_Carrito,
       (SELECT Stock_menu FROM Menu_Del_Dia WHERE Menu_Del_DiaID = 21) AS Stock_Menu
FROM Venta v INNER JOIN Carrito c ON v.VentaID = c.VentaID WHERE v.VentaID = 51 GROUP BY v.Monto_Total;

-- ---------------- P12 sp_generar_retiro ----------------
-- iii. Franja NULL: ERROR 'La franja de retiro no existe' (se prueba antes de generar el retiro)
CALL sp_generar_retiro(51, NULL);
-- i. Éxito: retiro en el receso del almuerzo (franja 4). Devuelve un código de 5 caracteres
CALL sp_generar_retiro(51, 4);
-- ii. Venta inexistente: ERROR
CALL sp_generar_retiro(9999, 4);
-- iii-bis. Límite: generar el retiro dos veces: ERROR 'La venta ya tiene código de retiro'
CALL sp_generar_retiro(51, 4);
-- iv. Consistencia: hay un solo retiro PENDIENTE para la venta 51, y ya no se le pueden agregar productos
SELECT RetiroID, Estado_Retiro_ID, ID_Franja, LENGTH(Codigo_Retiro) AS Largo_Codigo FROM Retiro WHERE VentaID = 51;
CALL sp_agregar_producto_carrito(51, 5, 1);

-- ---------------- P13 sp_actualizar_estado_retiro ----------------
-- i. Éxito: la cajera (UserID 21) entrega el retiro 51
CALL sp_actualizar_estado_retiro(21, 51, 2);
-- ii. Retiro inexistente: ERROR 'El retiro no existe'
CALL sp_actualizar_estado_retiro(21, 99999, 2);
-- iii. Límite: volver a cambiar un pedido ya entregado (doble entrega): ERROR / estado NULL: ERROR
CALL sp_actualizar_estado_retiro(21, 51, 3);
CALL sp_actualizar_estado_retiro(21, 46, NULL);
-- iv. Consistencia: cancelar el retiro 46 (pendiente) devuelve el stock.
--     La venta 46 tenía 3 manzanas (producto 7): el stock pasa de 70 a 73.
SELECT Stock_Producto AS Stock_Antes FROM Producto WHERE ProductoID = 7;
CALL sp_actualizar_estado_retiro(21, 46, 3);
SELECT Stock_Producto AS Stock_Despues FROM Producto WHERE ProductoID = 7;
SELECT RetiroID, Estado_Retiro_ID FROM Retiro WHERE RetiroID IN (46, 51);


-- ############################################################################
-- REPORTES
-- Rango con datos: 2026-10-14 a 2026-11-10. Rango sin datos: año 2020.
-- Ojo: en P13 se canceló el retiro 46, así que la venta 46 ya no suma en los reportes.
-- (La venta 51 de las pruebas es de hoy y queda fuera del rango.)
-- ############################################################################
USE schema_comercial;

-- ---------------- P14 sp_reporte_ventas_por_categoria ----------------
CALL sp_reporte_ventas_por_categoria('2026-10-14', '2026-11-10');   -- i
CALL sp_reporte_ventas_por_categoria('2020-01-01', '2020-12-31');   -- ii (vacío)
CALL sp_reporte_ventas_por_categoria(NULL, '2026-11-10');           -- iii
-- iv. SELECT directo: la recaudación total tiene que ser igual a la suma del reporte
SELECT SUM(c.Cantidad * c.Precio_Unitario) AS Total_Directo
FROM Carrito c INNER JOIN Venta v ON c.VentaID = v.VentaID INNER JOIN Retiro r ON v.VentaID = r.VentaID
WHERE v.Fecha BETWEEN '2026-10-14' AND '2026-11-10' AND r.Estado_Retiro_ID <> 3;

-- ---------------- P15 sp_reporte_top_productos ----------------
CALL sp_reporte_top_productos('2026-10-14', '2026-11-10');          -- i
CALL sp_reporte_top_productos('2020-01-01', '2020-12-31');          -- ii
CALL sp_reporte_top_productos('2026-11-10', '2026-10-14');          -- iii: ERROR 'Rango de fechas inválido' (rango invertido)
-- iv
SELECT c.ProductoID, SUM(c.Cantidad) AS Unidades
FROM Carrito c INNER JOIN Venta v ON c.VentaID = v.VentaID INNER JOIN Retiro r ON v.VentaID = r.VentaID
WHERE c.ProductoID IS NOT NULL AND v.Fecha BETWEEN '2026-10-14' AND '2026-11-10' AND r.Estado_Retiro_ID <> 3
GROUP BY c.ProductoID ORDER BY Unidades DESC, c.ProductoID;

-- ---------------- P16 sp_reporte_metodos_pago ----------------
CALL sp_reporte_metodos_pago('2026-10-14', '2026-11-10');           -- i
CALL sp_reporte_metodos_pago('2020-01-01', '2020-12-31');           -- ii
CALL sp_reporte_metodos_pago(NULL, NULL);                           -- iii
-- iv
SELECT v.Metodo_PagoID, COUNT(*) AS Transacciones, SUM(v.Monto_Total) AS Total
FROM Venta v INNER JOIN Retiro r ON v.VentaID = r.VentaID
WHERE v.Fecha BETWEEN '2026-10-14' AND '2026-11-10' AND r.Estado_Retiro_ID <> 3
GROUP BY v.Metodo_PagoID ORDER BY Total DESC;

-- ---------------- P17 sp_reporte_ventas_diarias ----------------
CALL sp_reporte_ventas_diarias('2026-10-14', '2026-11-10');         -- i
CALL sp_reporte_ventas_diarias('2020-01-01', '2020-12-31');         -- ii
CALL sp_reporte_ventas_diarias('2026-10-14', NULL);                 -- iii
-- iv
SELECT COUNT(DISTINCT v.Fecha) AS Dias, COUNT(*) AS Ventas, SUM(v.Monto_Total) AS Total
FROM Venta v INNER JOIN Retiro r ON v.VentaID = r.VentaID
WHERE v.Fecha BETWEEN '2026-10-14' AND '2026-11-10' AND r.Estado_Retiro_ID <> 3;

-- ---------------- P18 sp_reporte_top_estudiantes ----------------
CALL sp_reporte_top_estudiantes('2026-10-14', '2026-11-10');        -- i
CALL sp_reporte_top_estudiantes('2020-01-01', '2020-12-31');        -- ii
CALL sp_reporte_top_estudiantes(NULL, NULL);                        -- iii
-- iv
SELECT v.UserID, COUNT(*) AS Compras, SUM(v.Monto_Total) AS Total
FROM Venta v INNER JOIN Retiro r ON v.VentaID = r.VentaID
WHERE v.Fecha BETWEEN '2026-10-14' AND '2026-11-10' AND r.Estado_Retiro_ID <> 3
GROUP BY v.UserID ORDER BY Total DESC, v.UserID;

-- ---------------- P19 sp_reporte_rendimiento_empleados ----------------
CALL sp_reporte_rendimiento_empleados('2026-10-14', '2026-11-10');  -- i
CALL sp_reporte_rendimiento_empleados('2020-01-01', '2020-12-31');  -- ii
CALL sp_reporte_rendimiento_empleados('2026-11-10', '2026-10-14');  -- iii
-- iv
SELECT v.EmpleadoID, COUNT(*) AS Ventas, SUM(v.Monto_Total) AS Total
FROM Venta v INNER JOIN Retiro r ON v.VentaID = r.VentaID
WHERE v.EmpleadoID IS NOT NULL AND v.Fecha BETWEEN '2026-10-14' AND '2026-11-10' AND r.Estado_Retiro_ID <> 3
GROUP BY v.EmpleadoID ORDER BY Total DESC;

-- ---------------- P20 sp_reporte_retiros_por_franja ----------------
CALL sp_reporte_retiros_por_franja('2026-10-14', '2026-11-10');     -- i
CALL sp_reporte_retiros_por_franja('2020-01-01', '2020-12-31');     -- ii
CALL sp_reporte_retiros_por_franja(NULL, '2026-11-10');             -- iii
-- iv: el total de pedidos del reporte tiene que dar 50
SELECT COUNT(*) AS Total_Retiros FROM Retiro r INNER JOIN Venta v ON r.VentaID = v.VentaID
WHERE v.Fecha BETWEEN '2026-10-14' AND '2026-11-10';

-- ---------------- P21 sp_reporte_demanda_menu_dia ----------------
CALL sp_reporte_demanda_menu_dia('2026-10-14', '2026-11-10');       -- i
CALL sp_reporte_demanda_menu_dia('2020-01-01', '2020-12-31');       -- ii
CALL sp_reporte_demanda_menu_dia(NULL, NULL);                       -- iii
-- iv
SELECT c.Menu_Del_DiaID, SUM(c.Cantidad) AS Vendidos
FROM Carrito c INNER JOIN Menu_Del_Dia m ON c.Menu_Del_DiaID = m.Menu_Del_DiaID
INNER JOIN Retiro r ON c.VentaID = r.VentaID
WHERE m.Fecha_menu BETWEEN '2026-10-14' AND '2026-11-10' AND r.Estado_Retiro_ID <> 3
GROUP BY c.Menu_Del_DiaID ORDER BY Vendidos DESC, c.Menu_Del_DiaID;


-- ############################################################################
-- AUDITORÍA
-- Se generan algunos eventos de hoy para tener datos en el log.
-- ############################################################################
USE schema_usuarios;
CALL sp_iniciar_sesion('jorgedah09', 'Jorgedah#2026');
CALL sp_iniciar_sesion('jorgedah09', 'clave-mal');
CALL sp_cerrar_sesion(1);
CALL sp_iniciar_sesion('marcela.ibanez', 'MarcelaBuffet#2026');

-- ---------------- P22 sp_reporte_auditoria ----------------
CALL sp_reporte_auditoria(CURDATE(), CURDATE(), NULL, NULL);        -- i: todos los eventos de hoy (los generaron las pruebas de arriba)
CALL sp_reporte_auditoria(CURDATE(), CURDATE(), 1, 3);              -- i-bis: solo los fallos del usuario 1
CALL sp_reporte_auditoria('2020-01-01', '2020-12-31', NULL, NULL);  -- ii: vacío
CALL sp_reporte_auditoria(NULL, NULL, 1, NULL);                     -- iii: ERROR rango inválido
-- iv
SELECT COUNT(*) AS Eventos_Hoy FROM Log_Auditoria WHERE Fecha = CURDATE();


-- ############################################################################
-- TUNING: el resultado del SP tuneado tiene que ser idéntico al original
-- ############################################################################
USE schema_comercial;

-- ---------------- P23 sp_reporte_top_productos_tuneado ----------------
CALL sp_reporte_top_productos_tuneado('2026-10-14', '2026-11-10');  -- i: mismas filas que P15-i
CALL sp_reporte_top_productos_tuneado('2020-01-01', '2020-12-31');  -- ii
CALL sp_reporte_top_productos_tuneado(NULL, NULL);                  -- iii
-- iv. Equivalencia: se compara con el SELECT directo (mismas filas y valores que el SP)
SELECT c.ProductoID, SUM(c.Cantidad) AS Total_Unidades, SUM(c.Cantidad * c.Precio_Unitario) AS Total_Recaudado
FROM Carrito c INNER JOIN Venta v ON c.VentaID = v.VentaID INNER JOIN Retiro r ON v.VentaID = r.VentaID
WHERE c.ProductoID IS NOT NULL AND v.Fecha BETWEEN '2026-10-14' AND '2026-11-10' AND r.Estado_Retiro_ID <> 3
GROUP BY c.ProductoID ORDER BY Total_Unidades DESC, c.ProductoID;

-- ---------------- P24 sp_reporte_top_estudiantes_tuneado ----------------
CALL sp_reporte_top_estudiantes_tuneado('2026-10-14', '2026-11-10'); -- i: mismas filas que P18-i
CALL sp_reporte_top_estudiantes_tuneado('2020-01-01', '2020-12-31'); -- ii
CALL sp_reporte_top_estudiantes_tuneado('2026-10-14', NULL);         -- iii
-- iv. Equivalencia: se compara con el SELECT directo (mismas filas y valores que el SP)
SELECT v.UserID, COUNT(*) AS Compras_Realizadas, SUM(v.Monto_Total) AS Total_Gastado, MAX(v.Fecha) AS Ultima_Compra
FROM Venta v INNER JOIN Retiro r ON v.VentaID = r.VentaID
WHERE v.Fecha BETWEEN '2026-10-14' AND '2026-11-10' AND r.Estado_Retiro_ID <> 3
GROUP BY v.UserID ORDER BY Total_Gastado DESC, v.UserID;
