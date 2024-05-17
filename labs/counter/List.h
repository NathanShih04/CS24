#ifndef LIST_H
#define LIST_H

#include <iostream>
#include <string>
using namespace std;

struct Node{
    string key;
    int value;
    Node* prev;
    Node* next;
};

class List{
public:
    Node* head;
    Node* tail;
    
public:
    List();
    ~List();
    void insert(string key, int value);
    Node* find(string key);
    void remove(Node* node);
};

#endif
