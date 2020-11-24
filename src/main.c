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

static void runAi(int T);

int 
main(void)
{
    int T = 0;
    level = readLevelFromFile("levels/level.txt");
    
    Weapon sword = { "Basic Sword", 3, 5, 1, 10, 3 };
    Item isword1 = { &sword, 5, 5, ITEM_TYPE_WEAPON };
    Item isword2 = { &sword, 10, 5, ITEM_TYPE_WEAPON };
    Item isword3 = { &sword, 5, 13, ITEM_TYPE_WEAPON };

    Weapon fists = { "A pair of fists", 0, 0, 3, -1, 5 };

    Armour naked = { "Unarmoured", 1, 1, 1, -1 };

    groundItems[0] = &isword1;
    groundItems[1] = &isword2; 
    groundItems[2] = &isword3;    

    player = createActor(1, 1, 20, '@', 1, &fists, &naked, -1);
    undead = createActor(8, 8, 20, 'Z', 1, &sword, &naked, 1);

    monsters[0] = undead;

    if (init() < 0)
        return -1;

    do {
        runAi(T);
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
    } while ((T = readKeyInput(player)) >= 0);

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
    initDisplay();
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
runAi(int T)
{
    int dT, lT;
    if (T == 0)
        return;
    Room room = getRoomBordersFromActor(level, player);
    for (int i = 0; i < MONSTER_COUNT; i++) {
        if (monsters[i] == NULL) 
            continue;
        if (monsters[i]->y <= room[0][0] && monsters[i]->y >= room[1][0])
            continue;
        if (monsters[i]->x <= room[0][1] && monsters[i]->x >= room[1][1])
            continue;

        dT = 1;
        for (lT = T; lT > 0 && dT != 0; lT += -dT) {
            switch(monsters[i]->aiType) {
            case -1:
                break;
            case 0:
                break;
            case 1:
                dT = chaserAi(lT, monsters[i], player);
                break;
            default:
                break;
            }
        }
    }
}