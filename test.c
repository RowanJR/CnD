#include <stdio.h>
#include <stdlib.h>
#include "map.h"
#include "entities.h"
#include "abilities.h"
#include "ability_system.h"
#include "dice.h"
#include "tiles.h"

int main()
{
    InitializeEventManager();

    int* greg = malloc(sizeof(int));
    int* breg = malloc(sizeof(int));

    *greg = 1;
    *breg = 2;

    Subscribe(TURN_END, &greg);

    DEBUG_PrintManager();

    Unsubscribe(TURN_END, &greg);

    //DEBUG_PrintManager();

    return 0;
}

