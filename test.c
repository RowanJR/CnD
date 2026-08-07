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

    /*Loaded_area* loaded = InitializeLoaded(1);

    {
    int side = (loaded->loaddistance * 2) + 1;

    printf("C ");
    for(int i = 0; i < side; i++)
    {
        if(loaded->downbuffer != NULL)
        {
            printf("%d ", loaded->downbuffer[i]->cells[0]);
        }
        else
        {
            printf("N ");
        }
    }
    printf("C \n");

    for(int i = 0; i < side; i++)
    {
        
        if(loaded->leftbuffer != NULL)
        {
            printf("%d ", loaded->leftbuffer[i]->cells[0]);
        }
        else
        {
            printf("N ");
        }
        for(int j = 0; j < side; j++)
        {
            if(loaded->chunks[j] != NULL)
            {
                printf("%d ", loaded->chunks[i*side+j]->cells[0]);
            }
            else
            {
                printf("N ");
            }
        }
        if(loaded->rightbuffer != NULL)
        {
            printf("%d \n", loaded->rightbuffer[i]->cells[0]);
        }
        else
        {
            printf("N \n");
        }
    }

    printf("C ");
    for(int i = 0; i < side; i++)
    {
        if(loaded->upbuffer != NULL)
        {
            printf("%d ", loaded->upbuffer[i]->cells[0]);
        }
        else
        {
            printf("N ");
        }
    }
    printf("C \n");
    }

    ShiftLoaded(loaded, MakeVec(0, -1));
    printf("\n");
    {
    int side = (loaded->loaddistance * 2) + 1;

    printf("C ");
    for(int i = 0; i < side; i++)
    {
        if(loaded->downbuffer != NULL)
        {
            printf("%d ", loaded->downbuffer[i]->cells[0]);
        }
        else
        {
            printf("N ");
        }
    }
    printf("C \n");

    for(int i = 0; i < side; i++)
    {
        
        if(loaded->leftbuffer != NULL)
        {
            printf("%d ", loaded->leftbuffer[i]->cells[0]);
        }
        else
        {
            printf("N ");
        }
        for(int j = 0; j < side; j++)
        {
            if(loaded->chunks[j] != NULL)
            {
                printf("%d ", loaded->chunks[i*side+j]->cells[0]);
            }
            else
            {
                printf("N ");
            }
        }
        if(loaded->rightbuffer != NULL)
        {
            printf("%d \n", loaded->rightbuffer[i]->cells[0]);
        }
        else
        {
            printf("N \n");
        }
    }

    printf("C ");
    for(int i = 0; i < side; i++)
    {
        if(loaded->upbuffer != NULL)
        {
            printf("%d ", loaded->upbuffer[i]->cells[0]);
        }
        else
        {
            printf("N ");
        }
    }
    printf("C \n");
    }

    UnloadLoaded(loaded);*/

    return 0;
}

