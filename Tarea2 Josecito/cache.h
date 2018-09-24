/*********************************************************************
cache.h
**********************************************************************/
#ifndef CLASECACHE
#define CLASECACHE

#include<string>
#include<iostream>
using namespace std;

//Typo bloque son los bloques del cache.
typedef struct C_Block{
  int dirty_bit=0; //Indicador si el dirty bit del bloque esta en 1 o en 0
  long Tag=0; // tag del bloque almacenado
  long Index=0; //indez del bloque de la via del cache
  int empty=1; // indicdor si el cache esta vacio, empty = 1 esta vacio, si es 0 esta ocupado
  int RRPV=3; // Estado del valor de prediccion de reuso para politica de reemplazo RRIP, si es 0 es cercado, 2 es distante y 3 es lejado.

}Block;

class Cache {

  public:
    Cache(int, int, int); //Constructuror de la clase Cache
    ~Cache(void); // destructor de la clase Cache
//----------------Atributos----------------------------------------------------
    long mask_index, mask_tag; //Variables Long que contienen las mascaras para comparar index y tag de la direccion del trace
    int dirty_evictions = 0; // Contador de cuantos bloques con en el dirty bit en 1 fueron victimizados
    int load_misses = 0; // Contador de la cantidad de misses por lods
    int store_misses = 0; // contador de la cantidad de misses por Stores
    int load_hits=0; // Cantidad de hits por loads
    int store_hits = 0; //Cantidad de misses por Loads
    int tag_bit_count, index_bit_count, offset_bit_count; //Cantidad de bits de tag, index y de offser
    int Assoc; //Numero de vias del cache
    Block **cache_head;// Puntero que apunta a la matriz que contedra los los bloques de cache


//-------------------Metodos--------------------------------------------------
    void check_addr(int, int); //lee la linea del trace y verifica si la direccion esta en el cache, sino llama a replace_block o a victim
    void replace_block(int, int, int, int); //agrega el bloque de la direccion al cache
    void increase_RRPV(int);// incrementa el valor de preddicion de reuso en 1 si al llamar a victim no hay un bloque que lo tenga en 3
    void victim(int, int, int);// metdodo que escoge cual bloque victimizar segun la politica de reemplazo

};
#endif
