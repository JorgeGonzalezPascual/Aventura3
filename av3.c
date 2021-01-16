/**
 * Sistemas Operativos - AVENTURA 3
 *
 * Jorge González Pascual - Lluís Barca Pons - Joan Martorell Ferriol
 */

//#define _POSIX_C_SOURCE 200112L

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "my_lib.h"

#define NUM_THREADS 5
#define NUM_ITERATIONS 5

#define DEBUG 1

//Funciones
struct my_stack *init_stack(char *file);
void *worker(void *ptr);

//Variables
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
        printf("Hilos: %i, Iteraciones: %i \n", NUM_THREADS, NUM_ITERATIONS);
        stack = init_stack(nameStack);

        // Crear los hilos
        pthread_t threads[NUM_THREADS];

        for (int i = 0; i < NUM_THREADS; i++)
        {
            pthread_create(&threads[i], NULL, worker, NULL);
            printf("\n%d - Hilo %ld creado", i, threads[i]);
        }

        // Esperar hasta que los hilos acaben
        for (int i = 0; i < NUM_THREADS; i++)
        {
            pthread_join(threads[i], NULL);
        }

        // Guardar en la pila
        int var = my_stack_write(stack, argv[1]);
        printf("\n\nElementos escritos de la pila al fichero: %d", var);
        var = my_stack_purge(stack);
        printf("\nBytes eliminados: %d\n", var);

        return EXIT_SUCCESS;
    }
    else
    {
        fprintf(stderr, "Error de sintaxis: ./av3 nombre archivo\n");
        return EXIT_FAILURE;
    }
}

/**
 * M
 */ 
struct my_stack *init_stack(char *file)
{
    struct my_stack *stack;
    stack = my_stack_read(file);

    //Si la pila existe
    if (stack)
    {
        printf("\nLongitud inicial de la pila: %d\n", my_stack_len(stack));

        int len = my_stack_len(stack);
        // Si no hay 10 elemenotos
        if (len != NUM_THREADS)
        {
            // Si < 10: rellenar los restantes
            if (len < NUM_THREADS)
            {
                //Rellenamos la pila
                for (int i = len; i < NUM_THREADS; i++)
                {
                    int *data = malloc(sizeof(int));
                    *data = 0;
                    my_stack_push(stack, data);
                }
            }
            // Si > 10: Nos quedeamos con los 10 primeros
            else
            {
                //Vaciamos la pila
                while (my_stack_len(stack) != NUM_THREADS)
                {
                    my_stack_pop(stack);
                }
            }
        }
    }
    // Si no esta creada la pila la creamos
    else
    {
        stack = my_stack_init(sizeof(int));

        //Imprimir algo??
        while (my_stack_len(stack) != NUM_THREADS)
        {
            int *data = malloc(sizeof(int));
            *data = 0;
            my_stack_push(stack, data);
        }
    }

    //Imprimir algo???

    return stack;
}

void *worker(void *ptr)
{
    int i = 0;

    int *data;

    while (i < NUM_ITERATIONS)
    {
        // Sección crítica nº1
        pthread_mutex_lock(&mutex);
#ifdef DEBUG
        printf("\nSoy el hilo %ld ejecutando pop", pthread_self());
#endif
        data = my_stack_pop(stack);
        pthread_mutex_unlock(&mutex);

        (*data)++;

        // Sección crítica nº2
        pthread_mutex_lock(&mutex);
#ifdef DEBUG
        printf("\nSoy el hilo %ld ejecutando push", pthread_self());
#endif
        my_stack_push(stack, data);
        i++;
        pthread_mutex_unlock(&mutex);

        //i++;
    }

    pthread_exit(NULL);
}