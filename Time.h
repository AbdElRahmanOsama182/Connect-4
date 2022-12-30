#ifndef TIME
#define TIME
#include "Global.h"
//Spliting time into hours, minutes and seconds.
Time split_time( long int s)
{
    Time result;
    result.minutes = s/60;
    result.seconds = s%60;
    return result;
}
//Calculating the game time.
Time Timer()
{
    time(&time_end);
    Time result = split_time(difftime(time_end,time_start));
    return result;
}
#endif
