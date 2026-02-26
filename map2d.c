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

int CHUNK_WIDTH = 5;
int CHUNK_AREA = 25;

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

