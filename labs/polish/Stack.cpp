#include "Stack.h"

Stack::Stack(int size){
    count = 0;
    stack = new AST*[size];
}

Stack::~Stack() {
    delete[] stack;
}

void Stack::push(AST* newData) {
    stack[count] = newData;
}

AST* Stack::pop() {
    if(count == 0){
        return nullptr;
    }

    AST* poppedData = stack[count - 1];

    return poppedData;
}