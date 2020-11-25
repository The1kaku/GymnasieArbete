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
    int attackerDamage, defenderDamage;
    const int *atk, *def;
    atk = getItemAtk(a->weapon);
    def = getItemDef(d->armour);

    if (atk == NULL || def == NULL)
        return;

    attackerDamage = 0;
    attackerDamage += atk[ATK_IMPACT] / def[DEF_IMPACT];
    attackerDamage += atk[ATK_PIERCE] / def[DEF_PIERCE];
    attackerDamage += atk[ATK_SLASH] / def[DEF_SLASH];

    defenderDamage = 0;
    defenderDamage += def[DEF_THORNS];

    a->health -= defenderDamage;
    d->health -= attackerDamage;

    if (defenderDamage > 0)
        addInfo("%c attacked %c for %d and took %d thorns.\n", a->symbol, d->symbol, attackerDamage, defenderDamage);
    else 
        addInfo("%c attacked %c for %d", a->symbol, d->symbol, attackerDamage);
}

int 
giveItemToActor(Actor *a, ItemID id)
{
    if (getItemCap(id, CAP_ATTACK) && promptPlayer("Swap your current weapon? (y or n)")  == 'y') {
        if (putItemInInventory(a, a->weapon) > 0) {
            addInfo("Put current weapon in inventory\n");
        } else {
            addInfo("Could not put current weapon in inventory\n");
            return -1;
        }
        return a->weapon = id;
    } else if (getItemCap(id, CAP_DEFEND) && promptPlayer("Swap your current armour? (y or n)") == 'y') {
        if (putItemInInventory(a, a->armour) > 0) {
            addInfo("Put current armour in inventory\n");
        } else {
            addInfo("Could not put current weapon in inventory\n");
            return -1;
        }
        return a->armour = id;
    } else {
        return putItemInInventory(a, id);
    }
}

int
putItemInInventory(Actor *a, ItemID id)
{
    int i;
    if (id == 0)
        return -1;
    for (i = 0; i < a->inventorySize && a->inventory[i] > 0; i++)
        ; // * iterating
    if (i == a->inventorySize)
        return -1;
    return a->inventory[i] = id;
}

Actor *
createActor(const int y, const int x, const int health, const char symbol, const int speed, const ItemID weapon, const ItemID armour, const int aiType, const unsigned inventorySize)
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
    a->savedTime = 0;
    a->inventorySize = inventorySize;
    a->inventory = (ItemID *) calloc(inventorySize, sizeof(ItemID));
    if (a->inventory == NULL)
        return NULL;

    return a;
}