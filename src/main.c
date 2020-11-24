#include <curses.h>
#include <stdlib.h>

#include "main.h"
#include "display.h"
#include "input.h"
#include "commands.h"
#include "items.h"
#include "ai.h"

static int init(void);

static void loseScreen(void);
static void winScreen(void);

static void updateItems(void);
static void updateMonsters(void);

static void runAi(void);

int 
main(void)
{
    level = readLevelFromFile("levels/level.txt");
    
    Weapon sword = { "Basic Sword", 3, 5, 1, 10, 3 };
    Item isword = { &sword, 5, 5, ITEM_TYPE_WEAPON };

    Weapon fists = { "A pair of fists", 0, 0, 3, -1, 5 };

    Armour naked = { "Unarmoured", 1, 1, 1, -1 };

    groundItems[0] = &isword;   

    player = createActor(1, 1, 20, '@', 1, &fists, &naked, -1);
    undead = createActor(8, 8, 20, 'Z', 1, &sword, &naked, 1);

    monsters[0] = undead;

    if (init < 0)
        return -1;

    do {
        runAi();
        if (player->health < 1) {
            loseScreen();
            break;
        }
        if (player->y == 11 && player->x == 19) {
            winScreen();
            break;
        }
        clearDisplay();
        focusCameraOnActor(player);
        addLevel(level);

        updateItems();
        updateMonsters();

        //Room room = getRoomBordersFromActor(level, player);
        //wprintw(playerWin, "ROOM (%02d,%02d) to (%02d,%02d)", room[0][0], room[0][1], room[1][0], room[1][1]); 
        addActor(player);
        //addCamera();
        refreshDisplay();
    } while (readKeyInput(player) >= 0);

    deleteDisplay();
    endwin();

    return 0;
}

static int
init(void)
{
    if (player == NULL)
        return -1;
    if (level == NULL)
        return -1;
    if (initDisplay < 1)
        return -1;
    return 0;
}

static void
loseScreen(void)
{
    clear();
    printw("You died.");
    mvprintw(15, 5, "Press any key to exit");
    refresh();
    getch();
    clear();
    refresh();
}

static void
winScreen(void)
{
    clear();
    printw("You Won!");
    mvprintw(15, 5, "Press any key to exit");
    refresh();
    getch();
    clear();
    refresh();
}

static void
updateItems(void)
{
    for (int i = 0; i < ITEM_COUNT; i++)
        if (groundItems[i] != NULL) {
            if (groundItems[i]->y == player->y && groundItems[i]->x == player->x && (giveItemToActor(player, groundItems[i]) > 0)) {
                groundItems[i] = NULL;
            } else {
                addItem(groundItems[i]);
            }
        }
}

static void
updateMonsters(void)
{
    for (int i = 0; i < MONSTER_COUNT; i++)
        if (monsters[i] != NULL) {
            if (monsters[i]->health < 1) {
                monsters[i] = NULL;
            } else {
                addActor(monsters[i]);
            }
        }
}

static void
runAi(void)
{
    Room room = getRoomBordersFromActor(level, player);
    for (int i = 0; i < MONSTER_COUNT; i++) {
        if (monsters[i] == NULL) 
            continue;
        if (monsters[i]->y <= room[0][0] && monsters[i]->y >= room[1][0])
            continue;
        if (monsters[i]->x <= room[0][1] && monsters[i]->x >= room[1][1])
            continue;
        if (monsters[i]->health < 1)
            continue;
        
        switch(monsters[i]->aiType) {
            case -1:
                break;
            case 0:
                break;
            case 1:
                chaserAi(monsters[i], player);
                break;
            default:
                break;
        }
    }
}