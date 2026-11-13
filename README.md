# El Buffet de Alejandro

Sistema Click & Collect para el buffet escolar. El alumno elige del menú, paga y elige el recreo de retiro. Recibe un **código único de 5 caracteres** (por ejemplo `3C6MB`) que dicta en caja para retirar el pedido en menos de 30 segundos.

Proyecto Informático I y Bases de Datos. Aplicación de escritorio en **C++ con Qt 6** y base de datos **MySQL**.

## Estructura

```
app/               código de la aplicación Qt (.cpp, .h y .ui de cada ventana)
app/resources/     styles.qss, la hoja de estilos de la app
database/          script MySQL, DER y modelo lógico
docs/              material de cátedra: investigación, requerimientos, métricas y riesgos,
                   marco lógico, informe de mercado y lista de validaciones
presentacion/      presentación HTML (reveal.js)
CMakeLists.txt     proyecto de Qt Creator
```

## Cómo compilar

1. Abrir `CMakeLists.txt` con **Qt Creator** (Qt 6, módulos Widgets y Sql).
2. Elegir un kit (por ejemplo *Desktop Qt 6 MinGW 64-bit*) y compilar.
3. Al compilar, `app/resources/styles.qss` se copia junto al ejecutable.

Desde la terminal:

```bash
cmake -S . -B build
cmake --build build
```

## Base de datos

Ver [`database/README.md`](database/README.md): son 3 schemas y 22 tablas. Si MySQL no está corriendo, la app funciona igual con los mismos datos en memoria.

## Roles

| Rol | Cómo entra | Qué hace |
|---|---|---|
| Alumno | DNI o correo @bue.edu.ar | Menú, carrito, pago y código de retiro |
| Cajero | Legajo y PIN de turno de 4 dígitos | Busca el código, entrega o anula con motivo |
| Administración | Usuario de administración | Productos, precios, stock y auditoría |
| Desarrollo | Usuario técnico y token | Consola de conexión MySQL y procedimientos |

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
| [`docs/guion-exposicion.md`](docs/guion-exposicion.md) | Guion para exponer la presentación (Noguera y San Martín, unos 12 minutos) |

## Equipo

Noguera y San Martín.
