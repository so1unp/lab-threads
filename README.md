# Laboratorio 5 - Hilos

:bulb: Las respuestas a las preguntas en los ejercicios pueden incluirlas en un archivo de texto con el nombre del ejercicio. Por ejemplo `ejercicio1.txt`.

## Ejercicio 1

Completar el programa `say.c` para que, desde un hilo, imprima un mensaje la cantidad de veces indicada. El mensaje y el número de repeticiones se indican desde la línea de comandos. El hilo debe esperar 1 segundo entre cada `printf()`. Debe ser posible también indicar un número distinto de segundos desde la línea de comandos, como tercer parámetro opcional. Antes de empezar a imprimir el mensaje el hilo debe imprimir su identificador. Desde `main()` se debe esperar a que el hilo termine su ejecución e imprimir un aviso antes de finalizar.

La ejecución tendría que tener una salida similar a la siguiente:

```sh
$ bin/say "¡Hola\!" 5
Hilo 140346665010944
[0] ¡Hola!
[1] ¡Hola!
[2] ¡Hola!
[3] ¡Hola!
[4] ¡Hola!
El hilo 140346665010944 terminó.
$
```

Utilizar las siguientes funciones para trabajar con hilos:

* [`pthread_create()`](http://man7.org/linux/man-pages/man3/pthread_create.3.html): crea un hilo.
* [`pthread_join()`](http://man7.org/linux/man-pages/man3/pthread_join.3.html): espera a que el hilo indicado finalice.
* [`pthread_self()`](http://man7.org/linux/man-pages/man3/pthread_self.3.html): para obtener el _id_ del hilo actualmente en ejecución.
* [`pthread_exit()`](http://man7.org/linux/man-pages/man3/pthread_exit.3.html): finaliza la ejecución de un hilo.

Responder:

1. No realizar la invocación de `pthred_join()`. Si `main()` sigue utilizando `exit()`, ¿Que sucede? Justificar.
2. Sin agregar la invocación a `pthread_join()`, reemplazar en `main()` la invocación a `exit()` por `pthread_exit()`. ¿Qué sucede ahora? Justificar.

## Ejercicio 2

Estudiar las llamadas al sistema que invoca `say.c` durante su ejecución. Indicar que imprima sólo un mensaje para hacer más fácil de seguir la traza:

```sh
$ strace bin/say hola 1 > /dev/null
```

Responder:

1. ¿Cuál es la llamada al sistema que crea el hilo? Justificar.

## Ejercicio 3

Completar el programa [`threads.c`](threads.c) para que cree *n* hilos:

* El número *n* debe ser indicado como parámetro en la línea de comandos.
* Cada hilo debe tener asignado un _id_ único (un número entero). El primer hilo creado debe tener el _id_ 1, el segundo el _id_ 2 y así sucesivamente.
* Cada hilo debe imprimir por la salida estándar su *identificador* y _id_. Para obtener el identificador del hilo emplear la función [`pthread_self()`](http://man7.org/linux/man-pages/man3/pthread_self.3.html).
* Cada hilo debe esperar un número aleatorio de segundos no mayor a 10, antes de terminar. Utilizar la función [`sleep()`](http://man7.org/linux/man-pages/man3/sleep.3.html).
* Al finalizar, cada hilo debe indicar el número de segundos que durmió como parámetro al invocar [`pthread_exit()`](http://man7.org/linux/man-pages/man3/pthread_exit.3.html).
* El hilo `main` debe esperar a que el resto de los hilos finalicen, e imprimir cuantos segundos durmió cada hilo. Estos datos se obtiene mediante [`pthread_join()`](http://man7.org/linux/man-pages/man3/pthread_join.3.html).

Por ejemplo, si se ejecuta el programa indicando que se creen 3 hilos, debe obtenerse una salida similar a la siguiente:

```bash
$ bin/threads 3
Hilo 3434: id 2
Hilo 3432: id 3
Hilo 3431: id 1
Hilo 3434 durmió 4 segundos.
Hilo 3432 durmió 2 segundos.
Hilo 3431 durmió 7 segundos.
bin/threads: todos los hilos terminaron.
$
```

## Ejercicio 4

Vamos a comparar el costo de crear hilos comparado con el de crear procesos. Completar el programa `benchmark.c` para generar una cierta cantidad de procesos hijos o de hilos, según se le indique por la línea de comandos.

El programa, cada vez que crea un proceso o hilo, espera a que este termine antes de continuar. Completar las invocaciones necesarias (las mismas que en el **Ejercicio 1**).

Para medir el tiempo de ejecución vamos a utilizar el comando [`time`](http://man7.org/linux/man-pages/man1/time.1.html):

```sh
$ /usr/bin/time -p bin/benchmark 1 1000
$ /usr/bin/time -p bin/benchmark 2 1000
```

**Nota** :warning: : es importante usar el _path_ completo (`/usr/bin/time`), para no ejecutar el comando `time` provisto por el _shell_. 

Responder:

1. ¿Cual de las dos variantes tuvo menos costo, la creación de hilos o la creación de procesos? Justificar.

## Ejercicio 5

En este ejercicio vamos a implementar hilos a nivel de usuario en _xv6_. Para esto copiar estos archivos en el directorio de _xv6_:

* `uthread.c`: contiene un programa de prueba que utiliza la librería de hilos a nivel de usuario.

* `uthreadlib.c` y `uthreadlib.h`: contienen la implementación de una librería de hilos a nivel de usuario.

* `uthread_switch.S`: contiene el código en ensamblador para realizar el cambio de contexto entre dos hilos.

Modificar luego el archivo `Makefile`, agregando `_uthread` a la lista `UPROGS` y lo siguiente al final del archivo:

```Makefile
_uthread: uthread.o uthreadlib.o uthread_switch.o
	$(LD) $(LDFLAGS) -N -e main -Ttext 0 -o _uthread uthread.o uthreadlib.o uthread_switch.o $(ULIB)
	$(OBJDUMP) -S _uthread > uthread.asm
```

Para probar que se haya agregado correctamente el nuevo programa, ejecutar _xv6_ y luego invocar el programa `uthread`. Al ejecutarlo, tiene que indicar que todos los hilos finalizaron:

```sh
xv6...
cpu0: starting 0
sb: size 1000 nblocks 941 ninodes 200 nlog 30 logstart 2 inodestart 32 bmap start 58
init: starting sh
$ uthread 2 1
all threads ended.
$
```

Como puede verse en la ejecución, ningun hilo se ejecutó ya que falta implementar el cambio de contexto en el archivo `uthread_switch.S`.

### Cambio de contexto

En `uthreadlib.c` hay dos variables `current_thread` y `next_thread`, que son punteros a una estructura de tipo `thread`. Esta estructura contiene la pila de un hilo y una copia de su puntero a pila (`sp` o _stack pointer_). Para realizar el cambio de contexto, la función `uthread_switch` debe guardar el estado del hilo actual en `current_thread`, restaurar el estado del hilo indicado en `next_thread` y hacer que `current_thread` apunte a `next_thread`.

En el cambio de contexto de un hilo a otro tiene se tienen que realizar las siguientes acciones:

1. Guardar el estado de la CPU del hilo actual en la pila, ccon la instrucción `pushal`. Esta instrucción almacena los registros `EAX`, `EBX`, `ECX`, `EDX`, `ESP`, `EBP`, `ESI`, `EDI` en la pila.

2. Almacenar el puntero a pila actual (`current_thread->sp = esp`).

3. Actualizar el registro `ESP` para que apunte a la pila del hilo a ejecutar (`esp = next_thread->sp`).

4. Actualizar el valor de `current_thread` para que apunte al hilo a ejecutar.

5. Asignar cero a la variable `next_thread`.

6. Restaurar el contexto del hilo a ejecutar con la instrucción `popal`, que restaura en los registros de la CPU los valores que almaceno una invocación previa de `pushal`.

7. Realizar el _return_ de la función, con la instrucción `ret`. De esta manera, se continua con la ejecución del hilo seleccionado.

Una ejecución correcta de `uthread.c` tendría que dar como resultado:

```bash
$ uthread 2 5
my thread running
Thread 0x2D68: 0
my thread running
Thread 0x4D70: 0
Thread 0x2D68: 1
Thread 0x4D70: 1
Thread 0x2D68: 2
Thread 0x4D70: 2
Thread 0x2D68: 3
Thread 0x4D70: 3
Thread 0x2D68: 4
Thread 0x4D70: 4
my thread: exit
my thread: exit
thread_schedule: no more runnable threads
all threads ended.
$
```

## Responder

1. Completar el código de cambio de contexto en el archivo `uthread_switch.S` y comprobar que el programa `uthread.c` ejecute correctamente.

2. ¿Por qué basta con ejecutar `popa` para continuar con la ejecución de `next_thread`?

3. ¿Qué tipo de política de planificación se implementa? Justificar.

4. La implementación tiene una limitación: si ejecutamos más de dos hilos, el tercero no se ejecuta hasta que finalicen los dos primeros. Indicar por qué sucede esto y modificar el código para solucionarlo.

5. Modificar el código para que cuando no existan más hilos para ejecutar, se retome el hilo `main`.

---

¡Fin del Laboratorio 5!
