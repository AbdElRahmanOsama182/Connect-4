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
int ROWS, COLS, HIGHSCORES;
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
int word_in_text(char word[],char text[])
{
    int found = -1;
    int i = 0;
    while (text[i]!='\0')
    {
        int j = 0;
        if (text[i]==word[j])
        {
            while ((text[i+j] == word[j]) && (word[j] != '\0'))
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
    char config_content[MAX_LEN];
    char configurations_s[]="<Configurations>"; char configurations_e[]="</Configurations>";
    char height_s[]="<Height>"; char height_e[]="</Height>";
    char width_s[]="<Width>"; char width_e[]="</Width>";
    char highscores_s[]="<Highscores>"; char highscores_e[]="</Highscores>";
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
    int config_content_S = word_in_text(configurations_s,xml_content);
    int config_content_E = word_in_text(configurations_e,xml_content);
    if ((config_content_S != -1)&&(config_content_E != -1))
    {
        for (int i = config_content_S; i < config_content_E; i++)
        {
            config_content[i] = xml_content[i];
        }
    }
    ROWS = bring_parameter(height_s, height_e,config_content);
    COLS = bring_parameter(width_s, width_e,config_content);
    HIGHSCORES = bring_parameter(highscores_s, highscores_e,config_content);
}
void XML()
{
    FILE* xml_file= fopen("parameters.xml", "r");
    int tries = 3; char config_content[MAX_LEN];
    if (!(xml_file == NULL))
    {
        ReadXML(xml_file);
        fclose(xml_file);
    }
    while (((xml_file == NULL) || (ROWS <= 0) || (COLS <= 0) || (HIGHSCORES <= 0)) && (tries)) 
    {
        printf(RED"File error, Please Enter the correct file path with a valid parameters: "WHITE);
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
        tries--;
    }
    if (((xml_file == NULL) || (ROWS <= 0) || (COLS <= 0) || (HIGHSCORES <= 0)) && (tries == 0))
    {
        ROWS = 9;
        COLS = 7;
        HIGHSCORES = 10;
        printf(RED"Failed 3 times!\nLoading the defaults....\n");
        printf(GREEN"Height = 9\nWidth = 7\nHighscores = 10\n"WHITE);
    }
}
//Sorting the Top Players list descending by MergeSort
void merge(Player arr[],int start,int mid,int end)
{
    int i, s , j, k; Player temp[MAX_LEN];
    s=start;
    j=mid+1;
    i=start;

    while((s<=mid)&&(j<=end))
    {
        if(arr[s].score>=arr[j].score)
        {
            temp[i]=arr[s];
            s++;
        }
        else
        {
            temp[i]=arr[j];
            j++;
        }
        i++;
    }
    if(s>mid)
    {
        for(k=j; k<=end; k++)
        {
            temp[i]=arr[k];
            i++;
        }
    }
    else
    {
        for(k=s; k<=mid; k++)
        {
            temp[i]=arr[k];
            i++;
        }
    }
    for(k=start; k<=end; k++)
    {
        arr[k]=temp[k];
    }
}
void mergeSort(Player arr[], int start, int end)
{
    int mid;
    if(start<end)
    {
    mid = (start+end)/2;
    mergeSort(arr, start, mid);
    mergeSort(arr, mid+1, end);
    merge(arr, start, mid, end);
    }
}
int scores_counter = 0;
void addWinnerTofile()
{
    FILE * scores_file = fopen("hScores.txt", "ab");
    fwrite(&winner,sizeof(winner),1,scores_file);
    fclose(scores_file);
}
void read_scores()
{
    scores_counter = 0;
    FILE * scores_file = fopen("hScores.txt", "rb");
    if (scores_file == NULL)
    {
        printf(RED"NO Highscores yet\n"WHITE);
    }
    else
    {
        while (!(feof(scores_file)))
        {
            fread(&TopPlayers[scores_counter],sizeof(TopPlayers[scores_counter]),1,scores_file);
            scores_counter++;
        }
        fclose(scores_file);
        mergeSort(TopPlayers,0,scores_counter-1);
    }
}
void removeDublicates()
{
    //If the player has two scores the lower will be zero
    for (int i = 0; i < scores_counter-1; i++)
    {
        for (int j = i+1; j < scores_counter-1; j++)
        {
            if (strcasecmp(TopPlayers[i].name,TopPlayers[j].name)==0)
            {
                TopPlayers[j].score = 0;
            }
        }
    }
    mergeSort(TopPlayers,0,scores_counter-1);
}
int get_rank()
{
    removeDublicates();
    int rank;
    for (int i = 0; i < scores_counter-1; i++)
    {
        if (strcasecmp(winner.name,TopPlayers[i].name)==0)
        {
            rank = i + 1;
            break;
        }
    }
    return rank;
}
void print_TopPlayers()
{
    int size = scores_counter-1;
    removeDublicates();
    if (HIGHSCORES < scores_counter-1)
    {
        size = HIGHSCORES;
    }
    printf("  Player\tScore\n");
    for (int i = 0; i < size; i++)
    {
        if (TopPlayers[i].symbol=='X')
        {
            printf(RED);
        }
        else
        {
            printf(YELLOW);
        }
        if (TopPlayers[i].score)
        {
            printf("%d. %s (%c) %d\n", i+1, TopPlayers[i].name, TopPlayers[i].symbol, TopPlayers[i].score);
        }
    }
    printf(WHITE);
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
void NewBoard()
{
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            board[i][j]=' ';
        }
    }
}
void Save ()
{
    savegame.moves_num = moves_counter;
    for (int i = 0; i < moves_counter; i++)
    {
        savegame.moves[i]=moves[i];
    }
    savegame.rows = ROWS;
    savegame.cols = COLS;
    
    FILE * savedGames = fopen("SavedGames.bin","ab");
    fwrite(&savegame,sizeof(savegame),1,savedGames);
    fclose(savedGames);
}
void Load()
{
    FILE * savedGames = fopen("SavedGames.bin","rb");
    if (savedGames==NULL)
    {
        printf(RED"No Saved Games starting a New One\n"WHITE);
        NewBoard();
    }
    else
    {
        int boards_counter = 0;
        while (!feof(savedGames))
        {
            fread(&savedboards[boards_counter],sizeof(savedboards[boards_counter]),1,savedGames);
            boards_counter++;
        }
        fclose(savedGames);
        //if the number of games is more than 5 clear the old ones
        if (boards_counter > 6)
        {
            int s = boards_counter-1;
            FILE * savedGames = fopen("SavedGames.bin","wb");
            for (int i = s-5; i < s; i++)
            {
                fwrite(&savedboards[i],sizeof(savedboards[i]),1,savedGames);
            }
            fclose(savedGames);
        }
        
        printf(GREEN"Choose Game number from 1 to %d: "WHITE,boards_counter-1);
        char game_num[MAX_LEN];
        int gameNum;
        fgets(game_num, MAX_LEN, stdin);
        if ((atoi(game_num) > 0) && (atoi(game_num) < boards_counter))
        {
            gameNum = atoi(game_num)-1;
        }
        else
        {
            while (!((atoi(game_num) > 0) && (atoi(game_num) < boards_counter)))
            {
                printf(RED"Enter valid game number: ");
                fgets(game_num, MAX_LEN, stdin);
            }
            printf(WHITE);
            gameNum = atoi(game_num)-1;
        }
        moves_counter = savedboards[gameNum].moves_num;
        for (int i = 0; i < moves_counter; i++)
        {
            moves[i] = savedboards[gameNum].moves[i];
        }
        ROWS = savedboards[gameNum].rows;
        COLS = savedboards[gameNum].cols;
        NewBoard();
        for (int i = 0; i < moves_counter; i++)
        {
            if ( i % 2 == 0)
            {
                board[lowest_row(moves[i])][moves[i]] = 'X';
            }
            else
            {
                board[lowest_row(moves[i])][moves[i]] = 'O';
            }
        }
    }
}
void print_board()
{
    printf(BLUE);
    if (COLS<10)
    {
        printf(" ");
        for (int i = 1; i <= COLS; i++)
        {
            printf(" %d  ",i);
        }
        printf("\n");
    }
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
int Score(int base, char player)
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
                score += countns(base,count);
                count = 0;
            }
            if (j == COLS-1)
            {
                score += countns(base,count);
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
                score += countns(base,count);
                count = 0;
            }
            if (j == ROWS-1)
            {
                score += countns(base,count);
            }
        }
    }
    //Diagonal '\' Left to Right
    for (int s = -COLS+1; s < ROWS; s++)
    {
        int count = 0;
        for (int i = 0; i < ROWS; i++)
        {
            if ((i+s >=COLS)||(i<0))
            {
                score += countns(base,count);
                count = 0;
                continue;
            }
            if (board[i][i+s] == player)
            {
                count++;
            }
            else if (i < ROWS-1)
            {
                score += countns(base,count);
                count = 0;
            }
            if (i == ROWS-1)
            {
                score += countns(base,count);
            }
        }
    }
    //Diagonal '/' Right to Left
    for (int s = 0; s < ROWS+COLS-1; s++)
    {
        int count = 0;
        for (int i = 0; i < ROWS; i++)
        {
            if ((s-i >= COLS)||(s-i < 0))
            {
                score += countns(base,count);
                count = 0;
                continue;
            }
            if (board[i][s-i] == player)
            {
                count++;
            }
            else if (i < ROWS-1)
            {
                score += countns(base,count);
                count = 0;
            }
            if (i == ROWS-1)
            {
                score += countns(base,count);
            }
        }
    }
    return score;
}
int ScoreAtPoint(int base, char player,int col)
{
    board[lowest_row(col)][col] = player;
    int score = Score(base, player);
    board[lowest_row(col)+1][col] = ' ';
    return score;
}
int ScoreX_At2Points(int base, int col1)
{
    int score;
    if (lowest_row(col1)>0)
    {
        board[lowest_row(col1)][col1] = 'O';
        board[lowest_row(col1)][col1] = 'X';
        score = Score(base, 'X');
        board[lowest_row(col1)+1][col1] = ' ';
        board[lowest_row(col1)+1][col1] = ' ';
    }
    else
    {
        score = Score(base, 'X');
    }
    
    return score;
}
void print_Fscore(int mode)
{
    winner.score = 0;
    player1.score = Score(4,'X');
    player2.score = Score(4,'O');
    printf(RED"\nPlayer1's Final Score : %d\n"WHITE,player1.score);
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
        addWinnerTofile();
        read_scores();
        int rank = get_rank();
        printf(GREEN"your rank is %d\n"WHITE,rank);
        if (rank <= HIGHSCORES)
        {
            print_TopPlayers();
        }
        
    }
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
    else if ((c[0]=='s')||(c[0]=='S'))
    {
        col = -15;
    }
    else if ((c[0]=='e')||(c[0]=='E'))
    {
        col = -20;
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
    printf(GREEN"For Human Vs Computer :Enter 1\nFor Human Vs Human :Enter 2\n");
    char g_mode[MAX_LEN];
    fgets(g_mode,sizeof(g_mode),stdin);
    if (atoi(g_mode)==1)
    {
        printf("Human Vs Computer\n"WHITE);
        mode = 0;
    }
    else if (atoi(g_mode)==2)
    {
        printf("Human Vs Human\n"WHITE);
        mode = 1;
    }
    else
    {
        printf(RED"Error Try Again\n");
        game_mode();
    }
    return mode;
}
int game_level()
{
    int level;
    printf(GREEN"Easy Mode :Enter 1\nMedium Mode :Enter 2\nHard Mode :Enter 3\n");
    char g_level[MAX_LEN];
    fgets(g_level,sizeof(g_level),stdin);
    if (atoi(g_level)==1)
    {
        printf("Easy Mode\n"WHITE);
        level = 0;
    }
    else if (atoi(g_level)==2)
    {
        printf("Medium Mode\n"WHITE);
        level = 1;
    }
    else if (atoi(g_level)==3)
    {
        printf("Hard Mode\n"WHITE);
        level = 2;
    }
    else
    {
        printf(RED"Error Try Again\n");
        game_level();
    }
    return level;
}
void Computer_Play(int level)
{
    if (not_full())
    {
        int col = -1;
        if (level == 2)
        {
            for (int n = 4; n > 1; n--)
            {
                int scoreX1 = Score(n,'X');
                int scoreX2 = Score(n,'X');
                int scoreO = Score(n,'O');
                for (int i = 0; i < COLS; i++)
                {
                    if (lowest_row(i)!=-1)
                    {
                        if (((ScoreAtPoint(n,'X',i)+ScoreAtPoint(n,'O',i))>(scoreX1+scoreO))&&(scoreX2==ScoreX_At2Points(n,i)))
                        {
                        col = i; scoreX1 = ScoreAtPoint(n,'X',i); scoreO = ScoreAtPoint(n,'O',i);
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
            for (int n = 4; n > 1; n--)
            {
                int scoreX1 = Score(n,'X');
                int scoreO = Score(n,'O');
                for (int i = 0; i < COLS; i++)
                {
                    if (lowest_row(i)!=-1)
                    {
                        if (((ScoreAtPoint(n,'X',i)+ScoreAtPoint(n,'O',i))>(scoreX1+scoreO)))
                        {
                        col = i; scoreX1 = ScoreAtPoint(n,'X',i); scoreO = ScoreAtPoint(n,'O',i);
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
            while ((lowest_row(col)==-1)||(col == -1))
            {
                col = rand()%(COLS);
            }
        }
        moves[moves_counter] = col;
        board[lowest_row(col)][col] = 'O';
        moves_counter++;
        moves_counter2 = moves_counter;
    }
}
int choose(int mode,int level)
{
    char choice[MAX_LEN];
    print_board();
    if (moves_counter%2==0)
    {
        tempPlayer = player1;
        printf(RED);
    }
    else
    {
        tempPlayer = player2;
        printf(YELLOW);
    }
    printf("Player %d,Turn: "WHITE,tempPlayer.number );
    if (( mode )||( moves_counter%2==0 ))
    {
        int col = take_column(choice);
        while ((((col > -1) && (col < COLS)) && (board[0][col]!=' '))||(col>=COLS)||(col==-1))
        {
            printf(RED"Error Enter a valid column: "WHITE);
            col = take_column(choice);
        }
        if (col==-5)
        {
            if (moves_counter == 0)
            {
                printf(RED"\nThis is the first move No moves to Undo\n"WHITE);
                choose(mode,level);
            }
            else
            {
                Undo(mode);
            }
        }
        else if (col==-10)
        {
            if (moves_counter<moves_counter2)
            {
                Redo(mode);
            }
            else
            {
                printf(RED"\nNo Redo this the last move\n"WHITE);
                choose(mode,level);
            }
        }
        else if (col==-15)
        {
            Save();
            printf(GREEN"Game Saved\n"WHITE);
        }
        else if (col < COLS && col >= 0 && (board[0][col]==' '))
        {
            board[lowest_row(col)][col] = tempPlayer.symbol;
            moves[moves_counter] = col;
            moves_counter++;moves_counter2 = moves_counter;
        }
        return col;
    }
    else
    {
        Computer_Play(level);
        return 0;
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
void print_game_data()
{
    printf(GREEN"\n%d:%d:%d%s", timer.hours, timer.minutes, timer.seconds);
    printf(RED"\nPlayer1 score: %d\t"YELLOW"Player2 score: %d\n", Score(4,'X'), Score(4,'O'));
    printf(RED"Player1 moves: %d\t"YELLOW"Player2 moves: %d\n"WHITE, Moves('X'), Moves('O'));
}
int MainMenu()
{
    printf(GREEN"Start New Game: 1\n");
    printf("Load Game: 2\n");
    printf("Top Players: 3\n");
    printf("Quit: 4\n");
    char option[MAX_LEN];
    fgets(option, MAX_LEN, stdin);
    if ((atoi(option) < 5) && (atoi(option)))
    {
        printf(WHITE);
        return atoi(option);
    }
    else
    {
        printf(RED"Error enter a valid option!\n");
        return MainMenu();
    }
}
int BackToMain()
{
    printf(GREEN"Back to Main Menu: 1\n");
    printf("Quit: 2\n");
    char option[MAX_LEN];
    fgets(option, MAX_LEN, stdin);
    if (atoi(option) == 1)
    {
        printf(WHITE);
        MainMenu();
    }
    else if ((atoi(option) == 0)||(atoi(option) > 2))
    {
        printf(RED"Error enter a valid option!\n");
        return BackToMain();
    }
    else
    {
        return 4;
    }
}
void Play(int option)
{
    if (option < 4)
    {
        XML();
        if (option < 3)
        {
            if (option == 1)
                NewBoard();
            else
                Load();
            int mode = game_mode();
            int level = 0;
            if (!mode)
            {
                level = game_level();
            }
            time(&time_start);
            int end_game = 0;
            while (not_full())
            {
                system(" ");
                printf(PURPLE"\nChoose Number Of The Column Or\n");
                printf("( 'U' for Undo / 'R' for Redo / 'S' for Save / 'E' for Exit )\n"WHITE);
                end_game = choose(mode,level);
                timer = Timer();
                print_game_data();
                if(end_game ==-20)
                {
                    printf(GREEN"Ending Game\n");
                    break;
                }
            }
            if (end_game != -20)
            {
                print_board();
                print_Fscore(mode);
            }
        }
        else
        {
            read_scores();
            print_TopPlayers();
        }
    moves_counter = moves_counter2 = 0;
    return Play(BackToMain());
    }
    printf(WHITE);
}
int main() {
    //Player1                  //Player2
    player1.number = 1 ;       player2.number = 2;
    player1.symbol = 'X';      player2.symbol = 'O';
    tempPlayer = player1;
    Play(MainMenu());
    return 0;
}