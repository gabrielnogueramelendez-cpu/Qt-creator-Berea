-- ============================================================================
-- PROYECTO INFORMÁTICO I - SECCIÓN BASES DE DATOS
-- El Buffet de Alejandro - Escuela Técnica N° 3 D.E. 9 "María Sánchez de Thompson"
-- PUNTO 4: POBLAR EL MODELO CON DATOS
--
-- Ejecutar DESPUÉS de 03_ModeloSQL. Se puede ejecutar varias veces:
-- vacía cada tabla con TRUNCATE (que además reinicia los AUTO_INCREMENT) y la vuelve a cargar.
-- Venta (tabla principal) tiene 50 registros; el resto, 20 o más.
-- Log_Auditoria NO se carga acá: la completan los Stored Procedures (consigna 4.c).
-- ============================================================================

SET NAMES utf8mb4;  -- para que las tildes y la ñ se guarden bien desde cualquier cliente
SET FOREIGN_KEY_CHECKS = 0;

TRUNCATE TABLE schema_comercial.Retiro;
TRUNCATE TABLE schema_comercial.Carrito;
TRUNCATE TABLE schema_comercial.Detalle_Menu_Dia;
TRUNCATE TABLE schema_comercial.Venta;
TRUNCATE TABLE schema_comercial.Menu_Del_Dia;
TRUNCATE TABLE schema_comercial.Producto;
TRUNCATE TABLE schema_comercial.Franja_Retiro;
TRUNCATE TABLE schema_comercial.Estado_Retiro;
TRUNCATE TABLE schema_comercial.Metodo_Pago;
TRUNCATE TABLE schema_comercial.Categoria;
TRUNCATE TABLE schema_usuarios.Log_Auditoria;
TRUNCATE TABLE schema_usuarios.Telefono;
TRUNCATE TABLE schema_usuarios.Email;
TRUNCATE TABLE schema_usuarios.Empleado;
TRUNCATE TABLE schema_usuarios.Usuario;
TRUNCATE TABLE schema_usuarios.Estudiante;
TRUNCATE TABLE schema_usuarios.Tipo_Evento;
TRUNCATE TABLE schema_usuarios.Puesto;
TRUNCATE TABLE schema_usuarios.Curso;
TRUNCATE TABLE schema_geografia.Colegio;
TRUNCATE TABLE schema_geografia.Distrito_Escolar;
TRUNCATE TABLE schema_geografia.Region;
TRUNCATE TABLE schema_geografia.Ciudad;
TRUNCATE TABLE schema_geografia.Provincia;
TRUNCATE TABLE schema_geografia.Pais;

-- ============================================================================
-- SCHEMA GEOGRAFÍA
-- ============================================================================
USE schema_geografia;

-- ----------------------------------------------------------------------------
-- Pais (20)
-- ----------------------------------------------------------------------------
INSERT INTO Pais (Nombre_Pais) VALUES ('Argentina');
INSERT INTO Pais (Nombre_Pais) VALUES ('Uruguay');
INSERT INTO Pais (Nombre_Pais) VALUES ('Paraguay');
INSERT INTO Pais (Nombre_Pais) VALUES ('Brasil');
INSERT INTO Pais (Nombre_Pais) VALUES ('Chile');
INSERT INTO Pais (Nombre_Pais) VALUES ('Bolivia');
INSERT INTO Pais (Nombre_Pais) VALUES ('Perú');
INSERT INTO Pais (Nombre_Pais) VALUES ('Ecuador');
INSERT INTO Pais (Nombre_Pais) VALUES ('Colombia');
INSERT INTO Pais (Nombre_Pais) VALUES ('Venezuela');
INSERT INTO Pais (Nombre_Pais) VALUES ('Panamá');
INSERT INTO Pais (Nombre_Pais) VALUES ('Costa Rica');
INSERT INTO Pais (Nombre_Pais) VALUES ('México');
INSERT INTO Pais (Nombre_Pais) VALUES ('Cuba');
INSERT INTO Pais (Nombre_Pais) VALUES ('República Dominicana');
INSERT INTO Pais (Nombre_Pais) VALUES ('Guatemala');
INSERT INTO Pais (Nombre_Pais) VALUES ('España');
INSERT INTO Pais (Nombre_Pais) VALUES ('Italia');
INSERT INTO Pais (Nombre_Pais) VALUES ('Estados Unidos');
INSERT INTO Pais (Nombre_Pais) VALUES ('Francia');

-- ----------------------------------------------------------------------------
-- Provincia (20 - todas de Argentina, ID_Pais = 1)
-- ----------------------------------------------------------------------------
INSERT INTO Provincia (ID_Pais, Nombre_Provincia) VALUES (1, 'Ciudad Autónoma de Buenos Aires');
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
INSERT INTO Provincia (ID_Pais, Nombre_Provincia) VALUES (1, 'Santa Fe');

-- ----------------------------------------------------------------------------
-- Ciudad (20 - una capital por provincia)
-- ----------------------------------------------------------------------------
INSERT INTO Ciudad (ID_Provincia, Nombre_Ciudad) VALUES (1, 'Ciudad Autónoma de Buenos Aires');
INSERT INTO Ciudad (ID_Provincia, Nombre_Ciudad) VALUES (2, 'La Plata');
INSERT INTO Ciudad (ID_Provincia, Nombre_Ciudad) VALUES (3, 'San Fernando del Valle de Catamarca');
INSERT INTO Ciudad (ID_Provincia, Nombre_Ciudad) VALUES (4, 'Resistencia');
INSERT INTO Ciudad (ID_Provincia, Nombre_Ciudad) VALUES (5, 'Rawson');
INSERT INTO Ciudad (ID_Provincia, Nombre_Ciudad) VALUES (6, 'Córdoba');
INSERT INTO Ciudad (ID_Provincia, Nombre_Ciudad) VALUES (7, 'Corrientes');
INSERT INTO Ciudad (ID_Provincia, Nombre_Ciudad) VALUES (8, 'Paraná');
INSERT INTO Ciudad (ID_Provincia, Nombre_Ciudad) VALUES (9, 'Formosa');
INSERT INTO Ciudad (ID_Provincia, Nombre_Ciudad) VALUES (10, 'San Salvador de Jujuy');
INSERT INTO Ciudad (ID_Provincia, Nombre_Ciudad) VALUES (11, 'Santa Rosa');
INSERT INTO Ciudad (ID_Provincia, Nombre_Ciudad) VALUES (12, 'La Rioja');
INSERT INTO Ciudad (ID_Provincia, Nombre_Ciudad) VALUES (13, 'Mendoza');
INSERT INTO Ciudad (ID_Provincia, Nombre_Ciudad) VALUES (14, 'Posadas');
INSERT INTO Ciudad (ID_Provincia, Nombre_Ciudad) VALUES (15, 'Neuquén');
INSERT INTO Ciudad (ID_Provincia, Nombre_Ciudad) VALUES (16, 'Viedma');
INSERT INTO Ciudad (ID_Provincia, Nombre_Ciudad) VALUES (17, 'Salta');
INSERT INTO Ciudad (ID_Provincia, Nombre_Ciudad) VALUES (18, 'San Juan');
INSERT INTO Ciudad (ID_Provincia, Nombre_Ciudad) VALUES (19, 'San Luis');
INSERT INTO Ciudad (ID_Provincia, Nombre_Ciudad) VALUES (20, 'Santa Fe');

-- ----------------------------------------------------------------------------
-- Region (20 - las 9 regiones educativas de CABA + regiones de otras ciudades)
-- ----------------------------------------------------------------------------
INSERT INTO Region (ID_Ciudad, Nombre_Region) VALUES (1, 'Región Educativa I');
INSERT INTO Region (ID_Ciudad, Nombre_Region) VALUES (1, 'Región Educativa II');
INSERT INTO Region (ID_Ciudad, Nombre_Region) VALUES (1, 'Región Educativa III');
INSERT INTO Region (ID_Ciudad, Nombre_Region) VALUES (1, 'Región Educativa IV');
INSERT INTO Region (ID_Ciudad, Nombre_Region) VALUES (1, 'Región Educativa V');
INSERT INTO Region (ID_Ciudad, Nombre_Region) VALUES (1, 'Región Educativa VI');
INSERT INTO Region (ID_Ciudad, Nombre_Region) VALUES (1, 'Región Educativa VII');
INSERT INTO Region (ID_Ciudad, Nombre_Region) VALUES (1, 'Región Educativa VIII');
INSERT INTO Region (ID_Ciudad, Nombre_Region) VALUES (1, 'Región Educativa IX');
INSERT INTO Region (ID_Ciudad, Nombre_Region) VALUES (2, 'Región Educativa 1 - La Plata');
INSERT INTO Region (ID_Ciudad, Nombre_Region) VALUES (3, 'Región Educativa Catamarca Capital');
INSERT INTO Region (ID_Ciudad, Nombre_Region) VALUES (4, 'Región Educativa I - Resistencia');
INSERT INTO Region (ID_Ciudad, Nombre_Region) VALUES (5, 'Región Educativa Rawson');
INSERT INTO Region (ID_Ciudad, Nombre_Region) VALUES (6, 'Región Educativa Córdoba Capital');
INSERT INTO Region (ID_Ciudad, Nombre_Region) VALUES (7, 'Región Educativa I - Corrientes');
INSERT INTO Region (ID_Ciudad, Nombre_Region) VALUES (8, 'Región Educativa Paraná');
INSERT INTO Region (ID_Ciudad, Nombre_Region) VALUES (9, 'Región Educativa Formosa Capital');
INSERT INTO Region (ID_Ciudad, Nombre_Region) VALUES (10, 'Región Educativa San Salvador de Jujuy');
INSERT INTO Region (ID_Ciudad, Nombre_Region) VALUES (11, 'Región Educativa Santa Rosa');
INSERT INTO Region (ID_Ciudad, Nombre_Region) VALUES (13, 'Región Educativa Mendoza Capital');

-- ----------------------------------------------------------------------------
-- Distrito_Escolar (20 distritos escolares de CABA)
-- ----------------------------------------------------------------------------
INSERT INTO Distrito_Escolar (ID_Region, Jurisdiccion) VALUES (1, 'Distrito Escolar N° 1');
INSERT INTO Distrito_Escolar (ID_Region, Jurisdiccion) VALUES (1, 'Distrito Escolar N° 2');
INSERT INTO Distrito_Escolar (ID_Region, Jurisdiccion) VALUES (2, 'Distrito Escolar N° 3');
INSERT INTO Distrito_Escolar (ID_Region, Jurisdiccion) VALUES (2, 'Distrito Escolar N° 4');
INSERT INTO Distrito_Escolar (ID_Region, Jurisdiccion) VALUES (3, 'Distrito Escolar N° 5');
INSERT INTO Distrito_Escolar (ID_Region, Jurisdiccion) VALUES (3, 'Distrito Escolar N° 6');
INSERT INTO Distrito_Escolar (ID_Region, Jurisdiccion) VALUES (4, 'Distrito Escolar N° 7');
INSERT INTO Distrito_Escolar (ID_Region, Jurisdiccion) VALUES (4, 'Distrito Escolar N° 8');
INSERT INTO Distrito_Escolar (ID_Region, Jurisdiccion) VALUES (5, 'Distrito Escolar N° 9');
INSERT INTO Distrito_Escolar (ID_Region, Jurisdiccion) VALUES (5, 'Distrito Escolar N° 10');
INSERT INTO Distrito_Escolar (ID_Region, Jurisdiccion) VALUES (6, 'Distrito Escolar N° 11');
INSERT INTO Distrito_Escolar (ID_Region, Jurisdiccion) VALUES (6, 'Distrito Escolar N° 12');
INSERT INTO Distrito_Escolar (ID_Region, Jurisdiccion) VALUES (7, 'Distrito Escolar N° 13');
INSERT INTO Distrito_Escolar (ID_Region, Jurisdiccion) VALUES (7, 'Distrito Escolar N° 14');
INSERT INTO Distrito_Escolar (ID_Region, Jurisdiccion) VALUES (8, 'Distrito Escolar N° 15');
INSERT INTO Distrito_Escolar (ID_Region, Jurisdiccion) VALUES (8, 'Distrito Escolar N° 16');
INSERT INTO Distrito_Escolar (ID_Region, Jurisdiccion) VALUES (9, 'Distrito Escolar N° 17');
INSERT INTO Distrito_Escolar (ID_Region, Jurisdiccion) VALUES (9, 'Distrito Escolar N° 18');
INSERT INTO Distrito_Escolar (ID_Region, Jurisdiccion) VALUES (9, 'Distrito Escolar N° 19');
INSERT INTO Distrito_Escolar (ID_Region, Jurisdiccion) VALUES (9, 'Distrito Escolar N° 20');

