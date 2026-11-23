-- ============================================================================
-- PROYECTO INFORMÁTICO I - SECCIÓN BASES DE DATOS
-- El Buffet de Alejandro - Escuela Técnica N° 3 D.E. 9 "María Sánchez de Thompson"
-- PUNTO 5: FUNCIONALIDADES EN SQL (Stored Procedures)
-- IDs de Tipo_Evento usados (ver 04_DatosSQL):
--   1 Inicio de sesión      2 Cierre de sesión        3 Fallo de autenticación
--   4 Registro de usuario   6 Registro de venta
--   7 Producto al carrito   8 Menú al carrito         9 Código de retiro
--  10 Entrega de pedido    11 Cancelación de pedido  12 Cambio de estado de pedido
--  13 Alta de producto     14 Baja de producto       15 Modificación de stock
--  16 Alta de menú del día  21 Carga de saldo     22 Modificación de precio
-- IDs de Estado_Retiro usados: 1 PENDIENTE, 2 ENTREGADO, 3 CANCELADO
-- ID de Metodo_Pago usado: 21 Saldo de Cuenta Escolar
-- ============================================================================

SET NAMES utf8mb4;


-- ############################################################################
-- 5.a / 5.b  FUNCIONALIDADES - SCHEMA USUARIOS
-- ############################################################################
USE schema_usuarios;

-- ----------------------------------------------------------------------------
-- SP 1: Registrar un usuario nuevo para un estudiante o un profesor
-- ----------------------------------------------------------------------------
DROP PROCEDURE IF EXISTS sp_registrar_usuario;
DELIMITER //
CREATE PROCEDURE sp_registrar_usuario(p_DNI VARCHAR(20), p_Apodo VARCHAR(50), p_Contrasena VARCHAR(50))
BEGIN
    DECLARE codigo_error INT DEFAULT 0;
    DECLARE mensaje_error VARCHAR(255) DEFAULT '';

    DECLARE EXIT HANDLER FOR SQLEXCEPTION
    BEGIN
        GET DIAGNOSTICS CONDITION 1
            codigo_error = MYSQL_ERRNO,
            mensaje_error = MESSAGE_TEXT;
        SELECT 'ERROR' AS estado, codigo_error AS codigo_sql, mensaje_error AS descripcion_error;
    END;

    -- Si el DNI es de un profesor se guarda en DNI_Profesor con rol 2
    INSERT INTO Usuario (DNI_Profesor, Apodo, Contrasena, RolID)
    SELECT DNI, p_Apodo, p_Contrasena, 2
    FROM Profesor
    WHERE DNI = p_DNI;

    -- Si no, se guarda en DNI con rol 1 (si tampoco es estudiante, la FK da error)
    INSERT INTO Usuario (DNI, Apodo, Contrasena, RolID)
    SELECT p_DNI, p_Apodo, p_Contrasena, RolID
    FROM Rol
    WHERE RolID = 1
      AND NOT EXISTS (SELECT 1 FROM Profesor WHERE DNI = p_DNI);

    INSERT INTO Log_Auditoria (UserID, Tipo_Evento_ID, Fecha, Hora, Detalle)
    SELECT UserID, 4, CURDATE(), CURTIME(), CONCAT('Nuevo usuario: ', Apodo)
    FROM Usuario
    WHERE Apodo = p_Apodo;

    SELECT u.UserID, COALESCE(u.DNI, u.DNI_Profesor) AS DNI, u.Apodo, r.Nombre_Rol
    FROM Usuario u
    INNER JOIN Rol r ON u.RolID = r.RolID
    WHERE u.Apodo = p_Apodo;
END //
DELIMITER ;

-- ----------------------------------------------------------------------------
-- SP 2: Iniciar sesión
-- ----------------------------------------------------------------------------
DROP PROCEDURE IF EXISTS sp_iniciar_sesion;
DELIMITER //
CREATE PROCEDURE sp_iniciar_sesion(p_Apodo VARCHAR(50), p_Contrasena VARCHAR(50))
BEGIN
    DECLARE codigo_error INT DEFAULT 0;
    DECLARE mensaje_error VARCHAR(255) DEFAULT '';
    DECLARE filas INT DEFAULT 0;

    DECLARE EXIT HANDLER FOR SQLEXCEPTION
    BEGIN
        GET DIAGNOSTICS CONDITION 1
            codigo_error = MYSQL_ERRNO,
            mensaje_error = MESSAGE_TEXT;
        SELECT 'ERROR' AS estado, codigo_error AS codigo_sql, mensaje_error AS descripcion_error;
    END;

    -- Si el apodo y la contraseña son correctos, se registra el inicio de sesión
    INSERT INTO Log_Auditoria (UserID, Tipo_Evento_ID, Fecha, Hora, Detalle)
    SELECT UserID, 1, CURDATE(), CURTIME(), 'Inicio de sesión'
    FROM Usuario
    WHERE Apodo = p_Apodo AND Contrasena = p_Contrasena;

    SET filas = ROW_COUNT();

    -- Si no, se registra el intento fallido (UserID queda NULL si el apodo no existe)
    INSERT INTO Log_Auditoria (UserID, Tipo_Evento_ID, Fecha, Hora, Detalle)
    SELECT (SELECT UserID FROM Usuario WHERE Apodo = p_Apodo), 3, CURDATE(), CURTIME(),
           CONCAT('Intento fallido con el apodo: ', COALESCE(p_Apodo, '(vacío)'))
    FROM Rol
    WHERE RolID = 1 AND filas = 0;

    SELECT u.UserID, u.Apodo, r.Nombre_Rol AS Tipo_Usuario
    FROM Usuario u
    INNER JOIN Rol r ON u.RolID = r.RolID
    WHERE u.Apodo = p_Apodo AND u.Contrasena = p_Contrasena;
END //
DELIMITER ;

