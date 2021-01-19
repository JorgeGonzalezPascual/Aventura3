/**
 * Sistemas Operativos - AVENTURA 3
 *
 * Jorge González Pascual - Lluís Barca Pons - Joan Martorell Ferriol
 */

// Librerias
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "my_lib.h"

// Constantes
#define NUM_THREADS 10
#define NUM_ITERATIONS 1000000

#define DEBUG 0

// Funciones
struct my_stack *init_stack(char *file);
void *worker(void *ptr);
void filling();

// Variables
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static struct my_stack *stack;

/**
 * Main principal del programa
 * 
 */
int main(int args, char *argv[])
{
    char *nameStack = argv[1];

    if (nameStack)
    {
        stack = init_stack(nameStack);
        printf("Hilos: %i, Iteraciones: %i \n", NUM_THREADS, NUM_ITERATIONS);

        // Crear los hilos
        pthread_t threads[NUM_THREADS];

        for (int i = 0; i < NUM_THREADS; i++)
        {
            pthread_create(&threads[i], NULL, worker, NULL);
            printf("%d) Hilo %ld creado\n", i, threads[i]);
        }

        // Esperar hasta que los hilos acaben
        for (int i = 0; i < NUM_THREADS; i++)
        {
            pthread_join(threads[i], NULL);
        }

        // Guardar en la pila
        int var = my_stack_write(stack, argv[1]);
        printf("Longitud de la pila: %d\n", my_stack_len(stack));
        printf("Elementos escritos de la pila al fichero: %d\n", var);
        var = my_stack_purge(stack);
        printf("Bytes eliminados: %d\n\n", var);

        return EXIT_SUCCESS;
    }
    else
    {
        fprintf(stderr, "Error de sintaxis: ./av3 nombre archivo\n");
        return EXIT_FAILURE;
    }
}

/**
 * Método que inicializa la pila
 */
struct my_stack *init_stack(char *file)
{
    //struct my_stack *stack;
    stack = my_stack_read(file);

    // Si la pila existe
    if (stack)
    {
        printf("Longitud inicial de la pila: %d\n", my_stack_len(stack));

        // Si no hay 10 elemenotos
        if (my_stack_len(stack) != NUM_THREADS)
        {
            // Si < 10: rellenar los restantes
            if (my_stack_len(stack) < NUM_THREADS)
            {
                // Rellenamos la pila hasta NUM_THREADS
                filling();
            }
            // Si > 10: Los ignoramos
        }
    }
    // Si no esta creada la pila la creamos
    else
    {
        filling();
    }

    printf("Longitud final de la nueva pila: %d\n", my_stack_len(stack));

    return stack;
}

/**
 * Método que rellena la pila con los valores que faltan para su tratamiento
 */
void filling()
{
    struct my_stack *auxStack;

    printf("Stack->size: %ld\n", sizeof(int));
    //stack = my_stack_init(sizeof(int));
    printf("Contenido inicial de la pila:\n");

    auxStack = stack;
    while (my_stack_len(auxStack) != 0)
    {
        int *data;
        data = my_stack_pop(auxStack);
        printf("%d\n", *data);
    }

    printf("Longitud inicial de la pila: %d\n", my_stack_len(stack));
    printf("Apilar contenido para el tratamiento:\n");

    while (my_stack_len(stack) != NUM_THREADS)
    {
        int *data = malloc(sizeof(int));
        *data = 0;
        printf("%d\n", *data);
        my_stack_push(stack, data);
    }
}

/**
 * Método que regula la actividad de los hilos mediante un semáforo
 */ 
void *worker(void *ptr)
{
    int i = 0;

    while (i < NUM_ITERATIONS)
    {
        // Sección crítica nº1
        pthread_mutex_lock(&mutex);
#if DEBUG
        printf("\nSoy el hilo %ld ejecutando pop", pthread_self());
#endif
        int *data = my_stack_pop(stack);
        pthread_mutex_unlock(&mutex);

        (*data)++;

        // Sección crítica nº2
        pthread_mutex_lock(&mutex);
#if DEBUG
        printf("\nSoy el hilo %ld ejecutando push", pthread_self());
#endif
        my_stack_push(stack, data);
        i++;
        pthread_mutex_unlock(&mutex);
    }

    pthread_exit(0);
}
