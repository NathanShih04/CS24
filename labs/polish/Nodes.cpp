#include "Nodes.h"

#include <sstream>
#include <string>

// This creates the number format the autograder expects:
std::string format(double number) {
  std::ostringstream stream;
  stream << number;
  return stream.str();
}


// Implement your AST subclasses' member functions here.
std::string Number::prefix() const {
    // Add your code here
}

// Implement postfix function here
std::string Number::postfix() const {
    // Add your code here
}

// Implement value function here
double Number::value() const {
    // Add your code here
}

// Implement prefix function here
std::string Addition::prefix() const {
    // Add your code here
}

// Implement postfix function here
std::string Addition::postfix() const {
    // Add your code here
}

// Implement value function here
double Addition::value() const {
    // Add your code here
}

// Implement prefix function here
std::string Subtraction::prefix() const {
    // Add your code here
}

// Implement postfix function here
std::string Subtraction::postfix() const {
    // Add your code here
}

// Implement value function here
double Subtraction::value() const {
    // Add your code here
}

// Implement prefix function here
std::string Multiplication::prefix() const {
    // Add your code here
}

// Implement postfix function here
std::string Multiplication::postfix() const {
    // Add your code here
}

// Implement value function here
double Multiplication::value() const {
    // Add your code here
}

// Implement prefix function here
std::string Division::prefix() const {
    // Add your code here
}

// Implement postfix function here
std::string Division::postfix() const {
    // Add your code here
}

// Implement value function here
double Division::value() const {
    // Add your code here
}

// Implement prefix function here
std::string Modulo::prefix() const {
    // Add your code here
}

// Implement postfix function here
std::string Modulo::postfix() const {
    // Add your code here
}

// Implement value function here
double Modulo::value() const {
    // Add your code here
}