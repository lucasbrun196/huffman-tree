/*
    NOMES: Manuela Antunes e Lucas Brun
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

struct Node{
    int frequency;
    wchar_t word;
    wstring code;
    Node *next, *right, *left;
};

struct Frequency{
    wchar_t character;
    int frequency;
};

struct Size{
    wstring code;
    int frequency;
};

int charIsInVector(vector<Frequency> freq_vec, wchar_t ch){
    for(int i = 0; i < freq_vec.size(); ++i){
        if(freq_vec[i].character == ch){
            return i;
        }
    }
    return -1;
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

void sortedLinkedList(const wchar_t wd, const int fqc, Node *&nodeFirst){
    Node *node = new Node;
    node->frequency = fqc;
    node->word = wd;
    node->left = nullptr;
    node->right = nullptr;
    node->code = L"";
    node->next = nullptr;
    if(node->frequency < nodeFirst->frequency){
        node->next = nodeFirst;
        nodeFirst = node;
    }else{
        recursiveInsertToLinkedList(nodeFirst, node);
    }
}

//just to test linked list:
// void showLinkedList(Node *first){
//     if(!first) return; 
//     cout << first->word << endl;
//     cout << first->frequency << L" - ";
//     cout << first->code.size() << endl;
//     showLinkedList(first->next);
// }

void preOrder(Node *&root){
    if(!root) return;
    if(root->word != '+'){
        wcout << root->word << " -- " << root->frequency << " -- " << root->code << endl;
    }
    preOrder(root->left);
    preOrder(root->right);
}

void encodeTree(Node *&firstNode, wstring code){
    if(!firstNode) return;

    if(!firstNode->left && !firstNode->right){
        firstNode->code = code;
        return;
    }
    encodeTree(firstNode->left, code + L"0"); 
    encodeTree(firstNode->right, code + L"1");
}


void createTree(Node *&firtsNode){
     if(!firtsNode->next){
        encodeTree(firtsNode, L"");
        preOrder(firtsNode);
        return;
    };
    Node *secondNode = firtsNode->next;
    Node *subTree = new Node;
    subTree->word = '+';
    subTree->frequency = firtsNode->frequency + secondNode->frequency;
    subTree->left = firtsNode;
    subTree->right = secondNode;

    recursiveInsertToLinkedList(firtsNode, subTree);
    createTree(secondNode->next);
}

int calculateNewSize(const vector<int> &dict){
    int totalBits = 0;
    for(const auto &freq : dict){
        totalBits += freq;
    }

    return totalBits;
}

int encodedTextSize(Node *first, vector<int> auxi){
    if(first == nullptr) return 0;
    if (first->code.size() > 0) {
        int tam = first->frequency * first->code.size();
        auxi.push_back(tam);
    }

    if (!first->next) {
        return calculateNewSize(auxi); // Se for o último nó, calcule o tamanho total
    }

    return encodedTextSize(first->next, auxi); // Continue na lista
}

int calculateOriginalSize(const vector<Frequency>& vec){
    int totalBits = 0;
    for(const auto& freq : vec) {
        //sizeof(wchar_t) vai verificar o nº de bytes que o caracter unicode ocupa na memória
        //multiplica por 8 (bits)
        int charBits = sizeof(wchar_t) * 8;
        totalBits += freq.frequency * charBits;
    }
    return totalBits;
}

void sizeDifference(int tamOriginal, int tamEncoded){
    int perc = ((tamOriginal - tamEncoded) * 100) / tamOriginal;
    int dif = 100 - perc;

    cout << "A diferenca percentual entre os tamanhos e de " << dif << "%." << endl;
}

int main(){
    wifstream arq;
    wchar_t ch;
    int index;
    vector<Frequency> frequency_vec;
    vector<int> auxiliar_frequencia;
    vector<wstring> auxiliar_codificacao;
    vector<int> auxiliar_tamanho;
    
    arq.open("./txt/lorem.txt");
    
    if(!arq){
        cout << "Erro ao abrir arquivo!\n";
    }
    
    while(arq.get(ch)){
        index = charIsInVector(frequency_vec, ch);
        if(index == -1){
            Frequency freq;
            freq.character = ch;
            freq.frequency = 1;
            frequency_vec.push_back(freq);
        }else{
            frequency_vec[index].frequency++;
        }
    }   
    
    Node *nodeFirst = new Node;

    int contAux = 0;

    for(auto conj : frequency_vec){
        if(contAux == 0){
            nodeFirst->word = conj.character;
            nodeFirst->frequency = conj.frequency;
            nodeFirst->next = nullptr;
            nodeFirst->left = nullptr;
            nodeFirst->right = nullptr;
            nodeFirst->code = L"";
        }
        else{
            sortedLinkedList(conj.character, conj.frequency, nodeFirst); 
        }
        contAux++;
    }

    createTree(nodeFirst);

    int tamOriginal = calculateOriginalSize(frequency_vec);
    cout << "Tamanho original do arquivo: " << tamOriginal << " bits" << endl;
    int tamCod = encodedTextSize(nodeFirst, auxiliar_tamanho);
    cout << "Tamanho codificado do arquivo: " << tamCod << " bits" << endl;
    sizeDifference(tamOriginal, encodedTextSize(nodeFirst, auxiliar_tamanho));
    arq.close();

    return 0;
}