/*********************************************************************
cache.h
**********************************************************************/
#ifndef CLASECACHE
#define CLASECACHE

#include<string>
#include<iostream>
using namespace std;

//La directiva struct se usaba en C para crear nuevos tipos.
typedef struct C_Block{
  int dirty_bit=0;
  long Tag=0;
  long Index=0;
  int empty=1;
  int RRPV=3;

}Block;

class Cache {

public:
Cache(int, int, int);
~Cache(void);

long mask_index, mask_tag;
int dirty_evictions = 0;
int load_misses = 0;
int store_misses = 0;
int load_hits=0;
int store_hits = 0;
int tag_bit_count, index_bit_count, offset_bit_count;
int Assoc;
Block **cache_head;



void check_addr(int, int);
void replace_block(int, int, int, int);
void increase_RRPV(int);
void victim(int, int, int);

};
#endif
