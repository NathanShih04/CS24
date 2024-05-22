#include "Counter.h"

Counter::Iterator::Iterator(Node* node) : node(node) {}

Counter::Iterator::~Iterator() {}

const std::string& Counter::Iterator::key() const {
    return node->key;
}

int Counter::Iterator::value() const {
    return node->value;
}

void Counter::Iterator::operator++() {
    if (node) {
        node = node->next;
    }
}

bool Counter::Iterator::operator==(const Counter::Iterator& other) const {
    return node == other.node;
}

bool Counter::Iterator::operator!=(const Counter::Iterator& other) const {
    return node != other.node;
}