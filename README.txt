Laboratorio 4 - Hilos

💡 Las respuestas a las preguntas en los ejercicios pueden incluirlas en
un archivo de texto con el nombre respuestas.txt.

📅 Fecha de entrega: 23/04

Ejercicio 1

Completar el programa say.c para que imprima un mensaje una cierta
cantidad de veces desde un hilo. El mensaje y el número de repeticiones
se tienen que indicar desde la línea de comandos. El hilo debe esperar 1
segundo entre cada printf(). Debe ser posible también indicar un número
distinto de segundos desde la línea de comandos, como tercer parámetro
opcional. Antes de empezar a imprimir el mensaje el hilo debe indicar su
identificador. Desde main() se debe esperar a que el hilo termine su
ejecución e imprimir un aviso antes de finalizar.

La ejecución tendría que tener una salida similar a la siguiente:

    $ bin/say "¡Hola\!" 5
    Hilo 140346665010944
    [0] ¡Hola!
    [1] ¡Hola!
    [2] ¡Hola!
    [3] ¡Hola!
    [4] ¡Hola!
    El hilo 140346665010944 terminó.
    $

Utilizar las siguientes funciones para trabajar con hilos:

-   pthread_create(): crea un hilo.
-   pthread_join(): espera a que el hilo indicado finalice.
-   pthread_self(): para obtener el id del hilo actualmente en
    ejecución.
-   pthread_exit(): finaliza la ejecución de un hilo.

Responder:

1.  ¿Que sucede si comentan la funcion pthred_join() y main() sigue
    utilizando exit()? Justificar.
2.  Mantener comentado pthread_join() y reemplazar en main() la
    invocación a exit() por pthread_exit(). ¿Qué sucede ahora?
    Justificar.

Ejercicio 2

Estudiar las llamadas al sistema que invoca say.c durante su ejecución,
indicando que imprima sólo un mensaje (para hacer más fácil de seguir la
traza):

    $ strace bin/say hola 1 > /dev/null

Responder:

1.  ¿Cuál es la llamada al sistema que crea el hilo? Justificar.

Ejercicio 3

Completar el programa threads.c para que cree n hilos:

-   El número n debe ser indicado como parámetro en la línea de
    comandos.
-   Cada hilo debe tener asignado un id único (un número entero). El
    primer hilo creado debe tener el id 1, el segundo el id 2 y así
    sucesivamente.
-   Cada hilo debe imprimir por la salida estándar su identificador y su
    id. Para obtener el identificador del hilo emplear la función
    pthread_self().
-   Cada hilo debe esperar un número aleatorio de segundos, no mayor a
    10, antes de terminar. Utilizar la función sleep().
-   Al finalizar, cada hilo debe indicar el número de segundos que
    durmió como parámetro para pthread_exit().
-   El hilo main debe esperar a que el resto de los hilos finalicen, e
    imprimir cuantos segundos durmió cada hilo. Estos datos se obtiene
    mediante pthread_join().

Por ejemplo, si se ejecuta el programa indicando que se creen 3 hilos,
debe obtenerse una salida similar a la siguiente:

    $ bin/threads 3
    Hilo 3434: id 2
    Hilo 3432: id 3
    Hilo 3431: id 1
    Hilo 3434 durmió 4 segundos.
    Hilo 3432 durmió 2 segundos.
    Hilo 3431 durmió 7 segundos.
    bin/threads: todos los hilos terminaron.
    $

Ejercicio 4

Vamos a comparar el costo de crear hilos comparado con el de crear
procesos. Completar el programa benchmark.c para generar una cierta
cantidad de procesos hijos o de hilos, según se le indique por la línea
de comandos.

El programa, cada vez que crea un proceso o hilo, espera a que este
termine antes de continuar. Completar las invocaciones necesarias (las
mismas que en el Ejercicio 1).

Para medir el tiempo de ejecución vamos a utilizar el comando time:

    $ /usr/bin/time -p bin/benchmark 1 1000
    $ /usr/bin/time -p bin/benchmark 2 1000

