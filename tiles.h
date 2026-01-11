#ifndef TILES_H_
#define TILES_H_

typedef enum {
    CHASM,
    GRASS,
    DIRT,
    STONE,
    SHALLOW_WATER,
    DEEP_WATER,
    WOOD_WALL
}tiles;
/*
extern int nontraversible[] = {
    0, 5, 6
};

extern int blocksview[] = {
    6
};

extern int difficultterrain[] = {
    4
};*/

int IsNonTraversible(int tile);

int BlocksView(int tile);

int IsDifficultTerrain(int tile);

#endif // TILES_H_