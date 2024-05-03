#ifndef NODES_H
#define NODES_H

#include "AST.h"

// Declare your AST subclasses here.
#include <string>
using namespace std;

class Number : public AST {
private:
    int value;
public:

    std::string prefix() const override;
    std::string postfix() const override;
    double value() const override;
};

// ----------------------------------------------------------------

class Operator : public AST {
protected:
    AST* L;
    AST* R;
public:
    Operator(AST* left, AST* right) : L(left), R(right) {}
    virtual ~Operator();
};

class Addition : public Operator {
public:
    Addition(AST* left, AST* right) : Operator(left, right) {}
    std::string prefix() const override;
    std::string postfix() const override;
    double value() const override;

};

class Subtraction : public Operator {
public:
    Subtraction(AST* left, AST* right) : Operator(left, right) {}
    std::string prefix() const override;
    std::string postfix() const override;
    double value() const override;
};

class Multiplication : public Operator {
public:
    Multiplication(AST* left, AST* right) : Operator(left, right) {}
    std::string prefix() const override;
    std::string postfix() const override;
    double value() const override;
};

class Division : public Operator {
public:
    Division(AST* left, AST* right) : Operator(left, right) {}
    std::string prefix() const override;
    std::string postfix() const override;
    double value() const override;
};

class Modulo : public Operator {
public:
    Modulo(AST* left, AST* right) : Operator(left, right) {}
    std::string prefix() const override;
    std::string postfix() const override;
    double value() const override;
};

// ----------------------------------------------------------------

class Negate : public AST {

};

#endif
