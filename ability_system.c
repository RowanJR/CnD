#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "map.h"
#include "ability_system.h"

//global event manager used by notifier functions
Event_manager manager;
//global tracker of all events
Dynamic_array event_registry;

void DEBUG_PrintManager()
{
    for(int i = 0; i < EV_COUNT; i++)
    {
        printf("\n%d:\n", i);
        printf("subs: %d\n", manager.eventsubs[i].count);
        for(int j = 0; j < manager.eventsubs[i].count; j++)
        {   
            printf("%x\n", &(manager.eventsubs[i].elems[j]));
        }
    }
}

//__attribute__((constructor)) (look into this)
void InitializeEventManager()
{
    //set all starting event subscriber lists. each starts with 16 elements (arbitrary power of 2; very rare events probably won't show up more than that, common ones will)
    for(int i = 0; i < EV_COUNT; i++)
    {
        manager.eventsubs[i].leng = 16;
        manager.eventsubs[i].count = 0;
        manager.eventsubs[i].elems = malloc(sizeof(void*) * 16);
    }
}

void CloseEventManager()
{
    for(int i = 0; i < EV_COUNT; i++)
    {
        free(manager.eventsubs[i].elems);
    }
}

void Subscribe(Event event, void* ability)
{
    manager.eventsubs[event].count++;

    //if we've exceeded the size of our array, allocate a new larger one double the size
    if(manager.eventsubs[event].leng < manager.eventsubs[event].count)
    {
        void** newarr = malloc(sizeof(void*)*manager.eventsubs[event].leng*2);

        for(int i = 0; i < (manager.eventsubs[event].count - 1); i++)
        {
            newarr[i] = manager.eventsubs[event].elems[i];
        }

        free(manager.eventsubs[event].elems);
        manager.eventsubs[event].elems = (Ability**)newarr;
        manager.eventsubs[event].leng *= 2;
    }

    //add the new element
    manager.eventsubs[event].elems[manager.eventsubs[event].count - 1] = (Ability*)ability;

    return;
}

void Unsubscribe(Event event, void* ability)
{
    //loop through list to see if the provided ability is present
    for(int i = 0; i < manager.eventsubs[event].count; i++)
    {
        //if present, shift everything over 1 to remove the old instance
        if(manager.eventsubs[event].elems[i] == ability)
        {
            for(int j = i + 1; j < (manager.eventsubs[event].count-1); j++)
            {
                manager.eventsubs[event].elems[j-1] = manager.eventsubs[event].elems[i];
            }
            //set the last (now duplicate) element to null
            manager.eventsubs[event].elems[manager.eventsubs[event].count-1] = NULL;
            manager.eventsubs[event].count--;
        }
    }

    //if our list is now less than an eigth of our length, and we have more than 32 count
    if(manager.eventsubs[event].count < (manager.eventsubs[event].leng / 8) && manager.eventsubs[event].leng > 32)
    {
        //only quarter our list so we don't need to malloc more later.
        void** newarr = malloc(sizeof(void*) * (manager.eventsubs[event].leng / 4));

        for(int i = 0; i < (manager.eventsubs[event].count - 1); i++)
        {
            newarr[i] = manager.eventsubs[event].elems[i];
        }

        free(manager.eventsubs[event].elems);
        manager.eventsubs[event].elems = (Ability**)newarr;
    }

    return;
}

void FireEvent(Event event, node* info)
{
    int cont = manager.eventsubs[event].count;

    for(int i = 0; i < cont; i++)
    {
        manager.eventsubs[event].elems[i]->abilfunction(info, event, manager.eventsubs[event].elems[i]);
    }

    //TODO implement function I think?
}

void* ListSearch(node* info, char* query)
{
    void* ret = NULL;
    node* cur = info;

    while(1)
    {
        if(!(strcmp(cur->name, query)))
        {
            ret = cur->value;
        }
        if(cur->next == NULL)
        {
            break;
        }
    }

    return ret;
}

//TODO fix this god awful datatype system and find something more user friendly to free a node with a linked list as data
//TODO also test for memory leaks with valgrind
void AddNode(node** start, char* name, void* value, Datatype type)
{
    node* newnode = malloc(sizeof(node));

    newnode->type = type;

    int nameleng = strlen(name);
    
    newnode->name = malloc(sizeof(char) * (nameleng + 1));

    strncpy(newnode->name, name, nameleng);
    
    newnode->name[nameleng] = '\0';
    
    //allows passing of string literal as value when adding a node
    if(type == STR)
    {
        int valleng = strlen(value);
        newnode->value = malloc(sizeof(char) * (valleng + 1));
        strncpy(newnode->value, value, valleng);
        char* temp = (char *)newnode->value;
        temp[valleng] = '\0';
    }
    else
    {
        newnode->value = value;
    }
    
    newnode->next = NULL;
    node* cur = *start;

    //empty list case
    if(cur == NULL)
    {
        *start = newnode;
        return;
    }

    while(cur->next != NULL)
    {
        cur = cur->next;
    }
    
    cur->next = newnode;

    return;
}

void FreeList(node* start)
{
    node* current = start;
    node* next;

    while(current != NULL)
    {
        free(current->name);
        
        //if we're storing a linked list, take time to fully clear it as well
        if(current->type == LIST)
        {
            FreeList(current->value);
        }
        //delete normal info (including any arrays or single values)
        free(current->value);
        next = current->next;

        free(current);

        current = next;
    }
    
    return;
}

void NotifyAbility(node* info, Event event, Ability* ability)
{
    ability->abilfunction(info, event, ability);

    return;
}

void DEBUG_PrintList(node* start)
{
    printf("list print: \n");
    node* current = start;

    while(current != NULL)
    {
        printf("node name: %s\n", current->name);

        if(current->type == STR)
        {
            printf("\t string variable: %s\n", current->value);
        }
        else if(current->type == INT)
        {
            printf("\t int variable: %d\n", current->value);
        }

        current = current->next;
    }
}