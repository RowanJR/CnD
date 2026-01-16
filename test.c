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
    Ability ab;

    ab.variables = NULL;
    ab.ability_holder = NULL;
    ab.abilfunction = &DEBUG_Test;

    ab.abilfunction(NULL, INITIAL, &ab);

    FireEvent(TURN_START, NULL);
    FireEvent(TURN_START, NULL);
    FireEvent(TURN_START, NULL);

    ab.abilfunction(NULL, REMOVE, &ab);

    return 0;
}

