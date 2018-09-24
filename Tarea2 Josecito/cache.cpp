#include"cache.h"
#include"math.h"
#include"cmath"
#include <bitset>


Cache::Cache(int cache_size, int b_size, int vias){
  int numero_bloques = cache_size/b_size/vias;  // Se calcula el numero de bloques con el tamano de vias, del cache y de el numero dias
  this->vias=vias;
  // Sacamos bits del index
  float numero_bloques_float = (float)numero_bloques;
  float n_bits_float = log2(numero_bloques_float);
  this->bits_index = (int)n_bits_float; //////////////////////////////////////////


  //Sacamos bits de offset
  float b_size_float = (float)b_size;
  float bits_offset_float = log2(b_size_float);
  this->bits_offset = (int)bits_offset_float;
  this->bits_tag = 32-this->bits_offset-this->bits_index;

  //Creacion de la mascara de index
  for (int i = 0; i < this->bits_index; i++) {
    this->index_mask = this->index_mask << 1;
    this->index_mask = this->index_mask + 1;
  }
  this->index_mask = this->index_mask << this->bits_offset;

  //Creacion de la mascara de tag
  for (int i = 0; i < this->bits_tag; i++) {
    this->tag_mask = this->tag_mask << 1;
    this->tag_mask = this->tag_mask + 1;
  }
  this->tag_mask = this->tag_mask << (this->bits_offset+this->bits_index);

  //Creacion de la matriz de bloques
  this->Cabeza_vias= new Bloque*[vias]; // Se inicializa el vector con punteros a tipo bloque
  for (int i = 0; i < vias; i++) {
    this->Cabeza_vias[i]= new Bloque[numero_bloques]; // a cada uno de esos punteros se le asigna un arrego de tipo bloque del la cantidad de bloques que hay por dia.

  }
  int cont_index=0;
  for (int j = 0; j < numero_bloques; j++) {
    for (int i = 0; i < vias; i++) {
      this->Cabeza_vias[i][j].index=j;
    }/* code */
  }
}

Cache::~Cache(void){

}

void Cache::verificar_dir(int dir, int instr){
  //cero = load -> leer
  //uno = store -> escribir
  int Index_dir = dir & this->index_mask;
  int Tag_dir = dir & this->tag_mask;
  int contador_vias=0;

  // cout << "index dir: " << bitset<32>(Index_dir) << '\n';
  // cout << "tag dir:   " << bitset<32>(Tag_dir)<< '\n';

  Index_dir = Index_dir >> this->bits_offset;
  Tag_dir = Tag_dir >> (this->bits_offset+this->bits_index);
  //
  //  cout << "index dir: " << bitset<4>(Index_dir) << '\n';
  // // cout << "tag dir:   " << bitset<32>(Tag_dir)<< '\n';
  //  cout << "tag dir:   " << hex << Tag_dir<< '\n';


  while (contador_vias < this->vias) {
    // cout << "contador_vias: " << contador_vias << '\n';
    if (this->Cabeza_vias[contador_vias][Index_dir].vacio==1) {

     this->reemplazar(contador_vias, Index_dir, Tag_dir, instr);
     if (instr==0) {
       load_misses++;
     } else {
       store_misses++;
     }


     break;
      }
      else{
        if (this->Cabeza_vias[contador_vias][Index_dir].tag == Tag_dir ) {

          if (instr==0) {
            load_hits++;
          } else {
            store_hits++;
            this->Cabeza_vias[contador_vias][Index_dir].dirty_bit=1;
          }
          this->Cabeza_vias[contador_vias][Index_dir].RRPV = 0;  // valor de predicción cercano
          break;
        } else{
          contador_vias++;
        }
    }
    if (contador_vias == this->vias) {
      /* victimizar */

    this->victimizar(Index_dir, Tag_dir, instr);
    if (instr==0) {
      load_misses++;
    } else {
      store_misses++;
    }

    }

}
}

void Cache::reemplazar(int via, int index, int tag, int db){
  this->Cabeza_vias[via][index].tag=tag;
  this->Cabeza_vias[via][index].vacio=0;
  this->Cabeza_vias[via][index].RRPV=2;
  this->Cabeza_vias[via][index].dirty_bit=db;

}

void Cache::victimizar(int index, int tag, int db){
  //

  int contador_vias=0;
  while (true) {
    if (contador_vias==this->vias) {
      incrementar_RRPV(index);
      contador_vias=0;
    }
    if (this->Cabeza_vias[contador_vias][index].RRPV == 3) {
      if (this->Cabeza_vias[contador_vias][index].dirty_bit ==1) {
        contador_dirty_evictions ++;
      }
      this->reemplazar(contador_vias, index, tag, db);
      break;
    } else {
      contador_vias++;

    }
  }




}

void Cache::incrementar_RRPV(int index){
  for ( int i = 0; i < this->vias; i++) {
    if (this->Cabeza_vias[i][index].RRPV != 3) {
      this->Cabeza_vias[i][index].RRPV ++;
    }
  }
}
