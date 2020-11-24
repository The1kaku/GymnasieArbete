#include "ai.h"

#include <stdio.h>
#include <stdlib.h>
#include <curses.h>

#include "actor.h"
#include "commands.h"

void
chaserAi(Actor *a, Actor *p) 
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
    attackmoveCommand(a, args, argc);
}