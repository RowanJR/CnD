#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "map2d.h"
#include "tiles.h"

typedef enum {
    MAIN,
    LEFT,
    RIGHT,
    UP,
    DOWN
}loc;

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

Chunk* InitializeChunk(int seed)
{
    Chunk* retchunk = malloc(sizeof(Chunk));

    retchunk->entities = malloc(sizeof(Creature*) * 8);

    retchunk->arrayleng = 8;

    retchunk->numentities = 0;

    retchunk->cells = malloc(sizeof(unsigned short int) * CHUNK_AREA);

    //TODO proper seed generation
    for(int i = 0; i < CHUNK_AREA; i++)
    {
        retchunk->cells[i] = seed;
    }

    return retchunk;
}

Chunk* LoadChunk()
{
    //TODO actually load chunk
    return NULL;
}

void UnloadChunk(Chunk* chunk)
{
    for(int i = 0; i < chunk->numentities; i++)
    {
        free(chunk->entities[i]);
    }
    free(chunk->entities);
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
        ret->chunks[i] = InitializeChunk(0);
    }

    for(int i = 0; i < side; i++)
    {
        ret->upbuffer[i] = InitializeChunk(0);
        ret->downbuffer[i] = InitializeChunk(0);
        ret->leftbuffer[i] = InitializeChunk(0);
        ret->rightbuffer[i] = InitializeChunk(0);
    }

    return ret;
}

//TODO shifts work, but are not actually loading anything. Need to properly index and unload/load chunks to/from memory
void ShiftLoaded(Loaded_area* loaded, Vec shift)
{
    int side = (loaded->loaddistance * 2) + 1;

    //move to the right
    if(shift.x >= 0.5)
    {
        //unload last chunk in top and bottom array
        UnloadChunk(loaded->upbuffer[side - 1]);
        UnloadChunk(loaded->downbuffer[side - 1]);
        //move right side of array into right buffer
        //shift top and bottom buffers to the right one
        for(int i = 0; i < side; i++)
        {
            UnloadChunk(loaded->rightbuffer[i]);
        }
        for(int i = side - 1; i > 0; i--)
        {
            loaded->upbuffer[i] = loaded->upbuffer[i - 1];
            loaded->downbuffer[i] = loaded->downbuffer[i - 1];
        }
        for(int i = 0; i < side; i++)
        {
            loaded->rightbuffer[i] = loaded->chunks[side + (side * i) - 1];
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
            loaded->leftbuffer[i] = InitializeChunk(1);
        }
        loaded->upbuffer[0] = InitializeChunk(1);
        loaded->downbuffer[0] = InitializeChunk(1);
    }
    //move to the left
    else if(shift.x <= -0.5)
    {
        //unload first chunk in top and bottom array
        UnloadChunk(loaded->upbuffer[0]);
        UnloadChunk(loaded->downbuffer[0]);
        //move left side of array into left buffer
        //shift top and bottom buffers to the left one
        for(int i = 0; i < side; i++)
        {
            UnloadChunk(loaded->leftbuffer[i]);
            loaded->leftbuffer[i] = loaded->chunks[side * i];

            if(i < side - 1)
            {
                loaded->upbuffer[i] = loaded->upbuffer[i + 1];
                loaded->downbuffer[i] = loaded->downbuffer[i + 1];
            }
        }

        //shift contents of array left
        for(int i = 0; i < side; i++)
        {
            for(int j = 0; j < side; j++)
            {
                loaded->chunks[(i*side) + j] = loaded->chunks[(i*side) + j + 1];
            }
            //move right buffer into array
            loaded->chunks[(side - 1) + (i * side)] = loaded->rightbuffer[i];
        }
        
        //TODO load entire right buffer, and last element of up and down buffer
        for(int i = 0; i < side; i++)
        {
            loaded->rightbuffer[i] = InitializeChunk(1);
        }
        loaded->upbuffer[side - 1] = InitializeChunk(1);
        loaded->downbuffer[side - 1] = InitializeChunk(1);
    }

    //move up
    if(shift.y >= 0.5)
    {
        //unload last chunk in left and right array
        UnloadChunk(loaded->leftbuffer[side - 1]);
        UnloadChunk(loaded->rightbuffer[side - 1]);
        //move final row of array into up buffer
        //shift right and left array up one
        for(int i = 0; i < side; i++)
        {
            UnloadChunk(loaded->upbuffer[i]);
            loaded->upbuffer[i] = loaded->chunks[side * (side - 1) + i];

            if(i > 0)
            {
                loaded->leftbuffer[side - i] = loaded->leftbuffer[side - i - 1];
                loaded->rightbuffer[side - i] = loaded->rightbuffer[side - i - 1];
            }
        }

        //shift contents of array up
        for(int i = side - 1; i > 0; i--)
        {
            for(int j = 0; j < side; j++)
            {
                loaded->chunks[(i*side) + j] = loaded->chunks[((i - 1) * side) + j ];
            }
        }
        //move down buffer into array
        for(int i = 0; i < side; i++)
        {  
            loaded->chunks[i] = loaded->downbuffer[i];
        }

        //TODO load entire down buffer, and first element of right and left buffer
        for(int i = 0; i < side; i++)
        {
            loaded->downbuffer[i] = InitializeChunk(1);
        }
        loaded->rightbuffer[0] = InitializeChunk(1);
        loaded->leftbuffer[0] = InitializeChunk(1);
    }
    //move down
    else if(shift.y <= -0.5)
    {
        //unload first chunk in left and right array
        UnloadChunk(loaded->leftbuffer[0]);
        UnloadChunk(loaded->rightbuffer[0]);
        //move first row of array into down buffer
        for(int i = side - 1; i >= 0; i--)
        {
            UnloadChunk(loaded->downbuffer[i]);
            loaded->downbuffer[i] = loaded->chunks[i];
        }
        //shift right and left array down one
        for(int i = 0; i < side; i++)
        {
            loaded->leftbuffer[i] = loaded->leftbuffer[i + 1];
            loaded->rightbuffer[i] = loaded->rightbuffer[i + 1];
        }
        
        //shift contents of array down
        for(int i = 0; i < side; i++)
        {
            for(int j = 0; j < side; j++)
            {
                loaded->chunks[(i * side) + j] = loaded->chunks[((i + 1) * side) + j];
            }
        }
        //move up buffer into array
        for(int i = 0; i < side; i++)
        {
            loaded->chunks[side * (side - 1) + i] = loaded->upbuffer[i];
        }

        //TODO load entire up buffer, and last element of right and left buffer
        for(int i = 0; i < side; i++)
        {
            loaded->upbuffer[i] = InitializeChunk(1);
        }
        loaded->rightbuffer[side - 1] = InitializeChunk(1);
        loaded->leftbuffer[side - 1] = InitializeChunk(1);
    }

    return;
}

