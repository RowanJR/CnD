#include <stdio.h>
#include <stdlib.h>
#include "map.h"
#include "entities.h"
#include "abilities.h"
#include "ability_system.h"
#include "dice.h"
#include "tiles.h"
#include "turns.h"

int main()
{
    AddInitiative(4, NULL);
    AddInitiative(16, NULL);
    AddInitiative(12, NULL);
    AddInitiative(3, NULL);
    AddInitiative(28, NULL);
    AddInitiative(14, NULL);

    DEBUG_printInitiative();

    return 0;
}

