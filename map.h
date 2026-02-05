#ifndef MAP_H_
#define MAP_H_

#include "tiles.h"
//#include "entities.h"
//TODO figure out dependancies and architechture; map.c is a bit circularly dependant. most void pointers here and map.c are places holders for cross-dependancies

extern int CHUNK_WIDTH;
extern int CHUNK_AREA;

typedef struct{
    float x;
    float y;
}Coord;

typedef struct{
    //TODO figure out dependancies
    void *entity;
    Coord position;
}Entity_Position;

//a single chunk
typedef struct{
    unsigned short int *cells;
    Entity_Position *entities;
    int numentities;
}Chunk;

//entire map of an area
// unloaded chunks are NULL pointers
typedef struct{
    int width;
    int height;
    Chunk **chunks;
}Map;

Coord MakeCoord(float x, float y);

Chunk* InitializeChunk(int seed);

int UnloadChunk(Chunk* chunk);

int IndexChunk(Coord c);

Coord IndexToCoord(int ind);

Map* InitializeMap(int height, int width, int seed);

void UnloadMap(Map* map);

void TranslateCrossChunks(Map* map, Chunk* originchunk, Chunk* targetchunk, Coord position);

//TODO figure out dependancies
void MoveEntity(Chunk* chunk, void* entity, Coord movevector, int collision);

int CellType(Chunk* chunk, Coord c);

int ChangeCell(Chunk* chunk, Coord c, unsigned short int newcell);

int IncrementCell(Chunk* chunk, Coord c);

int DEBUG_PrintChunk(Chunk* chunk);

void DEBUG_PrintMap(Map* map);

int DEBUG_DrawLine(Chunk* chunk, Coord start, Coord end);

int DEBUG_DrawCircle(Chunk* chunk, Coord origin, float radius);

int DEBUG_DrawSquare(Chunk* chunk, Coord origin, float width);

float TriSign(Coord p1, Coord p2, Coord p3);

int DEBUG_DrawCone(Chunk* chunk, Coord start, Coord end, float width_factor);

int CheckLineOfSight(Chunk* chunk, Coord start, Coord end);

Entity_Position* CircleCollision(Chunk* chunk, Coord origin, float radius);


#endif