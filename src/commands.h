#ifndef COMMANDS_H
#define COMMANDS_H

#include "actor.h"

typedef struct Command {
    char *str;
    int playerCanUse;
    int (*fun)(Actor *a, int *args, int argc);
} Command;

// Movement
int moveRightCommand(Actor *a, int *args, int argc);
int moveLeftCommand(Actor *a, int *args, int argc);
int moveDownCommand(Actor *a, int *args, int argc);
int moveUpCommand(Actor *a, int *args, int argc);
int moveCommand(Actor *a, int *args, int argc);
// AttackMovement
int attackmoveRightCommand(Actor *a, int *args, int argc);
int attackmoveLeftCommand(Actor *a, int *args, int argc);
int attackmoveDownCommand(Actor *a, int *args, int argc);
int attackmoveUpCommand(Actor *a, int *args, int argc);
int attackmoveCommand(Actor *a, int *args, int argc);

// Exiting
int exitCommand(Actor *a, int *args, int argc);

static Command __attribute__((unused)) commands[] = {
    { "move right", 1, &moveRightCommand },
    { "move left", 1, &moveLeftCommand },
    { "move down", 1, &moveDownCommand },
    { "move up", 1, &moveUpCommand },
    { "attackmove right", 1, &attackmoveRightCommand },
    { "attackmove left", 1, &attackmoveLeftCommand },
    { "attackmove down", 1, &attackmoveDownCommand },
    { "attackmove up", 1, &attackmoveUpCommand },
    { "exit", 1, &exitCommand },
};


#endif