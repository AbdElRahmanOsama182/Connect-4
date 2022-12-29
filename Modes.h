#ifndef MODES
#define MODES
#include "Global.h"

//Choosing The Game Mode
int game_mode()
{
    int mode;
    printf(GREEN"For Human Vs Computer :Enter 1\nFor Human Vs Human :Enter 2\n");
    char g_mode[MAX_LEN];
    fgets(g_mode,sizeof(g_mode),stdin);
    system("cls");
    if (strToint(g_mode)==1)
    {
        printf("Human Vs Computer\n"WHITE);
        mode = 0;
    }
    else if (strToint(g_mode)==2)
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
//Choosing The Game Difficulty
int game_level()
{
    int level;
    printf(GREEN"Easy Mode :Enter 1\nMedium Mode :Enter 2\nHard Mode :Enter 3\n");
    char g_level[MAX_LEN];
    fgets(g_level,sizeof(g_level),stdin);
    system("cls");
    if (strToint(g_level)==1)
    {
        printf("Easy Mode\n"WHITE);
        level = 0;
    }
    else if (strToint(g_level)==2)
    {
        printf("Medium Mode\n"WHITE);
        level = 1;
    }
    else if (strToint(g_level)==3)
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
#endif