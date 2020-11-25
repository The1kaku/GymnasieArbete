#include <curses.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>

#include "display.h"
#include "level.h"

static struct {
    int y;
    int x;
} camera __attribute__((unused));

void 
initDisplay(void)
{
    initscr();
    refresh();

    noecho();
    cbreak();

    playerWin = newwin(PLAYER_WIN_HEIGHT, PLAYER_WIN_WIDTH, 0, 0);
    levelWin = newwin(LEVEL_WIN_HEIGHT, LEVEL_WIN_WIDTH, PLAYER_WIN_HEIGHT, 0);
    textWin = newwin(TEXT_WIN_HEIGHT, TEXT_WIN_WIDTH, PLAYER_WIN_HEIGHT+LEVEL_WIN_HEIGHT, 0);
    inventoryWin = newwin(INVENTORY_WIN_HEIGHT, INVENTORY_WIN_WIDTH, PLAYER_WIN_HEIGHT, LEVEL_WIN_WIDTH);
    infoWin = newwin(INFO_WIN_HEIGHT, INFO_WIN_WIDTH, PLAYER_WIN_HEIGHT+INVENTORY_WIN_HEIGHT, LEVEL_WIN_WIDTH);

    camera.y = camera.x = 0;
}

void 
clearDisplay(void)
{
    wclear(playerWin);
    wclear(levelWin);
    wclear(textWin);
    wclear(inventoryWin);
}

void 
refreshDisplay(void)
{
    wrefresh(levelWin);
    wrefresh(playerWin);
    wrefresh(textWin);
    wrefresh(inventoryWin);
}

void 
deleteDisplay(void)
{
    delwin(levelWin);
    delwin(playerWin);
    delwin(textWin);
    delwin(inventoryWin);
    delwin(infoWin);
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
}

void
addActorStats(const Actor *a)
{
    wprintw(playerWin, "%c (%02d,%02d) HP: %d W: %s A: %s\n", a->symbol, a->y, a->x, a->health, getItemName(a->weapon), getItemName(a->armour));
}

void
addItem(const GroundItem *i)
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
addInventory(const ItemID *inv, const unsigned invSize)
{
    wclear(inventoryWin);
    waddstr(inventoryWin, " INVENTORY:\n");
    for (int i = 0; i < invSize && i < INVENTORY_WIN_HEIGHT-1; i++) {
        if (getItemCap(inv[i], CAP_NAME)) {
            wprintw(inventoryWin, "%x. %s\n", i+1, getItemName(inv[i]));
        } else {
            wprintw(inventoryWin, "%x. - \n", i+1);
        }
    }
    wrefresh(inventoryWin);
}

void
addCamera(void)
{
    mvwprintw(playerWin, 1, 0, "CAMERA: (%d,%d)\n", camera.y, camera.x);
}

char
promptPlayer(const char *format, ...)
{
    char str[TEXT_WIN_WIDTH];

    va_list local_argv;
    va_start(local_argv, format);
    vsprintf(str, format, local_argv);
    va_end(local_argv);

    wclear(textWin);
    wprintw(textWin, "%s", str);
    wrefresh(textWin);

    noecho();
    cbreak();
    return getch();
}

char
invPromptPlayer(const Actor *a, const char *format, ...)
{
    int in = promptPlayer("Which item in your inventory do you want to equip? (1 to %x)", a->inventorySize);
    int c = in;
    if (isdigit(c)) {
        c -= '0' + 1;
    } else if (c >= 'a' && c <= 'f') {
        c += 8 - 'a';
    } else {
        addInfo("Unable to access '%c' in inventory.\n", in);
        return 0;
    }
    if (c <= a->inventorySize) {
        return c;
    }
    addInfo("Value '%c' out of range (1 to %x).\n", in, a->inventorySize);
    return 0;
}