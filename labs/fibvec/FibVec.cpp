#include "FibVec.h"

// This provides exception types:
#include <stdexcept>


// Fibonacci Helper
size_t fibHelp(size_t num){ 
    if(num <= 1){
        return num; 
    }

    return fibHelp(num - 1) + fibHelp(num - 2); 
}

int* resize(int capacity){
    int* resized = new int[capacity];
    return resized;
}

// FibVec Function Implementations

FibVec::FibVec(){
    vCapacity = 1; // size of storage buffer
    vec = new int[vCapacity];
    vCount = 0; // num items
    fibNum = 2;
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

    if(vCount == vCapacity){
        fibNum++;
        int* largerVec = resize(fibHelp(fibNum));
         // copy into array
        for(unsigned int i = 0; i < vCount; i++){
            largerVec[i] = vec[i];
        }

        delete[] vec;
        vec = largerVec;
    }

    vCapacity = fibHelp(fibNum);

    for(unsigned int i = vCount; i > index; i--){
        vec[i + 1] = vec[i];
    }

    vec[index] = value;
    vCount++;
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

    int* largerVec = resize(vCapacity);

    // copy into array
    for(unsigned int i = 0; i < vCount; i++){
        largerVec[i] = vec[i];
    }

    delete[] vec;
    vec = largerVec;

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

    if(vCount < fibHelp(vCount - 2)){
        fibNum--;
        int* largerVec = resize(fibHelp(fibNum));
         // copy into array
        for(unsigned int i = 0; i < vCount; i++){
            largerVec[i] = vec[i];
        }

        delete[] vec;
        vec = largerVec;
    }

    return holder;
}