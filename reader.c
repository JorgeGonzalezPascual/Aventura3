/**
 * Sistemas Operativos - AVENTURA 3
 *
 * Jorge González Pascual - Lluís Barca Pons - Joan Martorell Ferriol
 */

#include <stdio.h>
#include <limits.h>
#include "my_lib.h"

int main(int argc, char *argv[])
{
    struct my_stack *stack;

    if (argv[1])
    {
        stack = my_stack_read(argv[1]);

        if (stack)
        {
            int length = my_stack_len(stack);
            int sum = 0;
            int min = INT_MAX;
            int max = 0;
            int average = 0;
            printf("Longitud pila: %d", length);

            // Mientras la pila no esté vacía
            while (my_stack_len(stack))
            {
                int *data;
                data = my_stack_pop(stack);

                sum += *data;

                if (*data < min) 
                {
                    min = *data;
                }
                else if (*data > max) 
                {
                    max = *data;
                }
                
                printf("\n%d\n", *data);
            }

            if (length) 
            {
                average = sum / length;
            }

            printf("Elementos: %d Suma: %d Mínimo: %d, Máximo: %d, Media: %d\n", length, sum, min, max, average);
        }
        else
        {
            printf("Este fichero esta vacío, intenta con otro\n");
            return EXIT_FAILURE;
        }

        // Liberamos la memoria que corresponde a la pila
        my_stack_purge(stack);

        return EXIT_SUCCESS;
    }
    else
    {
        printf("La sintaxis correcta es ./reader 'nombre de fichero' \n");
        return EXIT_FAILURE;
    }
}