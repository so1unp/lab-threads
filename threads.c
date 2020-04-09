#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

char *palabra;

void* hilo(void *t)
{
    printf("[%ld] %s\n", pthread_self(), palabra);
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    pthread_t *threads;
    int rc; 
    long t;

    // Comprueba el número de argumentos
    if (argc != 3) {
        fprintf(stderr, "Uso: %s mensaje hilos\n", argv[0]);
        fprintf(stderr, "\tmensaje: la palabra a imprimir.\n");
        fprintf(stderr, "\thilos: el número de hilos a crear.\n");
        exit(EXIT_FAILURE);
    }

    // Número de hilos a crear
    int n = atoi(argv[2]);
    if (n <= 0) {
        fprintf(stderr, "Error: hilos debe ser mayor a cero.\n");
        exit(EXIT_FAILURE);
    }

    palabra = (char*) malloc((strlen(argv[1])+1) * sizeof(char));
    strcpy(palabra, argv[1]);

    // Arreglo de identificadores de hilos
    threads = (pthread_t*) malloc(n * sizeof(pthread_t));

    for(t = 0; t < n; t++) {
        rc = pthread_create(&threads[t], NULL, hilo, NULL);

        if (rc) {
            perror("pthread_create");
        }

        pthread_detach(threads[t]);
    }

    // Finaliza el hilo main()
    pthread_exit(NULL);
}
