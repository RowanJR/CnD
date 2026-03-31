#ifndef MAP2D_H_
#define MAP2D_H_

#include "tiles.h"

extern int CHUNK_WIDTH;
extern int CHUNK_AREA;

typedef struct{
    float x;
    float y;
}Vec;

Vec MakeVec(float x, float y);

struct Entity;
struct Chunk;

//position represents the bottom left cell of the creature, no matter the number of cells it occupies
typedef struct{
    struct Entity *entity;
    struct Chunk* currentchunk;
    float height;
    float width;
    Vec position;
    int occupies_space;
}Creature;

//a single chunk
typedef struct{
    unsigned short int *cells;
    Creature **entities;
    int numentities;
    int arrayleng;
}Chunk;

// returns a 1 dimensional index from a 2 dimensional square coordinate
//  coordinates start in the bottom left corner of a cell, with y going up and
//  x going right. coordinates are indexed at 0.
//  returns -1 if an index is outside the chunk
int IndexChunk(Vec c);

// converts back from chunk index to Vec from chunk
Vec IndexToCoord(int ind);

Chunk* InitializeChunk();

Chunk* LoadChunk();

void UnloadChunk(Chunk* chunk);

//returns 1 or 0 (true or false) depending on whether the specified square is occupied
int IsOccupied(Chunk* chunk, Vec space);

void AddEntity(Chunk* chunk, Creature* entity);

void RemoveEntity(Chunk* chunk, Creature* entity);

// chunks loaded around the player
//  chunks are partially loaded into the buffer zone from disc, then fully loaded when the player nears them
//  fully loaded chunks that enter the buffer remain loaded until they leave it
//  chunks are indexed from the bottom left 
typedef struct{
    unsigned short int loaddistance; // how far the distance extends in the cardinal directions from the player. Buffer space extends an extra square
    Chunk** chunks; // current square is the center of the 2d square array
    //buffers are indexed left to right and bottom to top for horizontal and vertical respectively
    Chunk** upbuffer; // holds partially loaded chunks before they're unloaded/fully loaded
    Chunk** downbuffer; // holds partially loaded chunks before they're unloaded/fully loaded
    Chunk** leftbuffer; // holds partially loaded chunks before they're unloaded/fully loaded
    Chunk** rightbuffer; // holds partially loaded chunks before they're unloaded/fully loaded
}Loaded_area;

Loaded_area* InitializeLoaded(int loaddist);

//shifts the current loaded area by up to one chunk in the direction specified by the shift vector (max of 1 chunk despite whatever is passed through)
void ShiftLoaded(Loaded_area* loaded, Vec shift);

//directly places an enemy in a chunk, returns 1 if space is occupied, 0 if all goes well
int PlaceEntity(Chunk* fromchunk, Chunk* tochunk, Creature* entity, Vec location);

//moves an entity along vector to set location, returns 1 if space is occupied, 0 if all goes well
int MoveEntity(Chunk* chunk, Creature* entity, Vec direction);

#endif