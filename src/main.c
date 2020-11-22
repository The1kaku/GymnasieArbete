#include <curses.h>
#include <stdlib.h>

#include "main.h"
#include "display.h"
#include "input.h"
#include "commands.h"
#include "items.h"

#define ITEM_COUNT 10

int 
main(void)
{
    player = createActor(1, 1, 20, '@', 1, NULL, NULL);
    level = readLevelFromFile("levels/level.txt");

    Weapon sword = { "Basic Sword", 3, 5, 1, 10 };
    Item isword = { &sword, 5, 5, ITEM_TYPE_WEAPON };
    Item **groundItems = (Item **) calloc(ITEM_COUNT, sizeof(Item *)); 
    groundItems[0] = &isword;

    if (level == NULL) {
        return -1;
    }

    initDisplay();
    endwin();

    setCamera(0, 0);

    do {
        clearDisplay();
        focusCameraOnActor(player);
        addLevel(level);
        for (int i = 0; i < ITEM_COUNT; i++)
            if (groundItems[i] != NULL) {
                if (groundItems[i]->y == player->y && groundItems[i]->x == player->x && (giveItemToActor(player, groundItems[i]) > 0)) {
                    groundItems[i] = NULL;
                } else {
                    addItem(groundItems[i]);
                }
            }
        addActor(player);
        //addCamera();
        refreshDisplay();
    } while (readKeyInput(player) >= 0);

    deleteDisplay();
    endwin();

    return 0;
}