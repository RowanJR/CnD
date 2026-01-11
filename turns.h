#ifndef TURNS_H_
#define TURNS_H_

#include "entities.h"

//nodes for a linked list
//  yes, this is slightly modified and repeated code from the ability system file.
typedef struct initiative_card{
    int initiative;                 //initiative rolled for the current combat
    int dexmod;                     //dexterity mod, saved here for convienence (higher dex mod is used as a tie-breaker)
    Entity* creature;               //pointer to the value of the node; points to an entity
    struct initiative_card* next;   //next node in linked list
}initiative_card;

//initiative tracker
//  functions similarly to a stream
typedef struct initiative_tracker{
    struct initiative_card* current;    //first card in initiative order
    struct initiative_card* start;      //next node in linked list
}initiative_tracker;

void DEBUG_printInitiative();

//adds a creature to the initiative tracker
//  initiative is the initiative + relevant modifiers
//  creature is the entity who's initiative is being sent
//      if a creature is already present in initiative order, the old initiative is removed and the new one is added
void AddInitiative(int initiative, Entity* creature);

//clears up everything important after ending initiative combat
void EndInitiative();

#endif // TURNS_H_