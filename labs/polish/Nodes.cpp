#include "Nodes.h"


#include <sstream>
#include <string>
#include <cmath>
using namespace std;

// This creates the number format the autograder expects:
std::string format(double number) {
  std::ostringstream stream;
  stream << number;
  return stream.str();
}

// Implement your AST subclasses' member functions here.
Number::Number(double value){
  number = value;
}

std::string Number::prefix() const {
    return format(number);
}

std::string Number::postfix() const {
    return format(number);
}

double Number::value() const {
  return number;
}

// ###################################################

std::string Addition::prefix() const {
  return "+ " + left->prefix() + " " + right->prefix();
}

std::string Addition::postfix() const {
  return left->postfix() + " " + right->postfix() + " +";
}

double Addition::value() const {
  return left->value() + right->value();
}

// ###################################################

std::string Subtraction::prefix() const {
  return "- " + left->prefix() + " " + right->prefix();
}

std::string Subtraction::postfix() const {
  return left->postfix() + " " + right->postfix() + " -";
}

double Subtraction::value() const {
  return left->value() - right->value();
}

// ###################################################

std::string Multiplication::prefix() const {
  return "* " + left->prefix() + " " + right->prefix();
}

std::string Multiplication::postfix() const {
  return left->postfix() + " " + right->postfix() + " *";
}

double Multiplication::value() const {
  return left->value() * right->value();
}

// ###################################################

std::string Division::prefix() const {
  return "/ " + left->prefix() + " " + right->prefix();
}

std::string Division::postfix() const {
  return left->postfix() + " " + right->postfix() + " /";
}

double Division::value() const {
  if(right->value() == 0){
    throw std::runtime_error("Division by zero.");
  }
  return left->value() / right->value();
}

// ###################################################

std::string Modulo::prefix() const {
  return "% " + left->prefix() + " " + right->prefix();
}

std::string Modulo::postfix() const {
  return left->postfix() + " " + right->postfix() + " %";
}

double Modulo::value() const {
  if(right->value() == 0){
    throw std::runtime_error("Division by zero.");
  }
  return fmod(left->value(), right->value());
}

// ###################################################

Negate::~Negate() {
    delete child;
}

std::string Negate::prefix() const {
  return "~ " + child->prefix();
}

std::string Negate::postfix() const {
    return child->postfix() + " ~";
}

double Negate::value() const {
    return -child->value();
}