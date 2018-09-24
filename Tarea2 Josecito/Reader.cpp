#include "Reader.h"
#include <string>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <sstream>

using namespace std;
/// @brief constructor
Reader::Reader(){
}
/// @brief asigna el archivo recibido
Reader::Reader(string s){


	this->archivo=s;
}
/// @brief destrcutor
Reader::~Reader(){

}
///@brief convierte una linea de texto en un string
string Reader::read(){
	///@code
	ifstream lectura_archivo;
    lectura_archivo.open("test.txt");
//	ifstream lectura_archivo (archivo.c_str());

    int contador_linea = 0;
    string linea_archivo;
    if(lectura_archivo.is_open()){
    	while (getline(lectura_archivo,linea_archivo)){
    	contador_linea=contador_linea+1;

	}
	lectura_archivo.close();
}
else{

	cout<<"Problema leer archivo"<< endl;
}
///@return devuelve el string
	return linea_archivo;
}
///:endcode


///@brief escribe el string en un txt
int Reader ::write(string escritura ){
	///@code
	if(escritura==""){

		 cout << " Problema escribir archivo " <<endl;
   }
   else{

   	ofstream escritura_archivo("Cadena.txt");
   	if(escritura_archivo.is_open()){
   		escritura_archivo<<escritura;

	   }
	   else{

		 cout << " Problema escribir archivo " << endl;
   }
   }
   ///@return devuelve cero cuando se completa
   return 0;
  ///:endcode
	}
///@brief lee varias lineas de txt y las convierte en string
string* Reader::readlines(){

///@code
string archivo = this->archivo;
string linea_archivo;
ifstream lectura_archivo;
lectura_archivo.open("test2.txt");
 int contador_linea = 0;
 if(lectura_archivo.is_open()){
    	while (getline(lectura_archivo,linea_archivo)){
    	contador_linea++;

	}
	lectura_archivo.close();
}
else{

	cout<<"Problema leer archivo"<< endl;
}

string *texto = new string [contador_linea+1];

if(contador_linea==1){

 cout << " Problema , es solo una linea " << endl;


}
else{
ifstream lectura_archivo;
lectura_archivo.open("test2.txt");
stringstream prueba;
prueba<<contador_linea+1;
string total = prueba.str();
//texto[0] = total;
contador_linea = 1;

if (lectura_archivo.is_open())
    {
      while ( getline (lectura_archivo,linea_archivo) )
      {
        texto[contador_linea-1] = linea_archivo;
        cout << linea_archivo << endl; //

				string delimiter = " ";
				string inicio ="#";
				size_t pos_inicio=0;
				pos_inicio=linea_archivo.find(inicio);
				linea_archivo.erase(0,pos_inicio+ inicio.length());

				size_t pos = 0;
				string Tipo_inst;
				string Direcc;
				pos = linea_archivo.find(delimiter);
				Tipo_inst = linea_archivo.substr(0,pos);
				linea_archivo.erase(0, pos + delimiter.length());
				pos = linea_archivo.find(delimiter);
				Direcc = linea_archivo.substr(0,pos);

				cout << Tipo_inst << endl; //
				cout << Direcc << endl; //









        contador_linea++;
      }
///@return devuelve un puntero donde estan los strings
      return texto;
      delete texto;
      lectura_archivo.close();
    }


}
}

///:endcode
///@brief Escribe las n lineas que recibe en un archivo de texto
int Reader::write(string* escritura, int n)
{

	if(escritura[0]!=""){

			ofstream escritura_archivo("Cadena2.txt");
				 if (escritura_archivo.is_open())
				 {

				 	for (int contador_linea = 1; contador_linea < n+1; contador_linea++) {
                    escritura_archivo << escritura[contador_linea-1]+"\n";

				 }

			escritura_archivo.close();
		}


}
///@return devuelve cero cuando se completa
return 0;

}
