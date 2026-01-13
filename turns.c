#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "turns.h"
#include "entities.h"
#include "ability_system.h"

//number of 6 second turns in some amount of time
int MINUTE = 10;
int HOUR = 600;
int DAY = 14400;
int WEEK = 100800;
int YEAR = 5256000;

//global time; number of turns since start of the world.
long worldtime;
//initiative tracker for initiative in combat
//  outside of combat, this is a NULL pointer, ensure a check before use
initiative_tracker initiative_track;

void DEBUG_printInitiative()
{
    initiative_card* current = initiative_track.start;

    printf("initiative:\n");

    while(current != NULL)
    {
        printf("i: %d, dm: %d\n", current->initiative, current->dexmod);
        
        current = current->next;
    }
}

//TODO; conduct more thorough testing of function
void AddInitiative(int initiative, Entity* creature)
{
    initiative_card* newcard = malloc(sizeof(initiative_card));

    newcard->creature = creature;
    newcard->initiative = initiative;

    //this shouldn't happen normally, but to make testing easier I allow for the creature passed in to be NULL
    if(creature != NULL)
    {
        newcard->dexmod = GetModifier(creature->stats.de);
    }
    else
    {
        newcard->dexmod = 0;
    }
    
    initiative_card* current = initiative_track.start;
    initiative_card* previous;

    //loop through list to find correct position
    while(current != NULL)
    {
        //if the previous initiative is already more than the new one, add the new one to the linked list
        if(newcard->initiative > current->initiative)
        { 
            if(initiative_track.start == current)
            {
                newcard->next = initiative_track.start;

                initiative_track.start = newcard;
            }
            else
            {
                newcard->next = current;

                previous->next = newcard;
            }

            break;
        }
        //otherwise, compare dexmods
        else if(newcard->initiative == current->initiative)
        {
            while(current != NULL)
            {
                if(newcard->dexmod >= current->dexmod)
                {
                    if(initiative_track.start == current)
                    {
                        newcard->next = initiative_track.start;

                        initiative_track.start = newcard;
                    }
                    else
                    {
                        newcard->next = current;

                        previous->next = newcard;
                    }
                    
                    break;
                }

                previous = current;
                current = current->next;
            }

            break;
        }

        previous = current;
        current = current->next;
    }

    //inserting at the start of the list
    if(initiative_track.start == current)
    {
        newcard->next = initiative_track.start;

        initiative_track.start = newcard;
    }
    //if our card goes at the end of the list, it wont have been inserted
    else if(current == NULL)
    {
        newcard->next = current;

        previous->next = newcard;
    }

    return;
}

//TODO test this for memory leak problems when ending combat
void EndInitiative()
{
    initiative_card* current = initiative_track.start;
    initiative_card* next;

    while(current != NULL)
    {   
        next = current->next;

        free(current);

        current = next;
    }
    
    initiative_track.start, initiative_track.current = NULL;

    return;
}

void NextTurn()
{
    node* info = NULL;

    AddNode(&info, "target", initiative_track.current->creature, ENTITY);
    FireEvent(TURN_END, info);
    NotifyAbilities(initiative_track.current->creature, TURN_END, info);
    FreeList(info);

    initiative_track.current = initiative_track.current->next;

    info = NULL;

    AddNode(&info, "target", initiative_track.current->creature, ENTITY);
    FireEvent(TURN_START, info);
    NotifyAbilities(initiative_track.current->creature, TURN_START, info);
    FreeList(info);

    return;
}
