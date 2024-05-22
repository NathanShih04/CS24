#include "List.h"

List::List() : head(nullptr), tail(nullptr) {}

List::~List() {
    Node* current = head;
    while(current != nullptr){
        Node* nextNode = current->next;
        delete current;
        current = nextNode;
    }
}

void List::insert(const std::string& key, int value) {
    Node* newNode = new Node{key, value, tail, nullptr};
    if(tail != nullptr){
        tail->next = newNode;
    } 
    else{
        head = newNode;
    }

    tail = newNode;
}

Node* List::find(const std::string& key) const {
    Node* current = head;
    while(current != nullptr){
        if(current->key == key){
            return current;
        }
        current = current->next;
    }

    return nullptr;
}

void List::remove(Node* node) {
    if(!node){
        return;
    }

    if(node->prev){
        node->prev->next = node->next;
    } 
    else{
        head = node->next;
    }

    if(node->next){
        node->next->prev = node->prev;
    } 
    else{
        tail = node->prev;
    }
    
    delete node;
}
