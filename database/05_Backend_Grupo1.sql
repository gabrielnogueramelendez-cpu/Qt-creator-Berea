-- ============================================================================
-- PROYECTO INFORMÁTICO I - SECCIÓN BASES DE DATOS
-- El Buffet de Alejandro - Escuela Técnica N° 3 D.E. 9 "María Sánchez de Thompson"
-- PUNTO 5: FUNCIONALIDADES EN SQL (Stored Procedures)
--
-- Ejecutar DESPUÉS de 03_ModeloSQL y 04_DatosSQL. Se puede ejecutar varias veces
-- (cada SP se borra con DROP PROCEDURE IF EXISTS antes de crearse).
--
-- Todos los SPs devuelven una fila con la columna "estado":
--   'OK'    -> la operación se hizo
--   'ERROR' -> no se hizo; la columna "mensaje" (o "descripcion_error") dice por qué
--
-- CONTENIDO
--   5.a/b  Funcionalidades del sistema (cada una deja su registro en Log_Auditoria)
--   5.c    8 reportes (agrupamiento + reuniones + fechas)
--   5.d    Reporte de auditoría con parámetros
--   5.e    2 reportes con tuning (_tuneado)
--
-- IDs de Tipo_Evento usados (ver 04_DatosSQL):
--   1 Inicio de sesión      2 Cierre de sesión        3 Fallo de autenticación
--   4 Registro de usuario   5 Cambio de contraseña    6 Registro de venta
--   7 Producto al carrito   8 Menú al carrito         9 Código de retiro
--  10 Entrega de pedido    11 Cancelación de pedido  12 Cambio de estado de pedido
--  13 Alta de producto     14 Baja de producto       15 Modificación de stock
--  16 Alta de menú del día
-- IDs de Estado_Retiro usados: 1 PENDIENTE, 2 ENTREGADO, 3 CANCELADO
-- ============================================================================

SET NAMES utf8mb4;  -- para que las tildes y la ñ se guarden bien desde cualquier cliente


-- ############################################################################
-- 5.a / 5.b  FUNCIONALIDADES - SCHEMA USUARIOS
-- ############################################################################
USE schema_usuarios;

-- ----------------------------------------------------------------------------
-- sp_registrar_log: guarda un evento en Log_Auditoria con la fecha y hora actual.
-- Lo llaman todos los demás SPs. No tiene handler propio: si falla, el error
-- lo atrapa el handler del SP que lo llamó.
-- ----------------------------------------------------------------------------
DROP PROCEDURE IF EXISTS sp_registrar_log;
DELIMITER //
CREATE PROCEDURE sp_registrar_log(p_UserID INT, p_Tipo_Evento_ID INT, p_Detalle VARCHAR(255))
BEGIN
    INSERT INTO schema_usuarios.Log_Auditoria (UserID, Tipo_Evento_ID, Fecha, Hora, Detalle)
    VALUES (p_UserID, p_Tipo_Evento_ID, CURDATE(), CURTIME(), p_Detalle);
END //
DELIMITER ;

-- ----------------------------------------------------------------------------
-- SP 1: Registrar un usuario nuevo (alumno que se da de alta en la app)
-- ----------------------------------------------------------------------------
DROP PROCEDURE IF EXISTS sp_registrar_usuario;
DELIMITER //
CREATE PROCEDURE sp_registrar_usuario(p_DNI VARCHAR(20), p_Apodo VARCHAR(50),
                                      p_Contrasena VARCHAR(50), p_Email VARCHAR(150))
BEGIN
    DECLARE codigo_error INT DEFAULT 0;
    DECLARE mensaje_error VARCHAR(255) DEFAULT '';
    DECLARE v_UserID INT;

    DECLARE EXIT HANDLER FOR SQLEXCEPTION
    BEGIN
        GET DIAGNOSTICS CONDITION 1
            codigo_error = MYSQL_ERRNO,
            mensaje_error = MESSAGE_TEXT;
        SELECT 'ERROR' AS estado, codigo_error AS codigo_sql, mensaje_error AS descripcion_error;
    END;

    IF p_DNI IS NULL OR p_DNI = '' OR p_Apodo IS NULL OR p_Apodo = ''
       OR p_Contrasena IS NULL OR p_Contrasena = '' OR p_Email IS NULL OR p_Email = '' THEN
        SELECT 'ERROR' AS estado, 'Faltan datos obligatorios' AS mensaje;
    ELSEIF NOT EXISTS (SELECT 1 FROM Estudiante WHERE DNI = p_DNI) THEN
        SELECT 'ERROR' AS estado, 'El DNI no corresponde a un estudiante' AS mensaje;
    ELSEIF EXISTS (SELECT 1 FROM Usuario WHERE DNI = p_DNI) THEN
        SELECT 'ERROR' AS estado, 'Ese estudiante ya tiene un usuario' AS mensaje;
    ELSEIF EXISTS (SELECT 1 FROM Usuario WHERE Apodo = p_Apodo) THEN
        SELECT 'ERROR' AS estado, 'El apodo ya está en uso' AS mensaje;
    ELSEIF EXISTS (SELECT 1 FROM Email WHERE Email = p_Email) THEN
        SELECT 'ERROR' AS estado, 'El email ya está registrado' AS mensaje;
    ELSE
        INSERT INTO Usuario (DNI, Apodo, Contrasena)
        VALUES (p_DNI, p_Apodo, p_Contrasena);
        SET v_UserID = LAST_INSERT_ID();

        INSERT INTO Email (UserID, Email) VALUES (v_UserID, p_Email);

        CALL sp_registrar_log(v_UserID, 4, CONCAT('Nuevo usuario: ', p_Apodo));
        SELECT 'OK' AS estado, v_UserID AS UserID, 'Usuario registrado' AS mensaje;
    END IF;
