# Base de datos

Scripts MySQL de El Buffet de Alejandro: **3 schemas y 27 tablas** en tercera forma normal (3FN), datos de ejemplo, 27 stored procedures y sus pruebas.

| Archivo | Contenido |
|---|---|
| `03_ModeloSQL_GrupoNoguera_SanMartin.sql` | Crea los schemas y las tablas |
| `04_DatosSQL_GrupoNogueraSanMartin.sql` | Datos de ejemplo |
| `05_Backend_GrupoNoguera_SanMartin.sql` | Stored procedures y reportes |
| `06_PruebasSQL_GrupoNoguera_SanMartin.sql` | Pruebas de cada SP (casos i a iv) |
| `06_PruebasSQL.xlsx` | Planilla de pruebas con el resultado de cada caso |
| `AVISO_CAMBIOS_DESDE_QT.md` | Cambios hechos en la BDD para que coincida con la app Qt |
| `modelo-conceptual-DER.png` | Diagrama Entidad-Relación (modelo conceptual) |
| `modelo-logico-codd.png` | Esquema relacional de Codd (modelo lógico) |

## Schemas y tablas

| Schema | Tablas |
|---|---|
| `schema_geografia` (6) | Pais, Provincia, Ciudad, Region, Distrito_Escolar, Colegio |
| `schema_usuarios` (11) | Curso, Puesto, Tipo_Evento, Rol, Estudiante, Profesor, Usuario, Empleado, Email, Telefono, Log_Auditoria |
| `schema_comercial` (10) | Categoria, Metodo_Pago, Estado_Retiro, Franja_Retiro, Producto, Menu_Del_Dia, Venta, Detalle_Menu_Dia, Carrito, Retiro |

## Cómo cargarla

Con MySQL Community Server corriendo en `localhost:3306`:

Ejecutar los scripts en orden: 03, 04, 05 y después 06.

```bash
mysql -u root -p < database/03_ModeloSQL_GrupoNoguera_SanMartin.sql
mysql -u root -p < database/04_DatosSQL_GrupoNogueraSanMartin.sql
mysql -u root -p < database/05_Backend_GrupoNoguera_SanMartin.sql
mysql -u root -p --force < database/06_PruebasSQL_GrupoNoguera_SanMartin.sql
```

El 06 lleva `--force` porque algunos casos esperan un error a propósito. También se pueden abrir en MySQL Workbench y ejecutarlos completos, en el mismo orden.

La aplicación necesita el servidor MySQL: si no está disponible, avisa al arrancar y se cierra.
