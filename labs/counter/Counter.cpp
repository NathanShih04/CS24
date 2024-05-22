
#include "Counter.h"

Counter::Counter() : list(new List()), index(new Index()), Lcount(0), Ltotal(0) {}

Counter::~Counter() {
    delete list;
    delete index;
}

size_t Counter::count() const {
    return Lcount;
}

int Counter::total() const {
    return Ltotal;
}

void Counter::inc(const std::string& key, int by) {
    Node* node = index->find(key);

    if(node != nullptr){
        node->value += by;
    } 
    else{
        list->insert(key, by);
        node = list->tail;
        index->insert(key, node);
        Lcount++;
    }

    Ltotal += by;
}

void Counter::dec(const std::string& key, int by) {
    Node* node = index->find(key);
    if(node != nullptr){
        node->value -= by;
    } 
    else{
        list->insert(key, -by);
        node = list->tail;
        index->insert(key, node);
        Lcount++;
    }

    Ltotal -= by;
}

void Counter::del(const std::string& key) {
    Node* node = index->find(key);
    if(node != nullptr){
        Ltotal -= node->value;
        list->remove(node);
        index->remove(key);
        Lcount--;
    }
}

int Counter::get(const std::string& key) const {
    Node* node = index->find(key);
    if(node != nullptr){
        return node->value;
    } 
    else{
        return 0;
    }
}

void Counter::set(const std::string& key, int count) {
    Node* node = index->find(key);
    if(node != nullptr){
        Ltotal += (count - node->value);
        node->value = count;
    } 
    else{
        list->insert(key, count);
        node = list->tail;
        index->insert(key, node);
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