END //
DELIMITER ;

-- ----------------------------------------------------------------------------
-- SP 2: Iniciar sesión (apodo + contraseña)
-- Si los datos son correctos registra "Inicio de Sesión Exitoso" (1);
-- si no, registra "Fallo de Autenticación" (3).
-- Devuelve el tipo de usuario: 'Estudiante' o el puesto del empleado.
-- ----------------------------------------------------------------------------
DROP PROCEDURE IF EXISTS sp_iniciar_sesion;
DELIMITER //
CREATE PROCEDURE sp_iniciar_sesion(p_Apodo VARCHAR(50), p_Contrasena VARCHAR(50))
BEGIN
    DECLARE codigo_error INT DEFAULT 0;
    DECLARE mensaje_error VARCHAR(255) DEFAULT '';
    DECLARE v_UserID INT DEFAULT NULL;

    DECLARE EXIT HANDLER FOR SQLEXCEPTION
    BEGIN
        GET DIAGNOSTICS CONDITION 1
            codigo_error = MYSQL_ERRNO,
            mensaje_error = MESSAGE_TEXT;
        SELECT 'ERROR' AS estado, codigo_error AS codigo_sql, mensaje_error AS descripcion_error;
    END;

    SELECT UserID INTO v_UserID
    FROM Usuario
    WHERE Apodo = p_Apodo AND Contrasena = p_Contrasena;

    IF v_UserID IS NULL THEN
        -- Se busca el usuario solo por apodo, para saber a quién intentaron entrar
        SELECT UserID INTO v_UserID FROM Usuario WHERE Apodo = p_Apodo;
        CALL sp_registrar_log(v_UserID, 3, CONCAT('Intento fallido con el apodo: ', IFNULL(p_Apodo, '(vacío)')));
        SELECT 'ERROR' AS estado, 'Usuario o contraseña incorrectos' AS mensaje;
    ELSE
        CALL sp_registrar_log(v_UserID, 1, 'Inicio de sesión');
        SELECT 'OK' AS estado,
               u.UserID,
               u.Apodo,
               IFNULL(p.Nombre_Puesto, 'Estudiante') AS Tipo_Usuario
        FROM Usuario u
        LEFT JOIN Empleado e ON u.UserID = e.UserID
        LEFT JOIN Puesto p ON e.ID_Puesto = p.ID_Puesto
        WHERE u.UserID = v_UserID;
    END IF;
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

    IF NOT EXISTS (SELECT 1 FROM Usuario WHERE UserID = p_UserID) THEN
        SELECT 'ERROR' AS estado, 'El usuario no existe' AS mensaje;
    ELSE
        CALL sp_registrar_log(p_UserID, 2, 'Cierre de sesión');
        SELECT 'OK' AS estado, p_UserID AS UserID, 'Sesión cerrada' AS mensaje;
    END IF;
END //
DELIMITER ;

-- ----------------------------------------------------------------------------
-- SP 4: Cambiar contraseña (pide la actual para confirmar)
-- ----------------------------------------------------------------------------
DROP PROCEDURE IF EXISTS sp_cambiar_contrasena;
DELIMITER //
CREATE PROCEDURE sp_cambiar_contrasena(p_UserID INT, p_Actual VARCHAR(50), p_Nueva VARCHAR(50))
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

    IF p_Nueva IS NULL OR p_Nueva = '' THEN
        SELECT 'ERROR' AS estado, 'La contraseña nueva no puede estar vacía' AS mensaje;
    ELSEIF NOT EXISTS (SELECT 1 FROM Usuario
                       WHERE UserID = p_UserID AND Contrasena = p_Actual) THEN
        SELECT 'ERROR' AS estado, 'Usuario o contraseña actual incorrectos' AS mensaje;
    ELSE
        UPDATE Usuario SET Contrasena = p_Nueva WHERE UserID = p_UserID;
        CALL sp_registrar_log(p_UserID, 5, 'Cambio de contraseña');
        SELECT 'OK' AS estado, 'Contraseña actualizada' AS mensaje;
    END IF;
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
    DECLARE v_ProductoID INT;

    DECLARE EXIT HANDLER FOR SQLEXCEPTION
    BEGIN
        GET DIAGNOSTICS CONDITION 1
            codigo_error = MYSQL_ERRNO,
            mensaje_error = MESSAGE_TEXT;
        SELECT 'ERROR' AS estado, codigo_error AS codigo_sql, mensaje_error AS descripcion_error;
    END;

    IF p_Nombre IS NULL OR p_Nombre = '' OR p_Stock IS NULL OR p_Stock < 0
       OR p_Precio IS NULL OR p_Precio < 0 THEN
        SELECT 'ERROR' AS estado, 'Datos del producto inválidos' AS mensaje;
    ELSEIF NOT EXISTS (SELECT 1 FROM Categoria WHERE ID_Categoria = p_ID_Categoria) THEN
        SELECT 'ERROR' AS estado, 'La categoría no existe' AS mensaje;
    ELSE
        INSERT INTO Producto (ID_Categoria, Nombre_Producto, Descripcion_Producto, Stock_Producto, Precio_Producto)
        VALUES (p_ID_Categoria, p_Nombre, p_Descripcion, p_Stock, p_Precio);
        SET v_ProductoID = LAST_INSERT_ID();

        CALL schema_usuarios.sp_registrar_log(p_UserID, 13, CONCAT('Alta de producto: ', p_Nombre));
        SELECT 'OK' AS estado, v_ProductoID AS ProductoID, 'Producto dado de alta' AS mensaje;
    END IF;
