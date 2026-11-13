# Guion de exposición

Presentación: https://gabrielnogueramelendez-cpu.github.io/Qt-creator-Berea/ (21 diapositivas, unos **12 minutos**).

**Reparto:** Noguera abre con el problema (1 a 7), San Martín explica el sistema (8 a 13), Noguera vuelve con la base y la competencia (14 a 16), San Martín cierra con la planificación (17 a 19) y los dos terminan juntos (20 y 21).

Las diapositivas marcadas con (*) se pueden saltear si hay menos tiempo. Lo que está en *cursiva entre paréntesis* es una acción en pantalla, no se dice.

---

## Noguera · el problema (≈ 3:30)

### 1 · Portada (0:20)
"Buenos días. Somos Noguera y San Martín, y les presentamos El Buffet de Alejandro: un sistema para pedir antes del recreo y retirar en segundos. Ese código que ven en pantalla, 3C6MB, es el que genera nuestra aplicación al confirmar un pedido. Lo vamos a ver varias veces."

### 2 · El recreo dura 10 minutos (0:30)
"Empecemos por un número. El recreo dura 10 minutos. Comprar en el buffet lleva entre 4 y 7. Miren la fila de la pantalla: se atiende de a uno, en una sola caja, y cada cobro tarda entre 30 y 60 segundos. En el peor caso, 7 de los 10 minutos se van esperando. Quedan 3 para comer."

### 3 · La gente no espera (0:30)
"Esto no pasa solo en los colegios. Según una encuesta de WaitWhile de 2023, el 82 % de las personas evita los locales con filas visibles, y el 73 % abandona la compra si tiene que esperar más de 5 minutos. Para un buffet, abandonar la fila es una venta perdida y un alumno sin comer."

### 4 · Convivencia (0:30) (*)
"Además de tiempo, la fila genera conflicto. La Capital contó el caso de un alumno del Colegio San José de Rosario al que le rompieron la nariz en una pelea que empezó en la cola de la cantina. Y la provincia de Buenos Aires ya lo reconoce: la Resolución Conjunta 1490/2021 regula los buffets justamente para evitar la aglomeración en los recreos."

### 5 · Árbol de problemas (0:45)
"Armamos el árbol de problemas. Arriba están los efectos: pérdidas de entre 20 % y 35 % en ventas, alumnos frustrados que abandonan la fila, y comer apurado o llegar tarde a clase. En el centro, el problema: amontonamiento y filas largas en recreos de 10 minutos. Y abajo, tres causas. Una: el cobro manual en efectivo, de 30 a 60 segundos por alumno. Dos: todos salen al recreo al mismo tiempo. Tres: no existe ninguna forma digital de reservar."

*(Tocar cada causa para mostrar sus raíces.)*

### 6 · Árbol de objetivos (0:30) (*)
"Si damos vuelta el árbol, cada causa se convierte en un medio: pago elegido de antemano con el vuelto calculado, menú disponible en el sistema, y Click & Collect con validación por código. Los fines responden a los tres efectos: ventas recuperadas, mostrador ordenado y comer tranquilo."

### 7 · Otros ya lo probaron (0:30)
"No somos los primeros en pensar esto. OrderEAT nació de estudiantes que vivían el mismo caos en su colegio y hoy procesa más de 2 millones de dólares con decenas de colegios. En Chile, estudiantes de la PUCV hicieron pedidos agendados con casilleros inteligentes. Nuestra diferencia la vamos a ver en la competencia. Ahora, San Martín les cuenta cómo funciona."

---

## San Martín · el sistema (≈ 4:15)

### 8 · Tres pasos (0:30)
"El sistema se resume en tres pasos. Uno: el alumno elige del menú, y si un producto no tiene stock, no se puede pedir. Dos: paga y elige el recreo en el que va a retirar. Tres: recibe un código y lo dicta en caja. Por ejemplo, este: 8A2KP."

### 9 · Cómo viaja un pedido (0:40)
"Este dibujo muestra lo más importante de la arquitectura: el alumno y el cajero nunca hablan entre sí. Los dos leen y escriben la misma base de datos."

*(Seguir el punto amarillo.)*

"Uno: al confirmar, se crea la venta y se descuenta el stock del producto. Dos: la base genera el retiro con su código único y se lo devuelve al alumno. Tres: en el recreo, el cajero escribe el código y la base trae el pedido. Cuatro: al entregar, el retiro pasa a ENTREGADO y ese código no se puede volver a usar."

### 10 · Cuatro puertas de entrada (0:30) (*)
"Es una aplicación de escritorio en C++ con Qt, con cuatro roles. El alumno se registra con DNI y correo institucional. El cajero entra con su legajo y un PIN de turno de 4 dígitos. Administración carga productos y revisa la auditoría. Y desarrollo tiene una consola técnica para probar la conexión con MySQL. Un alumno no puede entrar como cajero, ni al revés."

### 11 · Recorrido del alumno (1:00)
"Estas son capturas reales de la aplicación."

*(Pasar los 5 pasos o tocar Reproducir.)*

