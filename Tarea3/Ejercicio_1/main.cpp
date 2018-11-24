#include <iostream>
#include <string>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <sstream>
#include "cpu.h"
#include <time.h>
#include <bitset>
using namespace std;


int RW;
int AddressH = 0;
int Load_Store = 0;
string linea_trace="";
string load_store="";
string Address="";
int int_cntr = 1;
CPUs *cpus1;

//Metodo Main
//Parametros: recibe un vector con los parametros para crear el elemento de la clase cache que se usara para la simulacion
int main(int argc, char* argv[]){
  cout << "Iniciando Simulacion con mcf.trace.gz." << endl;
  clock_t start5=clock();
  cpus1= new CPUs; // Se crea el nuevo elemento de la clase cache con los parametros de entrada de
  // cout << bitset<32>(cpus1->mask_index) << endl;
  // cout << bitset<32>(cpus1->mask_tag) << endl;
  // cout << bitset<32>(cpus1->mask_indexL2) << endl;
  // cout << bitset<32>(cpus1->mask_tagL2) << endl;
  while (getline(cin, linea_trace)) // Bucle while que lee una linea del cin y cuando se acaban las lineas se retorna false y se acaba el bucle
  {
  //  cout << "---------------------------------------------------------------" <<endl;
      //cout << "Contador de instruciones: " << int_cntr << endl;
      linea_trace.erase(0,2); // se borrando el signo de # y de espacio al inicio de la lina del trace
      //cout << linea_trace << endl;
  //    cout << linea_trace << endl;
      stringstream linestream(linea_trace); // se convierte la linea del trace en un stringstream
      getline(linestream, load_store, ' '); // se obtine el numero indiciador de Load_Store
      getline(linestream, Address, ' '); // se obtiene la direccion
      RW = stoi(load_store); // se guarda el numero de Load_Store como un entero
      stringstream hexAddress(Address); // el string que se contiene la direccion se hace un stringstream

      hexAddress >> hex >> AddressH;

      cpus1->check_addr(AddressH,RW, int_cntr); // se llama al metodo check_addr
      int_cntr++;
      if (int_cntr > 4) {
        int_cntr =1;
      }

    //cout << "---------------------------------------------------------------" <<endl;
  }

 //Prueba para verificar mascaras
  // cout << bitset<32>(cpus1->mask_index) << endl;
  // cout << bitset<32>(cpus1->mask_tag) << endl;
  // cout << bitset<32>(cpus1->mask_indexL2) << endl;
  // cout << bitset<32>(cpus1->mask_tagL2) << endl;

// Imprimir Estado del cache--------------------------------------------------
//   for (int i = 0; i < 2; i++) {
//     cout << dec << "Via "<< i+1 << endl;
//     cout << "index: ";
//     for (int j = 0; j < 256; j++) {
//
//     cout << bitset<8>(j) << " ";
//     }
//     cout << endl;/* code */
//     cout << "tag:   ";
//     for (int j = 0; j < 256; j++) {
//      cout << hex << (cpus1.L1P1_head[i][j].Tag) << "    ";
//     }
//     cout << endl;/* code */
//     cout << "Coherencia:   ";
//     for (int j = 0; j < 256; j++) {
//
//      cout << (cpus1.L1P1_head[i][j].coherencia) << "    ";
//     }
//     cout << endl;/* code */
//     cout << "LRU:    ";
//     for (int j = 0; j < 256; j++) {
//
//      cout << (cpus1.L1P1_head[i][j].LRU) << "    ";
//     }
//     cout << endl;/* code */
//   }
// // Para cache L1P1-------------------------------------------------------------
//   for (int i = 0; i < 2; i++) {
//     cout << dec << "Via "<< i+1 << endl;
//     cout << "index: ";
//     for (int j = 0; j < 256; j++) {
//
//     cout << bitset<8>(j) << " ";
//     }
//     cout << endl;/* code */
//     cout << "tag:   ";
//     for (int j = 0; j < 256; j++) {
//      cout << hex << (cpus1.L1P2_head[i][j].Tag) << "    ";
//     }
//     cout << endl;/* code */
//     cout << "Coherencia:   ";
//     for (int j = 0; j < 256; j++) {
//
//      cout << hex << (cpus1.L1P2_head[i][j].coherencia) << "    ";
//     }
//     cout << endl;/* code */
//     cout << "LRU:    ";
//     for (int j = 0; j < 256; j++) {
//
//      cout << hex << (cpus1.L1P2_head[i][j].LRU) << "    ";
//     }
//     cout << endl;/* code */
//   }





// Hay que recordar que los parametros estan invertidos para el procesador 1 y el procesador 2, por lo que
// Por lo que se imprimen ya correctamente:
  cout << "Resultados de simulacion: " << endl;
  cout << "-------------------------------------------------------------------" << endl;
  float global_missrate = ((float)(cpus1->global_misses))/((float)(cpus1->misses_L1P1+cpus1->misses_L1P2+cpus1->hits_L1P1+cpus1->hits_L1P2+cpus1->hits_globales));
  // cout << "Misses globales: "<< cpus1->global_misses<<endl;
  // cout << "Hits globales: " << cpus1->hits_globales<< endl;
  // cout << "Hits L1P2: " << cpus1->hits_L1P1<< endl;
  // cout << "Hits L1P1: " << cpus1->hits_L1P2<< endl;
  // //cout << "Hits totales: " << cpus1.hits_totales<< endl;
  // cout << "Misses L1P2: " << cpus1->misses_L1P1<< endl;
  // cout << "Misses L1P1: " << cpus1->misses_L1P2<< endl;
  // cout << "Invalidaciones CPU2: " << cpus1->invalid_CPU1<< endl;
  // cout << "Invalidaciones CPU1: " << cpus1->invalid_CPU2<< endl;
//----------------------------------------------------------------------------
// Se imprimen las estadisticas de la simualcion
  cout << "Miss rate global: "<< global_missrate << endl;
  cout << "Miss rate L1 CPU2:"  << ((float)(cpus1->misses_L1P1))/((float)(cpus1->hits_L1P1+cpus1->misses_L1P1))<< endl;
  cout << "Miss rate L1 CPU1:"  << ((float)(cpus1->misses_L1P2))/((float)(cpus1->hits_L1P2+cpus1->misses_L1P2))<< endl;
  cout << "Invalidaciones por coherecia CPU2: "<< cpus1->invalid_CPU1 << endl;
  cout << "Invalidaciones por coherencia CPU1: "<< cpus1->invalid_CPU2 << endl;
cout << "-------------------------------------------------------------------" << endl;
  cout<<"Tiempo de ejecución:  " <<((double)clock()-start5)/CLOCKS_PER_SEC << " s"<< endl;
cout << "-------------------------------------------------------------------" << endl;
}
