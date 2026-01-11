#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "map.h"
#include "entities.h"
#include "ability_system.h"
#include "abilities.h"
#include "dice.h"

/*
template for a new ability. Ensure its added to the header file as well!

void template(node* info, Event event, Ability* instance)
{
    switch(event)
    {
        case INITIAL:

            break;
        case REMOVE:

            break;
        default:
            break;
    }
}
*/

//simple burning condition to demonstrate basic ability structures
// extra comments to help as a kind of tutorial, typical abilities don't need so many
void Debug_Burning(node* info, Event event, Ability* instance)
{
    switch(event)
    {
        //in our initial case, we need to set how long the condition will last. 
        // We do not need to subscribe to anything, because TURN_START will be sent directly when it is the ability holder's turn
        //TODO: we need a duplicate case for when a burning stack is already on an entity
        case INITIAL: 
            //our initial turns should be provided in the information passed to this function
            void* initial_turns = ListSearch(info, "initial_turns");

            //if our information doesn't have the initial turns, we'll have NULL returned, so we'll make that give us 1 turn of burning
            // note that we use pointers rather than direct variables, which is because this could be something like a string or other data type
            int *turns_remaining = malloc(sizeof(int));
            *turns_remaining = 1;
            
            if(initial_turns != NULL)
            {
                //we need to change the actual value by casting this void pointer to an int pointer and then getting its actual value
                *turns_remaining = *(int*)initial_turns;
            }

            //a little pointermancy is required;
            // we need the address of the start of the variable list, since it may be empty (as is the case here).
            // we can use a constant string for the name. Spaces can be used, I just use underscores for consistency.
            // we need to allocate the space and assign our variable outside of this AddInfo function, since it could be anything, and the function doesn't know, and we then need to cast it to void*.
            AddNode(&instance->variables, "turns_remaining", (void*)turns_remaining, INT);

            break;
        //in our turn start case, we want to do 1d4 fire damage and decrease the number of burning stacks by 1
        case TURN_START:  
            //first: deal 1d4 fire damage
            //find how much damage we do
            int firedamage = Roll(4);

            //when doing damage it needs to be passed an array of damages
            Damage_Types* damage = malloc(sizeof(Damage_Types) * TYPES_NUMBER);
            //access each damage type with its name as an index
            damage[FIRE] = firedamage;

            DealDamage(damage, instance->ability_holder);
            //make sure to free a damage array after use!
            free(damage);

            //second: decrement turns_remaining

            //find turns_remaining
            turns_remaining = (int*)ListSearch(instance->variables, "turns_remaining");

            //decrement the actual value in turns remaining
            *turns_remaining -= 1;

            //if we have no more stacks remaining, delete this ability
            if(*turns_remaining <= 0)
            {
                //TODO: remove ability
            }

            break;
        //clean up all of our allocated information and 
        case REMOVE:
            
            break;
        default:
            break;
    }
    return;
}

