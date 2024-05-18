#include <iostream>
#include <fstream>

using namespace std;

int main(){
    ifstream arq;
    string t;
    arq.open("lorem.txt");
    
    if(!arq){
        cout << "Erro ao abrir arquivo!\n";
    }
    return 0;
}