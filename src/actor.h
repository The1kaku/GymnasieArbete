#ifndef ACTOR_H
#define ACTOR_H

#include "items.h"

typedef struct Actor {
    int y;
    int x;
    int health;
    char symbol;
    int speed;
    int aiType;
    ItemID weapon;
    ItemID armour; 
    unsigned inventorySize;
    ItemID *inventory;
    unsigned savedTime;
} Actor;

void moveActor(Actor *a, const int dy, const int dx);
void attackActor(Actor *a, Actor *d);

int giveItemToActor(Actor *a, ItemID id);
int putItemInInventory(Actor *a, ItemID id);

Actor *createActor(const int y, const int x, const int health, const char symbol, const int speed, const ItemID weapon, const ItemID armour, const int aiType, const unsigned inventorySize);


#endif