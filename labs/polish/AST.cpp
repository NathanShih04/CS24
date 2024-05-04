#include "AST.h"
#include "Nodes.h"
#include "Stack.h"
#include <sstream>

AST* AST::parse(const std::string& expression) {
    std::string token;
    std::istringstream stream(expression);
    Stack stack = Stack(expression.size());

    while(stream >> token) {
        for(int i = 0; i < token.length(); i++){
            if(!isdigit(i)){
                if(token == "+"){
                    AST* second = stack.pop();
                    AST* first = stack.pop();
                    AST* node = new Addition(first, second);
                    stack.push(node);
                }
                else if(token == "-"){
                    AST* second = stack.pop();
                    AST* first = stack.pop();
                    AST* node = new Subtraction(first, second);
                    stack.push(node);
                }
                else if(token == "*"){
                    AST* second = stack.pop();
                    AST* first = stack.pop();
                    AST* node = new Multiplication(first, second);
                    stack.push(node);
                }
                else if(token == "/"){
                    AST* second = stack.pop();
                    AST* first = stack.pop();
                    AST* node = new Division(first, second);
                    stack.push(node);
                }
                else if(token == "%"){
                    AST* second = stack.pop();
                    AST* first = stack.pop();
                    AST* node = new Modulo(first, second);
                    stack.push(node);
                }
                else if(token == "~"){
                    AST* first = stack.pop();
                    AST* node = new Negate(first);
                    stack.push(node);
                }
            }
        }
        AST* node = new Number(stod(token));
        stack.push(node);

    }

    // ...
}
