#include <stdlib.h>
#include <stdarg.h>

#include "commands.h"
#include "display.h"
#include "main.h"

static int *
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
    if (level[a->y+dy][a->x+dx] != '.')
        return 0;
    moveActor(a, args[0], args[1]);
    return a->speed;
}

int attackmoveRightCommand(Actor *a, int *args, int argc)
{
    free(args);
    argc = 2;
    args = generateArgs(argc, 0, 1);
    return attackmoveCommand(a, args, argc);
}

int attackmoveLeftCommand(Actor *a, int *args, int argc)
{
    free(args);
    argc = 2;
    args = generateArgs(argc, 0, -1);
    return attackmoveCommand(a, args, argc);
}

int attackmoveDownCommand(Actor *a, int *args, int argc)
{
    free(args);
    argc = 2;
    args = generateArgs(argc, 1, 0);
    return attackmoveCommand(a, args, argc);
}

int attackmoveUpCommand(Actor *a, int *args, int argc)
{
    free(args);
    argc = 2;
    args = generateArgs(argc, -1, 0);
    return attackmoveCommand(a, args, argc);
}

int attackmoveCommand(Actor *a, int *args, int argc)
{
    if (a->aiType == -1) {
        for (int i = 0; i < MONSTER_COUNT; i++) {
            if (monsters[i] == NULL)
                return moveCommand(a, args, argc);
            if (monsters[i]->y == a->y + args[0] && monsters[i]->x == a->x + args[1]) {
                    attackActor(a, monsters[i]);
                    return a->weapon->speed;
            } else {
                return moveCommand(a, args, argc);
            }
        }
    }

    if (a->y + args[0] == player->y && a->x + args[1] == player->x) {
        attackActor(a, player);
        return a->weapon->speed;
    } 
    return moveCommand(a, args, argc);
}

int 
exitCommand(Actor *a, int *args, int argc)
{
    return -1;
}