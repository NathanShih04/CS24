#ifndef NODE_H
#define NODE_H

#include <string>
using namespace std;

struct Node {
  Node* next;
  string word;
  Node* left;
  Node* right;

  Node();

  Node(string data);
  
};

#endif