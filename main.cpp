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
using namespace std::chrono;

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
    cout<<"allocated"<<endl;
    //huffTree->encoded= "";
    huffTree->nodes = nodes;
    cout<<"allocated"<<endl;

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
  cout<<nodes.size()<<endl;
  for(int i = 0; i < nodes.size(); i++){
    cout<<"       "<<i<<endl;
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
  cout<<t->nodes.size()<<endl;
  cout<<"finished creating tree"<<endl;
  printInorder(t->nodes[0]);
  encodeString(t,n);
  cout<<"finished encoding"<<endl;
  cout<<t->encoded<<endl;
  return t;
}

struct LzNode{
  int offset;
  int length;
  char endChar;
};

LzNode* newLzNode(int offset, int lengthMatch, char endChar){
  struct LzNode* temp = (struct LzNode*)malloc(sizeof(struct LzNode));
  temp->offset = offset;
  temp->length = lengthMatch;
  temp->endChar = endChar;
  return temp;
}

template<typename T>
void pop_front(vector<T> &v, int n){
  for(int i = 0; i < n || i < v.size();i++){
    if (v.size() > 0) {
        v.erase(v.begin());
    }
  }
}

//pop up to index n
void pop_front(string &s,int n){
  for(int i = 0; i < n;i++) s.erase(s.begin()+i);
}

vector<LzNode*> lz77(string n,int searchBsize, int lookBsize){
  vector<char> sb;
  vector<char> lab;
  vector<LzNode*> finalNodes;
  int moveBufferBy = 0;
  finalNodes.push_back(newLzNode(0,0,n[0]));
  sb.push_back(n[0]);
  pop_front(n,0);


  while(!n.empty()){
    char matchingFrom = n[0];
    vector<int> indexMatchingTo;
    int offset = 0, length = 0;
    char endChar = '\0';
    bool foundMatch = false;

    for(int i = searchBsize-1; i >= 0;i++){
      if(sb[i] == matchingFrom){
        foundMatch = true;
        int tempoffset = i, templength = 1;
        char tempChar = n[1];
        for(int j = 1;j<searchBsize+searchBsize-i-1;i++){
          if(i+j >= searchBsize){
            if(n[j-(searchBsize-i)] == n[j]){
              tempoffset = i;
              templength = j+1;
              if(j == n.size()-1){
                tempChar = n[j];
                break;
              }else{
                tempChar = n[j+1];
              }
            }else{
              break;
            }
          }
          if(sb[i+j] == n[j]){
            tempoffset = i;
            templength = j+1;
            if(j == n.size()-1){
              tempChar = n[j];
            }else{
              tempChar = n[j+1];
            }
          }else{
            break;
          }
        }
        if(templength > length){
          offset = tempoffset;
          length = templength;
          endChar = tempChar;
        }
      }
    }

    finalNodes.push_back(newLzNode(offset,length,endChar));

    if(!foundMatch) length = 1;
    if(sb.size() == searchBsize || sb.size() + length < searchBsize){
      if(sb.size() + length < searchBsize){
        pop_front(sb,sb.size() + length - searchBsize);
      }else{
        pop_front(sb,length);
      int sbStart = 0;
      if(length > searchBsize) sbStart = length-searchBsize;
      for(int i = sbStart; i < length;i++){
        sb.push_back(n[i]);
      }
    }
    }else{
      for(int i = 0; i < length;i++){
        sb.push_back(n[i]);
      }
    }

    pop_front(n,length);
  }
  finalNodes.insert(finalNodes.begin(),newLzNode(searchBsize,lookBsize,'$'));
  cout<<finalNodes.size()<<endl;
  return finalNodes;
}


int main(){


  string total = "", n = "";
  bool start = false;
  int searchBsize = 6,lookBsize = 6;
  cout<<"welcome to the Huffman vs LZ77 compression algorithm comparison."<<endl;
  //cout<<"Please enter the searchBsize and lookBsize for LZ77 algorithm: ";
  //cin>>searchBsize;
  //cin>>lookBsize;

  cout << "Enter String for compression test between Huffman and LZ77: ";
  while(getline(cin,n)){
    total = total + n;
  }

  high_resolution_clock::time_point t1 = high_resolution_clock::now();
  HuffTree* compressedStr1 = huffManCompression(total);
  high_resolution_clock::time_point t2 = high_resolution_clock::now();
  cout<<sizeof(total)<<"  ";
  cout<<sizeof(compressedStr1)<<endl;

  duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
  cout<<"Huffman Compression took "<<time_span.count()<<"s"<<endl;

  high_resolution_clock::time_point t3 = high_resolution_clock::now();
  vector<LzNode*> compressedStr2 = lz77(n,searchBsize,lookBsize);
  high_resolution_clock::time_point t4 = high_resolution_clock::now();
  duration<double> time_span2 = duration_cast<duration<double>>(t4 - t3);
  cout<<"LZ77 Compression took "<<time_span2.count()<<"s"<<endl;

  return 0;
}
