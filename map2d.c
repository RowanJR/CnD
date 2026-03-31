#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "map2d.h"
#include "tiles.h"

Vec MakeVec(float x, float y)
{
    Vec newCoord;
    newCoord.x = x;
    newCoord.y = y;

    return newCoord;
}

int CHUNK_WIDTH = 10;
int CHUNK_AREA = 100;

int IndexChunk(Vec c)
{
    //ignore out of bounds placements
    if(c.x > CHUNK_WIDTH || c.y > CHUNK_WIDTH)
    {
        return -1;
    }
    if(c.x <= 0 || c.y <= 0)
    {
        return -1;
    }

    int index = (CHUNK_WIDTH - (int)round(c.y)) * CHUNK_WIDTH + (int)round(c.x - 1);

    return index;
}

Vec IndexToCoord(int ind)
{
    Vec newcoord;

    int y = CHUNK_WIDTH - (ind/CHUNK_WIDTH);
    int x = (ind%CHUNK_WIDTH) + 1;

    newcoord.y = y;
    newcoord.x = x;

    printf("coords: %d, %d\n", x, y);
    return newcoord;
}

Chunk* InitializeChunk()
{
    Chunk* retchunk = malloc(sizeof(Chunk));

    retchunk->entities = malloc(sizeof(Creature*) * 8);

    retchunk->arrayleng = 8;

    retchunk->numentities = 0;

    retchunk->cells = malloc(sizeof(unsigned short int) * CHUNK_AREA);

    return retchunk;
}

Chunk* LoadChunk()
{
    //TODO actually load chunk
    return InitializeChunk();
}

void UnloadChunk(Chunk* chunk)
{
    free(chunk->cells);
    free(chunk);
    return;
}

int IsOccupied(Chunk* chunk, Vec space)
{
    for(int i = 0; i < chunk->numentities; i++)
    {
        if(chunk->entities[i]->occupies_space)
        {
            float maxx = chunk->entities[i]->position.x + chunk->entities[i]->width - 1;
            float maxy = chunk->entities[i]->position.y + chunk->entities[i]->height - 1;

            if(space.x <= maxx && space.x >= chunk->entities[i]->position.x)
            {
                if(space.y <= maxy && space.y >= chunk->entities[i]->position.y)
                {
                    return 1;
                }
            }
        }
    }

    return 0;
}

void AddEntity(Chunk* chunk, Creature* entity)
{
    chunk->numentities++;

    if(chunk->numentities > chunk->arrayleng)
    {
        Creature** newarr = malloc(sizeof(Creature*) * chunk->arrayleng * 2);
        for(int i = 0; i < (chunk->arrayleng); i++)
        {
            newarr[i] = chunk->entities[i];
        }

        free(chunk->entities);
        chunk->entities = newarr;
        chunk->arrayleng *= 2;
    }

    chunk->entities[chunk->numentities - 1] = entity;
}

void RemoveEntity(Chunk* chunk, Creature* entity)
{
    //loop through list to see if the provided ability is present
    for(int i = 0; i < chunk->numentities; i++)
    {
        //if present, shift everything over 1 to remove the old instance
        if(chunk->entities[i] == entity)
        {
            for(int j = i + 1; j < chunk->numentities; j++)
            {
                chunk->entities[j-1] = chunk->entities[j];
            }
            //set the last (now duplicate) element to null
            chunk->entities[chunk->numentities-1] = NULL;
            chunk->numentities--;
        }
    }

    //if our list is now less than an eigth of our length, and we have more than 32 count
    if(chunk->numentities < (chunk->arrayleng / 8) && chunk->arrayleng > 32)
    {
        //only quarter our list so we don't need to malloc more later.
        void** newarr = malloc(sizeof(void*) * (chunk->arrayleng / 4));

        for(int i = 0; i < chunk->numentities; i++)
        {
            newarr[i] = chunk->entities[i];
        }

        free(chunk->entities);
        chunk->entities = (Creature**)newarr;
        chunk->arrayleng /= 4;
    }

    return;
}  

Loaded_area* InitializeLoaded(int loaddist)
{
    Loaded_area* ret = malloc(sizeof(Loaded_area));

    ret->loaddistance = loaddist;

    int side = (loaddist * 2) + 1;

    ret->chunks = malloc(sizeof(Chunk**) * (side * side));

    ret->upbuffer = malloc(sizeof(Chunk**) * side);
    ret->downbuffer = malloc(sizeof(Chunk**) * side);
    ret->leftbuffer = malloc(sizeof(Chunk**) * side);
    ret->rightbuffer = malloc(sizeof(Chunk**) * side);

    for(int i = 0; i < (side * side); i++)
    {
        //TODO actually load the right chunks
        ret->chunks[i] = LoadChunk();
    }

    for(int i = 0; i < side; i++)
    {
        ret->upbuffer[i] = LoadChunk();
        ret->downbuffer[i] = LoadChunk();
        ret->leftbuffer[i] = LoadChunk();
        ret->rightbuffer[i] = LoadChunk();
    }

    return ret;
}

void ShiftLoaded(Loaded_area* loaded, Vec shift)
{
    int side = (loaded->loaddistance * 2) + 1;

    //move to the right
    if(shift.x >= 0.5)
    {
        //move right side of array into right buffer
        //shift top and bottom buffers
        for(int i = 0; i < side; i++)
        {
            UnloadChunk(loaded->rightbuffer[i]);
            loaded->rightbuffer[i] = loaded->chunks[(side - 1) * i];

            loaded->upbuffer[side - i] = loaded->upbuffer[side - i - 1];
            loaded->downbuffer[side - i] = loaded->downbuffer[side - i - 1];
        }

        //shift contents of array right
        for(int i = 0; i < side; i++)
        {
            for(int j = side - 1; j > 0; j--)
            {
                loaded->chunks[(i*side) + j] = loaded->chunks[(i*side) + j - 1];
            }
            //move left buffer into array
            loaded->chunks[i*side] = loaded->leftbuffer[i];
        }
        
        //TODO load entire left buffer, and first element of up and down buffer
        for(int i = 0; i < side; i++)
        {
            loaded->leftbuffer[i] = LoadChunk();
        }
        loaded->upbuffer[0] = LoadChunk();
        loaded->downbuffer[0] = LoadChunk();
    }
    //move to the left
    else if(shift.x <= -0.5)
    {
        
    }

    //move up
    if(shift.y >= 0.5)
    {

    }
    //move down
    else if(shift.y <= -0.5)
    {
        
    }

    return;
}

int PlaceEntity(Chunk* fromchunk, Chunk* tochunk, Creature* entity, Vec location)
{
    if(entity->occupies_space)
    {
        if(IsOccupied(tochunk, location))
        {
            return 1;
        }
    }

    entity->position.x = location.x;
    entity->position.y = location.y;

    if(fromchunk != tochunk)
    {
        RemoveEntity(fromchunk, entity);
        AddEntity(tochunk, entity);     
    }

    return 0;
}

int MoveEntity(Chunk* chunk, Creature* entity, Vec direction)
{
    Vec nextpos;
    nextpos.x = entity->position.x + direction.x;
    nextpos.y = entity->position.y + direction.y;

    Chunk* tochunk = chunk;

    //handle movement onto another chunk
    if(nextpos.x > CHUNK_WIDTH || nextpos.y > CHUNK_WIDTH)
    {
        //TODO implement obtaining adjacent chunks
    }

    if(PlaceEntity(chunk, tochunk, entity, nextpos))
    {
        return 1;
    }

    return 0;
}


