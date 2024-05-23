#include <iostream>
#include <fstream>
#include <map>

using namespace std;

bool charSearch(const char& ch, const map<char, int>& frequency_map) {
    return frequency_map.find(ch) != frequency_map.end();
}

int main(){
    ifstream arq;
    char ch;
    int count_char;
    map<char, int> frequency_map;

    arq.open("./txt/lorem.txt");
    
    if(!arq){
        cout << "Erro ao abrir arquivo!\n";
    }

    
    while(arq.get(ch)){
        if(charSearch(ch, frequency_map)){
            frequency_map[ch]++;
        }else{
            frequency_map[ch] = 1;
        }
    }
    

    for(auto conj: frequency_map){
        cout << conj.first << " - " << conj.second << endl; 
    }
    return 0;
}