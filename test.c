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
    InitializeEventManager();
    Ability ab;

    ab.variables = NULL;
    ab.ability_holder = NULL;
    ab.abilfunction = &DEBUG_Test;

    ab.abilfunction(NULL, INITIAL, &ab);

    FireEvent(TURN_START, NULL);

    node *info = NULL;
    node *data = NULL; 

    node*** datapointer = malloc(sizeof(node**));
    *datapointer = &data;

    AddNode(&info, "dest", datapointer, PTR);
    ab.abilfunction(info, GET_DATA, &ab);
  
    DEBUG_PrintList(data);

    FreeList(data);
    FreeList(info);

    ab.abilfunction(NULL, REMOVE, &ab);

    CloseEventManager();

    return 0;
}

