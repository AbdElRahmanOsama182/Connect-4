#ifndef PLAY
#define PLAY
#include "Global.h"
//Bringing All The Functions Together And Controlling The Game Flow
void Play(int option)
{
    if (option < 4)
    {
        XML();
        if (option < 3)
        {
            if (option == 1)
            {
                printf("Starting a New Game\n");
                NewBoard();
            }
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
                printf(PURPLE"\nChoose Number Of The Column Or \n");
                printf("( 'U' for Undo / 'R' for Redo / 'H' for Hint / 'S' for Save / 'E' for Exit )\n"WHITE);
                end_game = choose(mode,level);
                if(end_game ==-20)
                {
                    printf(GREEN"Ending Game\n");
                    break;
                }
                else
                {
                    timer = Timer();
                    print_game_data();
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
#endif
