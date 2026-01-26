#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "map.h"

//TODO listed also in map.h, but architechture needs reworking because map is circularly dependant on entities
//I think all void pointers are entity pointer placeholders

int CHUNK_WIDTH = 5;
int CHUNK_AREA = 25;

//used to quickly construct a coordinate
Coord MakeCoord(float x, float y)
{
    Coord newCoord;
    newCoord.x = x;
    newCoord.y = y;

    return newCoord;
}

//creates a chunk for the first time from a seed
//a value of 0 for a seed gives a flat dirt patch
Chunk* InitializeChunk(int seed)
{
    Chunk *newchunk = malloc(sizeof(Chunk));
    (*newchunk).cells = malloc(sizeof(unsigned short int) * CHUNK_AREA);
    // if 0 is given, generate a flat dirt test patch
    if(seed == 0)
    {
        for(int i = 0; i < CHUNK_AREA; i++)
        {
            (*newchunk).cells[i] = 1;
        }
    }
    else
    {
        //TODO implement proper seed chunk generation
        for(int i = 0; i < CHUNK_AREA; i++)
        {
            (*newchunk).cells[i] = seed;
        }
    }

    return newchunk;
}

//unloads a chunk from the heap/RAM
//TODO: save the chunk to disk when unloaded
//TODO: this should be a map function so we can clear the pointer to the chunk after unloading it
int UnloadChunk(Chunk* chunk)
{
    free(chunk->cells);
    //TODO: free entity positions/current entities, we're causing a memory leak right now
    free(chunk);

    return 0;
}

//returns a 1 dimensional index from a 2 dimensional square coordinate
// coordinates start in the bottom left corner of a cell, with y going up and
// x going right. coordinates are indexed at 0.
// returns -1 if an index is outside the chunk
int IndexChunk(Coord c)
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

int CellType(Chunk* chunk, Coord c)
{
    return (*chunk).cells[IndexChunk(MakeCoord(c.x, c.y))];
}

int ChangeCell(Chunk* chunk, Coord c, unsigned short int newcell)
{
    int index = IndexChunk(MakeCoord(c.x, c.y));
    if(index >= 0)
    {
        (*chunk).cells[index] = newcell;
    }

    return 0;
}

int IncrementCell(Chunk* chunk, Coord c)
{
    ChangeCell(chunk, c, CellType(chunk, c) + 1);

    return 0;
}

//primarily for debug purposes
Coord IndexToCoord(int ind)
{
    Coord newcoord;

    int y = CHUNK_WIDTH - (ind/CHUNK_WIDTH);
    int x = (ind%CHUNK_WIDTH) + 1;

    newcoord.y = y;
    newcoord.x = x;

    printf("coords: %d, %d\n", x, y);
    return newcoord;
}

//the map currently loads the entire map when called, which shouldn't be done; we should initialize all we need, like the map size, and only load the chunks by the player's position, with all other chunk pointers as "NULL"
// current implementation is for debugging
// TODO: fix above issue
Map* InitializeMap(int height, int width, int seed)
{
    int length = height * width;

    Map *self = malloc(sizeof(Map));
    (*self).height = height;
    (*self).width = width;
    (*self).chunks = malloc(sizeof(Chunk)*length);

    for(int i = 0; i < length; i++)
    {
        //TODO proper seed generation, currently just counting chunks
        (*self).chunks[i] = InitializeChunk(i + 1);
    }

    return self;
}

//TODO; save the map to disk when unloaded
void UnloadMap(Map* map)
{
    int length = map->height * map->width;

    for(int i = 0; i < length; i++)
    {
        if(map->chunks[i] != NULL)
        {
            UnloadChunk(map->chunks[i]);
        }
    }

    free(map->chunks);
    free(map);
}

void TranslateCrossChunks(Map* map, Chunk* originchunk, Chunk* targetchunk, Coord position)
{
    
}

