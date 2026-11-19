# Qué modificar en el DER y en el modelo de Codd

Comparé `modelo-conceptual-DER.png` y `modelo-logico-codd.png` con la última versión de la BDD
(`03_ModeloSQL_GrupoNoguera_SanMartin.sql`, última modificación: 17/11, "menú fijo, 3 estados y SP sin DUAL",
más lo que dice `AVISO_CAMBIOS_DESDE_QT.md`).

**Resumen:** la BDD tiene **27 tablas** en 3 schemas. El DER tiene 18 entidades más 2 atributos multivaluados,
y el Codd tiene 20 tablas. Faltan 5 tablas que solo existen en el SQL y otras 2 que están en el DER pero no
aparecen en el Codd. Además hay columnas nuevas y relaciones que cambiaron.

Lo que **ya coincide** y no hay que tocar: toda la cadena geográfica (País → Provincia → Ciudad → Región →
Distrito_Escolar → Colegio), Categoría, Producto (salvo `Activo`), Detalle_Menu_Dia, Metodo_Pago,
Carrito, Venta–Usuario, Venta–Empleado, Venta–Metodo_Pago y Venta–Retiro 1:1.

---

## 1. Tablas nuevas (hay que dibujarlas en los dos diagramas)

| Tabla nueva | Columnas | Se relaciona con |
|---|---|---|
| **Curso** | ID_Curso (PK), Anio (1 a 6), Division. UNIQUE (Anio, Division) | Estudiante **M:1** Curso (Estudiante.ID_Curso, obligatorio) |
| **Profesor** | DNI (PK), CUIT, Nombre_Profesor, Apellido_Profesor, Fecha_Nacimiento, ID_Colegio (FK) | Profesor **M:1** Colegio. Usuario **1:1** Profesor (Usuario.DNI_Profesor, opcional) |
| **Rol** | RolID (PK), Nombre_Rol | Usuario **M:1** Rol (Usuario.RolID, obligatorio) |
| **Puesto** | ID_Puesto (PK), Nombre_Puesto | Empleado **M:1** Puesto (Empleado.ID_Puesto, obligatorio) |
| **Franja_Retiro** | ID_Franja (PK), Nombre_Franja, Hora_Retiro | Retiro **M:1** Franja_Retiro (Retiro.ID_Franja, obligatorio) |

## 2. Cambios en tablas que ya están

### Usuario
- Agregar: `Contrasena`, `Saldo`, `RolID` (FK a Rol) y `DNI_Profesor` (FK a Profesor, opcional, único).
- `DNI` (FK a Estudiante) pasa a ser **opcional**: un usuario es de un estudiante **o** de un profesor
  (o de ninguno, en el caso del personal). En el DER la relación "Crea" Usuario–Estudiante sigue 1:1,
  pero ahora es 0..1 de cada lado, y hay que agregar una igual con Profesor.

### Empleado
- Agregar: `ID_Puesto` (FK a Puesto), `UserID` (FK a Usuario, opcional, único), `PIN` (4 dígitos, opcional)
  y `Token_Acceso` (opcional, único).
- Sacar el atributo `Puesto_Empleado` del DER y del Codd, porque ahora es la tabla Puesto.
- Agregar la relación **Empleado 0..1 — 1 Usuario** (un empleado puede tener cuenta de usuario).
  Hoy el Empleado no está conectado con Usuario en ningún diagrama.

### Estudiante
- Agregar `ID_Curso` (FK a Curso) en el Codd y la relación con Curso en el DER.

### Menu_Del_Dia
- **Sacar `Fecha_Menu`** (DER) / `Fecha_menu` (Codd).
- **Agregar `Dia_Semana`**: 'Lunes' a 'Viernes', UNIQUE. Hay un solo menú por día hábil, o sea 5 filas.
  Los fines de semana no hay menú.

### Retiro
- Sacar `Tiempo_Retiro`. En su lugar va `ID_Franja` (FK a Franja_Retiro).
- Estado_Retiro ahora tiene 3 valores: PENDIENTE, ENTREGADO y CANCELADO. La estructura no cambia.
  En el Codd, la FK se llama `Estado_Retiro_ID` en el SQL y `EstadoID` en el dibujo: unificar el nombre.

### Venta
- `Fecha_Hora` está partido en **dos columnas: `Fecha` (DATE) y `Hora` (TIME)**. Actualizar en el DER y en el Codd.
  (Log_Auditoria ya las tenía separadas en el DER, así que queda coherente.)

### Producto
- Agregar `Activo` (booleano, por defecto verdadero).

