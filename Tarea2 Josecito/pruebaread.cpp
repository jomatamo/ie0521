#include <iostream>
#include <string>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <sstream>
#include"cache.h"
#include <bitset>

using namespace std;


string line;

string load_store;
int Instrucc;
string Direcc;
int DireccH;


Cache *prueba_cache;




int main(int argc, char* argv[]){

  // string TCs=argv[2];
  // string Vs=argv[3];
  // string TBs=argv[4];
  // int TC= stoi(TCs);
  // int V= stoi(Vs);
  // int TB = stoi(TBs);
  // cout << argv[0] << endl;
  // cout << argv[1] << endl;
  // cout << argv[2] << endl;
  // cout << argv[3] << endl;
  // cout << argv[4] << endl;
  // cout << argv[5] << endl;
  // cout << argv[6] << endl;
  int TC = stoi(argv[2]);
  int TB = stoi(argv[4]);
  int V = stoi(argv[6]);




  prueba_cache = new Cache(TC,TB,V);



  while (getline(cin, line))
  {
      line.erase(0,2);
      stringstream linestream(line);

      getline(linestream, load_store, ' ');
      getline(linestream, Direcc, ' ');
      ///load_store.erase(0,1);

      Instrucc = stoi(load_store);
      stringstream hexDirecc(Direcc);
      hexDirecc>>hex>>DireccH;
      // cout <<Instrucc<< " " << hex << DireccH <<endl;
      prueba_cache->verificar_dir(DireccH, Instrucc);


  }
// prueba_cache.verificar_dir(0x7fffed80);

  cout << bitset<32>(prueba_cache->index_mask) << endl;
  cout << bitset<32>(prueba_cache->tag_mask) << endl;

// Imprimir Estado del cache--------------------------------------------------
  for (int i = 0; i < V; i++) {
    cout << dec << "Via "<< i+1 << endl;
    cout << "index: ";
    for (int j = 0; j < (TC/TB/V); j++) {

      cout << bitset<4>(prueba_cache->Cabeza_vias[i][j].index) << " ";
    }
    cout << endl;/* code */
    cout << "tag:   ";
    for (int j = 0; j < (TC/TB/V); j++) {

      cout << hex << (prueba_cache->Cabeza_vias[i][j].tag) << "    ";
    }
    cout << endl;/* code */
    cout << "Vacio:   ";
    for (int j = 0; j < (TC/TB/V); j++) {

      cout << hex << (prueba_cache->Cabeza_vias[i][j].vacio) << "    ";
    }
    cout << endl;/* code */
    cout << "DB:    ";
    for (int j = 0; j < (TC/TB/V); j++) {

      cout << hex << (prueba_cache->Cabeza_vias[i][j].dirty_bit) << "    ";
    }
    cout << endl;/* code */
    cout << "RRPV:  ";
    for (int j = 0; j < (TC/TB/V); j++) {

      cout << hex << (prueba_cache->Cabeza_vias[i][j].RRPV) << "    ";
    }
    cout << endl;/* code */
  }
//----------------------------------------------------------------------------
cout << "Cache Parameters:"<< '\n';
cout << "Cache size (B):"  << dec << TC << '\n';
cout << "Cache Associativity:" << dec << V << '\n';
cout << "Cache Block Size (B):" << dec << TB << '\n';
cout << "Simulation Results:"<< '\n';
cout << "Store hits:" << dec << prueba_cache->store_hits<< '\n';
cout << "Store misses: " << dec << prueba_cache->store_misses << '\n';
cout << "Load hits: " << dec << prueba_cache->load_hits << '\n';
cout << "Load misses: " << dec << prueba_cache->load_misses << '\n';
cout << "Dirty evictions: " << dec << prueba_cache->contador_dirty_evictions <<'\n';
}
