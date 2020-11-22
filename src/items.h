#ifndef ITEMS_H
#define ITEMS_H

enum {
    ITEM_SYMBOL = '^',
    ITEM_TYPE_WEAPON = 1,
    ITEM_TYPE_ARMOUR = 2,
};

typedef struct Item {
    void *item;
    int y;
    int x;
    int type;
} Item;

typedef struct Weapon {
    char *name;
    int pierce;
    int slash;
    int impact;
    int durability;
} Weapon;

typedef struct Armour {
    char *name;
    int pierce;
    int slash;
    int impact;
    int durability;
} Armour;

#endif