END //
DELIMITER ;

-- ----------------------------------------------------------------------------
-- SP 6: Modificar el stock de un producto
-- ----------------------------------------------------------------------------
DROP PROCEDURE IF EXISTS sp_modificar_stock;
DELIMITER //
CREATE PROCEDURE sp_modificar_stock(p_UserID INT, p_ProductoID INT, p_NuevoStock INT)
BEGIN
    DECLARE codigo_error INT DEFAULT 0;
    DECLARE mensaje_error VARCHAR(255) DEFAULT '';
    DECLARE v_StockAnterior INT;

    DECLARE EXIT HANDLER FOR SQLEXCEPTION
    BEGIN
        GET DIAGNOSTICS CONDITION 1
            codigo_error = MYSQL_ERRNO,
            mensaje_error = MESSAGE_TEXT;
        SELECT 'ERROR' AS estado, codigo_error AS codigo_sql, mensaje_error AS descripcion_error;
    END;

    SELECT Stock_Producto INTO v_StockAnterior FROM Producto WHERE ProductoID = p_ProductoID;

    IF v_StockAnterior IS NULL THEN
        SELECT 'ERROR' AS estado, 'El producto no existe' AS mensaje;
    ELSEIF p_NuevoStock IS NULL OR p_NuevoStock < 0 THEN
        SELECT 'ERROR' AS estado, 'El stock no puede ser negativo ni vacío' AS mensaje;
    ELSE
        UPDATE Producto SET Stock_Producto = p_NuevoStock WHERE ProductoID = p_ProductoID;
        CALL schema_usuarios.sp_registrar_log(p_UserID, 15,
            CONCAT('Producto ', p_ProductoID, ': stock ', v_StockAnterior, ' -> ', p_NuevoStock));
        SELECT 'OK' AS estado, p_ProductoID AS ProductoID, p_NuevoStock AS Stock_Producto, 'Stock actualizado' AS mensaje;
    END IF;
END //
DELIMITER ;

-- ----------------------------------------------------------------------------
-- SP 7: Baja de producto (baja lógica: Activo = FALSE, no se borra la fila
-- porque las ventas viejas lo siguen referenciando)
-- ----------------------------------------------------------------------------
DROP PROCEDURE IF EXISTS sp_baja_producto;
DELIMITER //
CREATE PROCEDURE sp_baja_producto(p_UserID INT, p_ProductoID INT)
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

    IF NOT EXISTS (SELECT 1 FROM Producto WHERE ProductoID = p_ProductoID AND Activo = TRUE) THEN
        SELECT 'ERROR' AS estado, 'El producto no existe o ya estaba dado de baja' AS mensaje;
    ELSE
        UPDATE Producto SET Activo = FALSE WHERE ProductoID = p_ProductoID;
        CALL schema_usuarios.sp_registrar_log(p_UserID, 14, CONCAT('Baja del producto ', p_ProductoID));
        SELECT 'OK' AS estado, p_ProductoID AS ProductoID, 'Producto dado de baja' AS mensaje;
    END IF;
END //
DELIMITER ;

-- ----------------------------------------------------------------------------
-- SP 8: Alta del menú del día (con el producto que incluye, ej.: la bebida)
-- p_ProductoIncluido puede ser NULL si el menú no incluye ningún producto.
-- ----------------------------------------------------------------------------
DROP PROCEDURE IF EXISTS sp_alta_menu_dia;
DELIMITER //
CREATE PROCEDURE sp_alta_menu_dia(p_UserID INT, p_ID_Categoria INT, p_Plato VARCHAR(100),
                                  p_Precio DECIMAL(10,2), p_Stock INT, p_Fecha DATE,
                                  p_ProductoIncluido INT)
BEGIN
    DECLARE codigo_error INT DEFAULT 0;
    DECLARE mensaje_error VARCHAR(255) DEFAULT '';
    DECLARE v_MenuID INT;

    DECLARE EXIT HANDLER FOR SQLEXCEPTION
    BEGIN
        GET DIAGNOSTICS CONDITION 1
            codigo_error = MYSQL_ERRNO,
            mensaje_error = MESSAGE_TEXT;
        SELECT 'ERROR' AS estado, codigo_error AS codigo_sql, mensaje_error AS descripcion_error;
    END;

    IF p_Plato IS NULL OR p_Plato = '' OR p_Precio IS NULL OR p_Precio < 0
       OR p_Stock IS NULL OR p_Stock < 0 OR p_Fecha IS NULL THEN
        SELECT 'ERROR' AS estado, 'Datos del menú inválidos' AS mensaje;
    ELSEIF NOT EXISTS (SELECT 1 FROM Categoria WHERE ID_Categoria = p_ID_Categoria) THEN
        SELECT 'ERROR' AS estado, 'La categoría no existe' AS mensaje;
    ELSEIF p_ProductoIncluido IS NOT NULL
           AND NOT EXISTS (SELECT 1 FROM Producto WHERE ProductoID = p_ProductoIncluido) THEN
        SELECT 'ERROR' AS estado, 'El producto incluido no existe' AS mensaje;
    ELSE
        INSERT INTO Menu_Del_Dia (ID_Categoria, Plato_menu, Precio_menu, Stock_menu, Fecha_menu)
        VALUES (p_ID_Categoria, p_Plato, p_Precio, p_Stock, p_Fecha);
        SET v_MenuID = LAST_INSERT_ID();

        IF p_ProductoIncluido IS NOT NULL THEN
            INSERT INTO Detalle_Menu_Dia (Menu_Del_DiaID, ProductoID) VALUES (v_MenuID, p_ProductoIncluido);
        END IF;

        CALL schema_usuarios.sp_registrar_log(p_UserID, 16, CONCAT('Menú del ', p_Fecha, ': ', p_Plato));
        SELECT 'OK' AS estado, v_MenuID AS Menu_Del_DiaID, 'Menú del día cargado' AS mensaje;
    END IF;
