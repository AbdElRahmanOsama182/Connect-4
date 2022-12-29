#ifndef CHOOSE
#define CHOOSE
#include "Global.h"
int CharInText(char c,char text[])
{
    int found = 0;
    for (int i = 0; i < strlen(text); i++)
    {
        if (isspace(text[i]))
        {
            continue;
        }
        if (tolower(text[i]) == c)
        {
            found = 1;
        }
        else
        {
            found = 0;
            break;
        }
    }
    return found;
}
int case_To_num(char c[])
{
    int col;
    if (strToint(c)>0)
    {
        col = strToint(c)-1;
    }
    else if (CharInText('u',c))
    {
        col = -5;
    }
    else if (CharInText('r',c))
    {
        col = -10;
    }
    else if (CharInText('s',c))
    {
        col = -15;
    }
    else if (CharInText('e',c))
    {
        col = -20;
    }
    else if (CharInText('h',c))
    {
        col = -25;
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
        while ((((col > -1) && (col < paramrters.COLS)) && (board[0][col]!=' '))||(col>=paramrters.COLS)||(col==-1))
        {
            printf(RED"Error Enter a valid column: "WHITE);
            col = take_column(choice);
        }
        if (col==-5)
        {
            if (moves_counter == 0)
            {
                system("cls");
                printf(RED"This is the first move No moves to Undo\n"WHITE);
            }
            else
            {
                Undo(mode);
                system("cls");
            }
        }
        else if (col==-10)
        {
            if (moves_counter<moves_counter2)
            {
                Redo(mode);
                system("cls");
            }
            else
            {
                system("cls");
                printf(RED"No Redo this the last move\n"WHITE);
            }
        }
        else if (col==-15)
        {
            printf("Name the Game: ");
            gets(savegame.name);
            fflush(stdin);
            system("cls");
            Save();
            printf(GREEN"Game Saved\n"WHITE);
        }
        else if (col==-25)
        {
            system("cls");
            Computer_Play(2,tempPlayer.symbol,mode);
        }
        else if (col < paramrters.COLS && col >= 0 && (board[0][col]==' '))
        {
            board[lowest_row(col)][col] = tempPlayer.symbol;
            moves[moves_counter] = col;
            moves_counter++;moves_counter2 = moves_counter;
            system("cls");
        }
        return col;
    }
    else
    {
        Computer_Play(level,'O',mode);
        system("cls");
        return 0;
    }
}
#endif