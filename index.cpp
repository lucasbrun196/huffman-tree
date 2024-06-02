#include <iostream>
#include <fstream>
#include <map>

using namespace std;

struct Node{
    int frequency;
    char word;
    Node *next;
};


bool charSearch(const char &ch, const map<char, int> &frequency_map) {
    return frequency_map.find(ch) != frequency_map.end();
}

void recursiveInsertToLinkedList(Node *&nodeFirst, Node *&nodeToInsert){
    if(!nodeFirst->next){
        nodeFirst->next = nodeToInsert;
        return;
    }
    if(nodeFirst->next->frequency > nodeToInsert->frequency){
        nodeToInsert->next = nodeFirst->next;
        nodeFirst->next = nodeToInsert;
        return;
    }else{
        recursiveInsertToLinkedList(nodeFirst->next, nodeToInsert);
    }
}

void sortedLinkedList(const char wd, const int fqc, Node *&nodeFirst){
    Node *node = new Node;
    node->frequency = fqc;
    node->word = wd;
    if(node->frequency < nodeFirst->frequency){
        node->next = nodeFirst;
        nodeFirst = node;
    }else{
        recursiveInsertToLinkedList(nodeFirst, node);
    }
}

//just to teste:
void showLinkedList(Node *first){
    if(!first) return;
    cout << first->frequency << " - ";
    cout << first->word << endl;
    showLinkedList(first->next);
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
    
    
    
    Node *nodeFirst = new Node;
    int contAux = 0;
    for(auto conj : frequency_map){
        if(contAux == 0){
            nodeFirst->word = conj.first;
            nodeFirst->frequency = conj.second;
            nodeFirst->next = nullptr;
        }
        else{
            sortedLinkedList(conj.first, conj.second, nodeFirst);
        }
        contAux++;
    }

    //just to teste:
    showLinkedList(nodeFirst);
    

    return 0;
}
