#include "Errors.h"
#include "Move.h"
#include <string>
#include <sstream>
#include <bits/stdc++.h>
using namespace std;

// Space for implementing Move functions.

Move::Move(const std::string& input){

    vector<string> commands;
    string command;
    stringstream newInput(input);

    // stores commands seperated by spaces into vector
    while(newInput >> command){
        commands.push_back(command);

    }

    // checks to see if format is correct

    // not enough inputs
    if(commands.size() < 3){
        throw ParseError("Parse error.");
    }

    else if(commands[0].length() != 1 || isdigit(commands[0][0]) == false){
        throw ParseError("Parse error.");
    }

    else if((tolower(commands[1][0]) != 'x' && tolower(commands[1][0]) != 'o') || commands[1].length() != 1){
        throw ParseError("Parse error.");
    }

    else if(tolower(commands[2][0]) != 'a' && tolower(commands[2][0]) != 'b' && tolower(commands[2][0]) != 'c'){
        throw ParseError("Parse error.");
    }

    else if(commands[2][1] - '0' != 1 && commands[2][1] - '0' != 2 && commands[2][1] - '0' != 3){
        throw ParseError("Parse error.");
    }

    else if(commands.size() > 3){
        if(commands[3].size() > 1){
            throw ParseError("Parse error.");
        }
        else if(commands[3][0] != '#'){
            throw ParseError("Parse error.");
        }
    }

    // set vector elements to member variables
    number = stoi(commands[0]);
    player = commands[1][0];

    if(tolower(commands[2][0]) == 'a'){
        commands[2][0] = 1;
    }
    else if(tolower(commands[2][0]) == 'b'){
        commands[2][0] = 2;
    }
    else if(tolower(commands[2][0]) == 'c'){
        commands[2][0] = 3;
    }

    row = commands[2][0];
    column = commands[2][1] - '0';

}

// Helper to render a Move as text.
string Move::to_string() const{

    std::stringstream finalString;

    finalString << number << " " << player << " " << row << " " << column;

    string final = finalString.str();

    return final;
}