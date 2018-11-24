#include"math.h"
#include <mpi.h>
#include <iostream>
using namespace std;
#include <time.h>

int main(int argc, char *argv[]) {
  clock_t start5=clock();
  int COMPUTE = 10;
  int DONE = 15;
  int STOP=2;
  int root_process = 0;
  int my_id, size, ierr;
  int matriz_size=stoi(argv[2]);

  int cont_filas=0;
  int cont_filas2=0;
  int filas_disponibles=matriz_size;
  int iteraciones;
  int flag;

//-------------------Se crea la matriz-----------------------------------------

  int **matriz = new int*[matriz_size];
  for (int i = 0; i < matriz_size; i++) {
    matriz[i] = new int[matriz_size];
  }
//------------------Se crea el vector------------------------------------------
  int *vector = new int[matriz_size];
//-----------------Se rellena el vector----------------------------------------
  for (int i = 0; i < matriz_size; i++) {
    vector[i]=i;
  }
  // for (int i = 0; i < matriz_size; i++) {
  //   cout << vector[i] << endl;
  // }

//-----------------Se crea el vector temporal de los esclavos------------------
  int *temp= new int[matriz_size];
  int mult=0;
//----------------Se crea el vetor con el resultado final----------------------
  int *resultado = new int[matriz_size];
//---------------Se rellena la matriz principal--------------------------------
  for (int i = 0; i < matriz_size; i++) {
    for (int j = 0; j < matriz_size; j++) {
      matriz[i][j]=i+j;
    }
  }




//------------Se inicia la ejecucion paralela----------------------------------
  ierr = MPI_Init(&argc, &argv);
  // for (int i = 0; i < argc; i++) {
  //   cout << "argv["<<i<<"]:"<< argv[i] << endl;
  // }
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
  ierr = MPI_Comm_rank(MPI_COMM_WORLD, &my_id);
  ierr =MPI_Comm_size(MPI_COMM_WORLD, &size); // Se averigua el id del procesos

//-------------------------Procesador 0----------------------------------------
  if (my_id==root_process) {
    cout << "Matriz: " << endl;
    for (int i = 0; i < matriz_size; i++) {
       cout << "| ";
       for (int j = 0; j < matriz_size; j++) {
         cout << matriz[i][j]<<" ";
       }
       cout <<"|"<<endl;
    }
    cout << "Vector: " << endl;
    cout << "| ";
    for (int i = 0; i < matriz_size; i++) {
      cout << vector[i]<<" ";
    }
    cout << "|" << endl;
  }
  while(filas_disponibles>0){
    if (my_id==root_process) {
//Se inicializan la matriz y el vector que se van a multiplicar

      if (filas_disponibles>size) {
        iteraciones=size;
      } else {
        iteraciones=filas_disponibles;
      }

      for (int i = 0; i < matriz_size; i++) {
        resultado[cont_filas]=resultado[cont_filas]+matriz[cont_filas][i]*vector[i];
      }
      //cout <<" Resultado parcial P0: " << resultado[cont_filas] << endl;

      filas_disponibles--;
      cont_filas++;
      cont_filas2++;


      //cout <<"Cantidad de iteraciones: " << iteraciones << endl;

      for (int i = 1; i < iteraciones; i++) {
        ierr = MPI_Send( matriz[cont_filas], matriz_size, MPI_INT,i, COMPUTE, MPI_COMM_WORLD);
        // cout << "Procesador 0, enviendo lo siguiente a: " <<i<<endl;
        // for (int i = 0; i < matriz_size; i++) {
        //    cout << matriz[cont_filas][i] << ' ';
        //  }
        // cout << endl;
        cont_filas++;
        filas_disponibles--;
        cout << "Mensajes enviados a Procesador " << i <<endl;
      }
      cout << "Filas disponibles:"<<filas_disponibles<<endl;
      for (int i = 1; i < iteraciones; i++) {
        ierr = MPI_Recv( &resultado[cont_filas2], 1, MPI_INT,i, DONE, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        //cout << "Elemento del resultado "<<cont_filas2<<":"<<resultado[cont_filas2]<<endl;
        cont_filas2++;
      }
      if (filas_disponibles == 0){
        for (int i = 1; i < size; i++) {
          cout<<"Enviando STOP a procesador "<<i<<endl;
          ierr = MPI_Send(&matriz_size,1,MPI_INT,i,STOP,MPI_COMM_WORLD);
        }
      }
    } else { // Si es los procesos esclavos
      MPI_Status stat;
      mult = 0;
      ierr= MPI_Probe(0,MPI_ANY_TAG,MPI_COMM_WORLD,&stat);
      //cout << "Mensaje: "<< stat.MPI_TAG << endl;
      if (stat.MPI_TAG == COMPUTE){
        ierr = MPI_Recv( temp, matriz_size, MPI_INT,0, COMPUTE, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        //cout << "Procesador "<<my_id<<" recibi :";
        // for (int i = 0; i < matriz_size; i++) {
        //   cout << temp[i] << " ";
        // }
        // cout << endl;
        for (int i = 0; i < matriz_size; i++) {
          mult=mult+(temp[i]*vector[i]);
        }
        // cout << "Procesador "<<my_id<<" estoy enviando :"<<mult<<endl;
        ierr = MPI_Send( &mult, 1, MPI_INT,0, DONE, MPI_COMM_WORLD);
      } else if (stat.MPI_TAG==STOP)
        break;

    }
  }
  if (my_id==root_process) {
    cout << "Vector resultante: " << endl;
    cout << "| ";
    for (int i = 0; i < matriz_size; i++) {
      cout << resultado[i] << " ";
    }
    cout << "|" <<endl;
    cout<<"Tiempo de ejecución:  " <<((double)clock()-start5)/CLOCKS_PER_SEC << " s"<< endl;
  }
//----------------------------------------------------------------------------
  ierr = MPI_Finalize();

  return 0;

 }
