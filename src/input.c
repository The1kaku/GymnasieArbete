#include <stdio.h>

#include "input.h"
#include "commands.h"

int 
readInput(Actor *p)
{
    int c;

    c = getch();

    switch (c) {
        case KEY_MOVE_UP:
            return moveUpCommand(p, NULL, 0);
        case KEY_MOVE_RIGHT:
            return moveRightCommand(p, NULL, 0);
        case KEY_MOVE_DOWN:
            return moveDownCommand(p, NULL, 0);
        case KEY_MOVE_LEFT:
            return moveLeftCommand(p, NULL, 0);
        case KEY_EXIT_GAME:
            return exitCommand(NULL, NULL, 0);
        default:
            return 0;
    }
}