END //
DELIMITER ;

-- ----------------------------------------------------------------------------
-- SP 9: Registrar venta (abre la venta con monto 0; el monto se va sumando
-- al agregar productos o menús con los SPs 10 y 11).
-- p_EmpleadoID es NULL cuando el alumno hace el pedido desde la app.
-- ----------------------------------------------------------------------------
DROP PROCEDURE IF EXISTS sp_registrar_venta;
DELIMITER //
CREATE PROCEDURE sp_registrar_venta(p_UserID INT, p_EmpleadoID INT, p_Metodo_PagoID INT)
BEGIN
    DECLARE codigo_error INT DEFAULT 0;
    DECLARE mensaje_error VARCHAR(255) DEFAULT '';
    DECLARE v_VentaID INT;

    DECLARE EXIT HANDLER FOR SQLEXCEPTION
    BEGIN
        GET DIAGNOSTICS CONDITION 1
            codigo_error = MYSQL_ERRNO,
            mensaje_error = MESSAGE_TEXT;
        SELECT 'ERROR' AS estado, codigo_error AS codigo_sql, mensaje_error AS descripcion_error;
    END;

    IF NOT EXISTS (SELECT 1 FROM schema_usuarios.Usuario WHERE UserID = p_UserID) THEN
        SELECT 'ERROR' AS estado, 'El usuario no existe' AS mensaje;
    ELSEIF NOT EXISTS (SELECT 1 FROM Metodo_Pago WHERE Metodo_PagoID = p_Metodo_PagoID) THEN
        SELECT 'ERROR' AS estado, 'El método de pago no existe' AS mensaje;
    ELSEIF p_EmpleadoID IS NOT NULL
           AND NOT EXISTS (SELECT 1 FROM schema_usuarios.Empleado WHERE EmpleadoID = p_EmpleadoID) THEN
        SELECT 'ERROR' AS estado, 'El empleado no existe' AS mensaje;
    ELSE
        INSERT INTO Venta (EmpleadoID, UserID, Metodo_PagoID, Fecha, Hora, Monto_Total)
        VALUES (p_EmpleadoID, p_UserID, p_Metodo_PagoID, CURDATE(), CURTIME(), 0);
        SET v_VentaID = LAST_INSERT_ID();   -- se guarda ANTES de insertar el log

        CALL schema_usuarios.sp_registrar_log(p_UserID, 6, CONCAT('Venta ', v_VentaID));
        SELECT 'OK' AS estado, v_VentaID AS VentaID, 'Venta registrada' AS mensaje;
    END IF;
END //
DELIMITER ;

-- ----------------------------------------------------------------------------
-- SP 10: Agregar un producto al carrito de una venta
-- Controla stock, descuenta el stock y suma el importe al total de la venta.
-- ----------------------------------------------------------------------------
DROP PROCEDURE IF EXISTS sp_agregar_producto_carrito;
DELIMITER //
CREATE PROCEDURE sp_agregar_producto_carrito(p_VentaID INT, p_ProductoID INT, p_Cantidad INT)
BEGIN
    DECLARE codigo_error INT DEFAULT 0;
    DECLARE mensaje_error VARCHAR(255) DEFAULT '';
    DECLARE v_Precio DECIMAL(10,2);
    DECLARE v_Stock INT;
    DECLARE v_Activo BOOLEAN;
    DECLARE v_UserID INT;

    DECLARE EXIT HANDLER FOR SQLEXCEPTION
    BEGIN
        GET DIAGNOSTICS CONDITION 1
            codigo_error = MYSQL_ERRNO,
            mensaje_error = MESSAGE_TEXT;
        SELECT 'ERROR' AS estado, codigo_error AS codigo_sql, mensaje_error AS descripcion_error;
    END;

    SELECT Precio_Producto, Stock_Producto, Activo INTO v_Precio, v_Stock, v_Activo
    FROM Producto WHERE ProductoID = p_ProductoID;

    SELECT UserID INTO v_UserID FROM Venta WHERE VentaID = p_VentaID;

    IF v_UserID IS NULL THEN
        SELECT 'ERROR' AS estado, 'La venta no existe' AS mensaje;
    ELSEIF EXISTS (SELECT 1 FROM Retiro WHERE VentaID = p_VentaID) THEN
        SELECT 'ERROR' AS estado, 'La venta ya está cerrada (tiene código de retiro)' AS mensaje;
    ELSEIF v_Precio IS NULL OR v_Activo = FALSE THEN
        SELECT 'ERROR' AS estado, 'El producto no existe o está dado de baja' AS mensaje;
    ELSEIF p_Cantidad IS NULL OR p_Cantidad <= 0 THEN
        SELECT 'ERROR' AS estado, 'La cantidad debe ser mayor a 0' AS mensaje;
    ELSEIF p_Cantidad > v_Stock THEN
        SELECT 'ERROR' AS estado, CONCAT('Stock insuficiente. Disponible: ', v_Stock) AS mensaje;
    ELSE
        INSERT INTO Carrito (VentaID, ProductoID, Cantidad, Precio_Unitario)
        VALUES (p_VentaID, p_ProductoID, p_Cantidad, v_Precio);

        UPDATE Producto SET Stock_Producto = Stock_Producto - p_Cantidad WHERE ProductoID = p_ProductoID;
        UPDATE Venta SET Monto_Total = Monto_Total + (p_Cantidad * v_Precio) WHERE VentaID = p_VentaID;

        CALL schema_usuarios.sp_registrar_log(v_UserID, 7,
            CONCAT('Venta ', p_VentaID, ': ', p_Cantidad, ' x producto ', p_ProductoID));
        SELECT 'OK' AS estado, p_VentaID AS VentaID,
               (SELECT Monto_Total FROM Venta WHERE VentaID = p_VentaID) AS Monto_Total,
               'Producto agregado' AS mensaje;
    END IF;
