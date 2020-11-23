#ifndef ACTOR_H
#define ACTOR_H

#include "items.h"

typedef struct Actor {
    int y;
    int x;
    int health;
    char symbol;
    int speed;
    Armour *armour;
    Weapon *weapon;
    int aiType;
} Actor;

void moveActor(Actor *a, const int dy, const int dx);
void attackActor(Actor *a, Actor *d);

int giveItemToActor(Actor *a, Item *i);

Actor *createActor(const int y, const int x, const int health, const char symbol, const int speed, Weapon *weapon, Armour *armour, const int aiType);


#endif