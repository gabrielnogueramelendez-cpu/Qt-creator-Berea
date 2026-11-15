# El Buffet de Alejandro

Sistema Click & Collect para el buffet escolar. El alumno elige del menú, paga y elige el recreo de retiro. Recibe un **código único de 5 caracteres** (por ejemplo `3C6MB`) que dicta en caja para retirar el pedido en menos de 30 segundos.

Proyecto Informático I y Bases de Datos. Aplicación de escritorio en **C++ con Qt 6** y base de datos **MySQL**.

## Estructura

```
app/               código de la aplicación Qt (.cpp, .h y .ui de cada ventana)
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

En **Ubuntu** (laboratorio): instalar la librería de desarrollo de MySQL y abrir el mismo `CMakeLists.txt` en Qt Creator.

```bash
sudo apt install libmysqlclient-dev
```

**Esta rama (`version-ubuntu-QT5`) es para Ubuntu con Qt 5** (por ejemplo Qt 5.12 o 5.15 de los paquetes de Ubuntu):

- Las pantallas (`.ui`) usan la forma de Qt 5 para los valores (`Qt::Vertical` en vez de `Qt::Orientation::Vertical`).
- CMake busca primero Qt 5. Si el CMake del laboratorio es anterior a 3.16, abrir `ElBuffetDeAlejandro.pro` (qmake) en vez de `CMakeLists.txt`.
- Si faltan los paquetes de Qt 5: `sudo apt install qtbase5-dev qtcreator`.

Desde la terminal:

```bash
cmake -S . -B build
cmake --build build
```

## Base de datos

Ver [`database/README.md`](database/README.md): son 3 schemas, 27 tablas y 27 stored procedures. Antes de abrir la app hay que ejecutar en MySQL los scripts 03, 04 y 05. La app se conecta con la API de C de MySQL y usa los stored procedures; si MySQL no está corriendo, avisa y funciona con los mismos datos en memoria, sin guardar cambios.

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
