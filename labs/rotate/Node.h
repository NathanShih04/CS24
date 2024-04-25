#ifndef NODE_H
#define NODE_H

#include <string>
using namespace std;

struct Node {
  size_t weight;
  string word;
  Node* left;
  Node* right;

  Node();

  Node(string data);
  
};

#endif