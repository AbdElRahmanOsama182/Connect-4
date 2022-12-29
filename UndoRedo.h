#ifndef UNDOREDO
#define UNDOREDO
#include "Global.h"
//Undo One Move If (2Players) Two Moves If (1Player)
void Undo(int mode)
{
    board[lowest_row(moves[moves_counter-1])+1][moves[moves_counter-1]] = ' ';
    moves_counter--;
    if (!mode)
    {
        board[lowest_row(moves[moves_counter-1])+1][moves[moves_counter-1]] = ' ';
        moves_counter--;
        tempPlayer = (tempPlayer.number == 1) ? player2 : player1;
    }

}
//Redo One Move If (2Players) Two Moves If (1Player)
void Redo(int mode)
{
    board[lowest_row(moves[moves_counter])][moves[moves_counter]] = tempPlayer.symbol;
    moves_counter++;
    if (!mode)
    {
        tempPlayer = (tempPlayer.number == 1) ? player2 : player1;
        board[lowest_row(moves[moves_counter])][moves[moves_counter]] = tempPlayer.symbol;
        moves_counter++;
    }
}
#endif