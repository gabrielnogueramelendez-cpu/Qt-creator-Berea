# Base de datos

Script MySQL de El Buffet de Alejandro: **3 schemas y 22 tablas** en tercera forma normal (3FN), con datos de ejemplo.

| Archivo | Contenido |
|---|---|
| `ElBuffetDeAlejandroBDD.sql` | Crea los schemas, las tablas y los datos de ejemplo |
| `modelo-conceptual-DER.png` | Diagrama Entidad-Relación (modelo conceptual) |
| `modelo-logico-codd.png` | Esquema relacional de Codd (modelo lógico) |

## Schemas y tablas

| Schema | Tablas |
|---|---|
| `schema_geografia` (6) | Pais, Provincia, Ciudad, Region, Distrito_Escolar, Colegio |
| `schema_usuarios` (7) | Estudiante, Empleado, Tipo_Evento, Usuario, Email, Telefono, Log_Auditoria |
| `schema_comercial` (9) | Categoria, Metodo_Pago, Estado_Retiro, Producto, Menu_Del_Dia, Venta, Detalle_Menu_Dia, Carrito, Retiro |

## Cómo cargarla

Con MySQL Community Server corriendo en `localhost:3306`:

```bash
mysql -u root -p < database/ElBuffetDeAlejandroBDD.sql
```

También se puede abrir el script en MySQL Workbench y ejecutarlo completo.

Si el servidor MySQL no está disponible, la aplicación sigue funcionando con los mismos datos en memoria (ver `docs/validaciones_sistema.md`, sección 6).
