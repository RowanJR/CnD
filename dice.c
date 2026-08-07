#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "dice.h"

void InitializeDice()
{
    srand(time(NULL));
}

//rolls die
int Roll(int sides)
{   
    return (rand() % sides) + 1;;
}

int RollDisadvantage(int sides)
{
    int first = Roll(sides);
    int second = Roll(sides);

    if(first < second)
    {
        return first;
    }
    return second;
}

int RollAdvantage(int sides)
{
    int first = Roll(sides);
    int second = Roll(sides);

    if(first > second)
    {
        return first;
    }
    return second;
}