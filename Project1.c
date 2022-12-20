#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
//testing git
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
char header[] = "ABCDEFGHIJKLMNOPQTVWXYabcdefghijklmnopqtvwxy0123456789";
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
    for (int i = 0; i < COLS; i++)
    {
        printf("| %c ",header[i]);
    }
    printf("|\n");
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
            //Change Color
            if (board[i][j]=='X')
            {
                printf("\033[0;31m");
            }
            else if (board[i][j]=='O')
            {
                printf("\033[0;33m");
            }
            printf(" %c ",board[i][j]);
            //Back White
            printf("\033[0;37m");
        }
        printf("|\n");
    }
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
int case_To_num(char Case)
{
    int col;
    if (COLS > 22)
    {
        switch (Case)
        {
            case 'A': col = 0; break;   case 'a': col = 22; break;
            case 'B': col = 1; break;   case 'b': col = 23; break;
            case 'C': col = 2; break;   case 'c': col = 24; break;
            case 'D': col = 3; break;   case 'd': col = 25; break;
            case 'E': col = 4; break;   case 'e': col = 26; break;
            case 'F': col = 5; break;   case 'f': col = 27; break;
            case 'G': col = 6; break;   case 'g': col = 28; break;
            case 'H': col = 7; break;   case 'h': col = 29; break;
            case 'I': col = 8; break;   case 'i': col = 30; break;
            case 'J': col = 9; break;   case 'j': col = 31; break;
            case 'K': col = 10; break;   case 'k': col = 32; break;
            case 'L': col = 11; break;   case 'l': col = 33; break;
            case 'M': col = 12; break;   case 'm': col = 34; break;
            case 'N': col = 13; break;   case 'n': col = 35; break;
            case 'O': col = 14; break;   case 'o': col = 36; break;
            case 'P': col = 15; break;   case 'p': col = 37; break;
            case 'Q': col = 16; break;   case 'q': col = 38; break;
            case 'T': col = 17; break;   case 't': col = 39; break;
            case 'V': col = 18; break;   case 'v': col = 40; break;
            case 'W': col = 19; break;   case 'w': col = 41; break;
            case 'X': col = 20; break;   case 'x': col = 42; break;
            case 'Y': col = 21; break;   case 'y': col = 43; break;
            
            case '0': col = 43; break;
            case '1': col = 44; break;
            case '2': col = 45; break;
            case '3': col = 46; break;
            case '4': col = 47; break;
            case '5': col = 48; break;
            case '6': col = 49; break;
            case '7': col = 50; break;
            case '8': col = 51; break;
            case '9': col = 52; break;

            case 'U': case 'u': col = -5; break;
            case 'R': case 'r': col = -10; break;
            default:
                col = 100;
                break;
        }
    }
    else
    {
        switch (Case)
        {
            case 'A': case 'a': col = 0; break;
            case 'B': case 'b': col = 1; break;
            case 'C': case 'c': col = 2; break;
            case 'D': case 'd': col = 3; break;
            case 'E': case 'e': col = 4; break;
            case 'F': case 'f': col = 5; break;
            case 'G': case 'g': col = 6; break;
            case 'H': case 'h': col = 7; break;
            case 'I': case 'i': col = 8; break;
            case 'J': case 'j': col = 9; break;
            case 'K': case 'k': col = 10; break;
            case 'L': case 'l': col = 11; break;
            case 'M': case 'm': col = 12; break;
            case 'N': case 'n': col = 13; break;
            case 'O': case 'o': col = 14; break;
            case 'P': case 'p': col = 15; break;
            case 'Q': case 'q': col = 16; break;
            case 'T': case 't': col = 17; break;
            case 'V': case 'v': col = 18; break;
            case 'W': case 'w': col = 19; break;
            case 'X': case 'x': col = 20; break;
            case 'Y': case 'y': col = 21; break;
            case 'U': case 'u': col = -5; break;
            case 'R': case 'r': col = -10; break;
            default:
                col = 100;
                break;
        }
    }
    return col;
}
int take_column(char c)
{
    scanf(" %c",&c);
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
    printf("For Human Vs Computer :Enter A\nFor Human Vs Human :Enter B\n");
    char g_mode;
    scanf(" %c",&g_mode);
    switch (g_mode)
    {
        case 'A':
        case 'a':
            mode = 0;
            break;
        case 'B':
        case 'b':
            mode = 1;
            break;
    
    default:
        printf("Error Try Again\n");
        game_mode();
        break;
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
{ char Case;
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
         int col = take_column(Case);
        while (((col>=0&&col<COLS)&&(board[0][col]!=' '))||(col>=COLS))
        {
            print_board();
            printf("Error Enter a valid column: ");
            col = take_column(Case);
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
                printf("\nNo Redo this the last move\n");
                choose(mode);
            }
        }
        else if (col<COLS)
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
            board[i][j]='X';
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