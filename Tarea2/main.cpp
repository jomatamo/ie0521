#include <iostream>
#include <string>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <sstream>
#include"cache.h"
#include <bitset>
using namespace std;

Cache *cache1; // se crea un puntero a la base cache al cual se le asignara una nueva instancia en el main
int Load_Store;
int AddressH;
string linea_trace;
string load_store;
string Address;

//Metodo Main
//Parametros: recibe un vector con los parametros para crear el elemento de la clase cache que se usara para la simulacion
int main(int argc, char* argv[]){

  int TC = stoi(argv[2]); // Se guarda el tamano del cache
  int TB = stoi(argv[4]); // Se guarda el tamano del bloque
  int V = stoi(argv[6]); // Se guarda el numero de vias
  cache1 = new Cache(TC,TB,V); // Se crea el nuevo elemento de la clase cache con los parametros de entrada del programa


  while (getline(cin, linea_trace)) // Bucle while que lee una linea del cin y cuando se acaban las lineas se retorna false y se acaba el bucle
  {
      linea.erase(0,2); // se borrando el signo de # y de espacio al inicio de la lina del trace
      stringstream linestream(linea_trace); // se convierte la linea del trace en un stringstream
      getline(linestream, load_store, ' '); // se obtine el numero indiciador de Load_Store
      getline(linestream, Address, ' '); // se obtiene la direccion
      Load_Store = stoi(load_store); // se guarda el numero de Load_Store como un entero
      stringstream hexAddress(Address); // el string que se contiene la direccion se hace un stringstream
      hexAddress>>hex>>AddressH; // se convierte a hexadecimal la dirreccion
      cache1->check_addr(AddressH, Load_Store); // se llama al metodo check_addr de la clase para revisar la instrucicon y su direccion

  }

// Prueba para verificar mascaras
  // cout << bitset<32>(cache1->mask_index) << endl;
  // cout << bitset<32>(cache1->mask_tag) << endl;

// Imprimir Estado del cache--------------------------------------------------
  // for (int i = 0; i < V; i++) {
  //   cout << dec << "Via "<< i+1 << endl;
  //   cout << "index: ";
  //   for (int j = 0; j < (TC/TB/V); j++) {
  //
  //     cout << bitset<4>(cache1->cache_head[i][j].Index) << " ";
  //   }
  //   cout << endl;/* code */
  //   cout << "tag:   ";
  //   for (int j = 0; j < (TC/TB/V); j++) {
  //
  //     cout << hex << (cache1->cache_head[i][j].Tag) << "    ";
  //   }
  //   cout << endl;/* code */
  //   cout << "Vacio:   ";
  //   for (int j = 0; j < (TC/TB/V); j++) {
  //
  //     cout << hex << (cache1->cache_head[i][j].empty) << "    ";
  //   }
  //   cout << endl;/* code */
  //   cout << "DB:    ";
  //   for (int j = 0; j < (TC/TB/V); j++) {
  //
  //     cout << hex << (cache1->cache_head[i][j].dirty_bit) << "    ";
  //   }
  //   cout << endl;/* code */
  //   cout << "RRPV:  ";
  //   for (int j = 0; j < (TC/TB/V); j++) {
  //
  //     cout << hex << (cache1->cache_head[i][j].RRPV) << "    ";
  //   }
  //   cout << endl;/* code */
  // }
//----------------------------------------------------------------------------
// Se imprimen las estadisticas de la simualcion
cout << "Cache Parameters:"<< '\n';
cout << "Cache size (kB):"  << dec << TC/1000 << '\n';
cout << "Cache Associativity:" << dec << V << '\n';
cout << "Cache Block Size (B):" << dec << TB << '\n';
cout << "Simulation Results:"<< '\n';
cout << "Store hits:" << dec << cache1->store_hits<< '\n';
cout << "Store misses: " << dec << cache1->store_misses << '\n';
cout << "Load hits: " << dec << cache1->load_hits << '\n';
cout << "Load misses: " << dec << cache1->load_misses << '\n';
cout << "Dirty evictions: " << dec << cache1->dirty_evictions <<'\n';
cout << "Overall miss rate: " << (float)((cache1->store_misses+cache1->load_misses)/(cache1->store_misses+cache1->store_hits+cache1->load_misses+cache1->load_hits)) <<'\n';


}
