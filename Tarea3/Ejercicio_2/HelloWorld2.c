#include <mpi.h>
#include <stdio.h>

int main(int argc, char **argv){
  int rank,root_process, ierr, size;
  MPI_Status status;


  ierr = MPI_Init(&argc, &argv);




  ierr = MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  ierr = MPI_Comm_size(MPI_COMM_WORLD, &size);

  if(my_id % 2 == 0){
    printf("Hello! my ID is an even number. I am %d of %d\n",rank,size);
  } else{
    printf("Hello! my ID is an odd number. I am %d of %d\n",rank,size);
  }

  ierr = MPI_Finalize();
  return 0;





}
