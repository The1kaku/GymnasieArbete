#include "actor.h"

#include <stdlib.h>

void 
moveActor(Actor *a, const int dy, const int dx)
{
    a->y += dy;
    a->x += dx;
}

int 
giveItemToActor(Actor *a, Item *i)
{
    switch(i->type) {
        case ITEM_TYPE_WEAPON:
            a->weapon = (Weapon *) i->item;
            return ITEM_TYPE_WEAPON;
        case ITEM_TYPE_ARMOUR:
            a->armour = (Armour *) i->item;
            return ITEM_TYPE_ARMOUR;
        default:
            return 0;
    }
}

Actor *
createActor(const int y, const int x, const int health, const char symbol, const int speed, Weapon *weapon, Armour *armour)
{
    Actor *a;
    a = (Actor *) malloc(sizeof(Actor));
    a->y = y;
    a->x = x;
    a->health = health;
    a->symbol = symbol;
    a->speed = speed;
    a->armour = armour;
    a->weapon = weapon;

    return a;
}