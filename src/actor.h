#ifndef ACTOR_H
#define ACTOR_H

typedef struct Actor {
    int y;
    int x;
    int health;
    char symbol;
    int speed;
} Actor;

void moveActor(Actor *a, const int dy, const int dx);

Actor *createActor(const int y, const int x, const int health, const char symbol, const int speed);

#endif