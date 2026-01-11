#ifndef ENTITIES_H_
#define ENTITIES_H_

#include "ability_system.h"

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

//uses first two letters for stats because of "int" and they're goo enough
typedef struct{
    int st;
    int de;
    int co;
    int wi;
    int in;
    int ch;
}Stats;

//tracks proficiencies. 1 has proficiency, 2 has expertise
typedef struct{
    short int strength;
    short int dexterity;
    short int constitution;
    short int intelligence;
    short int wisdom;
    short int charisma;

    short int acrobatics;
    short int animal_handling;
    short int arcana;
    short int deception;
    short int history;
    short int insight;
    short int intimidation;
    short int investigation;
    short int medicine;
    short int nature;
    short int perception;
    short int performance;
    short int persuasion;
    short int religion;
    short int sleight_of_hand;
    short int stealth;
    short int survival;

    short int simple_weapons;
    short int martial_weapons;
    short int light_armor;
    short int medium_armor;
    short int heavy_armor;
}Skills;

//range in squares (feet/5) of each sense of a creature
typedef struct{
    float vision;
    float darkvision;
    float blindsight;

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
}Dice;

typedef struct{
    char **name;

    //a boolean that determines whether an entity takes up its space or can exist with other things in its space, like an item
    short int takes_space;
    //a boolean that determines whether an entity acts during initiative or can't, like a tree 
    short int initiative;

    int size;
    int type;

    Stats stats;
    Skills skills;
    Damage_Types resistances[TYPES_NUMBER];
    Senses senses;

    Dice hitdice;
    Dice currenthitdice;

    int AC;
    int maxHP;
    int currentHP;
    float CR;
    short int prof;

    //movement speeds, in squares (feet/5)
    float walk;
    float fly;
    float swim;
    float burrow;

    //determines how many weapons can be held and things can be grappled; ex, human has 2
    short int holds;
    Inventory inventory;

    Ability_Manager abilities;
}Entity;

int GetModifier(int value);

//info can be NULL if no info wants to be passed
void AddAbility(void* abilfunctionptr, Entity* entity, node* info);

//return the number of times the provided ability was found in the ability list and removed
int RemoveAbility(void* abilfunctionptr, Entity* entity);

//needs to be passed an array of damages
void DealDamage(Damage_Types* damage, Entity* entity);

#endif // ENTITIES_H_