#include "Stack.h"

Stack::Stack(int size){
    count = 0;
    stack = new AST*[size];
}

Stack::~Stack() {
    delete[] stack;
}

void Stack::push(AST* node) {
    stack[count] = node;
    count++;
}

AST* Stack::pop() {
    if(count == 0){
        return nullptr;
    }

    count--;
    AST* poppedData = stack[count];

    // Pop off the last node
    int length = 50;
    AST** newStack = new AST*[length];
    for(int i = 0; i < count; i++){
        newStack[i] = stack[i];
    }

    stack = newStack;

    return poppedData;
}

AST* Stack::peek(){
    return stack[count];
}