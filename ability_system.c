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
    manager.eventsubs[event].elems[manager.eventsubs[event].count - 1] = ability;

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
        //TODO test this, I think I'm calling the function pointer wrong
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
void AddNode(node** start, char* name, void* value, Datatype type)
{
    node* newnode = malloc(sizeof(node));

    newnode->name = malloc(sizeof(char) * strlen(name));
    strcpy(name, newnode->name);

    newnode->value = value;

    newnode->next = NULL;

    node* cur = *start;

    //empty list case
    if(cur == NULL)
    {
        *start = newnode;
        return;
    }

    while(1)
    {
        if(cur->next == NULL)
        {
            break;
        }
    }
    
    cur->next = newnode;

    cur->type = type;

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
        //delete normal info (including any arrays or single values) (don't deleting entities)
        else if(current->type != ENTITY)
        {
            free(current->value);
        }

        next = current->next;

        free(current);

        current = next;
    }
    
    free(start->next);

    return;
}

