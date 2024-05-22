#include "Index.h"
#include <functional>
#include <iostream>

using namespace std;

Index::Index(size_t initial_capacity, float load_factor) : table(initial_capacity, nullptr), capacity(initial_capacity), size(0), load_factor(load_factor) {}

Index::~Index() {}

size_t Index::hash(const std::string& key) const {
    return std::hash<std::string>{}(key) % capacity;
}

void Index::rehash() {
    size_t new_capacity = capacity * 2;
    vector<Node*> new_table(new_capacity, nullptr);

    for (size_t i = 0; i < capacity; ++i) {
        Node* node = table[i];
        if (node) {
            size_t new_index = std::hash<std::string>{}(node->key) % new_capacity;
            new_table[new_index] = node;
        }
    }

    table.swap(new_table);
    capacity = new_capacity;
}

void Index::insert(const std::string& key, Node* node) {
    if (size >= capacity * load_factor) {
        rehash();
    }

    size_t index = hash(key);
    while (table[index] && table[index]->key != key) {
        index = (index + 1) % capacity;
    }

    if (!table[index]) {
        size++;
    }

    table[index] = node;
}

Node* Index::find(const std::string& key) const {
    size_t index = hash(key);
    while (table[index] && table[index]->key != key) {
        index = (index + 1) % capacity;
    }

    return table[index];
}

void Index::remove(const std::string& key) {
    size_t index = hash(key);
    while (table[index] && table[index]->key != key) {
        index = (index + 1) % capacity;
    }

    if (table[index]) {
        table[index] = nullptr;
        size--;
    }
}
