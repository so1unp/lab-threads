#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>     // exit(), malloc()
#include <unistd.h>     // sleep()
#include <stdint.h>     // intptr_t
#include <string.h>     // strlen, strcpy

void* saluda(void*);

int contador;
char *palabra;
int sleeptime = 1;

int main(int argc, char* argv[])
{
    // Identificador del hilo (similar a pid_t)
    pthread_t hilo;

    // Verifica el número de argumentos
    if (argc < 3) {
        fprintf(stderr, "Uso: %s palabra contador [segundos]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Cuentas veces se debe imprimir el saludo
    contador = atoi(argv[2]);
    if (contador <= 0) {
        fprintf(stderr, "Error: contador debe ser mayor a 0.\n");
        exit(EXIT_FAILURE);
    }

    // Cuanto dormir
    if (argc == 4) {
        sleeptime = atoi(argv[3]);
        if (sleeptime <= 0) {
            fprintf(stderr, "Error: segundos debe ser mayor a cero.\n");
            exit(EXIT_FAILURE);
        }
    }

    palabra = (char*) malloc((strlen(argv[1] + 1)) * sizeof(char));
    strcpy(palabra, argv[1]);

    int ret = 0;

    // COMPLETAR: Crear el hilo
    ret = pthread_create(&hilo, NULL, saluda, NULL);

    // Chequea por error en la creación del hilo
    if (ret != 0) {
        perror("pthread_create");
    }

    // COMPLETAR: Espera a que el hilo finalice
    ret = pthread_join(hilo, NULL);
    
    // Chequea por error al esperar por la finalización del hilo
    if (ret != 0) {
        perror("pthread_join");
    }

    printf("El hilo %ld finalizó.\n", hilo);

    // Finaliza el proceso
    exit(EXIT_SUCCESS);
}
    
// Esta es la función que ejecuta el hilo
void* saluda(void* param)
{
    int i;

    printf("Hilo %ld\n", pthread_self());

    // Imprime varias veces hola por la salida estándar
    for(i = 0; i < contador; i++)
    {
        sleep(sleeptime);
        printf("[%d] %s\n", i, palabra);
    }

    // COMPLETAR: Finaliza el hilo
    pthread_exit(NULL);
}
