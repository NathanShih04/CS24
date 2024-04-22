#ifndef BOARD_H
#define BOARD_H

#include "Move.h"

// I recommended writing a Board class to manage your game state.
// Here's some space for the class definition; member functions go in Board.cpp.

struct Board{

    char board[3][3];
    int moveNum;
    char player;

    // Constructor finds and displays game status.
    Board();

    void check(Move move);

    void winner();

    void gameEval();
};


#endif