Nota ⚠️ : es importante usar el path completo (/usr/bin/time), para no
ejecutar el comando time provisto por el shell.

Responder:

1.  ¿Cual de las dos variantes tuvo menos costo, la creación de hilos o
    la creación de procesos? Justificar.

Ejercicio 5

En este ejercicio vamos a implementar hilos a nivel de usuario en xv6.
Para esto copiar estos dos archivos en el directorio de xv6:

-   uthread.c: contiene tanto el programa de prueba como la librería de
    hilos a nivel de usuario (creación de hilos y su planificación).

-   uthread_switch.S: contiene el código en ensamblador para realizar el
    cambio de contexto entre dos hilos.

Modificar luego el archivo Makefile, agregando _uthread a la lista
UPROGS y lo siguiente al final del archivo:

    _uthread: uthread.o uthread_switch.o
        $(LD) $(LDFLAGS) -N -e main -Ttext 0 -o _uthread uthread.o uthread_switch.o $(ULIB)
        $(OBJDUMP) -S _uthread > uthread.asm

Para probar que se haya agregado correctamente el nuevo programa,
ejecutar xv6 y luego invocar el programa uthread. Al ejecutarlo, tiene
indicar que todos los hilos finalizaron:

    xv6...
    cpu0: starting 0
    sb: size 1000 nblocks 941 ninodes 200 nlog 30 logstart 2 inodestart 32 bmap start 58
    init: starting sh
    $ uthread
    all threads ended.
    $

Sin embargo, como puede verse en la ejecución, ningun hilo se ejecutó ya
que falta implementar el cambio de contexto en el archivo
uthread_switch.S.

Cambio de contexto

En uthread.c hay dos variables globales current_thread y next_thread,
que son punteros a una estructura de tipo thread. Esta estructura
contiene la pila de un hilo y una copia de su puntero a pila (sp o stack
pointer). Para realizar el cambio de contexto, la función uthread_switch
debe guardar el estado del hilo actual en current_thread, restaurar el
estado del hilo indicado en next_thread y hacer que current_thread
apunte a next_thread.

En el cambio de contexto de un hilo a otro tiene se tienen que realizar
las siguientes acciones:

1.  Guardar el estado de la CPU del hilo actual en la pila, ccon la
    instrucción pushal. Esta instrucción almacena los registros EAX,
    EBX, ECX, EDX, ESP, EBP, ESI, EDI en la pila.

2.  Almacenar el puntero a pila actual (current_thread->sp = esp).

3.  Actualizar el registro ESP para que apunte a la pila del hilo a
    ejecutar (esp = next_thread->sp).

4.  Actualizar el valor de current_thread para que apunte al hilo a
    ejecutar.

5.  Asignar cero a la variable next_thread.

6.  Restaurar el contexto del hilo a ejecutar con la instrucción popal,
    que restaura en los registros de la CPU los valores que almaceno una
    invocación previa de pushal.

7.  Realizar el return de la función, con la instrucción ret. De esta
    manera, se continua con la ejecución del hilo seleccionado.

Una ejecución correcta de uthread.c tendría que dar como resultado:

    $ uthread
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

Responder

1.  Completar el código de cambio de contexto en el archivo
    uthread_switch.S y comprobar que el programa uthread.c ejecute
    correctamente.

2.  ¿Por qué basta con ejecutar popa para continuar con la ejecución de
    next_thread?

3.  ¿Qué tipo de política de planificación se implementa? Justificar.

4.  La implementación tiene un bug: si agregamos un tercer hilo, este no
    se ejecuta hasta que los dos primeros finalicen. Indicar por qué
    sucede esto y modificar el código para solucionarlo.

5.  Modificar el código para que cuando no existan más hilos para
    ejecutar, se retome el hilo main.

------------------------------------------------------------------------

¡Fin del Laboratorio 4!
