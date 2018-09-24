#include <iostream>
#include <string>


using namespace std;

class Comparator{
	public:
		Comparator();
		~Comparator();
		string translate(string s);
		string* translate(string* s, int n);
	private:
};
