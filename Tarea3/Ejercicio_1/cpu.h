/*********************************************************************
cache.h
**********************************************************************/
#ifndef CLASECPUS
#define CLASECPUS

#include<string>
#include<iostream>
using namespace std;

//Typo bloque son los bloques del cache.
typedef struct C_Block{
  int coherencia=3; // 0 = M, 1 = E, 2 = S, 3 = I. Se inicia todo en inválido.
  long Tag=0; // tag del bloque almacenado
  long Index=0; //index del bloque de la via del cache
  // int empty=1; // indicdor si el cache esta vacio, empty = 1 esta vacio, si es 0 esta ocupado
  // int RRPV=3; // Estado del valor de prediccion de reuso para politica de reemplazo RRIP, si es 0 es cercado, 2 es distante y 3 es lejado.
  int LRU=0; // LRU = 1 es el que se reemplazará.

}Block;

class CPUs {

  public:
    CPUs(); //Constructuror de la clase
    ~CPUs(void); // destructor de la clase

//----------------Atributos----------------------------------------------------
    long mask_index=0;
    long mask_tag=0; //Variables Long que contienen las mascaras para comparar index y tag de la direccion del trace
    long mask_indexL2 =0;
    long mask_tagL2 = 0;
    int invalid_CPU1 = 0; // Contador de invalidaciones por coherencia CPU1
    int invalid_CPU2 = 0; // Contador de invalidaciones por coherencia CPU2
    int global_misses = 0; // contador de misses
    int hits_globales=0; // Cantidad de hits
    int misses_L1P1=0;
    int hits_L1P1=0;
    int hits_L1P2=0;
    int misses_L1P2=0;
    //int store_hits = 0; //Cantidad de misses por Loads
    Block **L1P1_head = NULL;// Puntero que apunta a la matriz que contedra los los bloques de cache
    Block **L1P2_head = NULL;
    Block *L2_head = NULL;
    int bits_offset=0;
    int bits_tag=0;
    int bits_index=0;
    int bits_offsetL2=0;
    int bits_tagL2=0;
    int bits_indexL2=0;



//-------------------Metodos--------------------------------------------------
    void check_addr(int, int, int); //lee la linea del trace y verifica si la direccion esta en el cache, sino llama a replace_block o a victim
    //void add_block(int, int, int, int); //agrega el bloque de la direccion al cache
    //void increase_RRPV(int);// incrementa el valor de preddicion de reuso en 1 si al llamar a victim no hay un bloque que lo tenga en 3
    //void victim(int, int, int);// metdodo que escoge cual bloque victimizar segun la politica de reemplazo
    void revisar_coherencia(int,int,int,int,int);
    void victimizar(int, int,int,int);
    void es_miss(int,int,int,int,int);


};
#endif
