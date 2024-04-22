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
            board.check(userMove);
            board.winner();
            return 0;
        } 
        catch(const ParseError& e) {
            std::cout << "Parse error: " << e.what() << '\n';
            return 1;
        }
        catch (const InvalidMove& e) {
            std::cout << "Invalid move error: " << e.what() << '\n';
            return 2; // Return 2 if it's an InvalidMove
        }

    }

    board.winner();
    board.gameEval();   

    return 0;
}
