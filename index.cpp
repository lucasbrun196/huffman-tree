#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

struct Node{
    int frequency;
    char word;
    Node *next, *right, *left;
};

struct Frequency{
    char character;
    int frequency;
};

int charIsInVecto(vector<Frequency> freq_vec, char ch){
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
//     cout << first->frequency << " - ";
//     cout << first->word << endl;
//     showLinkedList(first->next);
// }

//just to test tree:
// void preOrder(Node *&root){
//     if(!root) return;
//     cout << root->word << endl;
//     preOrder(root->left);
//     preOrder(root->right);
// }

void creteTree(Node *&firtsNode){
    if(!firtsNode->next){
        //preOrder(firtsNode);
        return;
    };
    Node *secondNode = firtsNode->next;
    Node *subTree = new Node;
    subTree->word = '+';
    subTree->frequency = firtsNode->frequency + secondNode->frequency;
    subTree->left = firtsNode;
    subTree->right = secondNode;
    recursiveInsertToLinkedList(firtsNode, subTree);
    creteTree(secondNode->next);
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
        index = charIsInVecto(frequency_vec, ch);
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
        }
        else{
            sortedLinkedList(conj.character, conj.frequency, nodeFirst);
        }
        contAux++;
    }


    creteTree(nodeFirst);

    //just to teste:
    //showLinkedList(nodeFirst);
    arq.close();

    return 0;
}