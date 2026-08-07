#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "ability_system.h"
#include "entities.h"
#include "dice.h"

int RAWADVANTAGERULE = 0; //gamerule for rules-as-written advantage; if there is both advantage and disadvantages on a roll, it's straight, even with 100 advantages and 1 disadvantage. If 0, we use the logical "whichever has more" approach

Stats GetDefaultSkillScore(Skill skill)
{
    switch(skill)
    {
        case ACROBATICS:
            return DEXTERITY;
        case ANIMAL_HANDLING:
            return WISDOM;
        case ARCANA:
            return INTELLIGENCE;
        case ATHLETICS:
            return STRENGTH;
        case DECEPTION:
            return CHARISMA;
        case HISTORY:
            return INTELLIGENCE;
        case INSIGHT:
            return WISDOM;
        case INTIMIDATION:
            return CHARISMA;
        case INVESTIGATION:
            return INTELLIGENCE;
        case MEDICINE:
            return WISDOM;
        case NATURE:
            return INTELLIGENCE;
        case PERCEPTION:
            return WISDOM;
        case PERFORMANCE:
            return CHARISMA;
        case PERSUASION:
            return CHARISMA;
        case RELIGION:
            return INTELLIGENCE;
        case SLEIGHT_OF_HAND:
            return DEXTERITY;
        case STEALTH:
            return DEXTERITY;
        case SURVIVAL:
            return WISDOM;
        default:
            break;
    }
    //should raise confusion if something is wrong, since no skill uses constitution
    return CONSTITUTION;
}

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

int AbilityCheck(Entity* target, Skill prof, Stats score)
{
    Skill_Check_Pack* checkptr = malloc(sizeof(Skill_Check_Pack));

    checkptr->abilityscore = score;
    checkptr->proficiency = prof;
    checkptr->target = target;

    checkptr->advantages = 0;
    checkptr->disadvantages = 0;
    checkptr->additionalmodifier = 0;

    node *info = NULL; //make out info linked-list
    //we're making a pointer to a pointer so that we free the pointer when we free the list, instead of the struct which it points to
    Skill_Check_Pack** checkptrptr = malloc(sizeof(Skill_Check_Pack*)); //allocate the pointer to the pointer
    *checkptrptr = checkptr; //set the pointer to the packet we've made

    AddNode(&info, "Skill_Check_Pack", checkptrptr, UNKNOWN); //add the pointer to out info linked list

    FireEvent(CHECK, info); //fire the event and supply a pointer to the info

    FreeList(info); //free info list after everything notified by the listener system has gotten to act on our packet

    int finalroll;//initialize our roll
    //this step gives us our unmodified dice roll. Can change based on selected game rule.
    //"cancelling advantage" rules-as-written
    if(RAWADVANTAGERULE)
    {
        //roll with advantage
        if((checkptr->advantages > 0 && checkptr->disadvantages == 0))
        {
            finalroll = RollAdvantage(20);
        }
        //roll with disadvantage
        else if((checkptr->advantages == 0 && checkptr->disadvantages > 0))
        {
            finalroll = RollDisadvantage(20);
        }
        //roll straight
        else
        {
            finalroll = Roll(20);
        }
    }
    //"duelling advantage" rule
    else
    {
        int advantagedif = checkptr->advantages - checkptr->disadvantages;
        //roll with advantage
        if(advantagedif > 0)
        {
            finalroll = RollAdvantage(20);
        }
        //roll with disadvantage
        else if(advantagedif < 0)
        {
            finalroll = RollDisadvantage(20);
        }
        //roll straight
        else
        {
            finalroll = Roll(20);
        }
    }

    //TODO need some system for rolling 1s. maybe just automatically return a 1, should have a game rule for if 1s auto-fail Ability Checks.

    

    free(checkptr); //free our check packet before we go
    return finalroll;
}

void Attack(Entity* target, Entity* attacker)
{
    

    return;
}

void SavingThrow()
{


    return;
}