#ifndef STACK_H
#define STACK_H

#include "AST.h"
#include "Nodes.h"

// Use this file to define your Stack type.
// Implement its member functions in Stack.cpp.

class Stack {
public:
    int count;
private:
    AST** stack;

public:
    Stack(int size);
    ~Stack();
    void push(AST* node);
    AST* pop();
    AST* peek();
    int stackSize();
};


#endif
