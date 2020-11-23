#include <curses.h>
#include <stdlib.h>

#include "main.h"
#include "display.h"
#include "input.h"
#include "commands.h"
#include "items.h"
#include "ai.h"

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
    undead = createActor(8, 8, 20, 'U', 1, &sword, &naked, 1);

    monsters[0] = undead;

    if (level == NULL) {
        return -1;
    }

    initDisplay();

    do {
        runAi();
        if (player->health < 1)
            break;
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
        for (int i = 0; i < MONSTER_COUNT; i++)
            if (monsters[i] != NULL) {
                if (monsters[i]->health < 1) {
                    monsters[i] = NULL;
                } else {
                    addActor(monsters[i]);
                }
            }
        Room room = getRoomBordersFromActor(level, player);
        wprintw(playerWin, "ROOM (%02d,%02d) to (%02d,%02d)", room[0][0], room[0][1], room[1][0], room[1][1]); 
        addActor(player);
        //addCamera();
        refreshDisplay();
    } while (readKeyInput(player) >= 0);

    deleteDisplay();
    endwin();

    return 0;
}