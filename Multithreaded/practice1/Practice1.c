#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>

/*

Implemente un programa que cree un vector de 10 elementos relleno con números aleatorios
entre 1 y 9. No cree el vector de manera global, declárelo en el main(). Reparta ese vector entre 2
hebras o 5 hebras a partes iguales, según se indique por línea de argumentos un 2 o un 5, de forma
que cada hebra sume la parte del vector que le corresponda y se lo devuelva al hilo principa, el cual
mostrará la suma de los resultados devuelto por las hebrsa creadas.
Ejemplo de invocación del programa para crear 6 hebras que se repartan el vector: ./a.out 5

*/

void* addVector(void *arg);

int main(int argc, char** argv) {

    int vector[10];
    int nArg = atoi(argv[1]);

    if (nArg != 2 && nArg != 5) {
        printf("Error: El número de hilos debe ser 2 o 5.\n");
        return -1;
    }

    pthread_t threads[nArg];
    int segmentSize = 10 / nArg;
    int* ret;
    int total = 0;

    srand(time(NULL));
    for (int i = 0; i < 10; i++) {
        vector[i] = (rand() % 9) + 1; // Números aleatorios entre 1 y 9
        printf("Vector principal %d: %d\n", i, vector[i]);
    }


    for(int i=0; i<nArg; i++){

        /*----Creamos el vector auxiliar----*/

        int* aux = (int*) malloc((segmentSize + 1) * sizeof(int));
        aux[0] = segmentSize;

        for(int j=0; j<segmentSize; j++){

            aux[j+1] = vector[(i*segmentSize) + j];

        }
        /*----Terminamos de crear el vector----*/

        if(pthread_create(&threads[i], NULL, addVector, (void*) aux) != 0){
            printf("Error al crear el hilo: %s\n    ", strerror(errno));
            return -1;
        }
    }

    for(int i=0; i<nArg; i++){

        //Recogemos las hebras
        if(pthread_join(threads[i],(void **) &ret) != 0){
            printf("Error al unir el hilo: %s\n    ", strerror(errno));
            return -1;
        }


        printf("Total parcial del hilo %ld: %d\n", threads[i], *ret);
        total += *ret;

    }

    printf("La suma total es: %d\n", total);

    return 0;

}

void* addVector(void *arg) {
    int* vector = (int*) arg;
    int n = vector[0];
    int *sum = (int*) malloc(sizeof(int));
    *sum = 0;

    for(int i=0; i<n; i++){
        *sum += vector[i+1];
    }

    
    pthread_exit((void*) sum);
}

