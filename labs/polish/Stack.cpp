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
    if (count == 0) {
        return nullptr;
    }

    count--;

    // Store the popped data
    int size = 500;
    AST* poppedData = stack[count];

    // Create a new stack with reduced size
    AST** newStack = new AST*[size];
    for (int i = 0; i < count; i++) {
        newStack[i] = stack[i];
    }

    // Delete the top node and the original stack
    delete stack[count];
    delete[] stack;

    // Update the stack pointer
    stack = newStack;

    return poppedData;
}


AST* Stack::peek(){
    return stack[count - 1];
}

int Stack::stackSize(){
    return count;
}