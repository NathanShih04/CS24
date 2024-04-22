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
            board.winner();
            return 0;
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

    board.winner();
    board.gameEval();   

    return 0;
}
