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
    cbreak();

    playerWin = newwin(PLAYER_WIN_HEIGHT, PLAYER_WIN_WIDTH, 0, 0);
    levelWin = newwin(LEVEL_WIN_HEIGHT, LEVEL_WIN_WIDTH, 5, 0);
    textWin = newwin(TEXT_WIN_HEIGHT, TEXT_WIN_WIDTH, 26, 0);

    camera.y = camera.x = 0;
}

void 
clearDisplay(void)
{
    wclear(playerWin);
    wclear(levelWin);
    wclear(textWin);
}

void 
refreshDisplay(void)
{
    wrefresh(levelWin);
    wrefresh(playerWin);
    wrefresh(textWin);
}

void 
deleteDisplay(void)
{
    delwin(levelWin);
    delwin(playerWin);
    delwin(textWin);
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
    wprintw(playerWin, "%c (%02d,%02d) HP: %d W: %s A: %s\n", a->symbol, a->y, a->x, a->health, (a->weapon == NULL) ? "none" : a->weapon->name, (a->armour == NULL) ? "none" : a->armour->name);
}

void
addItem(const Item *i)
{
    int y, x;
    if ((y = i->y + camera.y) >= 0 && y < LEVEL_WIN_HEIGHT && (x = i->x + camera.x) >= 0 && x < LEVEL_WIN_WIDTH)
        mvwaddch(levelWin, y, x, ITEM_SYMBOL);
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