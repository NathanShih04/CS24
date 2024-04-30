#include "Tree.h"
#include <iostream>

// This file is for you to test your tree functions.
// It won't be graded - do whatever you want.

int main() {
  Tree tree;

  // Do tree things!
  tree.insert("badger");
  tree.insert("badger");
  tree.insert("badger");
  tree.insert("mushroom");
  tree.insert("mushroom");
  tree.insert("mushroom");
  tree.insert("snake");
  tree.insert("snake");
  tree.print();
  cout << "Mushroom found at index: " << tree.find("snake") << "\n";

  return 0;
}
