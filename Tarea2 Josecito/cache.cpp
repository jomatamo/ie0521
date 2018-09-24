#include"cache.h"
#include"math.h"
#include"cmath"
#include <bitset>


Cache::Cache(int cache_size, int b_size, int Assoc){
  int block_count = cache_size/b_size/Assoc;  // Se calcula el numero de bloques con el tamano de vias, del cache y de el numero dias
  this->Assoc=Assoc;
  // Sacamos bits del index
  float block_count_float = (float)block_count;
  this->index_bit_count = (int)(log2(block_count_float)); //////////////////////////////////////////


  //Sacamos bits de offset
  float b_size_float = (float)b_size;
  this->offset_bit_count = (int)(log2(b_size_float));

  this->tag_bit_count = 32-this->offset_bit_count-this->index_bit_count;

  //Creacion de la mascara de index
  for (int i = 0; i < this->index_bit_count; i++) {
    this->mask_index = this->mask_index << 1;
    this->mask_index = this->mask_index + 1;
  }
  this->mask_index = this->mask_index << this->offset_bit_count;

  //Creacion de la mascara de tag
  for (int i = 0; i < this->tag_bit_count; i++) {
    this->mask_tag = this->mask_tag << 1;
    this->mask_tag = this->mask_tag + 1;
  }
  this->mask_tag = this->mask_tag << (this->offset_bit_count+this->index_bit_count);

  //Creacion de la matriz de bloques
  this->cache_head= new Block*[Assoc]; // Se inicializa el vector con punteros a tipo bloque
  for (int i = 0; i < Assoc; i++) {
    this->cache_head[i]= new Block[block_count]; // a cada uno de esos punteros se le asigna un arrego de tipo bloque del la cantidad de bloques que hay por dia.

  }
  // int cont_index=0;
  for (int j = 0; j < block_count; j++) {
    for (int i = 0; i < Assoc; i++) {
      this->cache_head[i][j].Index=j;
    }/* code */
  }
}

Cache::~Cache(void){

}

void Cache::check_addr(int addr, int LS){
  //cero = load -> leer
  //uno = store -> escribir
  int addr_index = (addr & this->mask_index) >> this->offset_bit_count ;
  int addr_tag = (addr & this->mask_tag) >> (this->offset_bit_count+this->index_bit_count);
  int assoc_counter=0;

  // cout << "index dir: " << bitset<32>(Index_dir) << '\n';
  // cout << "tag dir:   " << bitset<32>(Tag_dir)<< '\n';

  // Index_dir = Index_dir >> this->offset_bit_count;
  // Tag_dir = Tag_dir >> (this->offset_bit_count+this->index_bit_count);
  //
  //  cout << "index dir: " << bitset<4>(Index_dir) << '\n';
  // // cout << "tag dir:   " << bitset<32>(Tag_dir)<< '\n';
  //  cout << "tag dir:   " << hex << Tag_dir<< '\n';


  while (assoc_counter < this->Assoc) {
    // cout << "contador_vias: " << contador_vias << '\n';
    if (this->cache_head[assoc_counter][addr_index].empty==1) {

     this->replace_block(assoc_counter, addr_index, addr_tag, LS);
     if (LS==0) {
       load_misses++;
     } else {
       store_misses++;
     }
     break;
      }
      else{
        if (this->cache_head[assoc_counter][addr_index].Tag == addr_tag ) {

          if (LS==0) {
            load_hits++;
          } else {
            store_hits++;
            this->cache_head[assoc_counter][addr_index].dirty_bit=1;
          }
          this->cache_head[assoc_counter][addr_index].RRPV = 0;  // valor de predicción cercano
          break;
        } else{
          assoc_counter++;
        }
    }
    if (assoc_counter == this->Assoc) {
      /* victimizar */

    this->victim(addr_index, addr_tag, LS);
    if (LS==0) {
      load_misses++;
    } else {
      store_misses++;
    }

    }

}
}

void Cache::replace_block(int via, int index, int tag, int db){
  this->cache_head[via][index].Tag=tag;
  this->cache_head[via][index].empty=0;
  this->cache_head[via][index].RRPV=2;
  this->cache_head[via][index].dirty_bit=db;

}

void Cache::victim(int index, int tag, int db){
  //

  int assoc_counter=0;
  while (true) {
    if (assoc_counter==this->Assoc) {
      increase_RRPV(index);
      assoc_counter=0;
    }
    if (this->cache_head[assoc_counter][index].RRPV == 3) {
      if (this->cache_head[assoc_counter][index].dirty_bit ==1) {
        dirty_evictions ++;
      }
      this->replace_block(assoc_counter, index, tag, db);
      break;
    } else {
      assoc_counter++;

    }
  }




}

void Cache::increase_RRPV(int index){
  for ( int i = 0; i < this->Assoc; i++) {
    if (this->cache_head[i][index].RRPV != 3) {
      this->cache_head[i][index].RRPV ++;
    }
  }
}
