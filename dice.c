#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "dice.h"

//rolls die
int Roll(int sides)
{
    srand(time(NULL));
    
    return (rand() % sides) + 1;;
}