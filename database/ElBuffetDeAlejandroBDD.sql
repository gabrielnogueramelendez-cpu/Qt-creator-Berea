SET FOREIGN_KEY_CHECKS = 0;

USE schema_geografia;
DROP TABLE IF EXISTS Colegio;
DROP TABLE IF EXISTS Distrito_Escolar;
DROP TABLE IF EXISTS Region;
DROP TABLE IF EXISTS Ciudad;
DROP TABLE IF EXISTS Provincia;
DROP TABLE IF EXISTS Pais;

USE schema_usuarios;
DROP TABLE IF EXISTS Log_Auditoria;
DROP TABLE IF EXISTS Telefono;
DROP TABLE IF EXISTS Email;
DROP TABLE IF EXISTS Usuario;
DROP TABLE IF EXISTS Tipo_Evento;
DROP TABLE IF EXISTS Empleado;
DROP TABLE IF EXISTS Estudiante;

USE schema_comercial;
DROP TABLE IF EXISTS Retiro;
DROP TABLE IF EXISTS Carrito;
DROP TABLE IF EXISTS Detalle_Menu_Dia;
DROP TABLE IF EXISTS Venta;
DROP TABLE IF EXISTS Menu_Del_Dia;
DROP TABLE IF EXISTS Producto;
DROP TABLE IF EXISTS Estado_Retiro;
DROP TABLE IF EXISTS Metodo_Pago;
DROP TABLE IF EXISTS Categoria;


-- ============================================================================
-- BASE DE DATOS 1: SCHEMA GEOGRAFIA
-- ============================================================================
CREATE DATABASE IF NOT EXISTS schema_geografia DEFAULT CHARACTER SET utf8mb4;
USE schema_geografia;

-- 1. TABLAS PADRE
CREATE TABLE IF NOT EXISTS Pais (
    ID_Pais INT AUTO_INCREMENT PRIMARY KEY,
    Nombre_Pais VARCHAR(100) NOT NULL UNIQUE
);

-- 2. TABLAS HIJAS
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
-- BASE DE DATOS 2: SCHEMA USUARIOS
-- ============================================================================
CREATE DATABASE IF NOT EXISTS schema_usuarios DEFAULT CHARACTER SET utf8mb4;
USE schema_usuarios;

-- 1. TABLAS PADRE
CREATE TABLE IF NOT EXISTS Estudiante (
    DNI VARCHAR(20) PRIMARY KEY,
    CUIT VARCHAR(20) UNIQUE,
    Nombre_Estudiante VARCHAR(100) NOT NULL,
    Apellido_Estudiante VARCHAR(100) NOT NULL,
    Fecha_Nacimiento DATE NOT NULL,
    ID_Colegio INT NOT NULL,
    CONSTRAINT fk_estudiante_colegio FOREIGN KEY (ID_Colegio) 
        REFERENCES schema_geografia.Colegio(ID_Colegio) ON DELETE RESTRICT ON UPDATE CASCADE
);

CREATE TABLE IF NOT EXISTS Empleado (
    EmpleadoID INT AUTO_INCREMENT PRIMARY KEY,
    Nombre_Empleado VARCHAR(100) NOT NULL,
    Apellido_Empleado VARCHAR(100) NOT NULL,
    Puesto_Empleado VARCHAR(50) NOT NULL
);

CREATE TABLE IF NOT EXISTS Tipo_Evento (
    Tipo_Evento_ID INT AUTO_INCREMENT PRIMARY KEY,
    Nombre_Evento VARCHAR(100) NOT NULL UNIQUE
);

-- 2. TABLAS HIJAS
CREATE TABLE IF NOT EXISTS Usuario (
    UserID INT AUTO_INCREMENT PRIMARY KEY,
    DNI VARCHAR(20) NOT NULL,
    Apodo VARCHAR(50),
    CONSTRAINT fk_usuario_estudiante FOREIGN KEY (DNI) 
        REFERENCES Estudiante(DNI) ON DELETE RESTRICT ON UPDATE CASCADE
);

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

CREATE TABLE IF NOT EXISTS Log_Auditoria (
    Log_ID INT AUTO_INCREMENT PRIMARY KEY,
    UserID INT NULL,
    Tipo_Evento_ID INT NOT NULL,
    Fecha DATE NOT NULL,
    Hora TIME NOT NULL,
    CONSTRAINT fk_log_usuario FOREIGN KEY (UserID) 
        REFERENCES Usuario(UserID) ON DELETE SET NULL ON UPDATE CASCADE,
    CONSTRAINT fk_log_tipoevento FOREIGN KEY (Tipo_Evento_ID) 
        REFERENCES Tipo_Evento(Tipo_Evento_ID) ON DELETE RESTRICT ON UPDATE CASCADE
);

-- ============================================================================
-- BASE DE DATOS 3: SCHEMA COMERCIAL
-- ============================================================================
CREATE DATABASE IF NOT EXISTS schema_comercial DEFAULT CHARACTER SET utf8mb4;
USE schema_comercial;

-- 1. TABLAS PADRE
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

