#include "actor.h"

#include <stdlib.h>

#include "display.h"

int 
moveActor(Actor *a, const int dy, const int dx)
{
    a->y += dy;
    a->x += dx;
    return a->speed;
}

int
attackActor(Actor *a, Actor *d)
{
    int attackerDamage, defenderDamage;
    const int *atk, *def;
    atk = getItemAtk(a->weapon);
    def = getItemDef(d->armour);

    if (atk == NULL) {
        addInfo("%c failed to attack %c.\n", a->symbol, d->symbol);
        return 0;
    }

    attackerDamage = 0;
    attackerDamage += atk[ATK_IMPACT] / ((def == NULL) ? 1 : def[DEF_IMPACT]);
    attackerDamage += atk[ATK_PIERCE] / ((def == NULL) ? 1 : def[DEF_PIERCE]);
    attackerDamage += atk[ATK_SLASH] / ((def == NULL) ? 1 : def[DEF_SLASH]);

    defenderDamage = 0;
    defenderDamage += ((def == NULL) ? 0 : def[DEF_THORNS]);

    a->health -= defenderDamage;
    d->health -= attackerDamage;

    if (d->health > 0) {
        if (defenderDamage > 0)
            addInfo("%c attacked %c for %dHP and took %dHP thorns.\n", a->symbol, d->symbol, attackerDamage, defenderDamage);
        else 
            addInfo("%c attacked %c for %dHP.\n", a->symbol, d->symbol, attackerDamage);
    } else {
        if (defenderDamage > 0)
            addInfo("%c killed %c but took %dHP thorns!\n", a->symbol, d->symbol, attackerDamage, defenderDamage);
        else 
            addInfo("%c killed %c!\n", a->symbol, d->symbol, attackerDamage);
    }
    
    
    return atk[ATK_SPEED];
}

int 
giveItemToActor(Actor *a, ItemID id)
{
    if (getItemCap(id, CAP_ATTACK))
        if (promptPlayer("Swap your current weapon? (y or n)")  == 'y') {
            if (a->weapon != 0) {
                if (putItemInInventory(a, a->weapon) > 0) {
                    addInfo("Put current weapon in inventory.\n");
                } else {
                    addInfo("Inventory is full.\n");
                    return -1;
                }
            }
            addInfo("Equiped new weapon.\n");
            return a->weapon = id;
        }

    if (getItemCap(id, CAP_DEFEND) == 1)
        if (promptPlayer("Swap your current armour? (y or n)") == 'y') {
            if (putItemInInventory(a, a->armour) > 0) {
                addInfo("Put current armour in inventory\n");
            } else {
                addInfo("Inventory is full.\n");
                return -1;
            }
            addInfo("Exquiped new armour.\n");
            return a->armour = id;
        }

    if (promptPlayer("Put item in inventory? (y or n)") == 'y') {
        addInfo("Item added to inventory.\n", a->symbol);
        return putItemInInventory(a, id);
    }
    return 0;
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