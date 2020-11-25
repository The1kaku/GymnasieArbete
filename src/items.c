#include "items.h"

#include <stdio.h>

#define DATABASE_SIZE 500
#define CAP_SIZE 3

typedef struct Item {
    int capabilities[CAP_SIZE];
    char *name;
    int atk[4]; 
    int def[4];
} Item;

static Item *database[DATABASE_SIZE];
static ItemID databaseIndex = 0;

int 
allocItem(Item *i)
{
    if (databaseIndex < DATABASE_SIZE) {
        database[databaseIndex] = i;
        return databaseIndex++;
    } else {
        return -1;
    }
}

const int
getItemCap(const ItemID id, const unsigned CAP_ID)
{
    if (id >= databaseIndex)
        return 0;
    if (CAP_ID >= CAP_SIZE)
        return 0;
    return database[id]->capabilities[CAP_ID];
}

const char *
getItemName(const ItemID id)
{
    return getItemCap(id, CAP_NAME) ? database[id]->name : NULL;
}

const int *
getItemAtk(const ItemID id)
{
    return getItemCap(id, CAP_ATTACK) ? database[id]->atk : NULL;
}

const int *
getItemDef(const ItemID id)
{
    return getItemCap(id, CAP_DEFEND) ? database[id]->def : NULL;
}