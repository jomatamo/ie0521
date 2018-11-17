#include <stdio.h>
#include <mpi.h>

 int main(int argc, char *argv[]) {
  int root_process = 0;
  MPI_Status status;
  int my_id, num_proc, ierr, an_id, numero, maximo;
  numero =0;
  maximo=11;


//------------Se inicia la ejecucion paralela----------------------------------
  ierr = MPI_Init(&argc, &argv);
//------------------------------------------------------------------------------


while(numero<maximo){//----------------------------------------------------------------------------
    ierr = MPI_Comm_rank(MPI_COMM_WORLD, &my_id); // Se averigua el id del proceso
    if (my_id==root_process) { // Si es el proceso maestro
      numero++;
      printf("Procesador root: Incrementado, Valor Actual: %d\n",numero );
      ierr = MPI_Send( &numero, 1, MPI_INT,1, 2000, MPI_COMM_WORLD);
      printf("Procesador root: Enviado a esclavo, Valor Actual: %d\n",numero );
      ierr = MPI_Recv( &numero, 1, MPI_INT,1, 2000, MPI_COMM_WORLD, &status);
      printf("Procesador root: Recibido de esclavo, Valor Actual: %d\n",numero );

    } else { // Si es uno de los procesos esclavos

      ierr = MPI_Recv( &numero, 1, MPI_INT,0, 2000, MPI_COMM_WORLD, &status);
      // printf("Procesador 2: Recibido, Valor Actual: %d\n",numero );
      if(numero<maximo){
      numero++;
      // printf("Procesador 2: Incrementado, Valor Actual: %d\n",numero );
    } else {
      // printf("Procesador 2: Valor maximo obtenido, Valor Actual: %d\n",numero );
    }

      ierr = MPI_Send( &numero, 1, MPI_INT,0, 2000, MPI_COMM_WORLD);
      // printf("Procesador 2: Enviado, Valor Actual: %d\n",numero );
    }
}

  ierr = MPI_Finalize();

  return 0;
 }
