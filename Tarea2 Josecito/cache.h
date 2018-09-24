/*********************************************************************
cache.h
**********************************************************************/
#ifndef CLASECACHE
#define CLASECACHE

#include<string>
#include<iostream>
using namespace std;

//La directiva struct se usaba en C para crear nuevos tipos.
typedef struct Bloque{

  long tag=0;
  long index=0;
  int dirty_bit=0;
  int vacio=1;
  int RRPV=3;

}Bloque;

class Cache {

public:
int contador_dirty_evictions = 0;
int store_misses = 0;
int store_hits = 0;
int load_misses = 0;
int load_hits=0;
long index_mask, tag_mask;
int bits_tag, bits_index, bits_offset;
int vias;
Bloque **Cabeza_vias;
Cache(int, int, int);
~Cache(void);
void verificar_dir(int, int);
void reemplazar(int, int, int, int);
void incrementar_RRPV(int);
void victimizar(int, int, int);

};
#endif
