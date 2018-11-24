#include <pthread.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

float total=0;
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
//Se define la estructura que va a tener los limites superioes e inferiores
typedef struct arg_struct {
    int inferior;
    int superior;
}suma_parcial;

void *partial_calc(void *args){ // Se define la funcion que recibe cada thread

  suma_parcial *sum = (suma_parcial *)args;
  // printf("Limite inferior: %d\n", sum-> inferior);
  // printf("Limite superior: %d\n", sum-> superior);
  float resultado_parcial =0; //Se define la variable que guarda el resultado final

  // Se realiza la sumatoria parcial basado en los limites
  for (int i = (sum->inferior); i <=(sum->superior); i++) {

    resultado_parcial=resultado_parcial+4*powf(-1,(float)(i-1))/(2.0*((float)(i-1))+1.0);
    // printf("Resultado parcial %d\n",sum->resultado);
  }
  printf("Resultado Parcial: %.*f\n",10,resultado_parcial);
  pthread_mutex_lock( &mutex1 ); // Se usa mutex lock para que se pueda agregar el resultado a la variable compartida
  total=total+resultado_parcial;
  //printf("Resultado Parcial: %f\n",total);
  pthread_mutex_unlock( &mutex1 );
  return NULL;
}


int main(int argc, char *argv[]){
  clock_t start5=clock(); // Se inicializa el cronometro del programa
  int thread_count = atoi(argv[2]);

  // printf("%d\n",thread_count);
  // pthread_t *threads = (new)[];
  // pthread_t threads[100];
  // suma_parcial sum_parc[100];
  //Se crean los arreglos de los threads y de las estructuras con los limites
  pthread_t *threads = (pthread_t*)malloc(thread_count*sizeof(pthread_t*));
  suma_parcial *sum_parc = (suma_parcial*)malloc(thread_count*sizeof(suma_parcial*));
//Se incializan los structs con los limites superiores, inferiores y los resultados parciales.
  for (int i = 0; i < thread_count; i++) {
    sum_parc[i].inferior=i*100000+1;
    sum_parc[i].superior=(i*100000)+100000;
    printf("Suma Parcial %d\n",i);
    printf("Limite inferior: %d\n",sum_parc[i].inferior);
    printf("Limite Superior: %d\n",sum_parc[i].superior);
  }

  printf("-----fin de inicializacion de de limites de sumatorios---------------------------\n");
// Se crean todos los threads recorriendo el arreglo
  for (int i = 0; i < thread_count; i++) { // Se inicializan los threads pasando los structs con los limites.
    if(pthread_create(&threads[i], NULL, partial_calc, &sum_parc[i])) {

      fprintf(stderr, "Error creating thread\n");
      return 1;

    }
  }
// Se unen todos los threads
  for (int i = 0; i < thread_count; i++) {
    if(pthread_join(threads[i], NULL)) {

      fprintf(stderr, "Error joining thread\n");
      return 2;

    }
  }
  printf("------------------------------------------------------------------\n");

//Se imprime el resultado final de la variable compoartida
  printf("Resultado total: %.*f\n",10,total);
  printf("Tiempo de ejecución: %.*f s\n",10,((double)clock()-start5)/CLOCKS_PER_SEC);

  return 0;

}
