#include"cpu.h"
#include"math.h"
#include"cmath"
#include <bitset>


CPUs::CPUs(void){
// Se definen los estados para coherencia
  int M=0;
  int E=1;
  int S=2;
  int I=3;

  //Se crea la matriz con bloques del cache L1P1
  this->L1P1_head= new Block*[2]; // Se le asiga a cache_head un vector de punteros a bloque del tamano de la vias
  for (int i = 0; i < 2; i++) {
    this->L1P1_head[i]= new Block[256]; // a cada uno de esos punteros se le asigna un vector del tamano del numero de bloques por via
  }

  //Se crea la matriz con bloques del cache L1P2
  this->L1P2_head = new Block*[2]; // Se le asiga a cache_head un vector de punteros a bloque del tamano de la vias
  for (int i = 0; i < 2; i++) {
    this->L1P2_head[i]= new Block[256]; // a cada uno de esos punteros se le asigna un vector del tamano del numero de bloques por via
  }

  this->L2_head= new Block[4096];

  //Bits para L1.
  this->bits_index=8;
  this->bits_offset= 5;
  this->bits_tag = 32-this->bits_offset-this->bits_index;

  //Se crea la mascara de comparacion de index de L1:
  //cout << bitset<32>(this->mask_index) << endl;
  for (int i = 0; i < this->bits_index; i++) {
    this->mask_index = this->mask_index << 1;
    this->mask_index = this->mask_index + 1;

  }

  this->mask_index = this->mask_index << this->bits_offset;
//cout << bitset<32>(this->mask_tag) << endl;
//--------------------------------------------------------------------------
  //Se crea la mascara de comparacion de tags de L1
  for (int i = 0; i < this->bits_tag; i++) {
    this->mask_tag = this->mask_tag << 1;
    this->mask_tag = this->mask_tag + 1;
  }
  this->mask_tag = this->mask_tag << (this->bits_offset+this->bits_index);
//----------------------------------------------------------------------------

// mascaras de L2------------------------------------------------------------
this->bits_indexL2=12;
this->bits_offsetL2= 5;
this->bits_tagL2 = 32-this->bits_offsetL2-this->bits_indexL2;

//Se crea la mascara de comparacion de index:
//cout << bitset<32>(this->mask_indexL2) << endl;
for (int i = 0; i < this->bits_indexL2; i++) {
  this->mask_indexL2 = this->mask_indexL2 << 1;
  this->mask_indexL2 = this->mask_indexL2 + 1;
}
this->mask_indexL2 = this->mask_indexL2 << this->bits_offsetL2;
//--------------------------------------------------------------------------
//Se crea la mascara de comparacion de tags
//cout << bitset<32>(this->mask_tagL2) << endl;
for (int i = 0; i < this->bits_tagL2; i++) {
  this->mask_tagL2 = this->mask_tagL2 << 1;
  this->mask_tagL2 = this->mask_tagL2 + 1;
}
this->mask_tagL2 = this->mask_tagL2 << (this->bits_offsetL2+this->bits_indexL2);
//


}

