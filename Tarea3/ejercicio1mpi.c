#include <stdio.h>
#include <mpi.h>

 int main(int argc, char *argv[]) {
  int root_process = 0;
  MPI_Status status;
  int my_id, num_proc, ierr, an_id, temp;
  int orden[8];


//------------Se inicia la ejecucion paralela----------------------------------
  ierr = MPI_Init(&argc, &argv);
//------------------------------------------------------------------------------
  ierr = MPI_Comm_rank(MPI_COMM_WORLD, &my_id); // Se averigua el id del proceso
  ierr = MPI_Comm_size(MPI_COMM_WORLD, &num_procs); // Se averigua el numero de procesos

//-----------------------------------------------------------------------------
  if (my_id==root_process) { // Si es el proceso maestro
    orden[0]=my_id;
    // Se colecta la informacion que envia cada proceso sobre su id y se guarda en orden[8]
    for(an_id = 1; an_id < num_procs; an_id++) {
      ierr = MPI_Recv( &temp, 1, MPI_INT, an_id, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
      orden[an_id]=temp; //El Id de cada proceso se guarda en orden en el arrary

    }
    // Se imprimen los ID en orden
    for (int i = 0; i < 8; i++) {
      printf("Hello, World. I am %d of %d\n",i, num_procs);
    }
//-----------------------------------------------------------------------------
  } else { // Si es uno de los procesos esclavos
    //Se envia el ID al root process
    ierr = MPI_Send( &my_id, 1, MPI_INT, root_process, MPI_ANY_TAG, MPI_COMM_WORLD);
  }

  return 0;

 }
