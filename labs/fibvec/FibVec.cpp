#include "FibVec.h"

// This provides exception types:
#include <stdexcept>


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

}

int FibVec::lookup(size_t index) const{
    if(index > vCount || index < 0){
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

}

int FibVec::remove(size_t index){
    if(index > vCount || index < 0){
        std::out_of_range("Out of range.");
    }

    int holder = vec[index];

    for(unsigned int i = index; i < vCount - index; i++){
        vec[i] = vec[i + 1];
    }

    return holder;
}