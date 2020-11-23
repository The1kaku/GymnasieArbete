#include "ai.h"

#include <stdio.h>
#include <stdlib.h>
#include <curses.h>

#include "actor.h"
#include "level.h"
#include "main.h"
#include "commands.h"

static void chaserAi(Actor *a);

void
runAi()
{
    Room r = getRoomBordersFromActor(level, player);
    for (int i = 0; i < MONSTER_COUNT; i++) {
        if (monsters[i] == NULL) 
            continue;
        if (monsters[i]->y <= r[0][0] && monsters[i]->y >= r[1][0])
            continue;
        if (monsters[i]->x <= r[0][1] && monsters[i]->x >= r[1][1])
            continue;
        
        switch(monsters[i]->aiType) {
            case -1:
                break;
            case 0:
                break;
            case 1:
                chaserAi(monsters[i]);
                break;
            default:
                break;
        }
    }
}

static void
chaserAi(Actor *a) 
{
    int dy, dx, *args, argc;
    dy = a->y - player->y;
    dx = a->x - player->x;
    argc = 2;
    if (abs(dy) > abs(dx)) {
        args = generateArgs(argc, -dy / abs(dy), 0);
    } else { 
        args = generateArgs(argc, 0, -dx / abs(dx));  
    }
    attackmoveCommand(a, args, argc);
}