-- ----------------------------------------------------------------------------
-- SP 2b: Acceso del personal
-- ----------------------------------------------------------------------------
DROP PROCEDURE IF EXISTS sp_acceso_personal;
DELIMITER //
CREATE PROCEDURE sp_acceso_personal(p_Apodo VARCHAR(50), p_Contrasena VARCHAR(50), p_Clave VARCHAR(30))
BEGIN
    DECLARE codigo_error INT DEFAULT 0;
    DECLARE mensaje_error VARCHAR(255) DEFAULT '';
    DECLARE filas INT DEFAULT 0;

    DECLARE EXIT HANDLER FOR SQLEXCEPTION
    BEGIN
        GET DIAGNOSTICS CONDITION 1
            codigo_error = MYSQL_ERRNO,
            mensaje_error = MESSAGE_TEXT;
        SELECT 'ERROR' AS estado, codigo_error AS codigo_sql, mensaje_error AS descripcion_error;
    END;

    -- Si apodo, contraseña y PIN (o token) son correctos, se registra el inicio de sesión
    INSERT INTO Log_Auditoria (UserID, Tipo_Evento_ID, Fecha, Hora, Detalle)
    SELECT u.UserID, 1, CURDATE(), CURTIME(), 'Inicio de sesión del personal'
    FROM Usuario u
    INNER JOIN Empleado e ON u.UserID = e.UserID
    WHERE u.Apodo = p_Apodo AND u.Contrasena = p_Contrasena
      AND (e.PIN = p_Clave OR e.Token_Acceso = p_Clave);

    SET filas = ROW_COUNT();

    -- Si no, se registra el intento fallido
    INSERT INTO Log_Auditoria (UserID, Tipo_Evento_ID, Fecha, Hora, Detalle)
    SELECT (SELECT UserID FROM Usuario WHERE Apodo = p_Apodo), 3, CURDATE(), CURTIME(),
           CONCAT('Intento fallido del personal con el apodo: ', COALESCE(p_Apodo, '(vacío)'))
    FROM Rol
    WHERE RolID = 1 AND filas = 0;

    SELECT u.UserID, u.Apodo, e.Nombre_Empleado, e.Apellido_Empleado, p.Nombre_Puesto
    FROM Usuario u
    INNER JOIN Empleado e ON u.UserID = e.UserID
    INNER JOIN Puesto p ON e.ID_Puesto = p.ID_Puesto
    WHERE u.Apodo = p_Apodo AND u.Contrasena = p_Contrasena
      AND (e.PIN = p_Clave OR e.Token_Acceso = p_Clave);
END //
DELIMITER ;

-- ----------------------------------------------------------------------------
-- SP 3: Cerrar sesión
-- ----------------------------------------------------------------------------
DROP PROCEDURE IF EXISTS sp_cerrar_sesion;
DELIMITER //
CREATE PROCEDURE sp_cerrar_sesion(p_UserID INT)
BEGIN
    DECLARE codigo_error INT DEFAULT 0;
    DECLARE mensaje_error VARCHAR(255) DEFAULT '';

    DECLARE EXIT HANDLER FOR SQLEXCEPTION
    BEGIN
        GET DIAGNOSTICS CONDITION 1
            codigo_error = MYSQL_ERRNO,
            mensaje_error = MESSAGE_TEXT;
        SELECT 'ERROR' AS estado, codigo_error AS codigo_sql, mensaje_error AS descripcion_error;
    END;

    INSERT INTO Log_Auditoria (UserID, Tipo_Evento_ID, Fecha, Hora, Detalle)
    SELECT UserID, 2, CURDATE(), CURTIME(), 'Cierre de sesión'
    FROM Usuario
    WHERE UserID = p_UserID;

    SELECT ROW_COUNT() AS Sesiones_Cerradas;
END //
DELIMITER ;

-- ----------------------------------------------------------------------------
-- SP 4: Cargar saldo en la cuenta escolar de un estudiante
-- ----------------------------------------------------------------------------
DROP PROCEDURE IF EXISTS sp_cargar_saldo;
DELIMITER //
CREATE PROCEDURE sp_cargar_saldo(p_UserID INT, p_Monto DECIMAL(10,2))
BEGIN
    DECLARE codigo_error INT DEFAULT 0;
    DECLARE mensaje_error VARCHAR(255) DEFAULT '';
    DECLARE filas INT DEFAULT 0;

    DECLARE EXIT HANDLER FOR SQLEXCEPTION
    BEGIN
        GET DIAGNOSTICS CONDITION 1
            codigo_error = MYSQL_ERRNO,
            mensaje_error = MESSAGE_TEXT;
        SELECT 'ERROR' AS estado, codigo_error AS codigo_sql, mensaje_error AS descripcion_error;
    END;

    UPDATE Usuario
    SET Saldo = Saldo + p_Monto
    WHERE UserID = p_UserID AND (DNI IS NOT NULL OR DNI_Profesor IS NOT NULL) AND p_Monto > 0;

    SET filas = ROW_COUNT();

    INSERT INTO Log_Auditoria (UserID, Tipo_Evento_ID, Fecha, Hora, Detalle)
    SELECT UserID, 21, CURDATE(), CURTIME(), CONCAT('Carga de saldo: $', p_Monto)
    FROM Usuario
    WHERE UserID = p_UserID AND filas = 1;

    SELECT filas AS Filas_Actualizadas, Saldo
    FROM Usuario
    WHERE UserID = p_UserID;
END //
DELIMITER ;


-- ############################################################################
-- 5.a / 5.b  FUNCIONALIDADES - SCHEMA COMERCIAL
-- ############################################################################
USE schema_comercial;

-- ----------------------------------------------------------------------------
-- SP 5: Alta de producto
-- ----------------------------------------------------------------------------
DROP PROCEDURE IF EXISTS sp_alta_producto;
DELIMITER //
CREATE PROCEDURE sp_alta_producto(p_UserID INT, p_ID_Categoria INT, p_Nombre VARCHAR(100),
                                  p_Descripcion TEXT, p_Stock INT, p_Precio DECIMAL(10,2))
BEGIN
    DECLARE codigo_error INT DEFAULT 0;
    DECLARE mensaje_error VARCHAR(255) DEFAULT '';

    DECLARE EXIT HANDLER FOR SQLEXCEPTION
    BEGIN
        GET DIAGNOSTICS CONDITION 1
            codigo_error = MYSQL_ERRNO,
            mensaje_error = MESSAGE_TEXT;
        SELECT 'ERROR' AS estado, codigo_error AS codigo_sql, mensaje_error AS descripcion_error;
    END;

    INSERT INTO Producto (ID_Categoria, Nombre_Producto, Descripcion_Producto, Stock_Producto, Precio_Producto)
    VALUES (p_ID_Categoria, p_Nombre, p_Descripcion, p_Stock, p_Precio);

    INSERT INTO schema_usuarios.Log_Auditoria (UserID, Tipo_Evento_ID, Fecha, Hora, Detalle)
    VALUES (p_UserID, 13, CURDATE(), CURTIME(), CONCAT('Alta de producto: ', p_Nombre));

    SELECT ProductoID, Nombre_Producto, Stock_Producto, Precio_Producto
    FROM Producto
    WHERE ProductoID = (SELECT MAX(ProductoID) FROM Producto);
END //
DELIMITER ;

-- ----------------------------------------------------------------------------
-- SP 6: Modificar el stock de un producto (un stock negativo da error por CHECK)
-- ----------------------------------------------------------------------------
DROP PROCEDURE IF EXISTS sp_modificar_stock;
DELIMITER //
CREATE PROCEDURE sp_modificar_stock(p_UserID INT, p_ProductoID INT, p_NuevoStock INT)
BEGIN
    DECLARE codigo_error INT DEFAULT 0;
    DECLARE mensaje_error VARCHAR(255) DEFAULT '';
    DECLARE filas INT DEFAULT 0;

    DECLARE EXIT HANDLER FOR SQLEXCEPTION
    BEGIN
        GET DIAGNOSTICS CONDITION 1
            codigo_error = MYSQL_ERRNO,
            mensaje_error = MESSAGE_TEXT;
        SELECT 'ERROR' AS estado, codigo_error AS codigo_sql, mensaje_error AS descripcion_error;
    END;

    UPDATE Producto
    SET Stock_Producto = p_NuevoStock
    WHERE ProductoID = p_ProductoID;

    SET filas = ROW_COUNT();

    INSERT INTO schema_usuarios.Log_Auditoria (UserID, Tipo_Evento_ID, Fecha, Hora, Detalle)
    SELECT p_UserID, 15, CURDATE(), CURTIME(), CONCAT('Producto ', ProductoID, ': stock ', Stock_Producto)
    FROM Producto
    WHERE ProductoID = p_ProductoID AND filas = 1;

    SELECT filas AS Filas_Actualizadas;