END //
DELIMITER ;

-- ----------------------------------------------------------------------------
-- SP 11: Agregar un menú del día al carrito de una venta
-- Solo se puede comprar el menú del mismo día de la venta.
-- ----------------------------------------------------------------------------
DROP PROCEDURE IF EXISTS sp_agregar_menu_carrito;
DELIMITER //
CREATE PROCEDURE sp_agregar_menu_carrito(p_VentaID INT, p_Menu_Del_DiaID INT, p_Cantidad INT)
BEGIN
    DECLARE codigo_error INT DEFAULT 0;
    DECLARE mensaje_error VARCHAR(255) DEFAULT '';
    DECLARE v_Precio DECIMAL(10,2);
    DECLARE v_Stock INT;
    DECLARE v_FechaMenu DATE;
    DECLARE v_FechaVenta DATE;
    DECLARE v_UserID INT;

    DECLARE EXIT HANDLER FOR SQLEXCEPTION
    BEGIN
        GET DIAGNOSTICS CONDITION 1
            codigo_error = MYSQL_ERRNO,
            mensaje_error = MESSAGE_TEXT;
        SELECT 'ERROR' AS estado, codigo_error AS codigo_sql, mensaje_error AS descripcion_error;
    END;

    SELECT Precio_menu, Stock_menu, Fecha_menu INTO v_Precio, v_Stock, v_FechaMenu
    FROM Menu_Del_Dia WHERE Menu_Del_DiaID = p_Menu_Del_DiaID;

    SELECT UserID, Fecha INTO v_UserID, v_FechaVenta
    FROM Venta WHERE VentaID = p_VentaID;

    IF v_UserID IS NULL THEN
        SELECT 'ERROR' AS estado, 'La venta no existe' AS mensaje;
    ELSEIF EXISTS (SELECT 1 FROM Retiro WHERE VentaID = p_VentaID) THEN
        SELECT 'ERROR' AS estado, 'La venta ya está cerrada (tiene código de retiro)' AS mensaje;
    ELSEIF v_Precio IS NULL THEN
        SELECT 'ERROR' AS estado, 'El menú no existe' AS mensaje;
    ELSEIF v_FechaMenu <> v_FechaVenta THEN
        SELECT 'ERROR' AS estado, 'Ese menú no corresponde al día de la venta' AS mensaje;
    ELSEIF p_Cantidad IS NULL OR p_Cantidad <= 0 THEN
        SELECT 'ERROR' AS estado, 'La cantidad debe ser mayor a 0' AS mensaje;
    ELSEIF p_Cantidad > v_Stock THEN
        SELECT 'ERROR' AS estado, CONCAT('No quedan suficientes menús. Disponibles: ', v_Stock) AS mensaje;
    ELSE
        INSERT INTO Carrito (VentaID, Menu_Del_DiaID, Cantidad, Precio_Unitario)
        VALUES (p_VentaID, p_Menu_Del_DiaID, p_Cantidad, v_Precio);

        UPDATE Menu_Del_Dia SET Stock_menu = Stock_menu - p_Cantidad WHERE Menu_Del_DiaID = p_Menu_Del_DiaID;
        UPDATE Venta SET Monto_Total = Monto_Total + (p_Cantidad * v_Precio) WHERE VentaID = p_VentaID;

        CALL schema_usuarios.sp_registrar_log(v_UserID, 8,
            CONCAT('Venta ', p_VentaID, ': ', p_Cantidad, ' x menú ', p_Menu_Del_DiaID));
        SELECT 'OK' AS estado, p_VentaID AS VentaID,
               (SELECT Monto_Total FROM Venta WHERE VentaID = p_VentaID) AS Monto_Total,
               'Menú agregado' AS mensaje;
    END IF;
END //
DELIMITER ;

