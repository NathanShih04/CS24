#include "List.h"

// List Member Functions

List::List(){
    head = nullptr;
    tail = nullptr;
}

List::~List() {
    while(head != nullptr){
        Node* temp = head;
        head = head->next;
        delete temp;
    }
    
    head = nullptr;
    tail = nullptr;
}

void List::insert(std::string key, int value){
    Node* newNode = new Node();
    newNode->key = key;
    newNode->value = value;
    newNode->next = nullptr;

    if(head == nullptr){
        head = newNode;
        tail = newNode;
        newNode->prev = nullptr;
    } 
    else{
        tail->next = newNode;
        newNode->prev = tail;
        tail = newNode;
    }
}

Node* List::find(std::string key){
    Node* current = nullptr;
    if(head != nullptr){
        current = head;
    }
    while(current != nullptr){
        if(current->key == key){
            return current;
        }
        current = current->next;
    }
    return current;
}

void List::remove(Node* node) {
    if(node == nullptr){
        return;
    }

    if(node->prev != nullptr){
        node->prev->next = node->next;
    } 
    else{
        head = node->next;
    }

    if(node->next != nullptr){
        node->next->prev = node->prev;
    } 
    else{
        tail = node->prev;
    }

    delete node;
}
