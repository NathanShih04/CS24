#include "Node.h"

// Node Function Implementations
Node::Node(){
    next = nullptr;
    word = "";
    left = nullptr;
    right = nullptr;

}

Node::Node(string data){
    next = nullptr;
    word = data;
    left = nullptr;
    right = nullptr;

}