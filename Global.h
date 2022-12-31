#ifndef GLOBAL
#define GLOBAL
//All The Libraries, CONSTS And Global Variables
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <ctype.h>
#include <conio.h>
#define RED "\033[0;31m"
#define BLUE "\033[0;34m"
#define YELLOW "\033[0;33m"
#define GREEN "\033[0;32m"
#define WHITE "\033[0;37m"
#define PURPLE "\033[0;35m"
#define MAX_LEN 10000
typedef struct
{
    int number;
    char symbol;
    int score;
    char name[100];
}Player;
typedef struct
{
    int minutes;
    int seconds;
}Time;
typedef struct
{
    int rows;
    int cols;
    int moves_num;
    char name[100];
    int moves[MAX_LEN];
}games;
typedef struct
{
    int ROWS;
    int COLS;
    int HIGHSCORES;
}Configurations;
extern int moves_counter, moves_counter2, moves[MAX_LEN*MAX_LEN], scores_counter;
extern char board[MAX_LEN][MAX_LEN], xml_content[MAX_LEN];
extern Player player1,player2,tempPlayer,winner,TopPlayers[MAX_LEN];
extern Time timer;
extern time_t time_start,time_end;
extern games savegame, savedboards[10];
extern Configurations paramrters;
#endif