### Log_Auditoria
- Agregar `Detalle` (texto opcional, hasta 255 caracteres). `UserID` es opcional.

### Email y Telefono (solo Codd)
- En el dibujo tienen **clave compuesta** (Email_ID + UserID, Telefono_ID + UserID). En el SQL la PK es solo
  `Email_ID` / `Telefono_ID`, y `UserID` es una FK común. Sacar la marca "PK" de `UserID` en las dos.

## 3. Cardinalidad que no coincide

- **Región – Distrito_Escolar**: los dos diagramas dicen **1:1**, pero en el SQL `Distrito_Escolar.ID_Region`
  no es UNIQUE, o sea que una región puede tener varios distritos (**1:M**). Hay dos opciones: cambiar los diagramas
  a 1:M, o agregar `UNIQUE` en `ID_Region` en el SQL. Lo más simple y consistente con el resto es cambiar los diagramas.

## 4. Falta en el Codd (está en el DER y en el SQL)

El Codd no muestra **Log_Auditoria** ni **Tipo_Evento**. Agregar:
- `Tipo_Evento` (PK `Tipo_Evento_ID`, `Nombre_Evento`).
- `Log_Auditoria` (PK `Log_ID`, FK `UserID` opcional, FK `Tipo_Evento_ID`, `Fecha`, `Hora`, `Detalle`).
- Relaciones: Usuario 1:M Log_Auditoria y Tipo_Evento 1:M Log_Auditoria.

## 5. Opcional: los 3 schemas

El SQL separa las tablas en schemas. Si querés reflejarlo, agrupar o colorear:
- **schema_geografia (6):** Pais, Provincia, Ciudad, Region, Distrito_Escolar, Colegio.
- **schema_usuarios (11):** Curso, Puesto, Tipo_Evento, Rol, Estudiante, Profesor, Usuario, Empleado, Email, Telefono, Log_Auditoria.
- **schema_comercial (10):** Categoria, Metodo_Pago, Estado_Retiro, Franja_Retiro, Producto, Menu_Del_Dia, Detalle_Menu_Dia, Venta, Carrito, Retiro.

---

## Lista de tareas (para ir tachando)

**DER**
- [ ] Entidad Curso + relación "Cursa" con Estudiante (M:1)
- [ ] Entidad Profesor + relación con Colegio (M:1) y relación con Usuario (1:1, opcional)
- [ ] Entidad Rol + relación con Usuario (M:1)
- [ ] Entidad Puesto + relación con Empleado (M:1); borrar el atributo `Puesto_Empleado`
- [ ] Entidad Franja_Retiro (Nombre_Franja, Hora_Retiro) + relación con Retiro (M:1); borrar `Tiempo_Retiro`
- [ ] Relación Empleado – Usuario (0..1 : 1)
- [ ] Usuario: agregar Contrasena y Saldo
- [ ] Empleado: agregar PIN y Token_Acceso
- [ ] Producto: agregar Activo
- [ ] Menu_Del_Dia: borrar `Fecha_Menu`, agregar `Dia_Semana`
- [ ] Venta: `Fecha_Hora` → `Fecha` y `Hora`
- [ ] Log_Auditoria: agregar Detalle
- [ ] Región – Distrito_Escolar: cambiar 1:1 por 1:M

**Codd**
- [ ] Tablas nuevas: Curso, Profesor, Rol, Puesto, Franja_Retiro, Log_Auditoria, Tipo_Evento
- [ ] Usuario: agregar Contrasena, Saldo, RolID (FK), DNI_Profesor (FK)
- [ ] Empleado: agregar ID_Puesto (FK), UserID (FK), PIN, Token_Acceso; borrar `Puesto_Empleado`
- [ ] Estudiante: agregar ID_Curso (FK)
- [ ] Menu_Del_Dia: `Fecha_menu` → `Dia_Semana`
- [ ] Retiro: `Tiempo_Retiro` → `ID_Franja` (FK); renombrar `EstadoID` a `Estado_Retiro_ID`
- [ ] Venta: `Fecha_Hora` → `Fecha` y `Hora`
- [ ] Producto: agregar Activo
- [ ] Email y Telefono: `UserID` deja de ser PK, queda solo como FK
- [ ] Región – Distrito_Escolar: cambiar 1:1 por 1:M
- [ ] Log_Auditoria: agregar Detalle

> Nota: leí los diagramas a partir de las imágenes. El del Codd tiene mucha resolución horizontal y algunos textos
> chicos, así que conviene revisar con el archivo drawio original que no se me haya escapado algún campo.
