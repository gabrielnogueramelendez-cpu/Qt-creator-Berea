-- ============================================================================
-- PROYECTO INFORMÁTICO I - SECCIÓN BASES DE DATOS
-- El Buffet de Alejandro - Escuela Técnica N° 3 D.E. 9 "María Sánchez de Thompson"
-- PUNTO 3: IMPLEMENTACIÓN DEL MODELO (schemas, tablas, restricciones e índices)
--
-- Orden de ejecución: 03_ModeloSQL -> 04_DatosSQL -> 05_Backend
-- Se puede ejecutar todas las veces que haga falta: borra y vuelve a crear todo.
-- ============================================================================

SET NAMES utf8mb4;  -- para que las tildes y la ñ se guarden bien desde cualquier cliente

SET FOREIGN_KEY_CHECKS = 0;

DROP DATABASE IF EXISTS schema_comercial;
DROP DATABASE IF EXISTS schema_usuarios;
DROP DATABASE IF EXISTS schema_geografia;

SET FOREIGN_KEY_CHECKS = 1;

-- ============================================================================
-- SCHEMA 1: GEOGRAFÍA (ubicación de los colegios)
-- ============================================================================
CREATE DATABASE IF NOT EXISTS schema_geografia DEFAULT CHARACTER SET utf8mb4;
USE schema_geografia;

CREATE TABLE IF NOT EXISTS Pais (
    ID_Pais INT AUTO_INCREMENT PRIMARY KEY,
    Nombre_Pais VARCHAR(100) NOT NULL UNIQUE
);

CREATE TABLE IF NOT EXISTS Provincia (
    ID_Provincia INT AUTO_INCREMENT PRIMARY KEY,
    ID_Pais INT NOT NULL,
    Nombre_Provincia VARCHAR(100) NOT NULL,
    CONSTRAINT fk_provincia_pais FOREIGN KEY (ID_Pais)
        REFERENCES Pais(ID_Pais) ON DELETE RESTRICT ON UPDATE CASCADE
);

CREATE TABLE IF NOT EXISTS Ciudad (
    ID_Ciudad INT AUTO_INCREMENT PRIMARY KEY,
    ID_Provincia INT NOT NULL,
    Nombre_Ciudad VARCHAR(100) NOT NULL,
    CONSTRAINT fk_ciudad_provincia FOREIGN KEY (ID_Provincia)
        REFERENCES Provincia(ID_Provincia) ON DELETE RESTRICT ON UPDATE CASCADE
);

CREATE TABLE IF NOT EXISTS Region (
    ID_Region INT AUTO_INCREMENT PRIMARY KEY,
    ID_Ciudad INT NOT NULL,
    Nombre_Region VARCHAR(100) NOT NULL,
    CONSTRAINT fk_region_ciudad FOREIGN KEY (ID_Ciudad)
        REFERENCES Ciudad(ID_Ciudad) ON DELETE RESTRICT ON UPDATE CASCADE
);

-- Una región tiene muchos distritos escolares (M:1)
CREATE TABLE IF NOT EXISTS Distrito_Escolar (
    ID_DE INT AUTO_INCREMENT PRIMARY KEY,
    ID_Region INT NOT NULL,
    Jurisdiccion VARCHAR(100) NOT NULL,
    CONSTRAINT fk_distrito_region FOREIGN KEY (ID_Region)
        REFERENCES Region(ID_Region) ON DELETE RESTRICT ON UPDATE CASCADE
);

CREATE TABLE IF NOT EXISTS Colegio (
    ID_Colegio INT AUTO_INCREMENT PRIMARY KEY,
    ID_DE INT NOT NULL,
    Nombre_Colegio VARCHAR(150) NOT NULL,
    Direccion VARCHAR(200) NOT NULL,
    CONSTRAINT fk_colegio_distrito FOREIGN KEY (ID_DE)
        REFERENCES Distrito_Escolar(ID_DE) ON DELETE RESTRICT ON UPDATE CASCADE
);

-- ============================================================================
-- SCHEMA 2: USUARIOS (personas, cuentas y auditoría)
-- ============================================================================
CREATE DATABASE IF NOT EXISTS schema_usuarios DEFAULT CHARACTER SET utf8mb4;
USE schema_usuarios;

