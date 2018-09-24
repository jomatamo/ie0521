#include <iostream>
#include <string>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <sstream>
#include"cache.h"
#include <bitset>
using namespace std;

Cache *cache1;
int Load_Store;
int AddressH;
string line;
string load_store;
string Address;


int main(int argc, char* argv[]){

  int TC = stoi(argv[2]);
  int TB = stoi(argv[4]);
  int V = stoi(argv[6]);
  cache1 = new Cache(TC,TB,V);


  while (getline(cin, line))
  {
      line.erase(0,2);
      stringstream linestream(line);
      getline(linestream, load_store, ' ');
      getline(linestream, Address, ' ');
      Load_Store = stoi(load_store);
      stringstream hexAddress(Address);
      hexAddress>>hex>>AddressH;
      cache1->check_addr(AddressH, Load_Store);

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

cout << "Cache Parameters:"<< '\n';
cout << "Cache size (B):"  << dec << TC << '\n';
cout << "Cache Associativity:" << dec << V << '\n';
cout << "Cache Block Size (B):" << dec << TB << '\n';
cout << "Simulation Results:"<< '\n';
cout << "Store hits:" << dec << cache1->store_hits<< '\n';
cout << "Store misses: " << dec << cache1->store_misses << '\n';
cout << "Load hits: " << dec << cache1->load_hits << '\n';
cout << "Load misses: " << dec << cache1->load_misses << '\n';
cout << "Dirty evictions: " << dec << cache1->dirty_evictions <<'\n';
}
