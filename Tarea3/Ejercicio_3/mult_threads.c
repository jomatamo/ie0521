#include <pthread.h>
#include <stdio.h>


int total;
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
  pthread_mutex_lock( &mutex1 );
  for (int i = (sum->inferior); i <=(sum->superior); i++) {

    total=total+i;
    // printf("Resultado parcial %d\n",sum->resultado);
  }
  printf("Resultado Parcial: %d\n",total);
  pthread_mutex_unlock( &mutex1 );
  return NULL;
}

pthread_t threads[10];
suma_parcial sum_parc[10];

int main(){
//Se incializan los structs con los limites superiores, inferiores y los resultados parciales.
  for (int i = 0; i < 10; i++) {
    sum_parc[i].inferior=i*5+1;
    sum_parc[i].superior=(i*5)+5;
    printf("Suma Parcial %d\n",i);
    printf("Limite inferior: %d\n",sum_parc[i].inferior);
    printf("Limite Superior: %d\n",sum_parc[i].superior);
  }

  printf("-----fin de inicializacion de de limites de sumatorios---------------------------\n");

  for (int i = 0; i < 10; i++) {
    if(pthread_create(&threads[i], NULL, partial_calc, &sum_parc[i])) {

      fprintf(stderr, "Error creating thread\n");
      return 1;

    }
  }


  for (int i = 0; i < 10; i++) {
    if(pthread_join(threads[i], NULL)) {

      fprintf(stderr, "Error joining thread\n");
      return 2;

    }
  }
  printf("------------------------------------------------------------------\n");


  printf("Resultado total: %d\n",total);
  //
  //
  //
  //
  //
  //
  // suma_parcial sum1;
  // sum1.inferior=0;
  // sum1.superior=5;
  // sum1.resultado=0;
  //
  //
  // suma_parcial sum2;
  // sum2.inferior=6;
  // sum2.superior=10;
  // sum2.resultado=0;
  //
  // /* show the initial values of x and y *
  //
  // /* this variable is our reference to the second thread */
  //
  // pthread_t sumando1;
  // pthread_t sumando2;
  //
  //
  // /* create a second thread which executes inc_x(&x) */
  // if(pthread_create(&sumando1, NULL, partial_calc, &sum1)) {
  //
  //   fprintf(stderr, "Error creating thread\n");
  //   return 1;
  //
  // }
  // if(pthread_create(&sumando2, NULL, partial_calc, &sum2)) {
  //
  //   fprintf(stderr, "Error creating thread\n");
  //   return 1;
  //
  // }
  //
  //
  //
  //
  //
  //
  // /* increment y to 100 in the first thread */
  // if(pthread_join(sumando1, NULL)) {
  //
  //   fprintf(stderr, "Error joining thread\n");
  //   return 2;
  //
  // }
  //
  // if(pthread_join(sumando2, NULL)) {
  //
  //   fprintf(stderr, "Error joining thread\n");
  //   return 2;
  //
  // }
  //
  // /* show the results - x is now 100 thanks to the second thread */
  // printf("resulado 1: %d, resultado 2: %d\n", sum1.resultado, sum2.resultado);

  return 0;

}
