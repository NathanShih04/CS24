#include "Counter.h"

using namespace std;

// Constructor
Counter::Counter() {}

// Destructor
Counter::~Counter() {}

size_t Counter::count() const {
    return Lcount;
}

int Counter::total() const {
    return Ltotal;
}

void Counter::inc(const std::string& key, int by) {
    Node* node = list->find(key);
    if(node != nullptr){
        node->value += by;
    } 
    else{
        list->insert(key, by);
        Lcount++;
    }
    Ltotal += by;

}

void Counter::dec(const std::string& key, int by) {
    Node* node = list->find(key);
    
    if(node != nullptr){
        node->value -= by;
        if(node->value == 0){
            list->remove(node);
            Lcount--;
        }
    } 
    else{
        list->insert(key, -by);
        Lcount++;
    }
    Ltotal -= by;
}

void Counter::del(const std::string& key) {
    Node* node = list->find(key);
    if(node != nullptr){
        Ltotal -= node->value;
        list->remove(node);
        Lcount--;
    }
}

int Counter::get(const std::string& key) const {
    Node* node = list->find(key);
    if(node != nullptr){
        return node->value;
    } 
    else{
        return 0;
    }
}

void Counter::set(const std::string& key, int count) {
    Node* node = list->find(key);
    if(node){
        Ltotal += (count - node->value);
        node->value = count;
    } 
    else{
        list->insert(key, count);
        Lcount++;
        Ltotal += count;
    }
}

Counter::Iterator Counter::begin() const {
    return Iterator(list->head);
}

Counter::Iterator Counter::end() const {
    return Iterator(nullptr);
}