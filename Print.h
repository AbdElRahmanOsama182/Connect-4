#ifndef Print
#define Print
#include "Global.h"
//Printing Colored Board
void print_board()
{
    printf(BLUE);
    if (paramrters.COLS<10)
    {
        printf(" ");
        for (int i = 1; i <= paramrters.COLS; i++)
        {
            printf(" %d  ",i);
        }
        printf("\n");
    }
    for (int i = 0; i < paramrters.ROWS; i++)
    {
        for (int j = 0; j < paramrters.COLS; j++)
        {
            printf("|---");
        }
        printf("|\n");
        for (int j = 0; j < paramrters.COLS; j++)
        {
            printf("|");
            if (board[i][j]=='X')
            {
                printf(RED);
            }
            else if (board[i][j]=='O')
            {
                printf(YELLOW);
            }
            printf(" %c ",board[i][j]);
            printf(BLUE);
        }
        printf("|\n");
    }
    printf(WHITE);
}
//Printing Final Scores & Taking The Winner Name
void print_Fscore(int mode)
{
    winner.score = 0;
    player1.score = Score(4,'X');
    player2.score = Score(4,'O');
    printf(RED"\nPlayer1's Final Score : %d\n"WHITE, player1.score);
    printf(YELLOW"Player2's Final Score : %d\n"WHITE, player2.score);
    if (Score(4,'X')>Score(4,'O'))
    {
        printf(RED"Player1 is the WINNER\n");
        winner = player1;
        printf(GREEN"Player1 Enter you name, please: "WHITE);
    }
    else if (Score(4,'X')<Score(4,'O'))
    {
        printf(YELLOW"Player2 is the WINNER\n"WHITE);
        if (mode)
        {
            winner = player2;
            printf(GREEN"Player2 Enter you name, please: "WHITE);
        }
    }
    else
        printf(RED"EV"YELLOW"EN\n"WHITE);
    if (winner.score)
    {
        gets(winner.name);
        fflush(stdin);
        while (winner.name[0]=='\0')
        {
            printf(GREEN"Enter you name, please: "WHITE);
            gets(winner.name);
            fflush(stdin);
        }
        read_scores();
        addWinnerToList();
        addWinnerTofile();
        int rank = get_rank();
        printf(GREEN"your rank is %d\n"WHITE,rank);
        if (rank <= paramrters.HIGHSCORES)
        {
            print_TopPlayers();
        }
    }
}
//Returning Player Moves
int Moves(char player)
{
    if (player=='X')
    {
        return moves_counter/2 + moves_counter%2;
    }
    else
    {
        return moves_counter/2;
    }
}
//Printing ( Time & Scores & Moves )
void print_game_data()
{
    printf(GREEN"\n%d:%d", timer.minutes, timer.seconds);
    printf(RED"\nPlayer1 score: %d\t"YELLOW"Player2 score: %d\n", Score(4,'X'), Score(4,'O'));
    printf(RED"Player1 moves: %d\t"YELLOW"Player2 moves: %d\n"WHITE, Moves('X'), Moves('O'));
}
#endif