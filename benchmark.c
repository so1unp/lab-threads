#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void test_fork(int count);
void test_thread(int count);

int main(int argc, char *argv[]) 
{
    int modo, count;
    
    // Chequea los  parametros
    if (argc < 3) {
        fprintf(stderr, "Uso: %s modo contador\n", argv[0]);
        fprintf(stderr, "\tmodo: 1 (crea procesos) o 2 (crea hilos)\n");
        fprintf(stderr, "\tcontador: número de procesos o hilos a generar.\n");
        exit(EXIT_FAILURE);
    }
    
    modo = atoi(argv[1]);
    count = atoi(argv[2]);
    
    if (count <= 0) {
        fprintf(stderr, "%s: el contador debe ser mayor que cero.\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (modo != 1 && modo != 2) {
        fprintf(stderr, "%s: modo invalido - usar 1 (fork) o 2 (threads)\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    
    if (modo == 1) {
        printf("Probando fork()...\n");
        test_fork(count);
    } else if (modo == 2) {
        printf("Probando pthread_create()...\n");
        test_thread(count);
    }

    exit(EXIT_SUCCESS);
}

//==========================================================================
// Código para la prueba con fork()
//==========================================================================
void test_fork(int count)
{
    pid_t pid;
    int j, status;

    for (j = 0; j < count; j++) {
        pid = fork();

        if (pid < 0) {
            perror("fork");
            exit(0);
        }        
        else if (pid == 0) {
            /* Hijo */
            exit(EXIT_SUCCESS);
        }        
        else {
            /* Padre - espera por el hijo */
            waitpid(pid, &status, 0);
        }
    }
}

//==========================================================================
// Código para la prueba con pthread_create()
//==========================================================================

void test_thread(int count) 
{
    int j;
    
    for (j = 0; j < count; j++) {
        // COMPLETAR: CREAR HILO
        // COMPLETAR: ESPERAR POR HILO
    }
}