END //
DELIMITER ;

-- ----------------------------------------------------------------------------
-- SP 6b: Modificar el precio de un producto (un precio negativo da error por CHECK)
-- ----------------------------------------------------------------------------
DROP PROCEDURE IF EXISTS sp_modificar_precio;
DELIMITER //
CREATE PROCEDURE sp_modificar_precio(p_UserID INT, p_ProductoID INT, p_NuevoPrecio DECIMAL(10,2))
BEGIN
    DECLARE codigo_error INT DEFAULT 0;
    DECLARE mensaje_error VARCHAR(255) DEFAULT '';
    DECLARE filas INT DEFAULT 0;

    DECLARE EXIT HANDLER FOR SQLEXCEPTION
    BEGIN
        GET DIAGNOSTICS CONDITION 1
            codigo_error = MYSQL_ERRNO,
            mensaje_error = MESSAGE_TEXT;
        SELECT 'ERROR' AS estado, codigo_error AS codigo_sql, mensaje_error AS descripcion_error;
    END;

    UPDATE Producto
    SET Precio_Producto = p_NuevoPrecio
    WHERE ProductoID = p_ProductoID;

    SET filas = ROW_COUNT();

    INSERT INTO schema_usuarios.Log_Auditoria (UserID, Tipo_Evento_ID, Fecha, Hora, Detalle)
    SELECT p_UserID, 22, CURDATE(), CURTIME(), CONCAT('Producto ', ProductoID, ': precio ', Precio_Producto)
    FROM Producto
    WHERE ProductoID = p_ProductoID AND filas = 1;

    SELECT filas AS Filas_Actualizadas;
END //
DELIMITER ;

-- ----------------------------------------------------------------------------
-- SP 7: Baja de producto (baja lógica)
-- ----------------------------------------------------------------------------
DROP PROCEDURE IF EXISTS sp_baja_producto;
DELIMITER //
CREATE PROCEDURE sp_baja_producto(p_UserID INT, p_ProductoID INT)
BEGIN
    DECLARE codigo_error INT DEFAULT 0;
    DECLARE mensaje_error VARCHAR(255) DEFAULT '';
    DECLARE filas INT DEFAULT 0;

    DECLARE EXIT HANDLER FOR SQLEXCEPTION
    BEGIN
        GET DIAGNOSTICS CONDITION 1
            codigo_error = MYSQL_ERRNO,
            mensaje_error = MESSAGE_TEXT;
        SELECT 'ERROR' AS estado, codigo_error AS codigo_sql, mensaje_error AS descripcion_error;
    END;

    UPDATE Producto
    SET Activo = FALSE
    WHERE ProductoID = p_ProductoID AND Activo = TRUE;

    SET filas = ROW_COUNT();

    INSERT INTO schema_usuarios.Log_Auditoria (UserID, Tipo_Evento_ID, Fecha, Hora, Detalle)
    SELECT p_UserID, 14, CURDATE(), CURTIME(), CONCAT('Baja del producto ', ProductoID)
    FROM Producto
    WHERE ProductoID = p_ProductoID AND filas = 1;

    SELECT filas AS Filas_Actualizadas;
END //
DELIMITER ;

-- ----------------------------------------------------------------------------
-- SP 8: Modificar el menú fijo de un día (plato, precio y stock).
-- El menú es fijo: hay un solo menú por día hábil, cargado en 04_DatosSQL.
-- Sábado, domingo o un día que no existe: no modifica nada y devuelve 0 filas
-- ----------------------------------------------------------------------------
DROP PROCEDURE IF EXISTS sp_modificar_menu_dia;
DELIMITER //
CREATE PROCEDURE sp_modificar_menu_dia(p_UserID INT, p_Dia VARCHAR(10), p_Plato VARCHAR(100),
                                       p_Precio DECIMAL(10,2), p_Stock INT)
BEGIN
    DECLARE codigo_error INT DEFAULT 0;
    DECLARE mensaje_error VARCHAR(255) DEFAULT '';
    DECLARE filas INT DEFAULT 0;

    DECLARE EXIT HANDLER FOR SQLEXCEPTION
    BEGIN
        GET DIAGNOSTICS CONDITION 1
            codigo_error = MYSQL_ERRNO,
            mensaje_error = MESSAGE_TEXT;
        SELECT 'ERROR' AS estado, codigo_error AS codigo_sql, mensaje_error AS descripcion_error;
    END;

    UPDATE Menu_Del_Dia
    SET Plato_menu = p_Plato, Precio_menu = p_Precio, Stock_menu = p_Stock
    WHERE Dia_Semana = p_Dia;

    SET filas = ROW_COUNT();

    INSERT INTO schema_usuarios.Log_Auditoria (UserID, Tipo_Evento_ID, Fecha, Hora, Detalle)
    SELECT p_UserID, 16, CURDATE(), CURTIME(), CONCAT('Menú del ', p_Dia, ': ', p_Plato)
    FROM Menu_Del_Dia
    WHERE Dia_Semana = p_Dia AND filas = 1;

    SELECT filas AS Filas_Actualizadas, Menu_Del_DiaID, Plato_menu, Precio_menu, Stock_menu, Dia_Semana
    FROM Menu_Del_Dia
    WHERE Dia_Semana = p_Dia;
END //
DELIMITER ;

-- ----------------------------------------------------------------------------
-- SP 9: Registrar venta (abre la venta con monto 0, con la fecha y hora actual).
-- ----------------------------------------------------------------------------
DROP PROCEDURE IF EXISTS sp_registrar_venta;
DELIMITER //
CREATE PROCEDURE sp_registrar_venta(p_UserID INT, p_EmpleadoID INT, p_Metodo_PagoID INT)
BEGIN
    DECLARE codigo_error INT DEFAULT 0;
    DECLARE mensaje_error VARCHAR(255) DEFAULT '';

    DECLARE EXIT HANDLER FOR SQLEXCEPTION
    BEGIN
        GET DIAGNOSTICS CONDITION 1
            codigo_error = MYSQL_ERRNO,
            mensaje_error = MESSAGE_TEXT;
        SELECT 'ERROR' AS estado, codigo_error AS codigo_sql, mensaje_error AS descripcion_error;
    END;

    INSERT INTO Venta (EmpleadoID, UserID, Metodo_PagoID, Fecha, Hora, Monto_Total)
    VALUES (p_EmpleadoID, p_UserID, p_Metodo_PagoID, CURDATE(), CURTIME(), 0);

    INSERT INTO schema_usuarios.Log_Auditoria (UserID, Tipo_Evento_ID, Fecha, Hora, Detalle)
    SELECT p_UserID, 6, CURDATE(), CURTIME(), CONCAT('Venta ', MAX(VentaID))
    FROM Venta
    WHERE UserID = p_UserID;

    SELECT VentaID, UserID, EmpleadoID, Fecha, Hora, Monto_Total
    FROM Venta
    WHERE VentaID = (SELECT MAX(VentaID) FROM Venta WHERE UserID = p_UserID);
