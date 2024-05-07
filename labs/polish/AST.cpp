#include "AST.h"
#include "Nodes.h"
#include "Stack.h"
#include <sstream>
#include <string>
#include <iostream>

AST* AST::parse(const std::string& expression) {
    std::string token;
    std::istringstream stream(expression);
    Stack stack = Stack(expression.size());

    try {
        while(stream >> token){

            if((isdigit(token[0]) == false) && (token.length() == 1)){
                if(token == "+"){
                    if(stack.stackSize() < 2){
                        throw std::runtime_error("Not enough operands.");
                    }
                    AST* second = stack.pop();
                    AST* first = stack.pop();
                    AST* node = new Addition(first, second);
                    stack.push(node);
                }
                else if(token == "-"){
                    if(stack.stackSize() < 2){
                        throw std::runtime_error("Not enough operands.");
                    }
                    AST* second = stack.pop();
                    AST* first = stack.pop();
                    AST* node = new Subtraction(first, second);
                    stack.push(node);
                }
                else if(token == "*"){
                    if(stack.stackSize() < 2){
                        throw std::runtime_error("Not enough operands.");
                    }
                    AST* second = stack.pop();
                    AST* first = stack.pop();
                    AST* node = new Multiplication(first, second);
                    stack.push(node);
                }
                else if(token == "/"){
                    if(stack.stackSize() < 2){
                        throw std::runtime_error("Not enough operands.");
                    }
                    AST* second = stack.pop();
                    AST* first = stack.pop();
                    AST* node = new Division(first, second);
                    stack.push(node);
                }
                else if(token == "%"){
                    if(stack.stackSize() < 2){
                        throw std::runtime_error("Not enough operands.");
                    }
                    AST* second = stack.pop();
                    AST* first = stack.pop();
                    AST* node = new Modulo(first, second);
                    stack.push(node);
                }
                else if(token == "~"){
                    if(stack.stackSize() < 1){
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
            // NUMBERS
            else if(token[0] == '-'){
                int counter = 0;
                for(unsigned int i = 1; i < token.length(); i++){
                    if(token[i] == '.'){
                        counter++;
                    }
                    if((isdigit(token[i]) == false) && (token[i] != '.')){
                        string message = "Invalid token: " + token;
                        throw std::runtime_error(message);
                    }
                }
                if(counter > 1){
                    string message = "Invalid token: " + token;
                    throw std::runtime_error(message);
                }

                try {
                    double value = std::stod(token.substr(1));
                    AST* node = new Number(-1 * value);
                    stack.push(node);
                } 
                catch (const std::invalid_argument& e) {
                    string message = "Invalid token: " + token;
                    throw std::runtime_error(message);
                }
            }
            else if(token[0] == '+'){
                int counter = 0;
                for(unsigned int i = 1; i < token.length(); i++){
                    if(token[i] == '.'){
                        counter++;
                    }
                    if((isdigit(token[i]) == false) && (token[i] != '.')){
                        string message = "Invalid token: " + token;
                        throw std::runtime_error(message);
                    }
                }
                if(counter > 1){
                    string message = "Invalid token: " + token;
                    throw std::runtime_error(message);
                }
                
                try {
                    double value = std::stod(token.substr(1));
                    AST* node = new Number(value);
                    stack.push(node);
                } 
                catch (const std::invalid_argument& e) {
                    string message = "Invalid token: " + token;
                    throw std::runtime_error(message);
                }
            }
            else{
                int counter = 0;
                for(unsigned int i = 0; i < token.length(); i++){
                    if(token[i] == '.'){
                        counter++;
                    }
                    if((isdigit(token[i]) == false) && (token[i] != '.')){
                        string message = "Invalid token: " + token;
                        throw std::runtime_error(message);
                    }
                }
                if(counter > 1){
                    string message = "Invalid token: " + token;
                    throw std::runtime_error(message);
                }
                
                try {
                    double value = std::stod(token);
                    AST* node = new Number(value);
                    stack.push(node);
                } 
                catch (const std::invalid_argument& e) {
                    string message = "Invalid token: " + token;
                    throw std::runtime_error(message);
                }
            }

        }

        if(stack.stackSize() == 0){
            throw std::runtime_error("No input.");
        }
        else if(stack.stackSize() > 1){
            throw std::runtime_error("Too many operands.");
        }
        return stack.peek();
    }
    catch(...){
        while (stack.stackSize() > 0) {
            delete stack.pop();
        }
        throw;
    }
}