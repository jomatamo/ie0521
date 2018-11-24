#include <pthread.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

float total=0;
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;

typedef struct arg_struct {
    int inferior;
    int superior;
}suma_parcial;
/* this function is run by the second thread */
void *partial_calc(void *args){
/* increment x to 100 */
  suma_parcial *sum = (suma_parcial *)args;
  // printf("Limite inferior: %d\n", sum-> inferior);
  // printf("Limite superior: %d\n", sum-> superior);
  float resultado_parcial =0;


  for (int i = (sum->inferior); i <=(sum->superior); i++) {

    resultado_parcial=resultado_parcial+4*powf(-1,(float)(i-1))/(2.0*((float)(i-1))+1.0);
    // printf("Resultado parcial %d\n",sum->resultado);
  }
  printf("Resultado Parcial: %.*f\n",10,resultado_parcial);
  pthread_mutex_lock( &mutex1 );
  total=total+resultado_parcial;
  //printf("Resultado Parcial: %f\n",total);
  pthread_mutex_unlock( &mutex1 );
  return NULL;
}


int main(int argc, char *argv[]){
  clock_t start5=clock();
  int thread_count = atoi(argv[2]);

  // printf("%d\n",thread_count);
  // pthread_t *threads = (new)[];
  // pthread_t threads[100];
  // suma_parcial sum_parc[100];
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

  for (int i = 0; i < thread_count; i++) { // Se inicializan los threads pasando los structs con los limites.
    if(pthread_create(&threads[i], NULL, partial_calc, &sum_parc[i])) {

      fprintf(stderr, "Error creating thread\n");
      return 1;

    }
  }

  for (int i = 0; i < thread_count; i++) {
    if(pthread_join(threads[i], NULL)) {

      fprintf(stderr, "Error joining thread\n");
      return 2;

    }
  }
  printf("------------------------------------------------------------------\n");


  printf("Resultado total: %.*f\n",10,total);
  printf("Tiempo de ejecución: %.*f s\n",10,((double)clock()-start5)/CLOCKS_PER_SEC);

  return 0;

}
