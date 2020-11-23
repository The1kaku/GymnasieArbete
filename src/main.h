#ifndef MAIN_H
#define MAIN_H

#include "level.h"
#include "actor.h"

#define MONSTER_COUNT 10
#define ITEM_COUNT 10

Level level;
Actor *player;

Actor *monsters[MONSTER_COUNT];
Actor *undead;

Item *groundItems[ITEM_COUNT];

#endif