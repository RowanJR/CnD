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
    /*
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
    ab.abilfunction(info, GET_TAGS, &ab);
  
    DEBUG_PrintList(data);

    FreeList(data);
    FreeList(info);

    ab.abilfunction(NULL, REMOVE, &ab);*/

    Entity* johnny;

    johnny = DEBUG_SimpleEntity();

    AddAbility(&DEBUG_Test, johnny, NULL);

    FireEvent(TURN_START, NULL);

    node *info = NULL;
    node *data = NULL; 

    node*** datapointer = malloc(sizeof(node**));
    *datapointer = &data;

    AddNode(&info, "dest", datapointer, PTR);

    NotifyAllAbilities(johnny, GET_TAGS, info);

    DEBUG_PrintList(data);

    FreeList(data);
    FreeList(info);

    RemoveAbility(&DEBUG_Test, johnny);

    CloseEventManager();

    return 0;
}

