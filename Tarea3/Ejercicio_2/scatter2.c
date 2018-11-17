#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

 int main(int argc, char *argv[]) {
  int root_process = 0;
  int my_id, size, ierr;
  int matriz_size=3;
  int sum = 0; // Sum of counts. Used to calculate displacements


//-------------------Arreglos para las matrices-------------------------------
  int *submatriz = (int*)malloc(matriz_size*matriz_size*sizeof(int*));
  int *matriz = (int*)malloc(matriz_size*matriz_size*sizeof(int*));
  int *gather_matriz = (int*)malloc(matriz_size*matriz_size*sizeof(int));
//------------------Arreglos para los vectores---------------------------------
  int *subvector = (int*)malloc(matriz_size*matriz_size*sizeof(int*));
  int *vector = (int*)malloc(matriz_size*matriz_size*sizeof(int*));
  int *mult_m = (int*)malloc(matriz_size*sizeof(int));
//------------------Dimensiones de la matriz  ---------------------------------
  int columnas = matriz_size;
  int filas= matriz_size;

//------------Se inicia la ejecucion paralela----------------------------------
  ierr = MPI_Init(&argc, &argv);
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
  ierr = MPI_Comm_rank(MPI_COMM_WORLD, &my_id);
  ierr =MPI_Comm_size(MPI_COMM_WORLD, &size); // Se averigua el id del procesos

  int rem = (matriz_size*matriz_size)%size; // elements remaining after division among
  // printf("%d\n",my_id);

  int *sendcounts = malloc(sizeof(int)*size);
  int *displs = malloc(sizeof(int)*size);

  // calculate send counts and displacements
  for (int i = 0; i < size; i++) {
      sendcounts[i] = (matriz_size*matriz_size)/size;
      if (rem > 0) {
          sendcounts[i]++;
          rem--;
      }

      displs[i] = sum;
      sum += sendcounts[i];
  }

  // print calculated send counts and displacements for each process
  if (my_id == root_process) {
      for (int i = 0; i < size; i++) {
          printf("sendcounts[%d] = %d\tdispls[%d] = %d\n", i, sendcounts[i], i, displs[i]);
      }
  }


  if (my_id==root_process) { // Si es el proceso maestro
//Se inicializan la matriz y el vector que se van a multiplicar

    for (int cont_filas = 0; cont_filas < (columnas*filas); cont_filas=cont_filas+columnas) {
      printf("| ");
      for (int cont_col= 0; cont_col < columnas; cont_col++) {
        matriz[cont_filas+cont_col]=cont_col+(cont_filas)/filas;
        printf("%d ",matriz[cont_filas+cont_col] );
      }
      printf("|\n");
    }

    for (int cont_filas = 0; cont_filas < (columnas*filas); cont_filas=cont_filas+columnas) {
      printf("| ");
      for (int cont_col= 0; cont_col < columnas; cont_col++) {
        vector[cont_filas+cont_col]=cont_col;
        printf("%d ",vector[cont_filas+cont_col] );
      }
      printf("|\n");
    }
  }
//--------------Se hace Scatter a la matriz hacia los procesadores--------------
  ierr=MPI_Scatterv(matriz, sendcounts, displs, MPI_INT, submatriz, matriz_size*matriz_size, MPI_INT, root_process, MPI_COMM_WORLD);
  printf("Procesador %d: ", my_id);
  for (int i = 0; i < sendcounts[my_id]; i++) {
      printf("%d ", submatriz[i]);
  }
  printf("\n");
//--------------Se le hace Scatter al vector hacia los procesadores-------------
  ierr=MPI_Scatterv(vector, sendcounts, displs, MPI_INT, subvector, matriz_size*matriz_size, MPI_INT, root_process, MPI_COMM_WORLD);
  printf("Procesador %d: ", my_id);
  for (int i = 0; i < sendcounts[my_id]; i++) {
      printf("%d ", subvector[i]);
  }
  printf("\n");

//-------------------Se realiza la multiplicacion de elementos----------------
  for (int i = 0; i < sendcounts[my_id]; i++) {
    submatriz[i]=submatriz[i]*subvector[i];
  }
//--------------------------------------------------------------------------


  ierr = MPI_Gatherv(submatriz, sendcounts[my_id], MPI_INT, gather_matriz,
    sendcounts, displs, MPI_INT, root_process, MPI_COMM_WORLD);

  if(my_id == root_process){
    printf("Procesador: %d, matriz gatherv: \n", my_id);
    for (int i = 0; i < matriz_size*matriz_size; i++) {
        printf("%d ", gather_matriz[i]);
    }
    printf("\n");
    int cont=0;
    for (int i = 0; i < matriz_size; i++) {
      for(int j = 0; j < matriz_size; j++){
        mult_m[i]=mult_m[i]+gather_matriz[j+cont];

      }
      cont=cont+matriz_size;
    }
    printf("Vector resultante: \n");
    printf("| ");
    for (int i = 0; i < matriz_size; i++) {
      printf("%d ", mult_m[i]);
    }
    printf("|\n");





  }
//----------------------------------------------------------------------------
  ierr = MPI_Finalize();

  return 0;

 }