-- ----------------------------------------------------------------------------
-- Colegio (20 escuelas técnicas de CABA) - ID_Colegio 1 = ET N° 3 D.E. 9
-- ----------------------------------------------------------------------------
INSERT INTO Colegio (ID_DE, Nombre_Colegio, Direccion) VALUES (9, 'Escuela Técnica N° 3 D.E. 9 "María Sánchez de Thompson"', 'Av. Cabildo 40, Palermo');
INSERT INTO Colegio (ID_DE, Nombre_Colegio, Direccion) VALUES (1, 'Escuela Técnica N° 12 D.E. 1 "Libertador Gral. José de San Martín"', 'Av. del Libertador 238, Retiro');
INSERT INTO Colegio (ID_DE, Nombre_Colegio, Direccion) VALUES (2, 'Escuela Técnica N° 2 D.E. 2 "Osvaldo Magnasco"', 'Av. Córdoba 1950, Balvanera');
INSERT INTO Colegio (ID_DE, Nombre_Colegio, Direccion) VALUES (3, 'Escuela Técnica N° 25 D.E. 3 "Teniente Primero de Artillería Luis A. Febre"', 'Av. Jujuy 255, Balvanera');
INSERT INTO Colegio (ID_DE, Nombre_Colegio, Direccion) VALUES (4, 'Escuela Técnica N° 1 D.E. 4 "Otto Krause"', 'Av. Paseo Colón 650, San Telmo');
INSERT INTO Colegio (ID_DE, Nombre_Colegio, Direccion) VALUES (5, 'Escuela Técnica N° 4 D.E. 5 "República del Líbano"', 'Av. Regimiento de Patricios 1933, Barracas');
INSERT INTO Colegio (ID_DE, Nombre_Colegio, Direccion) VALUES (6, 'Escuela Técnica N° 11 D.E. 6 "Manuel Belgrano"', 'Cochabamba 2830, San Cristóbal');
INSERT INTO Colegio (ID_DE, Nombre_Colegio, Direccion) VALUES (7, 'Escuela Técnica N° 9 D.E. 7 "Ing. Luis A. Huergo"', 'Martín de Gainza 1050, Caballito');
INSERT INTO Colegio (ID_DE, Nombre_Colegio, Direccion) VALUES (8, 'Escuela Técnica N° 19 D.E. 8 "Alejandro Volta"', 'Av. José María Moreno 480, Caballito');
INSERT INTO Colegio (ID_DE, Nombre_Colegio, Direccion) VALUES (10, 'Escuela Técnica N° 28 D.E. 10 "República Francesa"', 'Cuba 2410, Belgrano');
INSERT INTO Colegio (ID_DE, Nombre_Colegio, Direccion) VALUES (11, 'Escuela Técnica N° 37 D.E. 11 "Hogar Naval Stella Maris"', 'Av. Pedro Goyena 1480, Caballito');
INSERT INTO Colegio (ID_DE, Nombre_Colegio, Direccion) VALUES (12, 'Escuela Técnica N° 8 D.E. 12 "Paula Albarracín de Sarmiento"', 'Pío Collivadino 436, Flores');
INSERT INTO Colegio (ID_DE, Nombre_Colegio, Direccion) VALUES (13, 'Escuela Técnica N° 17 D.E. 13 "Brig. Gral. Cornelio Saavedra"', 'Av. Lacarra 535, Parque Avellaneda');
INSERT INTO Colegio (ID_DE, Nombre_Colegio, Direccion) VALUES (14, 'Escuela Técnica N° 32 D.E. 14 "Gral. José de San Martín"', 'Teodoro García 3899, Chacarita');
INSERT INTO Colegio (ID_DE, Nombre_Colegio, Direccion) VALUES (15, 'Escuela Técnica N° 36 D.E. 15 "Almirante Guillermo Brown"', 'Galván 3710, Saavedra');
INSERT INTO Colegio (ID_DE, Nombre_Colegio, Direccion) VALUES (16, 'Escuela Técnica N° 33 D.E. 16 "Fundación A.M.I.A."', 'Av. Alvarez Thomas 3391, Villa Urquiza');
INSERT INTO Colegio (ID_DE, Nombre_Colegio, Direccion) VALUES (17, 'Escuela Técnica N° 24 D.E. 17 "Héctor Lorenzi"', 'Elpidio González 5120, Villa del Parque');
INSERT INTO Colegio (ID_DE, Nombre_Colegio, Direccion) VALUES (18, 'Escuela Técnica N° 35 D.E. 18 "Ing. Eduardo Latzina"', 'Av. Lope de Vega 2150, Monte Castro');
INSERT INTO Colegio (ID_DE, Nombre_Colegio, Direccion) VALUES (19, 'Escuela Técnica N° 7 D.E. 19 "Dolores Lavalle de Lavalle"', 'Zavaleta 204, Nueva Pompeya');
INSERT INTO Colegio (ID_DE, Nombre_Colegio, Direccion) VALUES (20, 'Escuela Técnica N° 15 D.E. 20 "Maestro José Antonio Balseiro"', 'Av. Lisandro de la Torre 1185, Liniers');

-- ============================================================================
-- SCHEMA USUARIOS
-- ============================================================================
USE schema_usuarios;

-- ----------------------------------------------------------------------------
-- Curso (24 - de 1° a 6° año, divisiones 1ra a 4ta)
-- ----------------------------------------------------------------------------
INSERT INTO Curso (Anio, Division) VALUES (1, '1ra');
INSERT INTO Curso (Anio, Division) VALUES (1, '2da');
INSERT INTO Curso (Anio, Division) VALUES (1, '3ra');
INSERT INTO Curso (Anio, Division) VALUES (1, '4ta');
INSERT INTO Curso (Anio, Division) VALUES (2, '1ra');
INSERT INTO Curso (Anio, Division) VALUES (2, '2da');
INSERT INTO Curso (Anio, Division) VALUES (2, '3ra');
INSERT INTO Curso (Anio, Division) VALUES (2, '4ta');
INSERT INTO Curso (Anio, Division) VALUES (3, '1ra');
INSERT INTO Curso (Anio, Division) VALUES (3, '2da');
INSERT INTO Curso (Anio, Division) VALUES (3, '3ra');
INSERT INTO Curso (Anio, Division) VALUES (3, '4ta');
INSERT INTO Curso (Anio, Division) VALUES (4, '1ra');
INSERT INTO Curso (Anio, Division) VALUES (4, '2da');
INSERT INTO Curso (Anio, Division) VALUES (4, '3ra');
INSERT INTO Curso (Anio, Division) VALUES (4, '4ta');
INSERT INTO Curso (Anio, Division) VALUES (5, '1ra');
INSERT INTO Curso (Anio, Division) VALUES (5, '2da');
INSERT INTO Curso (Anio, Division) VALUES (5, '3ra');
INSERT INTO Curso (Anio, Division) VALUES (5, '4ta');
INSERT INTO Curso (Anio, Division) VALUES (6, '1ra');
INSERT INTO Curso (Anio, Division) VALUES (6, '2da');
INSERT INTO Curso (Anio, Division) VALUES (6, '3ra');
INSERT INTO Curso (Anio, Division) VALUES (6, '4ta');

-- ----------------------------------------------------------------------------
-- Puesto (20)
-- ----------------------------------------------------------------------------
INSERT INTO Puesto (Nombre_Puesto) VALUES ('Cajera Turno Mañana');
INSERT INTO Puesto (Nombre_Puesto) VALUES ('Cajero Turno Mañana');
INSERT INTO Puesto (Nombre_Puesto) VALUES ('Cajero Turno Tarde');
INSERT INTO Puesto (Nombre_Puesto) VALUES ('Cajera Turno Tarde');
INSERT INTO Puesto (Nombre_Puesto) VALUES ('Encargado General de Buffet');
INSERT INTO Puesto (Nombre_Puesto) VALUES ('Cocinera');
INSERT INTO Puesto (Nombre_Puesto) VALUES ('Cocinero');
INSERT INTO Puesto (Nombre_Puesto) VALUES ('Ayudante de Cocina');
INSERT INTO Puesto (Nombre_Puesto) VALUES ('Atención al Mostrador Turno Mañana');
INSERT INTO Puesto (Nombre_Puesto) VALUES ('Atención al Mostrador Turno Tarde');
INSERT INTO Puesto (Nombre_Puesto) VALUES ('Repostera');
INSERT INTO Puesto (Nombre_Puesto) VALUES ('Repositor de Mercadería');
INSERT INTO Puesto (Nombre_Puesto) VALUES ('Responsable de Compras');
INSERT INTO Puesto (Nombre_Puesto) VALUES ('Mantenimiento');
INSERT INTO Puesto (Nombre_Puesto) VALUES ('Limpieza');
INSERT INTO Puesto (Nombre_Puesto) VALUES ('Auxiliar de Depósito');
INSERT INTO Puesto (Nombre_Puesto) VALUES ('Administrativa Contable');
INSERT INTO Puesto (Nombre_Puesto) VALUES ('Encargado de Turno Mañana');
INSERT INTO Puesto (Nombre_Puesto) VALUES ('Encargado de Turno Tarde');
INSERT INTO Puesto (Nombre_Puesto) VALUES ('Ayudante de Mostrador');

