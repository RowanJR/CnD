#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "ability_system.h"
#include "entities.h"

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
    entity->abilities.count++;

    if(entity->abilities.count > entity->abilities.length)
    {
        Ability* newarr = malloc(sizeof(Ability*)*entity->abilities.length*2);

        for(int i = 0; i < entity->abilities.count; i++)
        {
            newarr[i] = entity->abilities.list[i];
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

    FireEvent(INITIAL, info);
    
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
        if(entity->resistances[i] = -1)
        {
            entity->currentHP -= (2 * damage[i]);
        }
        //resistant
        else if(entity->resistances[i] = 1)
        {
            entity->currentHP -= (damage[i] / 2);
        }
        //immune
        else if(entity->resistances[i] = 2)
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