-- ----------------------------------------------------------------------------
-- SP 12: Generar el retiro (cierra la venta y da el código de 5 caracteres)
-- ----------------------------------------------------------------------------
DROP PROCEDURE IF EXISTS sp_generar_retiro;
DELIMITER //
CREATE PROCEDURE sp_generar_retiro(p_VentaID INT, p_ID_Franja INT)
BEGIN
    DECLARE codigo_error INT DEFAULT 0;
    DECLARE mensaje_error VARCHAR(255) DEFAULT '';
    DECLARE v_UserID INT;
    DECLARE v_Codigo CHAR(5);

    DECLARE EXIT HANDLER FOR SQLEXCEPTION
    BEGIN
        GET DIAGNOSTICS CONDITION 1
            codigo_error = MYSQL_ERRNO,
            mensaje_error = MESSAGE_TEXT;
        SELECT 'ERROR' AS estado, codigo_error AS codigo_sql, mensaje_error AS descripcion_error;
    END;

    SELECT UserID INTO v_UserID FROM Venta WHERE VentaID = p_VentaID;

    IF v_UserID IS NULL THEN
        SELECT 'ERROR' AS estado, 'La venta no existe' AS mensaje;
    ELSEIF NOT EXISTS (SELECT 1 FROM Carrito WHERE VentaID = p_VentaID) THEN
        SELECT 'ERROR' AS estado, 'La venta no tiene productos' AS mensaje;
    ELSEIF EXISTS (SELECT 1 FROM Retiro WHERE VentaID = p_VentaID) THEN
        SELECT 'ERROR' AS estado, 'La venta ya tiene código de retiro' AS mensaje;
    ELSEIF NOT EXISTS (SELECT 1 FROM Franja_Retiro WHERE ID_Franja = p_ID_Franja) THEN
        SELECT 'ERROR' AS estado, 'La franja de retiro no existe' AS mensaje;
    ELSE
        -- Código al azar de 5 caracteres (letras y números)
        SET v_Codigo = UPPER(LEFT(MD5(RAND()), 5));

        INSERT INTO Retiro (VentaID, Estado_Retiro_ID, ID_Franja, Codigo_Retiro)
        VALUES (p_VentaID, 1, p_ID_Franja, v_Codigo);

        CALL schema_usuarios.sp_registrar_log(v_UserID, 9, CONCAT('Venta ', p_VentaID, ': código ', v_Codigo));
        SELECT 'OK' AS estado, p_VentaID AS VentaID, v_Codigo AS Codigo_Retiro, 'Pedido confirmado' AS mensaje;
    END IF;
END //
DELIMITER ;

-- ----------------------------------------------------------------------------
-- SP 13: Cambiar el estado de un retiro (lo usa el cajero)
-- Un pedido ENTREGADO o CANCELADO ya no se puede modificar (evita doble entrega).
-- Si se cancela, se devuelve el stock de productos y menús.
-- ----------------------------------------------------------------------------
DROP PROCEDURE IF EXISTS sp_actualizar_estado_retiro;
DELIMITER //
CREATE PROCEDURE sp_actualizar_estado_retiro(p_UserID INT, p_RetiroID INT, p_Nuevo_EstadoID INT)
BEGIN
    DECLARE codigo_error INT DEFAULT 0;
    DECLARE mensaje_error VARCHAR(255) DEFAULT '';
    DECLARE v_EstadoActual INT;
    DECLARE v_VentaID INT;

    DECLARE EXIT HANDLER FOR SQLEXCEPTION
    BEGIN
        GET DIAGNOSTICS CONDITION 1
            codigo_error = MYSQL_ERRNO,
            mensaje_error = MESSAGE_TEXT;
        SELECT 'ERROR' AS estado, codigo_error AS codigo_sql, mensaje_error AS descripcion_error;
    END;

    SELECT Estado_Retiro_ID, VentaID INTO v_EstadoActual, v_VentaID
    FROM Retiro WHERE RetiroID = p_RetiroID;

    IF v_EstadoActual IS NULL THEN
        SELECT 'ERROR' AS estado, 'El retiro no existe' AS mensaje;
    ELSEIF NOT EXISTS (SELECT 1 FROM Estado_Retiro WHERE Estado_Retiro_ID = p_Nuevo_EstadoID) THEN
        SELECT 'ERROR' AS estado, 'El estado no existe' AS mensaje;
    ELSEIF v_EstadoActual IN (2, 3) THEN
        SELECT 'ERROR' AS estado, 'El pedido ya fue entregado o cancelado' AS mensaje;
    ELSE
        UPDATE Retiro SET Estado_Retiro_ID = p_Nuevo_EstadoID WHERE RetiroID = p_RetiroID;

        IF p_Nuevo_EstadoID = 3 THEN
            -- Devolver el stock de lo que tenía el pedido
            UPDATE Producto p
            INNER JOIN Carrito c ON p.ProductoID = c.ProductoID
            SET p.Stock_Producto = p.Stock_Producto + c.Cantidad
            WHERE c.VentaID = v_VentaID;

            UPDATE Menu_Del_Dia m
            INNER JOIN Carrito c ON m.Menu_Del_DiaID = c.Menu_Del_DiaID
            SET m.Stock_menu = m.Stock_menu + c.Cantidad
            WHERE c.VentaID = v_VentaID;

            CALL schema_usuarios.sp_registrar_log(p_UserID, 11, CONCAT('Retiro ', p_RetiroID, ' cancelado'));
        ELSEIF p_Nuevo_EstadoID = 2 THEN
            CALL schema_usuarios.sp_registrar_log(p_UserID, 10, CONCAT('Retiro ', p_RetiroID, ' entregado'));
        ELSE
            CALL schema_usuarios.sp_registrar_log(p_UserID, 12,
                CONCAT('Retiro ', p_RetiroID, ': estado ', v_EstadoActual, ' -> ', p_Nuevo_EstadoID));
        END IF;

        SELECT 'OK' AS estado, p_RetiroID AS RetiroID, 'Estado del retiro actualizado' AS mensaje;
    END IF;
END //
DELIMITER ;


