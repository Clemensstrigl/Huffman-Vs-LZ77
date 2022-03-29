// CSCI 411 - Spring 2022
// Project 1
// Author: Clemens Strigl
//010741264

#include <iostream>
#include <vector>
#include <algorithm>
#include <limits.h>
#include <cmath>
#include <chrono>
#include <cstring>
#include <map>
using namespace std;

struct Node {
    int freq;
    char c;
    Node* right;
    Node* left;
    bool isLeaf;
};

struct HuffTree{
  string encoded;
  vector<Node*> nodes;
};


struct Node* newNode(char c, int freq){
    struct Node* temp = (struct Node*) malloc(sizeof(struct Node));
    temp->left = temp->right = NULL;
    temp->c = c;
    temp->freq = freq;
    temp->isLeaf = true;
    return temp;
}

struct Node* newNode(Node* right, Node* left){
    struct Node* temp = (struct Node*) malloc(sizeof(struct Node));
    temp->left = left;
    temp->right = right;
    temp->freq = right->freq + left->freq;
    temp->isLeaf = false;
    return temp;
}

void pop_front(std::vector<Node*> &v)
{
    if (v.size() > 0) {
        v.erase(v.begin());
    }
}

void sortNodes(vector<Node*> &nodes){
  sort(nodes.begin(),nodes.end(),[](Node* a, Node* b){
    return a->freq < b->freq;
  });
}

struct HuffTree* initTree(vector<Node*> &nodes){
    struct HuffTree* huffTree = (struct HuffTree*)malloc(sizeof(struct HuffTree));
    //huffTree->encoded= "";
    huffTree->nodes = nodes;
    return huffTree;
}

void createTree(HuffTree *t){
  while(t->nodes.size() > 1){
    Node* left = t->nodes[0];
    pop_front(t->nodes);
    Node* right = t->nodes[0];
    pop_front(t->nodes);
    t->nodes.push_back(newNode(right, left));
    sortNodes(t->nodes);
  }
}



void printInorder(struct Node* node)
{
    if (node == NULL)
        return;

    /* first recur on left child */
    printInorder(node->left);

    /* then print the data of node */
    cout << node->freq << " ";

    /* now recur on right child */
    printInorder(node->right);
}

void getCharBits(struct Node* node, string code, map<char,string> &m){
    if (node->isLeaf){
        m[node->c] = code;
        return;
    }
    code = code + "0";

    getCharBits(node->left, code,m);
    code.erase(code.end()-1);
      code = code + "1";
    /* now recur on right subtree */
    getCharBits(node->right, code, m);
}
void encodeString(HuffTree*t, string n){
  map<char,string> m;
  string code = "";
  getCharBits(t->nodes[0],code,m);
  string final = "";
  for(int i = 0; i < n.size(); i++){
    final+= m[n[i]];
  }
  t->encoded = final;
}

HuffTree* huffManCompression(string n){
  map<char,int> cc;
  int sizeCC = 0;
  for(int i = 0; i < n.size();i++){
    n[i] = tolower(n[i]);
    cout<<(n[i])<<endl;
    cc[tolower(n[i])] ++;
    cout << cc[n[i]] << endl;
  }
  map<char,int>::iterator it;
  vector<Node*> nodes;
  for(int i = 0; i < n.size();i++){
    it = cc.find(n[i]);
    if (it != cc.end()){
      nodes.push_back(newNode(n[i],cc[n[i]]));
      cc.erase (it);
    }
  }
  cout<<"assigned into list"<<endl;
  sortNodes(nodes);
  cout<<"sorted"<<endl;
  for(int i = 0; i < nodes.size(); i++){
    cout<<nodes[i]->c<<endl;
    cout<<nodes[i]->freq<<endl;
  }

  HuffTree* t = initTree(nodes);
  cout<<"inited tree"<<endl;
  for(int i = 0; i < nodes.size(); i++){
    cout<<t->nodes[i]->c<<endl;
    cout<<t->nodes[i]->freq<<endl;
  }
  createTree(t);
  cout<<"finished creating tree"<<endl;
  printInorder(t->nodes[0]);
  encodeString(t,n);
  cout<<"finished encoding"<<endl;
  cout<<t->encoded<<endl;
  return t;
}

// HuffMan* huffManCompression(string n){
//   map<char,int> cc;
//   int sizeCC = 0;
//   for(int i = 0; i < n.size();i++){
//     n[i] = tolower(n[i]);
//     cout<<(n[i])<<endl;
//     cc[tolower(n[i])] ++;
//     cout << cc[n[i]] << endl;
//   }
//   characterCount* r[cc.size()];
//   sizeCC = cc.size();
//   cout<<cc.size()<<endl;
//   int index = 0;
//   cout<<"finished mapping"<<endl;
//   map<char,int>::iterator it;
//   int id = 0;
//   vector<char> characters(cc.size());
//   vector<int> freq(cc.size());
//   for(int i = 0; i < n.size();i++){
//     it = cc.find(n[i]);
//     if (it != cc.end()){
//       int insertIndex = 0;
//       for(int j = 0; j< freq.size();j++){
//         if(freq[j]< cc[n[i]]){
//           insertIndex = j;
//           break;
//         }
//       }
//       vector<int>::iterator itI;
//       vector<char>::iterator itC;
//       itC = characters.begin() + insertIndex;
//       characters.insert ( itC , n[i] );
//       itI = freq.begin()+ insertIndex;
//       freq.insert ( itI , cc[n[i]] );
//       cc.erase (it);
//     }
//
//   }
//   cout<<"assigned into list"<<endl;
//   //sort(r, r+cc.size(), compare);
//   cout<<"sorted"<<endl;
//   for(int i = 0; i < sizeCC; i++){
//     cout<<characters[i]<<endl;
//     cout<<freq[i]<<endl;
//   }
//
//   HuffTree* t = initTree();
//   createTree(t,characters,freq);
//
//
//
//   return NULL;
// }



int main(){
  cout << "Enter String for compression test between Huffman and LZ77: ";
  string n = "";
  getline(cin,n);
  cout<<n<<endl;

//  auto start = high_resolution_clock::now();
  HuffTree* compressedStr = huffManCompression(n);
//  auto stop = high_resolution_clock::now();
//  auto duration = duration_cast<microseconds>(stop - start);
  //cout<<"Huffman Compression took "<<duration<<"μs, and uses "<<bitesUsed<<"bites"<<endl;

  //start = high_resolution_clock::now();
  //compressed = LZ77(n);
//  stop = high_resolution_clock::now();
//  duration = duration_cast<microseconds>(stop - start);
  //cout<<"LZ77 Compression took "<<duration<<"μs, and uses "<<bitesUsed<<"bites"<<endl;

  return 0;
}
