#ifndef HighScores
#define HighScores
#include "Global.h"
//Using Merge Sort ALgorithm To Sort The Scores
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
//Read Scores From File & Sort it
void read_scores()
{
    scores_counter = 0;
    FILE * scores_file = fopen("hScores.bin", "rb");
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
//Update The Player Score If It's Higher OR Add It If It's His First Score
void addWinnerToList()
{
    int found = 0;
    for (int i = 0; i < scores_counter-1; i++)
    {
        if (strcasecmp(TopPlayers[i].name,winner.name)==0)
        {
            found = 1;
            if(winner.score>TopPlayers[i].score)
            {
                TopPlayers[i].score = winner.score;
            }
        }
    }
    if(!found)
    {
        TopPlayers[scores_counter] = winner;
        scores_counter++;
    }
    mergeSort(TopPlayers,0,scores_counter-1);
}
//Write The New Scores In The File
void addWinnerTofile()
{
    FILE * scores_file = fopen("hScores.bin", "wb");
    for (int i = 0; i < scores_counter; i++)
    {
        fwrite(&TopPlayers[i],sizeof(TopPlayers[i]),1,scores_file);
    }
    fclose(scores_file);
}
//Getting The Player Rank Among The Players
int get_rank()
{
    addWinnerToList();
    int rank = 1;
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
//Printing The HighScores List
void print_TopPlayers()
{
    int size = scores_counter-1;
    if (paramrters.HIGHSCORES < scores_counter-1)
    {
        size = paramrters.HIGHSCORES;
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
#endif