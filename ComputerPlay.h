#ifndef CompPlay
#define CompPlay
#include "Global.h"
//Detirmining If The Board Is Empty To End The Game
int not_full()
{
    int full = 1;
    for (int i = 0; i < paramrters.COLS; i++)
    {
        if (board[0][i] == ' ')
        {
            full = 0;
            break;
        }
    }
    return !full;
}
//Computer Plays With The Chosen Difficulty
void Computer_Play(int level,char player,int mode)
{
    if (not_full())
    {
        char opp;
        if (player=='X')
        {
            opp = 'O';
        }
        else
        {
            opp = 'X';
        }
        int col = -1;
        if (level == 2)
        {
            //Looking Two Moves Forward To Find The Best Move (HARD)
            for (int n = 4; n > 1; n--)
            {
                int scoreX = Score(n,'X');
                int scoreOpp = Score(n,opp);
                int scoreO = Score(n,'O');
                for (int i = 0; i < paramrters.COLS; i++)
                {
                    if (lowest_row(i)!=-1)
                    {
                        if (((ScoreAtPoint(n,'X',i)+ScoreAtPoint(n,'O',i))>(scoreX+scoreO))&&(scoreOpp==ScoreAt2Points(n,player,i)))
                        {
                        col = i; scoreX = ScoreAtPoint(n,'X',i); scoreO = ScoreAtPoint(n,'O',i);
                        }
                    }
                }
                if (col != -1) 
                {
                    break;
                }
            }
        }
        if (level && (col== -1))
        {
            //Looking One Move Forward To Find The Best Move (MEDIUM)
            for (int n = 4; n > 1; n--)
            {
                int scoreX = Score(n,'X');
                int scoreO = Score(n,'O');
                for (int i = 0; i < paramrters.COLS; i++)
                {
                    if (lowest_row(i)!=-1)
                    {
                        if (((ScoreAtPoint(n,'X',i)+ScoreAtPoint(n,'O',i))>(scoreX+scoreO)))
                        {
                        col = i; scoreX = ScoreAtPoint(n,'X',i); scoreO = ScoreAtPoint(n,'O',i);
                        }
                    }
                }
                if (col != -1) 
                {
                    break;
                }
            }
        }
        if (col == -1)
        {
            //Choosing Random Column (EASY)
            while ((lowest_row(col)==-1)||(col == -1))
            {
                col = rand()%(paramrters.COLS);
            }
        }
        if ((mode)||((!mode)&&(player == 'X')))
        {
            printf(GREEN"# HINT: You Should Play in Column %d"WHITE,col+1);
        }
        else
        {
            moves[moves_counter] = col;
            board[lowest_row(col)][col] = player;
            moves_counter++;
            moves_counter2 = moves_counter;
        }
    }
}
#endif