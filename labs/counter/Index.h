#ifndef INDEX_H
#define INDEX_H

#include <string>
#include <vector>
#include "List.h"

class Index {
private:
    std::vector<Node*> table;
    size_t capacity;
    size_t size;
    float load_factor;

    size_t hash(const std::string& key) const;
    void rehash();

public:
    Index(size_t initial_capacity = 16, float load_factor = 0.75);
    ~Index();

    void insert(const std::string& key, Node* node);
    Node* find(const std::string& key) const;
    void remove(const std::string& key);
};

#endif
