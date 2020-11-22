#ifndef INPUT_H
#define INPUT_H

#include <curses.h>

#include "actor.h"

enum {
    KEY_MOVE_UP = 'w',
    KEY_MOVE_RIGHT = 's',
    KEY_MOVE_DOWN = 'r',
    KEY_MOVE_LEFT = 'a',
    KEY_EXIT_GAME = '1'
};

int readInput(Actor *p);

#endif