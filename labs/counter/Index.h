#ifndef INDEX_H
#define INDEX_H

#include <string>

class Node;

class Index {
public:
    Index(size_t initial_capacity = 16, float load_factor = 0.75);
    ~Index();

    void insert(const std::string& key, Node* node);

    Node* find(const std::string& key) const;

    void remove(const std::string& key);

private:
    size_t hash(const std::string& key) const;

    void rehash();

    struct Entry {
        std::string key;
        Node* node;
        bool is_occupied = false;
        bool is_dirty = false;
    };

    Entry* table;
    size_t capacity;
    size_t size;
    float load_factor;
};

#endif