-- ############################################################################
-- 5.c  REPORTES (8)
-- Todos reciben un rango de fechas. Si falta una fecha o el rango está al revés,
-- devuelven un mensaje de error y no ejecutan la consulta.
-- Las ventas que cuentan son las confirmadas (con retiro) y NO canceladas.
-- ############################################################################
USE schema_comercial;

-- ----------------------------------------------------------------------------
-- Reporte 1: Ventas por categoría (incluye productos y menús del día)
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

    IF fechaInicio IS NULL OR fechaFin IS NULL OR fechaInicio > fechaFin THEN
        SELECT 'ERROR' AS estado, 'Rango de fechas inválido' AS mensaje;
    ELSE
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
        INNER JOIN Categoria cat ON cat.ID_Categoria = IFNULL(p.ID_Categoria, m.ID_Categoria)
        WHERE v.Fecha BETWEEN fechaInicio AND fechaFin
          AND r.Estado_Retiro_ID <> 3
        GROUP BY cat.ID_Categoria, cat.Categoria
        ORDER BY Recaudacion_Total DESC;
    END IF;
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

    IF fechaInicio IS NULL OR fechaFin IS NULL OR fechaInicio > fechaFin THEN
        SELECT 'ERROR' AS estado, 'Rango de fechas inválido' AS mensaje;
    ELSE
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
    END IF;
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

    IF fechaInicio IS NULL OR fechaFin IS NULL OR fechaInicio > fechaFin THEN
        SELECT 'ERROR' AS estado, 'Rango de fechas inválido' AS mensaje;
    ELSE
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
        GROUP BY mp.Metodo_PagoID, mp.MetodoPago
        ORDER BY Total_Facturado DESC;
    END IF;
END //
DELIMITER ;

-- ----------------------------------------------------------------------------
-- Reporte 4: Ventas por día
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

    IF fechaInicio IS NULL OR fechaFin IS NULL OR fechaInicio > fechaFin THEN
        SELECT 'ERROR' AS estado, 'Rango de fechas inválido' AS mensaje;
    ELSE
        SELECT
            v.Fecha,
            COUNT(v.VentaID) AS Cantidad_Ventas,
            SUM(v.Monto_Total) AS Total_Dia
        FROM Venta v
        INNER JOIN Retiro r ON v.VentaID = r.VentaID
        WHERE v.Fecha BETWEEN fechaInicio AND fechaFin
          AND r.Estado_Retiro_ID <> 3
        GROUP BY v.Fecha
        ORDER BY v.Fecha ASC;
    END IF;
END //
DELIMITER ;

-- ----------------------------------------------------------------------------
-- Reporte 5: Estudiantes con mayor consumo (con su curso y su última compra)
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

    IF fechaInicio IS NULL OR fechaFin IS NULL OR fechaInicio > fechaFin THEN
        SELECT 'ERROR' AS estado, 'Rango de fechas inválido' AS mensaje;
    ELSE
        SELECT
            u.UserID,
            u.Apodo,
            e.Nombre_Estudiante,
            e.Apellido_Estudiante,
            CONCAT(cu.Anio, '° ', cu.Division) AS Curso,
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
        ORDER BY Total_Gastado DESC, u.UserID;
    END IF;
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

    IF fechaInicio IS NULL OR fechaFin IS NULL OR fechaInicio > fechaFin THEN
        SELECT 'ERROR' AS estado, 'Rango de fechas inválido' AS mensaje;
    ELSE
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
    END IF;
END //
DELIMITER ;

-- ----------------------------------------------------------------------------
-- Reporte 7: Pedidos por franja de retiro y estado (qué recreo se usa más
-- y cuántos pedidos se entregan, quedan pendientes o se cancelan).
-- Este reporte SÍ cuenta los cancelados, porque justamente los muestra.
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

    IF fechaInicio IS NULL OR fechaFin IS NULL OR fechaInicio > fechaFin THEN
        SELECT 'ERROR' AS estado, 'Rango de fechas inválido' AS mensaje;
    ELSE
        SELECT
            f.Nombre_Franja,
            f.Hora_Retiro,
            er.Estado,
            COUNT(r.RetiroID) AS Cantidad_Pedidos
        FROM Retiro r
        INNER JOIN Franja_Retiro f ON r.ID_Franja = f.ID_Franja
        INNER JOIN Estado_Retiro er ON r.Estado_Retiro_ID = er.Estado_Retiro_ID
        INNER JOIN Venta v ON r.VentaID = v.VentaID
        WHERE v.Fecha BETWEEN fechaInicio AND fechaFin
        GROUP BY f.ID_Franja, f.Nombre_Franja, f.Hora_Retiro, er.Estado_Retiro_ID, er.Estado
        ORDER BY f.Hora_Retiro, er.Estado;
    END IF;
END //
DELIMITER ;

-- ----------------------------------------------------------------------------
-- Reporte 8: Demanda del menú del día (cuántos se vendieron de cada menú
-- y cuántos quedaron sin vender)
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

    IF fechaInicio IS NULL OR fechaFin IS NULL OR fechaInicio > fechaFin THEN
        SELECT 'ERROR' AS estado, 'Rango de fechas inválido' AS mensaje;
    ELSE
        SELECT
            m.Menu_Del_DiaID,
            m.Plato_menu,
            m.Fecha_menu,
            SUM(c.Cantidad) AS Cantidad_Vendida,
            m.Stock_menu AS Quedaron_Sin_Vender,
            SUM(c.Cantidad * c.Precio_Unitario) AS Recaudacion_Menu
        FROM Menu_Del_Dia m
        INNER JOIN Carrito c ON m.Menu_Del_DiaID = c.Menu_Del_DiaID
        INNER JOIN Venta v ON c.VentaID = v.VentaID
        INNER JOIN Retiro r ON v.VentaID = r.VentaID
        WHERE m.Fecha_menu BETWEEN fechaInicio AND fechaFin
          AND r.Estado_Retiro_ID <> 3
        GROUP BY m.Menu_Del_DiaID, m.Plato_menu, m.Fecha_menu, m.Stock_menu
        ORDER BY Cantidad_Vendida DESC, m.Fecha_menu;
    END IF;
