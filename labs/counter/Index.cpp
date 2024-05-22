#include "Index.h"
#include <functional>
#include <iostream> 

Index::Index(size_t initial_capacity, float load_factor)
    : capacity(initial_capacity), size(0), load_factor(load_factor) {
    table = new Entry[capacity];
}

Index::~Index() {
    delete[] table;
}

size_t Index::hash(const std::string& key) const {
    return std::hash<std::string>{}(key) % capacity;
}

void Index::rehash() {
    size_t new_capacity = capacity * 2;
    Entry* new_table = new Entry[new_capacity];

    for (size_t i = 0; i < capacity; ++i) {
        if (table[i].is_occupied) {
            size_t new_index = std::hash<std::string>{}(table[i].key) % new_capacity;
            while (new_table[new_index].is_occupied) {
                new_index = (new_index + 1) % new_capacity;
            }
            new_table[new_index] = table[i];
        }
    }

    delete[] table;
    table = new_table;
    capacity = new_capacity;
}

void Index::insert(const std::string& key, Node* node) {
    if (size >= capacity * load_factor) {
        rehash();
    }

    size_t index = hash(key);
    while (table[index].is_occupied && table[index].key != key) {
        index = (index + 1) % capacity;
    }

    if (!table[index].is_occupied) {
        size++;
    }

    table[index].key = key;
    table[index].node = node;
    table[index].is_occupied = true;
}

Node* Index::find(const std::string& key) const {
    size_t index = hash(key);
    while (table[index].is_occupied && table[index].key != key) {
        index = (index + 1) % capacity;
    }

    if (table[index].is_occupied) {
        return table[index].node;
    } else {
        return nullptr;
    }
}

void Index::remove(const std::string& key) {
    size_t index = hash(key);
    while (table[index].is_occupied && table[index].key != key) {
        index = (index + 1) % capacity;
    }

    if (table[index].is_occupied) {
        table[index].is_occupied = false;
        size--;
    }
}
