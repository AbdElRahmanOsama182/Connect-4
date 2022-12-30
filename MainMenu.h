#ifndef MAINMENU
#define MAINMENU
#include "Global.h"
//Main Menu
int MainMenu()
{
    printf(GREEN"1. Start New Game\n");
    printf("2. Load Game\n");
    printf("3. Top Players\n");
    printf("4. How To Play\n");
    printf("5. Quit\n");
    printf("Enter the option number: ");
    char option[MAX_LEN];
    fgets(option, MAX_LEN, stdin);
    system("cls");
    if ((strToint(option) < 6) && (strToint(option)>0))
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
void instructions()
{
    printf(GREEN"# Try to connect as many 4's as you can and prevent your opponent to do so\n");
    printf("# You can connect it Horizontally or Vertically or Diagonally\n");
    printf("# Choose the number of column you want to play in or the character of the option\n");
    printf("# Enjoy Our Game (:\n"WHITE);
}
//Back To The Main After Finishing
int BackToMain()
{
    printf(GREEN"1. Back to Main Menu\n");
    printf("2. Quit\n");
    printf("Enter the option number: ");
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
        return 5;
    }
}
#endif