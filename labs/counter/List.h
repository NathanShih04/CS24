#ifndef LIST_H
#define LIST_H

#include <iostream>
#include <string>

struct Node {
    std::string key;
    int value;
    Node* prev;
    Node* next;

    Node(const std::string& k, int v) : key(k), value(v), prev(nullptr), next(nullptr) {}
};

class List {
private:
    Node* head;
    Node* tail;

public:
    List();
    ~List();
    void insert(const std::string& key, int value);
    Node* find(const std::string& key);
    void remove(Node* node);
};

#endif // LIST_H
// #ifndef LIST_H
// #define LIST_H

// #include <iostream>
// #include <string>
// using namespace std;

// struct Node{
//     string key;
//     int value;
//     Node* prev;
//     Node* next;
// };

// class List{
// public:
//     Node* head;
//     Node* tail;
    
// public:
//     List();
//     ~List();
//     void insert(string key, int value);
//     Node* find(string key);
//     void remove(Node* node);
// };

// #endif