//moves an entity in a direction according to the given vector.
// handles movement between chunks
// move vector is the x, y vector from the starting position
// collision is a boolean that determines if other entities or objects can stop the target's movement 
// if collision is false, the entity will just teleport to the new positon
void MoveEntity(Chunk* chunk, void* entity, Coord movevector, int collision)
{
    if(collision)
    {
        //TODO implement collision
    }
    else
    {
        for(int i=0; i < (*chunk).numentities; i++)
        {
            if((*chunk).entities[i].entity == entity)
            {
                (*chunk).entities[i].position.x += movevector.x;
                (*chunk).entities[i].position.y += movevector.y;

                //if we move to the next chunk, handle that
                if(((*chunk).entities[i].position.x > CHUNK_WIDTH) || 
                    ((*chunk).entities[i].position.y > CHUNK_WIDTH) ||
                    ((*chunk).entities[i].position.x <= 0) || 
                    ((*chunk).entities[i].position.y <= 0))
                {
                    
                }

                break;
            }
        }
    }

    return;
}

//prints a chunk's values to the console, semi-formatted
int DEBUG_PrintChunk(Chunk* chunk)
{
    for(int i = 0; i < CHUNK_AREA; i++)
    {
        printf("%hu ", (*chunk).cells[i]);
        if((i + 1) % CHUNK_WIDTH == 0)
        {
            printf("\n");
        }
    }
}

void DEBUG_PrintMap(Map* map)
{
    int index = 0;
    int chunkindex = 0;
    //loop over chunks in rows
    for(int maprow = map->height - 1; maprow >= 0; maprow--)
    {
        //do each row of the chunk
        for(int row = CHUNK_WIDTH; row >= 1; row--)
        {
            //loop over map columns
            for(int mapcol = 0; mapcol < map->width; mapcol++)
            {
                chunkindex = (maprow * map->width) + mapcol;\
                //ensure chunk is loaded
                if(map->chunks[chunkindex] != NULL)
                {    
                    //get individual cells
                    for(int col = 1; col <= CHUNK_WIDTH; col++)
                    {
                        index = IndexChunk(MakeCoord(col, row));
                        printf("%hu ", map->chunks[chunkindex]->cells[index]);
                    }
                }
                else
                {
                    for(int col = 1; col <= CHUNK_WIDTH; col++)
                    {
                        printf("N ");
                    }
                }
            }
            printf("\n");
        }
    }
}

int DEBUG_DrawLine(Chunk* chunk, Coord start, Coord end)
{
    //if line basically doesn't exist, just fill in the square its in
    if((start.x - end.x == 0) && (start.y - end.y == 0))
    {
        ChangeCell(chunk, start, 0);
        //IncrementCell(chunk, start);
        return 0;
    }

    float slope = (float)(end.y - start.y) / (float)(end.x - start.x);

    //for slopes less than 45 degrees
    if((slope <= 1) && (slope >= -1))
    {
        int xval = 0;
        int yval = 0;
        int final = 0;
        if(((slope >= 0) && (end.y >= start.y)) || ((slope < 0) && (end.y < start.y)))
        {
            xval = start.x;
            final = end.x;
        }
        else
        {
            xval = end.x;
            final = start.x;
        }

        while(xval <= final)
        {

            yval = (int)round(start.y + (slope * (xval - start.x)));

            ChangeCell(chunk, MakeCoord(xval, yval), 0);
            //IncrementCell(chunk, MakeCoord(xval, yval));

            xval++;
        }
    }
    //for slopes above 45 degrees
    else
    {
        int xval = 0;
        int yval = 0;
        int final = 0;
        if(((slope >= 0) && (end.x >= start.x)) || ((slope < 0) && (end.x < start.x)))
        {
            yval = start.y;
            final = end.y;
        }
        else
        {
            yval = end.y;
            final = start.y;
        }
    
        while(yval <= final)
        {
            xval = (int)round(start.x + ((1/slope) * (yval - start.y)));

            ChangeCell(chunk, MakeCoord(xval, yval), 0);
            //IncrementCell(chunk, MakeCoord(xval, yval));

            yval ++;
        }
    }

    return 0;
}

