# El Buffet de Alejandro

Sistema Click & Collect para el buffet escolar. El alumno elige del menú, paga y elige el recreo de retiro. Recibe un **código único de 5 caracteres** (por ejemplo `3C6MB`) que dicta en caja para retirar el pedido en menos de 30 segundos.

Proyecto Informático I y Bases de Datos. Aplicación de escritorio en **C++ con Qt 6** y base de datos **MySQL**.

> **Branch `version-estructurada`**: la lógica de la app está escrita en **C++ estructurado** (variables globales y funciones, sin clases propias), con la misma estructura que los ejemplos del profesor Pablo Andrés Linares, y usa la base de datos de `database/` sin cambios. Ver [Versión estructurada](#versión-estructurada).

## Estructura

```
app/main.cpp       conexión a MySQL y apertura del login
app/basedatos.*    una función por consulta o SP de MySQL (API de C, material 006)
app/datos.*        variables globales (usuario, productos, carrito, pedidos) y las funciones que las cargan
app/pantallas.h    una función abrir_xxx() por pantalla
app/*.cpp          lógica de cada pantalla (login, registro, catálogo, detalle, carrito, pago,
                   comprobante, perfil, caja, admin, desarrollo)
app/*.ui           diseño de cada pantalla en Qt Designer (sin cambios)
app/recursos.qrc   incluye 05 y 06 de database/ para la consola de Desarrollo
database/          scripts MySQL 03 a 06, planilla de pruebas, DER y modelo lógico
docs/              material de cátedra: investigación, requerimientos, métricas y riesgos,
                   marco lógico, informe de mercado y lista de validaciones
presentacion/      presentación HTML (reveal.js)
CMakeLists.txt     proyecto de Qt Creator
```

## Cómo compilar

1. Abrir `CMakeLists.txt` con **Qt Creator** (Qt 6, módulos Widgets y Sql).
2. Elegir un kit (por ejemplo *Desktop Qt 6 MinGW 64-bit*) y compilar.

Requiere **MySQL Server 8.0** instalado en `C:/Program Files/MySQL/MySQL Server 8.0` (de ahí salen `mysql.h`, `libmysql.lib` y las DLL que CMake copia al lado del `.exe`). Usuario, contraseña y puerto de la conexión están en `app/basedatos.cpp`.

Desde la terminal:

```bash
cmake -S . -B build
cmake --build build
```

## Base de datos

Ver [`database/README.md`](database/README.md): son 3 schemas, 27 tablas y 27 stored procedures. Antes de abrir la app hay que ejecutar en MySQL los scripts 03, 04 y 05. La app se conecta con la API de C de MySQL y usa los stored procedures. Si MySQL no está corriendo, avisa y se cierra: todos los datos salen de la base.

## Versión estructurada

### Cómo está armado el código

Igual que en `ejemplobd01.cpp` a `ejemplobd04.cpp` del material 006:

- `main()` abre la conexión con `mysql_init()` y `mysql_real_connect()` y la cierra con `mysql_close()` al salir.
- En `basedatos.cpp` hay una función por consulta, que arma el `CALL` o el `SELECT` con un `std::string`, lo manda con `mysql_query()` y devuelve el `MYSQL_RES*` de `mysql_store_result()`.
- Cada pantalla recorre el resultado con `mysql_fetch_row()`, lee `fila[i]` y lo libera con `mysql_free_result()`.
- No hay clases propias. Los datos compartidos son variables globales en listas paralelas (`datos.h`) y cada pantalla es un archivo `.cpp` con funciones `static`. `abrir_xxx()` carga el `.ui` sobre una ventana común (`QWidget`, `QMainWindow` o `QDialog`) y conecta cada botón con su función mediante `QObject::connect`.
- Las validaciones y las reglas las hace la base de datos (FK, UNIQUE, CHECK y los HANDLER de cada SP). La app muestra el resultado.

### Material de Bases de Datos por bimestre (Prof. Pablo Andrés Linares)

| Bimestre | Material | Dónde está en el proyecto |
|---|---|---|
| 1.º | 001 Nivelación y normalización. Ejemplos DDL y DML | `03_ModeloSQL` (27 tablas en 3FN, 3 schemas, PK, FK, UNIQUE y CHECK) y `04_DatosSQL` (INSERT). DER y modelo de Codd en `database/` |
| 2.º | 002 Subconsultas y conjuntos | Subconsultas en los SP de `05_Backend` (por ejemplo `sp_menu_del_dia` y los reportes) y en `obtener_pedidos()` de `basedatos.cpp` |
| 2.º | 003 Blindaje y auditoría | `DECLARE EXIT HANDLER FOR SQLEXCEPTION` con `GET DIAGNOSTICS` en los 27 SP, `Log_Auditoria` y el reporte de auditoría (5.d). En la app, `error_del_sp()` lee el `estado = 'ERROR'` que devuelve el HANDLER |
| 3.º | 004 Optimización de consultas | Los 2 reportes tuneados de `05_Backend` (5.e), con sus pruebas en `06_PruebasSQL` |
| 3.º | 005 Consultas avanzadas y agrupamientos | Los 8 reportes de `05_Backend` (5.c): `GROUP BY`, `HAVING`, `COUNT`, `SUM`, `MIN`, `MAX` y `BETWEEN` |
| 4.º | 006 Conectar el gestor con C | Toda la app: `basedatos.cpp` (una función por SP, como `buscar_por_rango_fechas` de `ejemplobd04`), `datos.cpp` y las pantallas |
| Todo el año | Template de casos de prueba | `06_PruebasSQL` y `06_PruebasSQL.xlsx` (casos i a iv de cada SP) |

### Compilar en Ubuntu (como en la guía 006)

```bash
sudo apt install qt6-base-dev libmysqlclient-dev
cmake -S . -B build
cmake --build build
./build/ElBuffetDeAlejandro
```

En Windows se abre `CMakeLists.txt` con Qt Creator, igual que antes.

## Roles

| Rol | Cómo entra | Qué hace |
|---|---|---|
| Alumno o profesor | Apodo y contraseña | Menú, carrito, pago y código de retiro |
| Cajero | Apodo, contraseña y PIN de 4 dígitos | Busca el código, entrega o anula con motivo |
| Administración | Apodo y contraseña del encargado general | Productos, precios, stock y auditoría |
| Desarrollo | Apodo, contraseña y token de acceso | Consola de conexión MySQL y procedimientos |

Las reglas de cada pantalla están en [`docs/validaciones_sistema.md`](docs/validaciones_sistema.md).

## Presentación

En `presentacion/` (21 diapositivas). Para generarla:

```bash
cd presentacion
npm ci
npm run build
```

El resultado es un solo archivo, `presentacion/dist/index.html`, que se abre con doble clic y funciona sin internet. Teclas: flechas para avanzar, `S` para las notas del orador y `F` para pantalla completa.

## Material de cátedra

| Documento | Contenido |
|---|---|
| [`docs/investigacion.md`](docs/investigacion.md) | Casos y estadísticas sobre las filas en buffets |
| [`docs/requerimientos.md`](docs/requerimientos.md) | Requerimientos funcionales y no funcionales, e historias de usuario |
| [`docs/metricas-y-riesgos.md`](docs/metricas-y-riesgos.md) | KPIs y plan de mitigación de riesgos |
| [`docs/marco-logico.md`](docs/marco-logico.md) | Matriz de Marco Lógico |
| [`docs/informe-mercado.md`](docs/informe-mercado.md) | Competencia, propuesta de valor y ventaja competitiva |
| [`docs/validaciones_sistema.md`](docs/validaciones_sistema.md) | Lista maestra de validaciones de la app |

## Equipo

Noguera y San Martín.