-- ----------------------------------------------------------------------------
-- Tipo_Evento (20) - catálogo de eventos de auditoría
-- ----------------------------------------------------------------------------
-- Los IDs 1 a 16 los usan los Stored Procedures de 05_Backend.
INSERT INTO Tipo_Evento (Nombre_Evento) VALUES ('Inicio de Sesión Exitoso');  -- 1
INSERT INTO Tipo_Evento (Nombre_Evento) VALUES ('Cierre de Sesión');  -- 2
INSERT INTO Tipo_Evento (Nombre_Evento) VALUES ('Fallo de Autenticación');  -- 3
INSERT INTO Tipo_Evento (Nombre_Evento) VALUES ('Registro de Nuevo Usuario');  -- 4
INSERT INTO Tipo_Evento (Nombre_Evento) VALUES ('Cambio de Contraseña');  -- 5
INSERT INTO Tipo_Evento (Nombre_Evento) VALUES ('Registro de Venta');  -- 6
INSERT INTO Tipo_Evento (Nombre_Evento) VALUES ('Agregado de Producto al Carrito');  -- 7
INSERT INTO Tipo_Evento (Nombre_Evento) VALUES ('Agregado de Menú al Carrito');  -- 8
INSERT INTO Tipo_Evento (Nombre_Evento) VALUES ('Generación de Código de Retiro');  -- 9
INSERT INTO Tipo_Evento (Nombre_Evento) VALUES ('Entrega de Pedido');  -- 10
INSERT INTO Tipo_Evento (Nombre_Evento) VALUES ('Cancelación de Pedido');  -- 11
INSERT INTO Tipo_Evento (Nombre_Evento) VALUES ('Cambio de Estado de Pedido');  -- 12
INSERT INTO Tipo_Evento (Nombre_Evento) VALUES ('Alta de Producto');  -- 13
INSERT INTO Tipo_Evento (Nombre_Evento) VALUES ('Baja de Producto');  -- 14
INSERT INTO Tipo_Evento (Nombre_Evento) VALUES ('Modificación de Stock de Producto');  -- 15
INSERT INTO Tipo_Evento (Nombre_Evento) VALUES ('Alta de Menú del Día');  -- 16
INSERT INTO Tipo_Evento (Nombre_Evento) VALUES ('Actualización de Perfil');  -- 17
INSERT INTO Tipo_Evento (Nombre_Evento) VALUES ('Apertura de Caja');  -- 18
INSERT INTO Tipo_Evento (Nombre_Evento) VALUES ('Cierre de Caja');  -- 19
INSERT INTO Tipo_Evento (Nombre_Evento) VALUES ('Exportación de Reporte');  -- 20

-- ----------------------------------------------------------------------------
-- Estudiante (20 - todos de la ET N° 3, ID_Colegio = 1)
-- ----------------------------------------------------------------------------
-- El curso sale de la edad: 13 años = 1° año ... 18 años = 6° año.
INSERT INTO Estudiante (DNI, CUIT, Nombre_Estudiante, Apellido_Estudiante, Fecha_Nacimiento, ID_Colegio, ID_Curso) VALUES ('12345678', '20-12345678-3', 'Jorge', 'Dahan', '2008-08-21', 1, 21);
INSERT INTO Estudiante (DNI, CUIT, Nombre_Estudiante, Apellido_Estudiante, Fecha_Nacimiento, ID_Colegio, ID_Curso) VALUES ('45987654', '27-45987654-0', 'Lucía', 'Gómez', '2008-11-03', 1, 22);
INSERT INTO Estudiante (DNI, CUIT, Nombre_Estudiante, Apellido_Estudiante, Fecha_Nacimiento, ID_Colegio, ID_Curso) VALUES ('48499250', '23-48499250-9', 'Mateo', 'Fernández', '2011-09-30', 1, 11);
INSERT INTO Estudiante (DNI, CUIT, Nombre_Estudiante, Apellido_Estudiante, Fecha_Nacimiento, ID_Colegio, ID_Curso) VALUES ('51715407', '27-51715407-0', 'Valentina', 'Rodríguez', '2013-11-26', 1, 4);
INSERT INTO Estudiante (DNI, CUIT, Nombre_Estudiante, Apellido_Estudiante, Fecha_Nacimiento, ID_Colegio, ID_Curso) VALUES ('52929091', '20-52929091-9', 'Santiago', 'López', '2009-04-25', 1, 17);
INSERT INTO Estudiante (DNI, CUIT, Nombre_Estudiante, Apellido_Estudiante, Fecha_Nacimiento, ID_Colegio, ID_Curso) VALUES ('49373046', '27-49373046-6', 'Camila', 'Martínez', '2012-11-17', 1, 6);
INSERT INTO Estudiante (DNI, CUIT, Nombre_Estudiante, Apellido_Estudiante, Fecha_Nacimiento, ID_Colegio, ID_Curso) VALUES ('52303055', '20-52303055-9', 'Benjamín', 'García', '2013-09-01', 1, 3);
INSERT INTO Estudiante (DNI, CUIT, Nombre_Estudiante, Apellido_Estudiante, Fecha_Nacimiento, ID_Colegio, ID_Curso) VALUES ('52420093', '27-52420093-2', 'Martina', 'Pérez', '2013-02-09', 1, 4);
INSERT INTO Estudiante (DNI, CUIT, Nombre_Estudiante, Apellido_Estudiante, Fecha_Nacimiento, ID_Colegio, ID_Curso) VALUES ('49512712', '20-49512712-6', 'Thiago', 'Sánchez', '2008-03-11', 1, 21);
INSERT INTO Estudiante (DNI, CUIT, Nombre_Estudiante, Apellido_Estudiante, Fecha_Nacimiento, ID_Colegio, ID_Curso) VALUES ('52652318', '27-52652318-6', 'Sofía', 'Romero', '2009-01-25', 1, 18);
INSERT INTO Estudiante (DNI, CUIT, Nombre_Estudiante, Apellido_Estudiante, Fecha_Nacimiento, ID_Colegio, ID_Curso) VALUES ('48428925', '20-48428925-6', 'Joaquín', 'Díaz', '2011-05-21', 1, 11);
INSERT INTO Estudiante (DNI, CUIT, Nombre_Estudiante, Apellido_Estudiante, Fecha_Nacimiento, ID_Colegio, ID_Curso) VALUES ('48322516', '23-48322516-4', 'Julieta', 'Álvarez', '2013-03-16', 1, 4);
INSERT INTO Estudiante (DNI, CUIT, Nombre_Estudiante, Apellido_Estudiante, Fecha_Nacimiento, ID_Colegio, ID_Curso) VALUES ('47596375', '20-47596375-0', 'Bautista', 'Torres', '2013-08-30', 1, 1);
INSERT INTO Estudiante (DNI, CUIT, Nombre_Estudiante, Apellido_Estudiante, Fecha_Nacimiento, ID_Colegio, ID_Curso) VALUES ('53199127', '27-53199127-9', 'Agustina', 'Ruiz', '2011-09-09', 1, 10);
INSERT INTO Estudiante (DNI, CUIT, Nombre_Estudiante, Apellido_Estudiante, Fecha_Nacimiento, ID_Colegio, ID_Curso) VALUES ('49263492', '20-49263492-2', 'Lautaro', 'Acosta', '2012-08-14', 1, 7);
INSERT INTO Estudiante (DNI, CUIT, Nombre_Estudiante, Apellido_Estudiante, Fecha_Nacimiento, ID_Colegio, ID_Curso) VALUES ('49609844', '27-49609844-2', 'Micaela', 'Benítez', '2012-01-24', 1, 8);
INSERT INTO Estudiante (DNI, CUIT, Nombre_Estudiante, Apellido_Estudiante, Fecha_Nacimiento, ID_Colegio, ID_Curso) VALUES ('50491680', '20-50491680-5', 'Facundo', 'Medina', '2012-05-20', 1, 5);
INSERT INTO Estudiante (DNI, CUIT, Nombre_Estudiante, Apellido_Estudiante, Fecha_Nacimiento, ID_Colegio, ID_Curso) VALUES ('51808284', '27-51808284-7', 'Florencia', 'Herrera', '2009-01-07', 1, 18);
INSERT INTO Estudiante (DNI, CUIT, Nombre_Estudiante, Apellido_Estudiante, Fecha_Nacimiento, ID_Colegio, ID_Curso) VALUES ('50355436', '20-50355436-5', 'Nicolás', 'Suárez', '2009-03-03', 1, 19);
INSERT INTO Estudiante (DNI, CUIT, Nombre_Estudiante, Apellido_Estudiante, Fecha_Nacimiento, ID_Colegio, ID_Curso) VALUES ('52176234', '27-52176234-4', 'Rocío', 'Aguirre', '2011-06-10', 1, 12);

