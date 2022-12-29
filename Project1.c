#include "Global.h"
#include "XML.h"
#include "MainMenu.h"
#include "Score.h"
#include "Time.h"
#include "HighScores.h"
#include "SaveLoad.h"
#include "Print.h"
#include "UndoRedo.h"
#include "Modes.h"
#include "ComputerPlay.h"
#include "Choose.h"
#include "Play.h"
int main() {
    //Player1                  //Player2
    player1.number = 1 ;       player2.number = 2;
    player1.symbol = 'X';      player2.symbol = 'O';
    tempPlayer = player1;
    system(" ");
    system("cls");
    Play(MainMenu());
    return 0;
}