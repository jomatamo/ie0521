
///Este programa se encarga de leer los archivos de texto y convertirlos en strings.
#define READER

#include <iostream>

using namespace std;

///@class Reader
class Reader{

  private:

    string archivo;

  public:

    Reader();
    Reader(string s);
    ~Reader();

    string read();

    int write(string escritura);

    string* readlines();

    int write(string* s, int n);
};
