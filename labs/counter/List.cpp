#include "List.h"

// List Member Functions

List::List(){
    head = nullptr;
    tail = nullptr;
}

List::~List() {
    Node* current = head;
    while (current != nullptr) {
        Node* nextNode = current->next;
        delete current;
        current = nextNode;
    }
}

void List::insert(const std::string& key, int value){
    Node* newNode = new Node(key, value);
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

Node* List::find(const std::string& key) {
    Node* current = head;
    while (current != nullptr) {
        if (current->key == key) {
            return current;
        }
        current = current->next;
    }
    return nullptr;
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
