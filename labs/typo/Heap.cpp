#include "Heap.h"
#include <stdexcept>
using namespace std;

// Helper funcs ##########################################

void Heap::percolateUp(size_t index) {
    while(index > 0){
        size_t parentIndex = (index - 1) / 2;
        if(mData[index].score >= mData[parentIndex].score){
            break;
        }

        Entry temp = mData[index];
        mData[index] = mData[parentIndex];
        mData[parentIndex] = temp;
        index = parentIndex;
    }
}

void Heap::percolateDown(size_t index) {
    size_t leftChild, rightChild, smallestChild;

    while((leftChild = 2 * index + 1) < mCount){
        rightChild = leftChild + 1;

        if(rightChild < mCount && mData[rightChild].score < mData[leftChild].score){
            smallestChild = rightChild;
        } 
        else{
            smallestChild = leftChild;
        }

        if(mData[index].score <= mData[smallestChild].score){
            break;
        }
        
        Entry temp = mData[index];
        mData[index] = mData[smallestChild];
        mData[smallestChild] = temp;
        index = smallestChild;
    }
}

// #####################################################

Heap::Heap(size_t capacity) : mCapacity(capacity), mCount(0) {
    mData = new Entry[mCapacity];
}

Heap::Heap(const Heap& other) : mCapacity(other.mCapacity), mCount(other.mCount) {
    mData = new Entry[mCapacity];
    for(size_t i = 0; i < mCount; i++){
        mData[i] = other.mData[i];
    }
}

Heap::~Heap() {
    delete[] mData;
}

size_t Heap::capacity() const {
    return mCapacity;
}

size_t Heap::count() const {
    return mCount;
}

const Heap::Entry& Heap::lookup(size_t index) const {
    if(index >= mCount){
        throw std::out_of_range("Index out of range");
    }

    return mData[index];
}

Heap::Entry Heap::pop() {
    if(mCount == 0){
        throw underflow_error("Heap is empty");
    }

    Entry root = mData[0];
    mData[0] = mData[--mCount];
    percolateDown(0);
    return root;
}

Heap::Entry Heap::pushpop(const std::string& value, float score) {
    if(mCount == 0){
        throw underflow_error("Heap is empty");
    }

    Entry root = mData[0];
    mData[0] = {value, score};
    percolateDown(0);
    return root;
}

void Heap::push(const std::string& value, float score) {
    if(mCount >= mCapacity){
        throw overflow_error("Heap is full");
    }

    mData[mCount] = {value, score};
    percolateUp(mCount++);
}

const Heap::Entry& Heap::top() const {
    if(mCount == 0){
        throw underflow_error("Heap is empty");
    }

    return mData[0];
}