-- Tablas maestras
CREATE TABLE IF NOT EXISTS Curso (
    ID_Curso INT AUTO_INCREMENT PRIMARY KEY,
    Anio INT NOT NULL CHECK (Anio BETWEEN 1 AND 6),
    Division VARCHAR(10) NOT NULL,
    CONSTRAINT uq_curso UNIQUE (Anio, Division)
);

CREATE TABLE IF NOT EXISTS Puesto (
    ID_Puesto INT AUTO_INCREMENT PRIMARY KEY,
    Nombre_Puesto VARCHAR(60) NOT NULL UNIQUE
);

CREATE TABLE IF NOT EXISTS Tipo_Evento (
    Tipo_Evento_ID INT AUTO_INCREMENT PRIMARY KEY,
    Nombre_Evento VARCHAR(100) NOT NULL UNIQUE
);

-- Personas
CREATE TABLE IF NOT EXISTS Estudiante (
    DNI VARCHAR(20) PRIMARY KEY,
    CUIT VARCHAR(20) UNIQUE,
    Nombre_Estudiante VARCHAR(100) NOT NULL,
    Apellido_Estudiante VARCHAR(100) NOT NULL,
    Fecha_Nacimiento DATE NOT NULL,
    ID_Colegio INT NOT NULL,
    ID_Curso INT NOT NULL,
    CONSTRAINT fk_estudiante_colegio FOREIGN KEY (ID_Colegio)
        REFERENCES schema_geografia.Colegio(ID_Colegio) ON DELETE RESTRICT ON UPDATE CASCADE,
    CONSTRAINT fk_estudiante_curso FOREIGN KEY (ID_Curso)
        REFERENCES Curso(ID_Curso) ON DELETE RESTRICT ON UPDATE CASCADE
);

-- Cuenta para entrar al sistema.
-- La usa un estudiante (DNI cargado) o un empleado (Empleado.UserID apunta acá).
-- La contraseña se guarda cifrada con SHA2 (64 caracteres), nunca en texto plano.
CREATE TABLE IF NOT EXISTS Usuario (
    UserID INT AUTO_INCREMENT PRIMARY KEY,
    DNI VARCHAR(20) NULL UNIQUE,
    Apodo VARCHAR(50) NOT NULL UNIQUE,
    Contrasena CHAR(64) NOT NULL,
    CONSTRAINT fk_usuario_estudiante FOREIGN KEY (DNI)
        REFERENCES Estudiante(DNI) ON DELETE RESTRICT ON UPDATE CASCADE
);

CREATE TABLE IF NOT EXISTS Empleado (
    EmpleadoID INT AUTO_INCREMENT PRIMARY KEY,
    Nombre_Empleado VARCHAR(100) NOT NULL,
    Apellido_Empleado VARCHAR(100) NOT NULL,
    ID_Puesto INT NOT NULL,
    UserID INT NULL UNIQUE,
    CONSTRAINT fk_empleado_puesto FOREIGN KEY (ID_Puesto)
        REFERENCES Puesto(ID_Puesto) ON DELETE RESTRICT ON UPDATE CASCADE,
    CONSTRAINT fk_empleado_usuario FOREIGN KEY (UserID)
        REFERENCES Usuario(UserID) ON DELETE SET NULL ON UPDATE CASCADE
);

-- Atributos multivaluados del usuario
CREATE TABLE IF NOT EXISTS Email (
    Email_ID INT AUTO_INCREMENT PRIMARY KEY,
    UserID INT NOT NULL,
    Email VARCHAR(150) NOT NULL UNIQUE,
    CONSTRAINT fk_email_usuario FOREIGN KEY (UserID)
        REFERENCES Usuario(UserID) ON DELETE CASCADE ON UPDATE CASCADE
);

CREATE TABLE IF NOT EXISTS Telefono (
    Telefono_ID INT AUTO_INCREMENT PRIMARY KEY,
    UserID INT NOT NULL,
    Telefono VARCHAR(50) NOT NULL,
    CONSTRAINT fk_telefono_usuario FOREIGN KEY (UserID)
        REFERENCES Usuario(UserID) ON DELETE CASCADE ON UPDATE CASCADE
);