END //
DELIMITER ;


-- ############################################################################
-- 5.d  REPORTE DE AUDITORÍA
-- Parámetros: rango de fechas (obligatorio) + usuario y tipo de evento (opcionales).
-- Si p_UserID o p_Tipo_Evento_ID vienen en NULL, se muestran todos.
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

    IF fechaInicio IS NULL OR fechaFin IS NULL OR fechaInicio > fechaFin THEN
        SELECT 'ERROR' AS estado, 'Rango de fechas inválido' AS mensaje;
    ELSE
        SELECT
            l.Log_ID,
            l.Fecha,
            l.Hora,
            te.Nombre_Evento,
            l.UserID,
            u.Apodo,
            l.Detalle
        FROM Log_Auditoria l
        INNER JOIN Tipo_Evento te ON l.Tipo_Evento_ID = te.Tipo_Evento_ID
        LEFT JOIN Usuario u ON l.UserID = u.UserID
        WHERE l.Fecha BETWEEN fechaInicio AND fechaFin
          AND (p_UserID IS NULL OR l.UserID = p_UserID)
          AND (p_Tipo_Evento_ID IS NULL OR l.Tipo_Evento_ID = p_Tipo_Evento_ID)
        ORDER BY l.Fecha DESC, l.Hora DESC, l.Log_ID DESC;
    END IF;
END //
DELIMITER ;


-- ############################################################################
-- 5.e  REPORTES CON TUNING
-- Devuelven exactamente lo mismo que el original, pero con menos trabajo:
--  * El original une primero todas las tablas (Venta, Carrito/Retiro y las tablas
--    de nombres) y después agrupa por varias columnas de texto.
--  * El tuneado primero agrupa la tabla de movimientos por un solo ID numérico
--    (subconsulta) y recién después une ese resultado, que tiene pocas filas,
--    con las tablas de nombres. El GROUP BY es sobre un entero y el JOIN se
--    hace con una fila por producto / estudiante, no con una fila por venta.
--  * El filtro por fecha usa el índice idx_venta_fecha (creado en 03_ModeloSQL).
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

    IF fechaInicio IS NULL OR fechaFin IS NULL OR fechaInicio > fechaFin THEN
        SELECT 'ERROR' AS estado, 'Rango de fechas inválido' AS mensaje;
    ELSE
        SELECT
            p.ProductoID,
            p.Nombre_Producto,
            cat.Categoria,
            res.Total_Unidades,
            res.Total_Recaudado
        FROM (
            SELECT
                c.ProductoID,
                SUM(c.Cantidad) AS Total_Unidades,
                SUM(c.Cantidad * c.Precio_Unitario) AS Total_Recaudado
            FROM Venta v
            INNER JOIN Carrito c ON v.VentaID = c.VentaID
            INNER JOIN Retiro r ON v.VentaID = r.VentaID
            WHERE v.Fecha BETWEEN fechaInicio AND fechaFin
              AND r.Estado_Retiro_ID <> 3
              AND c.ProductoID IS NOT NULL
            GROUP BY c.ProductoID
        ) res
        INNER JOIN Producto p ON res.ProductoID = p.ProductoID
        INNER JOIN Categoria cat ON p.ID_Categoria = cat.ID_Categoria
        ORDER BY res.Total_Unidades DESC, p.ProductoID;
    END IF;
END //
DELIMITER ;

-- ----------------------------------------------------------------------------
-- Tuneado 2: Estudiantes con mayor consumo (original: Reporte 5)
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

    IF fechaInicio IS NULL OR fechaFin IS NULL OR fechaInicio > fechaFin THEN
        SELECT 'ERROR' AS estado, 'Rango de fechas inválido' AS mensaje;
    ELSE
        SELECT
            u.UserID,
            u.Apodo,
            e.Nombre_Estudiante,
            e.Apellido_Estudiante,
            CONCAT(cu.Anio, '° ', cu.Division) AS Curso,
            res.Compras_Realizadas,
            res.Total_Gastado,
            res.Ultima_Compra
        FROM (
            SELECT
                v.UserID,
                COUNT(v.VentaID) AS Compras_Realizadas,
                SUM(v.Monto_Total) AS Total_Gastado,
                MAX(v.Fecha) AS Ultima_Compra
            FROM Venta v
            INNER JOIN Retiro r ON v.VentaID = r.VentaID
            WHERE v.Fecha BETWEEN fechaInicio AND fechaFin
              AND r.Estado_Retiro_ID <> 3
            GROUP BY v.UserID
        ) res
        INNER JOIN schema_usuarios.Usuario u ON res.UserID = u.UserID
        INNER JOIN schema_usuarios.Estudiante e ON u.DNI = e.DNI
        INNER JOIN schema_usuarios.Curso cu ON e.ID_Curso = cu.ID_Curso
        ORDER BY res.Total_Gastado DESC, u.UserID;
    END IF;
END //
DELIMITER ;