-- ----------------------------------------------------------------------------
-- Usuario (30) - 1 a 20: estudiantes / 21 a 30: personal del buffet
-- ----------------------------------------------------------------------------
-- La contraseña se guarda cifrada con SHA2(texto, 256).
-- Las contraseñas en texto plano quedan solo acá, para poder hacer las pruebas.
INSERT INTO Usuario (DNI, Apodo, Contrasena) VALUES ('12345678', 'jorgedah09', SHA2('Jorgedah#2026', 256));
INSERT INTO Usuario (DNI, Apodo, Contrasena) VALUES ('45987654', 'luciagom08', SHA2('Luciagom#2026', 256));
INSERT INTO Usuario (DNI, Apodo, Contrasena) VALUES ('48499250', 'mateofer11', SHA2('Mateofer#2026', 256));
INSERT INTO Usuario (DNI, Apodo, Contrasena) VALUES ('51715407', 'valentinarod13', SHA2('Valentinarod#2026', 256));
INSERT INTO Usuario (DNI, Apodo, Contrasena) VALUES ('52929091', 'santiagolop09', SHA2('Santiagolop#2026', 256));
INSERT INTO Usuario (DNI, Apodo, Contrasena) VALUES ('49373046', 'camilamar12', SHA2('Camilamar#2026', 256));
INSERT INTO Usuario (DNI, Apodo, Contrasena) VALUES ('52303055', 'benjamingar13', SHA2('Benjamingar#2026', 256));
INSERT INTO Usuario (DNI, Apodo, Contrasena) VALUES ('52420093', 'martinaper13', SHA2('Martinaper#2026', 256));
INSERT INTO Usuario (DNI, Apodo, Contrasena) VALUES ('49512712', 'thiagosan08', SHA2('Thiagosan#2026', 256));
INSERT INTO Usuario (DNI, Apodo, Contrasena) VALUES ('52652318', 'sofiarom09', SHA2('Sofiarom#2026', 256));
INSERT INTO Usuario (DNI, Apodo, Contrasena) VALUES ('48428925', 'joaquindia11', SHA2('Joaquindia#2026', 256));
INSERT INTO Usuario (DNI, Apodo, Contrasena) VALUES ('48322516', 'julietaalv13', SHA2('Julietaalv#2026', 256));
INSERT INTO Usuario (DNI, Apodo, Contrasena) VALUES ('47596375', 'bautistator13', SHA2('Bautistator#2026', 256));
INSERT INTO Usuario (DNI, Apodo, Contrasena) VALUES ('53199127', 'agustinarui11', SHA2('Agustinarui#2026', 256));
INSERT INTO Usuario (DNI, Apodo, Contrasena) VALUES ('49263492', 'lautaroaco12', SHA2('Lautaroaco#2026', 256));
INSERT INTO Usuario (DNI, Apodo, Contrasena) VALUES ('49609844', 'micaelaben12', SHA2('Micaelaben#2026', 256));
INSERT INTO Usuario (DNI, Apodo, Contrasena) VALUES ('50491680', 'facundomed12', SHA2('Facundomed#2026', 256));
INSERT INTO Usuario (DNI, Apodo, Contrasena) VALUES ('51808284', 'florenciaher09', SHA2('Florenciaher#2026', 256));
INSERT INTO Usuario (DNI, Apodo, Contrasena) VALUES ('50355436', 'nicolassua09', SHA2('Nicolassua#2026', 256));
INSERT INTO Usuario (DNI, Apodo, Contrasena) VALUES ('52176234', 'rocioagu11', SHA2('Rocioagu#2026', 256));
INSERT INTO Usuario (DNI, Apodo, Contrasena) VALUES (NULL, 'marcela.ibanez', SHA2('MarcelaBuffet#2026', 256));  -- 21: Marcela Ibáñez (MarcelaBuffet#2026)
INSERT INTO Usuario (DNI, Apodo, Contrasena) VALUES (NULL, 'diego.paz', SHA2('DiegoBuffet#2026', 256));  -- 22: Diego Paz (DiegoBuffet#2026)
INSERT INTO Usuario (DNI, Apodo, Contrasena) VALUES (NULL, 'karina.luna', SHA2('KarinaBuffet#2026', 256));  -- 23: Karina Luna (KarinaBuffet#2026)
INSERT INTO Usuario (DNI, Apodo, Contrasena) VALUES (NULL, 'sergio.ledesma', SHA2('SergioBuffet#2026', 256));  -- 24: Sergio Ledesma (SergioBuffet#2026)
INSERT INTO Usuario (DNI, Apodo, Contrasena) VALUES (NULL, 'paola.vega', SHA2('PaolaBuffet#2026', 256));  -- 25: Paola Vega (PaolaBuffet#2026)
INSERT INTO Usuario (DNI, Apodo, Contrasena) VALUES (NULL, 'hector.maldonado', SHA2('HectorBuffet#2026', 256));  -- 26: Héctor Maldonado (HectorBuffet#2026)
INSERT INTO Usuario (DNI, Apodo, Contrasena) VALUES (NULL, 'alejandro.funes', SHA2('AlejandroBuffet#2026', 256));  -- 27: Alejandro Funes (AlejandroBuffet#2026)
INSERT INTO Usuario (DNI, Apodo, Contrasena) VALUES (NULL, 'silvia.arce', SHA2('SilviaBuffet#2026', 256));  -- 28: Silvia Arce (SilviaBuffet#2026)
INSERT INTO Usuario (DNI, Apodo, Contrasena) VALUES (NULL, 'ramiro.sosa', SHA2('RamiroBuffet#2026', 256));  -- 29: Ramiro Sosa (RamiroBuffet#2026)
INSERT INTO Usuario (DNI, Apodo, Contrasena) VALUES (NULL, 'veronica.campos', SHA2('VeronicaBuffet#2026', 256));  -- 30: Verónica Campos (VeronicaBuffet#2026)

-- ----------------------------------------------------------------------------
-- Empleado (20) - los que usan el sistema tienen UserID
-- ----------------------------------------------------------------------------
INSERT INTO Empleado (Nombre_Empleado, Apellido_Empleado, ID_Puesto, UserID) VALUES ('Marcela', 'Ibáñez', 1, 21);
INSERT INTO Empleado (Nombre_Empleado, Apellido_Empleado, ID_Puesto, UserID) VALUES ('Diego', 'Paz', 2, 22);
INSERT INTO Empleado (Nombre_Empleado, Apellido_Empleado, ID_Puesto, UserID) VALUES ('Karina', 'Luna', 1, 23);
INSERT INTO Empleado (Nombre_Empleado, Apellido_Empleado, ID_Puesto, UserID) VALUES ('Sergio', 'Ledesma', 3, 24);
INSERT INTO Empleado (Nombre_Empleado, Apellido_Empleado, ID_Puesto, UserID) VALUES ('Paola', 'Vega', 4, 25);
INSERT INTO Empleado (Nombre_Empleado, Apellido_Empleado, ID_Puesto, UserID) VALUES ('Héctor', 'Maldonado', 3, 26);
INSERT INTO Empleado (Nombre_Empleado, Apellido_Empleado, ID_Puesto, UserID) VALUES ('Alejandro', 'Funes', 5, 27);
INSERT INTO Empleado (Nombre_Empleado, Apellido_Empleado, ID_Puesto, UserID) VALUES ('Norma', 'Villalba', 6, NULL);
INSERT INTO Empleado (Nombre_Empleado, Apellido_Empleado, ID_Puesto, UserID) VALUES ('Raúl', 'Cardozo', 7, NULL);
INSERT INTO Empleado (Nombre_Empleado, Apellido_Empleado, ID_Puesto, UserID) VALUES ('Elena', 'Barrios', 8, NULL);
INSERT INTO Empleado (Nombre_Empleado, Apellido_Empleado, ID_Puesto, UserID) VALUES ('Marcos', 'Godoy', 8, NULL);
INSERT INTO Empleado (Nombre_Empleado, Apellido_Empleado, ID_Puesto, UserID) VALUES ('Silvia', 'Arce', 9, 28);
INSERT INTO Empleado (Nombre_Empleado, Apellido_Empleado, ID_Puesto, UserID) VALUES ('Ramiro', 'Sosa', 10, 29);
INSERT INTO Empleado (Nombre_Empleado, Apellido_Empleado, ID_Puesto, UserID) VALUES ('Gladys', 'Correa', 11, NULL);
INSERT INTO Empleado (Nombre_Empleado, Apellido_Empleado, ID_Puesto, UserID) VALUES ('Leandro', 'Fuentes', 12, NULL);
INSERT INTO Empleado (Nombre_Empleado, Apellido_Empleado, ID_Puesto, UserID) VALUES ('Patricia', 'Navarro', 13, NULL);
INSERT INTO Empleado (Nombre_Empleado, Apellido_Empleado, ID_Puesto, UserID) VALUES ('Walter', 'Bustos', 14, NULL);
INSERT INTO Empleado (Nombre_Empleado, Apellido_Empleado, ID_Puesto, UserID) VALUES ('Mónica', 'Escobar', 15, NULL);
INSERT INTO Empleado (Nombre_Empleado, Apellido_Empleado, ID_Puesto, UserID) VALUES ('Cristian', 'Toledo', 16, NULL);
INSERT INTO Empleado (Nombre_Empleado, Apellido_Empleado, ID_Puesto, UserID) VALUES ('Verónica', 'Campos', 17, 30);

-- ----------------------------------------------------------------------------
-- Email (20)
-- ----------------------------------------------------------------------------
INSERT INTO Email (UserID, Email) VALUES (1, 'jorge.dahan@bue.edu.ar');
INSERT INTO Email (UserID, Email) VALUES (2, 'lucia.gomez@bue.edu.ar');
INSERT INTO Email (UserID, Email) VALUES (3, 'mateo.fernandez@bue.edu.ar');
INSERT INTO Email (UserID, Email) VALUES (4, 'valentina.rodriguez@bue.edu.ar');
INSERT INTO Email (UserID, Email) VALUES (5, 'santiago.lopez@bue.edu.ar');
INSERT INTO Email (UserID, Email) VALUES (6, 'camila.martinez@bue.edu.ar');
INSERT INTO Email (UserID, Email) VALUES (7, 'benjamin.garcia@bue.edu.ar');
INSERT INTO Email (UserID, Email) VALUES (8, 'martina.perez@bue.edu.ar');
INSERT INTO Email (UserID, Email) VALUES (9, 'thiago.sanchez@bue.edu.ar');
INSERT INTO Email (UserID, Email) VALUES (10, 'sofia.romero@bue.edu.ar');
INSERT INTO Email (UserID, Email) VALUES (11, 'joaquin.diaz@bue.edu.ar');
INSERT INTO Email (UserID, Email) VALUES (12, 'julieta.alvarez@bue.edu.ar');
INSERT INTO Email (UserID, Email) VALUES (13, 'bautista.torres@bue.edu.ar');
INSERT INTO Email (UserID, Email) VALUES (14, 'agustina.ruiz@bue.edu.ar');
INSERT INTO Email (UserID, Email) VALUES (15, 'lautaro.acosta@bue.edu.ar');
INSERT INTO Email (UserID, Email) VALUES (16, 'micaela.benitez@bue.edu.ar');
INSERT INTO Email (UserID, Email) VALUES (17, 'facundo.medina@bue.edu.ar');
INSERT INTO Email (UserID, Email) VALUES (18, 'florencia.herrera@bue.edu.ar');
INSERT INTO Email (UserID, Email) VALUES (19, 'nicolas.suarez@bue.edu.ar');
INSERT INTO Email (UserID, Email) VALUES (20, 'rocio.aguirre@bue.edu.ar');

-- ----------------------------------------------------------------------------
-- Telefono (20)
-- ----------------------------------------------------------------------------
INSERT INTO Telefono (UserID, Telefono) VALUES (1, '+54 9 11 4356-8494');
INSERT INTO Telefono (UserID, Telefono) VALUES (2, '+54 9 11 3154-6062');
INSERT INTO Telefono (UserID, Telefono) VALUES (3, '+54 9 11 2202-7043');
INSERT INTO Telefono (UserID, Telefono) VALUES (4, '+54 9 11 4978-8554');
INSERT INTO Telefono (UserID, Telefono) VALUES (5, '+54 9 11 5458-2485');
INSERT INTO Telefono (UserID, Telefono) VALUES (6, '+54 9 11 5266-9166');
INSERT INTO Telefono (UserID, Telefono) VALUES (7, '+54 9 11 2953-7970');
INSERT INTO Telefono (UserID, Telefono) VALUES (8, '+54 9 11 6148-9045');
INSERT INTO Telefono (UserID, Telefono) VALUES (9, '+54 9 11 5261-9595');
INSERT INTO Telefono (UserID, Telefono) VALUES (10, '+54 9 11 4118-7880');
INSERT INTO Telefono (UserID, Telefono) VALUES (11, '+54 9 11 6620-8851');
INSERT INTO Telefono (UserID, Telefono) VALUES (12, '+54 9 11 6202-9486');
INSERT INTO Telefono (UserID, Telefono) VALUES (13, '+54 9 11 2207-4678');
INSERT INTO Telefono (UserID, Telefono) VALUES (14, '+54 9 11 3056-1800');
INSERT INTO Telefono (UserID, Telefono) VALUES (15, '+54 9 11 6286-2680');
INSERT INTO Telefono (UserID, Telefono) VALUES (16, '+54 9 11 5517-8701');
INSERT INTO Telefono (UserID, Telefono) VALUES (17, '+54 9 11 3166-8898');
INSERT INTO Telefono (UserID, Telefono) VALUES (18, '+54 9 11 3875-2999');
INSERT INTO Telefono (UserID, Telefono) VALUES (19, '+54 9 11 5742-8690');
INSERT INTO Telefono (UserID, Telefono) VALUES (20, '+54 9 11 6469-5112');

-- ============================================================================
-- SCHEMA COMERCIAL
-- ============================================================================
USE schema_comercial;

-- ----------------------------------------------------------------------------
-- Categoria (20)
-- ----------------------------------------------------------------------------
INSERT INTO Categoria (Categoria) VALUES ('Sándwiches y Tostados');
INSERT INTO Categoria (Categoria) VALUES ('Empanadas y Tartas');
INSERT INTO Categoria (Categoria) VALUES ('Pizzas');
INSERT INTO Categoria (Categoria) VALUES ('Panificados');
INSERT INTO Categoria (Categoria) VALUES ('Medialunas y Facturas');
INSERT INTO Categoria (Categoria) VALUES ('Ensaladas');
INSERT INTO Categoria (Categoria) VALUES ('Frutas');
INSERT INTO Categoria (Categoria) VALUES ('Lácteos');
INSERT INTO Categoria (Categoria) VALUES ('Yogures y Postres');
INSERT INTO Categoria (Categoria) VALUES ('Aguas');
INSERT INTO Categoria (Categoria) VALUES ('Jugos');
INSERT INTO Categoria (Categoria) VALUES ('Gaseosas');
INSERT INTO Categoria (Categoria) VALUES ('Infusiones Calientes');
INSERT INTO Categoria (Categoria) VALUES ('Cereales y Barras');
INSERT INTO Categoria (Categoria) VALUES ('Galletitas');
INSERT INTO Categoria (Categoria) VALUES ('Alfajores');
INSERT INTO Categoria (Categoria) VALUES ('Golosinas');
INSERT INTO Categoria (Categoria) VALUES ('Snacks Saludables');
INSERT INTO Categoria (Categoria) VALUES ('Platos Calientes');
INSERT INTO Categoria (Categoria) VALUES ('Combos Escolares');

-- ----------------------------------------------------------------------------
-- Metodo_Pago (20)
-- ----------------------------------------------------------------------------
INSERT INTO Metodo_Pago (MetodoPago) VALUES ('Efectivo en Mostrador');
INSERT INTO Metodo_Pago (MetodoPago) VALUES ('Mercado Pago');
INSERT INTO Metodo_Pago (MetodoPago) VALUES ('Transferencia Institucional');
INSERT INTO Metodo_Pago (MetodoPago) VALUES ('Tarjeta de Débito Visa');
INSERT INTO Metodo_Pago (MetodoPago) VALUES ('Tarjeta de Débito Mastercard');
INSERT INTO Metodo_Pago (MetodoPago) VALUES ('Tarjeta de Débito Maestro');
INSERT INTO Metodo_Pago (MetodoPago) VALUES ('Tarjeta de Crédito Visa');
INSERT INTO Metodo_Pago (MetodoPago) VALUES ('Tarjeta de Crédito Mastercard');
INSERT INTO Metodo_Pago (MetodoPago) VALUES ('Tarjeta de Crédito American Express');
INSERT INTO Metodo_Pago (MetodoPago) VALUES ('Tarjeta Naranja');
INSERT INTO Metodo_Pago (MetodoPago) VALUES ('MODO');
INSERT INTO Metodo_Pago (MetodoPago) VALUES ('Cuenta DNI');
INSERT INTO Metodo_Pago (MetodoPago) VALUES ('Ualá');
INSERT INTO Metodo_Pago (MetodoPago) VALUES ('Naranja X');
INSERT INTO Metodo_Pago (MetodoPago) VALUES ('Personal Pay');
INSERT INTO Metodo_Pago (MetodoPago) VALUES ('Prex');
INSERT INTO Metodo_Pago (MetodoPago) VALUES ('QR Interoperable');
INSERT INTO Metodo_Pago (MetodoPago) VALUES ('Brubank');
INSERT INTO Metodo_Pago (MetodoPago) VALUES ('Vale Institucional');
INSERT INTO Metodo_Pago (MetodoPago) VALUES ('Billetera Santander');

-- ----------------------------------------------------------------------------
-- Estado_Retiro (20) - 1 PENDIENTE, 2 ENTREGADO, 3 CANCELADO (los usan los SPs)
-- ----------------------------------------------------------------------------
INSERT INTO Estado_Retiro (Estado) VALUES ('PENDIENTE');
INSERT INTO Estado_Retiro (Estado) VALUES ('ENTREGADO');
INSERT INTO Estado_Retiro (Estado) VALUES ('CANCELADO');
INSERT INTO Estado_Retiro (Estado) VALUES ('PAGO PENDIENTE');
INSERT INTO Estado_Retiro (Estado) VALUES ('PAGO ACREDITADO');
INSERT INTO Estado_Retiro (Estado) VALUES ('PAGO RECHAZADO');
INSERT INTO Estado_Retiro (Estado) VALUES ('EN PREPARACION');
INSERT INTO Estado_Retiro (Estado) VALUES ('LISTO PARA RETIRAR');
INSERT INTO Estado_Retiro (Estado) VALUES ('DEMORADO');
INSERT INTO Estado_Retiro (Estado) VALUES ('SIN STOCK');
INSERT INTO Estado_Retiro (Estado) VALUES ('REPROGRAMADO');
INSERT INTO Estado_Retiro (Estado) VALUES ('NO RETIRADO');
INSERT INTO Estado_Retiro (Estado) VALUES ('VENCIDO');
INSERT INTO Estado_Retiro (Estado) VALUES ('RECLAMADO');
INSERT INTO Estado_Retiro (Estado) VALUES ('REEMBOLSADO');
INSERT INTO Estado_Retiro (Estado) VALUES ('ENTREGADO PARCIAL');
INSERT INTO Estado_Retiro (Estado) VALUES ('CANCELADO POR EL BUFFET');
INSERT INTO Estado_Retiro (Estado) VALUES ('CANCELADO POR EL ALUMNO');
INSERT INTO Estado_Retiro (Estado) VALUES ('EN ESPERA DE CONFIRMACION');
INSERT INTO Estado_Retiro (Estado) VALUES ('ARCHIVADO');

-- ----------------------------------------------------------------------------
-- Franja_Retiro (20) - recreos y horarios de la escuela, ordenados por hora
-- ----------------------------------------------------------------------------
INSERT INTO Franja_Retiro (Nombre_Franja, Hora_Retiro) VALUES ('Horario de Entrada Turno Mañana', '07:45:00');
INSERT INTO Franja_Retiro (Nombre_Franja, Hora_Retiro) VALUES ('Contraturno Taller Mañana', '08:20:00');
INSERT INTO Franja_Retiro (Nombre_Franja, Hora_Retiro) VALUES ('1er Recreo Turno Mañana', '09:00:00');
INSERT INTO Franja_Retiro (Nombre_Franja, Hora_Retiro) VALUES ('Salida a Educación Física', '10:00:00');
INSERT INTO Franja_Retiro (Nombre_Franja, Hora_Retiro) VALUES ('2do Recreo Turno Mañana', '10:35:00');
INSERT INTO Franja_Retiro (Nombre_Franja, Hora_Retiro) VALUES ('3er Recreo Turno Mañana', '11:40:00');
INSERT INTO Franja_Retiro (Nombre_Franja, Hora_Retiro) VALUES ('Salida de Clases Turno Mañana', '12:00:00');
INSERT INTO Franja_Retiro (Nombre_Franja, Hora_Retiro) VALUES ('Almuerzo Buffet Escolar', '12:05:00');
INSERT INTO Franja_Retiro (Nombre_Franja, Hora_Retiro) VALUES ('Almuerzo Buffet Escolar - 2do Turno', '12:45:00');
INSERT INTO Franja_Retiro (Nombre_Franja, Hora_Retiro) VALUES ('Recreo Jornada Completa', '13:00:00');
INSERT INTO Franja_Retiro (Nombre_Franja, Hora_Retiro) VALUES ('Horario de Entrada Turno Tarde', '13:30:00');
INSERT INTO Franja_Retiro (Nombre_Franja, Hora_Retiro) VALUES ('1er Recreo Turno Tarde', '14:50:00');
INSERT INTO Franja_Retiro (Nombre_Franja, Hora_Retiro) VALUES ('Contraturno Taller Tarde', '15:30:00');
INSERT INTO Franja_Retiro (Nombre_Franja, Hora_Retiro) VALUES ('2do Recreo Turno Tarde', '16:20:00');
INSERT INTO Franja_Retiro (Nombre_Franja, Hora_Retiro) VALUES ('3er Recreo Turno Tarde', '17:10:00');
INSERT INTO Franja_Retiro (Nombre_Franja, Hora_Retiro) VALUES ('Salida de Clases Turno Tarde', '17:50:00');
INSERT INTO Franja_Retiro (Nombre_Franja, Hora_Retiro) VALUES ('Horario de Entrada Turno Vespertino', '18:30:00');
INSERT INTO Franja_Retiro (Nombre_Franja, Hora_Retiro) VALUES ('1er Recreo Turno Vespertino', '19:40:00');
INSERT INTO Franja_Retiro (Nombre_Franja, Hora_Retiro) VALUES ('2do Recreo Turno Vespertino', '21:00:00');
INSERT INTO Franja_Retiro (Nombre_Franja, Hora_Retiro) VALUES ('Salida de Clases Turno Vespertino', '22:30:00');

-- ----------------------------------------------------------------------------
-- Producto (20)
-- ----------------------------------------------------------------------------
INSERT INTO Producto (ID_Categoria, Nombre_Producto, Descripcion_Producto, Stock_Producto, Precio_Producto) VALUES (1, 'Sándwich de Milanesa', 'Milanesa de carne en pan francés con lechuga y tomate', 40, '2800.00');
INSERT INTO Producto (ID_Categoria, Nombre_Producto, Descripcion_Producto, Stock_Producto, Precio_Producto) VALUES (2, 'Empanada de Carne', 'Empanada al horno rellena de carne cortada a cuchillo', 80, '1100.00');
INSERT INTO Producto (ID_Categoria, Nombre_Producto, Descripcion_Producto, Stock_Producto, Precio_Producto) VALUES (3, 'Porción de Pizza Muzzarella', 'Porción de pizza de muzzarella', 50, '1800.00');
INSERT INTO Producto (ID_Categoria, Nombre_Producto, Descripcion_Producto, Stock_Producto, Precio_Producto) VALUES (4, 'Pebete de Queso', 'Pebete con queso cremoso', 45, '1600.00');
INSERT INTO Producto (ID_Categoria, Nombre_Producto, Descripcion_Producto, Stock_Producto, Precio_Producto) VALUES (5, 'Medialuna de Manteca', 'Medialuna dulce de manteca', 90, '800.00');
INSERT INTO Producto (ID_Categoria, Nombre_Producto, Descripcion_Producto, Stock_Producto, Precio_Producto) VALUES (6, 'Ensalada Mixta', 'Lechuga, tomate y zanahoria', 25, '2200.00');
INSERT INTO Producto (ID_Categoria, Nombre_Producto, Descripcion_Producto, Stock_Producto, Precio_Producto) VALUES (7, 'Manzana', 'Manzana roja por unidad', 70, '700.00');
INSERT INTO Producto (ID_Categoria, Nombre_Producto, Descripcion_Producto, Stock_Producto, Precio_Producto) VALUES (8, 'Chocolatada 250 ml', 'Leche chocolatada en envase individual', 70, '1100.00');
INSERT INTO Producto (ID_Categoria, Nombre_Producto, Descripcion_Producto, Stock_Producto, Precio_Producto) VALUES (9, 'Yogur Bebible de Frutilla', 'Yogur bebible 200 ml', 60, '1200.00');
INSERT INTO Producto (ID_Categoria, Nombre_Producto, Descripcion_Producto, Stock_Producto, Precio_Producto) VALUES (10, 'Agua Mineral 500 ml', 'Agua mineral sin gas', 150, '1000.00');
INSERT INTO Producto (ID_Categoria, Nombre_Producto, Descripcion_Producto, Stock_Producto, Precio_Producto) VALUES (11, 'Jugo en Caja 200 ml', 'Jugo de frutas en envase individual', 100, '900.00');
INSERT INTO Producto (ID_Categoria, Nombre_Producto, Descripcion_Producto, Stock_Producto, Precio_Producto) VALUES (12, 'Gaseosa 500ml', 'Gaseosa línea cola 500 ml', 100, '1300.00');
INSERT INTO Producto (ID_Categoria, Nombre_Producto, Descripcion_Producto, Stock_Producto, Precio_Producto) VALUES (13, 'Café con Leche', 'Vaso de café con leche', 60, '1400.00');
INSERT INTO Producto (ID_Categoria, Nombre_Producto, Descripcion_Producto, Stock_Producto, Precio_Producto) VALUES (14, 'Barra de Cereal', 'Barra de cereal con frutas', 80, '900.00');
INSERT INTO Producto (ID_Categoria, Nombre_Producto, Descripcion_Producto, Stock_Producto, Precio_Producto) VALUES (15, 'Galletitas de Agua', 'Paquete individual de galletitas de agua', 80, '700.00');
INSERT INTO Producto (ID_Categoria, Nombre_Producto, Descripcion_Producto, Stock_Producto, Precio_Producto) VALUES (16, 'Alfajor de Maicena', 'Alfajor de maicena con dulce de leche y coco', 70, '800.00');
INSERT INTO Producto (ID_Categoria, Nombre_Producto, Descripcion_Producto, Stock_Producto, Precio_Producto) VALUES (17, 'Turrón de Maní', 'Turrón de maní individual', 60, '600.00');
INSERT INTO Producto (ID_Categoria, Nombre_Producto, Descripcion_Producto, Stock_Producto, Precio_Producto) VALUES (18, 'Mix de Frutos Secos', 'Mix de almendras, pasas y maní sin sal', 40, '1500.00');
INSERT INTO Producto (ID_Categoria, Nombre_Producto, Descripcion_Producto, Stock_Producto, Precio_Producto) VALUES (19, 'Milanesa de Pollo con Puré', 'Milanesa al horno con puré de papas', 25, '4200.00');
INSERT INTO Producto (ID_Categoria, Nombre_Producto, Descripcion_Producto, Stock_Producto, Precio_Producto) VALUES (20, 'Combo Sándwich + Gaseosa', 'Sándwich de milanesa + gaseosa 500 ml', 40, '4100.00');

-- ----------------------------------------------------------------------------
-- Menu_Del_Dia (20 - un menú por día hábil)
-- ----------------------------------------------------------------------------
INSERT INTO Menu_Del_Dia (ID_Categoria, Plato_menu, Precio_menu, Stock_menu, Fecha_menu) VALUES (19, 'Milanesa de pollo con puré', '4200.00', 40, '2026-10-14');
INSERT INTO Menu_Del_Dia (ID_Categoria, Plato_menu, Precio_menu, Stock_menu, Fecha_menu) VALUES (19, 'Fideos con tuco y queso', '3800.00', 35, '2026-10-15');
INSERT INTO Menu_Del_Dia (ID_Categoria, Plato_menu, Precio_menu, Stock_menu, Fecha_menu) VALUES (2, 'Tarta de zapallitos con ensalada', '3900.00', 30, '2026-10-16');
INSERT INTO Menu_Del_Dia (ID_Categoria, Plato_menu, Precio_menu, Stock_menu, Fecha_menu) VALUES (19, 'Pollo al horno con arroz', '4300.00', 40, '2026-10-19');
INSERT INTO Menu_Del_Dia (ID_Categoria, Plato_menu, Precio_menu, Stock_menu, Fecha_menu) VALUES (19, 'Tortilla de papas con ensalada', '3900.00', 40, '2026-10-20');
INSERT INTO Menu_Del_Dia (ID_Categoria, Plato_menu, Precio_menu, Stock_menu, Fecha_menu) VALUES (19, 'Ñoquis con salsa mixta', '4000.00', 35, '2026-10-21');
INSERT INTO Menu_Del_Dia (ID_Categoria, Plato_menu, Precio_menu, Stock_menu, Fecha_menu) VALUES (19, 'Hamburguesa casera con ensalada', '4400.00', 35, '2026-10-22');
INSERT INTO Menu_Del_Dia (ID_Categoria, Plato_menu, Precio_menu, Stock_menu, Fecha_menu) VALUES (19, 'Pastel de papa', '4100.00', 35, '2026-10-23');
INSERT INTO Menu_Del_Dia (ID_Categoria, Plato_menu, Precio_menu, Stock_menu, Fecha_menu) VALUES (19, 'Wok de verduras y pollo', '4300.00', 25, '2026-10-26');
INSERT INTO Menu_Del_Dia (ID_Categoria, Plato_menu, Precio_menu, Stock_menu, Fecha_menu) VALUES (19, 'Canelones de verdura', '4200.00', 30, '2026-10-27');
INSERT INTO Menu_Del_Dia (ID_Categoria, Plato_menu, Precio_menu, Stock_menu, Fecha_menu) VALUES (2, 'Empanadas (2) con ensalada', '3700.00', 30, '2026-10-28');
INSERT INTO Menu_Del_Dia (ID_Categoria, Plato_menu, Precio_menu, Stock_menu, Fecha_menu) VALUES (19, 'Guiso de lentejas', '3800.00', 20, '2026-10-29');
INSERT INTO Menu_Del_Dia (ID_Categoria, Plato_menu, Precio_menu, Stock_menu, Fecha_menu) VALUES (19, 'Arroz con pollo', '4000.00', 20, '2026-10-30');
INSERT INTO Menu_Del_Dia (ID_Categoria, Plato_menu, Precio_menu, Stock_menu, Fecha_menu) VALUES (3, 'Pizza casera con ensalada', '3900.00', 25, '2026-11-02');
INSERT INTO Menu_Del_Dia (ID_Categoria, Plato_menu, Precio_menu, Stock_menu, Fecha_menu) VALUES (19, 'Milanesa napolitana con papas', '4600.00', 40, '2026-11-03');
INSERT INTO Menu_Del_Dia (ID_Categoria, Plato_menu, Precio_menu, Stock_menu, Fecha_menu) VALUES (6, 'Salpicón de ave', '3800.00', 35, '2026-11-04');
INSERT INTO Menu_Del_Dia (ID_Categoria, Plato_menu, Precio_menu, Stock_menu, Fecha_menu) VALUES (19, 'Revuelto gramajo', '4100.00', 40, '2026-11-05');
INSERT INTO Menu_Del_Dia (ID_Categoria, Plato_menu, Precio_menu, Stock_menu, Fecha_menu) VALUES (19, 'Ravioles con salsa fileto', '4300.00', 35, '2026-11-06');
INSERT INTO Menu_Del_Dia (ID_Categoria, Plato_menu, Precio_menu, Stock_menu, Fecha_menu) VALUES (2, 'Tarta de choclo con ensalada', '3900.00', 30, '2026-11-09');
INSERT INTO Menu_Del_Dia (ID_Categoria, Plato_menu, Precio_menu, Stock_menu, Fecha_menu) VALUES (19, 'Zapallitos rellenos', '3900.00', 25, '2026-11-10');

-- ----------------------------------------------------------------------------
-- Detalle_Menu_Dia (20 - producto que incluye cada menú: bebida, fruta o postre)
-- ----------------------------------------------------------------------------
INSERT INTO Detalle_Menu_Dia (Menu_Del_DiaID, ProductoID) VALUES (1, 10);
INSERT INTO Detalle_Menu_Dia (Menu_Del_DiaID, ProductoID) VALUES (2, 11);
INSERT INTO Detalle_Menu_Dia (Menu_Del_DiaID, ProductoID) VALUES (3, 7);
INSERT INTO Detalle_Menu_Dia (Menu_Del_DiaID, ProductoID) VALUES (4, 9);
INSERT INTO Detalle_Menu_Dia (Menu_Del_DiaID, ProductoID) VALUES (5, 10);
INSERT INTO Detalle_Menu_Dia (Menu_Del_DiaID, ProductoID) VALUES (6, 11);
INSERT INTO Detalle_Menu_Dia (Menu_Del_DiaID, ProductoID) VALUES (7, 7);
INSERT INTO Detalle_Menu_Dia (Menu_Del_DiaID, ProductoID) VALUES (8, 9);
INSERT INTO Detalle_Menu_Dia (Menu_Del_DiaID, ProductoID) VALUES (9, 10);
INSERT INTO Detalle_Menu_Dia (Menu_Del_DiaID, ProductoID) VALUES (10, 11);
INSERT INTO Detalle_Menu_Dia (Menu_Del_DiaID, ProductoID) VALUES (11, 7);
INSERT INTO Detalle_Menu_Dia (Menu_Del_DiaID, ProductoID) VALUES (12, 9);
INSERT INTO Detalle_Menu_Dia (Menu_Del_DiaID, ProductoID) VALUES (13, 10);
INSERT INTO Detalle_Menu_Dia (Menu_Del_DiaID, ProductoID) VALUES (14, 11);
INSERT INTO Detalle_Menu_Dia (Menu_Del_DiaID, ProductoID) VALUES (15, 7);
INSERT INTO Detalle_Menu_Dia (Menu_Del_DiaID, ProductoID) VALUES (16, 9);
INSERT INTO Detalle_Menu_Dia (Menu_Del_DiaID, ProductoID) VALUES (17, 10);
INSERT INTO Detalle_Menu_Dia (Menu_Del_DiaID, ProductoID) VALUES (18, 11);
INSERT INTO Detalle_Menu_Dia (Menu_Del_DiaID, ProductoID) VALUES (19, 7);
INSERT INTO Detalle_Menu_Dia (Menu_Del_DiaID, ProductoID) VALUES (20, 9);

-- ----------------------------------------------------------------------------
-- Venta (50 - TABLA PRINCIPAL). Monto_Total = suma de sus renglones de Carrito
-- ----------------------------------------------------------------------------
INSERT INTO Venta (EmpleadoID, UserID, Metodo_PagoID, Fecha_Hora, Monto_Total) VALUES (1, 8, 1, '2026-10-14 09:05:00', '4200.00');
INSERT INTO Venta (EmpleadoID, UserID, Metodo_PagoID, Fecha_Hora, Monto_Total) VALUES (2, 5, 2, '2026-10-14 12:15:00', '2400.00');
INSERT INTO Venta (EmpleadoID, UserID, Metodo_PagoID, Fecha_Hora, Monto_Total) VALUES (1, 15, 1, '2026-10-15 10:40:00', '7600.00');
INSERT INTO Venta (EmpleadoID, UserID, Metodo_PagoID, Fecha_Hora, Monto_Total) VALUES (NULL, 12, 3, '2026-10-15 14:55:00', '2600.00');
INSERT INTO Venta (EmpleadoID, UserID, Metodo_PagoID, Fecha_Hora, Monto_Total) VALUES (NULL, 5, 2, '2026-10-15 16:25:00', '3800.00');
INSERT INTO Venta (EmpleadoID, UserID, Metodo_PagoID, Fecha_Hora, Monto_Total) VALUES (NULL, 17, 2, '2026-10-15 18:10:00', '1600.00');
INSERT INTO Venta (EmpleadoID, UserID, Metodo_PagoID, Fecha_Hora, Monto_Total) VALUES (NULL, 3, 2, '2026-10-16 09:10:00', '3900.00');
INSERT INTO Venta (EmpleadoID, UserID, Metodo_PagoID, Fecha_Hora, Monto_Total) VALUES (2, 2, 1, '2026-10-20 12:20:00', '3600.00');
INSERT INTO Venta (EmpleadoID, UserID, Metodo_PagoID, Fecha_Hora, Monto_Total) VALUES (6, 5, 2, '2026-10-20 16:25:00', '3900.00');
INSERT INTO Venta (EmpleadoID, UserID, Metodo_PagoID, Fecha_Hora, Monto_Total) VALUES (2, 9, 1, '2026-10-21 09:08:00', '4100.00');
INSERT INTO Venta (EmpleadoID, UserID, Metodo_PagoID, Fecha_Hora, Monto_Total) VALUES (4, 13, 1, '2026-10-21 14:55:00', '5600.00');
INSERT INTO Venta (EmpleadoID, UserID, Metodo_PagoID, Fecha_Hora, Monto_Total) VALUES (1, 16, 1, '2026-10-22 10:40:00', '1100.00');
INSERT INTO Venta (EmpleadoID, UserID, Metodo_PagoID, Fecha_Hora, Monto_Total) VALUES (NULL, 1, 3, '2026-10-23 09:12:00', '2800.00');
INSERT INTO Venta (EmpleadoID, UserID, Metodo_PagoID, Fecha_Hora, Monto_Total) VALUES (2, 18, 1, '2026-10-26 12:30:00', '1400.00');
INSERT INTO Venta (EmpleadoID, UserID, Metodo_PagoID, Fecha_Hora, Monto_Total) VALUES (NULL, 6, 3, '2026-10-26 14:55:00', '1100.00');
INSERT INTO Venta (EmpleadoID, UserID, Metodo_PagoID, Fecha_Hora, Monto_Total) VALUES (3, 1, 1, '2026-10-27 09:05:00', '4200.00');
INSERT INTO Venta (EmpleadoID, UserID, Metodo_PagoID, Fecha_Hora, Monto_Total) VALUES (NULL, 10, 2, '2026-10-27 10:42:00', '800.00');
INSERT INTO Venta (EmpleadoID, UserID, Metodo_PagoID, Fecha_Hora, Monto_Total) VALUES (3, 3, 1, '2026-10-27 12:15:00', '1400.00');
INSERT INTO Venta (EmpleadoID, UserID, Metodo_PagoID, Fecha_Hora, Monto_Total) VALUES (1, 20, 1, '2026-10-27 14:52:00', '3600.00');
INSERT INTO Venta (EmpleadoID, UserID, Metodo_PagoID, Fecha_Hora, Monto_Total) VALUES (2, 17, 1, '2026-10-27 16:22:00', '2800.00');
INSERT INTO Venta (EmpleadoID, UserID, Metodo_PagoID, Fecha_Hora, Monto_Total) VALUES (5, 6, 1, '2026-10-27 18:20:00', '4200.00');
INSERT INTO Venta (EmpleadoID, UserID, Metodo_PagoID, Fecha_Hora, Monto_Total) VALUES (4, 4, 2, '2026-10-28 16:25:00', '1400.00');
INSERT INTO Venta (EmpleadoID, UserID, Metodo_PagoID, Fecha_Hora, Monto_Total) VALUES (2, 14, 2, '2026-10-29 10:38:00', '3800.00');
INSERT INTO Venta (EmpleadoID, UserID, Metodo_PagoID, Fecha_Hora, Monto_Total) VALUES (2, 4, 1, '2026-10-29 12:25:00', '3800.00');
INSERT INTO Venta (EmpleadoID, UserID, Metodo_PagoID, Fecha_Hora, Monto_Total) VALUES (4, 4, 1, '2026-10-29 17:55:00', '2600.00');
INSERT INTO Venta (EmpleadoID, UserID, Metodo_PagoID, Fecha_Hora, Monto_Total) VALUES (NULL, 14, 2, '2026-10-30 09:14:00', '2200.00');
INSERT INTO Venta (EmpleadoID, UserID, Metodo_PagoID, Fecha_Hora, Monto_Total) VALUES (2, 10, 1, '2026-10-30 10:40:00', '1800.00');
INSERT INTO Venta (EmpleadoID, UserID, Metodo_PagoID, Fecha_Hora, Monto_Total) VALUES (1, 18, 1, '2026-11-02 09:05:00', '3900.00');
INSERT INTO Venta (EmpleadoID, UserID, Metodo_PagoID, Fecha_Hora, Monto_Total) VALUES (4, 18, 2, '2026-11-02 14:53:00', '700.00');
INSERT INTO Venta (EmpleadoID, UserID, Metodo_PagoID, Fecha_Hora, Monto_Total) VALUES (3, 9, 1, '2026-11-03 09:12:00', '4600.00');
INSERT INTO Venta (EmpleadoID, UserID, Metodo_PagoID, Fecha_Hora, Monto_Total) VALUES (NULL, 5, 3, '2026-11-03 12:10:00', '2100.00');
INSERT INTO Venta (EmpleadoID, UserID, Metodo_PagoID, Fecha_Hora, Monto_Total) VALUES (NULL, 6, 2, '2026-11-03 14:55:00', '1100.00');
INSERT INTO Venta (EmpleadoID, UserID, Metodo_PagoID, Fecha_Hora, Monto_Total) VALUES (2, 11, 1, '2026-11-03 16:22:00', '1800.00');
INSERT INTO Venta (EmpleadoID, UserID, Metodo_PagoID, Fecha_Hora, Monto_Total) VALUES (2, 6, 1, '2026-11-04 09:05:00', '700.00');
INSERT INTO Venta (EmpleadoID, UserID, Metodo_PagoID, Fecha_Hora, Monto_Total) VALUES (NULL, 1, 2, '2026-11-04 10:38:00', '2400.00');
INSERT INTO Venta (EmpleadoID, UserID, Metodo_PagoID, Fecha_Hora, Monto_Total) VALUES (NULL, 16, 2, '2026-11-04 12:20:00', '3300.00');
INSERT INTO Venta (EmpleadoID, UserID, Metodo_PagoID, Fecha_Hora, Monto_Total) VALUES (3, 16, 2, '2026-11-04 14:54:00', '1200.00');
INSERT INTO Venta (EmpleadoID, UserID, Metodo_PagoID, Fecha_Hora, Monto_Total) VALUES (NULL, 17, 2, '2026-11-05 10:40:00', '1100.00');
INSERT INTO Venta (EmpleadoID, UserID, Metodo_PagoID, Fecha_Hora, Monto_Total) VALUES (3, 18, 1, '2026-11-06 09:08:00', '8600.00');
INSERT INTO Venta (EmpleadoID, UserID, Metodo_PagoID, Fecha_Hora, Monto_Total) VALUES (1, 20, 2, '2026-11-06 12:15:00', '1800.00');
INSERT INTO Venta (EmpleadoID, UserID, Metodo_PagoID, Fecha_Hora, Monto_Total) VALUES (NULL, 11, 2, '2026-11-06 14:55:00', '4300.00');
INSERT INTO Venta (EmpleadoID, UserID, Metodo_PagoID, Fecha_Hora, Monto_Total) VALUES (2, 20, 1, '2026-11-09 09:05:00', '3900.00');
INSERT INTO Venta (EmpleadoID, UserID, Metodo_PagoID, Fecha_Hora, Monto_Total) VALUES (3, 6, 1, '2026-11-09 10:38:00', '3900.00');
INSERT INTO Venta (EmpleadoID, UserID, Metodo_PagoID, Fecha_Hora, Monto_Total) VALUES (NULL, 1, 2, '2026-11-09 12:22:00', '3900.00');
INSERT INTO Venta (EmpleadoID, UserID, Metodo_PagoID, Fecha_Hora, Monto_Total) VALUES (6, 3, 1, '2026-11-09 16:25:00', '800.00');
INSERT INTO Venta (EmpleadoID, UserID, Metodo_PagoID, Fecha_Hora, Monto_Total) VALUES (NULL, 3, 2, '2026-11-10 09:10:00', '2100.00');
INSERT INTO Venta (EmpleadoID, UserID, Metodo_PagoID, Fecha_Hora, Monto_Total) VALUES (NULL, 17, 2, '2026-11-10 10:40:00', '7800.00');
INSERT INTO Venta (EmpleadoID, UserID, Metodo_PagoID, Fecha_Hora, Monto_Total) VALUES (3, 18, 2, '2026-11-10 12:15:00', '3200.00');
INSERT INTO Venta (EmpleadoID, UserID, Metodo_PagoID, Fecha_Hora, Monto_Total) VALUES (2, 1, 1, '2026-11-10 14:55:00', '4100.00');
INSERT INTO Venta (EmpleadoID, UserID, Metodo_PagoID, Fecha_Hora, Monto_Total) VALUES (2, 2, 2, '2026-11-10 16:22:00', '2800.00');

-- ----------------------------------------------------------------------------
-- Carrito (50 - renglones de las ventas: producto O menú del día)
-- ----------------------------------------------------------------------------
INSERT INTO Carrito (VentaID, Menu_Del_DiaID, Cantidad, Precio_Unitario) VALUES (1, 1, 1, '4200.00');
INSERT INTO Carrito (VentaID, ProductoID, Cantidad, Precio_Unitario) VALUES (2, 9, 2, '1200.00');
INSERT INTO Carrito (VentaID, Menu_Del_DiaID, Cantidad, Precio_Unitario) VALUES (3, 2, 2, '3800.00');
INSERT INTO Carrito (VentaID, ProductoID, Cantidad, Precio_Unitario) VALUES (4, 12, 2, '1300.00');
INSERT INTO Carrito (VentaID, Menu_Del_DiaID, Cantidad, Precio_Unitario) VALUES (5, 2, 1, '3800.00');
INSERT INTO Carrito (VentaID, ProductoID, Cantidad, Precio_Unitario) VALUES (6, 5, 2, '800.00');
INSERT INTO Carrito (VentaID, Menu_Del_DiaID, Cantidad, Precio_Unitario) VALUES (7, 3, 1, '3900.00');
INSERT INTO Carrito (VentaID, ProductoID, Cantidad, Precio_Unitario) VALUES (8, 3, 2, '1800.00');
INSERT INTO Carrito (VentaID, Menu_Del_DiaID, Cantidad, Precio_Unitario) VALUES (9, 5, 1, '3900.00');
INSERT INTO Carrito (VentaID, ProductoID, Cantidad, Precio_Unitario) VALUES (10, 20, 1, '4100.00');
INSERT INTO Carrito (VentaID, ProductoID, Cantidad, Precio_Unitario) VALUES (11, 1, 2, '2800.00');
INSERT INTO Carrito (VentaID, ProductoID, Cantidad, Precio_Unitario) VALUES (12, 8, 1, '1100.00');
INSERT INTO Carrito (VentaID, ProductoID, Cantidad, Precio_Unitario) VALUES (13, 13, 2, '1400.00');
INSERT INTO Carrito (VentaID, ProductoID, Cantidad, Precio_Unitario) VALUES (14, 7, 2, '700.00');
INSERT INTO Carrito (VentaID, ProductoID, Cantidad, Precio_Unitario) VALUES (15, 2, 1, '1100.00');
INSERT INTO Carrito (VentaID, Menu_Del_DiaID, Cantidad, Precio_Unitario) VALUES (16, 10, 1, '4200.00');
INSERT INTO Carrito (VentaID, ProductoID, Cantidad, Precio_Unitario) VALUES (17, 16, 1, '800.00');
INSERT INTO Carrito (VentaID, ProductoID, Cantidad, Precio_Unitario) VALUES (18, 7, 2, '700.00');
INSERT INTO Carrito (VentaID, ProductoID, Cantidad, Precio_Unitario) VALUES (19, 3, 2, '1800.00');
INSERT INTO Carrito (VentaID, ProductoID, Cantidad, Precio_Unitario) VALUES (20, 1, 1, '2800.00');
INSERT INTO Carrito (VentaID, Menu_Del_DiaID, Cantidad, Precio_Unitario) VALUES (21, 10, 1, '4200.00');
INSERT INTO Carrito (VentaID, ProductoID, Cantidad, Precio_Unitario) VALUES (22, 7, 2, '700.00');
INSERT INTO Carrito (VentaID, Menu_Del_DiaID, Cantidad, Precio_Unitario) VALUES (23, 12, 1, '3800.00');
INSERT INTO Carrito (VentaID, Menu_Del_DiaID, Cantidad, Precio_Unitario) VALUES (24, 12, 1, '3800.00');
INSERT INTO Carrito (VentaID, ProductoID, Cantidad, Precio_Unitario) VALUES (25, 12, 2, '1300.00');
INSERT INTO Carrito (VentaID, ProductoID, Cantidad, Precio_Unitario) VALUES (26, 2, 2, '1100.00');
INSERT INTO Carrito (VentaID, ProductoID, Cantidad, Precio_Unitario) VALUES (27, 14, 2, '900.00');
INSERT INTO Carrito (VentaID, Menu_Del_DiaID, Cantidad, Precio_Unitario) VALUES (28, 14, 1, '3900.00');
INSERT INTO Carrito (VentaID, ProductoID, Cantidad, Precio_Unitario) VALUES (29, 7, 1, '700.00');
INSERT INTO Carrito (VentaID, Menu_Del_DiaID, Cantidad, Precio_Unitario) VALUES (30, 15, 1, '4600.00');
INSERT INTO Carrito (VentaID, ProductoID, Cantidad, Precio_Unitario) VALUES (31, 15, 3, '700.00');
INSERT INTO Carrito (VentaID, ProductoID, Cantidad, Precio_Unitario) VALUES (32, 8, 1, '1100.00');
INSERT INTO Carrito (VentaID, ProductoID, Cantidad, Precio_Unitario) VALUES (33, 11, 2, '900.00');
INSERT INTO Carrito (VentaID, ProductoID, Cantidad, Precio_Unitario) VALUES (34, 7, 1, '700.00');
INSERT INTO Carrito (VentaID, ProductoID, Cantidad, Precio_Unitario) VALUES (35, 9, 2, '1200.00');
INSERT INTO Carrito (VentaID, ProductoID, Cantidad, Precio_Unitario) VALUES (36, 8, 3, '1100.00');
INSERT INTO Carrito (VentaID, ProductoID, Cantidad, Precio_Unitario) VALUES (37, 9, 1, '1200.00');
INSERT INTO Carrito (VentaID, ProductoID, Cantidad, Precio_Unitario) VALUES (38, 8, 1, '1100.00');
INSERT INTO Carrito (VentaID, Menu_Del_DiaID, Cantidad, Precio_Unitario) VALUES (39, 18, 2, '4300.00');
INSERT INTO Carrito (VentaID, ProductoID, Cantidad, Precio_Unitario) VALUES (40, 14, 2, '900.00');
INSERT INTO Carrito (VentaID, Menu_Del_DiaID, Cantidad, Precio_Unitario) VALUES (41, 18, 1, '4300.00');
INSERT INTO Carrito (VentaID, Menu_Del_DiaID, Cantidad, Precio_Unitario) VALUES (42, 19, 1, '3900.00');
INSERT INTO Carrito (VentaID, Menu_Del_DiaID, Cantidad, Precio_Unitario) VALUES (43, 19, 1, '3900.00');
INSERT INTO Carrito (VentaID, Menu_Del_DiaID, Cantidad, Precio_Unitario) VALUES (44, 19, 1, '3900.00');
INSERT INTO Carrito (VentaID, ProductoID, Cantidad, Precio_Unitario) VALUES (45, 5, 1, '800.00');
INSERT INTO Carrito (VentaID, ProductoID, Cantidad, Precio_Unitario) VALUES (46, 7, 3, '700.00');
INSERT INTO Carrito (VentaID, Menu_Del_DiaID, Cantidad, Precio_Unitario) VALUES (47, 20, 2, '3900.00');
INSERT INTO Carrito (VentaID, ProductoID, Cantidad, Precio_Unitario) VALUES (48, 4, 2, '1600.00');
INSERT INTO Carrito (VentaID, ProductoID, Cantidad, Precio_Unitario) VALUES (49, 20, 1, '4100.00');
INSERT INTO Carrito (VentaID, ProductoID, Cantidad, Precio_Unitario) VALUES (50, 1, 1, '2800.00');

-- ----------------------------------------------------------------------------
-- Retiro (50 - uno por venta). La franja de retiro nunca termina antes de la hora de compra
-- ----------------------------------------------------------------------------
INSERT INTO Retiro (VentaID, Estado_Retiro_ID, ID_Franja, Codigo_Retiro) VALUES (1, 3, 3, 'SJX48');
INSERT INTO Retiro (VentaID, Estado_Retiro_ID, ID_Franja, Codigo_Retiro) VALUES (2, 2, 8, '8XACQ');
INSERT INTO Retiro (VentaID, Estado_Retiro_ID, ID_Franja, Codigo_Retiro) VALUES (3, 2, 8, 'HTWBP');
INSERT INTO Retiro (VentaID, Estado_Retiro_ID, ID_Franja, Codigo_Retiro) VALUES (4, 2, 12, '7TZFN');
INSERT INTO Retiro (VentaID, Estado_Retiro_ID, ID_Franja, Codigo_Retiro) VALUES (5, 2, 14, 'AGKXQ');
INSERT INTO Retiro (VentaID, Estado_Retiro_ID, ID_Franja, Codigo_Retiro) VALUES (6, 2, 16, '7PLVF');
INSERT INTO Retiro (VentaID, Estado_Retiro_ID, ID_Franja, Codigo_Retiro) VALUES (7, 2, 3, '4JGBT');
INSERT INTO Retiro (VentaID, Estado_Retiro_ID, ID_Franja, Codigo_Retiro) VALUES (8, 2, 8, '4CMQ8');
INSERT INTO Retiro (VentaID, Estado_Retiro_ID, ID_Franja, Codigo_Retiro) VALUES (9, 2, 14, 'XXYW8');
INSERT INTO Retiro (VentaID, Estado_Retiro_ID, ID_Franja, Codigo_Retiro) VALUES (10, 2, 5, 'S9WSC');
INSERT INTO Retiro (VentaID, Estado_Retiro_ID, ID_Franja, Codigo_Retiro) VALUES (11, 2, 14, '9JB8Z');
INSERT INTO Retiro (VentaID, Estado_Retiro_ID, ID_Franja, Codigo_Retiro) VALUES (12, 3, 5, 'S7NZT');
INSERT INTO Retiro (VentaID, Estado_Retiro_ID, ID_Franja, Codigo_Retiro) VALUES (13, 2, 3, '3VPD4');
INSERT INTO Retiro (VentaID, Estado_Retiro_ID, ID_Franja, Codigo_Retiro) VALUES (14, 2, 9, 'EV8FW');
INSERT INTO Retiro (VentaID, Estado_Retiro_ID, ID_Franja, Codigo_Retiro) VALUES (15, 2, 12, '9WXZM');
INSERT INTO Retiro (VentaID, Estado_Retiro_ID, ID_Franja, Codigo_Retiro) VALUES (16, 2, 3, 'EMS2G');
INSERT INTO Retiro (VentaID, Estado_Retiro_ID, ID_Franja, Codigo_Retiro) VALUES (17, 2, 5, 'PB7RP');
INSERT INTO Retiro (VentaID, Estado_Retiro_ID, ID_Franja, Codigo_Retiro) VALUES (18, 3, 7, '5JPT9');
INSERT INTO Retiro (VentaID, Estado_Retiro_ID, ID_Franja, Codigo_Retiro) VALUES (19, 2, 12, 'JWZ3N');
INSERT INTO Retiro (VentaID, Estado_Retiro_ID, ID_Franja, Codigo_Retiro) VALUES (20, 2, 14, 'KKKJD');
INSERT INTO Retiro (VentaID, Estado_Retiro_ID, ID_Franja, Codigo_Retiro) VALUES (21, 2, 17, 'CTVMJ');
INSERT INTO Retiro (VentaID, Estado_Retiro_ID, ID_Franja, Codigo_Retiro) VALUES (22, 2, 16, 'TYJ7L');
INSERT INTO Retiro (VentaID, Estado_Retiro_ID, ID_Franja, Codigo_Retiro) VALUES (23, 2, 5, 'MDMK4');
INSERT INTO Retiro (VentaID, Estado_Retiro_ID, ID_Franja, Codigo_Retiro) VALUES (24, 2, 8, '9BVQZ');
INSERT INTO Retiro (VentaID, Estado_Retiro_ID, ID_Franja, Codigo_Retiro) VALUES (25, 2, 16, 'F74KW');
INSERT INTO Retiro (VentaID, Estado_Retiro_ID, ID_Franja, Codigo_Retiro) VALUES (26, 2, 3, 'KC8AD');
INSERT INTO Retiro (VentaID, Estado_Retiro_ID, ID_Franja, Codigo_Retiro) VALUES (27, 2, 5, 'GYSXB');
INSERT INTO Retiro (VentaID, Estado_Retiro_ID, ID_Franja, Codigo_Retiro) VALUES (28, 2, 3, 'LNF55');
INSERT INTO Retiro (VentaID, Estado_Retiro_ID, ID_Franja, Codigo_Retiro) VALUES (29, 2, 12, 'NLRQ9');
INSERT INTO Retiro (VentaID, Estado_Retiro_ID, ID_Franja, Codigo_Retiro) VALUES (30, 2, 3, 'PZL55');
INSERT INTO Retiro (VentaID, Estado_Retiro_ID, ID_Franja, Codigo_Retiro) VALUES (31, 2, 8, '7ZPV6');
INSERT INTO Retiro (VentaID, Estado_Retiro_ID, ID_Franja, Codigo_Retiro) VALUES (32, 2, 12, '8FPNH');
INSERT INTO Retiro (VentaID, Estado_Retiro_ID, ID_Franja, Codigo_Retiro) VALUES (33, 2, 14, 'FYLHM');
INSERT INTO Retiro (VentaID, Estado_Retiro_ID, ID_Franja, Codigo_Retiro) VALUES (34, 2, 3, 'P5N5B');
INSERT INTO Retiro (VentaID, Estado_Retiro_ID, ID_Franja, Codigo_Retiro) VALUES (35, 2, 5, 'KTUZ3');
INSERT INTO Retiro (VentaID, Estado_Retiro_ID, ID_Franja, Codigo_Retiro) VALUES (36, 3, 7, 'A87TY');
INSERT INTO Retiro (VentaID, Estado_Retiro_ID, ID_Franja, Codigo_Retiro) VALUES (37, 2, 12, '9JGKQ');
INSERT INTO Retiro (VentaID, Estado_Retiro_ID, ID_Franja, Codigo_Retiro) VALUES (38, 2, 5, 'Y72DW');
INSERT INTO Retiro (VentaID, Estado_Retiro_ID, ID_Franja, Codigo_Retiro) VALUES (39, 2, 3, 'SF77Y');
INSERT INTO Retiro (VentaID, Estado_Retiro_ID, ID_Franja, Codigo_Retiro) VALUES (40, 2, 8, 'GTUCF');
INSERT INTO Retiro (VentaID, Estado_Retiro_ID, ID_Franja, Codigo_Retiro) VALUES (41, 2, 12, 'CE6G2');
INSERT INTO Retiro (VentaID, Estado_Retiro_ID, ID_Franja, Codigo_Retiro) VALUES (42, 2, 3, '78BKW');
INSERT INTO Retiro (VentaID, Estado_Retiro_ID, ID_Franja, Codigo_Retiro) VALUES (43, 2, 5, 'JBG3F');
INSERT INTO Retiro (VentaID, Estado_Retiro_ID, ID_Franja, Codigo_Retiro) VALUES (44, 2, 8, 'LSBCE');
INSERT INTO Retiro (VentaID, Estado_Retiro_ID, ID_Franja, Codigo_Retiro) VALUES (45, 2, 14, 'HXKV8');
INSERT INTO Retiro (VentaID, Estado_Retiro_ID, ID_Franja, Codigo_Retiro) VALUES (46, 1, 3, 'RW7R3');
INSERT INTO Retiro (VentaID, Estado_Retiro_ID, ID_Franja, Codigo_Retiro) VALUES (47, 1, 5, 'RW4C9');
INSERT INTO Retiro (VentaID, Estado_Retiro_ID, ID_Franja, Codigo_Retiro) VALUES (48, 1, 7, 'RM6NH');
INSERT INTO Retiro (VentaID, Estado_Retiro_ID, ID_Franja, Codigo_Retiro) VALUES (49, 1, 12, '3C6MB');
INSERT INTO Retiro (VentaID, Estado_Retiro_ID, ID_Franja, Codigo_Retiro) VALUES (50, 1, 14, '8A2KP');

SET FOREIGN_KEY_CHECKS = 1;