-- 2. TABLAS HIJAS
CREATE TABLE IF NOT EXISTS Producto (
    ProductoID INT AUTO_INCREMENT PRIMARY KEY,
    ID_Categoria INT NOT NULL,
    Nombre_Producto VARCHAR(100) NOT NULL,
    Descripcion_Producto TEXT,
    Stock_Producto INT NOT NULL DEFAULT 0 CHECK (Stock_Producto >= 0),
    Precio_Producto DECIMAL(10,2) NOT NULL CHECK (Precio_Producto >= 0),
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

CREATE TABLE IF NOT EXISTS Venta (
    VentaID INT AUTO_INCREMENT PRIMARY KEY,
    EmpleadoID INT NULL,
    UserID INT NOT NULL,
    Metodo_PagoID INT NOT NULL,
    Fecha_Hora DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
    Monto_Total DECIMAL(10,2) NOT NULL CHECK (Monto_Total >= 0),
    CONSTRAINT fk_venta_empleado FOREIGN KEY (EmpleadoID) 
        REFERENCES schema_usuarios.Empleado(EmpleadoID) ON DELETE SET NULL ON UPDATE CASCADE,
    CONSTRAINT fk_venta_usuario FOREIGN KEY (UserID) 
        REFERENCES schema_usuarios.Usuario(UserID) ON DELETE RESTRICT ON UPDATE CASCADE,
    CONSTRAINT fk_venta_metodopago FOREIGN KEY (Metodo_PagoID) 
        REFERENCES Metodo_Pago(Metodo_PagoID) ON DELETE RESTRICT ON UPDATE CASCADE
);

CREATE TABLE IF NOT EXISTS Detalle_Menu_Dia (
    Detalle_Menu_Dia INT AUTO_INCREMENT PRIMARY KEY,
    Menu_Del_DiaID INT NOT NULL,
    ProductoID INT NOT NULL,
    CONSTRAINT fk_detallemenu_menudia FOREIGN KEY (Menu_Del_DiaID) 
        REFERENCES Menu_Del_Dia(Menu_Del_DiaID) ON DELETE CASCADE ON UPDATE CASCADE,
    CONSTRAINT fk_detallemenu_producto FOREIGN KEY (ProductoID) 
        REFERENCES Producto(ProductoID) ON DELETE RESTRICT ON UPDATE CASCADE
);

CREATE TABLE IF NOT EXISTS Carrito (
    CarritoID INT AUTO_INCREMENT PRIMARY KEY,
    VentaID INT NOT NULL,
    ProductoID INT NULL,
    Menu_Del_DiaID INT NULL,
    Cantidad INT NOT NULL CHECK (Cantidad > 0),
    precio_Unitario DECIMAL(10,2) NOT NULL CHECK (precio_Unitario >= 0),
    CONSTRAINT fk_carrito_venta FOREIGN KEY (VentaID) 
        REFERENCES Venta(VentaID) ON DELETE CASCADE ON UPDATE CASCADE,
    CONSTRAINT fk_carrito_producto FOREIGN KEY (ProductoID) 
        REFERENCES Producto(ProductoID) ON DELETE RESTRICT ON UPDATE CASCADE,
    CONSTRAINT fk_carrito_menudia FOREIGN KEY (Menu_Del_DiaID) 
        REFERENCES Menu_Del_Dia(Menu_Del_DiaID) ON DELETE RESTRICT ON UPDATE CASCADE
);

CREATE TABLE IF NOT EXISTS Retiro (
    RetiroID INT AUTO_INCREMENT PRIMARY KEY,
    VentaID INT NOT NULL UNIQUE,
    EstadoID INT NOT NULL,
    Tiempo_Retiro VARCHAR(100) NOT NULL,
    Codigo_Retiro VARCHAR(10) NOT NULL UNIQUE,
    CONSTRAINT fk_retiro_venta FOREIGN KEY (VentaID) 
        REFERENCES Venta(VentaID) ON DELETE CASCADE ON UPDATE CASCADE,
    CONSTRAINT fk_retiro_estado FOREIGN KEY (EstadoID) 
        REFERENCES Estado_Retiro(Estado_Retiro_ID) ON DELETE RESTRICT ON UPDATE CASCADE
);

SET FOREIGN_KEY_CHECKS = 0;

-- ============================================================================
-- 1. SCHEMA GEOGRAFIA
-- ============================================================================

USE schema_geografia;

-- Tabla Pais (20 registros)
INSERT INTO Pais (Nombre_Pais) VALUES ('Argentina');
INSERT INTO Pais (Nombre_Pais) VALUES ('Uruguay');
INSERT INTO Pais (Nombre_Pais) VALUES ('Chile');
INSERT INTO Pais (Nombre_Pais) VALUES ('Paraguay');
INSERT INTO Pais (Nombre_Pais) VALUES ('Brasil');
INSERT INTO Pais (Nombre_Pais) VALUES ('Bolivia');
INSERT INTO Pais (Nombre_Pais) VALUES ('Perú');
INSERT INTO Pais (Nombre_Pais) VALUES ('Colombia');
INSERT INTO Pais (Nombre_Pais) VALUES ('Ecuador');
INSERT INTO Pais (Nombre_Pais) VALUES ('Venezuela');
INSERT INTO Pais (Nombre_Pais) VALUES ('México');
INSERT INTO Pais (Nombre_Pais) VALUES ('España');
INSERT INTO Pais (Nombre_Pais) VALUES ('Italia');
INSERT INTO Pais (Nombre_Pais) VALUES ('Francia');
INSERT INTO Pais (Nombre_Pais) VALUES ('Alemania');
INSERT INTO Pais (Nombre_Pais) VALUES ('Inglaterra');
INSERT INTO Pais (Nombre_Pais) VALUES ('Portugal');
INSERT INTO Pais (Nombre_Pais) VALUES ('Canadá');
INSERT INTO Pais (Nombre_Pais) VALUES ('Estados Unidos');
INSERT INTO Pais (Nombre_Pais) VALUES ('Japón');

-- Tabla Provincia (20 registros)
INSERT INTO Provincia (ID_Pais, Nombre_Provincia) VALUES (1, 'Buenos Aires');
INSERT INTO Provincia (ID_Pais, Nombre_Provincia) VALUES (1, 'Catamarca');
INSERT INTO Provincia (ID_Pais, Nombre_Provincia) VALUES (1, 'Chaco');
INSERT INTO Provincia (ID_Pais, Nombre_Provincia) VALUES (1, 'Chubut');
INSERT INTO Provincia (ID_Pais, Nombre_Provincia) VALUES (1, 'Córdoba');
INSERT INTO Provincia (ID_Pais, Nombre_Provincia) VALUES (1, 'Corrientes');
INSERT INTO Provincia (ID_Pais, Nombre_Provincia) VALUES (1, 'Entre Ríos');
INSERT INTO Provincia (ID_Pais, Nombre_Provincia) VALUES (1, 'Formosa');
INSERT INTO Provincia (ID_Pais, Nombre_Provincia) VALUES (1, 'Jujuy');
INSERT INTO Provincia (ID_Pais, Nombre_Provincia) VALUES (1, 'La Pampa');
INSERT INTO Provincia (ID_Pais, Nombre_Provincia) VALUES (1, 'La Rioja');
INSERT INTO Provincia (ID_Pais, Nombre_Provincia) VALUES (1, 'Mendoza');
INSERT INTO Provincia (ID_Pais, Nombre_Provincia) VALUES (1, 'Misiones');
INSERT INTO Provincia (ID_Pais, Nombre_Provincia) VALUES (1, 'Neuquén');
INSERT INTO Provincia (ID_Pais, Nombre_Provincia) VALUES (1, 'Río Negro');
INSERT INTO Provincia (ID_Pais, Nombre_Provincia) VALUES (1, 'Salta');
INSERT INTO Provincia (ID_Pais, Nombre_Provincia) VALUES (1, 'San Juan');
INSERT INTO Provincia (ID_Pais, Nombre_Provincia) VALUES (1, 'San Luis');
INSERT INTO Provincia (ID_Pais, Nombre_Provincia) VALUES (1, 'Santa Cruz');
INSERT INTO Provincia (ID_Pais, Nombre_Provincia) VALUES (1, 'Santa Fe');

-- Tabla Ciudad (20 registros)
INSERT INTO Ciudad (ID_Provincia, Nombre_Ciudad) VALUES (1, 'CABA');
INSERT INTO Ciudad (ID_Provincia, Nombre_Ciudad) VALUES (2, 'Avellaneda');
INSERT INTO Ciudad (ID_Provincia, Nombre_Ciudad) VALUES (3, 'La Plata');
INSERT INTO Ciudad (ID_Provincia, Nombre_Ciudad) VALUES (4, 'Quilmes');
INSERT INTO Ciudad (ID_Provincia, Nombre_Ciudad) VALUES (5, 'Lanús');
INSERT INTO Ciudad (ID_Provincia, Nombre_Ciudad) VALUES (6, 'Lomas de Zamora');
INSERT INTO Ciudad (ID_Provincia, Nombre_Ciudad) VALUES (7, 'San Isidro');
INSERT INTO Ciudad (ID_Provincia, Nombre_Ciudad) VALUES (8, 'Vicente López');
INSERT INTO Ciudad (ID_Provincia, Nombre_Ciudad) VALUES (9, 'Tigre');
INSERT INTO Ciudad (ID_Provincia, Nombre_Ciudad) VALUES (10, 'Morón');
INSERT INTO Ciudad (ID_Provincia, Nombre_Ciudad) VALUES (11, 'San Justo');
INSERT INTO Ciudad (ID_Provincia, Nombre_Ciudad) VALUES (12, 'Merlo');
INSERT INTO Ciudad (ID_Provincia, Nombre_Ciudad) VALUES (13, 'Moreno');
INSERT INTO Ciudad (ID_Provincia, Nombre_Ciudad) VALUES (14, 'Pilar');
INSERT INTO Ciudad (ID_Provincia, Nombre_Ciudad) VALUES (15, 'Escobar');
INSERT INTO Ciudad (ID_Provincia, Nombre_Ciudad) VALUES (16, 'Bahía Blanca');
INSERT INTO Ciudad (ID_Provincia, Nombre_Ciudad) VALUES (17, 'Mar del Plata');
INSERT INTO Ciudad (ID_Provincia, Nombre_Ciudad) VALUES (18, 'Tandil');
INSERT INTO Ciudad (ID_Provincia, Nombre_Ciudad) VALUES (19, 'Oliva');
INSERT INTO Ciudad (ID_Provincia, Nombre_Ciudad) VALUES (20, 'Rosario');

-- Tabla Region (20 registros)
INSERT INTO Region (ID_Ciudad, Nombre_Region) VALUES (1, 'Región Urbana 1');
INSERT INTO Region (ID_Ciudad, Nombre_Region) VALUES (2, 'Región Urbana 2');
INSERT INTO Region (ID_Ciudad, Nombre_Region) VALUES (3, 'Región Urbana 3');
INSERT INTO Region (ID_Ciudad, Nombre_Region) VALUES (4, 'Región Urbana 4');
INSERT INTO Region (ID_Ciudad, Nombre_Region) VALUES (5, 'Región Urbana 5');
INSERT INTO Region (ID_Ciudad, Nombre_Region) VALUES (6, 'Región Urbana 6');
INSERT INTO Region (ID_Ciudad, Nombre_Region) VALUES (7, 'Región Urbana 7');
INSERT INTO Region (ID_Ciudad, Nombre_Region) VALUES (8, 'Región Urbana 8');
INSERT INTO Region (ID_Ciudad, Nombre_Region) VALUES (9, 'Región Urbana 9');
INSERT INTO Region (ID_Ciudad, Nombre_Region) VALUES (10, 'Región Urbana 10');
INSERT INTO Region (ID_Ciudad, Nombre_Region) VALUES (11, 'Región Urbana 11');
INSERT INTO Region (ID_Ciudad, Nombre_Region) VALUES (12, 'Región Urbana 12');
INSERT INTO Region (ID_Ciudad, Nombre_Region) VALUES (13, 'Región Urbana 13');
INSERT INTO Region (ID_Ciudad, Nombre_Region) VALUES (14, 'Región Urbana 14');
INSERT INTO Region (ID_Ciudad, Nombre_Region) VALUES (15, 'Región Urbana 15');
INSERT INTO Region (ID_Ciudad, Nombre_Region) VALUES (16, 'Región Urbana 16');
INSERT INTO Region (ID_Ciudad, Nombre_Region) VALUES (17, 'Región Urbana 17');
INSERT INTO Region (ID_Ciudad, Nombre_Region) VALUES (18, 'Región Urbana 18');
INSERT INTO Region (ID_Ciudad, Nombre_Region) VALUES (19, 'Región Urbana 19');
INSERT INTO Region (ID_Ciudad, Nombre_Region) VALUES (20, 'Región Urbana 20');

-- Tabla Distrito_Escolar (20 registros)
INSERT INTO Distrito_Escolar (ID_Region, Jurisdiccion) VALUES (1, 'Distrito Escolar DE-1');
INSERT INTO Distrito_Escolar (ID_Region, Jurisdiccion) VALUES (2, 'Distrito Escolar DE-2');
INSERT INTO Distrito_Escolar (ID_Region, Jurisdiccion) VALUES (3, 'Distrito Escolar DE-3');
INSERT INTO Distrito_Escolar (ID_Region, Jurisdiccion) VALUES (4, 'Distrito Escolar DE-4');
INSERT INTO Distrito_Escolar (ID_Region, Jurisdiccion) VALUES (5, 'Distrito Escolar DE-5');
INSERT INTO Distrito_Escolar (ID_Region, Jurisdiccion) VALUES (6, 'Distrito Escolar DE-6');
INSERT INTO Distrito_Escolar (ID_Region, Jurisdiccion) VALUES (7, 'Distrito Escolar DE-7');
INSERT INTO Distrito_Escolar (ID_Region, Jurisdiccion) VALUES (8, 'Distrito Escolar DE-8');
INSERT INTO Distrito_Escolar (ID_Region, Jurisdiccion) VALUES (9, 'Distrito Escolar DE-9');
INSERT INTO Distrito_Escolar (ID_Region, Jurisdiccion) VALUES (10, 'Distrito Escolar DE-10');
INSERT INTO Distrito_Escolar (ID_Region, Jurisdiccion) VALUES (11, 'Distrito Escolar DE-11');
INSERT INTO Distrito_Escolar (ID_Region, Jurisdiccion) VALUES (12, 'Distrito Escolar DE-12');
INSERT INTO Distrito_Escolar (ID_Region, Jurisdiccion) VALUES (13, 'Distrito Escolar DE-13');
INSERT INTO Distrito_Escolar (ID_Region, Jurisdiccion) VALUES (14, 'Distrito Escolar DE-14');
INSERT INTO Distrito_Escolar (ID_Region, Jurisdiccion) VALUES (15, 'Distrito Escolar DE-15');
INSERT INTO Distrito_Escolar (ID_Region, Jurisdiccion) VALUES (16, 'Distrito Escolar DE-16');
INSERT INTO Distrito_Escolar (ID_Region, Jurisdiccion) VALUES (17, 'Distrito Escolar DE-17');
INSERT INTO Distrito_Escolar (ID_Region, Jurisdiccion) VALUES (18, 'Distrito Escolar DE-18');
INSERT INTO Distrito_Escolar (ID_Region, Jurisdiccion) VALUES (19, 'Distrito Escolar DE-19');
INSERT INTO Distrito_Escolar (ID_Region, Jurisdiccion) VALUES (20, 'Distrito Escolar DE-20');

-- Tabla Colegio (20 registros)
INSERT INTO Colegio (ID_DE, Nombre_Colegio, Direccion) VALUES (1, 'Instituto Educativo Berea Nro 1', 'Av. San Martín 1050');
INSERT INTO Colegio (ID_DE, Nombre_Colegio, Direccion) VALUES (2, 'Instituto Educativo Berea Nro 2', 'Av. San Martín 1100');
INSERT INTO Colegio (ID_DE, Nombre_Colegio, Direccion) VALUES (3, 'Instituto Educativo Berea Nro 3', 'Av. San Martín 1150');
INSERT INTO Colegio (ID_DE, Nombre_Colegio, Direccion) VALUES (4, 'Instituto Educativo Berea Nro 4', 'Av. San Martín 1200');
INSERT INTO Colegio (ID_DE, Nombre_Colegio, Direccion) VALUES (5, 'Instituto Educativo Berea Nro 5', 'Av. San Martín 1250');
INSERT INTO Colegio (ID_DE, Nombre_Colegio, Direccion) VALUES (6, 'Instituto Educativo Berea Nro 6', 'Av. San Martín 1300');
INSERT INTO Colegio (ID_DE, Nombre_Colegio, Direccion) VALUES (7, 'Instituto Educativo Berea Nro 7', 'Av. San Martín 1350');
INSERT INTO Colegio (ID_DE, Nombre_Colegio, Direccion) VALUES (8, 'Instituto Educativo Berea Nro 8', 'Av. San Martín 1400');
INSERT INTO Colegio (ID_DE, Nombre_Colegio, Direccion) VALUES (9, 'Instituto Educativo Berea Nro 9', 'Av. San Martín 1450');
INSERT INTO Colegio (ID_DE, Nombre_Colegio, Direccion) VALUES (10, 'Instituto Educativo Berea Nro 10', 'Av. San Martín 1500');
INSERT INTO Colegio (ID_DE, Nombre_Colegio, Direccion) VALUES (11, 'Instituto Educativo Berea Nro 11', 'Av. San Martín 1550');
INSERT INTO Colegio (ID_DE, Nombre_Colegio, Direccion) VALUES (12, 'Instituto Educativo Berea Nro 12', 'Av. San Martín 1600');
INSERT INTO Colegio (ID_DE, Nombre_Colegio, Direccion) VALUES (13, 'Instituto Educativo Berea Nro 13', 'Av. San Martín 1650');
INSERT INTO Colegio (ID_DE, Nombre_Colegio, Direccion) VALUES (14, 'Instituto Educativo Berea Nro 14', 'Av. San Martín 1700');
INSERT INTO Colegio (ID_DE, Nombre_Colegio, Direccion) VALUES (15, 'Instituto Educativo Berea Nro 15', 'Av. San Martín 1750');
INSERT INTO Colegio (ID_DE, Nombre_Colegio, Direccion) VALUES (16, 'Instituto Educativo Berea Nro 16', 'Av. San Martín 1800');
INSERT INTO Colegio (ID_DE, Nombre_Colegio, Direccion) VALUES (17, 'Instituto Educativo Berea Nro 17', 'Av. San Martín 1850');
INSERT INTO Colegio (ID_DE, Nombre_Colegio, Direccion) VALUES (18, 'Instituto Educativo Berea Nro 18', 'Av. San Martín 1900');
INSERT INTO Colegio (ID_DE, Nombre_Colegio, Direccion) VALUES (19, 'Instituto Educativo Berea Nro 19', 'Av. San Martín 1950');
INSERT INTO Colegio (ID_DE, Nombre_Colegio, Direccion) VALUES (20, 'Instituto Educativo Berea Nro 20', 'Av. San Martín 2000');

-- ============================================================================
-- 2. SCHEMA USUARIOS
-- ============================================================================

USE schema_usuarios;

-- Tabla Estudiante (20 registros - PK DNI no es auto-increment)
INSERT INTO Estudiante (DNI, CUIT, Nombre_Estudiante, Apellido_Estudiante, Fecha_Nacimiento, ID_Colegio) VALUES ('44000001', '20440000017', 'Alejandro', 'Gómez', '2005-02-02', 1);
INSERT INTO Estudiante (DNI, CUIT, Nombre_Estudiante, Apellido_Estudiante, Fecha_Nacimiento, ID_Colegio) VALUES ('44000002', '20440000027', 'Sofía', 'Martínez', '2005-03-03', 2);
INSERT INTO Estudiante (DNI, CUIT, Nombre_Estudiante, Apellido_Estudiante, Fecha_Nacimiento, ID_Colegio) VALUES ('44000003', '20440000037', 'Lucas', 'Rodríguez', '2005-04-04', 3);
INSERT INTO Estudiante (DNI, CUIT, Nombre_Estudiante, Apellido_Estudiante, Fecha_Nacimiento, ID_Colegio) VALUES ('44000004', '20440000047', 'Valentina', 'López', '2005-05-05', 4);
INSERT INTO Estudiante (DNI, CUIT, Nombre_Estudiante, Apellido_Estudiante, Fecha_Nacimiento, ID_Colegio) VALUES ('44000005', '20440000057', 'Mateo', 'Pérez', '2005-06-06', 5);
INSERT INTO Estudiante (DNI, CUIT, Nombre_Estudiante, Apellido_Estudiante, Fecha_Nacimiento, ID_Colegio) VALUES ('44000006', '20440000067', 'Camila', 'González', '2005-07-07', 6);
INSERT INTO Estudiante (DNI, CUIT, Nombre_Estudiante, Apellido_Estudiante, Fecha_Nacimiento, ID_Colegio) VALUES ('44000007', '20440000077', 'Joaquín', 'Sánchez', '2005-08-08', 7);
INSERT INTO Estudiante (DNI, CUIT, Nombre_Estudiante, Apellido_Estudiante, Fecha_Nacimiento, ID_Colegio) VALUES ('44000008', '20440000087', 'Martina', 'Romero', '2005-09-09', 8);
INSERT INTO Estudiante (DNI, CUIT, Nombre_Estudiante, Apellido_Estudiante, Fecha_Nacimiento, ID_Colegio) VALUES ('44000009', '20440000097', 'Nicolás', 'Sosa', '2005-10-10', 9);
INSERT INTO Estudiante (DNI, CUIT, Nombre_Estudiante, Apellido_Estudiante, Fecha_Nacimiento, ID_Colegio) VALUES ('44000010', '20440000107', 'Lucía', 'Torres', '2005-11-11', 10);
INSERT INTO Estudiante (DNI, CUIT, Nombre_Estudiante, Apellido_Estudiante, Fecha_Nacimiento, ID_Colegio) VALUES ('44000011', '20440000117', 'Gabriel', 'Álvarez', '2005-12-12', 11);
INSERT INTO Estudiante (DNI, CUIT, Nombre_Estudiante, Apellido_Estudiante, Fecha_Nacimiento, ID_Colegio) VALUES ('44000012', '20440000127', 'Elena', 'Ruiz', '2005-01-13', 12);
INSERT INTO Estudiante (DNI, CUIT, Nombre_Estudiante, Apellido_Estudiante, Fecha_Nacimiento, ID_Colegio) VALUES ('44000013', '20440000137', 'Tomás', 'Díaz', '2005-02-14', 13);
INSERT INTO Estudiante (DNI, CUIT, Nombre_Estudiante, Apellido_Estudiante, Fecha_Nacimiento, ID_Colegio) VALUES ('44000014', '20440000147', 'Mariana', 'Moreno', '2005-03-15', 14);
INSERT INTO Estudiante (DNI, CUIT, Nombre_Estudiante, Apellido_Estudiante, Fecha_Nacimiento, ID_Colegio) VALUES ('44000015', '20440000157', 'Agustín', 'Muñoz', '2005-04-16', 15);
INSERT INTO Estudiante (DNI, CUIT, Nombre_Estudiante, Apellido_Estudiante, Fecha_Nacimiento, ID_Colegio) VALUES ('44000016', '20440000167', 'Paula', 'Navarro', '2005-05-17', 16);
INSERT INTO Estudiante (DNI, CUIT, Nombre_Estudiante, Apellido_Estudiante, Fecha_Nacimiento, ID_Colegio) VALUES ('44000017', '20440000177', 'Gonzalo', 'Rojas', '2005-06-18', 17);
INSERT INTO Estudiante (DNI, CUIT, Nombre_Estudiante, Apellido_Estudiante, Fecha_Nacimiento, ID_Colegio) VALUES ('44000018', '20440000187', 'Victoria', 'Mendoza', '2005-07-19', 18);
INSERT INTO Estudiante (DNI, CUIT, Nombre_Estudiante, Apellido_Estudiante, Fecha_Nacimiento, ID_Colegio) VALUES ('44000019', '20440000197', 'Federico', 'Flores', '2005-08-20', 19);
INSERT INTO Estudiante (DNI, CUIT, Nombre_Estudiante, Apellido_Estudiante, Fecha_Nacimiento, ID_Colegio) VALUES ('44000020', '20440000207', 'Daniela', 'Acosta', '2005-09-21', 20);

-- Tabla Empleado (20 registros)
INSERT INTO Empleado (Nombre_Empleado, Apellido_Empleado, Puesto_Empleado) VALUES ('Joaquín', 'Pérez', 'Cocinero');
INSERT INTO Empleado (Nombre_Empleado, Apellido_Empleado, Puesto_Empleado) VALUES ('Martina', 'González', 'Administrador');
INSERT INTO Empleado (Nombre_Empleado, Apellido_Empleado, Puesto_Empleado) VALUES ('Nicolás', 'Sánchez', 'Encargado de Stock');
INSERT INTO Empleado (Nombre_Empleado, Apellido_Empleado, Puesto_Empleado) VALUES ('Lucía', 'Romero', 'Auxiliar de Limpieza');
INSERT INTO Empleado (Nombre_Empleado, Apellido_Empleado, Puesto_Empleado) VALUES ('Gabriel', 'Sosa', 'Cajero');
INSERT INTO Empleado (Nombre_Empleado, Apellido_Empleado, Puesto_Empleado) VALUES ('Elena', 'Torres', 'Cocinero');
INSERT INTO Empleado (Nombre_Empleado, Apellido_Empleado, Puesto_Empleado) VALUES ('Tomás', 'Álvarez', 'Administrador');
INSERT INTO Empleado (Nombre_Empleado, Apellido_Empleado, Puesto_Empleado) VALUES ('Mariana', 'Ruiz', 'Encargado de Stock');
INSERT INTO Empleado (Nombre_Empleado, Apellido_Empleado, Puesto_Empleado) VALUES ('Agustín', 'Díaz', 'Auxiliar de Limpieza');
INSERT INTO Empleado (Nombre_Empleado, Apellido_Empleado, Puesto_Empleado) VALUES ('Paula', 'Moreno', 'Cajero');
INSERT INTO Empleado (Nombre_Empleado, Apellido_Empleado, Puesto_Empleado) VALUES ('Gonzalo', 'Muñoz', 'Cocinero');
INSERT INTO Empleado (Nombre_Empleado, Apellido_Empleado, Puesto_Empleado) VALUES ('Victoria', 'Navarro', 'Administrador');
INSERT INTO Empleado (Nombre_Empleado, Apellido_Empleado, Puesto_Empleado) VALUES ('Federico', 'Rojas', 'Encargado de Stock');
INSERT INTO Empleado (Nombre_Empleado, Apellido_Empleado, Puesto_Empleado) VALUES ('Daniela', 'Mendoza', 'Auxiliar de Limpieza');
INSERT INTO Empleado (Nombre_Empleado, Apellido_Empleado, Puesto_Empleado) VALUES ('Alejandro', 'Flores', 'Cajero');
INSERT INTO Empleado (Nombre_Empleado, Apellido_Empleado, Puesto_Empleado) VALUES ('Sofía', 'Acosta', 'Cocinero');
INSERT INTO Empleado (Nombre_Empleado, Apellido_Empleado, Puesto_Empleado) VALUES ('Lucas', 'Gómez', 'Administrador');
INSERT INTO Empleado (Nombre_Empleado, Apellido_Empleado, Puesto_Empleado) VALUES ('Valentina', 'Martínez', 'Encargado de Stock');
INSERT INTO Empleado (Nombre_Empleado, Apellido_Empleado, Puesto_Empleado) VALUES ('Mateo', 'Rodríguez', 'Auxiliar de Limpieza');
INSERT INTO Empleado (Nombre_Empleado, Apellido_Empleado, Puesto_Empleado) VALUES ('Camila', 'López', 'Cajero');

-- Tabla Tipo_Evento (20 registros)
INSERT INTO Tipo_Evento (Nombre_Evento) VALUES ('Inicio de Sesión Exitoso');
INSERT INTO Tipo_Evento (Nombre_Evento) VALUES ('Cierre de Sesión');
INSERT INTO Tipo_Evento (Nombre_Evento) VALUES ('Confirmación de Venta');
INSERT INTO Tipo_Evento (Nombre_Evento) VALUES ('Entrega de Pedido');
INSERT INTO Tipo_Evento (Nombre_Evento) VALUES ('Intento Fallido de Login');
INSERT INTO Tipo_Evento (Nombre_Evento) VALUES ('Cambio de Contraseña');
INSERT INTO Tipo_Evento (Nombre_Evento) VALUES ('Actualización de Perfil');
INSERT INTO Tipo_Evento (Nombre_Evento) VALUES ('Registro de Usuario');
INSERT INTO Tipo_Evento (Nombre_Evento) VALUES ('Cancelación de Pedido');
INSERT INTO Tipo_Evento (Nombre_Evento) VALUES ('Carga de Saldo');
INSERT INTO Tipo_Evento (Nombre_Evento) VALUES ('Apertura de Caja');
INSERT INTO Tipo_Evento (Nombre_Evento) VALUES ('Cierre de Caja');
INSERT INTO Tipo_Evento (Nombre_Evento) VALUES ('Alta de Producto');
INSERT INTO Tipo_Evento (Nombre_Evento) VALUES ('Baja de Producto');
INSERT INTO Tipo_Evento (Nombre_Evento) VALUES ('Modificación de Stock');
INSERT INTO Tipo_Evento (Nombre_Evento) VALUES ('Creación de Menú del Día');
INSERT INTO Tipo_Evento (Nombre_Evento) VALUES ('Generación de Reporte');
INSERT INTO Tipo_Evento (Nombre_Evento) VALUES ('Asignación de Rol');
INSERT INTO Tipo_Evento (Nombre_Evento) VALUES ('Auditoría de Sistema');
INSERT INTO Tipo_Evento (Nombre_Evento) VALUES ('Error de Conexión');

-- Tabla Usuario (20 registros)
INSERT INTO Usuario (DNI, Apodo) VALUES ('44000001', 'user_alejandro');
INSERT INTO Usuario (DNI, Apodo) VALUES ('44000002', 'user_sofía');
INSERT INTO Usuario (DNI, Apodo) VALUES ('44000003', 'user_lucas');
INSERT INTO Usuario (DNI, Apodo) VALUES ('44000004', 'user_valentina');
INSERT INTO Usuario (DNI, Apodo) VALUES ('44000005', 'user_mateo');
INSERT INTO Usuario (DNI, Apodo) VALUES ('44000006', 'user_camila');
INSERT INTO Usuario (DNI, Apodo) VALUES ('44000007', 'user_joaquín');
INSERT INTO Usuario (DNI, Apodo) VALUES ('44000008', 'user_martina');
INSERT INTO Usuario (DNI, Apodo) VALUES ('44000009', 'user_nicolás');
INSERT INTO Usuario (DNI, Apodo) VALUES ('44000010', 'user_lucía');
INSERT INTO Usuario (DNI, Apodo) VALUES ('44000011', 'user_gabriel');
INSERT INTO Usuario (DNI, Apodo) VALUES ('44000012', 'user_elena');
INSERT INTO Usuario (DNI, Apodo) VALUES ('44000013', 'user_tomás');
INSERT INTO Usuario (DNI, Apodo) VALUES ('44000014', 'user_mariana');
INSERT INTO Usuario (DNI, Apodo) VALUES ('44000015', 'user_agustín');
INSERT INTO Usuario (DNI, Apodo) VALUES ('44000016', 'user_paula');
INSERT INTO Usuario (DNI, Apodo) VALUES ('44000017', 'user_gonzalo');
INSERT INTO Usuario (DNI, Apodo) VALUES ('44000018', 'user_victoria');
INSERT INTO Usuario (DNI, Apodo) VALUES ('44000019', 'user_federico');
INSERT INTO Usuario (DNI, Apodo) VALUES ('44000020', 'user_daniela');

-- Tabla Email (20 registros)
INSERT INTO Email (UserID, Email) VALUES (1, 'alejandro.gómez1@estudiante.edu.ar');
INSERT INTO Email (UserID, Email) VALUES (2, 'sofía.martínez2@estudiante.edu.ar');
INSERT INTO Email (UserID, Email) VALUES (3, 'lucas.rodríguez3@estudiante.edu.ar');
INSERT INTO Email (UserID, Email) VALUES (4, 'valentina.lópez4@estudiante.edu.ar');
INSERT INTO Email (UserID, Email) VALUES (5, 'mateo.pérez5@estudiante.edu.ar');
INSERT INTO Email (UserID, Email) VALUES (6, 'camila.gonzález6@estudiante.edu.ar');
INSERT INTO Email (UserID, Email) VALUES (7, 'joaquín.sánchez7@estudiante.edu.ar');
INSERT INTO Email (UserID, Email) VALUES (8, 'martina.romero8@estudiante.edu.ar');
INSERT INTO Email (UserID, Email) VALUES (9, 'nicolás.sosa9@estudiante.edu.ar');
INSERT INTO Email (UserID, Email) VALUES (10, 'lucía.torres10@estudiante.edu.ar');
INSERT INTO Email (UserID, Email) VALUES (11, 'gabriel.álvarez11@estudiante.edu.ar');
INSERT INTO Email (UserID, Email) VALUES (12, 'elena.ruiz12@estudiante.edu.ar');
INSERT INTO Email (UserID, Email) VALUES (13, 'tomás.díaz13@estudiante.edu.ar');
INSERT INTO Email (UserID, Email) VALUES (14, 'mariana.moreno14@estudiante.edu.ar');
INSERT INTO Email (UserID, Email) VALUES (15, 'agustín.muñoz15@estudiante.edu.ar');
INSERT INTO Email (UserID, Email) VALUES (16, 'paula.navarro16@estudiante.edu.ar');
INSERT INTO Email (UserID, Email) VALUES (17, 'gonzalo.rojas17@estudiante.edu.ar');
INSERT INTO Email (UserID, Email) VALUES (18, 'victoria.mendoza18@estudiante.edu.ar');
INSERT INTO Email (UserID, Email) VALUES (19, 'federico.flores19@estudiante.edu.ar');
INSERT INTO Email (UserID, Email) VALUES (20, 'daniela.acosta20@estudiante.edu.ar');

-- Tabla Telefono (20 registros)
INSERT INTO Telefono (UserID, Telefono) VALUES (1, '1144000001');
INSERT INTO Telefono (UserID, Telefono) VALUES (2, '1144000002');
INSERT INTO Telefono (UserID, Telefono) VALUES (3, '1144000003');
INSERT INTO Telefono (UserID, Telefono) VALUES (4, '1144000004');
INSERT INTO Telefono (UserID, Telefono) VALUES (5, '1144000005');
INSERT INTO Telefono (UserID, Telefono) VALUES (6, '1144000006');
INSERT INTO Telefono (UserID, Telefono) VALUES (7, '1144000007');
INSERT INTO Telefono (UserID, Telefono) VALUES (8, '1144000008');
INSERT INTO Telefono (UserID, Telefono) VALUES (9, '1144000009');
INSERT INTO Telefono (UserID, Telefono) VALUES (10, '1144000010');
INSERT INTO Telefono (UserID, Telefono) VALUES (11, '1144000011');
INSERT INTO Telefono (UserID, Telefono) VALUES (12, '1144000012');
INSERT INTO Telefono (UserID, Telefono) VALUES (13, '1144000013');
INSERT INTO Telefono (UserID, Telefono) VALUES (14, '1144000014');
INSERT INTO Telefono (UserID, Telefono) VALUES (15, '1144000015');
INSERT INTO Telefono (UserID, Telefono) VALUES (16, '1144000016');
INSERT INTO Telefono (UserID, Telefono) VALUES (17, '1144000017');
INSERT INTO Telefono (UserID, Telefono) VALUES (18, '1144000018');
INSERT INTO Telefono (UserID, Telefono) VALUES (19, '1144000019');
INSERT INTO Telefono (UserID, Telefono) VALUES (20, '1144000020');

-- Tabla Log_Auditoria (20 registros)-- Luego lps poblamos

-- ============================================================================
-- 3. SCHEMA COMERCIAL
-- ============================================================================

USE schema_comercial;

-- Tabla Categoria (20 registros)
INSERT INTO Categoria (Categoria) VALUES ('Salado');
INSERT INTO Categoria (Categoria) VALUES ('Dulce');
INSERT INTO Categoria (Categoria) VALUES ('Snacks');
INSERT INTO Categoria (Categoria) VALUES ('Bebidas Sin Alcohol');
INSERT INTO Categoria (Categoria) VALUES ('Menú del Día');
INSERT INTO Categoria (Categoria) VALUES ('Golosinas');
INSERT INTO Categoria (Categoria) VALUES ('Ensaladas');
INSERT INTO Categoria (Categoria) VALUES ('Frutas');
INSERT INTO Categoria (Categoria) VALUES ('Sandwiches Especiales');
INSERT INTO Categoria (Categoria) VALUES ('Panadería');
INSERT INTO Categoria (Categoria) VALUES ('Cafetería');
INSERT INTO Categoria (Categoria) VALUES ('Bebidas Calientes');
INSERT INTO Categoria (Categoria) VALUES ('Licuados');
INSERT INTO Categoria (Categoria) VALUES ('Postres');
INSERT INTO Categoria (Categoria) VALUES ('Pizzas y Empanadas');
INSERT INTO Categoria (Categoria) VALUES ('Tartas');
INSERT INTO Categoria (Categoria) VALUES ('Combos Promocionales');
INSERT INTO Categoria (Categoria) VALUES ('Platos Calientes');
INSERT INTO Categoria (Categoria) VALUES ('Comida Saludable');
INSERT INTO Categoria (Categoria) VALUES ('Snacks Saludables');

-- Tabla Metodo_Pago (20 registros)
INSERT INTO Metodo_Pago (MetodoPago) VALUES ('Efectivo');
INSERT INTO Metodo_Pago (MetodoPago) VALUES ('Mercado Pago');
INSERT INTO Metodo_Pago (MetodoPago) VALUES ('Cuenta Corriente');
INSERT INTO Metodo_Pago (MetodoPago) VALUES ('Tarjeta de Débito Visa');
INSERT INTO Metodo_Pago (MetodoPago) VALUES ('Tarjeta de Débito Mastercard');
INSERT INTO Metodo_Pago (MetodoPago) VALUES ('Tarjeta de Crédito Visa');
INSERT INTO Metodo_Pago (MetodoPago) VALUES ('Tarjeta de Crédito Mastercard');
INSERT INTO Metodo_Pago (MetodoPago) VALUES ('Transferencia bancaria');
INSERT INTO Metodo_Pago (MetodoPago) VALUES ('QR Modo');
INSERT INTO Metodo_Pago (MetodoPago) VALUES ('Cuenta DNI');
INSERT INTO Metodo_Pago (MetodoPago) VALUES ('BUEPP');
INSERT INTO Metodo_Pago (MetodoPago) VALUES ('Tarjeta Naranja');
INSERT INTO Metodo_Pago (MetodoPago) VALUES ('Vale de Buffet');
INSERT INTO Metodo_Pago (MetodoPago) VALUES ('Beca de Alimentación');
INSERT INTO Metodo_Pago (MetodoPago) VALUES ('Tarjeta CABAL');
INSERT INTO Metodo_Pago (MetodoPago) VALUES ('Pago Mis Cuentas');
INSERT INTO Metodo_Pago (MetodoPago) VALUES ('Ualá');
INSERT INTO Metodo_Pago (MetodoPago) VALUES ('Brubank');
INSERT INTO Metodo_Pago (MetodoPago) VALUES ('Personal Pay');
INSERT INTO Metodo_Pago (MetodoPago) VALUES ('AURA');

-- Tabla Estado_Retiro (20 registros)
INSERT INTO Estado_Retiro (Estado) VALUES ('PENDIENTE');
INSERT INTO Estado_Retiro (Estado) VALUES ('ENTREGADO');
INSERT INTO Estado_Retiro (Estado) VALUES ('CANCELADO');
INSERT INTO Estado_Retiro (Estado) VALUES ('EN PREPARACION');
INSERT INTO Estado_Retiro (Estado) VALUES ('LISTO PARA RETIRAR');
INSERT INTO Estado_Retiro (Estado) VALUES ('DEMORADO');
INSERT INTO Estado_Retiro (Estado) VALUES ('NO RETIRADO');
INSERT INTO Estado_Retiro (Estado) VALUES ('RECLAMADO');
INSERT INTO Estado_Retiro (Estado) VALUES ('REEMBOLSADO');
INSERT INTO Estado_Retiro (Estado) VALUES ('EN ESPERA DE STOCK');
INSERT INTO Estado_Retiro (Estado) VALUES ('CONFIRMADO');
INSERT INTO Estado_Retiro (Estado) VALUES ('EN TRANSITO');
INSERT INTO Estado_Retiro (Estado) VALUES ('EN MOSTRADOR');
INSERT INTO Estado_Retiro (Estado) VALUES ('ANULADO');
INSERT INTO Estado_Retiro (Estado) VALUES ('RESERVADO');
INSERT INTO Estado_Retiro (Estado) VALUES ('EXPIRED');
INSERT INTO Estado_Retiro (Estado) VALUES ('PAUSADO');
INSERT INTO Estado_Retiro (Estado) VALUES ('PRIORITARIO');
INSERT INTO Estado_Retiro (Estado) VALUES ('ENVERIFICACION');
INSERT INTO Estado_Retiro (Estado) VALUES ('FINALIZADO');

-- Tabla Producto (20 registros)
INSERT INTO Producto (ID_Categoria, Nombre_Producto, Descripcion_Producto, Stock_Producto, Precio_Producto) VALUES (1, 'Sánguche de Miga Jamón y Queso', 'Sánguche fresco de miga doble', 30, 1500.00);
INSERT INTO Producto (ID_Categoria, Nombre_Producto, Descripcion_Producto, Stock_Producto, Precio_Producto) VALUES (1, 'Empanada de Carne', 'Empanada al horno de carne cortada a cuchillo', 50, 1200.00);
INSERT INTO Producto (ID_Categoria, Nombre_Producto, Descripcion_Producto, Stock_Producto, Precio_Producto) VALUES (2, 'Alfajor de Chocolate', 'Alfajor relleno de dulce de leche', 40, 900.00);
INSERT INTO Producto (ID_Categoria, Nombre_Producto, Descripcion_Producto, Stock_Producto, Precio_Producto) VALUES (3, 'Papas Fritas 80g', 'Bolsa de papas fritas de paquete', 25, 1000.00);
INSERT INTO Producto (ID_Categoria, Nombre_Producto, Descripcion_Producto, Stock_Producto, Precio_Producto) VALUES (4, 'Agua Mineral 500ml', 'Agua sin gas', 60, 800.00);
INSERT INTO Producto (ID_Categoria, Nombre_Producto, Descripcion_Producto, Stock_Producto, Precio_Producto) VALUES (4, 'Gaseosa Cola 500ml', 'Gaseosa marca reconocida', 45, 1300.00);
INSERT INTO Producto (ID_Categoria, Nombre_Producto, Descripcion_Producto, Stock_Producto, Precio_Producto) VALUES (7, 'Ensalada Caesar', 'Ensalada con pollo, lechuga y croutons', 15, 2800.00);
INSERT INTO Producto (ID_Categoria, Nombre_Producto, Descripcion_Producto, Stock_Producto, Precio_Producto) VALUES (8, 'Manzana Roja', 'Fruta fresca de estación', 50, 400.00);
INSERT INTO Producto (ID_Categoria, Nombre_Producto, Descripcion_Producto, Stock_Producto, Precio_Producto) VALUES (11, 'Café con Leche', 'Vasito de café con leche caliente', 40, 1100.00);
INSERT INTO Producto (ID_Categoria, Nombre_Producto, Descripcion_Producto, Stock_Producto, Precio_Producto) VALUES (10, 'Medialuna de Manteca', 'Medialuna artesanal', 60, 500.00);
INSERT INTO Producto (ID_Categoria, Nombre_Producto, Descripcion_Producto, Stock_Producto, Precio_Producto) VALUES (16, 'Tarta de Jamón y Queso', 'Porción de tarta individual', 20, 2200.00);
INSERT INTO Producto (ID_Categoria, Nombre_Producto, Descripcion_Producto, Stock_Producto, Precio_Producto) VALUES (15, 'Pizza Muzzarella Porción', 'Porción de pizza a la piedra', 35, 1400.00);
INSERT INTO Producto (ID_Categoria, Nombre_Producto, Descripcion_Producto, Stock_Producto, Precio_Producto) VALUES (13, 'Jugo de Naranja Natural', 'Jugo exprimir 300ml', 25, 1600.00);
INSERT INTO Producto (ID_Categoria, Nombre_Producto, Descripcion_Producto, Stock_Producto, Precio_Producto) VALUES (6, 'Chocolatina 25g', 'Chocolate con leche', 45, 700.00);
INSERT INTO Producto (ID_Categoria, Nombre_Producto, Descripcion_Producto, Stock_Producto, Precio_Producto) VALUES (6, 'Turrón de Maní', 'Turrón obleas con maní', 70, 350.00);
INSERT INTO Producto (ID_Categoria, Nombre_Producto, Descripcion_Producto, Stock_Producto, Precio_Producto) VALUES (19, 'Yogur con Cereales', 'Pote de yogur y granola', 20, 1700.00);
INSERT INTO Producto (ID_Categoria, Nombre_Producto, Descripcion_Producto, Stock_Producto, Precio_Producto) VALUES (9, 'Sánguche de Milanesa', 'Sánguche de milanesa en pan francés', 25, 3200.00);
INSERT INTO Producto (ID_Categoria, Nombre_Producto, Descripcion_Producto, Stock_Producto, Precio_Producto) VALUES (13, 'Licuado de Banana', 'Licuado con leche 400ml', 18, 1900.00);
INSERT INTO Producto (ID_Categoria, Nombre_Producto, Descripcion_Producto, Stock_Producto, Precio_Producto) VALUES (1, 'Panchito con Papas', 'Pancho con lluvia de papas', 30, 1300.00);
INSERT INTO Producto (ID_Categoria, Nombre_Producto, Descripcion_Producto, Stock_Producto, Precio_Producto) VALUES (20, 'Barrita de Cereal', 'Barrita nutritiva de avena y miel', 50, 600.00);

-- Tabla Menu_Del_Dia (20 registros)
INSERT INTO Menu_Del_Dia (ID_Categoria, Plato_menu, Precio_menu, Stock_menu, Fecha_menu) VALUES (5, 'Menú Ejecutivo Día 1 - Plato Especial', 2550.00, 16, '2026-10-02');
INSERT INTO Menu_Del_Dia (ID_Categoria, Plato_menu, Precio_menu, Stock_menu, Fecha_menu) VALUES (5, 'Menú Ejecutivo Día 2 - Plato Especial', 2600.00, 17, '2026-10-03');
INSERT INTO Menu_Del_Dia (ID_Categoria, Plato_menu, Precio_menu, Stock_menu, Fecha_menu) VALUES (5, 'Menú Ejecutivo Día 3 - Plato Especial', 2650.00, 18, '2026-10-04');
INSERT INTO Menu_Del_Dia (ID_Categoria, Plato_menu, Precio_menu, Stock_menu, Fecha_menu) VALUES (5, 'Menú Ejecutivo Día 4 - Plato Especial', 2700.00, 19, '2026-10-05');
INSERT INTO Menu_Del_Dia (ID_Categoria, Plato_menu, Precio_menu, Stock_menu, Fecha_menu) VALUES (5, 'Menú Ejecutivo Día 5 - Plato Especial', 2750.00, 20, '2026-10-06');
INSERT INTO Menu_Del_Dia (ID_Categoria, Plato_menu, Precio_menu, Stock_menu, Fecha_menu) VALUES (5, 'Menú Ejecutivo Día 6 - Plato Especial', 2800.00, 21, '2026-10-07');
INSERT INTO Menu_Del_Dia (ID_Categoria, Plato_menu, Precio_menu, Stock_menu, Fecha_menu) VALUES (5, 'Menú Ejecutivo Día 7 - Plato Especial', 2850.00, 22, '2026-10-08');
INSERT INTO Menu_Del_Dia (ID_Categoria, Plato_menu, Precio_menu, Stock_menu, Fecha_menu) VALUES (5, 'Menú Ejecutivo Día 8 - Plato Especial', 2900.00, 23, '2026-10-09');
INSERT INTO Menu_Del_Dia (ID_Categoria, Plato_menu, Precio_menu, Stock_menu, Fecha_menu) VALUES (5, 'Menú Ejecutivo Día 9 - Plato Especial', 2950.00, 24, '2026-10-10');
INSERT INTO Menu_Del_Dia (ID_Categoria, Plato_menu, Precio_menu, Stock_menu, Fecha_menu) VALUES (5, 'Menú Ejecutivo Día 10 - Plato Especial', 3000.00, 15, '2026-10-11');
INSERT INTO Menu_Del_Dia (ID_Categoria, Plato_menu, Precio_menu, Stock_menu, Fecha_menu) VALUES (5, 'Menú Ejecutivo Día 11 - Plato Especial', 3050.00, 16, '2026-10-12');
INSERT INTO Menu_Del_Dia (ID_Categoria, Plato_menu, Precio_menu, Stock_menu, Fecha_menu) VALUES (5, 'Menú Ejecutivo Día 12 - Plato Especial', 3100.00, 17, '2026-10-13');
INSERT INTO Menu_Del_Dia (ID_Categoria, Plato_menu, Precio_menu, Stock_menu, Fecha_menu) VALUES (5, 'Menú Ejecutivo Día 13 - Plato Especial', 3150.00, 18, '2026-10-14');
INSERT INTO Menu_Del_Dia (ID_Categoria, Plato_menu, Precio_menu, Stock_menu, Fecha_menu) VALUES (5, 'Menú Ejecutivo Día 14 - Plato Especial', 3200.00, 19, '2026-10-15');
INSERT INTO Menu_Del_Dia (ID_Categoria, Plato_menu, Precio_menu, Stock_menu, Fecha_menu) VALUES (5, 'Menú Ejecutivo Día 15 - Plato Especial', 3250.00, 20, '2026-10-16');
INSERT INTO Menu_Del_Dia (ID_Categoria, Plato_menu, Precio_menu, Stock_menu, Fecha_menu) VALUES (5, 'Menú Ejecutivo Día 16 - Plato Especial', 3300.00, 21, '2026-10-17');
INSERT INTO Menu_Del_Dia (ID_Categoria, Plato_menu, Precio_menu, Stock_menu, Fecha_menu) VALUES (5, 'Menú Ejecutivo Día 17 - Plato Especial', 3350.00, 22, '2026-10-18');
INSERT INTO Menu_Del_Dia (ID_Categoria, Plato_menu, Precio_menu, Stock_menu, Fecha_menu) VALUES (5, 'Menú Ejecutivo Día 18 - Plato Especial', 3400.00, 23, '2026-10-19');
INSERT INTO Menu_Del_Dia (ID_Categoria, Plato_menu, Precio_menu, Stock_menu, Fecha_menu) VALUES (5, 'Menú Ejecutivo Día 19 - Plato Especial', 3450.00, 24, '2026-10-20');
INSERT INTO Menu_Del_Dia (ID_Categoria, Plato_menu, Precio_menu, Stock_menu, Fecha_menu) VALUES (5, 'Menú Ejecutivo Día 20 - Plato Especial', 3500.00, 15, '2026-10-21');

-- Tabla Detalle_Menu_Dia (20 registros)
INSERT INTO Detalle_Menu_Dia (Menu_Del_DiaID, ProductoID) VALUES (1, 1);
INSERT INTO Detalle_Menu_Dia (Menu_Del_DiaID, ProductoID) VALUES (2, 2);
INSERT INTO Detalle_Menu_Dia (Menu_Del_DiaID, ProductoID) VALUES (3, 3);
INSERT INTO Detalle_Menu_Dia (Menu_Del_DiaID, ProductoID) VALUES (4, 4);
INSERT INTO Detalle_Menu_Dia (Menu_Del_DiaID, ProductoID) VALUES (5, 5);
INSERT INTO Detalle_Menu_Dia (Menu_Del_DiaID, ProductoID) VALUES (6, 6);
INSERT INTO Detalle_Menu_Dia (Menu_Del_DiaID, ProductoID) VALUES (7, 7);
INSERT INTO Detalle_Menu_Dia (Menu_Del_DiaID, ProductoID) VALUES (8, 8);
INSERT INTO Detalle_Menu_Dia (Menu_Del_DiaID, ProductoID) VALUES (9, 9);
INSERT INTO Detalle_Menu_Dia (Menu_Del_DiaID, ProductoID) VALUES (10, 10);
INSERT INTO Detalle_Menu_Dia (Menu_Del_DiaID, ProductoID) VALUES (11, 11);
INSERT INTO Detalle_Menu_Dia (Menu_Del_DiaID, ProductoID) VALUES (12, 12);
INSERT INTO Detalle_Menu_Dia (Menu_Del_DiaID, ProductoID) VALUES (13, 13);
INSERT INTO Detalle_Menu_Dia (Menu_Del_DiaID, ProductoID) VALUES (14, 14);
INSERT INTO Detalle_Menu_Dia (Menu_Del_DiaID, ProductoID) VALUES (15, 15);
INSERT INTO Detalle_Menu_Dia (Menu_Del_DiaID, ProductoID) VALUES (16, 16);
INSERT INTO Detalle_Menu_Dia (Menu_Del_DiaID, ProductoID) VALUES (17, 17);
INSERT INTO Detalle_Menu_Dia (Menu_Del_DiaID, ProductoID) VALUES (18, 18);
INSERT INTO Detalle_Menu_Dia (Menu_Del_DiaID, ProductoID) VALUES (19, 19);
INSERT INTO Detalle_Menu_Dia (Menu_Del_DiaID, ProductoID) VALUES (20, 20);

-- Tabla Venta (50 registros - TABLA PRINCIPAL)
INSERT INTO Venta (EmpleadoID, UserID, Metodo_PagoID, Fecha_Hora, Monto_Total) VALUES (1, 1, 1, '2026-10-01 09:07:00', 1150.00);
INSERT INTO Venta (EmpleadoID, UserID, Metodo_PagoID, Fecha_Hora, Monto_Total) VALUES (2, 2, 2, '2026-10-02 10:14:00', 1300.00);
INSERT INTO Venta (EmpleadoID, UserID, Metodo_PagoID, Fecha_Hora, Monto_Total) VALUES (3, 3, 3, '2026-10-03 11:21:00', 1450.00);
INSERT INTO Venta (EmpleadoID, UserID, Metodo_PagoID, Fecha_Hora, Monto_Total) VALUES (4, 4, 4, '2026-10-04 12:28:00', 1600.00);
INSERT INTO Venta (EmpleadoID, UserID, Metodo_PagoID, Fecha_Hora, Monto_Total) VALUES (5, 5, 5, '2026-10-05 13:35:00', 1750.00);
INSERT INTO Venta (EmpleadoID, UserID, Metodo_PagoID, Fecha_Hora, Monto_Total) VALUES (6, 6, 6, '2026-10-06 14:42:00', 1900.00);
INSERT INTO Venta (EmpleadoID, UserID, Metodo_PagoID, Fecha_Hora, Monto_Total) VALUES (7, 7, 7, '2026-10-07 15:49:00', 2050.00);
INSERT INTO Venta (EmpleadoID, UserID, Metodo_PagoID, Fecha_Hora, Monto_Total) VALUES (8, 8, 8, '2026-10-08 16:56:00', 2200.00);
INSERT INTO Venta (EmpleadoID, UserID, Metodo_PagoID, Fecha_Hora, Monto_Total) VALUES (9, 9, 9, '2026-10-09 17:03:00', 2350.00);
INSERT INTO Venta (EmpleadoID, UserID, Metodo_PagoID, Fecha_Hora, Monto_Total) VALUES (10, 10, 10, '2026-10-10 08:10:00', 2500.00);
INSERT INTO Venta (EmpleadoID, UserID, Metodo_PagoID, Fecha_Hora, Monto_Total) VALUES (11, 11, 11, '2026-10-11 09:17:00', 2650.00);
INSERT INTO Venta (EmpleadoID, UserID, Metodo_PagoID, Fecha_Hora, Monto_Total) VALUES (12, 12, 12, '2026-10-12 10:24:00', 2800.00);
INSERT INTO Venta (EmpleadoID, UserID, Metodo_PagoID, Fecha_Hora, Monto_Total) VALUES (13, 13, 13, '2026-10-13 11:31:00', 2950.00);
INSERT INTO Venta (EmpleadoID, UserID, Metodo_PagoID, Fecha_Hora, Monto_Total) VALUES (14, 14, 14, '2026-10-14 12:38:00', 3100.00);
INSERT INTO Venta (EmpleadoID, UserID, Metodo_PagoID, Fecha_Hora, Monto_Total) VALUES (15, 15, 15, '2026-10-15 13:45:00', 3250.00);
INSERT INTO Venta (EmpleadoID, UserID, Metodo_PagoID, Fecha_Hora, Monto_Total) VALUES (16, 16, 16, '2026-10-16 14:52:00', 3400.00);
INSERT INTO Venta (EmpleadoID, UserID, Metodo_PagoID, Fecha_Hora, Monto_Total) VALUES (17, 17, 17, '2026-10-17 15:59:00', 3550.00);
INSERT INTO Venta (EmpleadoID, UserID, Metodo_PagoID, Fecha_Hora, Monto_Total) VALUES (18, 18, 18, '2026-10-18 16:06:00', 3700.00);
INSERT INTO Venta (EmpleadoID, UserID, Metodo_PagoID, Fecha_Hora, Monto_Total) VALUES (19, 19, 19, '2026-10-19 17:13:00', 3850.00);
INSERT INTO Venta (EmpleadoID, UserID, Metodo_PagoID, Fecha_Hora, Monto_Total) VALUES (20, 20, 20, '2026-10-20 08:20:00', 4000.00);
INSERT INTO Venta (EmpleadoID, UserID, Metodo_PagoID, Fecha_Hora, Monto_Total) VALUES (1, 1, 1, '2026-10-21 09:27:00', 4150.00);
INSERT INTO Venta (EmpleadoID, UserID, Metodo_PagoID, Fecha_Hora, Monto_Total) VALUES (2, 2, 2, '2026-10-22 10:34:00', 4300.00);
INSERT INTO Venta (EmpleadoID, UserID, Metodo_PagoID, Fecha_Hora, Monto_Total) VALUES (3, 3, 3, '2026-10-23 11:41:00', 4450.00);
INSERT INTO Venta (EmpleadoID, UserID, Metodo_PagoID, Fecha_Hora, Monto_Total) VALUES (4, 4, 4, '2026-10-24 12:48:00', 4600.00);
INSERT INTO Venta (EmpleadoID, UserID, Metodo_PagoID, Fecha_Hora, Monto_Total) VALUES (5, 5, 5, '2026-10-25 13:55:00', 4750.00);
INSERT INTO Venta (EmpleadoID, UserID, Metodo_PagoID, Fecha_Hora, Monto_Total) VALUES (6, 6, 6, '2026-10-01 14:02:00', 4900.00);
INSERT INTO Venta (EmpleadoID, UserID, Metodo_PagoID, Fecha_Hora, Monto_Total) VALUES (7, 7, 7, '2026-10-02 15:09:00', 5050.00);
INSERT INTO Venta (EmpleadoID, UserID, Metodo_PagoID, Fecha_Hora, Monto_Total) VALUES (8, 8, 8, '2026-10-03 16:16:00', 5200.00);
INSERT INTO Venta (EmpleadoID, UserID, Metodo_PagoID, Fecha_Hora, Monto_Total) VALUES (9, 9, 9, '2026-10-04 17:23:00', 5350.00);
INSERT INTO Venta (EmpleadoID, UserID, Metodo_PagoID, Fecha_Hora, Monto_Total) VALUES (10, 10, 10, '2026-10-05 08:30:00', 5500.00);
INSERT INTO Venta (EmpleadoID, UserID, Metodo_PagoID, Fecha_Hora, Monto_Total) VALUES (11, 11, 11, '2026-10-06 09:37:00', 5650.00);
INSERT INTO Venta (EmpleadoID, UserID, Metodo_PagoID, Fecha_Hora, Monto_Total) VALUES (12, 12, 12, '2026-10-07 10:44:00', 5800.00);
INSERT INTO Venta (EmpleadoID, UserID, Metodo_PagoID, Fecha_Hora, Monto_Total) VALUES (13, 13, 13, '2026-10-08 11:51:00', 5950.00);
INSERT INTO Venta (EmpleadoID, UserID, Metodo_PagoID, Fecha_Hora, Monto_Total) VALUES (14, 14, 14, '2026-10-09 12:58:00', 6100.00);
INSERT INTO Venta (EmpleadoID, UserID, Metodo_PagoID, Fecha_Hora, Monto_Total) VALUES (15, 15, 15, '2026-10-10 13:05:00', 6250.00);
INSERT INTO Venta (EmpleadoID, UserID, Metodo_PagoID, Fecha_Hora, Monto_Total) VALUES (16, 16, 16, '2026-10-11 14:12:00', 6400.00);
INSERT INTO Venta (EmpleadoID, UserID, Metodo_PagoID, Fecha_Hora, Monto_Total) VALUES (17, 17, 17, '2026-10-12 15:19:00', 6550.00);
INSERT INTO Venta (EmpleadoID, UserID, Metodo_PagoID, Fecha_Hora, Monto_Total) VALUES (18, 18, 18, '2026-10-13 16:26:00', 6700.00);
INSERT INTO Venta (EmpleadoID, UserID, Metodo_PagoID, Fecha_Hora, Monto_Total) VALUES (19, 19, 19, '2026-10-14 17:33:00', 6850.00);
INSERT INTO Venta (EmpleadoID, UserID, Metodo_PagoID, Fecha_Hora, Monto_Total) VALUES (20, 20, 20, '2026-10-15 08:40:00', 7000.00);
INSERT INTO Venta (EmpleadoID, UserID, Metodo_PagoID, Fecha_Hora, Monto_Total) VALUES (1, 1, 1, '2026-10-16 09:47:00', 7150.00);
INSERT INTO Venta (EmpleadoID, UserID, Metodo_PagoID, Fecha_Hora, Monto_Total) VALUES (2, 2, 2, '2026-10-17 10:54:00', 7300.00);
INSERT INTO Venta (EmpleadoID, UserID, Metodo_PagoID, Fecha_Hora, Monto_Total) VALUES (3, 3, 3, '2026-10-18 11:01:00', 7450.00);
INSERT INTO Venta (EmpleadoID, UserID, Metodo_PagoID, Fecha_Hora, Monto_Total) VALUES (4, 4, 4, '2026-10-19 12:08:00', 7600.00);
INSERT INTO Venta (EmpleadoID, UserID, Metodo_PagoID, Fecha_Hora, Monto_Total) VALUES (5, 5, 5, '2026-10-20 13:15:00', 7750.00);
INSERT INTO Venta (EmpleadoID, UserID, Metodo_PagoID, Fecha_Hora, Monto_Total) VALUES (6, 6, 6, '2026-10-21 14:22:00', 7900.00);
INSERT INTO Venta (EmpleadoID, UserID, Metodo_PagoID, Fecha_Hora, Monto_Total) VALUES (7, 7, 7, '2026-10-22 15:29:00', 8050.00);
INSERT INTO Venta (EmpleadoID, UserID, Metodo_PagoID, Fecha_Hora, Monto_Total) VALUES (8, 8, 8, '2026-10-23 16:36:00', 8200.00);
INSERT INTO Venta (EmpleadoID, UserID, Metodo_PagoID, Fecha_Hora, Monto_Total) VALUES (9, 9, 9, '2026-10-24 17:43:00', 8350.00);
INSERT INTO Venta (EmpleadoID, UserID, Metodo_PagoID, Fecha_Hora, Monto_Total) VALUES (10, 10, 10, '2026-10-25 08:50:00', 8500.00);

-- Tabla Carrito (50 registros - para las 50 ventas)
INSERT INTO Carrito (VentaID, ProductoID, Menu_Del_DiaID, Cantidad, precio_Unitario) VALUES (1, 1, NULL, 2, 850.00);
INSERT INTO Carrito (VentaID, ProductoID, Menu_Del_DiaID, Cantidad, precio_Unitario) VALUES (2, 2, NULL, 3, 900.00);
INSERT INTO Carrito (VentaID, ProductoID, Menu_Del_DiaID, Cantidad, precio_Unitario) VALUES (3, 3, NULL, 1, 950.00);
INSERT INTO Carrito (VentaID, ProductoID, Menu_Del_DiaID, Cantidad, precio_Unitario) VALUES (4, 4, NULL, 2, 1000.00);
INSERT INTO Carrito (VentaID, ProductoID, Menu_Del_DiaID, Cantidad, precio_Unitario) VALUES (5, 5, NULL, 3, 1050.00);
INSERT INTO Carrito (VentaID, ProductoID, Menu_Del_DiaID, Cantidad, precio_Unitario) VALUES (6, 6, NULL, 1, 1100.00);
INSERT INTO Carrito (VentaID, ProductoID, Menu_Del_DiaID, Cantidad, precio_Unitario) VALUES (7, 7, NULL, 2, 1150.00);
INSERT INTO Carrito (VentaID, ProductoID, Menu_Del_DiaID, Cantidad, precio_Unitario) VALUES (8, 8, NULL, 3, 1200.00);
INSERT INTO Carrito (VentaID, ProductoID, Menu_Del_DiaID, Cantidad, precio_Unitario) VALUES (9, 9, NULL, 1, 1250.00);
INSERT INTO Carrito (VentaID, ProductoID, Menu_Del_DiaID, Cantidad, precio_Unitario) VALUES (10, 10, NULL, 2, 1300.00);
INSERT INTO Carrito (VentaID, ProductoID, Menu_Del_DiaID, Cantidad, precio_Unitario) VALUES (11, 11, NULL, 3, 1350.00);
INSERT INTO Carrito (VentaID, ProductoID, Menu_Del_DiaID, Cantidad, precio_Unitario) VALUES (12, 12, NULL, 1, 1400.00);
INSERT INTO Carrito (VentaID, ProductoID, Menu_Del_DiaID, Cantidad, precio_Unitario) VALUES (13, 13, NULL, 2, 1450.00);
INSERT INTO Carrito (VentaID, ProductoID, Menu_Del_DiaID, Cantidad, precio_Unitario) VALUES (14, 14, NULL, 3, 1500.00);
INSERT INTO Carrito (VentaID, ProductoID, Menu_Del_DiaID, Cantidad, precio_Unitario) VALUES (15, 15, NULL, 1, 1550.00);
INSERT INTO Carrito (VentaID, ProductoID, Menu_Del_DiaID, Cantidad, precio_Unitario) VALUES (16, 16, NULL, 2, 1600.00);
INSERT INTO Carrito (VentaID, ProductoID, Menu_Del_DiaID, Cantidad, precio_Unitario) VALUES (17, 17, NULL, 3, 1650.00);
INSERT INTO Carrito (VentaID, ProductoID, Menu_Del_DiaID, Cantidad, precio_Unitario) VALUES (18, 18, NULL, 1, 1700.00);
INSERT INTO Carrito (VentaID, ProductoID, Menu_Del_DiaID, Cantidad, precio_Unitario) VALUES (19, 19, NULL, 2, 1750.00);
INSERT INTO Carrito (VentaID, ProductoID, Menu_Del_DiaID, Cantidad, precio_Unitario) VALUES (20, 20, NULL, 3, 1800.00);
INSERT INTO Carrito (VentaID, ProductoID, Menu_Del_DiaID, Cantidad, precio_Unitario) VALUES (21, 1, NULL, 1, 1850.00);
INSERT INTO Carrito (VentaID, ProductoID, Menu_Del_DiaID, Cantidad, precio_Unitario) VALUES (22, 2, NULL, 2, 1900.00);
INSERT INTO Carrito (VentaID, ProductoID, Menu_Del_DiaID, Cantidad, precio_Unitario) VALUES (23, 3, NULL, 3, 1950.00);
INSERT INTO Carrito (VentaID, ProductoID, Menu_Del_DiaID, Cantidad, precio_Unitario) VALUES (24, 4, NULL, 1, 2000.00);
INSERT INTO Carrito (VentaID, ProductoID, Menu_Del_DiaID, Cantidad, precio_Unitario) VALUES (25, 5, NULL, 2, 2050.00);
INSERT INTO Carrito (VentaID, ProductoID, Menu_Del_DiaID, Cantidad, precio_Unitario) VALUES (26, 6, NULL, 3, 2100.00);
INSERT INTO Carrito (VentaID, ProductoID, Menu_Del_DiaID, Cantidad, precio_Unitario) VALUES (27, 7, NULL, 1, 2150.00);
INSERT INTO Carrito (VentaID, ProductoID, Menu_Del_DiaID, Cantidad, precio_Unitario) VALUES (28, 8, NULL, 2, 2200.00);
INSERT INTO Carrito (VentaID, ProductoID, Menu_Del_DiaID, Cantidad, precio_Unitario) VALUES (29, 9, NULL, 3, 2250.00);
INSERT INTO Carrito (VentaID, ProductoID, Menu_Del_DiaID, Cantidad, precio_Unitario) VALUES (30, 10, NULL, 1, 2300.00);
INSERT INTO Carrito (VentaID, ProductoID, Menu_Del_DiaID, Cantidad, precio_Unitario) VALUES (31, 11, NULL, 2, 2350.00);
INSERT INTO Carrito (VentaID, ProductoID, Menu_Del_DiaID, Cantidad, precio_Unitario) VALUES (32, 12, NULL, 3, 2400.00);
INSERT INTO Carrito (VentaID, ProductoID, Menu_Del_DiaID, Cantidad, precio_Unitario) VALUES (33, 13, NULL, 1, 2450.00);
INSERT INTO Carrito (VentaID, ProductoID, Menu_Del_DiaID, Cantidad, precio_Unitario) VALUES (34, 14, NULL, 2, 2500.00);
INSERT INTO Carrito (VentaID, ProductoID, Menu_Del_DiaID, Cantidad, precio_Unitario) VALUES (35, 15, NULL, 3, 2550.00);
INSERT INTO Carrito (VentaID, ProductoID, Menu_Del_DiaID, Cantidad, precio_Unitario) VALUES (36, 16, NULL, 1, 2600.00);
INSERT INTO Carrito (VentaID, ProductoID, Menu_Del_DiaID, Cantidad, precio_Unitario) VALUES (37, 17, NULL, 2, 2650.00);
INSERT INTO Carrito (VentaID, ProductoID, Menu_Del_DiaID, Cantidad, precio_Unitario) VALUES (38, 18, NULL, 3, 2700.00);
INSERT INTO Carrito (VentaID, ProductoID, Menu_Del_DiaID, Cantidad, precio_Unitario) VALUES (39, 19, NULL, 1, 2750.00);
INSERT INTO Carrito (VentaID, ProductoID, Menu_Del_DiaID, Cantidad, precio_Unitario) VALUES (40, 20, NULL, 2, 2800.00);
INSERT INTO Carrito (VentaID, ProductoID, Menu_Del_DiaID, Cantidad, precio_Unitario) VALUES (41, 1, NULL, 3, 2850.00);
INSERT INTO Carrito (VentaID, ProductoID, Menu_Del_DiaID, Cantidad, precio_Unitario) VALUES (42, 2, NULL, 1, 2900.00);
INSERT INTO Carrito (VentaID, ProductoID, Menu_Del_DiaID, Cantidad, precio_Unitario) VALUES (43, 3, NULL, 2, 2950.00);
INSERT INTO Carrito (VentaID, ProductoID, Menu_Del_DiaID, Cantidad, precio_Unitario) VALUES (44, 4, NULL, 3, 3000.00);
INSERT INTO Carrito (VentaID, ProductoID, Menu_Del_DiaID, Cantidad, precio_Unitario) VALUES (45, 5, NULL, 1, 3050.00);
INSERT INTO Carrito (VentaID, ProductoID, Menu_Del_DiaID, Cantidad, precio_Unitario) VALUES (46, 6, NULL, 2, 3100.00);
INSERT INTO Carrito (VentaID, ProductoID, Menu_Del_DiaID, Cantidad, precio_Unitario) VALUES (47, 7, NULL, 3, 3150.00);
INSERT INTO Carrito (VentaID, ProductoID, Menu_Del_DiaID, Cantidad, precio_Unitario) VALUES (48, 8, NULL, 1, 3200.00);
INSERT INTO Carrito (VentaID, ProductoID, Menu_Del_DiaID, Cantidad, precio_Unitario) VALUES (49, 9, NULL, 2, 3250.00);
INSERT INTO Carrito (VentaID, ProductoID, Menu_Del_DiaID, Cantidad, precio_Unitario) VALUES (50, 10, NULL, 3, 3300.00);

-- Tabla Retiro (50 registros - para las 50 ventas)
INSERT INTO Retiro (VentaID, EstadoID, Tiempo_Retiro, Codigo_Retiro) VALUES (1, 1, 'Recreo 1 (11:15 hs)', 'COD001X');
INSERT INTO Retiro (VentaID, EstadoID, Tiempo_Retiro, Codigo_Retiro) VALUES (2, 2, 'Recreo 2 (12:15 hs)', 'COD002X');
INSERT INTO Retiro (VentaID, EstadoID, Tiempo_Retiro, Codigo_Retiro) VALUES (3, 3, 'Recreo 3 (10:15 hs)', 'COD003X');
INSERT INTO Retiro (VentaID, EstadoID, Tiempo_Retiro, Codigo_Retiro) VALUES (4, 4, 'Recreo 1 (11:15 hs)', 'COD004X');
INSERT INTO Retiro (VentaID, EstadoID, Tiempo_Retiro, Codigo_Retiro) VALUES (5, 5, 'Recreo 2 (12:15 hs)', 'COD005X');
INSERT INTO Retiro (VentaID, EstadoID, Tiempo_Retiro, Codigo_Retiro) VALUES (6, 6, 'Recreo 3 (10:15 hs)', 'COD006X');
INSERT INTO Retiro (VentaID, EstadoID, Tiempo_Retiro, Codigo_Retiro) VALUES (7, 7, 'Recreo 1 (11:15 hs)', 'COD007X');
INSERT INTO Retiro (VentaID, EstadoID, Tiempo_Retiro, Codigo_Retiro) VALUES (8, 8, 'Recreo 2 (12:15 hs)', 'COD008X');
INSERT INTO Retiro (VentaID, EstadoID, Tiempo_Retiro, Codigo_Retiro) VALUES (9, 9, 'Recreo 3 (10:15 hs)', 'COD009X');
INSERT INTO Retiro (VentaID, EstadoID, Tiempo_Retiro, Codigo_Retiro) VALUES (10, 10, 'Recreo 1 (11:15 hs)', 'COD010X');
INSERT INTO Retiro (VentaID, EstadoID, Tiempo_Retiro, Codigo_Retiro) VALUES (11, 11, 'Recreo 2 (12:15 hs)', 'COD011X');
INSERT INTO Retiro (VentaID, EstadoID, Tiempo_Retiro, Codigo_Retiro) VALUES (12, 12, 'Recreo 3 (10:15 hs)', 'COD012X');
INSERT INTO Retiro (VentaID, EstadoID, Tiempo_Retiro, Codigo_Retiro) VALUES (13, 13, 'Recreo 1 (11:15 hs)', 'COD013X');
INSERT INTO Retiro (VentaID, EstadoID, Tiempo_Retiro, Codigo_Retiro) VALUES (14, 14, 'Recreo 2 (12:15 hs)', 'COD014X');
INSERT INTO Retiro (VentaID, EstadoID, Tiempo_Retiro, Codigo_Retiro) VALUES (15, 15, 'Recreo 3 (10:15 hs)', 'COD015X');
INSERT INTO Retiro (VentaID, EstadoID, Tiempo_Retiro, Codigo_Retiro) VALUES (16, 16, 'Recreo 1 (11:15 hs)', 'COD016X');
INSERT INTO Retiro (VentaID, EstadoID, Tiempo_Retiro, Codigo_Retiro) VALUES (17, 17, 'Recreo 2 (12:15 hs)', 'COD017X');
INSERT INTO Retiro (VentaID, EstadoID, Tiempo_Retiro, Codigo_Retiro) VALUES (18, 18, 'Recreo 3 (10:15 hs)', 'COD018X');
INSERT INTO Retiro (VentaID, EstadoID, Tiempo_Retiro, Codigo_Retiro) VALUES (19, 19, 'Recreo 1 (11:15 hs)', 'COD019X');
INSERT INTO Retiro (VentaID, EstadoID, Tiempo_Retiro, Codigo_Retiro) VALUES (20, 20, 'Recreo 2 (12:15 hs)', 'COD020X');
INSERT INTO Retiro (VentaID, EstadoID, Tiempo_Retiro, Codigo_Retiro) VALUES (21, 1, 'Recreo 3 (10:15 hs)', 'COD021X');
INSERT INTO Retiro (VentaID, EstadoID, Tiempo_Retiro, Codigo_Retiro) VALUES (22, 2, 'Recreo 1 (11:15 hs)', 'COD022X');
INSERT INTO Retiro (VentaID, EstadoID, Tiempo_Retiro, Codigo_Retiro) VALUES (23, 3, 'Recreo 2 (12:15 hs)', 'COD023X');
INSERT INTO Retiro (VentaID, EstadoID, Tiempo_Retiro, Codigo_Retiro) VALUES (24, 4, 'Recreo 3 (10:15 hs)', 'COD024X');
INSERT INTO Retiro (VentaID, EstadoID, Tiempo_Retiro, Codigo_Retiro) VALUES (25, 5, 'Recreo 1 (11:15 hs)', 'COD025X');
INSERT INTO Retiro (VentaID, EstadoID, Tiempo_Retiro, Codigo_Retiro) VALUES (26, 6, 'Recreo 2 (12:15 hs)', 'COD026X');
INSERT INTO Retiro (VentaID, EstadoID, Tiempo_Retiro, Codigo_Retiro) VALUES (27, 7, 'Recreo 3 (10:15 hs)', 'COD027X');
INSERT INTO Retiro (VentaID, EstadoID, Tiempo_Retiro, Codigo_Retiro) VALUES (28, 8, 'Recreo 1 (11:15 hs)', 'COD028X');
INSERT INTO Retiro (VentaID, EstadoID, Tiempo_Retiro, Codigo_Retiro) VALUES (29, 9, 'Recreo 2 (12:15 hs)', 'COD029X');
INSERT INTO Retiro (VentaID, EstadoID, Tiempo_Retiro, Codigo_Retiro) VALUES (30, 10, 'Recreo 3 (10:15 hs)', 'COD030X');
INSERT INTO Retiro (VentaID, EstadoID, Tiempo_Retiro, Codigo_Retiro) VALUES (31, 11, 'Recreo 1 (11:15 hs)', 'COD031X');
INSERT INTO Retiro (VentaID, EstadoID, Tiempo_Retiro, Codigo_Retiro) VALUES (32, 12, 'Recreo 2 (12:15 hs)', 'COD032X');
INSERT INTO Retiro (VentaID, EstadoID, Tiempo_Retiro, Codigo_Retiro) VALUES (33, 13, 'Recreo 3 (10:15 hs)', 'COD033X');
INSERT INTO Retiro (VentaID, EstadoID, Tiempo_Retiro, Codigo_Retiro) VALUES (34, 14, 'Recreo 1 (11:15 hs)', 'COD034X');
INSERT INTO Retiro (VentaID, EstadoID, Tiempo_Retiro, Codigo_Retiro) VALUES (35, 15, 'Recreo 2 (12:15 hs)', 'COD035X');
INSERT INTO Retiro (VentaID, EstadoID, Tiempo_Retiro, Codigo_Retiro) VALUES (36, 16, 'Recreo 3 (10:15 hs)', 'COD036X');
INSERT INTO Retiro (VentaID, EstadoID, Tiempo_Retiro, Codigo_Retiro) VALUES (37, 17, 'Recreo 1 (11:15 hs)', 'COD037X');
INSERT INTO Retiro (VentaID, EstadoID, Tiempo_Retiro, Codigo_Retiro) VALUES (38, 18, 'Recreo 2 (12:15 hs)', 'COD038X');
INSERT INTO Retiro (VentaID, EstadoID, Tiempo_Retiro, Codigo_Retiro) VALUES (39, 19, 'Recreo 3 (10:15 hs)', 'COD039X');
INSERT INTO Retiro (VentaID, EstadoID, Tiempo_Retiro, Codigo_Retiro) VALUES (40, 20, 'Recreo 1 (11:15 hs)', 'COD040X');
INSERT INTO Retiro (VentaID, EstadoID, Tiempo_Retiro, Codigo_Retiro) VALUES (41, 1, 'Recreo 2 (12:15 hs)', 'COD041X');
INSERT INTO Retiro (VentaID, EstadoID, Tiempo_Retiro, Codigo_Retiro) VALUES (42, 2, 'Recreo 3 (10:15 hs)', 'COD042X');
INSERT INTO Retiro (VentaID, EstadoID, Tiempo_Retiro, Codigo_Retiro) VALUES (43, 3, 'Recreo 1 (11:15 hs)', 'COD043X');
INSERT INTO Retiro (VentaID, EstadoID, Tiempo_Retiro, Codigo_Retiro) VALUES (44, 4, 'Recreo 2 (12:15 hs)', 'COD044X');
INSERT INTO Retiro (VentaID, EstadoID, Tiempo_Retiro, Codigo_Retiro) VALUES (45, 5, 'Recreo 3 (10:15 hs)', 'COD045X');
INSERT INTO Retiro (VentaID, EstadoID, Tiempo_Retiro, Codigo_Retiro) VALUES (46, 6, 'Recreo 1 (11:15 hs)', 'COD046X');
INSERT INTO Retiro (VentaID, EstadoID, Tiempo_Retiro, Codigo_Retiro) VALUES (47, 7, 'Recreo 2 (12:15 hs)', 'COD047X');
INSERT INTO Retiro (VentaID, EstadoID, Tiempo_Retiro, Codigo_Retiro) VALUES (48, 8, 'Recreo 3 (10:15 hs)', 'COD048X');
INSERT INTO Retiro (VentaID, EstadoID, Tiempo_Retiro, Codigo_Retiro) VALUES (49, 9, 'Recreo 1 (11:15 hs)', 'COD049X');
INSERT INTO Retiro (VentaID, EstadoID, Tiempo_Retiro, Codigo_Retiro) VALUES (50, 10, 'Recreo 2 (12:15 hs)', 'COD050X');

SET FOREIGN_KEY_CHECKS = 1;