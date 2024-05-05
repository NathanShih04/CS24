#include "AST.h"
#include "Nodes.h"
#include "Stack.h"
#include <sstream>

AST* AST::parse(const std::string& expression) {
    std::string token;
    std::istringstream stream(expression);
    Stack stack = Stack(expression.size());

    while(stream >> token) {
        for(unsigned int i = 0; i < token.length(); i++){
            if(!isdigit(i)){
                if(token == "+"){
                    if(stack.count < 2){
                        throw std::runtime_error("Not enough operands.");
                    }
                    AST* second = stack.pop();
                    AST* first = stack.pop();
                    AST* node = new Addition(first, second);
                    stack.push(node);
                }
                else if(token == "-"){
                    if(stack.count < 2){
                        throw std::runtime_error("Not enough operands.");
                    }
                    AST* second = stack.pop();
                    AST* first = stack.pop();
                    AST* node = new Subtraction(first, second);
                    stack.push(node);
                }
                else if(token == "*"){
                    if(stack.count < 2){
                        throw std::runtime_error("Not enough operands.");
                    }
                    AST* second = stack.pop();
                    AST* first = stack.pop();
                    AST* node = new Multiplication(first, second);
                    stack.push(node);
                }
                else if(token == "/"){
                    if(stack.count < 2){
                        throw std::runtime_error("Not enough operands.");
                    }
                    AST* second = stack.pop();
                    AST* first = stack.pop();
                    AST* node = new Division(first, second);
                    stack.push(node);
                }
                else if(token == "%"){
                    if(stack.count < 2){
                        throw std::runtime_error("Not enough operands.");
                    }
                    AST* second = stack.pop();
                    AST* first = stack.pop();
                    AST* node = new Modulo(first, second);
                    stack.push(node);
                }
                else if(token == "~"){
                    if(stack.count < 1){
                        throw std::runtime_error("Not enough operands.");
                    }
                    AST* first = stack.pop();
                    AST* node = new Negate(first);
                    stack.push(node);
                }
                else{
                    string message = "Invalid token: " + token;
                    throw std::runtime_error(message);
                }
            }
        }
        AST* node = new Number(stod(token));
        stack.push(node);

    }

    if(stack.count == 0){
        throw std::runtime_error("No input.");
    }
    else if(stack.count > 1){
        throw std::runtime_error("Too many operands.");
    }
    return stack.peek();

}