void UnloadLoaded(Loaded_area* loaded)
{
    int side = (loaded->loaddistance * 2) + 1;

    for(int i = 0; i < (side * side); i++)
    {
        UnloadChunk(loaded->chunks[i]);
    }

    free(loaded->chunks);

    for(int i = 0; i < side; i++)
    {
        UnloadChunk(loaded->downbuffer[i]);
        UnloadChunk(loaded->upbuffer[i]);
        UnloadChunk(loaded->rightbuffer[i]);
        UnloadChunk(loaded->leftbuffer[i]);
    }

    free(loaded->downbuffer);
    free(loaded->upbuffer);
    free(loaded->leftbuffer);
    free(loaded->rightbuffer);

    free(loaded);
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

Chunk* ReferenceChunk(Loaded_area* loaded, Chunk* current, Vec dir)
{
    int side = (loaded->loaddistance * 2) + 1;

    int index = 0;

    loc location = MAIN;

    //2D vector showing our current's location (indexed starting in the corner between the left and bottom buffers)
    Vec curloc = MakeVec(0, 0);

    dir.x = round(dir.x);
    dir.y = round(dir.y);

    for(int i = 0; i < side; i++)
    {
        if(loaded->rightbuffer[i] == current)
        {
            index = i;
            location = RIGHT;
            break;
        }
        if(loaded->leftbuffer[i] == current)
        {
            index = i;
            location = LEFT;
            break;
        }
        if(loaded->upbuffer[i] == current)
        {
            index = i;
            location = UP;
            break;
        }
        if(loaded->downbuffer[i] == current)
        {
            index = i;
            location = DOWN;
            break;
        }
        for(int j = 0; j < side; j++)
        {
            if(loaded->chunks[(i * side) + j] == current)
            {
                index = (i * side) + j;
                location = MAIN;
                break;
            }
        }
    }

    switch(location)
    {
        case MAIN:
            if(dir.x > side || dir.y > side || dir.y < -side || dir.x < -side)
            {
                return NULL;
            }
            curloc = MakeVec((index - (index/side)), (index/side));
            
            break;
        case RIGHT:
            if(dir.x > 0 || dir.y > side || dir.y < -side)
            {
                return NULL;
            }
            curloc = MakeVec(side + 1, index);
            break;
        case LEFT:
            if(dir.x < 0  || dir.y > side || dir.y < -side)
            {
                return NULL;
            }
            curloc = MakeVec(0, index);
            break;
        case UP:
            if(dir.y > 0 || dir.x > side || dir.x < -side)
            {
                return NULL;
            }
            curloc = MakeVec(index, side + 1);
            break;
        case DOWN:
            if(dir.y < 0 || dir.x > side || dir.x < -side)
            {
                return NULL;
            }
            curloc = MakeVec(index, 0);
            break;
        default:
            break;
    }
    
    return current;
}
