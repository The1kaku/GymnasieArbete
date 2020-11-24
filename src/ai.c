#include "ai.h"

#include <stdio.h>
#include <stdlib.h>
#include <curses.h>

#include "actor.h"
#include "commands.h"

int
chaserAi(int T, Actor *a, Actor *p) 
{
    int dy, dx, *args, argc;
    dy = a->y - p->y;
    dx = a->x - p->x;
    argc = 2;
    if (abs(dy) > abs(dx)) {
        args = generateArgs(argc, -dy / abs(dy), 0);
    } else { 
        args = generateArgs(argc, 0, -dx / abs(dx));  
    }
    if (T >= a->speed) {
        return (T >= a->weapon->speed) ? attackmoveCommand(a, args, argc) : moveCommand(a, args, argc);
    } else {
        return (T >= a->weapon->speed) ? attackCommand(a, args, argc) : 0;
    }
}