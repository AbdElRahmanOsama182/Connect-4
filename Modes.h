#ifndef MODES
#define MODES
#include "Global.h"

//Choosing The Game Mode
int game_mode()
{
    int mode;
    printf(GREEN"1. For Human Vs Computer\n2. For Human Vs Human\n");
    printf("Enter the option number: ");
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
    printf(GREEN"1. Easy Mode\n2. Medium Mode\n3. Hard Mode\n");
    printf("Enter the option number: ");
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