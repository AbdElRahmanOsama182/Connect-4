#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <ctype.h>
#include <conio.h>
#define MAX_LEN 10000
int ROWS, COLS, HIGHSCORES;
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
int word_in_text(char word[],char text[])
{
    int found = -1;int i = 0;
    while (text[i]!='\0')
    {
        int j = 0;
        if (tolower(text[i])==word[j])
        {
            while ((tolower(text[i+j]) == word[j]) && (word[j] != '\0'))
            {
                j++;
            }
            if (word[j]=='\0')
            {
                found = i;
            }
        }
        i++;
    }
    return found;
}
int bring_parameter(char start[],char end[],char text[])
{
    int s = word_in_text(start,text)+strlen(start);
    int e = word_in_text(end,text);
    int parameter;
    if ((e == -1)||(s == -1)||(e-s == 0))
    {
        parameter = 0;
    }
    else
    {
        char temp[MAX_LEN];
        for (int i = 0; i < e-s; i++)
        {
            temp[i] = text[s+i];
        }
        parameter = atoi(temp);
        }
    return parameter;
}
char xml_content[MAX_LEN];
void ReadXML(FILE* xml_file)
{
    int i = 0;
    while (!feof(xml_file))
        {
            char temp = fgetc(xml_file);
            if ((temp!=' ')&&(temp!='\t')&&(temp!='\n'))
            {
                xml_content[i] = temp;
                i++;
            }
        }
}
void XML()
{
    char configurations_s[]="<configurations>"; char configurations_e[]="</configurations>";
    char height_s[]="<height>"; char height_e[]="</height>";
    char width_s[]="<width>"; char width_e[]="</width>";
    char highscores_s[]="<highscores>"; char highscores_e[]="</highscores>";
    FILE* xml_file= fopen("parameters.xml", "r");
    int tries = 3;
    if (!(xml_file == NULL))
    {
        ReadXML(xml_file);
        fclose(xml_file);
        ROWS = bring_parameter(height_s, height_e,xml_content);
        COLS = bring_parameter(width_s, width_e,xml_content);
        HIGHSCORES = bring_parameter(highscores_s, highscores_e,xml_content);
    }
    while (((xml_file == NULL) || (ROWS <= 0) || (COLS <= 0) || (HIGHSCORES <= 0)) && (tries)) 
    {
        printf("\033[0;31mFile error, Please Enter the correct file path with a valid parameters: \033[0;37m");
        char path[MAX_LEN];
        scanf("%s",path);
        getchar();
        xml_file = fopen(path, "r");
        if (xml_file == NULL)
        {
            tries--;
            continue;
        }
        ReadXML(xml_file);
        fclose(xml_file);
        ROWS = bring_parameter(height_s, height_e,xml_content);
        COLS = bring_parameter(width_s, width_e,xml_content);
        HIGHSCORES = bring_parameter(highscores_s, highscores_e,xml_content);
        printf("%d %d %d \n\n",ROWS,COLS,HIGHSCORES);
        tries--;
    }
    if (((xml_file == NULL) || (ROWS <= 0) || (COLS <= 0) || (HIGHSCORES <= 0)) && (tries == 0))
    {
        ROWS = 6;
        COLS = 7;
        HIGHSCORES = 10;
        printf("\033[0;31mFailed 3 times Loading the defaults\033[0;37m\n");
        printf("Height = 6\nWidth = 7\nHighscores = 10\n");
    }
}
char board[MAX_LEN][MAX_LEN];
int moves[MAX_LEN*MAX_LEN];
int moves_counter = 0;
int moves_counter2 = 0;
Time split_time( long int s)
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
/////////////////////////////////
//////////////////////////////////
void save ()
{
    FILE*fp;

    fp=fopen("save.txt","wb");

    //board[ROWS][COLS]
    fwrite(board,sizeof(board),1,fp);
    fclose(fp);
}





//load from binary file
void load()
{
    FILE*fp;

    fp=fopen("save.txt","rb");
    if (fp==NULL)    //check if the file is available
    {
        //printf("can't find and open file");
        for (int i = 0; i < ROWS; i++)
        {
            for (int j = 0; j < COLS; j++)
            {
                board[i][j]='^';
            }
        }

    }
    else{
            fread(board,sizeof(board),1,fp);
    fclose(fp);
    }

}




//////////////////////////////////
/////////////////////////////////
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
int not_full()
{
    int full = 1;
    for (int i = 0; i < COLS; i++)
    {
        if (board[0][i] == '^')
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
        if (board[i][col]=='^')
        {
            row = i;
            break;
        }
    }
    return row;
}
int case_To_num(char c[])   //turn the char that user enter to number like redo,undo
{
    int col;
    if (atoi(c))    //fun that if the array of string is number return int, if it is string return 0
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
    board[lowest_row(moves[moves_counter-1])+1][moves[moves_counter-1]] = '^';
    moves_counter--;
    if (!mode)
    {
        board[lowest_row(moves[moves_counter-1])+1][moves[moves_counter-1]] = '^';
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
        moves_counter++;
        moves_counter2 = moves_counter;
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
            while ((((col > -1) && (col < COLS)) && (board[0][col]!='^'))||(col>=COLS)||(col==-1))
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
        else if (col < COLS && col >= 0 && (board[0][col]=='^'))
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

int main() {
    XML();
    //load();
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            board[i][j]='^';
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
        system(" ");
        choose(mode);
        timer = Timer();
        printf("\n\033[0;32m%d:%d:%d\033[0;37m",timer.hours,timer.minutes,timer.seconds);
        printf("\n\033[0;31mPlayer1 score: %d\t\033[0;33mPlayer2 score: %d\033[0;37m\n",Score('X'),Score('O'));
        printf("\033[0;31mPlayer1 moves: %d\t\033[0;33mPlayer2 moves: %d\033[0;37m\n\n",Moves('X'),Moves('O'));
        tempPlayer = (tempPlayer.number == 1) ? player2 : player1;
        if(tempPlayer.number==1){
            save();
        }
    }
    print_board();
    print_score();

    return 0;
}
