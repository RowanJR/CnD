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
        case GET_DATA:
            node** destination = (node**)ListSearch(info, "dest");
            if(destination != NULL)
            {
                AddNode(destination, "name", value, DATATYPE);
            }
            break;
        case GET_TAGS:
            //node** destination = (node**)ListSearch(info, "dest");
            if(destination != NULL)
            {
                AddNode(destination, "tag", "true", STR);
            }
            break;
        default:
            break;
    }
}
*/

//test function
void DEBUG_Test(node* info, Event event, Ability* instance)
{
    switch(event)
    {
        case INITIAL:
            printf("initializing ability\n");

            Subscribe(TURN_START, instance);

            break;
        case TURN_START:

            printf("turn start detected\n");

            break;
        case REMOVE:
            printf("removing ability\n");

            Unsubscribe(TURN_START, instance);

            FreeList(instance->variables);

            break;
        case GET_DATA:
            node*** destination = (node***)ListSearch(info, "dest");
            if(destination != NULL)
            {
                AddNode(*destination, "name", "DEBUG_test", STR);
                AddNode(*destination, "description", "DEBUG_test description", STR);
            }
            break;
        case GET_TAGS:
            //void* destination = ListSearch(info, "dest");
            if(destination != NULL)
            {
                AddNode(*destination, "DEBUG", "true", STR);
            }
            break;
        default:
            break;
    }
}

//simple burning condition to demonstrate basic ability structures
// extra comments to help as a kind of tutorial, typical abilities don't need so many
void Debug_Burning(node* info, Event event, Ability* instance)
{
    switch(event)
    {
        //in our initial case, we need to set how long the condition will last. 
        //TODO: we need a duplicate case for when a burning stack is already on an entity
        case INITIAL: 
            //subscribe to TURN_START
            Subscribe(TURN_START, instance);
            //subscribe to TIMESTEP
            Subscribe(TIMESTEP, instance);

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
        //this is TURN_START outside of combat. Cautious when using this, because the info node contains nothing
        case TIMESTEP:
        //in our turn start case, we want to do 1d4 fire damage and decrease the number of burning stacks by 1
        case TURN_START:  
            //each turn, we need to check whose turn it is. If it's the entity holder's turn, we execute our start of turn code
            if(event == TIMESTEP || (ListSearch(info, "target") == instance->ability_holder))
            {
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
            }
            break;
        //clean up all of our allocated information and resolve any potential bugs caused by an ability being removed
        // always keep in mind, the ability being removed will be completely gone, so get rid of everything it allocates
        case REMOVE:
            Unsubscribe(TURN_START, instance);    
            Unsubscribe(TIMESTEP, instance);   

            break;
        //simple pattern that allows simple, human readable code
        // a desitnation must be supplied and freed after use when calling GET_DATA
        // the string "true" is used for any boolean. Memory allocated for this shouldn't persist at all after initial use, and ListSearch() needs something to return
        // do not use any caps unless its a description or Debug/DEBUG
        case GET_DATA:
            node** destination = (node**)ListSearch(info, "dest");
            if(destination != NULL)
            {
                AddNode(destination, "name", "Debug_burning", STR);
                AddNode(destination, "description", "Debug burning debuff", STR);
                AddNode(destination, "is_usable", "true", STR);
                AddNode(destination, "is_usable", "true", STR);
                AddNode(destination, "use_description", "Use an action to remove burning.", STR);
            }
            break;
        //similar to GET_DATA, though all tags are booleans
        // no need for any "false" booleans, ListSeach returning NULL is equivalent to false
        // do not use any caps unless its a Debug/DEBUG
        case GET_TAGS:
            //void* destination = ListSearch(info, "dest");
            if(destination != NULL)
            {
                AddNode(destination, "DEBUG", "true", STR);
                AddNode(destination, "burning", "true", STR);
                AddNode(destination, "status", "true", STR);
                AddNode(destination, "harmful", "true", STR);
            }
            break;
        default:
            break;
    }
    return;
}

