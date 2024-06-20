#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

struct Node{
    int frequency;
    char word;
    string code;
    Node *next, *right, *left;
};

struct Frequency{
    char character;
    int frequency;
};

struct Size{
    string code;
    int frequency;
};

int charIsInVector(vector<Frequency> freq_vec, char ch){
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

void sortedLinkedList(const char wd, const int fqc, Node *&nodeFirst){
    Node *node = new Node;
    node->frequency = fqc;
    node->word = wd;
    node->left = nullptr;
    node->right = nullptr;
    node->code = "";
    if(node->frequency < nodeFirst->frequency){
        node->next = nodeFirst;
        nodeFirst = node;
    }else{
        recursiveInsertToLinkedList(nodeFirst, node);
    }
}

//just to test linked list:
void showLinkedList(Node *first){
    if(!first) return;
    
    cout << first->word << endl;
    cout << first->frequency << " - ";
    cout << first->code.size() << endl;

    showLinkedList(first->next);
}

//just to test tree:
void preOrder(Node *&root){
    if(!root) return;
    cout << root->word << ": " << root->code << endl;

    preOrder(root->left);
    preOrder(root->right);
}


void encodeTree(Node *&firstNode, string code, vector<string> code_aux){
    if(!firstNode) return;

    if(!firstNode->left && !firstNode->right){
        firstNode->code = code;
        code_aux.push_back(firstNode->code);
        return;
    }
    encodeTree(firstNode->left, code + "0", code_aux); 
    encodeTree(firstNode->right, code + "1", code_aux);
}

void exportNode(std::ofstream& dot, Node* node) {
    if (!node) return;
    if (!node->left && !node->right) {
        dot << "\"" << node->word << node->frequency << "\" [shape=record, label=\"{{" << node->word << "|" << node->frequency << "}|{" << node->code << "}}\"];\n";
    }
    else {
        dot << "\"" << node->word << node->frequency << "\" [label=\"" << node->frequency << "\"];\n";
    }
    if (node->left) {
        dot << "\"" << node->word << node->frequency << "\" -> \"" << node->left->word << node->left->frequency << "\" [label=0];\n";
        exportNode(dot, node->left);
    }
    if (node->right) {
        dot << "\"" << node->word << node->frequency << "\" -> \"" << node->right->word << node->right->frequency << "\" [label=1];\n";
        exportNode(dot, node->right);
    }
}

void exportHuffmanTreeToDot(Node* root, const std::string& filename) {
    std::ofstream dot(filename);
    dot << "digraph G {\n";

    exportNode(dot, root);

    dot << "}\n";
}

void draw(Node* root){
    exportHuffmanTreeToDot(root, "huffmanTree.dot");
    std::system("dot -Tpng huffmanTree.dot -o graph.png"); //Windows
    //std::system("dot -Tx11 huffmanTree.dot"); // Linux
}

void createTree(Node *&firtsNode, vector<int> freq_aux, vector<string> code_aux){
     if(!firtsNode->next){
        encodeTree(firtsNode, "", code_aux);
        freq_aux.push_back(firtsNode->frequency);
        draw(firtsNode);
        // preOrder(firtsNode);
        return;
    };
    Node *secondNode = firtsNode->next;
    Node *subTree = new Node;
    subTree->word = '+';
    subTree->frequency = firtsNode->frequency + secondNode->frequency;
    subTree->left = firtsNode;
    subTree->right = secondNode;

    recursiveInsertToLinkedList(firtsNode, subTree);
    createTree(secondNode->next, freq_aux, code_aux);
}

int calculateNewSize(const vector<int> &dict){
    int totalBits = 0;
    for(const auto &freq:dict){
        totalBits += freq;
    }

    return totalBits;
}

void encodedTextSize(Node *first, vector<int> auxi){
    if(!first->next){
        int tamanho = calculateNewSize(auxi);
        cout << tamanho << " bits." << endl;
        return;
    }

    if(first->code.size() > 0){
        int tam = first->frequency * first->code.size();
        auxi.push_back(tam);
    }

    encodedTextSize(first->next, auxi);
}

//TODO
// int calculatedOriginalSize(const vector<Frequency>& vec){
//     int totalBits = 0;
//     for(const auto& freq : vec) {
//         //sizeof(wchar_t) vai verificar o nº de bytes que o caracter unicode ocupa na memória
//         //multiplica por 8 (bits)
//         int charBits = sizeof(wchar_t) * 8;
//         totalBits += freq.frequency * charBits;
//     }
//     return totalBits;
// }

int main(){
    ifstream arq;
    char ch;
    int index;
    vector<Frequency> frequency_vec;
    vector<int> auxiliar_frequencia;
    vector<string> auxiliar_codificacao;
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
            nodeFirst->code = "";
        }
        else{
            sortedLinkedList(conj.character, conj.frequency, nodeFirst); 
        }
        contAux++;
    }

    createTree(nodeFirst, auxiliar_frequencia, auxiliar_codificacao);

    // int tamOriginal = calculatedOriginalSize(frequency_vec);
    // cout << "Tamanho original do arquivo: " << tamOriginal << " bits" << endl;

    cout << "Tamanho codificado do arquivo: ";
    encodedTextSize(nodeFirst, auxiliar_tamanho);

    arq.close();

    return 0;
}