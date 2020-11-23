#ifndef LEVEL_H
#define LEVEL_H

#include "actor.h"

#define LEVEL_HEIGHT 100
#define LEVEL_WIDTH 100

typedef char **Level;
typedef int **Room;

Level readLevelFromFile(const char *f);

int **getRoomBordersFromActor(const Level l, const Actor *a);
int **getRoomBordersFromPoint(const Level l, const int y, const int x);

#endif