END //
DELIMITER ;

-- ----------------------------------------------------------------------------
-- SP 10: Agregar un producto al carrito de una venta
-- ----------------------------------------------------------------------------
DROP PROCEDURE IF EXISTS sp_agregar_producto_carrito;
DELIMITER //
CREATE PROCEDURE sp_agregar_producto_carrito(p_VentaID INT, p_ProductoID INT, p_Cantidad INT)
BEGIN
    DECLARE codigo_error INT DEFAULT 0;
    DECLARE mensaje_error VARCHAR(255) DEFAULT '';
    DECLARE filas INT DEFAULT 0;

    DECLARE EXIT HANDLER FOR SQLEXCEPTION
    BEGIN
        GET DIAGNOSTICS CONDITION 1
            codigo_error = MYSQL_ERRNO,
            mensaje_error = MESSAGE_TEXT;
        SELECT 'ERROR' AS estado, codigo_error AS codigo_sql, mensaje_error AS descripcion_error;
    END;

    INSERT INTO Carrito (VentaID, ProductoID, Cantidad, Precio_Unitario)
    SELECT p_VentaID, ProductoID, p_Cantidad, Precio_Producto
    FROM Producto
    WHERE ProductoID = p_ProductoID
      AND Activo = TRUE
      AND Stock_Producto >= p_Cantidad
      AND NOT EXISTS (SELECT 1 FROM Retiro WHERE VentaID = p_VentaID);

    SET filas = ROW_COUNT();

    UPDATE Producto
    SET Stock_Producto = Stock_Producto - p_Cantidad
    WHERE ProductoID = p_ProductoID AND filas = 1;

    UPDATE Venta
    SET Monto_Total = (SELECT COALESCE(SUM(Cantidad * Precio_Unitario), 0) FROM Carrito WHERE VentaID = p_VentaID)
    WHERE VentaID = p_VentaID;

    INSERT INTO schema_usuarios.Log_Auditoria (UserID, Tipo_Evento_ID, Fecha, Hora, Detalle)
    SELECT UserID, 7, CURDATE(), CURTIME(), CONCAT('Venta ', VentaID, ': ', p_Cantidad, ' x producto ', p_ProductoID)
    FROM Venta
    WHERE VentaID = p_VentaID AND filas = 1;

    SELECT filas AS Filas_Agregadas, VentaID, Monto_Total
    FROM Venta
    WHERE VentaID = p_VentaID;
END //
DELIMITER ;

-- ----------------------------------------------------------------------------
-- SP 11: Agregar un menú del día al carrito de una venta
-- ----------------------------------------------------------------------------
DROP PROCEDURE IF EXISTS sp_agregar_menu_carrito;
DELIMITER //
CREATE PROCEDURE sp_agregar_menu_carrito(p_VentaID INT, p_Menu_Del_DiaID INT, p_Cantidad INT, p_Dia VARCHAR(10))
BEGIN
    DECLARE codigo_error INT DEFAULT 0;
    DECLARE mensaje_error VARCHAR(255) DEFAULT '';
    DECLARE filas INT DEFAULT 0;

    DECLARE EXIT HANDLER FOR SQLEXCEPTION
    BEGIN
        GET DIAGNOSTICS CONDITION 1
            codigo_error = MYSQL_ERRNO,
            mensaje_error = MESSAGE_TEXT;
        SELECT 'ERROR' AS estado, codigo_error AS codigo_sql, mensaje_error AS descripcion_error;
    END;

    INSERT INTO Carrito (VentaID, Menu_Del_DiaID, Cantidad, Precio_Unitario)
    SELECT p_VentaID, Menu_Del_DiaID, p_Cantidad, Precio_menu
    FROM Menu_Del_Dia
    WHERE Menu_Del_DiaID = p_Menu_Del_DiaID
      AND Stock_menu >= p_Cantidad
      -- Solo el menú del día de hoy: el día lo calcula la app y lo pasa en p_Dia
      -- (sábado y domingo no hay menú, así que no coincide ninguno)
      AND Dia_Semana = p_Dia
      AND NOT EXISTS (SELECT 1 FROM Retiro WHERE VentaID = p_VentaID);

    SET filas = ROW_COUNT();

    UPDATE Menu_Del_Dia
    SET Stock_menu = Stock_menu - p_Cantidad
    WHERE Menu_Del_DiaID = p_Menu_Del_DiaID AND filas = 1;

    UPDATE Venta
    SET Monto_Total = (SELECT COALESCE(SUM(Cantidad * Precio_Unitario), 0) FROM Carrito WHERE VentaID = p_VentaID)
    WHERE VentaID = p_VentaID;

    INSERT INTO schema_usuarios.Log_Auditoria (UserID, Tipo_Evento_ID, Fecha, Hora, Detalle)
    SELECT UserID, 8, CURDATE(), CURTIME(), CONCAT('Venta ', VentaID, ': ', p_Cantidad, ' x menú ', p_Menu_Del_DiaID)
    FROM Venta
    WHERE VentaID = p_VentaID AND filas = 1;

    SELECT filas AS Filas_Agregadas, VentaID, Monto_Total
    FROM Venta
    WHERE VentaID = p_VentaID;
END //
DELIMITER ;

-- ----------------------------------------------------------------------------
-- SP 11b: Menú del día. Devuelve el menú fijo del día de la semana que manda la
-- aplicación (Qt calcula el día). Sábado, domingo o NULL: 0 filas.
-- ----------------------------------------------------------------------------
DROP PROCEDURE IF EXISTS sp_menu_del_dia;
DELIMITER //
CREATE PROCEDURE sp_menu_del_dia(p_Dia VARCHAR(10))
BEGIN
    DECLARE codigo_error INT DEFAULT 0;
    DECLARE mensaje_error VARCHAR(255) DEFAULT '';

    DECLARE EXIT HANDLER FOR SQLEXCEPTION
    BEGIN
        GET DIAGNOSTICS CONDITION 1
            codigo_error = MYSQL_ERRNO,
            mensaje_error = MESSAGE_TEXT;
        SELECT 'ERROR' AS estado, codigo_error AS codigo_sql, mensaje_error AS descripcion_error;
    END;

    SELECT m.Menu_Del_DiaID, m.Plato_menu, m.Precio_menu, m.Stock_menu, m.Dia_Semana,
           p.Nombre_Producto AS Incluye
    FROM Menu_Del_Dia m
    LEFT JOIN Detalle_Menu_Dia d ON m.Menu_Del_DiaID = d.Menu_Del_DiaID
    LEFT JOIN Producto p ON d.ProductoID = p.ProductoID
    WHERE m.Dia_Semana = p_Dia;
