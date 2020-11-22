#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "input.h"
#include "commands.h"
#include "display.h"

int 
readKeyInput(Actor *p)
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
        case KEY_TEXT_MODE:
            return readTextInput(p);
        case KEY_EXIT_GAME:
            return exitCommand(NULL, NULL, 0);
        default:
            return 0;
    }
}

int
readTextInput(Actor *p)
{
    char *sp, str[COMMAND_SIZE]; 
    int size, res, T;
    echo();
    nocbreak();
    mvwaddch(textWin, 0, 0, '/');
    
    wgetstr(textWin, str);

    T = 1;
    if (isdigit(str[0])) {
        sp = (char *) malloc(COMMAND_SIZE * sizeof(char));
        sscanf(str, "%d%80[^\n]s", &T, sp);
    } else {
        sp = str;
    }

    size = sizeof(commands) / sizeof(commands[0]);
    res = 0;
    for (int i = 0; i < size; i++) {
        if (strcmp(sp, commands[i].str) == 0 && commands[i].playerCanUse > 0)
            for (int j = 0; j < T && j < 100; j++)
                res = (*(commands[i].fun))(p, NULL, 0);
    }

    wmove(levelWin, 0, 0);
    noecho();
    cbreak();

    return res;
}