#ifndef INFO_H
#define INFO_H

#include <curses.h>

#define INFO_WIN_HEIGHT 9
#define INFO_WIN_WIDTH 30

WINDOW *infoWin;

void addInfo(const char *format, ...);

#endif