END //
DELIMITER ;

-- ----------------------------------------------------------------------------
-- SP 12: Generar el retiro (cierra la venta)
-- ----------------------------------------------------------------------------
DROP PROCEDURE IF EXISTS sp_generar_retiro;
DELIMITER //
CREATE PROCEDURE sp_generar_retiro(p_VentaID INT, p_ID_Franja INT, p_Codigo CHAR(5))
BEGIN
    DECLARE codigo_error INT DEFAULT 0;
    DECLARE mensaje_error VARCHAR(255) DEFAULT '';
    DECLARE filas INT DEFAULT 0;

    DECLARE EXIT HANDLER FOR SQLEXCEPTION
    BEGIN
        GET DIAGNOSTICS CONDITION 1
            codigo_error = MYSQL_ERRNO,
            mensaje_error = MESSAGE_TEXT;
        SELECT 'ERROR' AS estado, codigo_error AS codigo_sql, mensaje_error AS descripcion_error;
    END;

    INSERT INTO Retiro (VentaID, Estado_Retiro_ID, ID_Franja, Codigo_Retiro)
    SELECT VentaID, 1, p_ID_Franja, p_Codigo
    FROM Venta
    WHERE VentaID = p_VentaID
      AND EXISTS (SELECT 1 FROM Carrito WHERE VentaID = p_VentaID)
      AND (Metodo_PagoID <> 21
           OR (SELECT Saldo FROM schema_usuarios.Usuario u WHERE u.UserID = Venta.UserID) >= Monto_Total);

    SET filas = ROW_COUNT();

    UPDATE schema_usuarios.Usuario u
    INNER JOIN Venta v ON v.UserID = u.UserID
    SET u.Saldo = u.Saldo - v.Monto_Total
    WHERE v.VentaID = p_VentaID AND v.Metodo_PagoID = 21 AND filas = 1;

    INSERT INTO schema_usuarios.Log_Auditoria (UserID, Tipo_Evento_ID, Fecha, Hora, Detalle)
    SELECT UserID, 9, CURDATE(), CURTIME(), CONCAT('Venta ', VentaID, ': código ', p_Codigo)
    FROM Venta
    WHERE VentaID = p_VentaID AND filas = 1;

    SELECT filas AS Filas_Agregadas, r.RetiroID, r.Codigo_Retiro, f.Nombre_Franja
    FROM Retiro r
    INNER JOIN Franja_Retiro f ON r.ID_Franja = f.ID_Franja
    WHERE r.VentaID = p_VentaID;
END //
DELIMITER ;

-- ----------------------------------------------------------------------------
-- SP 13: Cambiar el estado de un retiro (lo usa el cajero): ENTREGADO (2) o CANCELADO (3)
-- ----------------------------------------------------------------------------
DROP PROCEDURE IF EXISTS sp_actualizar_estado_retiro;
DELIMITER //
CREATE PROCEDURE sp_actualizar_estado_retiro(p_UserID INT, p_RetiroID INT, p_Nuevo_EstadoID INT)
BEGIN
    DECLARE codigo_error INT DEFAULT 0;
    DECLARE mensaje_error VARCHAR(255) DEFAULT '';
    DECLARE filas INT DEFAULT 0;

    DECLARE EXIT HANDLER FOR SQLEXCEPTION
    BEGIN
        GET DIAGNOSTICS CONDITION 1
            codigo_error = MYSQL_ERRNO,
            mensaje_error = MESSAGE_TEXT;
        SELECT 'ERROR' AS estado, codigo_error AS codigo_sql, mensaje_error AS descripcion_error;
    END;

    UPDATE Retiro
    SET Estado_Retiro_ID = p_Nuevo_EstadoID
    -- Solo un pedido PENDIENTE (1) puede pasar a ENTREGADO (2) o CANCELADO (3)
    WHERE RetiroID = p_RetiroID AND Estado_Retiro_ID = 1 AND p_Nuevo_EstadoID IN (2, 3);

    SET filas = ROW_COUNT();

    UPDATE Producto
    SET Stock_Producto = Stock_Producto + (SELECT SUM(c.Cantidad)
                                           FROM Carrito c
                                           WHERE c.ProductoID = Producto.ProductoID
                                             AND c.VentaID = (SELECT VentaID FROM Retiro WHERE RetiroID = p_RetiroID))
    WHERE ProductoID IN (SELECT ProductoID FROM Carrito
                         WHERE VentaID = (SELECT VentaID FROM Retiro WHERE RetiroID = p_RetiroID))
      AND p_Nuevo_EstadoID = 3 AND filas = 1;

    UPDATE schema_usuarios.Usuario u
    INNER JOIN Venta v ON v.UserID = u.UserID
    INNER JOIN Retiro r ON r.VentaID = v.VentaID
    SET u.Saldo = u.Saldo + v.Monto_Total
    WHERE r.RetiroID = p_RetiroID AND v.Metodo_PagoID = 21
      AND p_Nuevo_EstadoID = 3 AND filas = 1;

    -- Log: entrega (10) o cancelación (11)
    INSERT INTO schema_usuarios.Log_Auditoria (UserID, Tipo_Evento_ID, Fecha, Hora, Detalle)
    SELECT p_UserID, 10, CURDATE(), CURTIME(), CONCAT('Retiro ', p_RetiroID, ' entregado')
    FROM Retiro
    WHERE RetiroID = p_RetiroID AND filas = 1 AND p_Nuevo_EstadoID = 2;

    INSERT INTO schema_usuarios.Log_Auditoria (UserID, Tipo_Evento_ID, Fecha, Hora, Detalle)
    SELECT p_UserID, 11, CURDATE(), CURTIME(), CONCAT('Retiro ', p_RetiroID, ' cancelado')
    FROM Retiro
    WHERE RetiroID = p_RetiroID AND filas = 1 AND p_Nuevo_EstadoID = 3;

    SELECT filas AS Filas_Actualizadas, r.RetiroID, er.Estado
    FROM Retiro r
    INNER JOIN Estado_Retiro er ON r.Estado_Retiro_ID = er.Estado_Retiro_ID
    WHERE r.RetiroID = p_RetiroID;
END //
DELIMITER ;


-- ############################################################################
-- 5.c  REPORTES (8)
-- ############################################################################
USE schema_comercial;

