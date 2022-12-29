#ifndef MAINMENU
#define MAINMENU
#include "Global.h"

//Main Menu
int MainMenu()
{
    printf(GREEN"Start New Game: 1\n");
    printf("Load Game: 2\n");
    printf("Top Players: 3\n");
    printf("Quit: 4\n");
    char option[MAX_LEN];
    fgets(option, MAX_LEN, stdin);
    system("cls");
    if ((strToint(option) < 5) && (strToint(option)>0))
    {
        printf(WHITE);
        return strToint(option);
    }
    else
    {
        printf(RED"Error enter a valid option!\n");
        return MainMenu();
    }
}
//Back To The Main After Finishing
int BackToMain()
{
    printf(GREEN"Back to Main Menu: 1\n");
    printf("Quit: 2\n");
    char option[MAX_LEN];
    fgets(option, MAX_LEN, stdin);
    system("cls");
    if (strToint(option) == 1)
    {
        printf(WHITE);
        MainMenu();
    }
    else if (strToint(option) != 2)
    {
        printf(RED"Error enter a valid option!\n");
        return BackToMain();
    }
    else
    {
        return 4;
    }
}
#endif