"Ingreso: elige su rol y entra con DNI o correo. Menú: combos, salados, dulces y bebidas con precio. Carrito: hasta 10 unidades por producto y nunca más que el stock. Pago: elige el recreo de retiro, los horarios son los reales del colegio, y el método de pago; en efectivo, el vuelto se calcula solo. Y por último, el comprobante con su código."

*(Si hay tiempo, repetir el recorrido en vivo desde Qt Creator.)*

### 12 · El mostrador (0:45)
"Acá se gana el tiempo. La barra de arriba es la fila tradicional: de 4 a 7 minutos. La de abajo es con código: menos de 30 segundos. Están a la misma escala. El cajero escribe el código, el sistema lo valida en menos de 5 segundos, y el pedido pasa de pendiente a entregado. Un pedido entregado no se puede entregar dos veces."

### 13 · Reglas dentro del sistema (0:30) (*)
"Las reglas del buffet están en el sistema, no en la cabeza del cajero. Dos ejemplos: máximo 10 unidades por producto, para evitar compras excesivas en el recreo, y anular un pedido exige un motivo y devuelve el stock. En total son más de 50 validaciones, todas documentadas."

---

## Noguera · base y competencia (≈ 1:50)

### 14 · Base de datos (0:40)
"Debajo de todo hay una base MySQL con 22 tablas en tres schemas: comercial, usuarios y geografía. Venta se relaciona uno a uno con Retiro, que guarda el código. Si el servidor MySQL no responde, la aplicación sigue funcionando con los mismos datos en memoria."

### 15 · Competencia (0:40)
"Frente a lo que existe: una app SaaS como OrderEAT cobra comisión y abono mensual y depende de los datos móviles de los alumnos. Un sistema POS pide licencias y hardware. Y el cobro manual, además de lento, hace perder entre 20 % y 30 % de las ventas. Nuestro sistema valida en menos de 5 segundos, descuenta el stock automáticamente, deja todo auditado y cuesta cero en licencias."

### 16 · Propuesta de valor (0:30) (*)
"Si lo resumimos en una frase: un sistema propio que saca la fila del recreo, con menos de 30 segundos por alumno, sin licencias ni comisiones y con cada operación registrada."

---

## San Martín · planificación (≈ 1:40)

### 17 · Matriz de Marco Lógico (0:40) (*)
"Planificamos con la Matriz de Marco Lógico. El fin es mejorar la compra y venta en el buffet. El propósito es pasar de 4 a 7 minutos por compra a menos de 30 segundos. Los componentes: la app de reservas, el cobro y la base de stock. Y las actividades estimadas: 20 horas de código, 6 de interfaz y 12 de base de datos, con costo cero. Una aclaración: la matriz original dice PIN de 4 dígitos; el código de retiro del alumno es de 5 caracteres."

### 18 · Cómo sabremos que funcionó (0:30)
"Estas son las metas. La compra completa baja de 7 minutos a menos de 30 segundos. La validación en caja, de 60 segundos a menos de 5. Esperamos entre 20 % y 30 % más de ventas y menos del 1 % de errores en los códigos. Se miden con las estadísticas de la base y con cronometraje en el recreo."

### 19 · Riesgos (0:30)
"Identificamos seis riesgos. Si el alumno no tiene señal, el código queda guardado en el dispositivo. Si falla el cobro, hay reintento automático y cobro manual. Si se satura en el recreo, usamos caché. El más importante es la adopción: si nadie lo usa, ningún indicador mejora. Por eso, afiches, una promoción para el primer pedido y capacitación corta para el personal."

---

## Los dos · cierre (≈ 1:00)

### 20 · Preguntas que nos pueden hacer (0:30)
Si quedan menos de 2 minutos, saltear esta diapositiva y pasar a la 21. Si el panel pregunta, tocar la tarjeta que corresponda:

- **¿Por qué no OrderEAT?** "Cobra comisión y abono, y depende de los datos móviles. El nuestro cuesta cero en licencias y funciona con la red del colegio."
- **¿Y si se cae MySQL?** "La app sigue respondiendo con los mismos datos en memoria."
- **¿El pago es real?** "Hoy se registra: efectivo con vuelto, saldo de cuenta o comprobante de Mercado Pago. Conectar una pasarela real es el paso siguiente."
- **¿Qué tan firmes son los números?** "Los de abandono y filas son de estudios externos. Los tiempos del buffet son estimaciones nuestras: falta cronometrarlos en el recreo."

### 21 · Cierre (0:30)
"Volvemos al código de la portada: 3C6MB, ahora entregado. El pedido se completó, sin fila. Gracias. Quedamos a disposición para preguntas. El código, la base de datos y los documentos están en el repositorio que aparece a la derecha."

---

## Consejos

- **Ensayo:** cronometrar con el reloj del celular. La barra de arriba de la presentación va de 10:35 a 10:45 a lo largo de las diapositivas y sirve de referencia visual.
- **Animaciones:** empiezan solas al entrar a cada diapositiva. Esperar 2 o 3 segundos antes de hablar de ellas.
- **Teclas:** flechas para avanzar, `S` abre las notas del orador en otra ventana y `F` pone pantalla completa.
- **Decirlo antes de que pregunten:** todavía no hay mediciones reales en el colegio, y los stored procedures que menciona el informe de mercado los simula la consola de desarrollador (el script SQL no los define).