-- ----------------------------------------------------------------------------
-- Reporte 1: Ventas por categoría (productos y menús del día)
-- ----------------------------------------------------------------------------
DROP PROCEDURE IF EXISTS sp_reporte_ventas_por_categoria;
DELIMITER //
CREATE PROCEDURE sp_reporte_ventas_por_categoria(fechaInicio DATE, fechaFin DATE)
BEGIN
    DECLARE codigo_error INT DEFAULT 0;
    DECLARE mensaje_error VARCHAR(255) DEFAULT '';

    DECLARE EXIT HANDLER FOR SQLEXCEPTION
    BEGIN
        GET DIAGNOSTICS CONDITION 1
            codigo_error = MYSQL_ERRNO,
            mensaje_error = MESSAGE_TEXT;
        SELECT 'ERROR' AS estado, codigo_error AS codigo_sql, mensaje_error AS descripcion_error;
    END;

    SELECT
        cat.Categoria,
        COUNT(DISTINCT v.VentaID) AS Total_Ventas,
        SUM(c.Cantidad) AS Unidades_Vendidas,
        SUM(c.Cantidad * c.Precio_Unitario) AS Recaudacion_Total
    FROM Carrito c
    INNER JOIN Venta v ON c.VentaID = v.VentaID
    INNER JOIN Retiro r ON v.VentaID = r.VentaID
    LEFT JOIN Producto p ON c.ProductoID = p.ProductoID
    LEFT JOIN Menu_Del_Dia m ON c.Menu_Del_DiaID = m.Menu_Del_DiaID
    INNER JOIN Categoria cat ON cat.ID_Categoria = COALESCE(p.ID_Categoria, m.ID_Categoria)
    WHERE v.Fecha BETWEEN fechaInicio AND fechaFin
      AND r.Estado_Retiro_ID <> 3
    GROUP BY cat.Categoria
    ORDER BY Recaudacion_Total DESC;
END //
DELIMITER ;

-- ----------------------------------------------------------------------------
-- Reporte 2: Ranking de productos más vendidos
-- ----------------------------------------------------------------------------
DROP PROCEDURE IF EXISTS sp_reporte_top_productos;
DELIMITER //
CREATE PROCEDURE sp_reporte_top_productos(fechaInicio DATE, fechaFin DATE)
BEGIN
    DECLARE codigo_error INT DEFAULT 0;
    DECLARE mensaje_error VARCHAR(255) DEFAULT '';

    DECLARE EXIT HANDLER FOR SQLEXCEPTION
    BEGIN
        GET DIAGNOSTICS CONDITION 1
            codigo_error = MYSQL_ERRNO,
            mensaje_error = MESSAGE_TEXT;
        SELECT 'ERROR' AS estado, codigo_error AS codigo_sql, mensaje_error AS descripcion_error;
    END;

    SELECT
        p.ProductoID,
        p.Nombre_Producto,
        cat.Categoria,
        SUM(c.Cantidad) AS Total_Unidades,
        SUM(c.Cantidad * c.Precio_Unitario) AS Total_Recaudado
    FROM Producto p
    INNER JOIN Categoria cat ON p.ID_Categoria = cat.ID_Categoria
    INNER JOIN Carrito c ON p.ProductoID = c.ProductoID
    INNER JOIN Venta v ON c.VentaID = v.VentaID
    INNER JOIN Retiro r ON v.VentaID = r.VentaID
    WHERE v.Fecha BETWEEN fechaInicio AND fechaFin
      AND r.Estado_Retiro_ID <> 3
    GROUP BY p.ProductoID, p.Nombre_Producto, cat.Categoria
    ORDER BY Total_Unidades DESC, p.ProductoID;
END //
DELIMITER ;

-- ----------------------------------------------------------------------------
-- Reporte 3: Métodos de pago más utilizados
-- ----------------------------------------------------------------------------
DROP PROCEDURE IF EXISTS sp_reporte_metodos_pago;
DELIMITER //
CREATE PROCEDURE sp_reporte_metodos_pago(fechaInicio DATE, fechaFin DATE)
BEGIN
    DECLARE codigo_error INT DEFAULT 0;
    DECLARE mensaje_error VARCHAR(255) DEFAULT '';

    DECLARE EXIT HANDLER FOR SQLEXCEPTION
    BEGIN
        GET DIAGNOSTICS CONDITION 1
            codigo_error = MYSQL_ERRNO,
            mensaje_error = MESSAGE_TEXT;
        SELECT 'ERROR' AS estado, codigo_error AS codigo_sql, mensaje_error AS descripcion_error;
    END;

    SELECT
        mp.MetodoPago,
        COUNT(v.VentaID) AS Cantidad_Transacciones,
        SUM(v.Monto_Total) AS Total_Facturado,
        ROUND(AVG(v.Monto_Total), 2) AS Ticket_Promedio
    FROM Metodo_Pago mp
    INNER JOIN Venta v ON mp.Metodo_PagoID = v.Metodo_PagoID
    INNER JOIN Retiro r ON v.VentaID = r.VentaID
    WHERE v.Fecha BETWEEN fechaInicio AND fechaFin
      AND r.Estado_Retiro_ID <> 3
    GROUP BY mp.MetodoPago
    ORDER BY Total_Facturado DESC;
END //
DELIMITER ;

-- ----------------------------------------------------------------------------
-- Reporte 4: Ventas por día (cantidad, total, venta mínima y máxima)
-- ----------------------------------------------------------------------------
DROP PROCEDURE IF EXISTS sp_reporte_ventas_diarias;
DELIMITER //
CREATE PROCEDURE sp_reporte_ventas_diarias(fechaInicio DATE, fechaFin DATE)
BEGIN
    DECLARE codigo_error INT DEFAULT 0;
    DECLARE mensaje_error VARCHAR(255) DEFAULT '';

    DECLARE EXIT HANDLER FOR SQLEXCEPTION
    BEGIN
        GET DIAGNOSTICS CONDITION 1
            codigo_error = MYSQL_ERRNO,
            mensaje_error = MESSAGE_TEXT;
        SELECT 'ERROR' AS estado, codigo_error AS codigo_sql, mensaje_error AS descripcion_error;
    END;

    SELECT
        v.Fecha,
        COUNT(v.VentaID) AS Cantidad_Ventas,
        SUM(v.Monto_Total) AS Total_Dia,
        MIN(v.Monto_Total) AS Venta_Minima,
        MAX(v.Monto_Total) AS Venta_Maxima
    FROM Venta v
    INNER JOIN Retiro r ON v.VentaID = r.VentaID
    WHERE v.Fecha BETWEEN fechaInicio AND fechaFin
      AND r.Estado_Retiro_ID <> 3
    GROUP BY v.Fecha
    ORDER BY v.Fecha;
END //
DELIMITER ;

