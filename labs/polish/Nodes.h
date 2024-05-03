#ifndef NODES_H
#define NODES_H

#include "AST.h"

// Declare your AST subclasses here.
#include <string>
using namespace std;

class Number : public AST {
private:
    double number;
public:
    Number(double value);
    std::string prefix() const override;
    std::string postfix() const override;
    double value() const override;
};

// ----------------------------------------------------------------

class Operator : public AST {
protected:
    AST* left;
    AST* right;
public:
    Operator(AST* L, AST* R) : left(L), right(R) {}
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

class Negate : public AST{
protected:
    AST* child;
public:
    Negate(AST* child) : child(child) {}
    virtual ~Negate();
    std::string prefix() const override;
    std::string postfix() const override;
    double value() const override;
};

#endif