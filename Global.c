#include <time.h>
#define MAX_LEN 10000
typedef struct
{
    int number;
    char symbol;
    int score;
    char name[100];
}Player;
Player player1,player2,tempPlayer,winner,TopPlayers[MAX_LEN];
typedef struct
{
    int hours;
    int minutes;
    int seconds;
}Time;
Time timer;
time_t time_start,time_end;
typedef struct
{
    int rows;
    int cols;
    int moves_num;
    int moves[MAX_LEN];
}games;
games savegame, savedboards[10];
typedef struct
{
    int ROWS;
    int COLS;
    int HIGHSCORES;
}Configurations;
Configurations paramrters;
char board[MAX_LEN][MAX_LEN];
char xml_content[MAX_LEN];
int moves[MAX_LEN*MAX_LEN];
int moves_counter = 0;
int moves_counter2 = 0;
int scores_counter;