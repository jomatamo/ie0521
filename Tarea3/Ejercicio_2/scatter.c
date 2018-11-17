#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

 int main(int argc, char *argv[]) {
  int root_process = 0;
  int *sendcounts;    // array describing how many elements to send to each process
  int *displs;
  int rem = (matriz_size*matriz_size)%size; // elements remaining after division among processes
  int sum = 0; // Sum of counts. Used to calculate displacements
  // int num_proc = atoi(argv[0]);
  // printf("%d\n",num_proc );
  MPI_Status status;
  int my_id, size, ierr;
  int matriz_size=3;
  int *submatriz = (int*)malloc(matriz_size*matriz_size*sizeof(int*));
  int *subvector = (int*)malloc(matriz_size*matriz_size*sizeof(int*));

  int *matriz = (int*)malloc(matriz_size*matriz_size*sizeof(int*));
  int *vector = (int*)malloc(matriz_size*matriz_size*sizeof(int*));
  int *gather_matriz = (int*)malloc(matriz_size*matriz_size*sizeof(int*));

  int columnas = matriz_size;
  int filas= matriz_size;

  // for (int cont_filas = 0; cont_filas < (columnas*filas); cont_filas=cont_filas+columnas) {
  //   printf("| ");
  //   for (int cont_col= 0; cont_col < columnas; cont_col++) {
  //     matriz[cont_filas+cont_col]=cont_col+(cont_filas)/filas;
  //     printf("%d ",matriz[cont_filas+cont_col] );
  //   }
  //   printf("|\n");
  // }

  // printf("-------------------------------------------------------------------\n");
  //
  // for (int cont_filas = 0; cont_filas < (columnas*filas); cont_filas=cont_filas+columnas) {
  //   printf("| ");
  //   for (int cont_col= 0; cont_col < columnas; cont_col++) {
  //     vector[cont_filas+cont_col]=cont_col;
  //     printf("%d ",vector[cont_filas+cont_col] );
  //   }
  //   printf("|\n");
  // }






//------------Se inicia la ejecucion paralela----------------------------------
  ierr = MPI_Init(&argc, &argv);
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
  ierr = MPI_Comm_rank(MPI_COMM_WORLD, &my_id);
  ierr =MPI_Comm_size(MPI_COMM_WORLD, &size); // Se averigua el id del procesos
  // printf("%d\n",my_id);
  if (my_id==root_process) { // Si es el proceso maestro


    for (int cont_filas = 0; cont_filas < (columnas*filas); cont_filas=cont_filas+columnas) {
      printf("| ");
      for (int cont_col= 0; cont_col < columnas; cont_col++) {
        matriz[cont_filas+cont_col]=cont_col+(cont_filas)/filas;
        printf("%d ",matriz[cont_filas+cont_col] );
      }
      printf("|\n");
    }
  }

  ierr = MPI_Scatter(matriz,3,MPI_INT,submatriz,3,MPI_INT,0,MPI_COMM_WORLD);

  printf("Procesador %d, submatriz: \n",my_id );
  printf("|");
  for (int i = 0; i < 3; i++) {
    printf(" %d ",submatriz[i]);
  }
  printf("|\n");

//----------------------------------------------------------------------------
  ierr = MPI_Finalize();

  return 0;

 }
