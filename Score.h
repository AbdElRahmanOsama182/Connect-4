#ifndef SCORE
#define SCORE
#include "Global.h"
//Bringing First Empty Row in Column
int lowest_row(int col)
{
    int row = -1;
    for (int i = paramrters.ROWS-1; i >= 0; i--)
    {
        if (board[i][col]==' ')
        {
            row = i;
            break;
        }
    }
    return row;
}
//Calculating Number Of Sequences
int countns(int base,int count)
{
    if (count >= base)
    {
        return count+1-base;
    }
    else
    {
        return 0;
    }
}
//Calculating The Score
int Score(int base, char player)
{
    int score = 0;
    //Horizontal
    for (int i = 0; i < paramrters.ROWS; i++)
    {
        int count = 0;
        for (int j = 0; j < paramrters.COLS; j++)
        {
            if (board[i][j] == player)
            {
                count++;
            }
            else if (j < paramrters.COLS-1)
            {
                score += countns(base,count);
                count = 0;
            }
            if (j == paramrters.COLS-1)
            {
                score += countns(base,count);
            }
        }
    }
    //Vertical
    for (int i = 0; i < paramrters.COLS; i++)
    {
        int count = 0;
        for (int j = 0; j < paramrters.ROWS; j++)
        {
            if (board[j][i] == player)
            {
                count++;
            }
            else if (j < paramrters.ROWS-1)
            {
                score += countns(base,count);
                count = 0;
            }
            if (j == paramrters.ROWS-1)
            {
                score += countns(base,count);
            }
        }
    }
    //Diagonal '\' Left to Right
    for (int s = -paramrters.COLS+1; s < paramrters.ROWS; s++)
    {
        int count = 0;
        for (int i = 0; i < paramrters.ROWS; i++)
        {
            if ((i+s >=paramrters.COLS)||(i<0))
            {
                score += countns(base,count);
                count = 0;
                continue;
            }
            if (board[i][i+s] == player)
            {
                count++;
            }
            else if (i < paramrters.ROWS-1)
            {
                score += countns(base,count);
                count = 0;
            }
            if (i == paramrters.ROWS-1)
            {
                score += countns(base,count);
            }
        }
    }
    //Diagonal '/' Right to Left
    for (int s = 0; s < paramrters.ROWS+paramrters.COLS-1; s++)
    {
        int count = 0;
        for (int i = 0; i < paramrters.ROWS; i++)
        {
            if ((s-i >= paramrters.COLS)||(s-i < 0))
            {
                score += countns(base,count);
                count = 0;
                continue;
            }
            if (board[i][s-i] == player)
            {
                count++;
            }
            else if (i < paramrters.ROWS-1)
            {
                score += countns(base,count);
                count = 0;
            }
            if (i == paramrters.ROWS-1)
            {
                score += countns(base,count);
            }
        }
    }
    return score;
}
//Caculating The Expected Score After One Move
int ScoreAtPoint(int base, char player,int col)
{
    board[lowest_row(col)][col] = player;
    int score = Score(base, player);
    board[lowest_row(col)+1][col] = ' ';
    return score;
}
//Caculating The Expected Score After Two Moves
int ScoreAt2Points(int base,char player, int col)
{
    int score;
    char opp;
    if (player=='X')
    {
        opp = 'O';
    }
    else
    {
        opp = 'X';
    }
    
    if (lowest_row(col)>0)
    {
        board[lowest_row(col)][col] = player;
        board[lowest_row(col)][col] = opp;
        score = Score(base, opp);
        board[lowest_row(col)+1][col] = ' ';
        board[lowest_row(col)+1][col] = ' ';
    }
    else
    {
        score = Score(base, opp);
    }
    return score;
}
#endif