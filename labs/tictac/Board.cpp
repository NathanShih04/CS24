#include "Errors.h"
#include "Board.h"
#include <string.h>
#include <iostream>
using namespace std;

// Space for implementing Board functions.
Board::Board(){
    char board[3][3] = {
        {' ', ' ', ' '},
        {' ', ' ', ' '},
        {' ', ' ', ' '}
    };
    int moveNum = 0;
    char player = ' ';
}

void Board::check(Move move){
    if(move.number != moveNum + 1){
        throw InvalidMove("Invalid move.");
    }
    else if(move.player == player){
        throw InvalidMove("Invalid move.");
    }
    else if(board[move.row][move.column] != ' '){
        throw InvalidMove("Invalid move.");
    }

    board[move.row][move.column] = move.player;
    moveNum++;
    player = move.player;

}

void Board::winner(){

    // side to side and up and down for player X and O
    for(int i = 0; i < 3; i++){
        if((board[i][0] == 'X' && board[i][1] == 'X' && board[i][2] == 'X') || (board[0][i] == 'X' && board[1][i] == 'X' && board[2][i] == 'X')){
            cout << "Game over: X wins.\n";
            exit(0);
        }
        else if((board[i][0] == 'O' && board[i][1] == 'O' && board[i][2] == 'O') || (board[0][i] == 'O' && board[1][i] == 'O' && board[2][i] == 'O')){
            cout << "Game over: O wins.\n";
            exit(0);
        }
    }

    // diagonals for player X and O
    if((board[0][0] == 'X' && board[1][1] == 'X' && board[2][2] == 'X') || (board[0][2] == 'X' && board[1][1] == 'X' && board[2][0] == 'X')) {
        cout << "Game over: X wins.\n";
        exit(0);
    }

    else if((board[0][0] == 'O' && board[1][1] == 'O' && board[2][2] == 'O') || (board[0][2] == 'O' && board[1][1] == 'O' && board[2][0] == 'O')){
        cout << "Game over: O wins.\n";
        exit(0);
    }

}

void Board::gameEval(){

    if(moveNum == 0){
        cout << "Game in progress: New game.\n";
        exit(0);
    }
    else if(player == 'X'){
        cout << "Game in progress: O's turn.\n";
        exit(0);
    }
    else if(player == 'O'){
        cout << "Game in progress: X's turn.\n";
        exit(0);
    }
    else if(moveNum == 9){
        cout << "Game over: Draw.\n";
        exit(0);
    }
}