#ifndef DISPLAY_H
#define DISPLAY_H

#include <curses.h>

#include "actor.h"
#include "items.h"
#include "level.h"


enum {
    LEVEL_WIN_HEIGHT = 20,
    LEVEL_WIN_WIDTH = 50,
    PLAYER_WIN_HEIGHT = 2,
    PLAYER_WIN_WIDTH = 80,
    TEXT_WIN_HEIGHT = 2,
    TEXT_WIN_WIDTH = 50,
    INVENTORY_WIN_HEIGHT = 22,
    INVENTORY_WIN_WIDTH = 30,
};

WINDOW *playerWin;
WINDOW *levelWin;
WINDOW *textWin;
WINDOW *inventoryWin;

void initDisplay(void);
void clearDisplay(void);
void refreshDisplay(void);
void deleteDisplay(void);

void focusCameraOnActor(const Actor *a);
void focusCameraOnPosition(const int y, const int x);
void setCamera(const int y, const int x);
 
void addActor(const Actor *a);
void addItem(const Item *i);
void addLevel(const Level l);
void addCamera(void);

char promptPlayer(const char *s);

#endif