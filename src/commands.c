#include <stdlib.h>

#include "commands.h"
#include "display.h"
#include "main.h"

int 
moveRightCommand(Actor *a, int *args, int argc)
{
    free(args);
    argc = 2;
    args = malloc(argc * sizeof(int));
    args[0] =  0;
    args[1] =  1;
    return moveCommand(a, args, argc);
}

int 
moveLeftCommand(Actor *a, int *args, int argc)
{
    free(args);
    argc = 2;
    args = malloc(argc * sizeof(int));
    args[0] =  0;
    args[1] = -1;
    return moveCommand(a, args, argc);
}

int 
moveDownCommand(Actor *a, int *args, int argc)
{
    free(args);
    argc = 2;
    args = malloc(argc * sizeof(int));
    args[0] =  1;
    args[1] =  0;
    return moveCommand(a, args, argc);
}

int 
moveUpCommand(Actor *a, int *args, int argc)
{
    free(args);
    argc = 2;
    args = malloc(argc * sizeof(int));
    args[0] = -1;
    args[1] =  0;
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

int 
exitCommand(Actor *a, int *args, int argc)
{
    return -1;
}