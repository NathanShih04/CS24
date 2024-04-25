#include "Node.h"

// Node Function Implementations
Node::Node(){
    weight = 0;
    word = "";
    left = nullptr;
    right = nullptr;

}

Node::Node(string data){
    weight = 1;
    word = data;
    left = nullptr;
    right = nullptr;

}