#include "Stack.h"

Stack::Stack(int size){
    count = 0;
    stack = new AST*[size];
}

Stack::~Stack() {
    delete[] stack;
}

void Stack::push(AST* node) {
    AST** newStack = new AST*[count + 2];

    for (int i = 0; i < count; i++) {
        newStack[i] = stack[i];
    }

    delete[] stack;

    stack = newStack;

    stack[count] = node;
    count++;
}

// void Stack::push(AST* node) {
//     stack[count] = node;
//     count++;
// }

AST* Stack::pop() {
    if(count == 0){
        return nullptr;
    }

    count--;
    AST* poppedData = stack[count];

    // Pop off the last node
    AST** newStack = new AST*[count];
    for(int i = 0; i < count; i++){
        newStack[i] = stack[i];
    }
    delete[] stack;
    stack = newStack;

    return poppedData;
}

// AST* Stack::pop() {
//     if(count == 0){
//         return nullptr;
//     }

//     count--;
//     AST* poppedData = stack[count];

//     // Pop off the last node
//     int length = 500;
//     AST** newStack = new AST*[length];
//     for(int i = 0; i < count; i++){
//         newStack[i] = stack[i];
//     }
//     for(int i = 0; i <= count; i++){
//         delete stack[i];
//     }
//     delete[] stack;
//     stack = newStack;

//     return poppedData;
// }

AST* Stack::peek(){
    return stack[count - 1];
}

int Stack::stackSize(){
    return count;
}