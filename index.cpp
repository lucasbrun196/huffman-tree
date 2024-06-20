#include <iostream>
#include <fstream>
#include <vector>

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
    cout << first->frequency << " - ";
    cout << first->word << endl;
    cout << first->next->word << endl;
    cout << first->code << endl;

    showLinkedList(first->next);
}

//just to test tree:
void preOrder(Node *&root){
    if(!root) return;
    cout << root->word << ": " << root->code << endl;

    preOrder(root->left);
    preOrder(root->right);
}

void createTree(Node *&firtsNode){
     if(!firtsNode->next){
        // preOrder(firtsNode);
        // cout << firtsNode->word << " -> " << firtsNode->left->word << " | " << firtsNode->right->word << endl;
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

void encodeTree(Node *&root, string code){
    if(!root) return;

    cout << "OIIII:  " << root->next->word << endl;

    if(!root->left && !root->right){
        root->code = code;
        return;
    } else {
        if(root->left){
            cout << "Fui pra esquerda!" << endl;
            encodeTree(root->left, code + "0");
        } else {
            cout << "Fui pra direita!" << endl;
            encodeTree(root->right, code + "1");
        }
    }
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

int main(){
    ifstream arq;
    char ch;
    int index;
    vector<Frequency> frequency_vec;
    
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

    createTree(nodeFirst);

    cout << "LISTA: " << endl;

    encodeTree(nodeFirst, "");

    showLinkedList(nodeFirst);

    // cout << "OOOIIIIII3" << endl;

    // cout << nodeFirst->word << ": " << nodeFirst->code << endl;

    // draw(nodeFirst);

    //just to teste:
    //showLinkedList(nodeFirst);
    arq.close();

    return 0;
}