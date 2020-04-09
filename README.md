# Laboratorio 4 - Hilos

En este laboratorio vamos a trabajar con hilos.

:bulb: Las respuestas a las preguntas en los ejercicios pueden incluirlas en un archivo de texto con el nombre `respuestas.txt`.

:date: Fecha de entrega: 23/04

## Ejercicio 1

Completar el programa `say.c`, para que cree un hilo que imprima la palabra que se indique como parámetro una cierta cantidad de veces. El hilo debe realizar una pausa de 1 segundo entre cada impresión. Antes de empezar a imprimir la palabra, el hilo debe indicar su identificador. Antes de finalizar, se debe imprimir un aviso desde `main()`.

La ejecución del programa tendría que tener una salida similar a la siguiente:

```sh
$ bin/say ¡Hola! 5
Hilo 140346665010944
[0] ¡Hola!
[1] ¡Hola!
[2] ¡Hola!
[3] ¡Hola!
[4] ¡Hola!
El hilo 140346665010944 terminó.
$
```

Utilizar estas funciones:

* [`pthread_create()`](http://man7.org/linux/man-pages/man3/pthread_create.3.html): crea un hilo.
* [`pthread_join()`](http://man7.org/linux/man-pages/man3/pthread_join.3.html): espera a que el hilo indicado finalice.
* [`pthread_self()`](http://man7.org/linux/man-pages/man3/pthread_self.3.html): para obtener el _id_ del hilo actualmente en ejecución.
* [`pthread_exit()`](http://man7.org/linux/man-pages/man3/pthread_exit.3.html): finaliza la ejecución de un hilo.

Responder:

1. ¿Que sucede si comentan la funcion `pthred_join()` y vuelven a ejecutar el programa? ¿Por qué?
2. Mantener comentado `pthread_join()` y reemplazar en `main()` la invocación a `exit()` por una invocación a `pthread_exit()`. ¿Qué sucede ahora? ¿Por qué?

## Ejercicio 2

Completar el programa `threads.c`, para que genere tantos hilos como se le indique por la línea de comandos. Cada hilo debe imprimir su identificador y el mensaje indicado en la línea de comandos. Deben utilizar las mismas funciones que en el **Ejercicio 1**.

La ejecución del programa tendría que tener una salida similar a la siguiente:

```sh
$ bin/threads "hola mundo\!" 3
[140144208054016] hola mundo!
[140144199599872] hola mundo!
[140144191145728] hola mundo!
$
```

En este programa al hilo `main` no le interesa el resultado de los hilos creados, por lo que no debe realizar un _join_ sobre cada hilo. Utilizar la función [`pthread_detach()`](http://man7.org/linux/man-pages/man3/pthread_detach.3.html) para indicarlo.

## Ejercicio 3

En este ejercicio vamos a comparar cuanto cuesta crear un mismo número de hilos comparado con el mismo número de procesos hijos.

Completar el programa `benchmark.c` para que cree la cantidad de procesos hijos o de hilos, según se le indique por la línea de comandos.

Cada vez que se crea un proceso hijo o hilo, espera a que el mismo termine antes de continuar creando el resto. Deben completar las invocaciones necesarias para crear los hilos (las mismas que en el **Ejercicio 1**).

Para medir el tiempo de ejecución vamos a utilizar el comando [`time`](http://man7.org/linux/man-pages/man1/time.1.html):

```sh
$ /usr/bin/time -p bin/benchmark 1 1000
$ /usr/bin/time -p bin/benchmark 2 1000
```

**Nota** :warning: : es importante usar el _path_ completo (`/usr/bin/time`), para no ejecutar el comando `time` provisto por el _shell_. 

Responder:

1. ¿Cual de las dos variantes tuvo menos costo, la creación de hilos o la creación de procesos? ¿Por qué?

---

¡Fin del Laboratorio 4!
