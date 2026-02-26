#ifndef MAP2D_H_
#define MAP2D_H_

#include "tiles.h"

//TODO figure out dependancies and architechture; map.c is a bit circularly dependant. most void pointers here and map.c are places holders for cross-dependancies

extern int CHUNK_WIDTH;
extern int CHUNK_AREA;

typedef struct{
    float x;
    float y;
}Vec;

Vec MakeVec(float x, float y);

typedef struct{
    //TODO figure out dependancies
    void *entity;
    Vec position;
}Entity_Position;

//a single chunk
typedef struct{
    unsigned short int *cells;
    Entity_Position *entities;
    int numentities;
}Chunk;

// returns a 1 dimensional index from a 2 dimensional square coordinate
//  coordinates start in the bottom left corner of a cell, with y going up and
//  x going right. coordinates are indexed at 0.
//  returns -1 if an index is outside the chunk
int IndexChunk(Vec c);

// converts back from chunk index to Vec from chunk
Vec IndexToCoord(int ind);

// chunks loaded around the player
//  chunks are partially loaded into the buffer zone from disc, then fully loaded when the player nears them
//  fully loaded chunks that enter the buffer remain loaded until they leave it
typedef struct{
    unsigned short int loaddistance; // how far the distance extends in the cardinal directions from the player. Buffer space extends an extra square
    Chunk** chunks;
    Chunk** buffer; // holds partially loaded chunks before they're unloaded/fully loaded
}Loaded_area;



#endif