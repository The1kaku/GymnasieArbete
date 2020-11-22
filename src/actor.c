#include "actor.h"

#include <stdlib.h>

void 
moveActor(Actor *a, const int dy, const int dx)
{
    a->y += dy;
    a->x += dx;
}

Actor *
createActor(const int y, const int x, const int health, const char symbol, const int speed)
{
    Actor *a;
    a = (Actor *) malloc(sizeof(Actor));
    a->y = y;
    a->x = x;
    a->health = health;
    a->symbol = symbol;
    a->speed = speed;

    return a;
}