/*PRACTICA 2 EJERCICIO 1
Implemente un programa que cree un número N de hebras. Cada hebra creará 2 números
aleatorios flotantes (consulte Moodle para la generación de aleatorios) y guardará su
suma en una variable para ello, que será devuelta a la hebra principal o llamadora (la
que invocó pthread_create()).
La hebra principal ira recogiendo los valores devueltos por las N hebras y los ira
sumando en una variable no global cuyo resultado mostrará al final por pantalla. Para
ver que los resultados finales son los que usted espera, muestre los 2 números que va
creando cada hebra y su suma, de forma que pueda comparar esas sumas parciales con la
suma final que realizará el main() o hebra principal. Utilice macros definidas y
comprobación de errores en sus programas (errno y comprobación de valores devueltos en
cada llamada, con sus posibles alternativas).*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>

struct AddThread{

    float total;

};

void* threadChildFunction(void* arg){

    float number1 = (rand() % 1000) / 10.0; // Número aleatorio flotante entre 0.0 y 99.9
    float number2 = (rand() % 1000) / 10.0;
    struct AddThread* add = (struct AddThread*) malloc(sizeof(struct AddThread));

    add->total = number1 + number2;
    printf("Hilo %lu: Números generados: %.2f y %.2f, Suma: %.2f\n", pthread_self(), number1, number2, number1 + number2);

    pthread_exit((void*) add);

}

int main(int argc, char** argv) {

    if(argc != 2){
        printf("Uso: %s <numero_de_hilos>\n", argv[0]);
        return -1;
    }

    int nThreads = atoi(argv[1]);

    pthread_t threadChild;
    struct AddThread* add;
    add->total = 0.0;

    struct AddThread* results;

    for(int i=0; i<nThreads; i++){
        
        if(pthread_create(&threadChild, NULL, threadChildFunction, NULL) != 0){

            printf("Error al crear el hilo hijo: %s\n", strerror(errno));
            exit(-1);

        }
    }

    for(int i=0; i<nThreads; i++){

        if(pthread_join(threadChild, (void**)&results) != 0){

            fprintf("Error al unir el hilo hijo: %s\n", strerror(errno));
            exit(-1);

        }

        printf("Hilo padre: Resultado recibido del hilo hijo: %.2f\n", results->total);
        add->total += results->total;

    }

    printf("Suma total de todas las hebras: %.2f\n", add->total);
    return 0;
}