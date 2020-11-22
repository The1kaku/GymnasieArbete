#include <curses.h>
#include <stdlib.h>

#include "display.h"
#include "level.h"

void 
initDisplay(void)
{
    initscr();
    refresh();

    noecho();

    levelWin = newwin(LEVEL_WIN_HEIGHT, LEVEL_WIN_WIDTH, 5, 0);
    playerWin = newwin(PLAYER_WIN_HEIGHT, PLAYER_WIN_WIDTH, 0, 0);

    camera.y = camera.x = 0;
}

void 
clearDisplay(void)
{
    wclear(levelWin);
    wclear(playerWin);
}

void 
refreshDisplay(void)
{
    wrefresh(levelWin);
    wrefresh(playerWin);
}

void 
deleteDisplay(void)
{
    delwin(levelWin);
    delwin(playerWin);
}

void 
focusCameraOnActor(const Actor *a)
{
    focusCameraOnPosition(a->y, a->x);
}

void 
focusCameraOnPosition(const int y, const int x)
{
    setCamera(LEVEL_WIN_HEIGHT/2 - y, LEVEL_WIN_WIDTH/2 - x);
}

void 
setCamera(const int y, const int x)
{ 
    camera.y = y; 
    camera.x = x; 
}

void 
addActor(const Actor *a)
{
    int y, x;
    if ((y = a->y + camera.y) >= 0 && y < LEVEL_WIN_HEIGHT && (x = a->x + camera.x) >= 0 && x < LEVEL_WIN_WIDTH)
       mvwaddch(levelWin, y, x, a->symbol);
    wprintw(playerWin, "%c (%d,%d)\n", a->symbol, a->y, a->x);
}

void 
addLevel(const Level l)
{
    for (int i = (camera.y > 0) ? camera.y : 0; i < LEVEL_WIN_HEIGHT; i++) {
        int row, col, n;
        char *str;
        row = i;
        col = ((camera.x > 0) ? camera.x : 0);

        str = l[i-camera.y] + ((camera.x > 0) ? 0 : abs(camera.x));
        
        n = ((camera.x > 0) ? LEVEL_WIN_WIDTH - camera.x : LEVEL_WIN_WIDTH);
        mvwaddnstr(levelWin, row, col, str, n);
    }
}

void
addCamera(void)
{
    mvwprintw(playerWin, 1, 0, "CAMERA: (%d,%d)\n", camera.y, camera.x);
}