-- Modelo de logs: lo completan solamente los Stored Procedures (consigna 4.c)
CREATE TABLE IF NOT EXISTS Log_Auditoria (
    Log_ID INT AUTO_INCREMENT PRIMARY KEY,
    UserID INT NULL,
    Tipo_Evento_ID INT NOT NULL,
    Fecha DATE NOT NULL,
    Hora TIME NOT NULL,
    Detalle VARCHAR(255) NULL,
    CONSTRAINT fk_log_usuario FOREIGN KEY (UserID)
        REFERENCES Usuario(UserID) ON DELETE SET NULL ON UPDATE CASCADE,
    CONSTRAINT fk_log_tipoevento FOREIGN KEY (Tipo_Evento_ID)
        REFERENCES Tipo_Evento(Tipo_Evento_ID) ON DELETE RESTRICT ON UPDATE CASCADE
);

-- Índice para el reporte de auditoría (siempre filtra por rango de fechas)
CREATE INDEX idx_log_fecha ON Log_Auditoria(Fecha);

-- ============================================================================
-- SCHEMA 3: COMERCIAL (productos, ventas y retiros)
-- ============================================================================
CREATE DATABASE IF NOT EXISTS schema_comercial DEFAULT CHARACTER SET utf8mb4;
USE schema_comercial;

-- Tablas maestras
CREATE TABLE IF NOT EXISTS Categoria (
    ID_Categoria INT AUTO_INCREMENT PRIMARY KEY,
    Categoria VARCHAR(50) NOT NULL UNIQUE
);

CREATE TABLE IF NOT EXISTS Metodo_Pago (
    Metodo_PagoID INT AUTO_INCREMENT PRIMARY KEY,
    MetodoPago VARCHAR(50) NOT NULL UNIQUE
);

CREATE TABLE IF NOT EXISTS Estado_Retiro (
    Estado_Retiro_ID INT AUTO_INCREMENT PRIMARY KEY,
    Estado VARCHAR(50) NOT NULL UNIQUE
);

-- Recreos / horarios en los que se puede retirar un pedido
CREATE TABLE IF NOT EXISTS Franja_Retiro (
    ID_Franja INT AUTO_INCREMENT PRIMARY KEY,
    Nombre_Franja VARCHAR(100) NOT NULL UNIQUE,
    Hora_Retiro TIME NOT NULL
);

-- Productos y menú
CREATE TABLE IF NOT EXISTS Producto (
    ProductoID INT AUTO_INCREMENT PRIMARY KEY,
    ID_Categoria INT NOT NULL,
    Nombre_Producto VARCHAR(100) NOT NULL,
    Descripcion_Producto TEXT,
    Stock_Producto INT NOT NULL DEFAULT 0 CHECK (Stock_Producto >= 0),
    Precio_Producto DECIMAL(10,2) NOT NULL CHECK (Precio_Producto >= 0),
    Activo BOOLEAN NOT NULL DEFAULT TRUE,
    CONSTRAINT fk_producto_categoria FOREIGN KEY (ID_Categoria)
        REFERENCES Categoria(ID_Categoria) ON DELETE RESTRICT ON UPDATE CASCADE
);

CREATE TABLE IF NOT EXISTS Menu_Del_Dia (
    Menu_Del_DiaID INT AUTO_INCREMENT PRIMARY KEY,
    ID_Categoria INT NOT NULL,
    Plato_menu VARCHAR(100) NOT NULL,
    Precio_menu DECIMAL(10,2) NOT NULL CHECK (Precio_menu >= 0),
    Stock_menu INT NOT NULL DEFAULT 0 CHECK (Stock_menu >= 0),
    Fecha_menu DATE NOT NULL,
    CONSTRAINT fk_menudia_categoria FOREIGN KEY (ID_Categoria)
        REFERENCES Categoria(ID_Categoria) ON DELETE RESTRICT ON UPDATE CASCADE
);