-- ----------------------------------------------------------------------------
-- Reporte 5: Estudiantes frecuentes (2 o más compras en el período)
-- ----------------------------------------------------------------------------
DROP PROCEDURE IF EXISTS sp_reporte_top_estudiantes;
DELIMITER //
CREATE PROCEDURE sp_reporte_top_estudiantes(fechaInicio DATE, fechaFin DATE)
BEGIN
    DECLARE codigo_error INT DEFAULT 0;
    DECLARE mensaje_error VARCHAR(255) DEFAULT '';

    DECLARE EXIT HANDLER FOR SQLEXCEPTION
    BEGIN
        GET DIAGNOSTICS CONDITION 1
            codigo_error = MYSQL_ERRNO,
            mensaje_error = MESSAGE_TEXT;
        SELECT 'ERROR' AS estado, codigo_error AS codigo_sql, mensaje_error AS descripcion_error;
    END;

    SELECT
        u.UserID,
        u.Apodo,
        e.Nombre_Estudiante,
        e.Apellido_Estudiante,
        cu.Anio,
        cu.Division,
        COUNT(v.VentaID) AS Compras_Realizadas,
        SUM(v.Monto_Total) AS Total_Gastado,
        MAX(v.Fecha) AS Ultima_Compra
    FROM schema_usuarios.Usuario u
    INNER JOIN schema_usuarios.Estudiante e ON u.DNI = e.DNI
    INNER JOIN schema_usuarios.Curso cu ON e.ID_Curso = cu.ID_Curso
    INNER JOIN Venta v ON u.UserID = v.UserID
    INNER JOIN Retiro r ON v.VentaID = r.VentaID
    WHERE v.Fecha BETWEEN fechaInicio AND fechaFin
      AND r.Estado_Retiro_ID <> 3
    GROUP BY u.UserID, u.Apodo, e.Nombre_Estudiante, e.Apellido_Estudiante, cu.Anio, cu.Division
    HAVING COUNT(v.VentaID) >= 2
    ORDER BY Total_Gastado DESC, u.UserID;
END //
DELIMITER ;

-- ----------------------------------------------------------------------------
-- Reporte 6: Rendimiento de empleados (ventas atendidas en mostrador)
-- ----------------------------------------------------------------------------
DROP PROCEDURE IF EXISTS sp_reporte_rendimiento_empleados;
DELIMITER //
CREATE PROCEDURE sp_reporte_rendimiento_empleados(fechaInicio DATE, fechaFin DATE)
BEGIN
    DECLARE codigo_error INT DEFAULT 0;
    DECLARE mensaje_error VARCHAR(255) DEFAULT '';

    DECLARE EXIT HANDLER FOR SQLEXCEPTION
    BEGIN
        GET DIAGNOSTICS CONDITION 1
            codigo_error = MYSQL_ERRNO,
            mensaje_error = MESSAGE_TEXT;
        SELECT 'ERROR' AS estado, codigo_error AS codigo_sql, mensaje_error AS descripcion_error;
    END;

    SELECT
        emp.EmpleadoID,
        emp.Nombre_Empleado,
        emp.Apellido_Empleado,
        pu.Nombre_Puesto,
        COUNT(v.VentaID) AS Ventas_Atendidas,
        SUM(v.Monto_Total) AS Total_Procesado,
        COUNT(DISTINCT v.Fecha) AS Dias_Trabajados
    FROM schema_usuarios.Empleado emp
    INNER JOIN schema_usuarios.Puesto pu ON emp.ID_Puesto = pu.ID_Puesto
    INNER JOIN Venta v ON emp.EmpleadoID = v.EmpleadoID
    INNER JOIN Retiro r ON v.VentaID = r.VentaID
    WHERE v.Fecha BETWEEN fechaInicio AND fechaFin
      AND r.Estado_Retiro_ID <> 3
    GROUP BY emp.EmpleadoID, emp.Nombre_Empleado, emp.Apellido_Empleado, pu.Nombre_Puesto
    ORDER BY Total_Procesado DESC;
END //
DELIMITER ;

-- ----------------------------------------------------------------------------
-- Reporte 7: Pedidos por franja de retiro y estado
-- ----------------------------------------------------------------------------
DROP PROCEDURE IF EXISTS sp_reporte_retiros_por_franja;
DELIMITER //
CREATE PROCEDURE sp_reporte_retiros_por_franja(fechaInicio DATE, fechaFin DATE)
BEGIN
    DECLARE codigo_error INT DEFAULT 0;
    DECLARE mensaje_error VARCHAR(255) DEFAULT '';

    DECLARE EXIT HANDLER FOR SQLEXCEPTION
    BEGIN
        GET DIAGNOSTICS CONDITION 1
            codigo_error = MYSQL_ERRNO,
            mensaje_error = MESSAGE_TEXT;
        SELECT 'ERROR' AS estado, codigo_error AS codigo_sql, mensaje_error AS descripcion_error;
    END;

    SELECT
        f.Nombre_Franja,
        er.Estado,
        COUNT(r.RetiroID) AS Cantidad_Pedidos
    FROM Retiro r
    INNER JOIN Franja_Retiro f ON r.ID_Franja = f.ID_Franja
    INNER JOIN Estado_Retiro er ON r.Estado_Retiro_ID = er.Estado_Retiro_ID
    INNER JOIN Venta v ON r.VentaID = v.VentaID
    WHERE v.Fecha BETWEEN fechaInicio AND fechaFin
    GROUP BY f.ID_Franja, f.Nombre_Franja, er.Estado
    ORDER BY f.ID_Franja, er.Estado;
END //
DELIMITER ;

-- ----------------------------------------------------------------------------
-- Reporte 8: Demanda del menú del día (vendidos por día de la semana y stock actual)
-- ----------------------------------------------------------------------------
DROP PROCEDURE IF EXISTS sp_reporte_demanda_menu_dia;
DELIMITER //
CREATE PROCEDURE sp_reporte_demanda_menu_dia(fechaInicio DATE, fechaFin DATE)
BEGIN
    DECLARE codigo_error INT DEFAULT 0;
    DECLARE mensaje_error VARCHAR(255) DEFAULT '';

    DECLARE EXIT HANDLER FOR SQLEXCEPTION
    BEGIN
        GET DIAGNOSTICS CONDITION 1
            codigo_error = MYSQL_ERRNO,
            mensaje_error = MESSAGE_TEXT;
        SELECT 'ERROR' AS estado, codigo_error AS codigo_sql, mensaje_error AS descripcion_error;
    END;

    -- El menú es fijo: se agrupa por día de la semana y el período se toma de la fecha de la venta
    SELECT
        m.Menu_Del_DiaID,
        m.Dia_Semana,
        m.Plato_menu,
        SUM(c.Cantidad) AS Cantidad_Vendida,
        m.Stock_menu AS Stock_Actual,
        SUM(c.Cantidad * c.Precio_Unitario) AS Recaudacion_Menu
    FROM Menu_Del_Dia m
    INNER JOIN Carrito c ON m.Menu_Del_DiaID = c.Menu_Del_DiaID
    INNER JOIN Venta v ON c.VentaID = v.VentaID
    INNER JOIN Retiro r ON c.VentaID = r.VentaID
    WHERE v.Fecha BETWEEN fechaInicio AND fechaFin
      AND r.Estado_Retiro_ID <> 3
    GROUP BY m.Menu_Del_DiaID, m.Dia_Semana, m.Plato_menu, m.Stock_menu
    ORDER BY Cantidad_Vendida DESC, m.Menu_Del_DiaID;
