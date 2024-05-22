#ifndef COUNTER_H
#define COUNTER_H

#include "List.h"
#include "Index.h"
#include <string>

class Counter {
public:
    class Iterator {
        Node* node;

    public:
        Iterator(Node* node);
        ~Iterator();
        const std::string& key() const;
        int value() const;
        void operator++();
        bool operator==(const Iterator& other) const;
        bool operator!=(const Iterator& other) const;
    };

private:
    List* list;
    size_t Lcount;
    int Ltotal;

public:
    Counter();
    ~Counter();

    size_t count() const;
    int total() const;

    void inc(const std::string& key, int by = 1);
    void dec(const std::string& key, int by = 1);
    void del(const std::string& key);
    int get(const std::string& key) const;
    void set(const std::string& key, int count);

    Iterator begin() const;
    Iterator end() const;
};

#endif