int DEBUG_DrawCircle(Chunk* chunk, Coord c, float radius)
{
    int xoffset = 0;
    int yoffset = radius;
    int notfirst = 0;

    //we loop over one eigth of the circle, which we then draw a line down to its opposite
    //this means we don't draw any cells on the y axis, so we can calculate it linearly
    while((xoffset) < (yoffset))
    {
        yoffset = (int)round(sqrt(radius*radius - (xoffset)*(xoffset)));

        //first arc
        /*
        IncrementCell(chunk, MakeCoord(c.x+xoffset, c.y+yoffset));

        //other octants, mirrored by offset (rotated clockwise)
        IncrementCell(chunk, MakeCoord(c.x+yoffset, c.y+xoffset));
        IncrementCell(chunk, MakeCoord(c.x+yoffset, c.y-xoffset));
        IncrementCell(chunk, MakeCoord(c.x+xoffset, c.y-yoffset));
        IncrementCell(chunk, MakeCoord(c.x-xoffset, c.y-yoffset));
        IncrementCell(chunk, MakeCoord(c.x-yoffset, c.y-xoffset));
        IncrementCell(chunk, MakeCoord(c.x-yoffset, c.y+xoffset));
        IncrementCell(chunk, MakeCoord(c.x-xoffset, c.y+yoffset));*/

        //using lines (greatly reduces overdraw)
        //this line prevents us from drawing over the center column twice
        if(notfirst == 1)
        {
            DEBUG_DrawLine(chunk, MakeCoord(c.x+xoffset, c.y+yoffset), MakeCoord(c.x+xoffset, c.y-yoffset));
        }
        DEBUG_DrawLine(chunk, MakeCoord(c.x+yoffset, c.y+xoffset), MakeCoord(c.x+yoffset, c.y-xoffset));  
        DEBUG_DrawLine(chunk, MakeCoord(c.x-xoffset, c.y-yoffset), MakeCoord(c.x-xoffset, c.y+yoffset));
        DEBUG_DrawLine(chunk, MakeCoord(c.x-yoffset, c.y+xoffset), MakeCoord(c.x-yoffset, c.y-xoffset));

        xoffset++;
        notfirst = 1;
    }

    //if we have a small cirle, just fill in the center cell
    if(radius < 1.5)
    {
        ChangeCell(chunk, c, 0);
        //IncrementCell(chunk, c);
    }
    
    return 0;
}

int DEBUG_DrawSquare(Chunk* chunk, Coord c, float width)
{
    

    return 0;
}

//colaboration with stack overflow (stolen code) to assist in determining if a point lies within a triangle
float TriSign(Coord p1, Coord p2, Coord p3)
{
    return((p1.x - p3.x) * (p2.y - p3.y) -(p2.x - p3.x) * (p1.y - p3.y));
}

