# Base de datos

MySQL 8 de El Buffet de Alejandro: **3 schemas y 25 tablas** en tercera forma normal (3FN).

## Entrega del Hito 2

Los scripts se ejecutan **en este orden** y se pueden volver a ejecutar las veces que haga falta:

| Archivo | Punto | Contenido |
|---|---|---|
| `03_ModeloSQL_Grupo1.sql` | 3 | Borra y crea los 3 schemas, las 25 tablas, las restricciones (PK, FK, UNIQUE, CHECK) y 2 índices |
| `04_DatosSQL_Grupo1.sql` | 4 | Vacía las tablas con `TRUNCATE` y las vuelve a cargar: 50 ventas y 20 o más registros en el resto. `Log_Auditoria` queda vacía porque la llenan los SPs |
| `05_Backend_Grupo1.sql` | 5 | 24 Stored Procedures (13 funcionalidades, 8 reportes, auditoría y 2 tuneados) y el auxiliar `sp_registrar_log` |
| `06_PruebasSQL_Grupo1.sql` | 6 | Script de pruebas: 4 casos por SP (éxito, sin coincidencias, NULL/límite y consistencia) |
| `06_PruebasSQL.xlsx` | 6 | Las 96 pruebas cargadas en la plantilla de la cátedra, con el resultado real obtenido en MySQL 8.0 |

Desde la terminal:

```bash
mysql -u root -p < database/03_ModeloSQL_Grupo1.sql
mysql -u root -p < database/04_DatosSQL_Grupo1.sql
mysql -u root -p < database/05_Backend_Grupo1.sql
```

En MySQL Workbench: abrir cada archivo y ejecutarlo completo (rayo), en el mismo orden.

## Schemas y tablas

| Schema | Tablas |
|---|---|
| `schema_geografia` (6) | Pais, Provincia, Ciudad, Region, Distrito_Escolar, Colegio |
| `schema_usuarios` (9) | Curso, Puesto, Tipo_Evento, Estudiante, Usuario, Empleado, Email, Telefono, Log_Auditoria |
| `schema_comercial` (10) | Categoria, Metodo_Pago, Estado_Retiro, Franja_Retiro, Producto, Menu_Del_Dia, Detalle_Menu_Dia, Venta, Carrito, Retiro |

## Stored Procedures

| Tipo | SPs |
|---|---|
| Usuarios | `sp_registrar_usuario`, `sp_iniciar_sesion`, `sp_cerrar_sesion`, `sp_cambiar_contrasena` |
| Productos y menú | `sp_alta_producto`, `sp_modificar_stock`, `sp_baja_producto`, `sp_alta_menu_dia` |
| Ventas y retiros | `sp_registrar_venta`, `sp_agregar_producto_carrito`, `sp_agregar_menu_carrito`, `sp_generar_retiro`, `sp_actualizar_estado_retiro` |
| Reportes (8) | `sp_reporte_ventas_por_categoria`, `sp_reporte_top_productos`, `sp_reporte_metodos_pago`, `sp_reporte_ventas_diarias`, `sp_reporte_top_estudiantes`, `sp_reporte_rendimiento_empleados`, `sp_reporte_retiros_por_franja`, `sp_reporte_demanda_menu_dia` |
| Auditoría | `sp_reporte_auditoria(fechaInicio, fechaFin, UserID, Tipo_Evento_ID)`: usuario y tipo de evento son opcionales (NULL = todos) |
| Tuning | `sp_reporte_top_productos_tuneado`, `sp_reporte_ventas_diarias_tuneado` |

Flujo de una compra: `sp_registrar_venta` → `sp_agregar_producto_carrito` / `sp_agregar_menu_carrito` (controlan stock y suman el total) → `sp_generar_retiro` (cierra la venta y da el código de 5 caracteres) → `sp_actualizar_estado_retiro` (el cajero entrega o cancela; si cancela, devuelve el stock).

Todos los SPs devuelven una columna `estado` con `OK` o `ERROR`, y cada funcionalidad deja su evento en `Log_Auditoria`.

## Usuarios de prueba

- Estudiantes: UserID 1 a 20. Ejemplo: `jorgedah09` / `Jorgedah#2026`.
- Personal del buffet: UserID 21 a 30. Ejemplo: cajera `marcela.ibanez` / `MarcelaBuffet#2026`. Encargado: `alejandro.funes` / `AlejandroBuffet#2026`.

Las contraseñas se guardan cifradas con `SHA2(clave, 256)`. Las de prueba están escritas en `04_DatosSQL_Grupo1.sql`.

## Diagramas

`modelo-conceptual-DER.png` (Chen) y `modelo-logico-codd.png` (Codd) son los del Hito 1. **Hay que actualizarlos** para que coincidan con el script nuevo (ver la lista de cambios en la sección siguiente).

### Cambios del modelo respecto del Hito 1

1. Tablas nuevas: `Curso`, `Puesto` y `Franja_Retiro` (eran atributos categorizables en texto libre: consigna 1.b).
2. `Estudiante.ID_Curso` (FK a Curso).
3. `Empleado.ID_Puesto` reemplaza a `Puesto_Empleado`, y se agrega `Empleado.UserID` (FK única a Usuario) para que el personal pueda entrar al sistema.
4. `Usuario`: `DNI` pasa a ser opcional y único (un empleado no tiene DNI de estudiante), `Apodo` es único y obligatorio, y se agrega `Contrasena` (cifrada).
5. `Log_Auditoria.Detalle`: qué se hizo en cada evento.
6. `Producto.Activo`: baja lógica.
7. `Retiro`: `Tiempo_Retiro` (texto) pasa a `ID_Franja` (FK a Franja_Retiro), y `EstadoID` se renombra `Estado_Retiro_ID`.
8. Renombres: `Carrito.precio_Unitario` → `Precio_Unitario`; la PK `Detalle_Menu_Dia` → `Detalle_Menu_DiaID`.
9. Índices: `Venta(Fecha_Hora)` y `Log_Auditoria(Fecha)`, usados por el tuning y la auditoría.

Si MySQL no está disponible, la aplicación sigue funcionando con datos en memoria (ver `docs/validaciones_sistema.md`, sección 6).
