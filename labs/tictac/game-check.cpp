#include "Board.h"
#include "Errors.h"
#include "Move.h"
#include <string>
#include <iostream>

using namespace std;

int main() {
    string userMove;
    Board board;

    while (getline(cin, userMove)) {
        try {  
            board.check(userMove);
        } 
        catch(const std::exception& e) {
            // Some error thingies???
        }
    }

    board.winner();
    board.gameEval();
    
    return 0;
}