END //
DELIMITER ;


-- ############################################################################
-- 5.d  REPORTE DE AUDITORÍA
-- ############################################################################
USE schema_usuarios;

DROP PROCEDURE IF EXISTS sp_reporte_auditoria;
DELIMITER //
CREATE PROCEDURE sp_reporte_auditoria(fechaInicio DATE, fechaFin DATE, p_UserID INT, p_Tipo_Evento_ID INT)
BEGIN
    DECLARE codigo_error INT DEFAULT 0;
    DECLARE mensaje_error VARCHAR(255) DEFAULT '';

    DECLARE EXIT HANDLER FOR SQLEXCEPTION
    BEGIN
        GET DIAGNOSTICS CONDITION 1
            codigo_error = MYSQL_ERRNO,
            mensaje_error = MESSAGE_TEXT;
        SELECT 'ERROR' AS estado, codigo_error AS codigo_sql, mensaje_error AS descripcion_error;
    END;

    SELECT
        l.Log_ID,
        l.Fecha,
        l.Hora,
        te.Nombre_Evento,
        l.UserID,
        COALESCE(u.Apodo, '(sin usuario)') AS Apodo,
        l.Detalle
    FROM Log_Auditoria l
    INNER JOIN Tipo_Evento te ON l.Tipo_Evento_ID = te.Tipo_Evento_ID
    LEFT JOIN Usuario u ON l.UserID = u.UserID
    WHERE l.Fecha BETWEEN fechaInicio AND fechaFin
      AND (p_UserID IS NULL OR l.UserID = p_UserID)
      AND (p_Tipo_Evento_ID IS NULL OR l.Tipo_Evento_ID = p_Tipo_Evento_ID)
    ORDER BY l.Log_ID DESC;
END //
DELIMITER ;


-- ############################################################################
-- 5.e  REPORTES CON TUNING (consultas optimizadas)
-- ############################################################################
USE schema_comercial;

-- ----------------------------------------------------------------------------
-- Tuneado 1: Ranking de productos más vendidos (original: Reporte 2)
-- ----------------------------------------------------------------------------
DROP PROCEDURE IF EXISTS sp_reporte_top_productos_tuneado;
DELIMITER //
CREATE PROCEDURE sp_reporte_top_productos_tuneado(fechaInicio DATE, fechaFin DATE)
BEGIN
    DECLARE codigo_error INT DEFAULT 0;
    DECLARE mensaje_error VARCHAR(255) DEFAULT '';

    DECLARE EXIT HANDLER FOR SQLEXCEPTION
    BEGIN
        GET DIAGNOSTICS CONDITION 1
            codigo_error = MYSQL_ERRNO,
            mensaje_error = MESSAGE_TEXT;
        SELECT 'ERROR' AS estado, codigo_error AS codigo_sql, mensaje_error AS descripcion_error;
    END;

    -- Paso 1: solo las ventas del período que no fueron canceladas
    DROP TEMPORARY TABLE IF EXISTS tmp_ventas;
    CREATE TEMPORARY TABLE tmp_ventas AS
        SELECT v.VentaID
        FROM Venta v
        INNER JOIN Retiro r ON v.VentaID = r.VentaID
        WHERE v.Fecha BETWEEN fechaInicio AND fechaFin
          AND r.Estado_Retiro_ID <> 3;

    -- Paso 2: totales por producto, usando solo esas ventas
    DROP TEMPORARY TABLE IF EXISTS tmp_totales;
    CREATE TEMPORARY TABLE tmp_totales AS
        SELECT c.ProductoID,
               SUM(c.Cantidad) AS Total_Unidades,
               SUM(c.Cantidad * c.Precio_Unitario) AS Total_Recaudado
        FROM Carrito c
        INNER JOIN tmp_ventas t ON c.VentaID = t.VentaID
        WHERE c.ProductoID IS NOT NULL
        GROUP BY c.ProductoID;

    -- Paso 3: se agregan los nombres (pocas filas)
    SELECT p.ProductoID, p.Nombre_Producto, cat.Categoria, t.Total_Unidades, t.Total_Recaudado
    FROM tmp_totales t
    INNER JOIN Producto p ON t.ProductoID = p.ProductoID
    INNER JOIN Categoria cat ON p.ID_Categoria = cat.ID_Categoria
    ORDER BY t.Total_Unidades DESC, p.ProductoID;

    DROP TEMPORARY TABLE IF EXISTS tmp_totales;
    DROP TEMPORARY TABLE IF EXISTS tmp_ventas;
END //
DELIMITER ;

-- ----------------------------------------------------------------------------
-- Tuneado 2: Estudiantes frecuentes (original: Reporte 5)
-- ----------------------------------------------------------------------------
DROP PROCEDURE IF EXISTS sp_reporte_top_estudiantes_tuneado;
DELIMITER //
CREATE PROCEDURE sp_reporte_top_estudiantes_tuneado(fechaInicio DATE, fechaFin DATE)
BEGIN
    DECLARE codigo_error INT DEFAULT 0;
    DECLARE mensaje_error VARCHAR(255) DEFAULT '';

    DECLARE EXIT HANDLER FOR SQLEXCEPTION
    BEGIN
        GET DIAGNOSTICS CONDITION 1
            codigo_error = MYSQL_ERRNO,
            mensaje_error = MESSAGE_TEXT;
        SELECT 'ERROR' AS estado, codigo_error AS codigo_sql, mensaje_error AS descripcion_error;
    END;

    -- Paso 1: compras por usuario, solo del período y no canceladas
    DROP TEMPORARY TABLE IF EXISTS tmp_compras;
    CREATE TEMPORARY TABLE tmp_compras AS
        SELECT v.UserID,
               COUNT(v.VentaID) AS Compras_Realizadas,
               SUM(v.Monto_Total) AS Total_Gastado,
               MAX(v.Fecha) AS Ultima_Compra
        FROM Venta v
        INNER JOIN Retiro r ON v.VentaID = r.VentaID
        WHERE v.Fecha BETWEEN fechaInicio AND fechaFin
          AND r.Estado_Retiro_ID <> 3
        GROUP BY v.UserID
        HAVING COUNT(v.VentaID) >= 2;

    -- Paso 2: se agregan los datos del estudiante
    SELECT u.UserID, u.Apodo, e.Nombre_Estudiante, e.Apellido_Estudiante, cu.Anio, cu.Division,
           t.Compras_Realizadas, t.Total_Gastado, t.Ultima_Compra
    FROM tmp_compras t
    INNER JOIN schema_usuarios.Usuario u ON t.UserID = u.UserID
    INNER JOIN schema_usuarios.Estudiante e ON u.DNI = e.DNI
    INNER JOIN schema_usuarios.Curso cu ON e.ID_Curso = cu.ID_Curso
    ORDER BY t.Total_Gastado DESC, u.UserID;

    DROP TEMPORARY TABLE IF EXISTS tmp_compras;
END //
DELIMITER ;