-- Relación N:M "un menú incluye productos" (ej.: bebida o postre del menú)
CREATE TABLE IF NOT EXISTS Detalle_Menu_Dia (
    Detalle_Menu_DiaID INT AUTO_INCREMENT PRIMARY KEY,
    Menu_Del_DiaID INT NOT NULL,
    ProductoID INT NOT NULL,
    CONSTRAINT uq_detalle_menu UNIQUE (Menu_Del_DiaID, ProductoID),
    CONSTRAINT fk_detallemenu_menudia FOREIGN KEY (Menu_Del_DiaID)
        REFERENCES Menu_Del_Dia(Menu_Del_DiaID) ON DELETE CASCADE ON UPDATE CASCADE,
    CONSTRAINT fk_detallemenu_producto FOREIGN KEY (ProductoID)
        REFERENCES Producto(ProductoID) ON DELETE RESTRICT ON UPDATE CASCADE
);

-- Ventas (tabla principal del sistema)
-- EmpleadoID queda NULL cuando el alumno hace el pedido desde la app, sin cajero.
CREATE TABLE IF NOT EXISTS Venta (
    VentaID INT AUTO_INCREMENT PRIMARY KEY,
    EmpleadoID INT NULL,
    UserID INT NOT NULL,
    Metodo_PagoID INT NOT NULL,
    Fecha_Hora DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
    Monto_Total DECIMAL(10,2) NOT NULL DEFAULT 0 CHECK (Monto_Total >= 0),
    CONSTRAINT fk_venta_empleado FOREIGN KEY (EmpleadoID)
        REFERENCES schema_usuarios.Empleado(EmpleadoID) ON DELETE SET NULL ON UPDATE CASCADE,
    CONSTRAINT fk_venta_usuario FOREIGN KEY (UserID)
        REFERENCES schema_usuarios.Usuario(UserID) ON DELETE RESTRICT ON UPDATE CASCADE,
    CONSTRAINT fk_venta_metodopago FOREIGN KEY (Metodo_PagoID)
        REFERENCES Metodo_Pago(Metodo_PagoID) ON DELETE RESTRICT ON UPDATE CASCADE
);

-- Índice usado por los reportes "_tuneado" (filtran por rango de Fecha_Hora)
CREATE INDEX idx_venta_fecha ON Venta(Fecha_Hora);

-- Renglones de la venta. Cada renglón es un producto O un menú del día (nunca los dos);
-- eso lo controlan los SPs sp_agregar_producto_carrito y sp_agregar_menu_carrito.
CREATE TABLE IF NOT EXISTS Carrito (
    CarritoID INT AUTO_INCREMENT PRIMARY KEY,
    VentaID INT NOT NULL,
    ProductoID INT NULL,
    Menu_Del_DiaID INT NULL,
    Cantidad INT NOT NULL CHECK (Cantidad > 0),
    Precio_Unitario DECIMAL(10,2) NOT NULL CHECK (Precio_Unitario >= 0),
    CONSTRAINT fk_carrito_venta FOREIGN KEY (VentaID)
        REFERENCES Venta(VentaID) ON DELETE CASCADE ON UPDATE CASCADE,
    CONSTRAINT fk_carrito_producto FOREIGN KEY (ProductoID)
        REFERENCES Producto(ProductoID) ON DELETE RESTRICT ON UPDATE CASCADE,
    CONSTRAINT fk_carrito_menudia FOREIGN KEY (Menu_Del_DiaID)
        REFERENCES Menu_Del_Dia(Menu_Del_DiaID) ON DELETE RESTRICT ON UPDATE CASCADE
);

-- Una venta tiene un único retiro (1:1)
CREATE TABLE IF NOT EXISTS Retiro (
    RetiroID INT AUTO_INCREMENT PRIMARY KEY,
    VentaID INT NOT NULL UNIQUE,
    Estado_Retiro_ID INT NOT NULL,
    ID_Franja INT NOT NULL,
    Codigo_Retiro CHAR(5) NOT NULL UNIQUE,
    CONSTRAINT fk_retiro_venta FOREIGN KEY (VentaID)
        REFERENCES Venta(VentaID) ON DELETE CASCADE ON UPDATE CASCADE,
    CONSTRAINT fk_retiro_estado FOREIGN KEY (Estado_Retiro_ID)
        REFERENCES Estado_Retiro(Estado_Retiro_ID) ON DELETE RESTRICT ON UPDATE CASCADE,
    CONSTRAINT fk_retiro_franja FOREIGN KEY (ID_Franja)
        REFERENCES Franja_Retiro(ID_Franja) ON DELETE RESTRICT ON UPDATE CASCADE
);
