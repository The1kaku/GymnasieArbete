#ifndef DISPLAY_H
#define DISPLAY_H

#include <curses.h>

#include "actor.h"
#include "items.h"
#include "level.h"
#include "info.h"


enum {
    LEVEL_WIN_HEIGHT = 22,
    LEVEL_WIN_WIDTH = 50,
    PLAYER_WIN_HEIGHT = 1,
    PLAYER_WIN_WIDTH = 80,
    TEXT_WIN_HEIGHT = 1,
    TEXT_WIN_WIDTH = 80,
    INVENTORY_WIN_HEIGHT = 13,
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
void addActorStats(const Actor *a);
void addItem(const GroundItem *i);
void addLevel(const Level l);
void addCamera(void);
void addInventory(const ItemID *inv, const unsigned invSize);

char promptPlayer(const char *format, ...);
char invPromptPlayer(const Actor *a, const char *format, ...);

#endif