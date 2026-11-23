# El Buffet de Alejandro

Sistema de pedidos para el buffet escolar. El alumno elige del menú, paga y recibe un **código de retiro de 5 caracteres** (por ejemplo `3C6MB`) que dicta en la caja en el recreo siguiente.

Proyecto Informático I y Bases de Datos. Aplicación de escritorio en **C++ con Qt 6** y base de datos **MySQL**, para Ubuntu.

## Cómo está escrito el código

C++ **estructurado**:

- No hay clases propias: variables globales y funciones.
- Cada tabla que se lee de la BDD se guarda en listas paralelas (`datos.h`).
- Cada pantalla es un `.cpp` con una función `abrir_xxx()` que carga su `.ui` y conecta cada botón con una función.
- `basedatos.cpp` tiene una función por consulta o stored procedure, con la API de C de MySQL (`mysql_query`, `mysql_store_result`, `mysql_fetch_row`).
- Cada archivo empieza con una línea que dice para qué sirve, y cada función tiene arriba un comentario corto "Sirve para…".

## Estructura

```
app/main.cpp        colores, conexión a MySQL y login
app/basedatos.*     una función por consulta o SP de MySQL
app/datos.*         variables globales (usuario, productos, menú, carrito, pedidos) y las funciones que las cargan
app/pantallas.h     una función abrir_xxx() por pantalla
app/*.cpp           una pantalla por archivo: login, registro, catalogo, detalle, carrito, pago,
                    comprobante, perfil, caja, admin, desarrollo
app/*.ui            diseño de cada pantalla en Qt Designer
app/imagenes/       fotos de los 20 productos (los nombres con tilde se guardan sin tilde
                    y recursos.qrc les pone el nombre real con alias)
app/recursos.qrc    las fotos y los scripts 05 y 06 de database/ para la consola del desarrollador
database/           scripts MySQL 03 a 06, planilla de pruebas, DER y modelo lógico
CMakeLists.txt      proyecto de Qt Creator
```

## Cómo compilar en Ubuntu

1. Instalar lo necesario:

   ```bash
   sudo apt install build-essential qt6-base-dev libmysqlclient-dev cmake mysql-server
   ```

2. Cargar la base de datos: ejecutar en MySQL los scripts 03, 04 y 05 de `database/` (ver [`database/README.md`](database/README.md)).
3. Revisar la clave de MySQL en `app/basedatos.cpp` (`BDD_CLAVE`, por defecto `12341234`).
4. Abrir `CMakeLists.txt` con Qt Creator, elegir el kit de Qt 6 y ejecutar (Ctrl+R). Desde la terminal:

   ```bash
   cmake -S . -B build
   cmake --build build
   ./build/ElBuffetDeAlejandro
   ```

Si MySQL no está corriendo, la app avisa y se cierra: todos los datos salen de la base.

## Roles

| Rol | Cómo entra | Qué hace |
|---|---|---|
| Alumno o profesor | Apodo y contraseña | Menú, carrito, pago y código de retiro |
| Caja | Apodo, contraseña y PIN de 4 dígitos | Busca el código y entrega o cancela el pedido |
| Administración | Apodo y contraseña | Productos, precios, stock y auditoría |
| Desarrollo | Apodo, contraseña y token | Consola de procedimientos y pruebas |

## Horario de pedidos

Se pide de un recreo para el siguiente; en horario de clases no se toman pedidos. Para una demostración: en `app/pago.cpp`, línea 34, cambiar `return 0;` por `return 2;` (está marcada como INTERRUPTOR DE HORARIO). Después, volver a `return 0;`.

## Equipo

Noguera y San Martín.
