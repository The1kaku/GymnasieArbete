#ifndef DISPLAY_H
#define DISPLAY_H

#include <curses.h>

#include "actor.h"
#include "level.h"

static struct {
    int y;
    int x;
} camera;

enum {
    LEVEL_WIN_HEIGHT = 20,
    LEVEL_WIN_WIDTH = 60,
    PLAYER_WIN_HEIGHT = 5,
    PLAYER_WIN_WIDTH = 40
};

WINDOW* levelWin;
WINDOW* playerWin;

void initDisplay(void);
void clearDisplay(void);
void refreshDisplay(void);
void deleteDisplay(void);

void focusCameraOnActor(const Actor *a);
void focusCameraOnPosition(const int y, const int x);
void setCamera(const int y, const int x);
 
void addActor(const Actor *a);
void addLevel(const Level l);
void addCamera(void);

#endif