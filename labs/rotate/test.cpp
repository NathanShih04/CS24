#include "Tree.h"
#include <iostream>

// This file is for you to test your tree functions.
// It won't be graded - do whatever you want.

int main() {
  Tree tree;

  // Do tree things!
  tree.insert("Sunday");
  tree.insert("Monday");
  tree.insert("Tuesday");
  tree.insert("Friday");
  tree.insert("Saturday");
  tree.insert("Thursday");
  tree.insert("Wednesday");

  tree.print();

  return 0;
}