//draws a cone, as described in the dnd rules
// default cone is 90 degrees, width factor multiplies or reduces this
int DEBUG_DrawCone(Chunk* chunk, Coord start, Coord end, float width_factor)
{
    //find the length of the cone
    float xlen = fmax((start.x - end.x), (end.x - start.x));
    float ylen = fmax((start.y - end.y), (end.y - start.y));
    float length = sqrt((xlen * xlen) + (ylen * ylen));

    //this is used in the next section; slope of the end of the cone, i.e. inverse th cone's slope
    float slope = (float)(ylen) / (float)(xlen);

    //find a bounding box so we only have to loop over a small area
    // follow the perpendicular slope to the end at the end of the cone
    //THIS CAME TO ME IN A DREAM, DO NoT ASK ME HOW IT WORKS, BUT IT GETS THE RISE FROM THE SLOPE AND THE HYPOTONUSE LENGTH
    float rise = sqrt(((length*width_factor)*(length*width_factor))/(1+(slope * slope))); 
    float run = sqrt(((length*width_factor)*(length*width_factor)) - (rise * rise));
    Coord edge1;
    Coord edge2;
    if(start.y <= end.y && start.x <= end.x)
    {
        edge1 = MakeCoord(end.x - run, end.y + rise);
        edge2 = MakeCoord(end.x + run, end.y - rise);
    }
    else if (start.y <= end.y && start.x >= end.x)
    {
        edge1 = MakeCoord(end.x - run, end.y - rise);
        edge2 = MakeCoord(end.x + run, end.y + rise);
    }
    else if ((start.y >= end.y) && (start.x >= end.x))
    {
        edge1 = MakeCoord(end.x + run, end.y - rise);
        edge2 = MakeCoord(end.x - run, end.y + rise);
    }
    else
    {
        edge1 = MakeCoord(end.x + run, end.y + rise);
        edge2 = MakeCoord(end.x - run, end.y - rise);
    }
    

    //bounding box corners, the furthest they get 
    // these are ugly, but oh well
    float xmin = floor(fmin(fmin(edge1.x, edge2.x), fmin(start.x, end.x)));
    float xmax = ceil(fmax(fmax(edge1.x, edge2.x), fmax(start.x, end.x)));
    float ymin = floor(fmin(fmin(edge1.y, edge2.y), fmin(start.y, end.y)));
    float ymax = ceil(fmax(fmax(edge1.y, edge2.y), fmax(start.y, end.y)));

    float curx = xmin;
    float cury = ymin;

    //stack overflow collaboration
    int has_neg, has_pos = 0;
    float d1, d2, d3 = 0;

    while(cury <= ymax)
    {
        while(curx <= xmax)
        {
            //don't ask how this works, stack overflow gave me this code
            d1 = TriSign(MakeCoord(curx, cury), start, edge1);
            d2 = TriSign(MakeCoord(curx, cury), edge1, edge2);
            d3 = TriSign(MakeCoord(curx, cury), edge2, start);

            has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
            has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

            if(!(has_neg && has_pos))
            {
                ChangeCell(chunk, MakeCoord(curx, cury), 0);
            }

            curx++;
        }
        curx = xmin;
        cury++;
    }

    /*ChangeCell(chunk, start, 2);
    ChangeCell(chunk, end, 3);
    ChangeCell(chunk, edge1, 4);
    ChangeCell(chunk, edge2, 5);*/

    return 0;
}

//checks line of sight between two points. Returns 1 if no obstructive tiles or entities are between the points, and 0 otherwise.
int CheckLineOfSight(Chunk* chunk, Coord start, Coord end)
{
    //if line basically doesn't exist, just fill in the square its in
    if((start.x - end.x == 0) && (start.y - end.y == 0))
    {
        
        return 0;
    }

    float slope = (float)(end.y - start.y) / (float)(end.x - start.x);

    //for slopes less than 45 degrees
    if((slope <= 1) && (slope >= -1))
    {
        int xval = 0;
        int yval = 0;
        int final = 0;
        if(((slope >= 0) && (end.y >= start.y)) || ((slope < 0) && (end.y < start.y)))
        {
            xval = start.x;
            final = end.x;
        }
        else
        {
            xval = end.x;
            final = start.x;
        }

        while(xval <= final)
        {

            yval = (int)round(start.y + (slope * (xval - start.x)));

            ChangeCell(chunk, MakeCoord(xval, yval), 0);
            //IncrementCell(chunk, MakeCoord(xval, yval));

            xval++;
        }
    }
    //for slopes above 45 degrees
    else
    {
        int xval = 0;
        int yval = 0;
        int final = 0;
        if(((slope >= 0) && (end.x >= start.x)) || ((slope < 0) && (end.x < start.x)))
        {
            yval = start.y;
            final = end.y;
        }
        else
        {
            yval = end.y;
            final = start.y;
        }
    
        while(yval <= final)
        {
            xval = (int)round(start.x + ((1/slope) * (yval - start.y)));

            ChangeCell(chunk, MakeCoord(xval, yval), 0);
            //IncrementCell(chunk, MakeCoord(xval, yval));

            yval ++;
        }
    }

    return 0;
}

//returns an array of entities for each cell that a sphere overlaps within the current chunk. 
// The first entity in the array is not a real entity; instead it's position's coord's x value is the length of the array.
//  ensure return value is freed imediately after use.
Entity_Position* CircleCollision(Chunk* chunk, Coord origin, float radius)
{
    //loop over all entities in the chunk and see if they fall within the radius of the circle. it does not check line of sight
    for(int i = 0; i < chunk->numentities; i++)
    {
        
    }
}

