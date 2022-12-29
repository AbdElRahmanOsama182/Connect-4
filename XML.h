#ifndef Xml
#define Xml
#include "Global.h"
//Turning String Into Intiger
int strToint(char c[])
{
    int result = 0;
    for (int i = 0; i < strlen(c); i++)
    {
        if (isspace(c[i]))
        {
            continue;
        }
        if(!(isdigit(c[i])))
        {
            result = -1;
            break;
        }
        result = result * 10 + (c[i] - '0');
    }
    return result;
}
//Searching For The Tags In The Text
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
                break;
            }
        }
        i++;
    }
    return found;
}
//Bringin The Parameters If Exists And Ignoring All White Spaces
int bring_parameter(char start[],char end[],char text[])
{
    int s = word_in_text(start,text)+strlen(start);
    int e = word_in_text(end,text);
    int parameter;
    if ((e == -1)||(s == -1)||(e-s <= 0))
    {
        parameter = 0;
    }
    else
    {
        char temp[MAX_LEN]={0};
        int onlyNums = 1;
        int j = 0;
        for (int i = 0; i < e-s; i++)
        {
            temp[i] = text[s+i];
        }
        parameter = strToint(temp);
    }
    return parameter;
}
//Bringing The Functions Up Together And Reading The File
void ReadXML(FILE* xml_file)
{
    char configurations_s[]="<Configurations>"; char configurations_e[]="</Configurations>";
    char height_s[]="<Height>"; char height_e[]="</Height>";
    char width_s[]="<Width>"; char width_e[]="</Width>";
    char highscores_s[]="<Highscores>"; char highscores_e[]="</Highscores>";
    int i = 0;
    int j = 0;
    while (!feof(xml_file))
    {
        char temp = fgetc(xml_file);
        xml_content[i] = temp;
        i++;
    }
    char config_content[MAX_LEN];
    int config_content_S = word_in_text(configurations_s,xml_content);
    int config_content_E = word_in_text(configurations_e,xml_content);
    if ((config_content_S != -1)&&(config_content_E != -1))
    {
        for (int i = config_content_S; i < config_content_E; i++)
        {
            config_content[j] = xml_content[i];
            j++;
        }
    }
    paramrters.ROWS = bring_parameter(height_s, height_e,config_content);
    paramrters.COLS = bring_parameter(width_s, width_e,config_content);
    paramrters.HIGHSCORES = bring_parameter(highscores_s, highscores_e,config_content);
}
// If The File Is Not Valid Take The Path From User And Try 3 Times Or Load The Defaults
void XML()
{
    FILE* xml_file= fopen("parameters.xml", "r");
    int tries = 3;
    if (!(xml_file == NULL))
    {
        ReadXML(xml_file);
        fclose(xml_file);
    }
    while (((xml_file == NULL) || (paramrters.ROWS <= 0) || (paramrters.COLS <= 0) || (paramrters.HIGHSCORES <= 0)) && (tries)) 
    {
        printf(RED"File error, Please Enter the correct file path with a valid parameters: "WHITE);
        char path[MAX_LEN];
        gets(path);
        fflush(stdin);
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
    if (((xml_file == NULL) || (paramrters.ROWS <= 0) || (paramrters.COLS <= 0) || (paramrters.HIGHSCORES <= 0)) && (tries == 0))
    {
        paramrters.ROWS = 9;
        paramrters.COLS = 7;
        paramrters.HIGHSCORES = 10;
        printf(RED"Failed 3 times!\nLoading the defaults....\n");
    }
    printf(GREEN"Height = %d, Width = %d, Highscores = %d\n"WHITE,paramrters.ROWS, paramrters.COLS, paramrters.HIGHSCORES);
}
#endif