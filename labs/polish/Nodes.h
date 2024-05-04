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
    std::string prefix() const;
    std::string postfix() const;
    double value() const;
};

// ----------------------------------------------------------------

class Addition : public AST {
protected:
    AST* left;
    AST* right;
public:
    Addition(AST* left, AST* right) : left(left), right(right) {}
    std::string prefix() const override;
    std::string postfix() const override;
    double value() const override;
};

class Subtraction : public AST {
protected:
    AST* left;
    AST* right;
public:
    Subtraction(AST* left, AST* right) : left(left), right(right) {}
    std::string prefix() const override;
    std::string postfix() const override;
    double value() const override;
};

class Multiplication : public AST {
protected:
    AST* left;
    AST* right;
public:
    Multiplication(AST* left, AST* right) : left(left), right(right) {}
    std::string prefix() const override;
    std::string postfix() const override;
    double value() const override;
};

class Division : public AST {
protected:
    AST* left;
    AST* right;
public:
    Division(AST* left, AST* right) : left(left), right(right) {}
    std::string prefix() const override;
    std::string postfix() const override;
    double value() const override;
};

class Modulo : public AST {
protected:
    AST* left;
    AST* right;
public:
    Modulo(AST* left, AST* right) : left(left), right(right) {}
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
    std::string prefix() const;
    std::string postfix() const;
    double value() const;
};

#endif