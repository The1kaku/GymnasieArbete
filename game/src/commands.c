#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>

#include "commands.h"
#include "display.h"
#include "main.h"

int *
generateArgs(int argc, ...)
{
    va_list args_list;
    int *args;

    va_start(args_list, argc);

    args = (int *) malloc(argc * sizeof(int));
    for (int i = 0; i < argc; i++) {
        args[i] = va_arg(args_list, int);
    }

    va_end(args_list);

    return args;
}

int 
moveRightCommand(Actor *a, int *args, int argc)
{
    free(args);
    argc = 2;
    args = generateArgs(argc, 0, 1);
    return moveCommand(a, args, argc);
}

int 
moveLeftCommand(Actor *a, int *args, int argc)
{
    free(args);
    argc = 2;
    args = generateArgs(argc, 0, -1);
    return moveCommand(a, args, argc);
}

int 
moveDownCommand(Actor *a, int *args, int argc)
{
    free(args);
    argc = 2;
    args = generateArgs(argc, 1, 0);
    return moveCommand(a, args, argc);
}

int 
moveUpCommand(Actor *a, int *args, int argc)
{
    free(args);
    argc = 2;
    args = generateArgs(argc, -1, 0);
    return moveCommand(a, args, argc);
}

int 
moveCommand(Actor *a, int *args, int argc)
{
    int dy, dx;
    dy = args[0];
    dx = args[1];
    if (a->y+dy < 0 || a->y+dy >= LEVEL_HEIGHT || a->x+dx < 0 || a->x+dx >= LEVEL_WIDTH)
        return 0;
    if (level[a->y+dy][a->x+dx] == '#')
        return 0;
    if (level[a->y+dy][a->x+dx] == ' ') {
        a->health = 0;
        return 0;
    }
    if (a->y+dy == player->y && a->x+dx == player->x)
        return 0;
    for (int i = 0; i < MONSTER_COUNT; i++) {
        if (monsters[i] == NULL)
            continue;
        if (a->y+dy == monsters[i]->y && a->x+dx == monsters[i]->x)
            return 0;
    }

    moveActor(a, args[0], args[1]);
    return a->speed;
}

int 
attackmoveRightCommand(Actor *a, int *args, int argc)
{
    free(args);
    argc = 2;
    args = generateArgs(argc, 0, 1);
    return attackmoveCommand(a, args, argc);
}

int 
attackmoveLeftCommand(Actor *a, int *args, int argc)
{
    free(args);
    argc = 2;
    args = generateArgs(argc, 0, -1);
    return attackmoveCommand(a, args, argc);
}

int 
attackmoveDownCommand(Actor *a, int *args, int argc)
{
    free(args);
    argc = 2;
    args = generateArgs(argc, 1, 0);
    return attackmoveCommand(a, args, argc);
}

int 
attackmoveUpCommand(Actor *a, int *args, int argc)
{
    free(args);
    argc = 2;
    args = generateArgs(argc, -1, 0);
    return attackmoveCommand(a, args, argc);
}

int 
attackmoveCommand(Actor *a, int *args, int argc)
{
    int attackRes;
    return (attackRes = attackCommand(a, args, argc)) ? attackRes : moveCommand(a, args, argc);
}

int
attackCommand(Actor *a, int *args, int argc)
{
    if (a->aiType == -1) {
        for (int i = 0; i < MONSTER_COUNT; i++) {
            if (monsters[i] == NULL)
                continue;
            if (monsters[i]->y == a->y + args[0] && monsters[i]->x == a->x + args[1]) {
                return attackActor(a, monsters[i]);
            }
        }
        return 0;
    }

    if (a->y + args[0] == player->y && a->x + args[1] == player->x) {
        return attackActor(a, player);
    }
    return 0;
}

int 
dropWeaponCommand(Actor *a, int *args, int argc)
{
    putItemOnGround(a->weapon, a);
    a->weapon = 0;
    addInfo("Dropped weapon.\n");
    return 0;
}

int 
dropArmourCommand(Actor *a, int *args, int argc)
{
    putItemOnGround(a->armour, a);
    a->armour = 0;
    addInfo("Dropped armour.\n");
    return 0;
}

int 
dropInventoryItemCommand(Actor *a, int *args, int argc)
{
    int c;
    if ((c = invPromptPlayer(a, "Which item in your inventory do you want to drop? (1 to %x)", a->inventorySize)) >= 0) {
        if (a->inventory[c] == 0) {
            addInfo("Dropped nothing.\n");
        } else {
            addInfo("Dropped item in slot %x.\n", c+1);
            putItemOnGround(a->inventory[c], a);
            a->inventory[c] = 0;
        }
    }
    return 0;
}

int
equipCommand(Actor *a, int *args, int argc)
{
    int saved, c;
    if ((c = invPromptPlayer(a, "Which item in your inventory do you want to equip? (1 to %x)", a->inventorySize)) >= 0) {
        if (a->inventory[c] == 0) {
            addInfo("Could not equip, nothing.\n");
        } else {
            saved = a->inventory[c];
            a->inventory[c] = 0;
            giveItemToActor(a, saved);
        }
    }
    return 0;
}

int
waitCommand(Actor *a, int *args, int argc)
{
    return 1;
}

int 
exitCommand(Actor *a, int *args, int argc)
{
    return -1;
}