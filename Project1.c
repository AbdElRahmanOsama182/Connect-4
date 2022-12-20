#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <ctype.h>
#include<conio.h>
#define MAX_LEN 1000
#define ROWS 6
#define COLS 7
typedef struct 
{
    int number;
    int score;
    char symbol;
    char name[];
}Player;
Player player1,player2,tempPlayer;
typedef struct
{
    int hours;
    int minutes;
    int seconds;
}Time;
Time timer;
time_t time_start,time_end;
char board[ROWS][COLS];
int moves[ROWS*COLS];
int moves_counter = 0;
int moves_counter2 = 0;
Time split_time(long long int s)
{
    Time result;
    result.hours = (s/3600)%24;
    result.minutes = (s%3600)/60;
    result.seconds = (s%3600)%60;
    return result;
}
Time Timer()
{
    time(&time_end);
    Time result = split_time(difftime(time_end,time_start));
    return result;
}
void print_board()
{
    printf("\033[0;34m");
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            printf("|---");
        }
        printf("|\n");
        for (int j = 0; j < COLS; j++)
        {
            printf("|");
            //Change Color according to the Player
            if (board[i][j]=='X')
            {
                printf("\033[0;31m");
            }
            else if (board[i][j]=='O')
            {
                printf("\033[0;33m");
            }
            printf(" %c ",board[i][j]);
            printf("\033[0;34m");
        }
        printf("|\n");
    }
    //Back White
    printf("\033[0;37m");
}
int count4s(int n)
{
    if (n >= 4)
    {
        return n-3;
    }
    else
    {
        return 0;
    }
}
int Score(char player)
{
    
    int score = 0;
    //Horizontal
    for (int i = 0; i < ROWS; i++)
    {
        int count = 0;
        for (int j = 0; j < COLS; j++)
        {
            if (board[i][j] == player)
            {
                count++;
            }
            else if (j < COLS-1)
            {
                score += count4s(count);
                count = 0;
            }
            if (j == COLS-1)
            {
                score += count4s(count);
            }
            
        }
    }
    //Vertical
    for (int i = 0; i < COLS; i++)
    {
        int count = 0;
        for (int j = 0; j < ROWS; j++)
        {
            if (board[j][i] == player)
            {
                count++;
            }
            else if (j < ROWS-1)
            {
                score += count4s(count);
                count = 0;
            }
            if (j == ROWS-1)
            {
                score += count4s(count);
            }
        }
    }
    //Diagonal '\' Left to Right
    for (int s = 0; s < COLS-3; s++)
    {
        int count = 0;
        for (int i = 0; i < ROWS; i++)
        {
            if (i+s >=COLS)
            {
                score += count4s(count);
                break;
            }
            if (board[i][i+s] == player)
            {
                count++;
            }
            else if (i < ROWS-1)
            {
                score += count4s(count);
                count = 0;
            }
            if (i == ROWS-1)
            {
                score += count4s(count);
            }
        }
    }
    for (int s = 1; s < ROWS-3; s++)
    {
        int count = 0;
        for (int i = 0; i < COLS; i++)
        {
            if (i+s >= ROWS)
            {
                score += count4s(count);
                break;
            }
            if (board[i+s][i] == player)
            {
                count++;
            }
            else if (i < COLS-1)
            {
                score += count4s(count);
                count = 0;
            }
            if (i == COLS-1)
            {
                score += count4s(count);
            }
        }
    }
    //Diagonal '/' Right to Left
    for (int s = COLS-1; s > 2; s--)
    {
        int count = 0;
        for (int i = 0; i < ROWS; i++)
        {
            if (s-i < 0)
            {
                score += count4s(count);
                break;
            }
            
            if (board[i][s-i] == player)
            {
                count++;
            }
            else if (i < ROWS-1)
            {
                score += count4s(count);
                count = 0;
            }
            if (i == ROWS-1)
            {
                score += count4s(count);
            }
        }
    }
    for (int s = 1; s < ROWS-3; s++)
    {
        int count = 0;
        for (int i = 0; i < COLS; i++)
        {
            if (s+i >= ROWS)
            {
                score += count4s(count);
                break;
            }
            
            if (board[s+i][COLS-1-i] == player)
            {
                count++;
            }
            else if (i < COLS-1)
            {
                score += count4s(count);
                count = 0;
            }
            if (i == COLS-1)
            {
                score += count4s(count);
            }
        }
    }
    return score;
}
int not_full()
{
    int full = 1;
    for (int i = 0; i < COLS; i++)
    {
        if (board[0][i] == ' ')
        {
            full = 0;
            break;
        }
    }
    return !full;
}
int lowest_row(int col)
{
    int row = -1;
    for (int i = ROWS-1; i >= 0; i--)
    {
        if (board[i][col]==' ')
        {
            row = i;
            break;
        }
    }
    return row;
}
int case_To_num(char c[])
{
    int col;
    if (atoi(c))
    {
        col = atoi(c)-1;
    }
    else if ((c[0]=='u')||(c[0]=='U'))
    {
        col = -5;
    }
    else if ((c[0]=='r')||(c[0]=='R'))
    {
        col = -10;
    }
    else
    {
        col = -1;
    }
    return col;
}
int take_column(char c[])
{
    fgets(c, MAX_LEN, stdin);
    return case_To_num(c);
}
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
int game_mode()
{
    int mode;
    printf("For Human Vs Computer :Enter 1\nFor Human Vs Human :Enter 2\n");
    char g_mode[MAX_LEN];
    fgets(g_mode,sizeof(g_mode),stdin);
    if (atoi(g_mode)==1)
    {
        printf("Human Vs Computer\n");
        mode = 0;
    }
    else if (atoi(g_mode)==2)
    {
        printf("Human Vs Human\n");
        mode = 1;
    }
    else
    {
        printf("\033[0;31mError Try Again\033[0;37m\nn");
        game_mode();
    }
    return mode;
}
void Computer_Play()
{
    if (not_full())
    {
        int col = rand()%(COLS-1);
        moves[moves_counter] = col;
        while (lowest_row(col)==-1)
        {
            col = rand()%(COLS-1);
        }
        board[lowest_row(col)][col] = 'O';
        moves_counter++;moves_counter2 = moves_counter;
    }
}
void choose(int mode)
{ char choice[MAX_LEN];
    print_board();
    if (tempPlayer.number==1)
    {
        printf("\033[0;31m");
    }
    else
    {
        printf("\033[0;33m");
    }
    
    printf("Player %d,Turn: \033[0;37m",tempPlayer.number);
    if ((mode)||(moves_counter%2==0))
    {
        int col = take_column(choice);
        while ((((col > -1) && (col < COLS)) && (board[0][col]!=' '))||(col>=COLS)||(col==-1))
        {
            print_board();
            printf("\033[0;31mError Enter a valid column:\033[0;37m ");
            col = take_column(choice);
        }
        if (col==-5)
        {
            Undo(mode);
        }
        else if (col==-10)
        {
            if (moves_counter<moves_counter2)
            {
                Redo(mode);
            }
            else
            {
                printf("\n\033[0;31mNo Redo this the last move\033[0;37m\n");
                choose(mode);
            }
        }
        else if (col < COLS && col >= 0 && (board[0][col]==' '))
        {
            board[lowest_row(col)][col] = tempPlayer.symbol;
            moves[moves_counter] = col;
            moves_counter++;moves_counter2 = moves_counter;
        }
    }
    else
    {
        Computer_Play();
    }
}
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
void print_score()
{
    printf("\n\033[0;31mPlayer1's Final Score : %d\033[0;37m\n",Score('X'));
    printf("\033[0;33mPlayer2's Final Score : %d\033[0;37m\n",Score('O'));
    if (Score('X')>Score('O'))
        printf("\033[0;31mPlayer1 is the WINNER\033[0;37m");
    else if (Score('X')<Score('O'))
        printf("\033[0;33mPlayer2 is the WINNER\033[0;37m");
    else
        printf("EVEN");
}
int main() {
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            board[i][j]=' ';
        }
    }
    //Player1                  //Player2
    player1.number = 1 ;       player2.number = 2;
    player1.symbol = 'X';      player2.symbol = 'O';
    tempPlayer = player1;
    int mode = game_mode();
    time(&time_start);
    while (not_full())
    {
        choose(mode);
        timer = Timer();
        printf("\n\033[0;32m%d:%d:%d\033[0;37m",timer.hours,timer.minutes,timer.seconds);
        printf("\n\033[0;31mPlayer1 score: %d\t\033[0;33mPlayer2 score: %d\033[0;37m\n",Score('X'),Score('O'));
        printf("\033[0;31mPlayer1 moves: %d\t\033[0;33mPlayer2 moves: %d\033[0;37m\n\n",Moves('X'),Moves('O'));
        tempPlayer = (tempPlayer.number == 1) ? player2 : player1;
    }
    print_board();
    print_score();
    return 0;
}