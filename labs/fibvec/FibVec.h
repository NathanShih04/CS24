#ifndef FIBVEC_H
#define FIBVEC_H

// This provides the size_t type:
#include <cstddef>

class FibVec {
  // Member Variables
  int *vec;
  size_t vCapacity;
  size_t vCount;
  // Helper Functions

public:
  // Constructor and Destructor
  FibVec();

  ~FibVec();

  // Member Functions
  size_t capacity() const;

  size_t count() const;

  void insert(int value, size_t index);

  int lookup(size_t index) const;

  int pop();

  void push(int value);

  int remove(size_t index);

};

#endif