CPUs::~CPUs(void){

}
//-------------Funcion check_addr---------------------------------------------//
void CPUs::check_addr(int addr, int RW,int int_cntr){
 //cout << "Estoy leyendo la direccion" << endl;
 //std::cout << "Instruccion: " << RW << '\n';
 // cout << hex << addr << endl;
  int M=0;
  int E=1;
  int S=2;
  int I=3;
// Se consigue el index de la direccion y el tag usando las mascaras
  int addr_index = (addr & this->mask_index) >> this->bits_offset ;
  int addr_tag = (addr & this->mask_tag) >> (this->bits_offset+this->bits_index);

  //cout << "Tag: " << bitset<19>(addr_tag) << endl;
  //cout << "index: " << bitset<8>(addr_index) << endl;
//--------------Se pregunta si la instruccion es para  CPU 1--------------------------
  if (int_cntr<4) {
    bool hit_vias_L1 = false;

    // Se va a revisar que ese sea hit en L1------------------------------------------
    for (int i = 0; i < 2; i++) { // Se itera sobre las dos vias
      //cout <<"todo bien 13"<<endl;
      if (this->L1P1_head[i][addr_index].Tag==addr_tag) { // Se pregunta si esta en la via

        if (this->L1P1_head[i][addr_index].coherencia !=I){ // Si no esta envalidado en
          hit_vias_L1 = true;
          //cout << "Es un hit en L1P1, voy a revisar coherencia" << endl;
          this->hits_L1P1++;
          this->revisar_coherencia(1,i,addr_index,addr_tag,RW);
        }
      }
    }
    if (!hit_vias_L1){
      //cout << "Es un miss en L1P1" << endl;
      this->misses_L1P1++;
      this->es_miss(addr,1,addr_index,addr_tag,RW); // Se llama la funcion miss

    }
//--------------Se pregunta si la instruccion es para CPU 2---------------------------
  } else {
    bool hit_vias_L1 = false;
    // Se va a revisar que ese sea hit en L2------------------------------------------
    for (int i = 0; i < 2; i++) { // Se itera sobre las dos vias
      if (this->L1P2_head[i][addr_index].Tag==addr_tag) { // Se pregunta si esta en la via
        if (this->L1P2_head[i][addr_index].coherencia !=I){ // Si no esta envalidado en
          hit_vias_L1 = true;
          //cout << "Es un hit de L1P2, voy a revisar coherencia." << endl;
          this->hits_L1P2++;
          this->revisar_coherencia(2,i,addr_index,addr_tag,RW);
        }
      }
    }
    if (!hit_vias_L1){
      //cout << "Es un miss en L1P2" << endl;
      this->misses_L1P2++;
      this->es_miss(addr,2,addr_index,addr_tag,RW); // Se llama la funcion miss
    }
  }
}
//---------------Funcion es_miss ---------------------------------------------//
void CPUs::es_miss(int addr, int CPU, int addr_index,int addr_tag, int RW){
// Se consigue el index de la direccion y el tag usando las mascaras
  int addr_indexL2 = (addr & this->mask_indexL2) >> this->bits_offsetL2 ;
  int addr_tagL2 = (addr & this->mask_tagL2) >> (this->bits_offsetL2+this->bits_indexL2);

  if (CPU==1) { // Si el miss se dio en el cache 1
    //cout << "Victimizando un bloque:" << endl;
    this->victimizar(1,addr_index,addr_tag,RW);
    //cout << "Logre victimizar con exito." << endl;
  } else if (CPU==2){
    //cout << "Victimizando un bloque:" << endl;
    this->victimizar(2,addr_index,addr_tag,RW);
    //cout << "Logre victimizar con exito." << endl;
  }

  if (this->L2_head[addr_indexL2].Tag != addr_tagL2 || this->L2_head[addr_indexL2].coherencia == 3 ) {
    //cout << "Es miss de L2." << endl;
    this->global_misses++;
    this->L2_head[addr_indexL2].Tag= addr_tagL2; // Si el bloque esta en el L2
    // Si el bloque se encontro ahora hay que pasarlo a L1, por eso se tiene que verificar la politica de reemplazo y una vez que se hace el reemplazo se revisa la coherencia con el otro cache L1
    //cout << "Trayendo de memoria principal"<<endl;
      this->L2_head[addr_indexL2].coherencia = 1; //E
  } else {
    //cout << "Es hit en L2" <<endl;
    this->hits_globales++;
  }
}
//--------------Funcion revisar coherencia------------------------------------//
void CPUs::revisar_coherencia(int CPU,int via, int addr_index, int addr_tag, int RW){
//-------------------------------Procesador 1-------------------------------------------
  int M=0;
  int E=1;
  int S=2;
  int I=3;
  //cout << "Instruccion para coherecia: " << RW << endl;
  if (CPU==1) { // Si la funcion la realiza el procesador 1
    bool found_L1P2 = false;
    int hit_via;
    //cout << "Voy a revisar coherencia" << endl;
    //cout << "Index de la direccion: " << bitset<8>(addr_index) << endl;
    //cout << "Tag de la dirrcion: " << bitset<19>(addr_tag) << endl;


    for (int j = 0; j < 2; j++) { // Se revisa en cual de las dos vias esta en  L1P2
    //cout << "Revisando via en L1P2: "<<j << '\n';
      if (this->L1P2_head[j][addr_index].Tag==addr_tag&&this->L1P2_head[j][addr_index].coherencia!=I){ // Se revisa el tag y que la coherencia sea diferente de invalido
        hit_via=j; // si el tag es el mismo, se india en cual via hubo hit
        found_L1P2 = true; // Se indica que se encontro el tag
        //cout << "Encontre la via " << '\n';
      } else {
        ////cout << "No era en esa via " << '\n';
      }
    }
    //cout << "Logre revisar bien L1P2." << '\n';
    if(found_L1P2){ // Si se encontro el bloque en alguna de las dos vias de L1P2
      //cout << "El bloque si estaba en L1P2" << '\n';
      //cout << "Estado de coherencia del bloque en P1: " << this->L1P1_head[hit_via][addr_index].coherencia << endl;
      //cout << "Estado de coherencia del bloque en P2: " << this->L1P2_head[hit_via][addr_index].coherencia << endl;
      if (RW==0){ // Si la instruccion es una lectura
        this->L1P1_head[via][addr_index].coherencia = S;
        this->L1P2_head[hit_via][addr_index].coherencia = S;
      } else { // Si es la instruccion es una escritura:
        this->L1P1_head[via][addr_index].coherencia = M;
        this->L1P2_head[hit_via][addr_index].coherencia = I;
        this->invalid_CPU2++;
      }
      //cout << "Estado de coherencia del bloque en P1: " << this->L1P1_head[via][addr_index].coherencia << endl;
      //cout << "Estado de coherencia del bloque en P2: " << this->L1P2_head[hit_via][addr_index].coherencia << endl;
    } else { // Si no se encontro el bloque en L1P2
      //cout << "No se encontraba en L1P2" << endl;

      if (RW==0){ // Si la instruccion es una lectura
          this->L1P1_head[via][addr_index].coherencia = E;
          //cout << "Estado de coherencia del bloque en P1: " << this->L1P1_head[via][addr_index].coherencia << endl;
      } else {
        //cout << "Modificando: " << endl;
        this->L1P1_head[via][addr_index].coherencia = E;
        //cout << "Estado de coherencia del bloque en P1: " << this->L1P1_head[via][addr_index].coherencia << endl;
      }

    }
    //cout << "Logre revisar bien las coherencias." << endl;


//---------------------------------Procesador 2------------------------------------
  } else if (CPU==2){ // Si la funcion la llama el procesador 2
    bool found_L1P1 = false;
    int hit_via;
    //cout << "Voy a revisar coherencia" << endl;
    //cout << "index: " << bitset<8>(addr_index) << endl;
    //cout << "tag: " << bitset<19>(addr_tag) << endl;
    for (int j = 0; j < 2; j++) { // Se revisa en cual de las dos vias esta en  L1P1
      //cout << "Revisando via en L1P1: "<<j << '\n';
      if (this->L1P1_head[j][addr_index].Tag==addr_tag &&this->L1P1_head[j][addr_index].coherencia!=I){ // Se revisa el tag
        hit_via=j; // si el tag es el mismo, se india en cual via hubo hit
        found_L1P1 = true; // Se indica que se encontro el tag
        //cout << "Encontre la via." << endl;
      } else {
        //cout << "No era en esa via " << '\n';
      }
    }
    //cout << "Logre revisar bien L1P1." << '\n';
    if(found_L1P1){ // Si se encontro el bloque en alguna de las dos vias de L1P2
      //cout << "El bloque si estaba en L1P1" << '\n';
      //cout << "Estado de coherencia del bloque en P1: " << this->L1P1_head[hit_via][addr_index].coherencia << endl;
      //cout << "Estado de coherencia del bloque en P2: " << this->L1P2_head[hit_via][addr_index].coherencia << endl;
      if (RW==0){ // Si la instruccion es una lectura
          this->L1P2_head[via][addr_index].coherencia = S;
          this->L1P1_head[hit_via][addr_index].coherencia = S;

      } else { // Si es la instruccion es una escritura:
        this->L1P2_head[via][addr_index].coherencia = M;
        this->L1P1_head[hit_via][addr_index].coherencia = I;
        this->invalid_CPU1++;
      }
      //cout << "Estado de coherencia del bloque en P2: " << this->L1P2_head[via][addr_index].coherencia << endl;
      //cout << "Estado de coherencia del bloque en P2: " << this->L1P1_head[hit_via][addr_index].coherencia << endl;
    } else { // Si no se encontro el bloque en L1P2
      //cout << "No estaba en L1P1" << '\n';

      if (RW==0){ // Si la instruccion es una lectura
          this->L1P2_head[via][addr_index].coherencia = E;
          //cout << "Estado de coherencia del bloque en P2: " << this->L1P2_head[via][addr_index].coherencia << endl;
      } else {
        //cout << "Modficiando:" << endl;
        this->L1P2_head[via][addr_index].coherencia = M;
        //cout << "Estado de coherencia del bloque en P2: " << this->L1P2_head[via][addr_index].coherencia << endl;
      }
    }
    //cout << "Logre revisar bien las coherencias. Estados finales: " << endl;
  }
}
//--------------Funcion victimizar--------------------------------------------//
void CPUs::victimizar(int CPU, int addr_index,int addr_tag,int RW){
  if (CPU==1) { // Si es el procesador 1 el que va a victimizar
    //cout << "Victimizando en P1." << endl;
    int via_victimizar;
    //cout << "Revisando las vias para victimizar, para el index: " << bitset<8>(addr_index)<< endl;
    //cout << "Tag: " << bitset<19>(addr_tag)<< endl;
    for (int i = 0; i < 2; i++) { // Se revisan las vias
      //cout << "Revisand la via "<< i << '\n';
      if(this->L1P1_head[i][addr_index].LRU==0){ // Si en una via se tiene el LRU en 0
        via_victimizar = i; // Se guarda cual via es
      }
    }
    //cout << " Vias revisadas la via para victimizar es: "<< via_victimizar <<endl;

    // for (int i = 0; i < 2; i++) {
    //    // Se le asigna el nuevo tag al bloque
    //   if (this->L1P1_head[i][addr_index].Tag=addr_tag) {
    //     if (this->L1P1_head[i][addr_index].coherecia != addr_tag) {
    //   }
    // }


    this->L1P1_head[via_victimizar][addr_index].Tag=addr_tag; // Se le asigna el nuevo tag al bloque

    if (via_victimizar == 0) {
      this->L1P1_head[via_victimizar][addr_index].LRU=1; // Se le asignan los nuevos valores de LRU
      this->L1P1_head[1][addr_index].LRU=0;
    }
    if (via_victimizar == 1) {
      this->L1P1_head[via_victimizar][addr_index].LRU=1;
      this->L1P1_head[0][addr_index].LRU=0;
    }

    //cout << "Valor de LRU para via 0: "<< this->L1P1_head[0][addr_index].LRU << endl;
    //cout << "Valor de LRU para via 1: "<< this->L1P1_head[1][addr_index].LRU << endl;
      //Ahora se procede a actualizar los valores de coherencia, revisa si en el otro cache L1P2 esta en bloque.
    this->revisar_coherencia(1,via_victimizar,addr_index,addr_tag,RW);

  } else if (CPU==2){
    int via_victimizar;
    //cout << "Victimizando en P2." << endl;
    //cout << "Revisando las vias para victimizar, para el index: " << bitset<8>(addr_index)<< endl;
    //cout << "Tag: " << bitset<8>(addr_tag)<< endl;
    for (int i = 0; i < 2; i++) { // Se revisan las vias
      //cout << "Revisando la via "<< i << '\n';
      if(this->L1P2_head[i][addr_index].LRU==0){ // Si en una via se tiene el LRU en 0
        via_victimizar = i; // Se guarda cual via es
      }
    }
    //cout << " Vias revisadas la via para victimizar es: "<< via_victimizar <<endl;
    this->L1P2_head[via_victimizar][addr_index].Tag=addr_tag; // Se le asigna el nuevo tag al bloque
    if (via_victimizar == 0) {
      this->L1P2_head[via_victimizar][addr_index].LRU=1; // Se le asignan los nuevos valores de LRU
      this->L1P2_head[1][addr_index].LRU=0;
    }
    if (via_victimizar == 1) {
      this->L1P2_head[via_victimizar][addr_index].LRU=1;
      this->L1P2_head[0][addr_index].LRU=0;
    }
      //Ahora se procede a actualizar los valores de coherencia, revisa si en el otro cache L1P2 esta en bloque.
    //cout << "Valor de LRU para via 0: "<< this->L1P2_head[0][addr_index].LRU << endl;
    //cout << "Valor de LRU para via 1: "<< this->L1P2_head[1][addr_index].LRU << endl;
    this->revisar_coherencia(2,via_victimizar,addr_index,addr_tag,RW);
  }
}
