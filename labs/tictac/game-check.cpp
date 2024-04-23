#include "Board.h"
#include "Errors.h"
#include "Move.h"
#include <string>
#include <iostream>

using namespace std;

int main() {
    string userMove;
    Board board;

    while(getline(cin, userMove)){
        try {
            Move move(userMove);
            board.check(move);
        } 
        
        catch(const ParseError& e) {
            cout << "Parse error.\n";
            return 1;
        }

        catch(const InvalidMove& e) {
            cout << "Invalid move error.\n";
            return 2;
        }

    }

    if(board.winner() == 0){
        board.gameEval();   

    }
    else{
        board.winner();
    }

    return 0;
}
