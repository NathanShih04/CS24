#include "FibVec.h"

// This provides exception types:
#include <stdexcept>


// Fibonacci Helper
int fibHelp(int num){ 
    if(num <= 1){
        return num; 
    }

    return fibHelp(num - 1) + fibHelp(num - 2); 
}

// FibVec Function Implementations

FibVec::FibVec(){
    vCapacity = 1; // size of storage buffer
    vec = new int[vCapacity];
    vCount = 0; // num items
    vNum = 1;
}

FibVec::~FibVec(){
    delete[] vec;
}

size_t FibVec::capacity() const{
    return vCapacity;
}

size_t FibVec::count() const{
    return vCount;
}

void FibVec::insert(int value, size_t index){
    if(index >= vCount){
        std::out_of_range("Out of range.");
    }

    vCount++;
    vCapacity = fibHelp(vCount + 1);

    for(unsigned int i = vCount; i > index; i--){
        vec[i + 1] = remove(i);
    }

    vec[index] = value;
}

int FibVec::lookup(size_t index) const{
    if(index >= vCount){
        std::out_of_range("Out of range.");
    }

    return vec[index];  
}

int FibVec::pop(){
    if(vCount == 0){
        std::underflow_error("Underflow error.");
    }

    return remove(vCount);
}

void FibVec::push(int value){
    vCount++;
    vCapacity = fibHelp(vCount + 1);

    vec[vCount] = value;
}

int FibVec::remove(size_t index){
    if(index >= vCount){
        std::out_of_range("Out of range.");
    }

    int holder = vec[index];
    vCount--;

    for(unsigned int i = index; i < vCount; i++){
        if(isdigit(vec[i + 1])){
            vec[i] = vec[i + 1];
        }
    }

    vCapacity = fibHelp(vCount + 1);

    return holder;
}