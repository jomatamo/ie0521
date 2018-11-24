#include"math.h"
#include <mpi.h>
#include <iostream>
using namespace std;
#include <time.h>

int main(int argc, char *argv[]) {
  clock_t start5=clock();
  int COMPUTE = 10; // Tag que indica que se realice la multiplacion
  int DONE = 15; // tag que devuelven los procesadores que terminaron
  int STOP=2; // tag que se le envia a los procesadores para que paren
  int root_process = 0;
  int my_id, size, ierr;
  int matriz_size=stoi(argv[2]); // Se guarda el tamano de la matriz

  int cont_filas=0; //contador de cuales filas se han enviado
  int cont_filas2=0; // contador de cuales filas se han recibido
  int filas_disponibles=matriz_size; // cantidad de filas disponibles
  int iteraciones; // cantidad de iteraciones calculadas para el ciclo que envia y recibe de los procesadores esclavos
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
  int *temp= new int[matriz_size]; // los esclavos reciben los datos enviados en este vector
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
  ierr = MPI_Comm_rank(MPI_COMM_WORLD, &my_id); // se averigua el id del procesador
  ierr =MPI_Comm_size(MPI_COMM_WORLD, &size); // Se averigua cuandos procesadores

//-------------------------Procesador 0----------------------------------------
  if (my_id==root_process) { // Se imprimen las matrices y el vector
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

//------------------------------Ciclo principal del programa------------------
  while(filas_disponibles>0){
    if (my_id==root_process) {
//Se inicializan la matriz y el vector que se van a multiplicar

      if (filas_disponibles>size) { //si las filas disponibles es mas grande que el numero de iteraciones
        iteraciones=size; // las iteraciones se limitan a la cantidad de procesadores
      } else {
        iteraciones=filas_disponibles; // sino las iteraciones son la cantidad de filas dispobiles
      }
// El procesador 0 procesa una fila y la guarda en el vector de resultado
      for (int i = 0; i < matriz_size; i++) {
        resultado[cont_filas]=resultado[cont_filas]+matriz[cont_filas][i]*vector[i];
      }
      //cout <<" Resultado parcial P0: " << resultado[cont_filas] << endl;

      filas_disponibles--; // Se bajan las filas disponibles
      cont_filas++; // se incrementa el contador que dice cual es la fila que se envia
      cont_filas2++;// se incrementa el contador que dice cual es la fila que se recibe


      //cout <<"Cantidad de iteraciones: " << iteraciones << endl;
// dependiendo de la cantidad de iteracioens se envian datos a los otros procesadores
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
      // Se reciben la cantidad de que se enviaron procesadas
      for (int i = 1; i < iteraciones; i++) {
        ierr = MPI_Recv( &resultado[cont_filas2], 1, MPI_INT,i, DONE, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        //cout << "Elemento del resultado "<<cont_filas2<<":"<<resultado[cont_filas2]<<endl;
        cont_filas2++;
      }
      // Si las filas dipobibles es igual a 0, se envian mensajes a los otros procesadores que paren
      if (filas_disponibles == 0){
        for (int i = 1; i < size; i++) {
          cout<<"Enviando STOP a procesador "<<i<<endl;
          ierr = MPI_Send(&matriz_size,1,MPI_INT,i,STOP,MPI_COMM_WORLD);
        }
      }
    } else { // Si es los procesos esclavos
      MPI_Status stat;
      mult = 0;
      // Se usa MPI_Probe dentro del ciclo para revisar si se tiene algun mensaje del procesador 0 de cualquier tag
      ierr= MPI_Probe(0,MPI_ANY_TAG,MPI_COMM_WORLD,&stat);
      //cout << "Mensaje: "<< stat.MPI_TAG << endl;
      if (stat.MPI_TAG == COMPUTE){ // Si el tag es el de COMPUTE, se procede a recibir los datos, procesarlos y devolverlos
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
      } else if (stat.MPI_TAG==STOP) // Si compute es igual a 0 se sale del ciclo
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
