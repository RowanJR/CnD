#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "ability_system.h"
#include "entities.h"

Entity* DEBUG_SimpleEntity()
{
    Entity* fin = malloc(sizeof(Entity));

    fin->abilities.count = 0;
    fin->abilities.length = 16;
    fin->abilities.list = malloc(sizeof(Ability)*16);

    fin->maxHP = 10;
    fin->currentHP = fin->maxHP;

    for(int i = 0; i < TYPES_NUMBER; i++)
    {
        fin->resistances[i] = 0;
    }

    return fin;
}

int GetModifier(int value)
{
    int mod = 0;

    if(value >= 10)
    {
        mod = (value - 10) / 2;
    }
    else
    {
        mod = (value - 11) / 2;
    }

    return mod;
}

void AddAbility(void* abilfunctionptr, Entity* entity, node* info)
{
    int alreadyexists = 0;

    Ability* existinginst = NULL;

    //need to iterate through list, call "repeat" event if this ability already exists, which queries an ability of what to do in the event of a repeat, and implement what the ability wants
    for(int i = 0; i < entity->abilities.count; i++)
    {
        if((long)(entity->abilities.list[i].abilfunction) == (long)abilfunctionptr)
        {
            alreadyexists = 1;
            existinginst = &(entity->abilities.list[i]);
            break;
        }
    }
    //TODO: PLACEHOLDER
    alreadyexists = 0;

    if(!alreadyexists)
    {
        entity->abilities.count++;

        if(entity->abilities.count > entity->abilities.length)
        {
            Ability* newarr = malloc(sizeof(Ability*)*entity->abilities.length*2);

            for(int i = 0; i < entity->abilities.count; i++)
            {
                newarr[i].ability_holder = entity->abilities.list[i].ability_holder;
                newarr[i].variables = entity->abilities.list[i].variables;
                newarr[i].abilfunction = entity->abilities.list[i].abilfunction;
            }

            free(entity->abilities.list);
            entity->abilities.list = newarr;
            entity->abilities.length *= 2;
        }

        //ISSUE this might not persist after this funciton ends, ensure abilities still remain
        Ability newab;

        newab.ability_holder = entity;
        newab.variables = NULL;
        newab.abilfunction = abilfunctionptr;

        entity->abilities.list[entity->abilities.count - 1] = newab;

        // trigger initial ability
        NotifyAbility(info, INITIAL, &(entity->abilities.list[entity->abilities.count - 1]));
    }
    else
    {
        int *repeatdesired = malloc(sizeof(int));

        node **infoptr = malloc(sizeof(node***));

        *infoptr = info;

        node *subinfo = NULL;

        AddNode(&subinfo, "new_info", infoptr, PTR);

        AddNode(&subinfo, "repeat", repeatdesired, INT);

        NotifyAbility(subinfo, REPEAT, existinginst);

        //yes, this is the exact code from above.
        if(repeatdesired)
        {
            entity->abilities.count++;

            if(entity->abilities.count > entity->abilities.length)
            {
                Ability* newarr = malloc(sizeof(Ability*)*entity->abilities.length*2);

                for(int i = 0; i < entity->abilities.count; i++)
                {
                    newarr[i].ability_holder = entity->abilities.list[i].ability_holder;
                    newarr[i].variables = entity->abilities.list[i].variables;
                    newarr[i].abilfunction = entity->abilities.list[i].abilfunction;
                }

                free(entity->abilities.list);
                entity->abilities.list = newarr;
                entity->abilities.length *= 2;
            }

            //ISSUE this might not persist after this funciton ends, ensure abilities still remain
            Ability newab;

            newab.ability_holder = entity;
            newab.variables = NULL;
            newab.abilfunction = abilfunctionptr;

            entity->abilities.list[entity->abilities.count - 1] = newab;

            // trigger initial ability
            NotifyAbility(info, INITIAL, &(entity->abilities.list[entity->abilities.count - 1]));
        }

        FreeList(subinfo);

    }  
    
    return;
}

int RemoveAbility(void* abilfunctionptr, Entity* entity)
{
    int total = 0;

    for(int i = 0; i < entity->abilities.count; i++)
    {
        if((long)(entity->abilities.list[i].abilfunction) == (long)abilfunctionptr)
        {
            total++;

            // trigger remove ability
            //entity->abilities.list[i].abilfunction(NULL, REMOVE, &entity->abilities.list[i]);
            NotifyAbility(NULL, REMOVE, &entity->abilities.list[i]);

            for(int j = i; j < entity->abilities.count; j++)
            {
                entity->abilities.list[j] = entity->abilities.list[j + 1];
            }
        }
    }

    return total;
}

void DealDamage(Damage_Types* damage, Entity* entity)
{
    for(int i = 0; i < TYPES_NUMBER; i++)
    {
        //vulnerable
        if(entity->resistances[i] == -1)
        {
            entity->currentHP -= (2 * damage[i]);
        }
        //resistant
        else if(entity->resistances[i] == 1)
        {
            entity->currentHP -= (damage[i] / 2);
        }
        //immune
        else if(entity->resistances[i] == 2)
        {

        }
        else
        {
            entity->currentHP -= damage[i];
        }
    }
    return;
}  

void Heal(int healing, Entity* entity)
{
    entity->currentHP += healing;

    if(entity->currentHP > entity->maxHP)
    {
        entity->currentHP = entity->maxHP;
    }

    return;
}

void NotifyAllAbilities(Entity* entity, Event event, node* info)
{
    int num = entity->abilities.count;
    for(int i = 0; i < num; i++)
    {
        entity->abilities.list[i].abilfunction(info, event, &(entity->abilities.list[i]));
    }

    return;
}

void FreeEntity(Entity* entity)
{
    free(entity->name);
    free(entity->abilities.list);

    //TODO free inventory (not yet imlemented)

    free(entity);

    return;
}
