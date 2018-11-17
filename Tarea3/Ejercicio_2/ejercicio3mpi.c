#include <stdio.h>
#include <mpi.h>
#include <string.h>

 int main(int argc, char *argv[]) {
  int root_process = 0;
  MPI_Status status;
  int my_id, num_proc, ierr;
  char pt0[]="pt0";
  char pt1[]="pt1";
  char pt2[]="pt2";
  char pt3[]="pt3";
  char msg[4];
  char temp[4];


//------------Se inicia la ejecucion paralela----------------------------------
  ierr = MPI_Init(&argc, &argv);
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
  ierr = MPI_Comm_rank(MPI_COMM_WORLD, &my_id);
  ierr =MPI_Comm_size(MPI_COMM_WORLD, &num_proc); // Se averigua el id del procesos
  // printf("%d\n",my_id);
  if (my_id==root_process) { // Si es el proceso maestro
    memcpy(&msg,&pt0, sizeof msg);
    ierr = MPI_Send( &msg, 4, MPI_CHAR,my_id+1, 2000, MPI_COMM_WORLD);
    printf("Procesador root: Enviando Mensaje: %s\n",msg);
    ierr = MPI_Recv( &msg, 4, MPI_INT,num_proc-1, 2000, MPI_COMM_WORLD, &status);
    printf("Procesador root: Recibieno Mensaje: %s\n",msg);
//------------------------------------------------------------------------------
} else if (my_id<num_proc-1){ // Si es uno de los procesos esclavos

    ierr = MPI_Recv( &msg, 4, MPI_CHAR,my_id-1,2000, MPI_COMM_WORLD, &status);
    printf("Procesador %d : Recibieno Mensaje: %s\n",my_id,msg);

    if (strcmp(pt0,msg)==0) {
      memcpy(&msg,&pt1, sizeof msg);
    } else if (strcmp(pt1,msg)==0) {
      memcpy(&msg,&pt2, sizeof msg);
    } else if (strcmp(pt2,msg)==0) {
      memcpy(&msg,&pt3, sizeof msg);
    } else if (strcmp(pt3,msg)==0) {
      memcpy(&msg,&pt0, sizeof msg);
    }

    ierr = MPI_Send( &msg, 4, MPI_CHAR,my_id+1, 2000, MPI_COMM_WORLD);
    printf("Procesador %d: Enviando Mensaje: %s\n",my_id,msg);
  } else {
    ierr = MPI_Recv( &msg, 4, MPI_CHAR,my_id-1, 2000, MPI_COMM_WORLD, &status);
    printf("Procesador %d : Recibieno Mensaje: %s\n",my_id,msg);

    if (strcmp(pt0,msg)==0) {
      memcpy(&msg,&pt1, sizeof msg);
    } else if (strcmp(pt1,msg)==0) {
      memcpy(&msg,&pt2, sizeof msg);
    } else if (strcmp(pt2,msg)==0) {
      memcpy(&msg,&pt3, sizeof msg);
    } else if (strcmp(pt3,msg)==0) {
      memcpy(&msg,&pt0, sizeof msg);
    }

    ierr = MPI_Send( &msg, 4, MPI_CHAR,root_process, 2000, MPI_COMM_WORLD);
    printf("Procesador %d: Enviando Mensaje: %s\n",my_id,msg);
  }
//------------------------------------------------------------------------------
  ierr = MPI_Finalize();

  return 0;

 }
