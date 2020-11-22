#ifndef INPUT_H
#define INPUT_H

#include <curses.h>

#include "actor.h"

#define COMMAND_SIZE 80

enum {
    KEY_MOVE_UP = 'w',
    KEY_MOVE_RIGHT = 's',
    KEY_MOVE_DOWN = 'r',
    KEY_MOVE_LEFT = 'a',
    KEY_EXIT_GAME = '1',
    KEY_TEXT_MODE = '/',
};

int readKeyInput(Actor *p);
int readTextInput(Actor *p);

#endif