#include "actor.h"

#include <stdlib.h>

#include "display.h"

void 
moveActor(Actor *a, const int dy, const int dx)
{
    a->y += dy;
    a->x += dx;
}

void
attackActor(Actor *a, Actor *d)
{
    int damage;
    damage = 0;
    damage += a->weapon->impact / d->armour->impact;
    damage += a->weapon->pierce / d->armour->pierce;
    damage += a->weapon->slash / d->armour->slash;
    d->health -= damage;
}

int 
giveItemToActor(Actor *a, Item *i)
{
    switch(i->type) {
    case ITEM_TYPE_WEAPON:
        if (promptPlayer("Swap your current weapon? (y or n)") == 'y') {
            a->weapon = (Weapon *) i->item;
        } else {
            return putItemInInventory(a, i);
        }
        return ITEM_TYPE_WEAPON;
    case ITEM_TYPE_ARMOUR:
        if (promptPlayer("Swap your current armour? (y or n)") == 'y') {
            a->armour = (Armour *) i->item;
        } else {
            return putItemInInventory(a, i);
        }
        return ITEM_TYPE_ARMOUR;
    default:
        return 0;
    }
}

int
putItemInInventory(Actor *a, Item *i)
{
    for (int j = 0; j < INVENTORY_SIZE; j++) {
        if (a->inventory[j] == NULL) {
            a->inventory[j] = i;
            return ITEM_TYPE_INVENTORY;
        }
    }
    return 0;
}

Actor *
createActor(const int y, const int x, const int health, const char symbol, const int speed, Weapon *weapon, Armour *armour, const int aiType)
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
    a->aiType = aiType;

    return a;
}