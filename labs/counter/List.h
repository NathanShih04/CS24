#ifndef LIST_H
#define LIST_H

#include <string>

struct Node {
    std::string key;
    int value;
    Node* prev;
    Node* next;
};

class List {
public:
    Node* head;
    Node* tail;

public:
    List();
    ~List();
    void insert(const std::string& key, int value);
    Node* find(const std::string& key) const;
    void remove(Node* node);
};

#endif
