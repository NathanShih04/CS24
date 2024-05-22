#ifndef INDEX_H
#define INDEX_H

#include <string>
#include "List.h"

class Index {
private:
    struct Entry {
        std::string key;
        Node* node;
        bool is_occupied;
        Entry() : node(nullptr), is_occupied(false) {}
    };

    Entry* table;
    size_t capacity;
    size_t size;
    float load_factor;

    size_t hash(const std::string& key) const;
    void rehash();

public:
    Index(size_t initial_capacity = 16384, float load_factor = 0.75);
    ~Index();

    void insert(const std::string& key, Node* node);
    Node* find(const std::string& key) const;
    void remove(const std::string& key);
};

#endif