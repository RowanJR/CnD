#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "turns.h"
#include "entities.h"
#include "ability_system.h"

//number of 6 second turns in some amount of time
int MINUTE = 10;
int HOUR = 600;
int DAY = 14400;
int WEEK = 100800;
int YEAR = 5256000;

//global time; number of turns since start of the world.
long worldtime;

