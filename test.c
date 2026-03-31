#include <stdio.h>
#include <stdlib.h>
#include "map2d.h"
#include "entities.h"
#include "abilities.h"
#include "ability_system.h"
#include "dice.h"
#include "tiles.h"
#include "turns.h"

int main()
{
    /*InitializeEventManager();
    InitializeDice();

    Entity* entityname;

    entityname = DEBUG_SimpleEntity();

    node *info = NULL;

    int* turns = malloc(sizeof(int));
    *turns = 100;
    AddNode(&info, "initial_turns", turns, INT);
    AddAbility(&Debug_Burning, entityname, info);
    FreeList(info);

    while(entityname->currentHP > 0)
    {
        FireEvent(TIMESTEP, NULL);
        printf("HP: %d\n", entityname->currentHP);
    }

    RemoveAbility(&Debug_Burning, entityname);

    FreeEntity(entityname);

    CloseEventManager();*/

    Loaded_area* loaded = InitializeLoaded(1);

    ShiftLoaded(loaded, MakeVec(-1, 0));

    for(int i = 0; i < 9; i++)
    {
        if(loaded->chunks[i] == NULL)
        {
            printf("main\n");
        }
    }

    for(int i = 0; i < 3; i++)
    {
        if(loaded->downbuffer[i] == NULL)
        {
            printf("db\n");
        }
        if(loaded->upbuffer[i] == NULL)
        {
            printf("ub\n");
        }
        if(loaded->leftbuffer[i] == NULL)
        {
            printf("lb\n");
        }
        if(loaded->rightbuffer[i] == NULL)
        {
            printf("rb\n");
        }
    }

    return 0;
}

