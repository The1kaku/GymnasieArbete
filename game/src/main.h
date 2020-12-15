#ifndef MAIN_H
#define MAIN_H

#include "level.h"
#include "actor.h"

#define MONSTER_COUNT 100
#define ITEM_COUNT 100

Level level;
Actor *player;

Actor *monsters[MONSTER_COUNT];
Actor *undead;

GroundItem *groundItems[ITEM_COUNT];

void putItemOnGround(const ItemID id, const Actor *a);

#endif