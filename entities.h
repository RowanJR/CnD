#ifndef ENTITIES_H_
#define ENTITIES_H_

#include "ability_system.h"
//#include "map.h"

typedef enum{
    SLASHING,
    BLUDGEONING,
    PIERCING,
    RADIANT,
    NECROTIC,
    COLD,
    FIRE,
    LIGHTNING,
    POISON,
    THUNDER,
    ACID,
    FORCE,
    PSYCHIC,
    TYPES_NUMBER
}Damage_Types;

typedef enum{
    ABBERATION,
    BEAST,
    CELESTIAL,
    CONSTRUCTS,
    DRAGON,
    ELEMENTALS,
    FEY,
    FIEND,
    GIANT,
    HUMANOID,
    MONSTROSITY,
    OOZE,
    PLANT,
    UNDEAD
}Creature_Type;

typedef enum{
    TINY,
    SMALL,
    MEDIUM,
    LARGE,
    HUGE,
    GARGANTUAN
}Size;

//uses first two letters for stats because of "int" and they're good enough
typedef struct{
    int st;
    int de;
    int co;
    int wi;
    int in;
    int ch;
}Stats;

//range in squares (feet/5) of each sense of a creature
typedef struct{
    float vision;
    float darkvision;
    float blindsight;
    float truesight;

    float hearing;
    float smell;
}Senses;

//controls abilities linked to an entity, active and passive
typedef struct{
    int count;
    int length;
    Ability* list;
}Ability_Manager;

//tracks the items someone has on their person
// TODO implement inventory
typedef struct{
    
}Inventory;

//tracks dice possession (primarily used for hit dice)
typedef struct{
    int d4;
    int d6;
    int d8;
    int d10;
    int d12;
    int d20;
    int d100;
}Dice;

typedef struct{
    char *name;
    char *species;

    Creature_Type type;
    Size size;

    //a boolean that determines whether an entity takes up its space or can exist with other things in its space, like an item
    short int occupies_space;
    //a boolean that determines whether an entity acts during initiative or can't, like a tree 
    short int initiative;


    Stats stats;
    Damage_Types resistances[TYPES_NUMBER];
    Senses senses;

    Dice hitdice;
    Dice currenthitdice;

    int AC;
    int maxHP;
    int currentHP;
    float CR;
    short int proficiency;

    //max movement speeds, in squares (feet/5)
    float walk;
    float fly;
    float swim;
    float burrow;

    //determines how many weapons can be held and things can be grappled; ex, human has 2
    short int holds;
    Inventory inventory;

    Ability_Manager abilities;

    //Chunk* currentchunk;
}Entity;

//initialize a basic entity for debugging and testing
Entity* DEBUG_SimpleEntity();

int GetModifier(int value);

//info can be NULL if no info wants to be passed
void AddAbility(void* abilfunctionptr, Entity* entity, node* info);

//return the number of times the provided ability was found in the ability list and removed
int RemoveAbility(void* abilfunctionptr, Entity* entity);

//needs to be passed an array of damages
void DealDamage(Damage_Types* damage, Entity* entity);

//heal an entity
void Heal(int healing, Entity* entity);

//notifies all abilities on an entity with the given event
// careful using, may notify lots of abilities that have no case for the given events (waste of processing/time)
void NotifyAllAbilities(Entity* entity, Event event, node* info);

void FreeEntity(Entity* entity);

#endif // ENTITIES_H_