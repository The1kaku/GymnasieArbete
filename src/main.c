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

    player = createActor( 1, 1, 20, '@', 1, 0, 0, 0, 10);
    undead = createActor( 8, 8, 20, 'Z', 3, 0, 0, 1, 5);
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

        addInventory(player->inventory, player->inventorySize);
        addActor(player);
        addActorStats(player);

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
            if (groundItems[i]->y == player->y && groundItems[i]->x == player->x && (giveItemToActor(player, groundItems[i]->id) > 0)) {
                groundItems[i] = NULL;
            } else {
                addItem(groundItems[i]);
            }
        }
    return;
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
        lT = T + monsters[i]->savedTime;
        monsters[i]->savedTime = 0;
        for (; lT > 0 && dT != 0; lT += -dT) {
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
        monsters[i]->savedTime += lT;
    }
}