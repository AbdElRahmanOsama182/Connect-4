#ifndef SaveLoad
#define SaveLoad
#include "Global.h"
//Initializing An Empty Board
void NewBoard()
{
    for (int i = 0; i < paramrters.ROWS; i++)
    {
        for (int j = 0; j < paramrters.COLS; j++)
        {
            board[i][j]=' ';
        }
    }
}
//Saving The Current Game Board
void Save()
{
    savegame.moves_num = moves_counter;
    for (int i = 0; i < moves_counter; i++)
    {
        savegame.moves[i]=moves[i];
    }
    savegame.rows = paramrters.ROWS;
    savegame.cols = paramrters.COLS;
    //Read The Games in The File & If The Game Name Already Exists Update The Game
    //If Not Add The Game To The Array
    FILE * savedGames = fopen("SavedGames.bin","rb");
    int boards_counter = 0;
    if (savedGames!=NULL)
    {
        while (!feof(savedGames))
        {
            fread(&savedboards[boards_counter],sizeof(savedboards[boards_counter]),1,savedGames);
            boards_counter++;
        }
    }
    fclose(savedGames);
    int found = 0;
    for (int i = 0; i < boards_counter-1; i++)
    {
        if (strcasecmp(savegame.name,savedboards[i].name)==0)
        {
            found = 1;
            savedboards[i] = savegame;
        }
    }
    if (found)
    {
        boards_counter--;
    }
    else
    {
        if (boards_counter)
        {
            savedboards[boards_counter-1] = savegame;
        }
        else
        {
            savedboards[0] = savegame;
            boards_counter++;
        }
    }
    //Writing The Games In The File And If It's More Than 5 Write The Newest Ones
    savedGames = fopen("SavedGames.bin","wb");
    int s = 0;
    if (boards_counter > 5)
    {
        s = boards_counter - 5;
    }
    for (int i = s; i < boards_counter; i++)
    {
        fwrite(&savedboards[i],sizeof(savedboards[i]),1,savedGames);
    }
    fclose(savedGames);
}
//Loading Games From The File & Choosing It By Its Name
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
        printf(GREEN"  Games Names\n");
        for (int i = 0; i < boards_counter-1; i++)
        {
            printf("\n%d. %s\n",i+1,savedboards[i].name);
        }
        printf(GREEN"Enter Game number : "WHITE);
        char game_num[MAX_LEN];
        int gameNum;
        fgets(game_num, MAX_LEN, stdin);
        if ((strToint(game_num) > 0) && (strToint(game_num) < boards_counter))
        {
            gameNum = strToint(game_num)-1;
        }
        else
        {
            while (!((strToint(game_num) > 0) && (strToint(game_num) < boards_counter)))
            {
                printf(RED"Enter valid game number: ");
                fgets(game_num, MAX_LEN, stdin);
            }
            printf(WHITE);
            gameNum = strToint(game_num)-1;
        }
        moves_counter = savedboards[gameNum].moves_num;
        moves_counter2 = moves_counter;
        for (int i = 0; i < moves_counter; i++)
        {
            moves[i] = savedboards[gameNum].moves[i];
        }
        paramrters.ROWS = savedboards[gameNum].rows;
        paramrters.COLS = savedboards[gameNum].cols;
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
#endif