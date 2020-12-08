#ifndef ITEMS_H
#define ITEMS_H

#define ITEM_SYMBOL '^'

enum CAPABILITIES {
    CAP_NAME = 0,
    CAP_ATTACK = 1,
    CAP_DEFEND = 2,
};

enum ATTACK_CAPABILITIES {
    ATK_IMPACT = 0,
    ATK_PIERCE = 1,
    ATK_SLASH = 2,
    ATK_SPEED = 3,
};

enum DEFEND_CAPABILITIES {
    DEF_IMPACT = 0,
    DEF_PIERCE = 1,
    DEF_SLASH = 2,
    DEF_THORNS = 3,
};

typedef unsigned short ItemID;

typedef struct GroundItem {
    ItemID id;
    int y;
    int x;
} GroundItem;

void initItems(void);

const char *getItemName(const ItemID id);
const int getItemCap(const ItemID, const unsigned CAP_ID);
const int *getItemAtk(const ItemID id);
const int *getItemDef(const ItemID id);

#endif